---
title: "Chương 01: Học Ngôn Ngữ Lập Trình C, Phần 1: Giới Thiệu"
type: book-vi-vn
order: 1
---

# Muốn có một giới thiệu nhanh về C?
* Tiếp tục đọc để có một khóa học nhanh về lập trình C bên dưới
* Sau đó xem [[trang wiki về các lỗi thường gặp trong C|Học Ngữ Lập Trình C, Phần 3: Các Lỗi Thường Gặp]].
* Và tìm hiểu về [[nhập/xuất văn bản|Học Ngữ Lập Trình C, Phần 2: Nhập và Xuất Văn Bản]].
* Thư giãn với [video giới thiệu của Lawrence](http://cs-education.github.io/sys/#) (Cũng có một máy ảo trong trình duyệt mà bạn có thể chơi thử!)

# Tài nguyên bên ngoài
* [Learn X in Y](https://learnxinyminutes.com/docs/c/) (Rất khuyến khích đọc lướt qua!)
* [C cho lập trình viên C++/Java](http://www.ccs.neu.edu/course/com3620/parent/C-for-Java-C++/c-for-c++-alt.html)
* [Hướng dẫn C của Brian Kernighan](http://www.lysator.liu.se/c/bwk-tutor.html)
* [c faq](http://c-faq.com/)
* [C Bootcamp](http://gribblelab.org/CBootCamp/index.html)
* [Tham khảo hàm C/C++](http://www.cplusplus.com/reference/clibrary/)
* [Hướng dẫn gdb (Gnu debugger)](http://www.unknownroad.com/rtfm/gdbtut/gdbtoc.html) Mẹo: chạy gdb với tham số dòng lệnh "-tui" để có phiên bản toàn màn hình của trình gỡ lỗi.
* Thêm tài nguyên yêu thích của bạn vào đây

# Khóa học nhanh về C

*Cảnh báo trang mới* Vui lòng sửa lỗi chính tả và định dạng cho tôi và thêm các liên kết hữu ích nữa.*

## Làm thế nào để viết một chương trình hello world hoàn chỉnh trong C?
```C
#include <stdio.h>
int main(void) { 
    printf("Hello World\n");
    return 0; 
}
```
## Tại sao chúng ta sử dụng `#include <stdio.h>`?
Chúng ta lười biếng! Chúng ta không muốn khai báo hàm `printf`. Nó đã được khai báo sẵn trong tệp `stdio.h`. Lệnh `#include` bao gồm nội dung của tệp đó như một phần của tệp chúng ta để biên dịch.

Cụ thể, lệnh `#include` lấy tệp `stdio.h` (viết tắt của **st**an**d**ard **i**nput **o**utput - **nhập xuất chuẩn**) nằm ở đâu đó trong hệ điều hành của bạn, sao chép nội dung và thay thế nó vào vị trí của lệnh `#include`.

## Chuỗi trong C được biểu diễn như thế nào?
Chúng được biểu diễn dưới dạng các ký tự trong bộ nhớ. Kết thúc của chuỗi bao gồm một byte NULL (0). Vì vậy, "ABC" yêu cầu bốn (4) byte `['A','B','C','\0']`. Cách duy nhất để tìm ra độ dài của một chuỗi C là tiếp tục đọc bộ nhớ cho đến khi bạn tìm thấy byte NULL. Các ký tự trong C luôn có kích thước chính xác là một byte.

Khi bạn viết một chuỗi ký tự `"ABC"` trong một biểu thức, chuỗi ký tự đó sẽ được đánh giá thành một con trỏ ký tự (`char *`), trỏ đến byte/ký tự đầu tiên của chuỗi. Điều này có nghĩa là `ptr` trong ví dụ dưới đây sẽ giữ địa chỉ bộ nhớ của ký tự đầu tiên trong chuỗi.
```C
char *ptr = "ABC";
```
Một số cách phổ biến để khởi tạo một chuỗi bao gồm:
```C
char *str = "ABC";
char str[] = "ABC";
char str[]={'A','B','C','\0'};
```
## Làm thế nào để khai báo một con trỏ?
Một con trỏ tham chiếu đến một địa chỉ bộ nhớ. Kiểu của con trỏ rất hữu ích - nó cho trình biên dịch biết cần đọc/ghi bao nhiêu byte. Bạn có thể khai báo một con trỏ như sau.
```C
int *ptr1;
char *ptr2;
```
Do cú pháp của C, một `int*` hoặc bất kỳ con trỏ nào thực ra không phải là một kiểu dữ liệu riêng biệt. Bạn phải đặt dấu hoa thị `*` trước mỗi biến con trỏ. Một lỗi phổ biến là khai báo như sau:
```C
int* ptr3, ptr4;
```
Khai báo này chỉ có `*ptr3` được khai báo là một con trỏ. `ptr4` thực ra sẽ là một biến int thông thường. Để sửa khai báo này, hãy giữ dấu `*` trước mỗi biến con trỏ:
```C
int *ptr3, *ptr4;
```
## Làm thế nào để sử dụng con trỏ để đọc/ghi một số vùng nhớ?
Giả sử rằng chúng ta khai báo một con trỏ `int *ptr`. Để dễ thảo luận, giả sử rằng `ptr` trỏ đến địa chỉ bộ nhớ `0x1000`. Nếu chúng ta muốn ghi vào một vùng nhớ mà con trỏ trỏ tới, chúng ta có thể **giải tham chiếu** con trỏ và gán giá trị bằng cách sử dụng `*ptr`.

```C
*ptr = 0; // Ghi vào một vùng nhớ.
``` 
C sẽ lấy kiểu của con trỏ là `int` và ghi `sizeof(int)` byte từ vị trí bắt đầu của con trỏ, có nghĩa là các byte tại địa chỉ `0x1000`, `0x1001`, `0x1002`, `0x1003` sẽ đều bằng không. Số byte được ghi phụ thuộc vào kiểu con trỏ. Điều này giống nhau cho tất cả các kiểu dữ liệu nguyên thủy nhưng struct thì hơi khác một chút.

## Số học con trỏ là gì?
Bạn có thể cộng một số nguyên vào một con trỏ. Tuy nhiên, kiểu của con trỏ được sử dụng để xác định mức độ tăng của con trỏ. Đối với con trỏ char, điều này rất đơn giản vì các ký tự luôn có kích thước một byte:
```C
char *ptr = "Hello"; // ptr giữ địa chỉ bộ nhớ của ký tự 'H'
ptr += 2; //ptr bây giờ trỏ đến ký tự 'l' đầu tiên
```
Nếu một int là 4 byte thì ptr+1 trỏ đến 4 byte sau vị trí mà ptr đang trỏ tới.
```C
char *ptr = "ABCDEFGH";
int *bna = (int *) ptr;
bna +=1; // Sẽ tăng lên một khoảng không gian của một số nguyên (tức là 4 byte trên một số hệ thống)
ptr = (char *) bna;
printf("%s", ptr);
/* Chú ý rằng chỉ có 'EFGH' được in ra. Tại sao lại như vậy? Như đã đề cập ở trên, khi thực hiện 'bna+=1' chúng ta đang tăng con trỏ **integer** lên 1, (tương đương với 4 byte trên hầu hết các hệ thống) điều này tương đương với 4 ký tự (mỗi ký tự chỉ có 1 byte)*/
return 0;
```
Vì số học con trỏ trong C luôn tự động được chia tỷ lệ theo kích thước của kiểu mà nó trỏ tới, bạn không thể thực hiện số học con trỏ trên con trỏ void.

Bạn có thể nghĩ về số học con trỏ trong C như thực hiện điều sau

Nếu tôi muốn làm
```C
int *ptr1 = ...;
int *offset = ptr1 + 4;
```
Hãy nghĩ
```C
int *ptr1 = ...;
char *temp_ptr1 = (char*) ptr1;
int *offset = (int*)(temp_ptr1 + sizeof(int)*4);
```
Để lấy giá trị. **Mỗi khi bạn thực hiện số học con trỏ, hãy hít thở sâu và đảm bảo rằng bạn đang dịch chuyển đúng số byte mà bạn nghĩ bạn đang dịch chuyển.**

## Con trỏ void là gì?
Con trỏ void là một con trỏ không có kiểu (rất giống với biến void). Con trỏ void được sử dụng khi kiểu dữ liệu bạn đang xử lý không xác định hoặc khi bạn đang giao tiếp mã C với các ngôn ngữ lập trình khác. Bạn có thể nghĩ về nó như một con trỏ thô, hoặc chỉ là một địa chỉ bộ nhớ. Bạn không thể trực tiếp đọc hoặc ghi vào vùng nhớ mà nó trỏ tới vì kiểu void không có kích thước. Ví dụ

```C
void *give_me_space = malloc(10);
char *string = give_me_space;
```
Điều này không yêu cầu ép kiểu vì C tự động chuyển đổi `void*` sang kiểu phù hợp của nó.
**Lưu ý:**

gcc và clang không hoàn toàn tuân thủ ISO-C, có nghĩa là chúng sẽ cho phép bạn thực hiện số học con trỏ trên con trỏ void. Chúng sẽ xử lý nó như một con trỏ char nhưng đừng làm điều này vì nó có thể không hoạt động với tất cả các trình biên dịch!

## `printf` gọi `write` hay `write` gọi `printf`?
`printf` gọi `write`. `printf` bao gồm một bộ đệm nội bộ, vì vậy để tăng hiệu suất, `printf` có thể không gọi `write` mỗi lần bạn gọi `printf`. `printf` là một hàm thư viện C. `write` là một **lệnh gọi hệ thống** và như chúng ta biết, lệnh gọi hệ thống rất tốn kém. Mặt khác, `printf` sử dụng một bộ đệm phù hợp với nhu cầu của chúng ta hơn tại thời điểm đó.

## Làm thế nào để in ra giá trị của con trỏ? số nguyên? chuỗi?
Sử dụng các **chỉ định định dạng** "%p" cho con trỏ, "%d" cho số nguyên và "%s" cho chuỗi.
Danh sách đầy đủ của tất cả các chỉ định định dạng có thể được tìm thấy [ở đây](http://www.cplusplus.com/reference/cstdio/printf/)

Ví dụ về số nguyên:
```C
int num1 = 10;
printf("%d", num1); //in ra giá trị của num1
```

Ví dụ về con trỏ số nguyên:
```C
int *ptr = (int *) malloc(sizeof(int));
*ptr = 10;
printf("%p\n", ptr); // in ra địa chỉ mà con trỏ trỏ tới
printf("%p\n", &ptr); /* in ra địa chỉ của con trỏ -- cực kỳ hữu ích
khi làm việc với con trỏ kép */
printf("%d", *ptr); // in ra nội dung số nguyên của ptr
return 0;
```
Ví dụ về chuỗi:
```C
char *str = (char *) malloc(256 * sizeof(char));
strcpy(str, "Hello there!");
printf("%p\n", str); // in ra địa chỉ trong heap
printf("%s", str);
return 0;
```
[Chuỗi dưới dạng Con trỏ & Mảng @ BU](https://www.cs.bu.edu/teaching/c/string/intro/)

## Làm thế nào để lưu đầu ra chuẩn vào một tệp?
Cách đơn giản nhất: chạy chương trình của bạn và sử dụng **chuyển hướng shell**
ví dụ:
```
./program > output.txt

# Để đọc nội dung của tệp,
cat output.txt
```
Cách phức tạp hơn: đóng(1) và sau đó sử dụng open để mở lại bộ mô tả tệp.
Xem [[http://cs-education.github.io/sys/#chapter/0/section/3/activity/0]]

## Sự khác biệt giữa con trỏ và mảng là gì? Đưa ra một ví dụ về điều gì đó bạn có thể làm với một cái nhưng không thể làm với cái kia.
```C
char ary[] = "Hello";
char *ptr = "Hello";
```
Ví dụ

Tên mảng trỏ đến byte đầu tiên của mảng. Cả `ary` và `ptr` đều có thể được in ra:
```C
char ary[] = "Hello";
char *ptr = "Hello";
// In ra địa chỉ và nội dung
printf("%p : %s\n", ary, ary);
printf("%p : %s\n", ptr, ptr);
```
Mảng có thể thay đổi, vì vậy chúng ta có thể thay đổi nội dung của nó (hãy cẩn thận không ghi đè lên các byte ngoài phạm vi của mảng). May mắn thay, 'World' không dài hơn 'Hello'.

Trong trường hợp này, con trỏ char `ptr` trỏ đến một vùng nhớ chỉ đọc (nơi lưu trữ chuỗi ký tự được cấp phát tĩnh), vì vậy chúng ta không thể thay đổi nội dung đó.
```C
strcpy(ary, "World"); // OK
strcpy(ptr, "World"); // KHÔNG OK - Lỗi phân đoạn (chương trình bị sập)

```
Tuy nhiên, không giống như mảng, chúng ta có thể thay đổi `ptr` để trỏ đến một vùng nhớ khác,
```C
ptr = "World"; // OK!
ptr = ary; // OK!
ary = (..anything..) ; // KHÔNG BIÊN DỊCH ĐƯỢC
// ary luôn luôn trỏ đến mảng ban đầu.
printf("%p : %s\n", ptr, ptr);
strcpy(ptr, "World"); // OK vì bây giờ ptr đang trỏ đến vùng nhớ có thể thay đổi (mảng)
```
Điều cần rút ra từ đây là con trỏ * có thể trỏ đến bất kỳ loại bộ nhớ nào trong khi mảng C [] chỉ có thể trỏ đến bộ nhớ trên **ngăn xếp** (stack). Trong trường hợp phổ biến hơn, con trỏ sẽ trỏ đến bộ nhớ **đống** (heap), trong trường hợp đó bộ nhớ được tham chiếu bởi con trỏ CÓ THỂ được sửa đổi.

## `sizeof()` trả về số byte. Vậy sử dụng đoạn mã trên, `sizeof(ary)` và `sizeof(ptr)` là gì?
`sizeof(ary)`: `ary` là một mảng. Trả về số byte cần thiết cho toàn bộ mảng (5 ký tự + byte zero = 6 byte)
`sizeof(ptr)`: Giống như `sizeof(char *)`. Trả về số byte cần thiết cho một con trỏ (ví dụ: 4 hoặc 8 cho máy 32 bit hoặc 64 bit)

`sizeof` là một toán tử đặc biệt. Thực ra nó là thứ mà trình biên dịch thay thế trước khi biên dịch chương trình vì kích thước của tất cả các kiểu đã được biết tại thời điểm biên dịch. Khi bạn có `sizeof(char*)`, nó lấy kích thước của một con trỏ trên máy của bạn (8 byte cho máy 64 bit và 4 cho máy 32 bit, v.v.). Khi bạn thử `sizeof(char[])`, trình biên dịch sẽ nhìn vào đó và thay thế bằng số byte mà **toàn bộ** mảng chứa vì tổng kích thước của mảng đã được biết tại thời điểm biên dịch.

```C
char str1[] = "will be 11";
char* str2 = "will be 8";
sizeof(str1); //11 vì nó là một mảng
sizeof(str2); //8 vì nó là một con trỏ
```

Hãy cẩn thận khi sử dụng sizeof để lấy độ dài của một chuỗi!

## Đoạn mã nào sau đây là đúng hoặc sai và tại sao?
```C
int* f1(int *p) {
    *p = 42;
    return p;
} // Đoạn mã này là đúng;
```

```C
char* f2() {
    char p[] = "Hello";
    return p;
} // Đoạn mã này là sai!
```
Giải thích: Một mảng `p` được tạo trên ngăn xếp với kích thước đủ để chứa các ký tự H, e, l, l, o và một byte null, tức là (6) byte. Mảng này được lưu trữ trên ngăn xếp và sẽ không hợp lệ sau khi chúng ta thoát khỏi hàm `f2`.
```C
char* f3() {
    char *p = "Hello";
    return p;
} // Đúng
```
Giải thích: p là một con trỏ. Nó giữ địa chỉ của hằng chuỗi. Hằng chuỗi không thay đổi và vẫn hợp lệ ngay cả sau khi f3 trả về.

```C
char* f4() {
    static char p[] = "Hello";
    return p;
} // Đúng
```
Giải thích: Mảng là **tĩnh** có nghĩa là nó tồn tại trong suốt thời gian của tiến trình (các biến tĩnh không nằm trên heap hoặc stack).

## Làm thế nào để tra cứu thông tin về các lệnh gọi hàm thư viện C và lệnh gọi hệ thống?
Sử dụng các **trang hướng dẫn** (man page). Lưu ý rằng các trang hướng dẫn được tổ chức thành các phần. Phần 2 = Lệnh gọi hệ thống. Phần 3 = Thư viện C.
Trên web: Google "man7 open"
Trên shell: `man -S2 open` hoặc `man -S3 printf`

## Làm thế nào để cấp phát bộ nhớ trên heap?
Sử dụng `malloc`. Ngoài ra còn có `realloc` và `calloc`.
Thường được sử dụng với `sizeof`. Ví dụ: cấp phát đủ không gian để chứa 10 số nguyên.
```C
int *space = malloc(sizeof(int) * 10);
```

## Có gì sai với đoạn mã sao chép chuỗi này?

```C
void mystrcpy(char* dest, char* src) { 
  // void có nghĩa là không có giá trị trả về
  while( *src ) { dest = src; src ++; dest++; }
}
```
Trong đoạn mã trên, nó chỉ đơn giản thay đổi con trỏ `dest` để trỏ đến chuỗi nguồn. Ngoài ra, các byte null không được sao chép. Đây là một phiên bản tốt hơn

```C
  while( *src ) { *dest = *src; src ++; dest++; } 
  *dest = *src;
```
Lưu ý rằng cũng thường thấy kiểu triển khai sau đây, thực hiện mọi thứ bên trong biểu thức kiểm tra, bao gồm cả việc sao chép byte null.
```C
  while( (*dest++ = *src++ )) {};
```

## Làm thế nào để viết một hàm thay thế `strdup`?
```C
// Sử dụng strlen+1 để tìm byte zero... 
char* mystrdup(char*source) {
   char *p = (char *) malloc ( strlen(source)+1 );
   strcpy(p,source);
   return p;
}
```
## Làm thế nào để giải phóng bộ nhớ trên heap?
Sử dụng `free`!
```C
int *n = (int *) malloc(sizeof(int));
*n = 10;
//Làm một số việc
free(n);
```

## Lỗi giải phóng bộ nhớ hai lần là gì? Làm thế nào để tránh? Con trỏ lơ lửng là gì? Làm thế nào để tránh?
Lỗi giải phóng bộ nhớ hai lần xảy ra khi bạn vô tình cố gắng giải phóng cùng một vùng nhớ hai lần.
```C
int *p = malloc(sizeof(int));
free(p);

*p = 123; // Oops! - Con trỏ lơ lửng! Ghi vào bộ nhớ mà chúng ta không còn sở hữu nữa

free(p); // Oops! - Giải phóng bộ nhớ hai lần!
```
Cách khắc phục đầu tiên là viết các chương trình đúng! Thứ hai, đó là thói quen lập trình tốt để đặt lại con trỏ về `NULL` sau khi bộ nhớ đã được giải phóng. Điều này đảm bảo rằng con trỏ không thể được sử dụng sai mà không làm chương trình bị sập.

Khắc phục:
```C
p = NULL; // Bây giờ bạn không thể sử dụng con trỏ này một cách nhầm lẫn nữa
```
## Ví dụ về tràn bộ đệm là gì?
Ví dụ nổi tiếng: Heartbleed (thực hiện một `memcpy` vào một bộ đệm có kích thước không đủ).
Ví dụ đơn giản: triển khai một `strcpy` và quên cộng thêm 1 vào `strlen` khi xác định kích thước bộ nhớ cần thiết.

## `typedef` là gì và bạn sử dụng nó như thế nào?
`typedef` dùng để khai báo một bí danh cho một kiểu dữ liệu. Thường được sử dụng với struct để giảm bớt sự lộn xộn khi phải viết `struct` như một phần của kiểu.
```C
typedef float real; 
real gravity = 10;
// Ngoài ra, typedef cung cấp cho chúng ta một lớp trừu tượng trên kiểu cơ bản được sử dụng.
// Trong tương lai, chúng ta chỉ cần thay đổi typedef này nếu muốn thư viện vật lý của chúng ta sử dụng kiểu double thay vì float.

typedef struct link link_t;
// Với struct, bao gồm từ khóa 'struct' như một phần của các kiểu ban đầu
```
Trong lớp học này, chúng ta thường sử dụng typedef cho các hàm. Ví dụ, một typedef cho một hàm có thể như sau:

```C
typedef int (*comparator)(void*,void*);

int greater_than(void* a, void* b){
    return a > b;
}
comparator gt = greater_than;
```
Điều này khai báo một kiểu hàm `comparator` chấp nhận hai tham số `void*` và trả về một số nguyên.

## Wow, đó là rất nhiều về C
Đừng lo, còn nhiều điều nữa sắp tới!

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output">
Tiếp theo: Học Ngữ Lập Trình C, Phần 2: Nhập và Xuất Văn Bản
</a>
</div>