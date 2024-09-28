# Solving Critical Sections

## What is a Critical Section?
A critical section is a section of code that can only be executed by one thread at a time, if the program is to function correctly (by correctly,  we mean gives right results, for now). If two threads (or processes) were to execute code inside the critical section at the same time then it is possible that program may no longer have correct behavior.

## Is just incrementing a variable a critical section?
Possibly. Incrementing a variable (`i++`) is performed in three individual steps: Copy the memory contents to the CPU register. Increment the value in the CPU. Store the new value in memory. If the memory location is only accessible by one thread (e.g. automatic variable `i` below) then there is no possibility of a race condition and no Critical Section associated with `i`. However the `sum` variable is a global variable and accessed by two threads. It is possible that two threads may attempt to increment the variable at the same time.
```C
#include <stdio.h>
#include <pthread.h>
// Compile with -pthread
// gcc -o ex1 ex1.c  -lpthread
// ./ex1

int sum = 0; //shared

void *countgold(void *param)
{
    int i; //local to each thread
    for (i = 0; i < 10000000; i++)
    {
        sum += 1;
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, countgold, NULL);
    pthread_create(&tid2, NULL, countgold, NULL);

    //Wait for both threads to finish:
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("ARRRRG sum is %d\n", sum);
    return 0;
}

```
Typical output of the above code is `ARGGGH sum is 8140268`
A different output is printed each time the program is run because there is a race condition; the code does not stop two threads from reading-writing `sum` at the same time. For example both threads copy the current value of `sum` into CPU that runs each thread (let's pick 123). Both threads increment one to their own copy. Both threads write back the value (124). If the threads had accessed the sum at different times then the count would have been 125.
* Can we provide an upper bound  and a lower bound on the output of the above program?

## How do I ensure only one thread at a time can access a global variable?
You mean, "Help - I need a mutex!"
If one thread is currently inside a critical section we would like another thread to wait until the first thread is complete. For this purpose we can use a mutex (short for Mutual Exclusion).

For simple examples the smallest amount of code we need to add is just three lines:
```C
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // global variable
pthread_mutex_lock(&m);   // start of Critical Section
pthread_mutex_unlock(&m); // end of Critical Section
```
Once we are finished with the mutex we should also call `pthread_mutex_destroy(&m)` too. Note, _you can only destroy an unlocked mutex._ Calling destroy on a destroyed lock, initializing an initialized lock, locking an already locked lock, unlocking an unlocked lock etc are unsupported (at least for default mutexes) and usually result in undefined behavior.

## If I lock a mutex, does it stop all other threads?
No, the other threads will continue. It's only when a thread attempts to lock a mutex that is already locked, will the thread have to wait. As soon as the original thread unlocks the mutex, the second (waiting) thread will acquire the lock and be able to continue.

## Are there other ways to create a mutex?
Yes. You can use the macro `PTHREAD_MUTEX_INITIALIZER` only for global ('static') variables.
`m = PTHREAD_MUTEX_INITIALIZER` is equivalent to the more general purpose
`pthread_mutex_init(&m,NULL)`. The init version includes options to _trade performance_ for additional error-checking and advanced sharing options.

```C
pthread_mutex_t *lock = malloc(sizeof(pthread_mutex_t)); 
pthread_mutex_init(lock, NULL);
//later 
// we must have equal number of unlocks and locks in an execution
pthread_mutex_destroy(lock);
free(lock);
```
Things to keep in mind about `init` and `destroy`:
* Multiple threads doing init/destroy has undefined behavior
* Destroying a locked mutex has undefined behavior
* Basically try to keep to the pattern of one thread initializing a mutex and one and only one thread destroying a mutex.

# Mutex Gotchas

## So `pthread_mutex_lock` stops the other threads when they read the same variable?
No. A mutex is not that smart - it works with code (threads), not data. Only when another thread calls `lock` on a locked mutex will the another thread would need to wait until the mutex is unlocked.

Consider
```C
int a;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

// later
// Thread 1
pthread_mutex_lock(&m1);
a++;
pthread_mutex_unlock(&m1);

// Thread 2
pthread_mutex_lock(&m2);
a++;
pthread_mutex_unlock(&m2);
```

Will still cause a race condition.


## Can I create mutex before fork-ing?
Yes - however the child and parent process will not share virtual memory and each one will have a mutex independent of the other.

(Advanced note: There are advanced options using shared memory that allow a child and parent to share a mutex if it's created with the correct options and uses a shared memory segment. See [stackoverflow example](http://stackoverflow.com/questions/19172541/procs-fork-and-mutexes))

## If one thread locks a mutex can another thread unlock it?
No. The same thread must unlock it.

## Can I use two or more mutex locks?
Yes! In fact it's common to have one lock per data structure that you need to update.

If you only have one lock, then they may be significant contention for the lock between two threads that was unnecessary. For example if two threads were updating two different counters, it might not be necessary to use the same lock.
 
However simply creating many locks is insufficient: It's important to be able to reason about critical sections e.g. it's important that one thread can't read two data structures while they are being updated and temporarily in an inconsistent state.

## Is there any overhead in calling lock and unlock?
There is a small amount of overhead of calling `pthread_mutex_lock` and `_unlock`; however this is the price you pay for correctly functioning programs!

## Simplest complete example?
A complete example is shown below
```C
#include <stdio.h>
#include <pthread.h>

// Compile with -pthread
// Create a mutex this ready to be locked!
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int sum = 0;

void *countgold(void *param) {
    int i;
    
    // Same thread that locks the mutex must unlock it
    // Critical section is just 'sum += 1'
    // However locking and unlocking a million times
    // has significant overhead in this simple answer
    
    pthread_mutex_lock(&m);

    // Other threads that call lock will have to wait until we call unlock

    for (i = 0; i < 10000000; i++) {
	sum += 1;
    }
    pthread_mutex_unlock(&m);
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, countgold, NULL);
    pthread_create(&tid2, NULL, countgold, NULL);

    //Wait for both threads to finish:
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("ARRRRG sum is %d\n", sum);
    return 0;
}
```

In the code above, the thread gets the lock to the counting house before entering. The critical section is only the `sum += 1` so the following version is also correct but slower - 
```C
    for (i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&m);
        sum += 1;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}
```
This process runs slower because we lock and unlock the mutex a million times, which is expensive - at least compared with incrementing a variable. (And in this simple example we didn't really need threads - we could have added up twice!)  A faster multi-thread example would be to add one million using an automatic(local) variable and only then adding it to a shared total after the calculation loop has finished:
```C
    int local = 0;
    for (i = 0; i < 10000000; i++) {
       local += 1;
    }

    pthread_mutex_lock(&m);
    sum += local;
    pthread_mutex_unlock(&m);

    return NULL;
}
```

## What happens if I forget to unlock?

Deadlock! We will talk about deadlock a little bit later but what is the problem with this loop if called by multiple threads.

```C
while (not_stop) {
    // stdin may not be thread safe
    pthread_mutex_lock(&m);
    char *line = getline(...);
    if (rand() % 2) { /* randomly skip lines */
         continue;
    }
    pthread_mutex_unlock(&m);
    
    process_line(line);
}
```

## When can I destroy the mutex?
You can only destroy an unlocked mutex.

## Can I copy a pthread_mutex_t to a new memory locaton?
No, copying the bytes of the mutex to a new memory location and then using the copy is _not_ supported.

## What would a simple implementation of a mutex look like?

A simple (but incorrect!) suggestion is shown below. The `unlock` function simply unlocks the mutex and returns. The lock function first checks to see if the lock is already locked. If it is currently locked, it will keep checking again until another thread has unlocked the mutex.
```C
// Version 1 (Incorrect!)
// assume that the `locked` variable is `bool`
// how would this behavior differ on an uni processor machine as compared to a multiprocessor machine? 

void lock(mutex_t *m) {
    while(m->locked) { /*Locked? Never-mind - just loop and check again!*/ }
    // what would be the behaviour if we put another check such as 
    // on a single cpu ?? on multiple cpuS ?? 
    if ( m->locked != 0 ) 
    {
        m->locked = 1;  
    }
    // or 
    // m->locked = true;
}

void unlock(mutex_t *m) {
    m->locked = 0;
    // or
    // m->locked = false;

}
```
Version 1 uses 'busy-waiting' (unnecessarily wasting CPU resources) however there is a more serious problem: We have a race-condition! 

If two threads both called `lock` concurrently it is possible that both threads would read 'm_locked' as zero. Thus both threads would believe they have exclusive access to the lock and both threads will continue. Oops!

What if one of the many threads which actually was able to take the lock, calls `unlock`, and what would be the behavior of the other threads which have been wanting the `lock`?  


We might attempt to reduce the CPU overhead a little by calling, [pthread_yield](http://man7.org/linux/man-pages/man3/pthread_yield.3.html) inside the loop  - pthread_yield suggests to the operating system that the thread does not use the CPU for a short while, so the CPU may be assigned to threads that are waiting to run. But does not fix the race-condition.

Why does it not fix the race-condition?
It's not even an attempt to fix it, it's an attempt to make the code run faster. 

We need a better implementation - can you work how to prevent the race-condition?


## How do I find out more?
[Play!](http://cs-education.github.io/sys) Read the man page!
* [pthread_mutex_lock man page](http://linux.die.net/man/3/pthread_mutex_lock)
* [pthread_mutex_unlock man page](http://linux.die.net/man/3/pthread_mutex_unlock)
* [pthread_mutex_init man page](http://linux.die.net/man/3/pthread_mutex_init)
* [pthread_mutex_destroy man page](http://linux.die.net/man/3/pthread_mutex_destroy)
