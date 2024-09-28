> Warning - question numbers subject to change

## Q1
Is the following code thread-safe? Redesign the following code to be thread-safe. Hint: A mutex is unnecessary if the message memory is unique to each call.

````C
static char message[20];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *format(int v) {
  pthread_mutex_lock(&mutex);
  sprintf(message, ":%d:" ,v);
  pthread_mutex_unlock(&mutex);
  return message;
}
````
## Q2
Which one of the following does not cause a process to exit?
* Returning from the pthread's starting function in the last running thread.
* The original thread returning from main.
* Any thread causing a segmentation fault.
* Any thread calling `exit`.
* Calling `pthread_exit` in the main thread with other threads still running.


## Q3
Write a mathematical expression for the number of "W" characters that will be printed by the following program. Assume a,b,c,d are small positive integers. Your answer may use a 'min' function that returns its lowest valued argument.

````C
unsigned int a=...,b=...,c=...,d=...;

void* func(void* ptr) {
  char m = * (char*)ptr;
  if(m == 'P') sem_post(s);
  if(m == 'W') sem_wait(s);
  putchar(m);
  return NULL;
}

int main(int argv, char** argc) {
  sem_init(s,0, a);
  while(b--) pthread_create(&tid, NULL, func, "W"); 
  while(c--) pthread_create(&tid, NULL, func, "P"); 
  while(d--) pthread_create(&tid, NULL, func, "W"); 
  pthread_exit(NULL); 
  /*Process will finish when all threads have exited */
}
````

## Q4
Complete the following code. The following code is supposed to print alternating `A` and `B`. It represents two threads that take turns to execute.  Add condition variable calls to `func` so that the waiting thread does not need to continually check the `turn` variable. Q: Is pthread_cond_broadcast necessary or is pthread_cond_signal sufficient?
````C
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* turn;

void* func(void* mesg) {
  while(1) {
// Add mutex lock and condition variable calls ...

    while(turn == mesg) { 
        /* poll again ... Change me - This busy loop burns CPU time! */ 
    }

    /* Do stuff on this thread */
    puts( (char*) mesg);
    turn = mesg;
    
  }
  return 0;
}

int main(int argc, char** argv){
  pthread_t tid1;
  pthread_create(&tid1, NULL, func, "A");
  func("B"); // no need to create another thread - just use the main thread
  return 0;
}
````

## Q5
Identify the critical sections in the given code. Add mutex locking to make the code thread safe. Add condition variable calls so that `total` never becomes negative or above 1000. Instead the call should block until it is safe to proceed. Explain why `pthread_cond_broadcast` is necessary.
````C
int total;
void add(int value) {
 if(value < 1) return;
 total += value;
}
void sub(int value) {
 if(value < 1) return;
 total -= value;
}
````

## Q6
A non-threadsafe data structure has `size()` `enq` and `deq` methods. Use condition variable and mutex lock to complete the thread-safe, blocking versions.
````C
void enqueue(void* data) {
  // should block if the size() would become greater than 256
  enq(data);
}
void* dequeue() {
  // should block if size() is 0
  return deq();
}
````

## Q7
Your startup offers path planning using latest traffic information. Your overpaid intern has created a non-threadsafe data structure with two functions: `shortest` (which uses but does not modify the graph) and `set_edge` (which modifies the graph).
````C
graph_t* create_graph(char* filename); // called once

// returns a new heap object that is the shortest path from vertex i to j
path_t* shortest(graph_t* graph, int i, int j); 

// updates edge from vertex i to j
void set_edge(graph_t* graph, int i, int j, double time); 
  
````
For performance, multiple threads must be able to call `shortest` at the same time but the graph can only be modified by one thread when no threads other are executing inside `shortest` or `set_edge`.
 
Use mutex lock and condition variables to implement a reader-writer solution. An incomplete attempt is shown below. Though this attempt is threadsafe (thus sufficient for demo day!), it does not allow multiple threads to calculate `shortest` path at the same time and will not have sufficient throughput.
````C
path_t* shortest_safe(graph_t* graph, int i, int j) {
  pthread_mutex_lock(&m);
  path_t* path = shortest(graph, i, j);
  pthread_mutex_unlock(&m);
  return path;
}
void set_edge_safe(graph_t* graph, int i, int j, double dist) {
  pthread_mutex_lock(&m);
  set_edge(graph, i, j, dist);
  pthread_mutex_unlock(&m);
}
````