---
title: "Chương 01: Giới thiệu ngôn ngữ lập trình C"
type: book-vi-vn
order: 3
---


Những lỗi phổ biến mà lập trình viên C thường mắc phải?

# Lỗi liên quan đến bộ nhớ

## Hằng chuỗi chỉ có thể đọc
```C
char array[] = "Hi!"; // mảng chứa một bản sao có thể thay đổi
strcpy(array, "OK");

char *ptr = "Can't change me"; // ptr trỏ đến một số bộ nhớ không thể thay đổi
strcpy(ptr, "Will not work"); // sẽ không hoạt động
```
Hằng chuỗi là các mảng ký tự được lưu trữ trong phân đoạn mã của chương trình, không thể thay đổi. Hai hằng chuỗi có thể chia sẻ cùng một không gian trong bộ nhớ. Dưới đây là một ví dụ:

```C
char *str1 = "Brandon Chong là TA tốt nhất";
char *str2 = "Brandon Chong là TA tốt nhất";
str1 == str2; // đúng
```
Các chuỗi mà `str1` và `str2` trỏ đến có thể thực sự nằm ở cùng một vị trí trong bộ nhớ.

Tuy nhiên, mảng ký tự chứa các bản sao sâu của chuỗi ký tự được gán cho chúng, và các mảng khác nhau đại diện cho các vị trí bộ nhớ khác nhau. Những mảng ký tự sau đây không nằm ở cùng một nơi trong bộ nhớ.

```C
char arr1[] = "Brandon Chong không viết cái này";
char arr2[] = "Brandon Chong không viết cái này";
arr1 == arr2;         // sai
&arr1[0] == &arr2[0]; // sai
```
## Tràn bộ đệm / Dưới bộ đệm
```C
int i = 10, array[10];
for (; i >= 0; i--) array[i] = i;
```
C không thực hiện kiểm tra giới hạn khi truy cập mảng. Ví dụ trên viết vào `array[10]` nằm ngoài giới hạn của mảng. Điều này có thể làm hỏng các biến khác trên stack cũng như cách thực hiện của stack gọi, khiến chương trình của bạn dễ bị tấn công từ hacker. Trên thực tế, những tràn này thường xuất phát từ việc sử dụng các lệnh thư viện không an toàn hoặc đặt giới hạn kích thước sai trên một lệnh thư viện an toàn hơn.

```C
gets(array); // Hy vọng đầu vào ngắn hơn mảng của tôi! (KHÔNG BAO GIỜ sử dụng gets)
fgets(array, 4096, stdin); // Ôi chao
```
## Xử lý con trỏ đến các biến tự động nằm ngoài phạm vi
```C
int *f() {
        int result = 42;
        static int imok;
        int *p;
        {
            int x = result;
            p = &x;
        }
        //imok = *p;      // Không ổn: x đã ra khỏi phạm vi
        //return &result; // Không ổn: result sẽ ra khỏi phạm vi sau khi hàm trả về 
        return &imok;     // Ổn - các biến static không nằm trên stack
}
```
Các biến tự động chỉ được gắn với bộ nhớ stack miễn là chúng đang trong phạm vi. Sau khi chúng ra khỏi phạm vi, dữ liệu được lưu trữ tại địa chỉ bộ nhớ của chúng trở nên không xác định. Các biến static nằm trong phân đoạn dữ liệu, có thể truy cập an toàn ngay cả khi các biến đó không nằm trong phạm vi.

## `sizeof(type *)` so với `sizeof(type)` 
```C
struct User {
  char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t *));
```
Trong ví dụ trên, chúng ta cần phân bổ đủ byte cho struct. Thay vào đó, chúng ta đã phân bổ đủ byte để giữ một con trỏ. Việc ghi vào con trỏ người dùng có thể làm hỏng heap. Mã đúng được hiển thị bên dưới.
```C
struct User {
  char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t));
```

## Strings Require `strlen(s)+1` Bytes

Every string must have a null byte after the last actual character. To store the string <code>"Hi"</code> it takes 3 bytes: <code>[H] [i] [\0]</code>.

```C
char *strdup(const char *input) {     /* return a copy of 'input' */
    char *copy;
    copy = malloc(sizeof (char *));   /* nope! this allocates space for a pointer, not a string */
    copy = malloc(strlen(input));     /* Almost...but what about the null terminator? */
    copy = malloc(strlen(input) + 1); /* That's right. */
    strcpy(copy, input);              /* strcpy will provide the null terminator */
    return copy;
}
```

## Failing to Initialize Memory
```C
void myfunct() {
    char array[10];
    char *p = malloc(10);
    printf("%s %s\n", array, p);
}
```
Automatic (stack) variables and heap memory obtained with `malloc` are not initialized to zero by default. The function above results in undefined behavior.

## Double-free
```C
char *p = malloc(10);
free(p);
//  .. later ...
free(p); 
```
It is an error to free the same heap memory twice.

## Dangling Pointers
```C
char *p = malloc(10);
strcpy(p, "Hello");
free(p);
//  .. later ...
strcpy(p,"World"); 
```
Accessing freed memory results in undefined behavior. A defensive programming practice is to set pointers to NULL as soon as the memory is freed, since there is no other way to reliably check if a pointer points to a valid address. The following macro accomplishes this.

```C
#define safer_free(p) {free(p); (p) = NULL;}
```

