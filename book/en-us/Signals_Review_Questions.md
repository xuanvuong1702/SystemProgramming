# Topics
* Signals
* Signal Handler Safe
* Signal Disposition
* Signal States
* Pending Signals when Forking/Exec
* Signal Disposition when Forking/Exec
* Raising Signals in C
* Raising Signals in a multithreaded program

# Questions
* What is a signal?
* How are signals served under UNIX? (Bonus: How about Windows?)
* What does it mean that a function is signal handler safe
* What is a process Signal Disposition?
* How do I change the signal disposition in a single threaded program? How about multithreaded?
* Why sigaction vs signal?
* How do I asynchronously and synchronously catch a signal?
* What happens to pending signals after I fork? Exec?
* What happens to my signal disposition after I fork? Exec?