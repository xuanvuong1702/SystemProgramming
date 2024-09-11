---
title: "Chương 01: Học ngữ lập trình C"
type: book-vi-vn
order: 4
---



# Chuỗi, Cấu trúc, và Những điểm cần chú ý

# Vậy chuỗi là gì?

![Chuỗi](https://i.imgur.com/CgsxyZb.png)

Trong C, chúng ta có chuỗi [Kết thúc bằng Null](https://en.wikipedia.org/wiki/Null-terminated_string) thay vì [Tiền tố Độ dài](https://en.wikipedia.org/wiki/String_(computer_science)#Length-prefixed) vì lý do lịch sử. Điều đó có nghĩa là trong lập trình hàng ngày, bạn cần nhớ đến ký tự null! Một chuỗi trong C được định nghĩa là một chuỗi byte cho đến khi bạn gặp '\0' hoặc Byte Null.

## Hai nơi cho chuỗi

Khi bạn định nghĩa một chuỗi hằng số (tức là một chuỗi dạng `char* str = "constant"`), chuỗi đó được lưu trữ trong phân đoạn _data_ hoặc _code_ mà là **chỉ đọc** nghĩa là bất kỳ cố gắng nào để sửa đổi chuỗi sẽ gây ra lỗi segfault.

Tuy nhiên, nếu bạn `malloc` không gian, bạn có thể thay đổi chuỗi đó thành bất cứ thứ gì bạn muốn.

## Quản lý Bộ nhớ sai lầm

Một điểm cần chú ý phổ biến là khi bạn viết như sau

```C
char* hello_string = malloc(14);
                       ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
// hello_string ----> | g | a | r | b | a | g | e | g | a | r | b | a | g | e |
                       ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
hello_string = "Hello Person!";
// (chuỗi hằng số trong phân đoạn văn bản)
// hello_string ----> [ "H" , "e" , "l" , "l" , "o" , " " , "P" , "e" , "r" , "s" , "o" , "n" , "!" , "\0" ]
                       ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
// memory_leak -----> | g | a | r | b | a | g | e | g | a | r | b | a | g | e |
                       ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
hello_string[9] = 't'; //segfault!!
```
Chúng ta đã làm gì? Chúng ta đã cấp phát không gian cho 14 byte, gán lại con trỏ và gây ra lỗi segfault thành công! Hãy nhớ theo dõi những gì con trỏ của bạn đang làm. Điều bạn có thể muốn làm là sử dụng hàm `strcpy` từ `string.h`.
```C
strcpy(hello_string, "Hello Person!");
```

## Nhớ đến byte NULL!

Quên kết thúc chuỗi bằng NULL có ảnh hưởng lớn đến chuỗi! Kiểm tra giới hạn là quan trọng. Lỗi heart bleed được đề cập trước đây trong sách wiki một phần là do điều này.

## Tôi có thể tìm thấy một giải thích chi tiết và bao quát về tất cả các hàm này ở đâu?

[Tại đây!](https://linux.die.net/man/3/string)

## Thông tin/So sánh Chuỗi: `strlen` `strcmp`

`int strlen(const char *s)` trả về độ dài của chuỗi không bao gồm byte null

`int strcmp(const char *s1, const char *s2)` trả về một số nguyên xác định thứ tự từ điển của các chuỗi. Nếu s1 đến trước s2 trong một từ điển, thì trả về -1. Nếu hai chuỗi bằng nhau, thì trả về 0. Ngược lại, trả về 1.

Với hầu hết các hàm này, chúng mong đợi chuỗi có thể đọc được và không phải NULL nhưng sẽ có hành vi không xác định khi bạn truyền cho chúng NULL.

## Thay đổi Chuỗi: `strcpy` `strcat` `strdup`

`char *strcpy(char *dest, const char *src)` Sao chép chuỗi tại `src` đến `dest`. **giả định dest có đủ không gian cho src**

`char *strcat(char *dest, const char *src)` Nối chuỗi tại `src` vào cuối đích. **Hàm này giả định rằng có đủ không gian cho `src` ở cuối đích bao gồm cả byte NULL**

`char *strdup(const char *dest)` Trả về một bản sao của chuỗi đã `malloc`.

## Tìm kiếm Chuỗi: `strchr` `strstr`

`char *strchr(const char *haystack, int needle)` Trả về một con trỏ đến lần xuất hiện đầu tiên của `needle` trong `haystack`. Nếu không tìm thấy, trả về `NULL`.

`char *strstr(const char *haystack, const char *needle)` Giống như trên nhưng lần này là một chuỗi!

## Tokenize Chuỗi: `strtok`

Một hàm nguy hiểm nhưng hữu ích, strtok lấy một chuỗi và tokenize nó. Nghĩa là nó sẽ biến đổi các chuỗi thành các chuỗi riêng biệt. Hàm này có rất nhiều thông số kỹ thuật nên hãy đọc trang man, một ví dụ tạo ra là dưới đây.

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

**Kết quả**
```C
strtok
is
tricky
!!
```

Điều gì xảy ra khi tôi thay đổi `upped` như thế này?
```C
char* upped = strdup("strtok,is,tricky,,,!!");
```

## Di chuyển Bộ nhớ: `memcpy` và `memmove`

Tại sao `memcpy` và `memmove` cả hai đều nằm trong `<string.h>`? Bởi vì chuỗi về cơ bản là bộ nhớ thô với một byte null ở cuối chúng!

`void *memcpy(void *dest, const void *src, size_t n)` di chuyển `n` byte bắt đầu từ `src` đến `dest`. **Hãy cẩn thận**, sẽ có hành vi không xác định khi các vùng bộ nhớ chồng chéo. Đây là một trong những ví dụ điển hình về máy của tôi hoạt động bởi vì nhiều lần valgrind sẽ không thể nhận nó vì nó sẽ trông như nó hoạt động trên máy của bạn. Khi autograder đánh, thất bại. Hãy xem xét phiên bản an toàn hơn là.

`void *memmove(void *dest, const void *src, size_t n)` làm cùng một việc như trên, nhưng nếu các vùng bộ nhớ chồng chéo thì nó đảm bảo rằng tất cả các byte sẽ được sao chép đúng.

# Vậy `struct` là gì?

Về mặt cấp thấp, một struct chỉ là một phần bộ nhớ liên tục, không hơn. Giống như một mảng, một struct có đủ không gian để giữ tất cả các thành viên của nó. Nhưng không giống như một mảng, nó có thể lưu trữ các loại khác nhau. Xem xét struct liên hệ được khai báo ở trên

```C
struct contact {
    char firstname[20];
    char lastname[20];
    unsigned int phone;
};

struct contact person;
```

**Phần bên**
```C
/* nhiều lần chúng ta sẽ làm như sau typedef
 để chúng ta có thể chỉ viết contact contact1 */

typedef struct contact contact;
contact person;

/* Bạn cũng có thể khai báo struct như thế này để hoàn thành
 nó trong một câu lệnh */
typedef struct optional_name {
    ...
} contact;
```

Nếu bạn biên dịch mã mà không có bất kỳ tối ưu hóa và sắp xếp lại nào, bạn có thể mong đợi các địa chỉ của mỗi biến sẽ trông như thế này.

```C
&person           // 0x100
&person.firstname // 0x100 = 0x100+0x00
&person.lastname  // 0x114 = 0x100+0x14
&person.phone     // 0x128 = 0x100+0x28
```

Bởi vì tất cả những gì trình biên dịch của bạn làm là nói 'hey dự trữ bao nhiêu không gian này, và tôi sẽ đi và tính toán các offset của bất kỳ biến nào bạn muốn ghi vào'.

## Những offset này có nghĩa là gì?

Các offset là nơi biến bắt đầu. Biến phone bắt đầu ở byte `0x128` và tiếp tục cho sizeof(int) byte, nhưng không phải lúc nào cũng vậy. **Offset không xác định nơi biến kết thúc**. Xem xét hack sau mà bạn thấy trong rất nhiều mã kernel.

```C

typedef struct {
    int length;
    char c_str[0];
} string;

const char* to_convert = "person";
int length = strlen(to_convert);

// Chuyển đổi thành chuỗi c
string* person;
person = malloc(sizeof(string) + length+1);
/*
Hiện tại, bộ nhớ của chúng ta trông như thế này với rác trong những không gian đen
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
 person = |   |   |   |   |   |   |   |   |   |   |   |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
*/


person->length = length;
/*
Điều này viết các giá trị sau đến bốn byte đầu tiên
Phần còn lại vẫn là rác
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
 person = | 0 | 0 | 0 | 6 |   |   |   |   |   |   |   |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
*/


strcpy(person->c_str, to_convert);
/*
Bây giờ chuỗi của chúng ta được điền đúng ở cuối struct

           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ____
 person = | 0 | 0 | 0 | 6 | p | e | r | s | o | n | \0 |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾‾
*/

strcmp(person->c_str, "person") == 0 //Các chuỗi bằng nhau!
```

## Nhưng không phải tất cả các struct đều hoàn hảo

Struct có thể yêu cầu một thứ gọi là [padding](http://www.catb.org/esr/structure-packing/) (hướng dẫn). **Chúng tôi không mong đợi bạn đóng gói các struct trong khóa học này, chỉ biết rằng nó tồn tại. Điều này là bởi vì trong những ngày đầu (và ngay cả bây giờ) khi bạn phải lấy một địa chỉ từ bộ nhớ, bạn phải làm nó trong các khối 32bit hoặc 64bit. Điều này cũng có nghĩa là bạn chỉ có thể yêu cầu các địa chỉ là bội số của số đó. Điều đó có nghĩa là

```C
struct picture{
    int height;
    pixel** data;
    int width;
    char* enconding;
}
// Bạn nghĩ picture trông như thế này
           height      data         width     encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |               |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```

Sẽ có vẻ như thế này về mặt khái niệm

```C
struct picture{
    int height;
    char slop1[4];
    pixel** data;
    int width;
    char slop2[4];
    char* enconding;
}
           height   slop1       data        width   slop2  encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |       |               |       |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```
Đây là trên một hệ thống 64-bit. Điều này không phải lúc nào cũng là trường hợp bởi vì đôi khi bộ xử lý của bạn hỗ trợ truy cập không cần căn chỉnh. Điều này có nghĩa là gì? Chà, có hai lựa chọn, bạn có thể đặt một thuộc tính

```C
struct __attribute__((packed, aligned(4))) picture{
    int height;
    pixel** data;
    int width;
    char* enconding;
}
// Sẽ trông như thế này
           height       data        width     encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |               |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```
Nhưng bây giờ, mỗi khi tôi muốn truy cập `data` hoặc `encoding`, tôi phải thực hiện hai truy cập bộ nhớ. Điều khác bạn có thể làm là sắp xếp lại struct, mặc dù điều này không phải lúc nào cũng có thể

```C
struct picture{
    int height;
    int width;
    pixel** data;
    char* enconding;
}
// Bạn nghĩ picture trông như thế này
           height   width        data         encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |       |               |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```


<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-3:-Common-Gotchas">
Back: C Programming, Part 3: Common Gotchas
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Next: C Programming, Part 5: Debugging
</a>
</div>
