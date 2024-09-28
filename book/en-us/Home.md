# Foreword

Welcome to Angrave's crowd-sourced System Programming wiki-book!
This wiki was actively built and maintained 2014-2018 by students and faculty from the University of Illinois. It was a crowd-source authoring experiment by Lawrence Angrave from CS @ Illinois and represents Angrave's CS241 (now CS341) "Intro to System Programming" course content from that time. This wiki will always be available to everyone to learn more about system programming. Please let me know (angrave @ illinois) if you find it helpful!

 University of Illinois students should use the CS341-specific coursebook -

http://cs341.cs.illinois.edu/coursebook/index.html

# Contents

## 0. Introduction
* [[Informal Glossary]]
* [[Programming Tricks, Part 1 ]]
* [[System Programming Short Stories and Songs ]]
* [[Systems Programming Bugs in Industry]]

## 1. Learning C
* [[C Programming, Part 1: Introduction]]
* [[C Programming, Part 2: Text Input And Output]]
   * [Printing to/Reading from Streams](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output#printing-to-streams)
   * [Parsing Input](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output#parsing-input)
* [[C Programming, Part 3: Common Gotchas]]
   * [Memory Mistakes](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#memory-mistakes)
   * [Logic/Programming Flow](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#logic-and-program-flow-mistakes)
   * [Other Gotchas](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#other-gotchas)
* [[C Programming, Part 4: Strings and Structs]]
   * [Strings](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs#strings-structs-and-gotchas)
   * [Structs](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs#so-whats-a-struct)
* [[C Programming, Part 5: Debugging]]
   * [In Code Debugging](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#in-code-debugging)
   * [Valgrind](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#valgrind)
   * [Tsan](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#tsan)
   * [GDB](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#gdb)
* [[C Programming, Review Questions]]

## 2. Processes
* [[Kernel, Shells, Terminals Oh My!]]
* [[Processes, Part 1: Introduction]]
   * [Overview](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#overview)
   * [Process Contents](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#process-contents)
   * [Bonus: More Contents](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#process-functionslimitations-bonus)
* [[Forking, Part 1: Introduction]]
   * [Introduction](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction#intro-to-fork)
   * [Waiting and Execing](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction#waiting-and-execing)
* [[Forking, Part 2: Fork, Exec, Wait]]
   * [The Pattern](https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-2:-Fork,-Exec,-Wait#the-pattern)
   * [Zombies](https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-2:-Fork,-Exec,-Wait#zombies)
* [[Process Control, Part 1: Wait macros, using signals]]
   * [Wait Macros](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#wait-macros)
   * [Signals](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#signals)
* [[Processes Review Questions]]

## 3. Memory and Allocators
* [[Memory, Part 1: Heap Memory Introduction]]
   * [C Dynamic Memory Allocation](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction#c-dynamic-memory-allocation)
   * [Introduction to Allocating](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction#intro-to-allocating)
* [[Memory, Part 2: Implementing a Memory Allocator]]
   * [Memory Allocator Tutorial](https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator#memory-allocator-tutorial)
* [[Memory, Part 3: Smashing the Stack Example]]
* [[Memory Review Questions]]

## 4. Intro to Pthreads
* [[Pthreads, Part 1: Introduction]]
   * [Intro to Threads](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction#intro-to-threads)
   * [Simple Pthreads](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction#simple-usage)
* [[Pthreads, Part 2: Usage in Practice]]
   * [More pthread Functions](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-2:-Usage-in-Practice#more-pthread-functions)
   * [Intro to Race Conditions](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-2:-Usage-in-Practice#intro-to-race-conditions)
* [[Pthreads, Part 3: Parallel Problems (Bonus) ]]
* [[Pthread Review Questions]]

## 5. Synchronization
* [[Synchronization, Part 1: Mutex Locks]]
    * [Solving Critical Sections](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-1:-Mutex-Locks#solving-critical-sections)
    * [Mutex Gotchas](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-1:-Mutex-Locks#mutex-gotchas)
* [[Synchronization, Part 2: Counting Semaphores]]
* [[Synchronization, Part 3: Working with Mutexes And Semaphores]]
    * [Thread Safe Stack](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-3%3A-Working-with-Mutexes-And-Semaphores#thread-safe-stack)
    * [Stack Semaphores](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-3%3A-Working-with-Mutexes-And-Semaphores#stack-semaphores)
* [[Synchronization, Part 4: The Critical Section Problem]]
    * [Candidate Solutions](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#candidate-solutions)
    * [Working Solutions](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#working-solutions)
    * [Hardware Solutions](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#hardware-solutions)
* [[Synchronization, Part 5: Condition Variables]]
    * [Intro To Condition Variables](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-5%3A-Condition-Variables#intro-to-condition-variables)
    * [Implementing a Counting Semaphore](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-5%3A-Condition-Variables#implementing-counting-semphore)
* [[Synchronization, Part 6: Implementing a barrier]]
* [[Synchronization, Part 7: The Reader Writer Problem]]
* [[Synchronization, Part 8: Ring Buffer Example]]
* [[Synchronization, Part 9: Synchronization Across Processes]]
* [[Synchronization Review Questions]]

## 6. Deadlock
* [[Deadlock, Part 1: Resource Allocation Graph]]
* [[Deadlock, Part 2: Deadlock Conditions]]
* [[Deadlock, Part 3: Dining Philosophers]]
    * [Failed Solutions](https://github.com/angrave/SystemProgramming/wiki/Deadlock,-Part-3:-Dining-Philosophers#failed-solutions)
    * [Viable Solutions](https://github.com/angrave/SystemProgramming/wiki/Deadlock,-Part-3:-Dining-Philosophers#viable-solutions)
* [[Deadlock Review Questions]]

## 7. Inter-process Communication & Scheduling
* [[Virtual Memory, Part 1: Introduction to Virtual Memory]]
    * [What is Virtual Memory?](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory#what-is-virtual-memory)
    * [Advanced Frames and Protections](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory#advanced-frames-and-page-protections)
* [[Pipes, Part 1: Introduction to pipes]]
* [[Pipes, Part 2: Pipe programming secrets]]
    * [Pipe Gotchas](https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#pipe-gotchas)
    * [Named Pipes](https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#named-pipes)
* [[ Files, Part 1: Working with files]]
* [[ Scheduling, Part 1: Scheduling Processes ]]
    * [Thinking about Scheduling](https://github.com/angrave/SystemProgramming/wiki/Scheduling%2C-Part-1%3A-Scheduling-Processes#thinking-about-scheduling)
    * [Measures of Efficiency](https://github.com/angrave/SystemProgramming/wiki/Scheduling%2C-Part-1%3A-Scheduling-Processes#measures-of-efficiency)
* [[ Scheduling, Part 2: Scheduling Processes: Algorithms ]]
* [[ IPC Review Questions]]

## 8. Networking
* [[ POSIX, Part 1: Error handling]]
* [[ Networking, Part 1: Introduction]]
* [[ Networking, Part 2: Using getaddrinfo ]]
* [[ Networking, Part 3: Building a simple TCP Client ]]
* [[ Networking, Part 4: Building a simple TCP Server ]]
* [[ Networking, Part 5: Shutting down ports, reusing ports and other tricks ]]
* [[ Networking, Part 6: Creating a UDP server ]]
* [[ Networking, Part 7: Nonblocking I O, select(), and epoll ]]
* [[ Networking, Part 8: Protocols (TCP Handshaking, HTTP latency, Heart Bleed) ]]
* [[ RPC, Part 1: Introduction to Remote Procedure Calls ]]
* [[ Networking Review Questions ]]

## 9. File Systems
* [[ File System, Part 1: Introduction ]]
    * [Navigation/Terminology](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction#navigationterminology)
    * [What's a File System?](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction#so-whats-a-file-system)
* [[ File System, Part 2: Files are inodes]]
* [[ File System, Part 3: Permissions ]]
* [[ File System, Part 4: Working with directories ]]
* [[ File System, Part 5: Virtual file systems ]]
* [[ File System, Part 6: Memory mapped files and Shared memory ]]
* [[ File System, Part 7: Scalable and Reliable Filesystems ]]
    * [Reliability with a Single Disk](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-7%3A-Scalable-and-Reliable-Filesystems#reliable-single-disk-filesystems)
    * [Redundancy](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-7%3A-Scalable-and-Reliable-Filesystems#redundancy)
* [[ File System, Part 8: Removing preinstalled malware from an Android device ]]
* [[ File System, Part 9: Disk blocks example ]]
* [[ File Systems Review Questions ]]

## 10. Signals
* [[Process Control, Part 1: Wait macros, using signals]]
   * [Wait Macros](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#wait-macros)
   * [Signals](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#signals)
* [[ Signals, Part 2: Pending Signals and Signal Masks ]]
   * [Signals in Depth](https://github.com/angrave/SystemProgramming/wiki/Signals,-Part-2:-Pending-Signals-and-Signal-Masks#signals-in-depth)
   * [Disposition in Threads/Children](https://github.com/angrave/SystemProgramming/wiki/Signals,-Part-2:-Pending-Signals-and-Signal-Masks#disposition-in-threadschildren)
* [[ Signals, Part 3: Raising signals ]]
* [[ Signals, Part 4: Sigaction ]]
* [[ Signals Review Questions ]]

## Exam Practice and Review Questions
Warning these are good practice but not comprehensive.

* [[Exam Topics]]
* [[C Programming: Review Questions]]
* [[Multi-threaded Programming: Review Questions]]
* [[Synchronization Concepts: Review Questions]]
* [[Memory: Review Questions]]
* [[Pipe: Review Questions]]
* [[Filesystem: Review Questions]]
* [[Networking: Review Questions]]
* [[Signals: Review Questions]]
* [[System Programming Jokes]]
