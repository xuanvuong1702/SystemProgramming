# Kernels

The operating system kernel is a special piece of software. This is the piece of software that is loaded up before all of your other programs even consider getting booted up. What the kernel does is the following, abbreviated

1. The operating system executes ROM or read only code
2. The operating system then executes a `boot_loader` or `EFI` extensions nowadays
3. The boot_loader loads your kernels
4. Your kernel executes `init` to [bootstrap](https://en.wikipedia.org/wiki/Bootstrapping) itself from nothing
5. The kernel executes start up scripts
6. The kernel executes userland scripts, and you get to use your computer!

You don't need to know the specifics of the booting process, but there it is. When you are executing in user space the kernel provides some important operations that programs don't have to worry about.
* Scheduling Processes and threads; in addition, handling synchronization primitives
* Providing System Calls like `write` or `read`
* Manages virtual memory and low level binary devices like `usb` drivers
* Handles reading and understanding a filesystem
* Handles communicating over networks
* Handles communications with other processes
* Dynamically linking libraries

The kernel handles all of this stuff in kernel mode. Kernel mode gets you greater power, like executing extra CPU instructions but at the cost of one failure crashes your entire computer -- ouch. That is what you are going to interacting with in this class.

### File Descriptors

One of the things that you have already become familiar with is that the kernel gives you file descriptors when you open text files. Here is a zine from Julia Evans that details it a bit.

![Fds](https://drawings.jvns.ca/drawings/filedescriptors.jpeg)

As the little zine shows, the Kernel keeps track of the file descriptors and what they point to. We will see later that file descriptors need not point to actual files and the OS keeps track of them for you. Also, notice that between processes file descriptors may be reused but inside of a process they are unique.

File descriptors also have a notion of position. You can read a file on disk completely because the OS keeps track of the position in the file, and that belongs to your process as well.

## Cool, what's a shell then?

A shell is actually how you are going to be interacting with the kernel. Before User Friendly operating systems, when a computer started up all you had access to was a shell. This meant that all of your commands and editing had to be done this way. Nowadays, our computers start up in desktop mode, but one can still access a shell using a terminal. When you open one up you should see something like this

```
(Stuff) $
```

It is ready for your next command! You can type in a lot of unix utilities like `ls`, `echo Hello` and the shell will execute them and give you the result. Some of these are what are known as `shell-builtins` meaning that the code is in the shell program itself. Some of these are compiled programs that you run. The shell only looks through a special variable called path which contains a list of `:` separated paths to search for an executable with your name, here is an example path.

```
$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
```

So when the shell executes `ls`, it looks through all of those directories, finds `/bin/ls` and executes that.

```
$ ls
...
$ /bin/ls
```

You can always call through the full path. That is always why in past classes if you want to run something on the terminal you've had to do `./exe` because typically the directory that you are working in is not in the `PATH` variable. The `.` expands to your current directory and your shell executes `<current_dir>/exe` which is a valid command.

### Shell tricks and tips
* The up arrow will get you your most recent command
* `ctrl-r` will search commands that you previously ran
* `ctrl-c` will interrupt your shell's process
* Add more!

## Alright then what's a terminal?

A terminal is just an application that displays the output from the shell. You can have your default terminal, a quake based terminal, terminator, the options are endless!