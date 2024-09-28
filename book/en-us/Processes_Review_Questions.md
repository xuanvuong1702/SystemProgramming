## Topics
* Correct use of fork, exec and waitpid
* Using exec with a path
* Understanding what fork and exec and waitpid do. E.g. how to use their return values.
* SIGKILL vs SIGSTOP vs SIGINT. 
* What signal is sent when you press CTRL-C
* Using kill from the shell or the kill POSIX call.
* Process memory isolation.
* Process memory layout (where is the heap, stack etc; invalid memory addresses).
* What is a fork bomb, zombie and orphan? How to create/remove them.
* getpid vs getppid
* How to use the WAIT exit status macros WIFEXITED etc.


# Questions/Exercises 

* What is the difference between execs with a p and without a p? What does the operating system
* How do you pass in command line arguments to `execl*`? How about `execv*`? What should be the first command line argument by convention?
* How do you know if `exec` or `fork` failed?
* What is the `int *status` pointer passed into wait? When does wait fail?
* What are some differences between `SIGKILL`, `SIGSTOP`, `SIGCONT`, `SIGINT`? What are the default behaviors? Which ones can you set up a signal handler for?
* What signal is sent when you press `CTRL-C`?
* My terminal is anchored to PID = 1337 and has just become unresponsive. Write me the terminal command and the C code to send `SIGQUIT` to it.
* Can one process alter another processes memory through normal means? Why?
* Where is the heap, stack, data, and text segment? Which segments can you write to? What are invalid memory addresses?
* Code me up a fork bomb in C (please don't run it).
* What is an orphan? How does it become a zombie? How do I be a good parent?
* Don't you hate it when your parents tell you that you can't do something? Write me a program that sends `SIGSTOP` to your parent.
* Write a function that fork exec waits an executable, and using the wait macros tells me if the process exited normally or if it was signaled. If the process exited normally, then print that with the return value. If not, then print the signal number that caused the process to terminate.
<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Process-Control,-Part-1:-Wait-macros,-using-signals">
Back: Process Control, Part 1: Wait macros, using signals
</a>
</div>