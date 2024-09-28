Mỗi luồng sử dụng một **bộ nhớ ngăn xếp**. Ngăn xếp 'phát triển xuống dưới' - nếu một hàm gọi một hàm khác, thì ngăn xếp được mở rộng đến các địa chỉ bộ nhớ nhỏ hơn.
Bộ nhớ ngăn xếp bao gồm các **biến tự động** (tạm thời) phi tĩnh, giá trị **tham số** và **địa chỉ trả về**.
Nếu **bộ đệm** quá nhỏ, một số dữ liệu (ví dụ: giá trị đầu vào từ người dùng), thì có khả năng thực sự là các biến ngăn xếp khác và thậm chí cả địa chỉ trả về sẽ bị **ghi đè**.
Bố cục chính xác của nội dung ngăn xếp và thứ tự của các biến tự động phụ thuộc vào kiến ​​trúc và trình biên dịch. Tuy nhiên, với một chút công việc điều tra, chúng ta có thể tìm hiểu cách **phá hủy ngăn xếp** một cách có chủ ý cho một kiến ​​trúc cụ thể.

Ví dụ dưới đây trình bày cách địa chỉ trả về được lưu trữ trên ngăn xếp. Đối với một kiến ​​trúc 32 bit cụ thể [Máy Linux Trực tiếp](http://cs-education.github.io/sys/), chúng tôi xác định rằng địa chỉ trả về được lưu trữ tại một địa chỉ hai con trỏ (8 byte) phía trên địa chỉ của biến tự động. Mã cố tình thay đổi giá trị ngăn xếp để khi hàm đầu vào trả về, thay vì tiếp tục bên trong phương thức chính, nó sẽ nhảy đến **hàm khai thác** thay thế.


````C
// Ghi đè địa chỉ trả về trên máy sau:
// http://cs-education.github.io/sys/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void breakout() {
    puts("Chào mừng. Hãy có một shell...");
    system("/bin/sh");
}
void input() {
  void *p;
  printf("Địa chỉ của biến ngăn xếp: %p\n", &p);
  printf("Một cái gì đó trông giống như một địa chỉ trả về trên ngăn xếp: %p\n", *((&p)+2));
  // Hãy thay đổi nó để trỏ đến đầu hàm lén lút của chúng ta.
  *((&p)+2) = breakout;
}
int main() {
    printf("mã main() bắt đầu tại %p\n",main);
    
    input();
    while (1) {
        puts("Xin chào");
        sleep(1);
    }

    return 0;
}
````

Có [rất nhiều](https://en.wikipedia.org/wiki/Stack_buffer_overflow) cách mà máy tính có xu hướng khắc phục điều này, ví dụ như **ASLR**, **Canary value**,...

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator">
Quay lại: Bộ nhớ, Phần 2: Triển khai Bộ cấp phát Bộ nhớ
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory-Review-Questions">
Tiếp theo: Câu hỏi Ôn tập về Bộ nhớ
</a>
</div>



