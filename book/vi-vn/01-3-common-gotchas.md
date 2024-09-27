---
title: "Chương 01: Học Ngôn Ngữ Lập Trình C, Phần 3: Các Lỗi Thường Gặp"
type: book-vi-vn
order: 3
---

# Những lỗi phổ biến mà lập trình viên C thường mắc phải?

## Hằng chuỗi chỉ có thể đọc
```C
char array[] = "Hi!"; // mảng chứa một bản sao có thể thay đổi
strcpy(array, "OK");

char *ptr = "Can't change me"; // ptr trỏ đến một vùng nhớ không thể thay đổi
strcpy(ptr, "Will not work"); // sẽ không hoạt động
```
**Hằng chuỗi** (string literal) là các mảng ký tự được lưu trữ trong phân đoạn mã của chương trình, không thể thay đổi. Hai hằng chuỗi có thể chia sẻ cùng một không gian trong bộ nhớ. Dưới đây là một ví dụ:

```C
char *str1 = "Brandon Chong là trợ giảng tốt nhất";
char *str2 = "Brandon Chong là trợ giảng tốt nhất";
str1 == str2; // đúng
```
Các chuỗi mà `str1` và `str2` trỏ đến có thể thực sự nằm ở cùng một vị trí trong bộ nhớ.

Tuy nhiên, **mảng ký tự** (character array) chứa các bản sao sâu của chuỗi ký tự được gán cho chúng, và các mảng khác nhau đại diện cho các vị trí bộ nhớ khác nhau. Những mảng ký tự sau đây không nằm ở cùng một nơi trong bộ nhớ.

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
C không thực hiện kiểm tra giới hạn khi truy cập mảng. Ví dụ trên ghi vào `array[10]` nằm ngoài giới hạn của mảng (mảng chỉ có các phần tử từ `array[0]` đến `array[9]`). Điều này có thể làm hỏng các biến khác trên ngăn xếp cũng như cách thực hiện của ngăn xếp gọi hàm, khiến chương trình của bạn dễ bị tấn công từ hacker. Trên thực tế, những **lỗi tràn bộ đệm** này thường xuất phát từ việc sử dụng các lệnh gọi hàm thư viện không an toàn hoặc đặt giới hạn kích thước sai trên một lệnh gọi hàm thư viện an toàn hơn.

```C
gets(array); // Hy vọng đầu vào ngắn hơn mảng của tôi! (KHÔNG BAO GIỜ sử dụng gets)
fgets(array, 4096, stdin); // Ôi chao (nếu kích thước của array nhỏ hơn 4096)
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
Các biến tự động (**automatic variable**) chỉ được gắn với bộ nhớ ngăn xếp miễn là chúng đang trong phạm vi. Sau khi chúng ra khỏi phạm vi, dữ liệu được lưu trữ tại địa chỉ bộ nhớ của chúng trở nên không xác định. Các biến tĩnh (**static variable**) nằm trong phân đoạn dữ liệu, có thể truy cập an toàn ngay cả khi các biến đó không nằm trong phạm vi.

## `sizeof(type *)` so với `sizeof(type)` 
```C
struct User {
  char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t *));
```
Trong ví dụ trên, chúng ta cần phân bổ đủ byte cho struct `user_t`. Thay vào đó, chúng ta đã phân bổ đủ byte để chứa một con trỏ. Việc ghi vào con trỏ `user` có thể làm hỏng heap. Mã đúng được hiển thị bên dưới.
```C
struct User {
  char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t));
```
## Chuỗi yêu cầu `strlen(s)+1` byte

Mỗi chuỗi phải có một byte null (`\0`) sau ký tự cuối cùng. Để lưu trữ chuỗi "Hi" cần 3 byte: `[H] [i] [\0]`.

```C
char *strdup(const char *input) {     /* trả về một bản sao của 'input' */
  char *copy;
  copy = malloc(sizeof (char *));   /* không! điều này cấp phát không gian cho một con trỏ, không phải là một chuỗi */
  copy = malloc(strlen(input));     /* Gần rồi...nhưng còn ký tự kết thúc null thì sao? */
  copy = malloc(strlen(input) + 1); /* Đúng rồi. */
  strcpy(copy, input);              /* strcpy sẽ cung cấp ký tự kết thúc null */
  return copy;
}
```
## Không khởi tạo bộ nhớ
```C
void myfunct() {
  char array[10];
  char *p = malloc(10);
  printf("%s %s\n", array, p);
}
```
Các biến tự động (trên ngăn xếp) và bộ nhớ heap được cấp phát bằng `malloc` không được khởi tạo thành zero theo mặc định. Hàm trên dẫn đến **hành vi không xác định** (undefined behavior).

## Giải phóng bộ nhớ hai lần (double-free)
```C
char *p = malloc(10);
free(p);
//  .. sau đó ...
free(p);
```
Giải phóng cùng một vùng nhớ heap hai lần là một lỗi.

## Con trỏ lơ lửng (dangling pointer)
```C
char *p = malloc(10);
strcpy(p, "Hello");
free(p);
//  .. sau đó ...
strcpy(p,"World");
```
Truy cập vào bộ nhớ đã được giải phóng dẫn đến hành vi không xác định. Một thực hành lập trình phòng thủ là đặt con trỏ thành `NULL` ngay khi bộ nhớ được giải phóng, vì không có cách nào khác để kiểm tra một cách đáng tin cậy xem một con trỏ có trỏ đến một địa chỉ hợp lệ hay không. Macro sau đây thực hiện điều này.
```C
#define safer_free(p) {free(p); (p) = NULL;}
```
## Quên sao chép bộ đệm `getline`
```C
#include <stdio.h>
  
