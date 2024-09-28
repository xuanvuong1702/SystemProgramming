# Thinking about scheduling.

[CPU Scheduling](https://en.wikipedia.org/wiki/Scheduling_(computing)) is the problem of efficiently selecting which process to run on a system's CPU cores. In a busy system there will be more ready-to-run processes than there are CPU cores, so the system kernel must evaluate which processes should be scheduled to run on the CPU and which processes should be placed in a ready queue to be executed later.

The additional complexity of multi-threaded and multiple CPU cores are considered a distraction to this initial exposition so are ignored here.

Another gotcha for non-native speakers is the dual meanings of "Time": The word "Time" can be used in both clock and elapsed duration context. For example "The arrival time of the first process was 9:00am." and, "The running time of the algorithm is 3 seconds".

## How is scheduling measured and which scheduler is best?

Scheduling affects the performance of the system, specifically the *latency* and *throughput* of the system. The throughput might be measured by a system value, for example the I/O throughput - the number of bytes written per second, or number of small processes that can complete per unit time, or using a higher level of abstraction for example number of customer records processed per minute. The latency might be measured by the response time (elapse time before a process can start to send a response) or wait time or turnaround time (the elapsed time to complete a task). Different schedulers offer different optimization trade-offs that may or may not be appropriate to desired use - there is no optimal scheduler for all possible environments and goals. For example 'shortest-job-first' will minimize total wait time across all jobs but in interactive (UI) environments it would be preferable to minimize response time (at the expense of some throughput), while FCFS seems intuitively fair and easy to implement but suffers from the Convoy Effect.

## What is arrival time?

The time at which a process first arrives at the ready queue, and is ready to start executing. If a CPU is idle, the arrival time would also be the starting time of execution.

## What is preemption?

Without preemption processes will run until they are unable to utilize the CPU any further. For example the following conditions would remove a process from the CPU and the CPU would be available to be scheduled for other processes: The process terminates due to a signal, is blocked waiting for concurrency primitive, or exits normally.
Thus once a process is scheduled it will continue even if another process with a high priority (e.g. shorter job) appears on the ready queue.

With preemption, the existing processes may be removed immediately if a more preferable process is added to the ready queue. For example, suppose at t=0 with a Shortest Job First scheduler there are two processes (P1 P2) with 10 and 20 ms execution times. P1 is scheduled. P1 immediately creates a new process P3, with execution time of 5 ms, which is added to the ready queue. Without preemption, P3 will run 10ms later (after P1 has completed). With preemption, P1 will be immediately evicted from the CPU and instead placed back in the ready queue, and P3 will be executed instead by the CPU.

## Which schedulers suffer from starvation?
Any scheduler that uses a form of prioritization can result in starvation because earlier processes may never be scheduled to run (assigned a CPU). For example with SJF, longer jobs may never be scheduled if the system continues to have many short jobs to schedule. It all depends on the [type of scheduler](https://en.wikipedia.org/wiki/Scheduling_(computing)#Types_of_operating_system_schedulers).

## Why might a process (or thread) be placed on the ready queue?

A process is placed on the ready queue when it is able to use a CPU. Some examples include:
* A process was blocked waiting for a `read` from storage or socket to complete and data is now available.
* A new process has been created and is ready to start.
* A process thread was blocked on a synchronization primitive (condition variable, semaphore, mutex lock) but is now able to continue.
* A process is blocked waiting for a system call to complete but a signal has been delivered and the signal handler needs to run.

Similar examples can be generated when considering threads.

# Measures of Efficiency

`start_time` is the wall-clock start time of the process (CPU starts working on it)
`end_time` is the end wall-clock of the process (CPU finishes the process)
`run_time` is the total amount of CPU time required
`arrival_time` is the time the process enters the scheduler (CPU may not start working on it)

## What is 'turnaround time'?

The total time from when you the process arrives to when it ends.

`turnaround_time = end_time - arrival_time`

## What is 'response time'?

The total latency (time) that it takes from when the process arrives to when the CPU actually starts working on it.

`response_time = start_time - arrival_time`

## What is 'wait time'?

Wait time is the *total* wait time i.e. the total time that a process is on the ready queue. A common mistake is to believe it is only the initial waiting time in the ready queue.

If a CPU intensive process with no I/O takes 7 minutes of CPU time to complete but required 9 minutes of wall-clock time to complete we can conclude that it was placed on the ready-queue for 2 minutes. For those 2 minutes the process was ready to run but had no CPU assigned. It does not matter when the job was waiting, the wait time is 2 minutes.

`wait_time  = (end_time - arrival_time) - run_time`


## What is the Convoy Effect?

"The Convoy Effect is where I/O intensive processes are continually backed up, waiting for CPU-intensive processes that hog the CPU. This results in poor I/O performance, even for processes that have tiny CPU needs."

Suppose the CPU is currently assigned to a CPU intensive task and there is a set of I/O intensive processes that are in the ready queue. These processes require just a tiny amount of CPU time but they are unable to proceed because they are waiting for the CPU-intensive task to be removed from the processor. These processes are starved until the the CPU bound process releases the CPU. But the CPU will rarely be released (for example in the case of a FCFS scheduler, we must wait until the processes is blocked due to an I/O request). The I/O intensive processes can now finally satisfy their CPU needs, which they can do quickly because their CPU needs are small and the CPU is assigned back to the CPU-intensive process again. Thus the I/O performance of the whole system suffers through an indirect effect of starvation of CPU needs of all processes.

This effect is usually discussed in the context of FCFS scheduler, however a round robin scheduler can also exhibit the Convoy effect for long time-quanta.


## Linux Scheduling
As of February 2016, Linux by default uses the *Completely Fair Scheduler* for CPU scheduling and the Budget Fair Scheduling "BFQ" for I/O scheduling. Appropriate scheduling can have a significant impact on throughput and latency. Latency is particularly important for interactive and soft-real time applications such as audio and video streaming. See the discussion and comparative benchmarks [here](https://lkml.org/lkml/2014/5/27/314) for more information.

Here is how the CFS schedules

* The CPU creates a Red-Black tree with the processes virtual runtime (runtime / nice\_value) and sleeper fairness (if the process is waiting on something give it the CPU when it is done waiting).
* (Nice values are the kernel's way of giving priority to certain processes, the lower nice value the higher priority)
* The kernel chooses the lowest one based on this metric and schedules that process to run next, taking it off the queue. Since the red-black tree is self balancing this operation is guaranteed $O(log(n))$ (selecting the min process is the same runtime)

Although it is called the Fair Scheduler there are a fair bit of problems.

* Groups of processes that are scheduled may have imbalanced loads so the scheduler roughly distributes the load. When another CPU gets free it can only look at the average load of a group schedule not the individual cores. So the free CPU may not take the work from a CPU that is burning so long as the average is fine.
* If a group of processes is running on non-adjacent cores then there is a bug. If the two cores are more than a hop away, the load balancing algorithm won't even consider that core. Meaning if a CPU is free and a CPU that is doing more work is more than a hop away, it won't take the work (may have been patched).
* After a thread goes to sleep on a subset of cores, when it wakes up it can only be scheduled on the cores that it was sleeping on. If those cores are now busy, the thread will have to wait on them, wasting opportunities to use other idle cores.
* To read more on the problems of the Fair Scheduler, read [here](https://blog.acolyer.org/2016/04/26/the-linux-scheduler-a-decade-of-wasted-cores).
