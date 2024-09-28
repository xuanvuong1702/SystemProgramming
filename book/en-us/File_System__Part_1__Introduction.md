# Navigation/Terminology

## Design a file system! What are your design goals?
The design of a file system is difficult problem because there many high-level design goals that we'd like to satisfy. An incomplete list of ideal goals include:

* Reliable and robust (even with hardware failures or incomplete writes due to power loss)
* Access (security) controls
* Accounting and quotas
* Indexing and search
* Versioning and backup capabilities
* Encryption
* Automatic compression
* High performance (e.g. Caching in-memory)
* Efficient use of storage de-duplication

Not all filesystems natively support all of these goals. For example, many filesystems do not automatically compress rarely-used files

## What are `.`, `..`, and `...`?
In standard unix file systems: 
* `.` represents the current directory  
* `..` represents the parent directory  
* `...` is NOT a valid representation of any directory (this not the grandparent directory). It _could_ however be the name of a file on disk.

## What are absolute and relative paths?
Absolute paths are paths that start from the 'root node' of your directory tree. Relative paths are paths that start from your current position in the tree.

## What are some examples of relative and absolute paths?
If you start in your home directory ("~" for short), then `Desktop/cs241` would be a relative path. Its absolute path counterpart might be something like `/Users/[yourname]/Desktop/cs241`.

## How do I simplify `a/b/../c/./`?
Remember that `..` means 'parent folder' and that `.` means 'current folder'.

Example: `a/b/../c/./`
- Step 1: `cd a` (in a)
- Step 2: `cd b` (in a/b)
- Step 3: `cd ..` (in a, because .. represents 'parent folder')
- Step 4: `cd c` (in a/c)
- Step 5: `cd .` (in a/c, because . represents 'current folder')

Thus, this path can be simplified to `a/c`.

# So what's a File System?

A filesystem is how information is organized on disk. Whenever you want to access a file, the filesystem dictates how the file is read. Here is a sample image of a filesystem.

![](http://tinf2.vub.ac.be/~dvermeir/manual/uintro/disk.gif)

Whoa that's a lot let's break it down
* Superblock: This block contains metadata about the filesystem, how large, last modified time, a journal, number of inodes and the first inode start, number of data block and the first data block start.
* Inode: This is the key abstraction. An inode is a file. 
* Disk Blocks: These are where the data is stored. The actual contents of the file

## How does inode store the file contents?
![](https://classes.soe.ucsc.edu/cmps111/Fall08/inode_with_signatures.jpg)

From [Wikipedia](http://en.wikipedia.org/wiki/Inode):

> *In a Unix-style file system, an index node, informally referred to as an inode, is a data structure used to represent a filesystem object, which can be one of various things including a file or a directory. Each inode stores the attributes and disk block location(s) of the filesystem object's data. Filesystem object attributes may include manipulation metadata (e.g. change, access, modify time), as well as owner and permission data (e.g. group-id, user-id, permissions).*

To read the first few bytes of the file, follow the first direct block pointer to the first direct block and read the first few bytes, writing is the same process. If you want to read the entire file, keep reading direct blocks until your size runs out (we will talk about indirect blocks in a bit)

> "All problems in computer science can be solved by another level of indirection." - David Wheeler

## Why make disk blocks the same size as memory pages?
To support virtual memory, so we can page stuff in and out of memory.

## What information do we want to store for each file?
* Filename
* File size
* Time created, last modified, last accessed
* Permissions
* Filepath
* Checksum
* File data (inode)

## What are the traditional permissions: user – group – other permissions for a file?
Some common file permissions include:
* 755: `rwx r-x r-x`

user: `rwx`, group: `r-x`, others: `r-x`

User can read, write and execute. Group and others can only read and execute.
* 644: `rw- r-- r--`

user: `rw-`, group: `r--`, others: `r--`

User can read and write. Group and others can only read.

## What are the the 3 permission bits for a regular file for each role?
* Read (most significant bit)  
* Write (2nd bit)  
* Execute (least significant bit)

## What do "644" "755" mean?
These are examples of permissions in octal format (base 8). Each octal digit corresponds to a different role (user, group, world).

We can read permissions in octal format as follows:  
* 644 - R/W user permissions, R group permissions, R world permissions  
* 755 - R/W/X user permissions, R/X group permissions, R/X world permissions

## How many pointers can you store in each indirection table? 
As a worked example, suppose we divide the disk into 4KB blocks and we want to address up to 2^32 blocks.

The maximum disk size is 4KB *2^32 = 16TB  (remember 2^10 = 1024)

A disk block can store 4KB / 4B (each pointer needs to be 32 bits) = 1024 pointers. Each pointer refers to a 4KB disk block - so you can refer up to 1024*4KB = 4MB of data

For the same disk configuration, a double indirect block stores 1024 pointers to 1024 indirection tables. Thus a double-indirect block can refer up to 1024 * 4MB = 4GB of data.

Similarly, a triple indirect block can refer up to 4TB of data.

[Go to File System: Part 2](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-2:-Files-are-inodes)