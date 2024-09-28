# The Pattern

## What does the following 'exec' example do?
```C
#include <unistd.h>
#include <fcntl.h> // O_CREAT, O_APPEND etc. defined here

int main() {
   close(1); // close standard out
   open("log.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
   puts("Captain's log");
   chdir("/usr/include");
   // execl( executable,  arguments for executable including program name and NULL at the end)

   execl("/bin/ls", /* Remaining items sent to ls*/ "/bin/ls", ".", (char *) NULL); // "ls ."
   perror("exec failed");
   return 0; // Not expected
}
```
There's no error checking in the above code (we assume close,open,chdir etc works as expected).
* open: will use the lowest available file descriptor (i.e. 1) ; so standard out now goes to the log file.
* chdir : Change the current directory to /usr/include
* execl : Replace the program image with /bin/ls and call its main() method
* perror : We don't expect to get here - if we did then exec failed.

## Subtle forkbomb bug

What's wrong with this code

```C
#include <unistd.h>
#define HELLO_NUMBER 10

int main(){
    pid_t children[HELLO_NUMBER];
    int i;
    for(i = 0; i < HELLO_NUMBER; i++){
        pid_t child = fork();
        if(child == -1){
            break;
        }
        if(child == 0){ //I am the child
             execlp("ehco", "echo", "hello", NULL);
        }
        else{
            children[i] = child;
        }
    }

    int j;
    for(j = 0; j < i; j++){
        waitpid(children[j], NULL, 0);
    }
    return 0;
}

```

We misspelled `ehco`, so we can't `exec` it. What does this mean? Instead of creating 10 processes we just created 2**10 processes, fork bombing our machine. How could we prevent this? Put an exit right after exec so in case exec fails we won't end up fork bombing our machine.

## What does the child inherit from the parent?
* Open file handles. If the parent later seeks, say, to the back to the beginning of the file then this will affect the child too (and vice versa). 
* Signal handlers
* Current working directory
* Environment variables

See the [fork man page](http://linux.die.net/man/2/fork) for more details.

## What is different in the child process than the parent process?
The process id is different. In the child calling `getppid()` (notice the two 'p's) will give the same result as calling getpid() in the parent. See the fork man page for more details.

## How do I wait for my child to finish?
Use `waitpid` or `wait`. The parent process will pause until `wait` (or `waitpid`) returns. Note this explanation glosses over the restarting discussion.

## What is the fork-exec-wait pattern

A common programming pattern is to call `fork` followed by `exec` and `wait`. The original process calls fork, which creates a child process. The child process then uses exec to start execution of a new program. Meanwhile the parent uses `wait` (or `waitpid`) to wait for the child process to finish.
See below for a complete code example.


## How do I start a background process that runs at the same time?
Don't wait for them! Your parent process can continue to execute code without having to wait for the child process. Note in practice background processes can also be disconnected from the parent's input and output streams by calling `close` on the open file descriptors before calling exec.

However child processes that finish before their parent finishes can become zombies. See the zombie page for more information.

# Zombies

## Good parents don't let their children become zombies!
Note, the word 'zombie' in this instance sheds some light as to what they actually represent. When a child finishes (or terminates) it still takes up a slot in the kernel process table. Furthermore, they still contain information about the process that got terminated, such as process id, exit status, etc. (i.e. a skeleton of the original process still remains).
Only when the child has been 'waited on' will the slot be available and the remaining information can be accessed by the parent.

A long running program could create many zombies by continually creating processes and never `wait`-ing for them.

## What would be effect of too many zombies?

Eventually there would be insufficient space in the kernel process table to create new processes. Thus `fork()` would fail and could make the system difficult / impossible to use - for example just logging in requires a new process!

## What does the system do to help prevent zombies?
Once a process completes, any of its children will be assigned to "init" - the first process with pid of 1. Thus these children would see getppid() return a value of 1. These orphans will eventually finish and for a brief moment become a zombie. Fortunately, the init process automatically waits for all of its children, thus removing these zombies from the system.

## How do I prevent zombies? (Warning: Simplified answer)
Wait on your child!
```C
waitpid(child, &status, 0); // Clean up and wait for my child process to finish.
```
Note we assume that the only reason to get a SIGCHLD event is that a child has finished (this is not quite true - see man page for more details).

A robust implementation would also check for interrupted status and include the above in a loop.
Read on for a discussion of a more robust implementation.

## How can I asynchronously wait for my child using SIGCHLD? (ADVANCED)

Warning: This section uses signals which we have not yet fully introduced.
The parent gets the signal SIGCHLD when a child completes, so the signal handler can wait on the process. A slightly simplified version is shown below.
```C
pid_t child;

void cleanup(int signal) {
  int status;
  waitpid(child, &status, 0);
  write(1,"cleanup!\n",9);
}
int main() {
   // Register signal handler BEFORE the child can finish
   signal(SIGCHLD, cleanup); // or better - sigaction
   child = fork();
   if (child == -1) { exit(EXIT_FAILURE);}

   if (child == 0) { /* I am the child!*/
     // Do background stuff e.g. call exec   
   } else { /* I'm the parent! */
      sleep(4); // so we can see the cleanup
      puts("Parent is done");
   }
   return 0;
} 
```

The above example however misses a couple of subtle points:
* More than one child may have finished but the parent will only get one SIGCHLD signal (signals are not queued)
* SIGCHLD signals can be sent for other reasons (e.g. a child process is temporarily stopped)

A more robust code to reap zombies is shown below.
```C
void cleanup(int signal) {
  int status;
  while (waitpid((pid_t) (-1), 0, WNOHANG) > 0) {}
}
```

## So what are environment variables?

Environment variables are variables that the system keeps for all processes to use. Your system has these set up right now! In Bash, you can check some of these

```
$ echo $HOME
/home/user
$ echo $PATH
/usr/local/sbin:/usr/bin:...
```

How would you get these in C/C++? You can use the `getenv` and `setenv` function

```C
char* home = getenv("HOME"); // Will return /home/user
setenv("HOME", "/home/user", 1 /*set overwrite to true*/ );
```

## Right, so how do these environment variables mean anything to parent/child?

Well each process gets its own dictionary of environment variables that are copied over to the child. Meaning, if the parent changes their environment variables it won't be transferred to the child and vice versa. This is important in the fork-exec-wait trilogy if you want to exec a program with different environment variables than your parent (or any other process).

For example, you can write a C program that loops through all of the time zones and executes the `date` command to print out the date and time in all locals. Environment variables are used for all sorts of programs so modifying them is important.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-1:-Introduction">
Back: Forking, Part 1: Introduction
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals">
Next: Process Control, Part 1: Wait macros, using signals
</a>
</div>
