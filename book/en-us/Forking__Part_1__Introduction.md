## A word of warning ##
Process forking is a very powerful (and very dangerous) tool. If you mess up and cause a fork bomb (explained later on this page), **you can bring down the entire system**. To reduce the chances of this, limit your maximum number of processes to a small number e.g
 40 by typing ```ulimit -u 40``` into a command line. Note that this limit is only for the user, which means if you fork bomb, then you won't be able to kill all of the processes you just created since calling ```killall``` requires your shell to fork() ... [ironic](https://imgur.com/gallery/wefaH) right? So what can we do about this. One solution is to spawn another shell instance as another user (for example root) before hand and kill processes from there. Another is to use the built in ```exec``` command to kill all the user processes (careful you only have one shot at this). Finally you could reboot the system :)

When testing fork() code, ensure that you have either root and/or physical access to the machine involved. If you must work on fork () code remotely, remember that **kill -9 -1** will save you in the event of an emergency.

TL;DR: Fork can be **extremely** dangerous if you aren't prepared for it. **You have been warned.**

# Intro to Fork

## What does fork do?

The `fork` system call clones the current process to create a new process. It creates a new process (the child process) by duplicating the state of the existing process with a few minor differences (discussed below). The child process does not start from main. Instead it returns from `fork()` just as the parent process does.

Just as a side remark, in older UNIX systems, the entire address space of the parent process was directly copied (regardless of whether the resource was modified or not). These days, kernel performs [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write), which saves a lot of resources, while being very time efficient.
## What is the simplest `fork()` example?
Here's a very simple example...
```C
printf("I'm printed once!\n");
fork();
// Now there are two processes running
// and each process will print out the next line.
printf("You see this line twice!\n");
```

## Why does this example print 42 twice?
The following program prints out 42 twice - but the `fork()` is after the `printf`!? Why?
```C
#include <unistd.h> /*fork declared here*/
#include <stdio.h> /* printf declared here*/
int main() {
   int answer = 84 >> 1;
   printf("Answer: %d", answer);
   fork();
   return 0;
}
```
The `printf` line _is_ executed only once however notice that the printed contents is not flushed to standard out (there's no newline printed, we didn't call `fflush`, or change the buffering mode).
The output text is therefore still in process memory waiting to be sent.
When `fork()` is executed the entire process memory is duplicated including the buffer. Thus the child process starts with a non-empty output buffer which will be flushed when the program exits.

## How do you write code that is different for the parent and child process?

Check the return value of `fork()`. 

If fork() returns -1, that implies something went wrong in the process of creating a new child. One should check the value stored in _errno_ to determine what kind of error occurred; commons one include EAGAIN and ENOMEM (check [this page](http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html) to get a description of the errors).

Similarly, a return value of 0 indicates that we are in the child process, while a positive integer shows that we are in parent process. The positive value returned by fork() gives as the process id (_pid_) of the child.

Here's one way to remember which is which:

The child process can find its parent - the original process that was duplicated -  by calling `getppid()` - so does not need any additional return information from `fork()`. The parent process however can only find out the id of the new child process from the return value of `fork`:
```C
pid_t id = fork();
if (id == -1) exit(1); // fork failed 
if (id > 0)
{ 
// I'm the original parent and 
// I just created a child process with id 'id'
// Use waitpid to wait for the child to finish
} else { // returned zero
// I must be the newly made child process
}
```

## What is a fork bomb ?
A 'fork bomb' is when you attempt to create an infinite number of processes. A simple example is shown below:
```C
while (1) fork();
```
This will often bring a system to a near-standstill as it attempts to allocate CPU time and memory to a very large number of processes that are ready to run. Comment: System administrators don't like fork-bombs and may set upper limits on the number of processes each user can have or may revoke login rights because it creates a disturbance in the force for other users' programs. You can also limit the number of child processes created by using `setrlimit()`.

fork bombs are not necessarily malicious - they occasionally occur due to student coding errors.

Angrave suggests that the Matrix trilogy, where the machine and man finally work together to defeat the multiplying Agent-Smith, was a cinematic plot based on an AI-driven fork-bomb.

# Waiting and Execing

## How does the parent process wait for the child to finish?
Use `waitpid` (or `wait`).

