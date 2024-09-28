# Topics
* Virtual Memory
* Page Table
* MMU/TLB
* Address Translation
* Page Faults
* Frames/Pages
* Single level vs multi level page table
* Calculating offsets for multi-level page table
* Pipes
* Pipe read write ends
* Writing to a zero reader pipe
* Reading from a zero writer pipe
* Named pipe and Unnamed Pipes
* Buffer Size/Atomicity
* Scheduling Algorithms
* Measures of Efficiency

# Questions
* What is virtual memory?
* What are the following and what is their purpose?
    * Translation Lookaside Buffer
    * Physical Address
    * Memory Management Unit. Multilevel page table. Frame number. Page number and page offset.
    * The dirty bit
    * The NX Bit
* What is a page table? How about a physical frame? Does a page always need to point to a physical frame?
* What is a page fault? What are the types? When does it result in a segfault?
* What are the advantages to a single level page table? Disadvantages? How about a multi leveled table?
* What does a multi leveled table look like in memory?
* How do you determine how many bits are used in the page offset?
* Given a 64 bit address space, 4kb pages and frames, and a 3 level page table, how many bits is the Virtual page number 1, VPN2, VPN3 and the offset?
* What is a pipe? How do I create a pipe?
* When is SIGPIPE delivered to a process?
* Under what conditions will calling read() on a pipe block? Under what conditions will read() immediately return 0
* What is the difference between a named pipe and an unnamed pipe?
* Is a pipe thread safe?
* Write a function that uses fseek and ftell to replace the middle character of a file with an 'X'
* Write a function that create a pipe and uses write to send 5 bytes, "HELLO" to the pipe. Return the read file descriptor of the pipe.
* What happens when you mmap a file?
* Why is getting the file size with ftell not recommended? How should you do it instead?
* What is scheduling?
* What is turnaround time? Response Time? Wait time?
* What is the convoy effect?
* Which algorithms have the best turnaround/response/wait time on average