Big idea: Forget names of files: The 'inode' is the file.  

It is common to think of the file name as the 'actual' file. It's not! Instead consider the inode as the file. The inode holds the meta-information (last accessed, ownership, size) and points to the disk blocks used to hold the file contents.

## So... How do we implement a directory?

A directory is just a mapping of names to inode numbers.
POSIX provides a small set of functions to read the filename and inode number for each entry (see below)

Let's think about what it looks like in the actual file system. Theoretically, directories are just like actual files. The disk blocks will contain _directory entries_ or _dirents_. What that means is that our disk block can look like this

| inode_num | name |
|-----------|------|
| 2043567   | hi.txt |
...

Each directory entry could either be a fixed size, or a variable c-string. It depends on how the particular filesystem implements it at the lower level.

## How can I find the inode number of a file?
From a shell, use `ls` with the `-i` option

```
$ ls -i
12983989 dirlist.c		12984068 sandwich.c
```

From C, call one of the stat functions (introduced below).

## How do I find out meta-information about a file (or directory)?

Use the stat calls. For example, to find out when my 'notes.txt' file was last accessed -

```c
struct stat s;
stat("notes.txt", &s);
printf("Last accessed %s", ctime(&s.st_atime));
```
There are actually three versions of `stat`;

```c
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
```

For example you can use `fstat` to find out the meta-information about a file if you already have an file descriptor associated with that file

```c
FILE *file = fopen("notes.txt", "r");
int fd = fileno(file); /* Just for fun - extract the file descriptor from a C FILE struct */
struct stat s;
fstat(fd, & s);
printf("Last accessed %s", ctime(&s.st_atime));
```

The third call 'lstat' we will discuss when we introduce symbolic links.

In addition to access,creation, and modified times, the stat structure includes the inode number, length of the file and owner information.

```c
struct stat {
    dev_t     st_dev;     /* ID of device containing file */
    ino_t     st_ino;     /* inode number */
    mode_t    st_mode;    /* protection */
    nlink_t   st_nlink;   /* number of hard links */
    uid_t     st_uid;     /* user ID of owner */
    gid_t     st_gid;     /* group ID of owner */
    dev_t     st_rdev;    /* device ID (if special file) */
    off_t     st_size;    /* total size, in bytes */
    blksize_t st_blksize; /* blocksize for file system I/O */
    blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
    time_t    st_atime;   /* time of last access */
    time_t    st_mtime;   /* time of last modification */
    time_t    st_ctime;   /* time of last status change */
};
```

## How do I list the contents of a directory ?

Let's write our own version of 'ls' to list the contents of a directory.

```c
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Usage: %s [directory]\n", *argv);
        exit(0);
    }
    struct dirent *dp;
    DIR *dirp = opendir(argv[1]);
    while ((dp = readdir(dirp)) != NULL) {
        puts(dp->d_name);
    }

    closedir(dirp);
    return 0;
}
```

## How do I read the contents of a directory?

Ans: Use `opendir`, `readdir`, `closedir`  
For example, here's a very simple implementation of `ls` to list the contents of a directory.

```c
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    if(argc ==1) {
        printf("Usage: %s [directory]\n", *argv);
        exit(0);
    }
    struct dirent *dp;
    DIR *dirp = opendir(argv[1]);
    while ((dp = readdir(dirp)) != NULL) {
        printf("%s %lu\n", dp-> d_name, (unsigned long)dp-> d_ino );
    }

    closedir(dirp);
    return 0;
}
```

Note: after a call to fork(), either (XOR) the parent or the child can use readdir(), rewinddir() or seekdir(). If both the parent and the child use the above, behavior is undefined.

## How do I check to see if a file is in the current directory?
For example, to see if a particular directory includes a file (or filename) 'name', we might write the following code. (Hint: Can you spot the bug?)

```c
int exists(char *directory, char *name)  {
    struct dirent *dp;
    DIR *dirp = opendir(directory);
    while ((dp = readdir(dirp)) != NULL) {
        puts(dp->d_name);
        if (!strcmp(dp->d_name, name)) {
 	    return 1; /* Found */
        }
    }
    closedir(dirp);
    return 0; /* Not Found */
}
```