## Forgetting to Copy `getline` Buffer
```C
#include <stdio.h>
  
int main(void){
  char *line = NULL;
  size_t linecap = 0;
  char *strings[3];

  // assume stdin contains "1\n2\n\3\n"
  for (size_t i = 0; i < 3; ++i)
    strings[i] = getline(&line, &linecap, stdin) >= 0 ? line : "";

  // this prints out "3\n3\n\3n" instead of "3\n\2\n1\n"
  for (size_t i = 3; i--;) // i=2,1,0
    printf("%s", strings[i]);
}
```
Since `getline` reuses a buffer, all pointers in the `strings` array are actually pointing to the same memory. We can fix this by setting the assignment of `strings[i]` to a deep copy of the buffer.
```C
   strings[i] = getline(&line, &linecap, stdin) >= 0 ? strdup(line) : "";
```
Fun fact: providing "1\n123456789abcdef\n3\n" to the broken version of the program might cause it to print out "3\n3\n1\n" instead of "3\n3\n3\n". Now why might that be? 
*Hint: use Valgrind*

# Logic and Program Flow Mistakes
## Forgetting `break` after `case` 
```C
int flag = 1; // Will print all three lines.
switch (flag) {
case 1: printf("I'm printed\n");
case 2: printf("Me too\n");
case 3: printf("Me three\n");
}
```
Case statements without a break will just continue onto the code of the next case statement. The correct code is shown below. The break for the last statement is unnecessary because there are no more cases to be executed after the last one.
```C
int flag = 1; // Will print only "I'm printed\n"
switch (flag) {
case 1: 
    printf("I'm printed\n");
    break;
case 2: 
    printf("Me too\n");
    break;
case 3: 
    printf("Me three\n");
    break; //unnecessary
}
```

## Assignment vs Equality Check

```C
int answer = 3; // Will print out the answer.
if (answer = 42) { printf("I've solved the answer! It's %d", answer); }
```
Compilers will usually warn you about this mistake. If you really want to perform an assignment, add an extra pair of parentheses to suppress these warnings.
```C
ssize_t x;
if ( (x = read(somefd, somebuf, somenum)) ){
  // do something
}
```

## Undeclared or Incorrectly Prototyped Functions

```C
#include <stdio.h>
int main(void){
  int start = time();
  printf("%d\n", start);
}
```
The library call `time` actually takes a parameter (a pointer to some memory that can receive the `time_t` structure). The compiler might not catch this error because the programmer did not provide a valid function prototype by including time.h. For this reason, calling undeclared functions is illegal in C99 and beyond.

## Extra Semicolons

Semicolons after `for` and `while` statements will cause them to be interpreted as "empty loops".

```C
int i;
for (i = 0; i < 5; i++);{
  printf("I'm printed once\n");
}
while (i < 10); // program goes into infinite loop
  i++;          // this code is never executed
```

Adding semicolons adjacent to each other in code blocks is perfectly legal and is ignored by the compiler. You can also omit components of a `for` statement by putting nothing between semicolons or between semicolons and parentheses.
```C
int i = 0;
for (; i++ < 5;) { // i=1,2,3,4,5
    printf("%d\n", i);;;;;;;;;;;;;
}
```

# Other Gotchas
## C Preprocessor macros and precedence

Preprocessing is an operation performed **before** the program is actually compiled. It is nothing but text substitution, i.e. copy-and-paste. Consider the following code.
```C
#define BADD(x,y) x+y
char buffer[BADD(5,5)*2];
```

After preprocessing, the code will look exactly like this.
```C
char buffer[5+5*2];
```

Notice that the buffer takes up 15 bytes instead of 20, since multiplication has higher precedence than addition. To prevent this kind of behavior, make sure to surround all macro variables as well as the entire macro expression with parentheses.
```C
#define ADD(x,y) ((x)+(y))
```

## C Preprocessor Macros and Side-Effects
```C
#define min(a,b) ((a)<(b) ? (a) : (b))
int x = 4;
if (min(x++, 100)) printf("%d is six", x);
```
The conditional expression effectively expands to `x++ < 100 ? x++ : 100`, which results in `x` being incremented twice. There is no good way to prevent these kinds of side effects when using Standard C macros. But [GNU C](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html) provides some workarounds (only works when using GCC).

## `sizeof(type[])` vs `sizeof(type *)`
```C
#define ARRAY_LENGTH(A) (sizeof((A)) / sizeof((A)[0]))
int fixed_length_array[10]; // ARRAY_LENGTH(fixed_length_array) = 10
int *dynamic_array = malloc(10); // ARRAY_LENGTH(dynamic_array) = 2 or 1
```
If we have a declared array variable like `fixed_length_array`, the `sizeof` operator yields the bytes that array takes up, and dividing this size by the size of the first element yields the number of elements in the array. Unfortunately, the size of a pointer is always the same (8 or 4 bytes), no matter the size or type of the array to which it points. Only compile-time declared array variables (and [C99 variable-length arrays](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html)) will expose the true array size through `sizeof`.

## `sizeof` and Side-Effects

```C
int a = 0;
size_t size = sizeof(a++);
printf("size: %lu, a: %d", size, a);
```
This code prints out the following.
```C
size: 4, a: 0
```
The expression passed into `sizeof` is not actually evaluated at runtime in most cases, since the type (hence the size) of the expression can be calculated at compile time. Though there are exceptions in the case of [C99 variable-length arrays](http://port70.net/~nsz/c/c11/n1570.html#6.5.3.4p2), since their sizes are determined at runtime.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-2:-Text-Input-And-Output">
Back: C Programming, Part 2: Text Input And Output
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs">
Next: C Programming, Part 4: Strings and Structs
</a>
</div>