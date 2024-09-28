## How does the operating system load my process and libraries into memory?
It maps the executable and library file contents into the address space of the process.
If many programs only need read-access to the same file (e.g. /bin/bash, the C library) then the same physical memory can be shared between multiple processes.

The same mechanism can be used by programs to directly map files into memory.

## How do I map a file into memory?
A simple program to map a file into memory is shown below. The key points to notice are:
* `mmap` requires a file descriptor, so we need to `open` the file first
* `mmap` only works on `lseek`able file descriptors, i.e. "true" files, not pipes or sockets
* We seek to our desired size and write one byte to ensure that the file is sufficient length (failing to do so causes your program to receive SIGBUS upon trying to access the file). `ftruncate` would also work.
* When finished, we call `munmap` to unmap the file from memory

This example also shows the preprocessor constants "__LINE__" and "__FILE__" that hold the current line number and filename of the file currently being compiled.
```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int fail(char *filename, int linenumber) { 
  fprintf(stderr, "%s:%d %s\n", filename, linenumber, strerror(errno)); 
  exit(1);
  return 0; /*Make compiler happy */
}
#define QUIT fail(__FILE__, __LINE__ )

int main() {
  // We want a file big enough to hold 10 integers, 
  // though the mapping is actually rounded up to a multiple of the page size (4kB on x86 Linux)
  int size = sizeof(int) * 10;

  int fd = open("data", O_RDWR | O_CREAT | O_TRUNC, 0600); //6 = read+write for me!

  lseek(fd, size, SEEK_SET);
  write(fd, "A", 1);
  
  void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("Mapped at %p\n", addr);
  if (addr == (void*) -1 ) QUIT;
  
  int *array = addr;
  array[0] = 0x12345678;
  array[1] = 0xdeadc0de;

  munmap(addr, size);
  close(fd);
  return 0;
  
}
```
The contents of our binary file can be listed using hexdump
```
$ hexdump data
0000000 78 56 34 12 de c0 ad de 00 00 00 00 00 00 00 00
0000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0000020 00 00 00 00 00 00 00 00 41   
```
The careful reader may notice that our integers were written in least-significant-byte format (because that is the endianness of the CPU) and that we allocated a file that is one byte too many!

The `PROT_READ | PROT_WRITE` options specify the virtual memory protection. The option `PROT_EXEC` (not used here) can be set to allow CPU execution of instructions in memory (e.g. this would be useful if you mapped an executable or library).

## Difference between `read` + `write` and `mmap`

On Linux, there are more similarities between these approaches than differences. In both cases, the file is actually loaded into the disk cache before reads or writes occur, and writes are first performed directly on pages in the disk cache rather than on the hard disk. The operating system will synchronize the disk cache with disk eventually; you can use `sync` (or its variants) to request that it do so immediately.

However, `mmap` allows your process to directly access the physical memory stored in the page cache. It does this by mapping the process page table entries to the same page frames found in the disk cache. `read`, on the other hand, must first load data from disk to the page cache, _and then copy over data_ from the page cache to a user-space buffer (`write` does the same, but in the opposite direction).

On the downside, creating this mapping between the process address space and the page cache requires the process to create many new page tables and leads to additional (minor) page faults upon first-time access of a file page.

## What are the advantages of memory mapping a file

For many applications the main advantages are:  
- Simplified coding - the file data is immediately available as if it were all in main memory. No need to parse the incoming data in chunks and store it in new memory structures like buffers.
- Sharing of files - memory mapped files are particularly convenient when the same data is shared between multiple processes.
- Lower memory pressure - when resources run tight, the kernel can easily drop data from the disk cache out of main memory and into backing disk storage (clean pages don't even need to be written back to disk). On the other hand, loading large files into buffers will eventually force the kernel to store those pages (even clean ones) in swap space, which may run out.

Note for simple sequential processing, memory mapped files are not necessarily faster than standard 'stream-based' approaches of `read`, `fscanf`, etc., due primarily to minor page faults and the fact that loading data from disk to the page cache is a much tighter performance bottleneck than copying from one buffer to another.

## How do I share memory between a parent and child process?

Easy -  Use `mmap` without a file - just specify the `MAP_ANONYMOUS` and `MAP_SHARED` options!

```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
  
  int size = 100 * sizeof(int);  
  void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  printf("Mapped at %p\n", addr);
  
  int *shared = addr;
  pid_t mychild = fork();
  if (mychild > 0) {
    shared[0] = 10;
    shared[1] = 20;
  } else {
    sleep(1); // We will talk about synchronization later
    printf("%d\n", shared[1] + shared[0]);
  }

  munmap(addr,size);
  return 0;
}
```

## Can I use shared memory for IPC ?

Yes! As a simple example you could reserve just a few bytes and change the value in shared memory when you want the child process to quit. Sharing anonymous memory is a very efficient form of inter-process communication because there is no copying, system call, or disk-access overhead - the two processes literally share the same _physical_ frame of main memory.

On the other hand, shared memory, like multithreading, creates room for data races. Processes that share writable memory might need to use synchronization primitives like mutexes to prevent these from happening.

[Go to File System: Part 7](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-7:-Scalable-and-Reliable-Filesystems)