int main(void){
  char *line = NULL;
  size_t linecap = 0;
  char *strings[3];

  // giả sử stdin chứa "1\n2\n3\n"
  for (size_t i = 0; i < 3; ++i)
    strings[i] = getline(&line, &linecap, stdin) >= 0 ? line : "";

  // đoạn này in ra "3\n3\n3\n" thay vì "3\n2\n1\n"
  for (size_t i = 3; i--;) // i=2,1,0
    printf("%s", strings[i]);
}
```
Vì `getline` tái sử dụng một bộ đệm, tất cả các con trỏ trong mảng `strings` thực sự đều đang trỏ đến cùng một bộ nhớ. Chúng ta có thể sửa điều này bằng cách đặt phép gán của `strings[i]` thành một bản sao sâu của bộ đệm.
```C
  strings[i] = getline(&line, &linecap, stdin) >= 0 ? strdup(line) : "";
```
Thực tế thú vị: nếu cung cấp "1\n123456789abcdef\n3\n" cho phiên bản lỗi của chương trình, nó có thể in ra "3\n3\n1\n" thay vì "3\n3\n3\n". Vậy tại sao lại như vậy? 
*Gợi ý: sử dụng Valgrind*

# Lỗi Logic và Lỗi Trong Luồng Chương Trình
## Quên `break` sau `case` 
```C
int flag = 1; // Sẽ in ra tất cả ba dòng.
switch (flag) {
case 1: printf("Tôi được in\n");
case 2: printf("Tôi cũng vậy\n");
case 3: printf("Tôi cũng thế\n");
}
```
Các câu lệnh `case` không có `break` sẽ tiếp tục thực thi mã của câu lệnh `case` tiếp theo. Mã đúng được hiển thị bên dưới. `break` cho câu lệnh cuối cùng không cần thiết vì không còn thêm câu lệnh `case` nào để thực thi sau câu lệnh cuối cùng.
```C
int flag = 1; // Chỉ in "Tôi được in\n"
switch (flag) {
case 1: 
    printf("Tôi được in\n");
    break;
case 2: 
    printf("Tôi cũng vậy\n");
    break;
case 3: 
    printf("Tôi cũng thế\n");
    break; //không cần thiết
}
```

## Phép gán so với Kiểm tra bằng nhau

```C
int answer = 3; // Sẽ in ra câu trả lời.
if (answer = 42) { printf("Tôi đã giải quyết câu trả lời! Nó là %d", answer); }
```
Trình biên dịch thường sẽ cảnh báo bạn về lỗi này. Nếu bạn thực sự muốn thực hiện một phép gán trong điều kiện `if`, hãy thêm một cặp dấu ngoặc đơn để tắt những cảnh báo này.
```C
ssize_t x;
if ( (x = read(somefd, somebuf, somenum)) ){
    // làm gì đó nếu read() trả về giá trị khác 0
}
```
## Hàm không được khai báo hoặc khai báo nguyên mẫu sai

```C
#include <stdio.h>
int main(void){
  int start = time();
  printf("%d\n", start);
}
```
Hàm thư viện `time` thực sự yêu cầu một tham số (một con trỏ đến một vùng nhớ có thể nhận cấu trúc `time_t`). Trình biên dịch có thể không bắt lỗi này vì lập trình viên không cung cấp một nguyên mẫu hàm hợp lệ bằng cách thêm `#include <time.h>`. Vì lý do này, việc gọi các hàm chưa được khai báo là bất hợp pháp trong C99 và các phiên bản mới hơn.

## Dấu chấm phẩy thừa

Dấu chấm phẩy sau các câu lệnh `for` và `while` sẽ khiến chúng được hiểu là "vòng lặp rỗng".

