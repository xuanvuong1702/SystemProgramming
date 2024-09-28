# Topics
Coffman Conditions
Resource Allocation Graphs
Dining Philosophers
* Failed DP Solutions
* Livelocking DP Solutions
* Working DP Solutions: Benefits/Drawbacks

# Questions
* What are the Coffman Conditions?
* What do each of the Coffman conditions mean? (e.g. can you provide a definition of each one)
* Give a real life example of breaking each Coffman condition in turn. A situation to consider: Painters, Paint, Paintbrushes etc. How would you assure that work would get done?
* Be able to identify when Dining Philosophers code causes a deadlock (or not).
For example, if you saw the following code snippet which Coffman condition is not satisfied?
```C
// Get both locks or none.
pthread_mutex_lock( a );
if( pthread_mutex_trylock( b ) ) { /*failed*/
   pthread_mutex_unlock( a );
   ...
}
```


* If one thread calls
```C
  pthread_mutex_lock(m1) // success
  pthread_mutex_lock(m2) // blocks
```
and another threads calls
```C
  pthread_mutex_lock(m2) // success
  pthread_mutex_lock(m1) // blocks
```
What happens and why? What happens if a third thread calls `pthread_mutex_lock(m1)` ?

* How many processes are blocked? As usual assume that a process is able to complete if it is able to acquire all of the resources listed below.
     * P1 acquires R1
     * P2 acquires R2
     * P1 acquires R3
     * P2 waits for R3
     * P3 acquires R5
     * P1 acquires  R4
     * P3 waits for R1
     * P4 waits for R5
     * P5 waits for R1

(Draw out the resource graph!)