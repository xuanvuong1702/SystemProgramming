> Question numbers subject to change

## Q1
Write a function that uses fseek and ftell to replace the middle character of a file with an 'X'
````C
void xout(char* filename) {
  FILE *f = fopen(filename, ____ );
  


}
````
## Q2
In an `ext2` filesystem how many inodes are read from disk to access the first byte of the file `/dir1/subdirA/notes.txt` ? Assume the directory names and inode numbers in the root directory (but not the inodes themselves) are already in memory.

## Q3
In an `ext2` filesystem what is the minimum number of disk blocks that must be read from disk to access the first byte of the file `/dir1/subdirA/notes.txt` ? Assume the directory names and inode numbers in the root directory and all inodes are already in memory.

## Q4
In an `ext2` filesystem with 32 bit addresses and 4KB disk blocks, an inodes that can store 10 direct disk block numbers. What is the minimum file size required to require an single indirection table? ii) a double direction table?

## Q5
Fix the shell command `chmod` below to set the permission of a file `secret.txt`  so that the owner can read,write,and execute permissions the group can read and everyone else has no access.
````
chmod 000 secret.txt
```