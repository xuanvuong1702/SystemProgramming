> Question numbers subject to change

## Q1
Fill in the blanks to make the following program print 123456789. If `cat` is given no arguments it simply prints its input until EOF. Bonus: Explain why the `close` call below is necessary.
````C
int main() {
    int i = 0;
    while (++i < 10) {
        pid_t pid = fork();
        if (pid == 0) { /* child */
            char buffer[16];
            sprintf(buffer, ______, i);
            int fds[______];
            pipe(fds);
            write(fds[1], ______, ______); // Write the buffer into the pipe
            close(______);
            dup2(fds[0], ______);
            execlp("cat", "cat",  ______);
            perror("exec"); exit(1);
        }
        waitpid(pid, NULL, 0);
    }
    return 0;
}
````

## Q2
Use POSIX calls `fork` `pipe` `dup2` and `close` to implement an autograding program. Capture the standard output of a child process into a pipe. The child process should `exec` the program `./test` with no additional arguments (other than the process name). In the parent process read from the pipe: Exit the parent process as soon as the captured output contains the ! character. Before exiting the parent process send SIGKILL to the child process. Exit 0 if the output contained a !. Otherwise if the child process exits causing the pipe write end to be closed, then exit with a value of 1. Be sure to close the unused ends of the pipe in the parent and child process

## Q3 (Advanced)
This advanced challenge uses pipes to get an "AI player" to play itself until the game is complete.
The program `tictactoe` accepts a line of input - the sequence of turns made so far, prints the same sequence followed by another turn, and then exits. A turn is specified using two characters. For example "A1" and "C3" are two opposite corner positions. The string `B2A1A3` is a game of 3 turns/plys. A valid response is `B2A1A3C1` (the C1 response blocks the diagonal B2 A3 threat). The output line may also include a suffix `-I win` `-You win` `-invalid` or `-draw`
Use pipes to control the input and output of each child process created. When the output contains a `-`, print the final output line (the entire game sequence and the result) and exit.

