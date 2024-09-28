# Topics
* pthread lifecycle
* Each thread has a stack
* Capturing return values from a thread
* Using `pthread_join`
* Using `pthread_create`
* Using `pthread_exit`
* Under what conditions will a process exit

# Questions
* What happens when a pthread gets created? (you don't need to go into super specifics)
* Where is each thread's stack?
* How do you get a return value given a `pthread_t`? What are the ways a thread can set that return value? What happens if you discard the return value?
* Why is `pthread_join` important (think stack space, registers, return values)?
* What does `pthread_exit` do under normal circumstances (ie you are not the last thread)? What other functions are called when you call pthread_exit?
* Give me three conditions under which a multithreaded process will exit. Can you think of any more?
* What is an embarrassingly parallel problem?