# Wait Macros

## Can I find out the exit value of my child?

You can find the lowest 8 bits of the child's exit value (the return value of `main()` or value included in `exit()`): Use the "Wait macros" - typically you will use "WIFEXITED" and "WEXITSTATUS" . See `wait`/`waitpid` man page for more information).
```C
int status;
pid_t child = fork();
if (child == -1) return 1; // Failed
if (child > 0) { /* I am the parent - wait for the child to finish */
    pid_t pid = waitpid(child, &status, 0);
    if (pid != -1 && WIFEXITED(status)) {
        int low8bits = WEXITSTATUS(status);
        printf("Process %d returned %d" , pid, low8bits);
    }
} else { /* I am the child */
    // do something interesting
    execl("/bin/ls", "/bin/ls", ".", (char *) NULL); // "ls ."
}
```

A process can only have 256 return values, the rest of the bits are informational.

## Bit Shifting

Note there is no need to memorize this, this is just a high level overview of how information is stored inside the status variables

From Android source code:
```C
/* If WIFEXITED(STATUS), the low-order 8 bits of the status. */

#define __WEXITSTATUS(status) (((status) & 0xff00) >> 8)

/* If WIFSIGNALED(STATUS), the terminating signal. */

#define __WTERMSIG(status) ((status) & 0x7f)

/* If WIFSTOPPED(STATUS), the signal that stopped the child. */

#define __WSTOPSIG(status) __WEXITSTATUS(status)

/* Nonzero if STATUS indicates normal termination. */

#define __WIFEXITED(status) (__WTERMSIG(status) == 0)
```

The kernel has an internal way of keeping track of signaled, exited, or stopped. That API is abstracted so that that the kernel developers are free to change at will.

## Being careful.

Remember that the the macros only make sense if the precondition is met. Meaning that a process' exit status won't be defined if the process is signaled. The macros will not do the checking for you, so it's up to the programming to make sure the logic checks out.

# Signals

## What's a signal?

A signal is a construct provided to us by the kernel. It allows one process to asynchronously send a signal (think a message) to another process. If that process wants to accept the signal, it can, and then, for most signals, can decide what to do with that signal. Here is a short list (non comprehensive) of signals.

|   Name   |             Default Action             | Usual Use Case |
|----------|----------------------------------------|--------------------------------|
| SIGINT   | Terminate Process (Can be caught)      | Tell the process to stop nicely |
| SIGQUIT  | Terminate Process (Can be caught)      | Tells the process to stop harshly |
| SIGSTOP  | Stop Process (Cannot be caught)        | Stops the process to be continued |
| SIGCONT  | Continues a Process                    | Continues to run the process |
| SIGKILL  | Terminate Process (Cannot be caught)   | You want your process gone |

## When are signals generated?

* When the user sends a signal. For example, you are at the terminal, and you send `CTRL-C`
* When a system event happens. For example, you get a `SIGCHILD` after forking to notice when one of your children have exited.
* When another program sends it. For example, when you execute `kill -9 PID`, it sends `SIGKILL`
* When an appropriate hardware interrupt is triggered. For example, if you access a page that you aren't supposed to, the hardware generates a segfault interrupt which gets intercepted by the kernel. The kernel finds the process that caused this and sends a software interrupt signal `SIGSEGV`.

## Can I pause my child?

Yes ! You can temporarily pause a running process by sending it a SIGSTOP signal.
If it succeeds it will freeze a process; i.e. the process will not be allocated any more CPU time.

To allow a process to resume execution send it the SIGCONT signal.

For example,
Here's program that slowly prints a dot every second, up to 59 dots.
```C
#include <unistd.h>
#include <stdio.h>
int main() {
    printf("My pid is %d\n", getpid());
    int i = 60;
    while (--i) { 
        write(1, ".", 1);
        sleep(1);
    }
    write(1, "Done!", 5);
    return 0;
}
```
We will first start the process in the background (notice the & at the end).
Then send it a signal from the shell process by using the kill command.
```
>./program &
My pid is 403
...
>kill -SIGSTOP 403
>kill -SIGCONT 403
```

## How do I kill/stop/suspend my child from C?
In C, send a signal to the child using `kill` POSIX call,
```C
kill(child, SIGUSR1); // Send a user-defined signal
kill(child, SIGSTOP); // Stop the child process (the child cannot prevent this)
kill(child, SIGTERM); // Terminate the child process (the child can prevent this)
kill(child, SIGINT); // Equivalent to CTRL-C (by default closes the process)
```

As we saw above there is also a kill command available in the shell
e.g. get a list of running processes and then terminate process 45 and process 46
```
ps
kill -l 
kill -9 45
kill -s TERM 46
```
## How can I detect "CTRL-C" and clean up gracefully?

We will return to signals later on - this is just a short introduction. On a Linux system, see `man -s7 signal` if you are interested in finding out more (for example a list of system and library calls that are async-signal-safe).

There are strict limitations on the executable code inside a signal handler. Most library and system calls are not 'async-signal-safe' - they may not be used inside a signal handler because they are not re-entrant safe. In a single-threaded program, signal handling momentarily interrupts the program execution to execute the signal handler code instead. Suppose your original program was interrupted while executing the library code of `malloc` ;  the memory structures used by malloc will not be in a consistent state. Calling `printf` (which uses `malloc`) as part of the signal handler is unsafe and will result in "undefined behavior" i.e. it is no longer a useful,predictable program. In practice your program might crash, compute or generate incorrect results or stop functioning ("deadlock"), depending on exactly what your program was executing when it was interrupted to execute the signal handler code.


One common use of signal handlers is to set a boolean flag that is occasionally polled (read) as part of the normal running of the program. For example,
```C
int pleaseStop; // See notes on why "volatile sig_atomic_t" is better

void handle_sigint(int signal) {
    pleaseStop = 1;
}

int main() {
    signal(SIGINT, handle_sigint);
    pleaseStop = 0;
    while (! pleaseStop) { 
        /* application logic here */ 
    }
    /* cleanup code here */
}
```

The above code might appear to be correct on paper. However, we need to provide a hint to the compiler and to the CPU core that will execute the `main()` loop. We need to prevent a compiler optimization: The expression `! pleaseStop` appears to be a loop invariant i.e. true forever, so can be simplified to `true`.  Secondly, we need to ensure that the value of `pleaseStop` is not cached using a CPU register and instead always read from and written to main memory. The `sig_atomic_t` type implies that all the bits of the variable can be read or modified as an "atomic operation" - a single uninterruptable operation. It is impossible to read a value that is composed of some new bit values and old bit values.

By specifying `pleaseStop` with the correct type `volatile sig_atomic_t` we can write portable code where the main loop will be exited after the signal handler returns. The `sig_atomic_t` type can be as large as an `int` on most modern platforms but on embedded systems can be as small as a `char` and only able to represent (-127 to 127) values.

```C
volatile sig_atomic_t pleaseStop;
```
Two examples of this pattern can be found in "COMP" a terminal based 1Hz 4bit computer (https://github.com/gto76/comp-cpp/blob/1bf9a77eaf8f57f7358a316e5bbada97f2dc8987/src/output.c#L121).
Two boolean flags are used. One to mark the delivery of `SIGINT` (CTRL-C), and gracefully shutdown the program, and the other to mark `SIGWINCH` signal to detect terminal resize and redraw the entire display. 

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait/_edit">
Back: Forking, Part 2: Fork, Exec, Wait
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Processes-Review-Questions">
Next: Processes Review Questions
</a>
</div>
