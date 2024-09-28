# Candidate Solutions

## What is the Critical Section Problem?

As already discussed in [[Synchronization, Part 3: Working with Mutexes And Semaphores]], there are critical parts of our code that can only be executed by one thread at a time. We describe this requirement as 'mutual exclusion'; only one thread (or process) may have access to the shared resource.

In multi-threaded programs we can wrap a critical section with mutex lock and unlock calls:

```C
pthread_mutex_lock() - one thread allowed at a time! (others will have to wait here)
... Do Critical Section stuff here!
pthread_mutex_unlock() - let other waiting threads continue
```
How would we implement these lock and unlock calls? Can we create an algorithm that assures mutual exclusion? An incorrect implementation is shown below, 
```C
pthread_mutex_lock(p_mutex_t *m)     { while(m->lock) {}; m->lock = 1;}
pthread_mutex_unlock(p_mutex_t *m)   { m->lock = 0; }
```


At first glance, the code appears to work; if one thread attempts to locks the mutex, a later thread must wait until the lock is cleared. However this implementation _does not satisfy Mutual Exclusion_. Let's take a close look at this 'implementation' from the point of view of two threads running around the same time. In the table below times runs from top to bottom-

Time | Thread 1 | Thread 2
-----|----------|---------
1 | `while(lock) {}`
2 | | `while(lock) {} ` | 
3 | lock = 1 | lock = 1 |
Ooops! There is a race condition. In the unfortunate case both threads checked the lock and read a false value and so we are unable to continue. 

## Candidate solutions to the critical section problem.
To simplify the discussion we consider only two threads. Note these arguments work for threads and processes and the classic CS literature discusses these problem in terms of two processes that need exclusive access (i.e. mutual exclusion) to a critical section or shared resource.

Raising a flag represents a thread/process's intention to enter the critical section.

Remember that the pseudo-code outlined below is part of a larger program; the thread or process will typically need to enter the critical section many times during the lifetime of the process. So imagine each example as wrapped inside a loop where for a random amount of time the thread or process is working on something else.

Is there anything wrong with candidate solution described below?
```
// Candidate #1
wait until your flag is lowered
raise my flag
// Do Critical Section stuff
lower my flag 
```

Answer: Candidate solution #1 also suffers a race condition i.e. it does not satisfy Mutual Exclusion because both threads/processes could read each other's flag value (=lowered) and continue. 

This suggests we should raise the flag _before_ checking the other thread's flag - which is candidate solution #2 below.

```
// Candidate #2
raise my flag
wait until your flag is lowered
// Do Critical Section stuff
lower my flag 
```

Candidate #2 satisfies mutual exclusion - it is impossible for two threads to be inside the critical section at the same time. However this code suffers from deadlock! Suppose two threads wish to enter the critical section at the same time:

Time | Thread 1 | Thread 2
-----|----------|---------
1 | raise flag
2 | | raise flag
3 | wait ... | wait ...
Ooops both threads / processes are now waiting for the other one to lower their flags. Neither one will enter the critical section as both are now stuck forever!

This suggests we should use a turn-based variable to try to resolve who should proceed. 

## Turn-based solutions
The following candidate solution #3 uses a turn-based variable to politely allow one thread and then the other to continue

```
// Candidate #3
wait until my turn is myid
// Do Critical Section stuff
turn = yourid
```
Candidate #3 satisfies mutual exclusion (each thread or process gets exclusive access to the Critical Section), however both threads/processes must take a strict turn-based approach to using the critical section; i.e. they are forced into an alternating critical section access pattern. For example, if thread 1 wishes to read a hashtable every millisecond but another thread writes to a hashtable every second, then the reading thread would have to wait another 999ms before being able to read from the hashtable again. This 'solution' is not effective, because our threads should be able to make progress and enter the critical section if no other thread is currently in the critical section.

## Desired properties for solutions to the Critical Section Problem?
There are three main desirable properties that we desire in a solution the critical section problem
* Mutual Exclusion - the thread/process gets exclusive access; others must wait until it exits the critical section.
* Bounded Wait - if the thread/process has to wait, then it should only have to wait for a finite,  amount of time (infinite waiting times are not allowed!). The exact definition of bounded wait is that there is an upper (non-infinite) bound on the number of times any other process can enter its critical section before the given process enters.
* Progress - if no thread/process is inside the critical section, then the thread/process should be able to proceed (make progress) without having to wait.


With these ideas in mind let's examine another candidate solution that uses a turn-based flag only if two threads both required access at the same time.

## Turn and Flag solutions

Is the following a correct solution to CSP?
```
\\ Candidate #4
raise my flag
if your flag is raised, wait until my turn
// Do Critical Section stuff
turn = yourid
lower my flag
```
One instructor and another CS faculty member initially thought so! However, analyzing these solutions is tricky. Even peer-reviewed papers on this specific subject contain incorrect solutions! At first glance it appears to satisfy Mutual Exclusion, Bounded Wait and Progress: The turn-based flag is only used in the event of a tie (so Progress and Bounded Wait is allowed) and mutual exclusion appears to be satisfied. However.... Perhaps you can find a counter-example?

