# Chuỗi, Cấu trúc và Các lỗi thường gặp

# Vậy chuỗi là gì?

![Chuỗi](https://i.imgur.com/CgsxyZb.png)

Trong C, chúng ta có chuỗi [Kết thúc bằng Null](https://en.wikipedia.org/wiki/Null-terminated_string) thay vì [Tiền tố độ dài](https://en.wikipedia.org/wiki/String_(computer_science)#Length-prefixed) vì lý do lịch sử. Điều đó có nghĩa là đối với lập trình hàng ngày trung bình của bạn, bạn cần phải nhớ ký tự null! Một chuỗi trong C được định nghĩa là một loạt các byte cho đến khi bạn gặp '\0' hoặc **Byte Null**.

## Hai nơi lưu trữ cho chuỗi

Bất cứ khi nào bạn định nghĩa một chuỗi hằng (ví dụ: một chuỗi ở dạng `char* str = "constant"`) Chuỗi đó được lưu trữ trong phân đoạn _dữ liệu_ hoặc _mã_ là **chỉ đọc** có nghĩa là bất kỳ nỗ lực nào để sửa đổi chuỗi sẽ gây ra lỗi segfault.

Tuy nhiên, nếu cấp phát không gian bằng `malloc`, bạn có thể thay đổi chuỗi đó thành bất cứ thứ gì bạn muốn.

## Quản lý bộ nhớ sai

Một lỗi thường gặp là khi bạn viết như sau

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

Bạn đã làm gì? Bạn đã cấp phát không gian cho 14 byte, gán lại con trỏ và segfault thành công! Hãy nhớ theo dõi những gì con trỏ của bạn đang làm. Những gì bạn có thể muốn làm là sử dụng hàm `strcpy` của `string.h`.
```C
strcpy(hello_string, "Hello Person!");
```

## Hãy nhớ byte NULL!

Quên kết thúc chuỗi bằng NULL có ảnh hưởng lớn đến chuỗi! Kiểm tra giới hạn là quan trọng. Lỗi heartbleed được đề cập trước đó trong wiki một phần là do điều này.

## Tôi có thể tìm thấy giải thích chuyên sâu và toàn diện về tất cả các hàm này ở đâu?

[Ngay tại đây!](https://linux.die.net/man/3/string)

## Thông tin/So sánh chuỗi: `strlen` `strcmp`

`int strlen(const char *s)` trả về độ dài của chuỗi không bao gồm byte null.

`int strcmp(const char *s1, const char *s2)` trả về một số nguyên xác định thứ tự từ vựng của các chuỗi. Nếu `s1` đứng trước `s2` trong từ điển, thì -1 được trả về. Nếu hai chuỗi bằng nhau, thì là 0. Ngược lại, là 1. 

Với hầu hết các hàm này, chúng mong đợi các chuỗi có thể đọc được và không phải là NULL nhưng có hành vi không xác định khi bạn truyền NULL cho chúng.

## Thay đổi chuỗi: `strcpy` `strcat` `strdup`

`char *strcpy(char *dest, const char *src)` Sao chép chuỗi tại `src` đến `dest`. **Giả định `dest` có đủ không gian cho `src`**.

`char *strcat(char *dest, const char *src)` Nối chuỗi tại `src` vào cuối chuỗi đích. **Hàm này giả định rằng có đủ không gian cho `src` ở cuối chuỗi đích bao gồm cả byte NULL**.

`char *strdup(const char *dest)` Trả về một bản sao của chuỗi được cấp phát bằng `malloc`.

## Tìm kiếm chuỗi: `strchr` `strstr`

`char *strchr(const char *haystack, int needle)` Trả về một con trỏ đến lần xuất hiện đầu tiên của `needle` trong `haystack`. Nếu không tìm thấy, `NULL` được trả về.

`char *strstr(const char *haystack, const char *needle)` Giống như trên nhưng lần này là một chuỗi!

## Tách chuỗi: `strtok`

Một hàm nguy hiểm nhưng hữu ích `strtok` nhận một chuỗi và tách nó. Có nghĩa là nó sẽ biến đổi các chuỗi thành các chuỗi riêng biệt. Hàm này có rất nhiều thông số kỹ thuật, vì vậy vui lòng đọc các trang hướng dẫn, một ví dụ được tạo ra bên dưới.

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

**Đầu ra**
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

## Di chuyển bộ nhớ: `memcpy` và `memmove`

Tại sao cả `memcpy` và `memmove` đều nằm trong `<string.h>`? Bởi vì chuỗi về cơ bản là bộ nhớ thô với một byte null ở cuối chúng!

`void *memcpy(void *dest, const void *src, size_t n)` di chuyển `n` byte bắt đầu từ `src` đến `dest`. **Hãy cẩn thận**, có hành vi không xác định khi các vùng nhớ chồng lấp. Đây là một trong những ví dụ kinh điển về **hoạt động trên máy của tôi** vì nhiều lần Valgrind sẽ không thể nhận ra nó vì nó sẽ trông giống như nó hoạt động trên máy của bạn. Khi chấm điểm tự động, nó sẽ thất bại. Hãy xem xét phiên bản an toàn hơn là.

`void *memmove(void *dest, const void *src, size_t n)` làm điều tương tự như trên, nhưng nếu các vùng nhớ chồng lấp thì đảm bảo rằng tất cả các byte sẽ được sao chép đúng cách.

# Vậy `struct` là gì?

Ở cấp độ thấp, một `struct` chỉ là một phần của bộ nhớ liền kề, không hơn không kém. Giống như một mảng, một `struct` có đủ không gian để lưu giữ tất cả các thành viên của nó. Nhưng không giống như một mảng, nó có thể lưu trữ các kiểu dữ liệu khác nhau. Hãy xem xét `struct contact` được khai báo ở trên

```C
struct contact {
    char firstname[20];
    char lastname[20];
    unsigned int phone;
};

struct contact person;
```

**Ngoại lệ ngắn gọn**
```C
/* rất nhiều lần chúng ta sẽ thực hiện typedef sau
 để chúng ta có thể chỉ cần viết contact contact1 */

typedef struct contact contact;
contact person;

/* Bạn cũng có thể khai báo struct như thế này để hoàn thành
 nó trong một câu lệnh */
typedef struct optional_name {
    ...
} contact;
```

Nếu bạn biên dịch mã mà không có bất kỳ tối ưu hóa và sắp xếp lại nào, bạn có thể mong đợi địa chỉ của mỗi biến sẽ trông như thế này.

```C
&person           // 0x100
&person.firstname // 0x100 = 0x100+0x00
&person.lastname  // 0x114 = 0x100+0x14
&person.phone     // 0x128 = 0x100+0x28
```

Bởi vì tất cả những gì trình biên dịch của bạn làm là nói 'này, hãy dành chỗ này, và tôi sẽ đi và tính toán độ lệch của bất kỳ biến nào bạn muốn ghi vào'.

## Các độ lệch này có nghĩa là gì?

Các **độ lệch** là vị trí bắt đầu của biến. Biến `phone` bắt đầu tại byte `0x128` và tiếp tục cho `sizeof(int)` byte, nhưng không phải lúc nào cũng vậy. **Tuy nhiên, độ lệch không xác định vị trí kết thúc của biến**. Hãy xem xét thủ thuật sau đây mà bạn thấy trong rất nhiều mã nhân.

```C

typedef struct {
    int length;
    char c_str[0];
} string;

const char* to_convert = "person";
int length = strlen(to_convert);

// Hãy chuyển đổi thành chuỗi c
string* person;
person = malloc(sizeof(string) + length+1);
/*
Hiện tại, bộ nhớ của chúng ta trông như thế này với rác trong những khoảng đen đó
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
 person = |   |   |   |   |   |   |   |   |   |   |   |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
*/


person->length = length;
/*
Điều này ghi các giá trị sau vào bốn byte đầu tiên
Phần còn lại vẫn là rác
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
 person = | 0 | 0 | 0 | 6 |   |   |   |   |   |   |   |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
*/


strcpy(person->c_str, to_convert);
/*
Bây giờ chuỗi của chúng ta được điền chính xác vào cuối struct

           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ____
 person = | 0 | 0 | 0 | 6 | p | e | r | s | o | n | \0 |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾‾
*/

strcmp(person->c_str, "person") == 0 //Các chuỗi bằng nhau!
```

## Nhưng không phải tất cả các struct đều hoàn hảo

Struct có thể yêu cầu một thứ gọi là [padding](http://www.catb.org/esr/structure-packing/) (hướng dẫn). **Chúng tôi không mong đợi bạn đóng gói struct trong khóa học này, chỉ cần biết rằng nó có ở đó.** Điều này là do trong những ngày đầu (và thậm chí bây giờ) khi bạn phải lấy một địa chỉ từ bộ nhớ, bạn phải làm điều đó trong các khối 32 bit hoặc 64 bit. Điều này cũng có nghĩa là bạn chỉ có thể yêu cầu các địa chỉ là bội số của điều đó. Có nghĩa là

```C
struct picture{
    int height;
    pixel** data;
    int width;
    char* encoding;
}
// Bạn nghĩ picture trông như thế này
           height      data         width     encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |               |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```

Về mặt khái niệm sẽ trông như thế này

```C
struct picture{
    int height;
    char slop1[4];
    pixel** data;
    int width;
    char slop2[4];
    char* encoding;
}
           height   slop1       data        width   slop2  encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |       |               |       |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```
Đây là trên hệ thống 64 bit. Điều này không phải lúc nào cũng đúng vì đôi khi bộ xử lý của bạn hỗ trợ truy cập không thẳng hàng. Điều này có nghĩa là gì? Chà, có hai tùy chọn: bạn có thể đặt một thuộc tính

```C
struct __attribute__((packed, aligned(4))) picture{
    int height;
    pixel** data;
    int width;
    char* encoding;
}
// Sẽ trông như thế này
           height       data        width     encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |               |       |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```
Nhưng bây giờ, mỗi khi bạn muốn truy cập `data` hoặc `encoding`, bạn phải thực hiện hai lần truy cập bộ nhớ. Điều khác bạn có thể làm là sắp xếp lại `struct`, mặc dù điều này không phải lúc nào cũng khả thi.

```C
struct picture{
    int height;
    int width;
    pixel** data;
    char* encoding;
}
// Bạn nghĩ picture trông như thế này
           height   width        data         encoding
           ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___ ___
picture = |       |       |               |               |
           ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾ ‾‾‾
```


<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-3:-Common-Gotchas">
Trở lại: Lập trình C, Phần 3: Các lỗi thường gặp
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Tiếp theo: Lập trình C, Phần 5: Gỡ lỗi
</a>
</div>
