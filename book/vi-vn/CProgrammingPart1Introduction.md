## Muốn tìm hiểu nhanh về C?
* Tiếp tục đọc để có cái nhìn tổng quan nhanh về Lập trình C bên dưới
* Sau đó, xem [[Trang wiki về các lỗi thường gặp trong C|Lập trình C, Phần 3: Các lỗi thường gặp]].
* Và tìm hiểu về [[Nhập/xuất văn bản|Lập trình C, Phần 2: Nhập và Xuất Văn bản]].
* Thư giãn với [video giới thiệu của Lawrence](http://cs-education.github.io/sys/#) (Ngoài ra còn có một máy ảo chạy ngay trong trình duyệt mà bạn có thể chơi!)

# Tài nguyên bên ngoài
* [Học X trong Y phút](https://learnxinyminutes.com/docs/c/) (Rất nên đọc lướt qua!)
* [C dành cho Lập trình viên C++/Java](http://www.ccs.neu.edu/course/com3620/parent/C-for-Java-C++/c-for-c++-alt.html)
* [Hướng dẫn C của Brian Kernighan](http://www.lysator.liu.se/c/bwk-tutor.html)
* [c faq](http://c-faq.com/)
* [Khóa huấn luyện C](http://gribblelab.org/CBootCamp/index.html)
* [Tài liệu tham khảo hàm C/C++](http://www.cplusplus.com/reference/clibrary/)
* [Hướng dẫn gdb (Trình gỡ lỗi Gnu)](http://www.unknownroad.com/rtfm/gdbtut/gdbtoc.html) Mẹo: chạy gdb với tham số dòng lệnh "-tui" để có phiên bản toàn màn hình của trình gỡ lỗi.
* Thêm tài nguyên yêu thích của bạn tại đây


# Khóa học cấp tốc giới thiệu về C

*Cảnh báo trang mới* Vui lòng sửa lỗi chính tả và định dạng cho tôi và thêm các liên kết hữu ích.*

## Làm thế nào để viết một chương trình hello world hoàn chỉnh bằng C?
```C
#include <stdio.h>
int main(void) { 
    printf("Hello World\n");
    return 0; 
}
```
## Tại sao chúng ta sử dụng '`#include <stdio.h>`'?
Chúng ta lười! Chúng ta không muốn khai báo hàm `printf`. Nó đã được thực hiện sẵn cho chúng ta bên trong tệp '`stdio.h`'. `#include` sẽ chèn nội dung của tệp được chỉ định vào tệp hiện tại để biên dịch.

Cụ thể, chỉ thị `#include` lấy tệp `stdio.h` (viết tắt của **st**an**d**ard **i**nput and **o**utput - đầu vào và đầu ra tiêu chuẩn) nằm ở đâu đó trong hệ điều hành của bạn, sao chép nội dung văn bản của nó và thay thế nó vào vị trí của dòng `#include`.

## Chuỗi C được biểu diễn như thế nào?
Chúng được biểu diễn dưới dạng các ký tự trong bộ nhớ.  Kết thúc chuỗi là một byte NULL (0). Vì vậy, "ABC" yêu cầu bốn (4) byte `['A','B','C','\0']`. Cách duy nhất để biết được độ dài của một chuỗi C là tiếp tục đọc bộ nhớ cho đến khi bạn tìm thấy byte NULL. Các ký tự C luôn luôn chiếm đúng một byte.

Khi bạn viết một chuỗi ký tự `"ABC"` trong một biểu thức, chuỗi ký tự đó được đánh giá thành một con trỏ char (`char *`), trỏ đến byte/ký tự đầu tiên của chuỗi.  Điều này có nghĩa là `ptr` trong ví dụ bên dưới sẽ lưu trữ địa chỉ bộ nhớ của ký tự đầu tiên trong chuỗi.
```C
char *ptr = "ABC"
```
Một số cách phổ biến để khởi tạo một chuỗi bao gồm:
```C
char *str = "ABC";
char str[] = "ABC";
char str[]={'A','B','C','\0'};
```

## Làm thế nào để khai báo một con trỏ? 
Con trỏ là một biến lưu trữ địa chỉ bộ nhớ. Kiểu của con trỏ rất hữu ích - nó cho trình biên dịch biết cần đọc/ghi bao nhiêu byte từ địa chỉ đó. Bạn có thể khai báo một con trỏ như sau.
```C
int *ptr1;
char *ptr2;
```

Do ngữ pháp của C, `int*` hoặc bất kỳ con trỏ nào thực chất không phải là một kiểu dữ liệu riêng biệt. Bạn phải đặt dấu hoa thị `*` trước mỗi biến con trỏ. Một lỗi thường gặp là đoạn mã sau:
```C
int* ptr3, ptr4;
```
Chỉ có `*ptr3` được khai báo là một con trỏ. `ptr4` thực chất sẽ là một biến int thông thường. Để sửa khai báo này, hãy đặt `*` trước mỗi biến con trỏ:
```C
int *ptr3, *ptr4;
```

## Làm thế nào để bạn sử dụng một con trỏ để đọc/ghi một vùng bộ nhớ?
Giả sử rằng chúng ta khai báo một con trỏ `int *ptr`. Để tiện thảo luận, giả sử rằng `ptr` trỏ đến địa chỉ bộ nhớ `0x1000`. Nếu chúng ta muốn ghi vào một vùng bộ nhớ mà con trỏ trỏ đến, chúng ta có thể giải tham chiếu và gán giá trị cho `*ptr`.

```C
*ptr = 0; // Ghi vào bộ nhớ.
``` 

C sẽ lấy kiểu của con trỏ, là `int` trong trường hợp này, và ghi `sizeof(int)` byte vào bộ nhớ bắt đầu từ địa chỉ mà con trỏ trỏ đến, nghĩa là các byte tại địa chỉ `0x1000`, `0x1001`, `0x1002`, `0x1003` sẽ đều bằng 0. Số byte được ghi phụ thuộc vào kiểu con trỏ. Nó giống nhau cho tất cả các kiểu dữ liệu nguyên thủy nhưng struct thì hơi khác một chút.

## Số học con trỏ là gì?
Bạn có thể cộng một số nguyên vào một con trỏ. Tuy nhiên, kiểu con trỏ được sử dụng để xác định mức độ tăng địa chỉ của con trỏ. Đối với con trỏ char, điều này khá đơn giản vì các ký tự luôn chiếm một byte:
```C
char *ptr = "Hello"; // ptr chứa địa chỉ bộ nhớ của ký tự 'H'
ptr += 2; //ptr bây giờ trỏ đến ký tự 'l' đầu tiên
```

Nếu một int chiếm 4 byte thì ptr+1 sẽ trỏ đến địa chỉ nằm sau địa chỉ hiện tại của ptr 4 byte.
```C
char *ptr = "ABCDEFGH";
int *bna = (int *) ptr;
bna +=1; // Sẽ khiến con trỏ nhảy một khoảng bằng kích thước của một số nguyên (tức là 4 byte trên một số hệ thống)
ptr = (char *) bna;
printf("%s", ptr);
/* Lưu ý rằng chỉ có 'EFGH' được in ra. Tại sao vậy? Như đã đề cập ở trên, khi thực hiện 'bna+=1', chúng ta đang tăng con trỏ **số nguyên** lên 1, (tương đương với việc tăng địa chỉ lên 4 byte trên hầu hết các hệ thống) tương đương với việc nhảy qua 4 ký tự (mỗi ký tự chỉ chiếm 1 byte)*/
return 0;
```
Vì số học con trỏ trong C luôn tự động được điều chỉnh theo kích thước của kiểu dữ liệu mà con trỏ trỏ đến, bạn không thể thực hiện số học con trỏ trên con trỏ void.

Bạn có thể hình dung số học con trỏ trong C về cơ bản hoạt động như sau:

Nếu tôi muốn thực hiện
```C
int *ptr1 = ...;
int *offset = ptr1 + 4;
```

Hãy hình dung nó như
```C
int *ptr1 = ...;
char *temp_ptr1 = (char*) ptr1;
int *offset = (int*)(temp_ptr1 + sizeof(int)*4);
```
Để lấy giá trị. **Mỗi khi bạn thực hiện số học con trỏ, hãy hít thở sâu và đảm bảo rằng bạn đang dịch chuyển địa chỉ con trỏ đúng số byte mong muốn.**

## Con trỏ void là gì?
Một con trỏ không có kiểu (rất giống với một biến void). Con trỏ void được sử dụng khi kiểu dữ liệu bạn đang xử lý chưa được biết hoặc khi bạn đang kết nối mã C với các ngôn ngữ lập trình khác. Bạn có thể coi đây là một con trỏ thô, hoặc chỉ đơn giản là một địa chỉ bộ nhớ. Bạn không thể trực tiếp đọc hoặc ghi dữ liệu vào địa chỉ mà nó trỏ đến vì kiểu void không có kích thước. Ví dụ:

```C
void *give_me_space = malloc(10);
char *string = give_me_space;
```
Điều này không yêu cầu ép kiểu vì C tự động chuyển đổi `void*` thành kiểu thích hợp.
**Ghi chú:**

gcc và clang không hoàn toàn tuân thủ ISO-C, nghĩa là chúng sẽ cho phép bạn thực hiện số học trên con trỏ void. Chúng sẽ coi nó như một con trỏ char nhưng bạn không nên làm điều này vì nó có thể không hoạt động với tất cả các trình biên dịch!

## `printf` gọi write hay write gọi `printf`?
`printf` gọi `write`. `printf` sử dụng một bộ đệm nội bộ, vì vậy, để tăng hiệu suất, `printf` có thể không gọi `write` mỗi khi bạn gọi `printf`. `printf` là một hàm thư viện C. `write` là một lệnh gọi hệ thống và như chúng ta biết, các lệnh gọi hệ thống rất tốn kém. Mặt khác, `printf` sử dụng bộ đệm, điều này phù hợp hơn với nhu cầu của chúng ta trong trường hợp này.

## Làm thế nào để in ra giá trị của con trỏ? số nguyên? chuỗi?
Sử dụng các định dạng "%p" cho con trỏ, "%d" cho số nguyên và "%s" cho chuỗi.
Danh sách đầy đủ tất cả các định dạng được tìm thấy [ở đây](http://www.cplusplus.com/reference/cstdio/printf/)

Ví dụ về số nguyên:
```C
int num1 = 10;
printf("%d", num1); //in num1
```

Ví dụ về con trỏ số nguyên:
```C
int *ptr = (int *) malloc(sizeof(int));
*ptr = 10;
printf("%p\n", ptr); //in địa chỉ mà con trỏ trỏ đến
printf("%p\n", &ptr); /*in địa chỉ của biến con trỏ -- cực kỳ hữu ích
khi xử lý con trỏ kép*/
printf("%d", *ptr); //in giá trị số nguyên tại địa chỉ mà ptr trỏ đến
return 0;
```
Ví dụ về chuỗi:
```C
char *str = (char *) malloc(256 * sizeof(char));
strcpy(str, "Hello there!");
printf("%p\n", str); // in địa chỉ trong heap
printf("%s", str);
return 0;
```

[Chuỗi dưới dạng Con trỏ & Mảng @ BU](https://www.cs.bu.edu/teaching/c/string/intro/)

## Làm thế nào để bạn lưu đầu ra tiêu chuẩn vào một tệp?
Cách đơn giản nhất: chạy chương trình của bạn và sử dụng chuyển hướng shell
ví dụ:
```
./program > output.txt

#Để đọc nội dung của tệp,
cat output.txt
```
Cách phức tạp hơn: đóng stdout bằng `close(1)` và sau đó sử dụng `open` để mở lại bộ mô tả tệp, trỏ đến tệp đích.
Xem [[http://cs-education.github.io/sys/#chapter/0/section/3/activity/0]]
## Sự khác biệt giữa con trỏ và mảng là gì? Đưa ra một ví dụ về điều bạn có thể làm với cái này nhưng không thể làm với cái kia.
```C
char ary[] = "Hello";
char *ptr = "Hello";
```
Ví dụ 

Tên mảng trỏ đến byte đầu tiên của mảng. Cả `ary` và `ptr` đều có thể được sử dụng để in ra chuỗi:
```C
char ary[] = "Hello";
char *ptr = "Hello";
// In ra địa chỉ và nội dung
printf("%p : %s\n", ary, ary);
printf("%p : %s\n", ptr, ptr);
```
Mảng có thể thay đổi, vì vậy chúng ta có thể thay đổi nội dung của nó (tuy nhiên, hãy cẩn thận đừng ghi byte vượt quá giới hạn cuối của mảng). May mắn thay, 'World' không dài hơn 'Hello".

Trong trường hợp này, con trỏ char `ptr` trỏ đến một vùng bộ nhớ chỉ đọc (nơi lưu trữ chuỗi ký tự được phân bổ tĩnh), vì vậy chúng ta không thể thay đổi nội dung đó.
```C
strcpy(ary, "World"); // OK
strcpy(ptr, "World"); // KHÔNG OK - Lỗi phân đoạn (gây crash chương trình)

```
Tuy nhiên, không giống như mảng, chúng ta có thể thay đổi `ptr` để trỏ đến một vùng bộ nhớ khác,
```C
ptr = "World"; // OK!
ptr = ary; // OK!
ary = (..bất cứ thứ gì..) ; // SẼ KHÔNG BIÊN DỊCH ĐƯỢC
// ary bị ràng buộc luôn tham chiếu đến mảng ban đầu.
printf("%p : %s\n", ptr, ptr);
strcpy(ptr, "World"); // OK vì bây giờ ptr đang trỏ đến bộ nhớ có thể thay đổi (mảng)
```

Điều cần rút ra từ đây là con trỏ * có thể trỏ đến bất kỳ loại bộ nhớ nào trong khi mảng C [] chỉ có thể trỏ đến bộ nhớ trên stack. Trong trường hợp phổ biến hơn, con trỏ sẽ trỏ đến bộ nhớ heap, trong trường hợp đó bộ nhớ được tham chiếu bởi con trỏ CÓ THỂ được sửa đổi.

## `sizeof()` trả về số byte. Vì vậy, sử dụng mã trên, sizeof(ary) và sizeof(ptr) là gì?
`sizeof(ary)`: `ary` là một mảng. Trả về số byte cần thiết để lưu trữ toàn bộ mảng (5 ký tự + byte null = 6 byte)
`sizeof(ptr)`: Giống như sizeof(char *). Trả về số byte cần thiết để lưu trữ một con trỏ (ví dụ: 4 hoặc 8 cho máy 32 bit hoặc 64 bit)

`sizeof` là một toán tử đặc biệt. Thực chất đó là thứ mà trình biên dịch thay thế trước khi biên dịch chương trình vì kích thước của tất cả các kiểu dữ liệu được biết tại thời điểm biên dịch. Khi bạn có `sizeof(char*)`, nó lấy kích thước của một con trỏ trên máy của bạn (8 byte cho máy 64 bit và 4 cho máy 32 bit, v.v.). Khi bạn thử `sizeof(char[])`, trình biên dịch sẽ xem xét điều đó và thay thế nó bằng số byte mà **toàn bộ** mảng chứa vì tổng kích thước của mảng được biết tại thời điểm biên dịch.

```C
char str1[] = "sẽ là 11";
char* str2 = "sẽ là 8";
sizeof(str1) //11 vì nó là một mảng
sizeof(str2) //8 vì nó là một con trỏ
```

Hãy cẩn thận khi sử dụng sizeof để lấy độ dài của một chuỗi!

## Đoạn mã nào sau đây là không chính xác hoặc chính xác và tại sao?
```C
int* f1(int *p) {
    *p = 42;
    return p;
} // Mã này là chính xác;
```

```C
char* f2() {
    char p[] = "Hello";
    return p;
} // Không chính xác!
```
Giải thích: Một mảng p được tạo trên stack với kích thước vừa đủ để chứa các ký tự H,e,l,l,o, và một byte null, tức là (6) byte. Mảng này được lưu trữ trên stack và sẽ không còn hợp lệ sau khi chúng ta trả về từ hàm f2.
```C
char* f3() {
    char *p = "Hello";
    return p;
} // OK
```
Giải thích: p là một con trỏ. Nó chứa địa chỉ của hằng số chuỗi "Hello". Hằng số chuỗi này không thay đổi và vẫn hợp lệ ngay cả sau khi f3 trả về.

```C
char* f4() {
    static char p[] = "Hello";
    return p;
} // OK
```
Giải thích: Mảng được khai báo là static, có nghĩa là nó tồn tại trong suốt vòng đời của tiến trình (các biến tĩnh không nằm trên heap hoặc stack).

## Làm thế nào để bạn tra cứu thông tin về các lệnh gọi thư viện C và lệnh gọi hệ thống?
Sử dụng các trang manual (man pages). Lưu ý các trang manual được tổ chức thành các phần. Phần 2 = Lệnh gọi hệ thống. Phần 3 = Thư viện C.
Web: Google "man7 open"
shell: man -S2 open  hoặc man -S3 printf

## Làm thế nào để bạn cấp phát bộ nhớ trên heap?
Sử dụng hàm malloc. Ngoài ra còn có realloc và calloc.
Thường được sử dụng với sizeof. ví dụ: cấp phát đủ không gian để chứa 10 số nguyên
```C
int *space = malloc(sizeof(int) * 10);
```

## Có gì sai với đoạn mã sao chép chuỗi này?

```C
void mystrcpy(char*dest, char* src) { 
  // void có nghĩa là không có giá trị trả về   
  while( *src ) { dest = src; src ++; dest++; }  
}
```
Trong đoạn mã trên, nó chỉ đơn giản là thay đổi con trỏ dest để trỏ đến chuỗi nguồn. Ngoài ra, byte null kết thúc chuỗi cũng không được sao chép. Đây là một phiên bản tốt hơn:
```
  while( *src ) { *dest = *src; src ++; dest++; } 
  *dest = *src;
```
Lưu ý rằng cũng thường thấy kiểu triển khai sau, thực hiện mọi thứ bên trong điều kiện của vòng lặp while, bao gồm cả việc sao chép byte null.
```C
  while( (*dest++ = *src++ )) {};
```

## Làm thế nào để bạn viết một hàm thay thế cho strdup?
```C
// Sử dụng strlen+1 để tính cả byte null... 
char* mystrdup(char*source) {
   char *p = (char *) malloc ( strlen(source)+1 );
   strcpy(p,source);
   return p;
}
```

## Làm thế nào để bạn giải phóng bộ nhớ trên heap?
Sử dụng hàm free!
```C
int *n = (int *) malloc(sizeof(int));
*n = 10;
//Làm một số việc
free(n);
```

## Lỗi double free là gì? Làm thế nào bạn có thể tránh? Con trỏ lơ lửng là gì? Làm thế nào để bạn tránh?
Lỗi double free xảy ra khi bạn vô tình cố gắng giải phóng cùng một vùng bộ nhớ đã được cấp phát hai lần.
```C
int *p = malloc(sizeof(int));
free(p);

*p = 123; // Oops! - Con trỏ lơ lửng! Ghi vào bộ nhớ mà chúng ta không còn sở hữu nữa

free(p); // Oops! - Double free!
```

Cách khắc phục trước tiên là viết các chương trình chính xác! Thứ hai, nên tạo thói quen đặt lại con trỏ
về NULL sau khi bộ nhớ đã được giải phóng. Điều này đảm bảo con trỏ không thể bị sử dụng sai mà không làm chương trình bị crash.

Sửa chữa:
```C
p = NULL; // Bây giờ bạn không thể sử dụng con trỏ này do nhầm lẫn nữa
```

## Ví dụ về tràn bộ đệm là gì?
Ví dụ nổi tiếng: Lỗ hổng Heartbleed (thực hiện memcpy vào một bộ đệm có kích thước không đủ).
Ví dụ đơn giản: triển khai hàm strcpy và quên cộng thêm 1 vào kết quả của strlen khi xác định kích thước bộ nhớ cần thiết.

## 'typedef' là gì và bạn sử dụng nó như thế nào? 
`typedef` được sử dụng để khai báo một bí danh cho một kiểu dữ liệu. Thường được sử dụng với struct để giảm sự lộn xộn về mặt hình ảnh khi phải viết 'struct' như một phần của kiểu.
```C
typedef float real; 
real gravity = 10;
// typedef cũng cung cấp cho chúng ta một lớp trừu tượng trên kiểu dữ liệu cơ bản được sử dụng. 
// Trong tương lai, chúng ta chỉ cần thay đổi typedef này nếu chúng ta
// muốn thư viện vật lý của mình sử dụng double thay vì float.

typedef struct link link_t; 
//Với struct, hãy bao gồm từ khóa 'struct' như một phần của kiểu dữ liệu gốc
```

Trong lớp học này, chúng ta thường xuyên sử dụng typedef cho các hàm. Một typedef cho một hàm có thể như sau:

```C
typedef int (*comparator)(void*,void*);

int greater_than(void* a, void* b){
    return a > b;
}
comparator gt = greater_than;
```

Điều này khai báo một kiểu hàm có tên là `comparator`, nhận hai tham số `void*` và trả về một số nguyên.

## Wow, đó là rất nhiều kiến thức về C
Đừng lo lắng, còn nhiều điều thú vị hơn nữa đang chờ đón bạn!

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output">
Tiếp theo: Lập trình C, Phần 2: Nhập và Xuất Văn bản
</a>
</div>