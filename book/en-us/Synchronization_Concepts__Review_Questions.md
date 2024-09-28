> Note thread-programming synchronization problems are on a separate wiki page. This page focuses on conceptual topics.
> Question numbers subject to change
 
## Q1
What do each of the Coffman conditions mean? (e.g. can you provide a definition of each one)
* Hold and wait
* Circular wait
* No pre-emption
* Mutual exclusion

## Q2
Give a real life example of breaking each Coffman condition in turn. A situation to consider: Painters, paint and paint brushes.
Hold and wait
Circular wait
No pre-emption
Mutual exclusion

## Q3
Identify when Dining Philosophers code causes a deadlock (or not). For example, if you saw the following code snippet which Coffman condition is not satisfied?

```C
// Get both locks or none.
pthread_mutex_lock(a);
if (pthread_mutex_trylock(b)) { /* failed */
    pthread_mutex_unlock(a);
    ...
}
```

## Q4
How many processes are blocked?

* P1 acquires R1
* P2 acquires R2
* P1 acquires R3
* P2 waits for R3
* P3 acquires R5
* P1 acquires R4
* P3 waits for R1
* P4 waits for R5
* P5 waits for R1

## Q5 
How many of the following statements are true for the reader-writer problem?

* There can be multiple active readers
* There can be multiple active writers
* When there is an active writer the number of active readers must be zero
* If there is an active reader the number of active writers must be zero
* A writer must wait until the current active readers have finished