```C
int i;
for (i = 0; i < 5; i++);{ 
  printf("Tôi chỉ được in một lần\n"); // Khối lệnh này nằm ngoài vòng lặp for
}
while (i < 10); // chương trình đi vào vòng lặp vô hạn
  i++;          // đoạn mã này không bao giờ được thực thi
```

Thêm dấu chấm phẩy liên tiếp vào các khối mã là hoàn toàn hợp pháp và sẽ bị trình biên dịch bỏ qua. Bạn cũng có thể bỏ qua các thành phần của câu lệnh `for` bằng cách không đặt gì giữa các dấu chấm phẩy hoặc giữa dấu chấm phẩy và dấu ngoặc đơn.
```C
int i = 0;
for (; i++ < 5;) { // i=1,2,3,4,5
  printf("%d\n", i);;;;;;;;;;;;; // Các dấu chấm phẩy thừa này sẽ bị bỏ qua
}
```

# Những vấn đề khác
## Macro tiền xử lý C và độ ưu tiên

**Tiền xử lý** (preprocessing) là một hoạt động được thực hiện **trước** khi chương trình thực sự được biên dịch. Nó không là gì khác ngoài việc thay thế văn bản, tức là sao chép và dán. Hãy xem xét đoạn mã sau.
```C
#define BADD(x,y) x+y
char buffer[BADD(5,5)*2];
```

Sau khi tiền xử lý, mã sẽ trông chính xác như thế này.
```C
char buffer[5+5*2];
```

Chú ý rằng bộ đệm chiếm 15 byte thay vì 20, vì phép nhân có độ ưu tiên cao hơn phép cộng. Để ngăn chặn hành vi này, hãy đảm bảo bao quanh tất cả các biến macro cũng như toàn bộ biểu thức macro bằng dấu ngoặc đơn.
```C
#define ADD(x,y) ((x)+(y))
```

## Macro tiền xử lý C và hiệu ứng phụ
```C
#define min(a,b) ((a)<(b) ? (a) : (b))
int x = 4;
if (min(x++, 100)) printf("%d is six", x);
```
Biểu thức điều kiện được mở rộng thành `x++ < 100 ? x++ : 100`, dẫn đến `x` được tăng hai lần. Không có cách tốt nào để ngăn chặn những hiệu ứng phụ này khi sử dụng macro C chuẩn. Nhưng [GNU C](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html) cung cấp một số giải pháp (chỉ hoạt động khi sử dụng GCC).

## `sizeof(type[])` so với `sizeof(type *)`
```C
#define ARRAY_LENGTH(A) (sizeof((A)) / sizeof((A)[0]))
int fixed_length_array[10]; // ARRAY_LENGTH(fixed_length_array) = 10
int *dynamic_array = malloc(10); // ARRAY_LENGTH(dynamic_array) = 2 hoặc 1 (trên hệ thống 64-bit hoặc 32-bit)
```
Nếu chúng ta có một biến mảng đã được khai báo như `fixed_length_array`, toán tử `sizeof` trả về số byte mà mảng chiếm dụng, và chia kích thước này cho kích thước của phần tử đầu tiên sẽ cho ra số lượng phần tử trong mảng. Tuy nhiên, kích thước của một con trỏ luôn giống nhau (8 byte trên hệ thống 64-bit hoặc 4 byte trên hệ thống 32-bit), bất kể kích thước hoặc kiểu của mảng mà nó trỏ đến. Chỉ có các biến mảng được khai báo tại thời điểm biên dịch (và [mảng có độ dài biến đổi C99](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html)) mới cho phép biết kích thước mảng thực sự thông qua `sizeof`.

## `sizeof` và hiệu ứng phụ

```C
int a = 0;
size_t size = sizeof(a++);
printf("size: %lu, a: %d", size, a);
```
Đoạn mã này in ra như sau.
```
size: 4, a: 0
```
Biểu thức được truyền vào `sizeof` **không** thực sự được đánh giá tại thời điểm chạy trong hầu hết các trường hợp, vì kiểu dữ liệu (và do đó là kích thước) của biểu thức có thể được tính toán tại thời điểm biên dịch. Tuy nhiên, có những ngoại lệ trong trường hợp [mảng có độ dài biến đổi C99](http://port70.net/~nsz/c/c11/n1570.html#6.5.3.4p2), vì kích thước của chúng được xác định tại thời điểm chạy.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-2:-Text-Input-And-Output">
Quay lại: Học Ngữ Lập Trình C, Phần 2: Nhập và Xuất Văn Bản
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs">
Tiếp theo: Học Ngữ Lập Trình C, Phần 4: Chuỗi và Cấu trúc
</a>
</div>
