Mỗi luồng sử dụng một bộ nhớ stack. Stack 'phát triển xuống dưới' - nếu một hàm gọi một hàm khác, thì stack được mở rộng đến các địa chỉ bộ nhớ nhỏ hơn.
Bộ nhớ stack bao gồm các biến tự động không tĩnh (tạm thời), giá trị tham số và địa chỉ trả về.
Nếu một bộ đệm quá nhỏ cho một số dữ liệu (ví dụ: giá trị nhập từ người dùng), thì có khả năng thực sự rằng các biến stack khác và thậm chí địa chỉ trả về sẽ bị ghi đè.
Cấu trúc chính xác của nội dung stack và thứ tự của các biến tự động phụ thuộc vào kiến trúc và trình biên dịch. Tuy nhiên, với một chút công việc điều tra, chúng ta có thể tìm hiểu cách cố ý làm vỡ stack cho một kiến trúc cụ thể.

Ví dụ dưới đây minh họa cách lưu trữ địa chỉ trả về trên stack. Đối với một kiến trúc 32 bit cụ thể [Máy Linux Trực Tiếp](http://cs-education.github.io/sys/), chúng tôi xác định rằng địa chỉ trả về được lưu trữ tại một địa chỉ hai con trỏ (8 byte) trên địa chỉ của biến tự động. Mã cố ý thay đổi giá trị stack để khi hàm nhập trả về, thay vì tiếp tục bên trong phương thức main, nó nhảy đến hàm exploit thay thế.

```C
// Ghi đè địa chỉ trả về trên máy sau:
// http://cs-education.github.io/sys/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void breakout() {
    puts("Chào mừng. Có một shell...");
    system("/bin/sh");
}
void input() {
  void *p;
  printf("Địa chỉ của biến stack: %p\n", &p);
  printf("Một cái gì đó giống như địa chỉ trả về trên stack: %p\n", *((&p)+2));
  // Hãy thay đổi nó để trỏ đến đầu của hàm lén lút của chúng tôi.
  *((&p)+2) = breakout;
}
int main() {
    printf("Mã của main() bắt đầu tại %p\n",main);
    
    input();
    while (1) {
        puts("Xin chào");
        sleep(1);
    }

    return 0;
}
````

Có [rất nhiều](https://vi.wikipedia.org/wiki/Tràn_bộ_đệm) cách mà máy tính thường vượt qua vấn đề này.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator">
Quay lại: Bộ nhớ, Phần 2: Triển khai Bộ cấp phát Bộ nhớ
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory-Review-Questions">
Tiếp theo: Câu hỏi Ôn tập về Bộ nhớ
</a>
</div>