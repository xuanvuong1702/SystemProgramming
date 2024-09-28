# Topics
* C Strings representation
* C Strings as pointers
* char p[]vs char* p
* Simple C string functions (strcmp, strcat, strcpy)
* sizeof char
* sizeof x vs x*
* Heap memory lifetime
* Calls to heap allocation
* Deferencing pointers
* Address-of operator
* Pointer arithmetic
* String duplication
* String truncation
* double-free error
* String literals
* Print formatting.
* memory out of bounds errors
* static memory
* fileio POSIX vs. C library
* C io fprintf and printf
* POSIX file IO (read, write, open)
* Buffering of stdout

# Questions/Exercises

* What does the following print out
```C
int main(){
    fprintf(stderr, "Hello ");
    fprintf(stdout, "It's a small ");
    fprintf(stderr, "World\n");
    fprintf(stdout, "place\n");
    return 0;
}
```
* What are the differences between the following two declarations? What does `sizeof` return for one of them?
```C
char str1[] = "another";
char *str2 = "another one";
```
* What is a string in c?
* Code up a simple `my_strcmp`. How about `my_strcat`, `my_strcpy`, or `my_strdup`? Bonus: Code the functions while only going through the strings _once_.
* What should the following usually return?
```C
int *ptr;
sizeof(ptr);
sizeof(*ptr);
```
* What is `malloc`? How is it different than `calloc`. Once memory is `malloc`ed how can I use `realloc`?
* What is the `&` operator? How about `*`?
* Pointer Arithmetic. Assume the following addresses. What are the following shifts?
```C
char** ptr = malloc(10); //0x100
ptr[0] = malloc(20); //0x200
ptr[1] = malloc(20); //0x300
```
     * `ptr + 2`
     * `ptr + 4`
     * `ptr[0] + 4`
     * `ptr[1] + 2000`
     * `*((int)(ptr + 1)) + 3`
* How do we prevent double free errors?
* What is the printf specifier to print a string, `int`, or `char`?
* Is the following code valid? If so, why? Where is `output` located?
```C
char *foo(int var){
    static char output[20];
    snprintf(output, 20, "%d", var);
    return output;
}
```
* Write a function that accepts a string and opens that file prints out the file 40 bytes at a time but every other print reverses the string (try using POSIX API for this).
* What are some differences between the POSIX filedescriptor model and C's `FILE*` (ie what function calls are used and which is buffered)? Does POSIX use C's `FILE*` internally or vice versa?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Back: C Programming, Part 5: Debugging
</a>
</div>
