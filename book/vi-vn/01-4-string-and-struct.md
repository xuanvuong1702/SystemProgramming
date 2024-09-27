---
title: "Chương 01: Học Ngôn Ngữ Lập Trình C, Phần 4: Chuỗi và Cấu Trúc"
type: book-vi-vn
order: 4
---

# Vậy chuỗi là gì?

![Chuỗi](https://i.imgur.com/CgsxyZb.png)

Trong C, chúng ta có chuỗi [Kết thúc bằng Null](https://en.wikipedia.org/wiki/Null-terminated_string) thay vì [Tiền tố Độ dài](https://en.wikipedia.org/wiki/String_(computer_science)#Length-prefixed) vì lý do lịch sử. Điều đó có nghĩa là trong lập trình hàng ngày, bạn cần nhớ đến ký tự null! Một chuỗi trong C được định nghĩa là một dãy byte cho đến khi bạn gặp '\0' hoặc **Byte Null**.

## Hai nơi lưu trữ chuỗi

Khi bạn định nghĩa một **chuỗi hằng** (tức là một chuỗi có dạng `char* str = "constant"`), chuỗi đó được lưu trữ trong phân đoạn _dữ liệu_ hoặc _mã_ mà là **chỉ đọc**, nghĩa là bất kỳ cố gắng nào để sửa đổi chuỗi sẽ gây ra lỗi **segfault** (lỗi phân đoạn).

Tuy nhiên, nếu bạn `malloc` không gian, bạn có thể thay đổi chuỗi đó thành bất cứ thứ gì bạn muốn.

## Quản lý bộ nhớ sai

Một lỗi thường gặp là khi bạn viết như sau:

```C
char* hello_string = malloc(14);
                       ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
// hello_string ----> | g | a | r | b | a | g | e | g | a | r | b | a | g | e |
                       ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
hello_string = "Hello Person!";
// (chuỗi hằng trong phân đoạn văn bản)
// hello_string ----> [ "H" , "e" , "l" , "l" , "o" , " " , "P" , "e" , "r" , "s" , "o" , "n" , "!" , "\0" ]
                       ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
// rò rỉ bộ nhớ -----> | g | a | r | b | a | g | e | g | a | r | b | a | g | e |
                       ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
hello_string[9] = 't'; //segfault!!
```
Chúng ta đã làm gì? Chúng ta đã cấp phát không gian cho 14 byte, gán lại con trỏ cho một chuỗi hằng và gây ra lỗi segfault! Hãy nhớ theo dõi những gì con trỏ của bạn đang làm. Điều bạn có thể muốn làm là sử dụng hàm `strcpy` từ `string.h`.
```C
strcpy(hello_string, "Hello Person!");
```

## Nhớ đến byte NULL!

Quên kết thúc chuỗi bằng NULL có ảnh hưởng lớn đến chuỗi! Kiểm tra giới hạn là rất quan trọng. Lỗi Heartbleed được đề cập trước đây trong sách wiki một phần là do lỗi này.

## Tôi có thể tìm thấy một giải thích chi tiết và đầy đủ về tất cả các hàm này ở đâu?

[Tại đây!](https://linux.die.net/man/3/string)

## Thông tin/So sánh Chuỗi: `strlen` `strcmp`

* `int strlen(const char *s)` trả về độ dài của chuỗi **không bao gồm** byte null.
* `int strcmp(const char *s1, const char *s2)` trả về một số nguyên xác định thứ tự từ điển của hai chuỗi. 
    * Nếu `s1` đứng trước `s2` trong từ điển, thì hàm trả về `-1`. 
    * Nếu hai chuỗi bằng nhau, thì trả về `0`. 
    * Ngược lại, trả về `1`.

Với hầu hết các hàm này, chúng mong đợi chuỗi có thể đọc được và khác `NULL`. Sẽ có hành vi không xác định khi bạn truyền `NULL` cho chúng.

## Thay đổi Chuỗi: `strcpy` `strcat` `strdup`

* `char *strcpy(char *dest, const char *src)` Sao chép chuỗi tại `src` vào `dest`. **Giả định `dest` có đủ không gian cho `src`**.
* `char *strcat(char *dest, const char *src)` Nối chuỗi tại `src` vào cuối chuỗi đích. **Hàm này giả định rằng có đủ không gian cho `src` ở cuối chuỗi đích, bao gồm cả byte NULL**.
* `char *strdup(const char *s)` Trả về một bản sao của chuỗi `s` đã được `malloc`.

## Tìm kiếm Chuỗi: `strchr` `strstr`

* `char *strchr(const char *haystack, int needle)` Trả về một con trỏ đến lần xuất hiện đầu tiên của ký tự `needle` trong chuỗi `haystack`. Nếu không tìm thấy, trả về `NULL`.
* `char *strstr(const char *haystack, const char *needle)` Tương tự như trên nhưng tìm kiếm một chuỗi `needle` trong chuỗi `haystack`.

## Tách chuỗi thành các token: `strtok`

Hàm `strtok` nhận một chuỗi và tách nó thành các token, nghĩa là nó sẽ biến đổi chuỗi thành các chuỗi riêng biệt. Hàm này có nhiều chi tiết kỹ thuật, vì vậy hãy đọc trang hướng dẫn (man page). Dưới đây là một ví dụ:

```C
#include <stdio.h>
#include <string.h>

int main(){
    char* upped = strdup("strtok,is,tricky,!!");
    char* start = strtok(upped, ",");
    do{
        printf("%s\n", start);
    }while((start = strtok(NULL, ",")));
    return 0;
}
```

**Kết quả:**
```
strtok
is
tricky
!!
```

Điều gì xảy ra khi bạn thay đổi `upped` như thế này?
```C
char* upped = strdup("strtok,is,tricky,,,!!");
```

## Di chuyển bộ nhớ: `memcpy` và `memmove`

Tại sao cả `memcpy` và `memmove` đều nằm trong `<string.h>`? Bởi vì chuỗi về cơ bản là bộ nhớ thô với một byte null ở cuối!

* `void *memcpy(void *dest, const void *src, size_t n)` sao chép `n` byte bắt đầu từ `src` vào `dest`. **Hãy cẩn thận**, sẽ có hành vi không xác định khi hai vùng nhớ chồng chéo. Đây là một trong những ví dụ điển hình về "hoạt động trên máy của tôi" (works on my machine) bởi vì nhiều khi Valgrind sẽ không thể phát hiện ra lỗi này, vì nó có vẻ như hoạt động bình thường trên máy của bạn. Khi nộp bài, nó có thể sẽ bị lỗi. Hãy xem xét phiên bản an toàn hơn là:
* `void *memmove(void *dest, const void *src, size_t n)` thực hiện chức năng tương tự như trên, nhưng nếu hai vùng nhớ chồng chéo thì nó đảm bảo rằng tất cả các byte sẽ được sao chép chính xác.

# Vậy `struct` là gì?

Ở cấp độ thấp, một `struct` chỉ là một vùng nhớ liên tục, không hơn. Giống như mảng, `struct` có đủ không gian để chứa tất cả các thành viên của nó. Nhưng không giống như mảng, nó có thể chứa các kiểu dữ liệu khác nhau. Hãy xem xét struct `contact` được khai báo dưới đây:

```C
struct contact {
    char firstname[20];
    char lastname[20];
    unsigned int phone;
};

struct contact person;
```

**Lưu ý:**
```C
/* Nhiều khi chúng ta sẽ sử dụng typedef như sau 
để có thể viết `contact person` thay vì `struct contact person` */

typedef struct contact contact;
contact person;

/* Bạn cũng có thể khai báo struct như sau để hoàn thành 
trong một câu lệnh */
typedef struct optional_name {
    ...
} contact;
```

Nếu bạn biên dịch mã mà không có bất kỳ tối ưu hóa và sắp xếp lại nào, bạn có thể mong đợi địa chỉ của mỗi biến sẽ trông như thế này:

```C
&person           // 0x100
&person.firstname // 0x100 = 0x100 + 0x00
&person.lastname  // 0x114 = 0x100 + 0x14
&person.phone     // 0x128 = 0x100 + 0x28
```

Bởi vì tất cả những gì trình biên dịch làm là nói "dành ra vùng nhớ này, và tôi sẽ tính toán offset của bất kỳ biến nào bạn muốn ghi vào".

## Ý nghĩa của các offset?

Offset là vị trí bắt đầu của biến trong struct. Ví dụ, biến `phone` bắt đầu ở byte thứ `0x128` và tiếp tục cho `sizeof(int)` byte, nhưng không phải lúc nào cũng vậy. **Offset không xác định nơi biến kết thúc**. Hãy xem xét kỹ thuật sau đây mà bạn thường thấy trong mã kernel:

```C
typedef struct {
    int length;
    char c_str[0];
} string;

const char* to_convert = "person";
int length = strlen(to_convert);

// Chuyển đổi thành chuỗi C
string* person;
person = malloc(sizeof(string) + length + 1);
/*
Hiện tại, bộ nhớ của chúng ta trông như thế này với rác trong những khoảng trống:
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
 person = |   |   |   |   |   |   |   |   |   |   |   |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
*/


person->length = length;
/*
Điều này ghi các giá trị sau vào 4 byte đầu tiên:
Phần còn lại vẫn là rác:
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
 person = | 0 | 0 | 0 | 6 |   |   |   |   |   |   |   |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
*/


strcpy(person->c_str, to_convert);
/*
Bây giờ chuỗi của chúng ta được điền đúng vào cuối struct:

           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ____
 person = | 0 | 0 | 0 | 6 | p | e | r | s | o | n | \0 |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾‾
*/

strcmp(person->c_str, "person") == 0; // Hai chuỗi bằng nhau!
```

## Nhưng không phải tất cả các struct đều hoàn hảo

Struct có thể yêu cầu một kỹ thuật gọi là [padding](http://www.catb.org/esr/structure-packing/) (xem hướng dẫn). **Chúng tôi không mong đợi bạn phải đóng gói các struct trong khóa học này, chỉ cần biết rằng nó tồn tại.** Điều này là bởi vì trong những ngày đầu (và ngay cả bây giờ) khi bạn truy cập một địa chỉ từ bộ nhớ, bạn phải thực hiện theo các khối 32 bit hoặc 64 bit. Điều này cũng có nghĩa là bạn chỉ có thể yêu cầu các địa chỉ là bội số của kích thước khối đó. Ví dụ:

```C
struct picture{
    int height;
    pixel** data;
    int width;
    char* encoding;
};
// Bạn nghĩ picture trông như thế này:
           height      data         width     encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |               |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```

Về mặt khái niệm, nó có thể trông như thế này (trên hệ thống 64-bit):

```C
struct picture{
    int height;
    char slop1[4]; // padding
    pixel** data;
    int width;
    char slop2[4]; // padding
    char* encoding;
};
           height   slop1       data        width   slop2  encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |       |               |       |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```
Đây là trên hệ thống 64-bit. Điều này không phải lúc nào cũng xảy ra vì đôi khi bộ xử lý của bạn hỗ trợ truy cập không căn chỉnh. Điều này có nghĩa là gì? 

Có hai lựa chọn: bạn có thể sử dụng thuộc tính `packed`:
```C
struct __attribute__((packed, aligned(4))) picture{
    int height;
    pixel** data;
    int width;
    char* encoding;
};
// Sẽ trông như thế này:
           height       data        width     encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |               |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```
Tuy nhiên, mỗi khi bạn muốn truy cập `data` hoặc `encoding`, bạn sẽ phải thực hiện hai lần truy cập bộ nhớ.

Lựa chọn khác là sắp xếp lại các thành viên trong struct, mặc dù điều này không phải lúc nào cũng khả thi:

```C
struct picture{
    int height;
    int width;
    pixel** data;
    char* encoding;
};
// Bạn nghĩ picture trông như thế này:
           height   width        data         encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |       |               |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-3:-Common-Gotchas">
Quay lại: Học Ngữ Lập Trình C, Phần 3: Các Lỗi Thường Gặp
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Tiếp theo: Học Ngữ Lập Trình C, Phần 5: Gỡ lỗi
</a>
</div>