Candidate #4 fails because a thread does not wait until the other thread lowers their flag. After some thought (or inspiration) the following scenario can be created to demonstrate how Mutual Exclusion is not satisfied.

Imagine the first thread runs this code twice (so the turn flag now points to the second thread). While the first thread is still inside the Critical Section, the second thread arrives. The second thread can immediately continue into the Critical Section!

Time | Turn | Thread #1 | Thread #2
-----|------|-----------|----------
1| 2 | raise my flag | 
2| 2 | if your flag is raised, wait until my turn | raise my flag  
3| 2 | // Do Critical Section stuff | if your flag is raised, wait until my turn(TRUE!) 
4| 2 | // Do Critical Section stuff | // Do Critical Section stuff - OOPS 

# Working Solutions

## What is Peterson's solution?
Peterson published his novel and surprisingly simple solution in a 2 page paper in 1981. 
A version of his algorithm is shown below that uses a shared variable `turn`: 

```
\\ Candidate #5
raise my flag
turn = your_id
wait while your flag is raised and turn is your_id
// Do Critical Section stuff
lower my flag
```

An equivalent pseudo-C version is below.
````C
\\ Candidate #5
bool flag1, flag2  //both initially false
int flag = 1

thread1:                          thread2:
  flag1 = true                      flag2 = true
  turn = 2                          turn = 1
  while(flag2 && turn == 2) {}      while(flag1 && turn == 1) {}
  Critical Section                  Critical Section
  flag1 = false                     flag2 = false
````
This solution satisfies Mutual Exclusion, Bounded Wait and Progress. If thread #2 has set turn to 1 and is currently inside the critical section. Thread #1 arrives, _sets the turn back to 2_ and now waits until thread 2 lowers the flag. 

A similar C-like version of Peterson's implementation is discussed at Wikipedia [here](https://en.wikipedia.org/wiki/Peterson%27s_algorithm#The_algorithm).

