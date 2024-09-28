# Backstory
<img src="https://upload.wikimedia.org/wikipedia/commons/7/7b/An_illustration_of_the_dining_philosophers_problem.png" height="500px" width="500px">

So you have your philosophers sitting around a table all wanting to eat some pasta (or whatever that is) and they are really hungry. Each of the philosophers are essentially the same, meaning that each philosopher has the same instruction set based on the other philosopher (ie; you can't tell every even-numbered philosopher to do one thing and every odd-numbered philosopher to do another thing).

# Failed Solutions
## Left-Right Deadlock
What do we do? Let's try a simple solution

````C
void* philosopher(void* forks){
     info phil_info = forks;
     pthread_mutex_t* left_fork = phil_info->left_fork;
     pthread_mutex_t* right_fork = phil_info->right_fork;
     while(phil_info->simulation){
          pthread_mutex_lock(left_fork);
          pthread_mutex_lock(right_fork);
          eat(left_fork, right_fork);
          pthread_mutex_unlock(left_fork);
          pthread_mutex_unlock(right_fork);
     }
}
````

But this runs into a problem! What if everyone picks up their left fork and is waiting on their right fork? We have deadlocked the program. It is important to note that deadlock doesn't happen all the time and the probability that this solution deadlocks goes down as the number of philosophers goes up. What is really important to note is that eventually that this solution will deadlock, letting threads starve which is bad.

## Trylock? More like livelock
So now you are thinking about breaking one of the coffman conditions. We have
- Mutual Exclusion
- No Preemption
- Hold and wait
- Circular Wait

Well we can't have two philosophers use a fork at the same time, mutual exclusion is out of the picture. In our current, simple model, we can't have the philosopher let go of the mutex lock once he/she has a hold of it, so we will take this solution out right now -- there are some notes at the bottom of the page about this solution. Let's break Hold and Wait!
````C
void* philosopher(void* forks){
     info phil_info = forks;
     pthread_mutex_t* left_fork = phil_info->left_fork;
     pthread_mutex_t* right_fork = phil_info->right_fork;
     while(phil_info->simulation){
          pthread_mutex_lock(left_fork);
          int failed = pthread_mutex_trylock(right_fork);
          if(!failed){
               eat(left_fork, right_fork);
               pthread_mutex_unlock(right_fork);
          }
          pthread_mutex_unlock(left_fork);
     }
}
````

Now our philosopher picks up the left fork and tries to grab the right. If it's available, they eat. If it's not available, they put the left fork down and try again. No deadlock!

But, there is a problem. What if all the philosophers pick up their left at the same time, try to grab their right, put their left down, pick up their left, try to grab their right.... We have now livelocked our solution! Our poor philosopher are still starving, so let's give them some proper solutions.

# Viable Solutions

## Arbitrator (Naive and Advanced).

The naive arbitrator solution is have one arbitrator (a mutex for example). Have each of the philosopher ask the arbitrator for permission to eat. This solution allows one philosopher to eat at a time. When they are done, another philosopher can ask for permission to eat.

This prevents deadlock because there is no circular wait! No philosopher has to wait on any other philosopher.

The advanced arbitrator solution is to implement a class that determines if the philosopher's forks are in the arbitrator's possession. If they are, they give them to the philosopher, let him eat, and take the forks back. This has the added bonus of being able to have multiple philosopher eat at the same time.

### Problems:
- These solutions are slow
- They have a single point of failure - the arbitrator - which can cause a bottleneck
- The arbitrator needs to also be fair, and be able to determine the deadlock in the second solution
- In practical systems, the arbitrator tends to give the forks repeatedly to philosophers that just ate because of process scheduling

## Leaving the Table (Stallings' Solution)
Why does the first solution deadlock? Well there are n philosophers and n chopsticks. What if there is only 1 philsopher at the table? Can we deadlock? No. 

How about 2 philsophers? 3? ... You can see where this is going. Stallings' solutions says to remove philosophers from the table until deadlock is not possible -- think about what the magic number of philosophers at the table is. The way to do this in actual system is through semaphores and letting a certain number of philosopher through.

### Problems:
- The solution requires a lot of context switching which is very expensive for the CPU
- You need to know about the number of resources before hand in order to only let that number of philosophers
- Again priority is given to the processes who have already eaten.

## Partial Ordering (Dijkstra's Solution)
This is Dijkstra's solution (he was the one to propose this problem on an exam). Why does the first solution deadlock? Dijkstra thought that the last philosopher who picks up his left fork (causing the solution to deadlock) should pick up his right. He accomplishes this by numbering the forks 1...n, and tells each of the philosopher to pick up his lower number fork.

Let's run through the deadlock condition again. Everyone tries to pick up their lower number fork first. Philosopher 1 gets fork 1, Philosopher 2 gets fork 2, and so on until we get to Philosopher n. They have to choose between fork 1 and n. fork 1 is already held up by philosopher 1, so they can't pick up that fork, meaning he won't pick up fork n. We have broken circular wait! Meaning deadlock isn't possible.

### Problems:
- The philosopher needs to know the set of resources in order before grabbing any resources.
- You need to define a partial order to all of the resources.
- Prioritizes philosopher who have already eaten.

## Advanced Solutions

There are many more advanced solutions a non-exhaustive list includes
- Clean/Dirty Forks (Chandra/Misra Solution)
- Actor Model (other Message passing models)
- Super Arbitrators (Complicated pipelines)

