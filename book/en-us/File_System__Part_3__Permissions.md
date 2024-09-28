# Remind Me What do Permissions mean again?

Every file and directory has a set of 9 permission bits and a type field
* r, permission to read the file
* w, permission to write to the file
* x, permission to execute the file

chmod 777 <file>

|chmod | 7 |  7  |  7  |
|----|-----|-----|-----|
| 01 | 111 | 111 | 111 |
| d | rwx | rwx | rwx |
| 1 | 2   |  3  |  4  |

1. Type of the file<br />
2. Owner Permissions<br />
3. Group Permissions<br />
4. Everybody else's permission<br />

`mknod` changes the first field, the type of the file.
`chmod` takes a number and a file and changes the permission bits.

The file has an owner. If your process has the same user id as the owner (or root) then the permissions in the first triad apply to you. If you are in the same group as the file (all files are also owned by a group) then the next set of permission bits applies to you. If none of the above apply, the last triad applies to you.

## How do I change the permissions on a file?
Use `chmod`  (short for "change the file mode bits")

There is a system call, `int chmod(const char *path, mode_t mode);` but we will concentrate on the shell command. There's two common ways to use `chmod` ; with an octal value or with a symbolic string:
```
$ chmod 644 file1
$ chmod 755 file2
$ chmod 700 file3
$ chmod ugo-w file4
$ chmod o-rx file4
```
The base-8 ('octal') digits describe the permissions for each role: The user who owns the file, the group and everyone else. The octal number is the sum of three values given to the three types of permission: read(4), write(2), execute(1)

Example: chmod 755 myfile
* r + w + x = digit
* user has 4+2+1, full permission
* group has 4+0+1, read and execute permission
* all users have 4+0+1, read and execute permission

## How do I read the permission string from ls?
Use `ls -l'. 
Note that the permissions will output in the format 'drwxrwxrwx'. The first character indicates the type of file type. 
Possible values for the first character:
* (-) regular file
* (d) directory
* (c) character device file
* (l) symbolic link
* (p) pipe
* (b) block device
* (s) socket

## What is sudo?
Use `sudo` to become the admin on the machine.
e.g. Normally (unless explicitly specified in the '/etc/fstab' file, you need root access to mount a filesystem). `sudo` can be used to temporarily run a command as root (provided the user has sudo privileges)

```
$ sudo mount /dev/sda2 /stuff/mydisk
$ sudo adduser fred
```

## How do I change ownership of a file?
Use `chown username filename`

## How do I set permissions from code?

`chmod(const char *path, mode_t mode);`

## Why are some files 'setuid'? What does this mean?
The set-user-ID-on-execution bit changes the user associated with the process when the file is run. This is typically used for commands that need to run as root but are executed by non-root users. An example of this is `sudo`

The set-group-ID-on-execution changes the group under which the process is run.

## Why are they useful?
The most common usecase is so that the user can have root(admin) access for the duration of the program.

## What permissions does sudo run as ?
```
$ ls -l /usr/bin/sudo
-r-s--x--x  1 root  wheel  327920 Oct 24 09:04 /usr/bin/sudo
```
The 's' bit means execute and set-uid; the effective userid of the process will be different from the parent process. In this example it will be root

## What's the difference between getuid() and geteuid()?
* `getuid` returns the real user id (zero if logged in as root)
* `geteuid` returns the effective userid (zero if acting as root, e.g. due to the setuid flag set on a program)

## How do I ensure only privileged users can run my code?
* Check the effective permissions of the user by calling `geteuid()`. A return value of zero means the program is running effectively as root.

[Go to File System: Part 4](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-4:-Working-with-directories)