Link to Peterson's original article pdf:
[G. L. Peterson: "Myths About the Mutual Exclusion Problem", Information Processing Letters 12(3) 1981, 115–116](http://dl.acm.org/citation.cfm?id=945527)

## Was Peterson's solution the first solution?

No, Dekkers Algorithm (1962) was the first provably correct solution. A version of the algorithm is below.
```
raise my flag
while(your flag is raised) :
   if it's your turn to win :
     lower my flag
     wait while your turn
     raise my flag
// Do Critical Section stuff
set your turn to win
lower my flag
```

Notice how the process's flag is always raised during the critical section no matter if the loop is iterated zero, once or more times. Further the flag can be interpreted as an immediate intent to enter the critical section. Only if the other process has also raised the flag will one process defer, lower their intent flag and wait.

## Can I just implement Peterson's (or Dekkers) algorithm in C or assembler?
Yes - and with a bit searching it is possible even today to find it in production for specific simple mobile processors: Peterson's algorithm is used to implement low-level Linux Kernel locks for the Tegra mobile processor (a system-on-chip ARM process and GPU core by Nvidia)
https://android.googlesource.com/kernel/tegra.git/+/android-tegra-3.10/arch/arm/mach-tegra/sleep.S#58

However in general, CPUs and C compilers can re-order CPU instructions or use CPU-core-specific local cache values that are stale if another core updates the shared variables. Thus a simple pseudo-code to C implementation is too naive for most platforms. You can stop reading now.

Oh... you decided to keep reading. Well, here be dragons! Don't say we didn't warn you. Consider this advanced and gnarly topic but (spoiler alert) a happy ending.

Consider the following code,

```C
while(flag2 ) { } /* busy loop - go around again */
```
An efficient compiler would infer that `flag2` variable is never changed inside the loop, so that test can be optimized to `while(true)` 
Using `volatile` goes someway to prevent compiler optimizations of this kind.

Independent instructions can be re-ordered by an optimizing compiler or at runtime by an out-of-order execution optimization by the CPU. These sophisticated optimizations if the code requires variables to be modified and checked and a precise order.

A related challenge is that CPU cores include a data cache to store recently read or modified main memory values. Modified values may not be written back to main memory or re-read from memory immediately. Thus data changes, such as the state of a flag and turn variable in the above examples, may not be shared between two CPU codes. 

But there is happy ending. Fortunately, modern hardware addresses these issues using 'memory fences' (also known as memory barrier) CPU instructions to ensure that main memory and the CPUs' cache is in a reasonable and coherent state. Higher level synchronization primitives, such as `pthread_mutex_lock` will call these CPU instructions as part of their implementation. Thus, in practice, surrounding critical section with a mutex lock and unlock calls is sufficient to ignore these lower-level problems.

Advanced Further reading: we suggest the following web post that discusses implementing Peterson's algorithm on an x86 process and the linux documentation on memory barriers.

http://bartoszmilewski.com/2008/11/05/who-ordered-memory-fences-on-an-x86/

https://elixir.bootlin.com/linux/latest/source/Documentation/memory-barriers.txt

# Hardware Solutions

##  How would you implement the Critical Section Problem in hardware?
On a simple single CPU machine, a process with access to all CPU instructions, could temporarily disable interrupts.
```C
disable_interrupts
// Critical section code
enable_interrupts
```
If interrupts are disabled then the current thread cannot be interrupted! i.e. the CPU instructions of the critical section will complete.

However most systems today have more than one CPU core and disabling interrupts is a privileged instruction - so the above technique is rarely appropriate.

Instead, suppose the CPU provided us with a special atomic instruction `__exch` that swaps the values at two memory locations. We could support Critical sections by implementing mutex locks using the following pseudo code.
````C
my_mutex_init(int* m)  { *m= 0; }

my_mutex_lock(int* m) {
  for(int q = 1; q ; ) {  __exch(&m , &q); }
} // when this returns it is safe to enter your critical section

// After you critical section is finished,call unlock...
my_mutex_unlock(int* m)  { *m= 0; }
````

The exchange instruction must be atomic i.e. it behaves as a single __uninterruptable__ and indivisible instruction. For example, if two threads both call `my_mutex_lock` (and then __exch) at the same time, then one thread _will_ receive a value of 0, and the other thread will loose and get the newer value of 1 (so will continue to poll).


## How do we really implement the Critical Section Problem on real hardware? (Advanced topic)

A complete solution using C11 atomics is detailed here (this is a spinlock mutex, [futex](https://locklessinc.com/articles/mutex_cv_futex/) implementations can be found online).

```C
typedef struct mutex_{
    atomic_int_least8_t lock;
    pthread_t owner;
} mutex;

#define UNLOCKED 0
#define LOCKED 1
#define UNASSIGNED_OWNER 0

int mutex_init(mutex* mtx){
    if(!mtx){
        return 0;
    }
    atomic_init(&mtx->lock, UNLOCKED); // Not thread safe the user has to take care of this
    mtx->owner = UNASSIGNED_OWNER;
    return 1;
}
```

This is the initialization code, nothing fancy here. We set the state of the mutex to unlocked and set the owner to locked.

```C
int mutex_lock(mutex* mtx){
    int_least8_t zero = UNLOCKED;
    while(!atomic_compare_exchange_weak_explicit
            (&mtx->lock, 
             &zero, 
             LOCKED,
             memory_order_acq_rel,
             memory_order_relaxed)){
        zero = UNLOCKED;
        sched_yield(); //Use system calls for scheduling speed
    }
    //We have the lock now!!!!
    mtx->owner = pthread_self();
    return 1;
}
```
Yikes! What does this code do? Well to start, it initializes a variable that we will keep as the unlocked state. [Atomic Compare and Exchange](https://en.wikipedia.org/wiki/Compare-and-swap) is an instruction supported by most modern architectures (on x86 it's `lock cmpxchg`). The pseudocode for this operation looks like this

```C
int atomic_compare_exchange_pseudo(int* addr1, int* addr2, int val){
    if(*addr1 == *addr2){
        *addr1 = val;
        return 1;
    }else{
        *addr2 = *addr1;
        return 0;
    }
}
```
Except it is all done _atomically_ meaning in one uninterruptible operation. What does the _weak_ part mean? Well atomic instructions are also prone to **spurious failures** meaning that there are two versions to these atomic functions a _strong_ and a _weak_ part, strong guarantee the success or failure while weak may fail. We are using weak because weak is faster and we are in a loop! That means we are okay if it fails a little bit more often because we will just keep spinning around anyway.

What is this memory order business? We were talking about memory fences earlier, here it is! We won't go into detail because it is outside the scope of this course but not the scope of [this article](https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync).

Inside the while loop, we have failed to grab the lock! We reset zero to unlocked and sleep for a little while. When we wake up we try to grab the lock again. Once we successfully swap, we are in the critical section! We set the mutex's owner to the current thread for the unlock method and return successful.

How does this guarantee mutual exclusion, when working with atomics we are not entirely sure! But in this simple example we can because the thread that is able to successfully expect the lock to be UNLOCKED (0) and swap it to a LOCKED (1) state is considered the winner. How do we implement unlock?

```C
int mutex_unlock(mutex* mtx){
    if(unlikely(pthread_self() != mtx->owner)){
        return 0; //You can't unlock a mutex if you aren't the owner
    }
    int_least8_t one = 1;
    mtx->owner = UNASSIGNED_OWNER;
    //Critical section ends after this atomic
    //Also this may fail, but that is fine
    if(!atomic_compare_exchange_strong_explicit(
                &mtx->lock, 
                &one, 
                UNLOCKED,
                memory_order_acq_rel,
                memory_order_relaxed)){
        //The mutex was never locked in the first place
        return 0;
    }
    return 1;
}
```

To satisfy the api, you can't unlock the mutex unless you are the one who owns it. Then we unassign the mutex owner, because critical section is over after the atomic. We want a strong exchange because we don't want to block (pthread_mutex_unlock doesn't block). We expect the mutex to be locked, and we swap it to unlock. If the swap was successful, we unlocked the mutex. If the swap wasn't, that means that the mutex was UNLOCKED and we tried to switch it from UNLOCKED to UNLOCKED, preserving the non blocking of unlock.
