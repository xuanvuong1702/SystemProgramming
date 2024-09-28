## What is a counting semaphore?
A counting semaphore contains a value[ non negative ]  and supports two operations "wait" and "post". Post increments the semaphore and immediately returns. "wait" will _wait_ if the count is zero. If the count is non-zero the _wait_ call decrements the count and immediately returns.

Why are they names as "wait"& "post"? 

An analogy is a count of the cookies in a cookie jar (or gold coins in the treasure chest). Before taking a cookie, call 'wait'. If there are no cookies left then `wait` will not return: It will `wait` until another thread increments the semaphore by calling post.

In short, `post` increments and immediately returns whereas `wait` will wait if the count is zero. Before returning it will decrement count, meaning, there is one lesser cookie.

## How do I create a semaphore?
This page introduces unnamed semaphores. Unfortunately Mac OS X does not support these yet.

First decide if the initial value should be zero or some other value (e.g. the number of remaining spaces in an array).
Unlike pthread mutex there are not shortcuts to creating a semaphore - use `sem_init`
```C
#include <semaphore.h>

sem_t s;
int main() {
  sem_init(&s, 0, 10); // max cookies are ten // returns -1 (=FAILED) on OS X 
  sem_wait(&s); // Could do this 10 times without blocking
  sem_post(&s); // Announce that we've finished (and one more resource item is available; increment count)
  sem_destroy(&s); // release resources of the semaphore
}
```

## Can I call wait and post from different threads?
Yes! Unlike a mutex, the increment and decrement can be from different threads.
Can there non global semaphores? What would be their use?

## Can I use a semaphore instead of a mutex?
Yes - though the overhead of a semaphore is greater. To use a semaphore:
* Initialize the semaphore with a count of one. [ is the the max value or the initial value? ] 
* Replace `...lock` with `sem_wait`
* Replace `...unlock` with `sem_post`

Why should the overhead of semaphore operations be higher? 

A mutex is an _initialized_ semaphore that always `waits` before it `posts`
If one does the opposite, what situation would one fall in? Deadlocks? Race conditions? 

```C
sem_t s;
sem_init(&s, 0, 1);

sem_wait(&s);
// Critical Section
sem_post(&s);
```

## Can I use sem_post inside a signal handler?
## Can I increase the count of the counter in semaphore inside a signal handler?
Yes! `sem_post` is one of a handful of functions that can be correctly used inside a signal handler.
_This means we can release a waiting thread which can now make all of the calls that we were not_
_allowed to call inside the signal handler itself (e.g. `printf`)._ ??????

```C
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

sem_t s;

void handler(int signal)
{
    sem_post(&s); /* Release the Kraken! */
}

void *singsong(void *param)
{
    sem_wait(&s); // see the value of semaphore  value which was used to initialize it
    printf("I had to wait until your signal released me!\n");
}

int main()
{
    int ok = sem_init(&s, 0, 0 /* Initial value of zero*/); 
    if (ok == -1) {
       perror("Could not create unnamed semaphore");
       return 1;
    }
    signal(SIGINT, handler); // Too simple! See note below

    pthread_t tid;
    pthread_create(&tid, NULL, singsong, NULL);
    pthread_exit(NULL); /* Process will exit when there are no more threads */
}
```
Note robust programs do not use `signal()` in a multi-threaded program ("The effects of signal() in a multithreaded process are unspecified." - the signal man page); a more correct program will need to use `sigaction`.

## How do I find out more?
Read the man pages:
* [sem_init](http://man7.org/linux/man-pages/man3/sem_init.3.html)
* [sem_wait](http://man7.org/linux/man-pages/man3/sem_wait.3.html)
* [sem_post](http://man7.org/linux/man-pages/man3/sem_post.3.html)
* [sem_destroy](http://man7.org/linux/man-pages/man3/sem_destroy.3.html)