```C
pid_t child_id = fork();
if (child_id == -1) { perror("fork"); exit(EXIT_FAILURE);}
if (child_id > 0) { 
  // We have a child! Get their exit code
  int status; 
  waitpid( child_id, &status, 0 );
  // code not shown to get exit status from child
} else { // In child ...
  // start calculation
  exit(123);
}
```

## Can I make the child process execute another program?
Yes. Use one of the [`exec`](http://man7.org/linux/man-pages/man3/exec.3.html) functions after forking. The `exec` set of functions replaces the process image with the the process image of what is being called. This means that any lines of code after the `exec` call are replaced. Any other work you want the child process to do should be done before the `exec` call.  

The [Wikipedia article](https://en.wikipedia.org/wiki/Exec_(system_call)#C_language_prototypes) does a great job helping you make sense of the names of the exec family.

The naming schemes can be shortened like this

> The base of each is exec (execute), followed by one or more letters:
>
> e – An array of pointers to environment variables is explicitly passed to the new process image.
>
> l – Command-line arguments are passed individually (a list) to the function.
>
> p – Uses the PATH environment variable to find the file named in the file argument to be executed.
>
> v – Command-line arguments are passed to the function as an array (vector) of pointers.

```C
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char**argv) {
  pid_t child = fork();
  if (child == -1) return EXIT_FAILURE;
  if (child) { /* I have a child! */
    int status;
    waitpid(child , &status ,0);
    return EXIT_SUCCESS;

  } else { /* I am the child */
    // Other versions of exec pass in arguments as arrays
    // Remember first arg is the program name
    // Last arg must be a char pointer to NULL

    execl("/bin/ls", "ls","-alh", (char *) NULL);

    // If we get to this line, something went wrong!
    perror("exec failed!");
  }
}
```
## A simpler way to execute another program
Use `system`. Here is how to use it:
```C

#include <unistd.h>
#include <stdlib.h>

int main(int argc, char**argv) {
  system("ls");
  return 0;
}
```
The `system` call will fork, execute the command passed by parameter and the original parent process will wait for this to finish. This also means that `system` is a blocking call: The parent process can't continue until the process started by `system` exits. This may or may not be useful. Also, `system` actually creates a shell which is then given the string, which is more overhead than just using `exec` directly. The standard shell will use the `PATH` environment variable to search for a filename that matches the command. Using system will usually be sufficient for many simple run-this-command problems but can quickly become limiting for more complex or subtle problems, and it hides the mechanics of the fork-exec-wait pattern so we encourage you to learn and use `fork` `exec` and `waitpid` instead.
 
## What is the silliest fork example?
A slightly silly example is shown below. What will it print? Try it with multiple arguments to your program.
```C
#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv) {
  pid_t id;
  int status; 
  while (--argc && (id=fork())) {
    waitpid(id,&status,0); /* Wait for child*/
  }
  printf("%d:%s\n", argc, argv[argc]);
  return 0;
}
```

The amazing parallel apparent-O(N) _sleepsort_ is today's silly winner. First published on [4chan in 2011 ](https://dis.4chan.org/read/prog/1295544154). A version of this awful but amusing sorting algorithm is shown below.
```C
int main(int c, char **v)
{
        while (--c > 1 && !fork());
        int val  = atoi(v[c]);
        sleep(val);
        printf("%d\n", val);
        return 0;
}
```

Note: The algorithm isn't actually O(N) because of how the system scheduler works. Though there are parallel algorithms that run in O(log(N)) per process, this is sadly not one of them.

## What is different in the child process than the parent process?
The key differences include:
* The process id returned by `getpid()`. The parent process id returned by `getppid()`.
* The parent is notified via a signal, SIGCHLD, when the child process finishes but not vice versa.
* The child does not inherit pending signals or timer alarms.
For a complete list see the [fork man page](http://man7.org/linux/man-pages/man2/fork.2.html)

# Do child processes share open filehandles?
Yes! In fact both processes use the same underlying kernel file descriptor. For example if one process rewinds the random access position back to the beginning of the file, then both processes are affected.

Both child and parent should `close` (or `fclose`) their file descriptors or file handle respectively.

## How can I find out more?
Read the man pages!
* [fork](http://man7.org/linux/man-pages/man2/fork.2.html)
* [exec](http://man7.org/linux/man-pages/man3/exec.3.html)
* [wait](http://man7.org/linux/man-pages/man2/wait.2.html)

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction">
Back: Processes, Part 1: Introduction
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait">
Next: Forking, Part 2: Fork, Exec, Wait
</a>
</div>