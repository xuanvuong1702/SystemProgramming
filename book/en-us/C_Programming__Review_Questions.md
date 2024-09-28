## Warning - question numbers subject to change
# Memory and Strings
## Q1.1
In the example below, which variables are guaranteed to print the value of zero?
````C
int a;
static int b;

void func() {
   static int c;
   int d;
   printf("%d %d %d %d\n",a,b,c,d);
}
````
## Q 1.2
In the example below, which variables are guaranteed to print the value of zero?
````C
void func() {
   int* ptr1 = malloc( sizeof(int) );
   int* ptr2 = realloc(NULL, sizeof(int) );
   int* ptr3 = calloc( 1, sizeof(int) );
   int* ptr4 = calloc( sizeof(int) , 1);
   
   printf("%d %d %d %d\n",*ptr1,*ptr2,*ptr3,*ptr4);
}
````
## Q 1.3
Explain the error in the following attempt to copy a string.
````C
char* copy(char*src) {
 char*result = malloc( strlen(src) ); 
 strcpy(result, src); 
 return result;
}
````
## Q 1.4
Why does the following attempt to copy a string sometimes work and sometimes fail?

````C
char* copy(char*src) {
 char*result = malloc( strlen(src) +1 ); 
 strcat(result, src); 
 return result;
}
````
## Q 1.4
Explain the two errors in the following code that attempts to copy a string.
````C
char* copy(char*src) {
 char result[sizeof(src)]; 
 strcpy(result, src); 
 return result;
}
````
## Q 1.5
Which of the following is legal?
````C
char a[] = "Hello"; strcpy(a, "World");
char b[] = "Hello"; strcpy(b, "World12345", b);
char* c = "Hello"; strcpy(c, "World");
````

## Q 1.6
Complete the function pointer typedef to declare a pointer to a function that takes a void* argument and returns a void*. Name your type 'pthread_callback'
````C
typedef ______________________;
````
## Q 1.7
In addition to the function arguments what else is stored on a thread's stack?

## Q 1.8
Implement a version of `char* strcat(char*dest, const char*src)` using only `strcpy`  `strlen` and pointer arithmetic
````C
char* mystrcat(char*dest, const char*src) {

  ? Use strcpy strlen here

  return dest;
}
````
## Q 1.9
Implement version of size_t strlen(const char*) using a loop and no function calls.
````C
size_t mystrlen(const char*s) {

}
````
## Q 1.10
Identify the three bugs in the following implementation of `strcpy`.
````C
char* strcpy(const char* dest, const char* src) {
  while(*src) { *dest++ = *src++; }
  return dest;
}
````



# Printing
## Q 2.1
Spot the two errors!
````
fprintf("You scored 100%");
````
# Formatting and Printing to a file
## Q 3.1
Complete the following code to print to a file. Print the name, a comma and the score to the file 'result.txt'
````C
char* name = .....;
int score = ......
FILE *f = fopen("result.txt",_____);
if(f) {
    _____
}
fclose(f);
````
# Printing to a string
## Q 4.1

How would you print the values of variables a,mesg,val and ptr to a string? Print a as an integer, mesg as C string, val as a double val and ptr as a hexadecimal pointer. You may assume the mesg points to a short C string(<50 characters).
Bonus: How would you make this code more robust or able to cope with?
```C
char* toString(int a, char*mesg, double val, void* ptr) {
   char* result = malloc( strlen(mesg) + 50);
    _____
   return result;
}
```

# Input parsing
## Q 5.1
Why should you check the return value of sscanf and scanf?
## Q 5.2
Why is 'gets' dangerous?

## Q 5.3
Write a complete program that uses `getline`. Ensure your program has no memory leaks.

## Heap memory
When would you use calloc not malloc? 
When would realloc be useful?

(Todo - move this question to another page)
What mistake did the programmer make in the following code? Is it possible to fix it i) using heap memory? ii) using global (static) memory?
```C
static int id;

char* next_ticket() {
  id ++;
  char result[20];
  sprintf(result,"%d",id);
  return result;
}
```