The above code has a subtle bug: It leaks resources! If a matching filename is found then 'closedir' is never called as part of the early return. Any file descriptors opened, and any memory allocated, by opendir are never released. This means eventually the process will run out of resources and an `open` or `opendir` call will fail.

The fix is to ensure we free up resources in every possible code-path. In the above code this means calling `closedir` before `return 1`. Forgetting to release resources is a common C programming bug because there is no support in the C lanaguage to ensure resources are always released with all codepaths.

## What are the gotcha's of using readdir? For example to recursively search directories?
There are two main gotchas and one consideration:
The `readdir` function returns "." (current directory) and ".." (parent directory). If you are looking for sub-directories, you need to explicitly exclude these directories.

For many applications it's reasonable to check the current directory first before recursively searching sub-directories. This can be achieved by storing the results in a linked list, or resetting the directory struct to restart from the beginning.

One final note of caution: `readdir` is not thread-safe! For multi-threaded searches use `readdir_r` which requires the caller to pass in the address of an existing dirent struct.

See the man page of readdir for more details.

## How do I determine if a directory entry is a directory?
Ans: Use `S_ISDIR` to check the mode bits stored in the stat structure

And to check if a file is regular file use `S_ISREG`,

```c
struct stat s;
if (0 == stat(name, &s)) {
    printf("%s ", name);
    if (S_ISDIR( s.st_mode)) puts("is a directory");
    if (S_ISREG( s.st_mode)) puts("is a regular file");
} else {
    perror("stat failed - are you sure I can read this file's meta data?");
}
```
## Does a directory have an inode too?
Yes! Though a better way to think about this, is that a directory (like a file) _is_ an inode (with some data - the directory name and inode contents). It just happens to be a special kind of inode.

From [Wikipedia](http://en.wikipedia.org/wiki/Inode):
> Unix directories are lists of association structures, each of which contains one filename and one inode number.

Remember, inodes don't contain filenames--only other file metadata.

## How can I have the same file appear in two different places in my file system?
First remember that a file name != the file. Think of the inode as 'the file' and a directory as just a list of names with each name mapped to an inode number. Some of those inodes may be regular file inodes, others may be directory inodes.

If we already have a file on a file system we can create another link to the same inode using the 'ln' command

```
$ ln file1.txt blip.txt
```
However blip.txt _is_ the same file; if I edit blip I'm editing the same file as 'file1.txt!'
We can prove this by showing that both file names refer to the same inode:
```
$ ls -i file1.txt blip.txt
134235 file1.txt
134235 blip.txt
```

These kinds of links (aka directory entries) are called 'hard links'

The equivalent C call is `link`
```C
link(const char *path1, const char *path2);

link("file1.txt", "blip.txt");
```

For simplicity the above examples made hard links inside the same directory however hard links can be created anywhere inside the same filesystem.

## What happens when I `rm` (remove) a file?
When you remove a file (using `rm` or `unlink`) you are removing an inode reference from a directory.
However the inode may still be referenced from other directories. In order to determine if the contents of the file are still required, each inode keeps a reference count that is updated whenever a new link is created or destroyed.

## Case study: Back up software that minimizes file duplication
An example use of hard-links is to efficiently create multiple archives of a file system at different points in time. Once the archive area has a copy of a particular file, then future archives can re-use these archive files rather than creating a duplicate file. Apple's "Time Machine" software does this.

## Can I create hard links to directories as well as regular files?
No. Well yes. Not really... Actually you didn't really want to do this, did you?
The POSIX standard says no you may not! The `ln` command will only allow root to do this and only if you provide the `-d` option. However even root may not be able to perform this because most filesystems prevent it! 

## Why does POSIX prevent hard links to directories?

The integrity of the file system assumes the directory structure (excluding softlinks which we will talk about later) is a non-cyclic tree that is reachable from the root directory. It becomes expensive to enforce or verify this constraint if directory linking is allowed. Breaking these assumptions can cause file integrity tools to not be able to repair the file system. Recursive searches potentially never terminate and directories can have more than one parent but ".." can only refer to a single parent. All in all, a bad idea.

---
<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction">
Back: File System, Part 1
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-3:-Permissions">
Next: File System, Part 3
</a>
</div>