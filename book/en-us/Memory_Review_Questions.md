# Topics
* Best Fit
* Worst Fit
* First Fit
* Buddy Allocator
* Internal Fragmentation
* External Fragmentation
* sbrk
* Natural Alignment
* Boundary Tag
* Coalescing
* Splitting
* Slab Allocation/Memory Pool

# Questions/Exercises
* What is Internal Fragmentation? When does it become an issue?
* What is External Fragmentation? When does it become an issue?
* What is a Best Fit placement strategy? How is it with External Fragmentation? Time Complexity?
* What is a Worst Fit placement strategy? Is it any better with External Fragmentation? Time Complexity?
* What is the First Fit Placement strategy? It's a little bit better with Fragmentation, right? Expected Time Complexity?
* Let's say that we are using a buddy allocator with a new slab of 64kb. How does it go about allocating 1.5kb?
* When does the 5 line `sbrk` implementation of malloc have a use?
* What is natural alignment?
* What is Coalescing/Splitting? How do they increase/decrease fragmentation? When can you coalesce or split?
* How do boundary tags work? How can they be used to coalesce or split?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-3%3A-Smashing-the-Stack-Example">
Back: Memory, Part 3: Smashing the Stack Example
</a>
</div>