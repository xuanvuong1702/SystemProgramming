# Giới thiệu về Luồng

## Luồng là gì?
Luồng là viết tắt của 'thread-of-execution' (luồng thực thi). Nó đại diện cho chuỗi các lệnh mà CPU đã (và sẽ) thực thi. Để ghi nhớ cách quay lại từ các lệnh gọi hàm và để lưu trữ các giá trị của các biến tự động và các tham số, một luồng sử dụng một ngăn xếp.

## Lightweight Process (LWP) là gì? Nó liên quan đến luồng như thế nào?

Về mọi mục đích và ý nghĩa, một luồng là một tiến trình (có nghĩa là việc tạo một luồng tương tự như `fork`) ngoại trừ việc **không có sao chép** có nghĩa là không có sao chép khi ghi. Điều này cho phép một tiến trình chia sẻ cùng một không gian địa chỉ, biến, heap, bộ mô tả tệp, v.v.

Lệnh gọi hệ thống thực tế để tạo một luồng tương tự như `fork`; đó là `clone`. Chúng ta sẽ không đi sâu vào chi tiết cụ thể nhưng bạn có thể đọc các [trang hướng dẫn](http://man7.org/linux/man-pages/man2/clone.2.html) lưu ý rằng nó nằm ngoài phạm vi trực tiếp của khóa học này.

LWP hoặc luồng được ưu tiên hơn so với forking trong nhiều trường hợp vì việc tạo chúng ít tốn kém hơn. Nhưng trong một số trường hợp (đáng chú ý là python sử dụng điều này), xử lý đa tiến trình là cách để làm cho mã của bạn nhanh hơn.

## Ngăn xếp của luồng hoạt động như thế nào?
Hàm main của bạn (và các hàm khác mà bạn có thể gọi) có các biến tự động. Chúng ta sẽ lưu trữ chúng trong bộ nhớ bằng cách sử dụng một ngăn xếp và theo dõi kích thước của ngăn xếp bằng cách sử dụng một con trỏ đơn giản (con trỏ ngăn xếp). Nếu luồng gọi một hàm khác, chúng ta di chuyển con trỏ ngăn xếp xuống, để chúng ta có thêm không gian cho các tham số và biến tự động. Khi nó trả về từ một hàm, chúng ta có thể di chuyển con trỏ ngăn xếp trở lại giá trị trước đó của nó. Chúng ta giữ một bản sao của giá trị con trỏ ngăn xếp cũ - trên ngăn xếp! Đây là lý do tại sao việc trả về từ một hàm rất nhanh - rất dễ dàng để 'giải phóng' bộ nhớ được sử dụng bởi các biến tự động - chúng ta chỉ cần thay đổi con trỏ ngăn xếp.

Trong một chương trình đa luồng, có nhiều ngăn xếp nhưng chỉ có một không gian địa chỉ. Thư viện pthread cấp phát một số không gian ngăn xếp (trong heap hoặc sử dụng một phần của ngăn xếp chương trình chính) và sử dụng lệnh gọi hàm `clone` để bắt đầu luồng tại địa chỉ ngăn xếp đó. Tổng không gian địa chỉ có thể trông giống như thế này.

![](https://i.imgur.com/ac2QDwu.png)

## Tiến trình của tôi có thể có bao nhiêu luồng?
Bạn có thể có nhiều hơn một luồng chạy bên trong một tiến trình. Bạn nhận được luồng đầu tiên miễn phí! Nó chạy mã bạn viết bên trong 'main'. Nếu bạn cần thêm luồng, bạn có thể gọi `pthread_create` để tạo một luồng mới bằng cách sử dụng thư viện pthread. Bạn sẽ cần chuyển một con trỏ đến một hàm để luồng biết bắt đầu từ đâu.

Tất cả các luồng bạn tạo đều nằm trong cùng một bộ nhớ ảo vì chúng là một phần của cùng một tiến trình. Do đó, tất cả chúng đều có thể nhìn thấy heap, các biến toàn cục và mã chương trình, v.v. Do đó, bạn có thể có hai (hoặc nhiều hơn) CPU hoạt động trên chương trình của mình cùng một lúc và bên trong cùng một tiến trình. Hệ điều hành sẽ phân bổ các luồng cho CPU. Nếu bạn có nhiều luồng hoạt động hơn CPU thì kernel sẽ gán luồng cho CPU trong một khoảng thời gian ngắn (hoặc cho đến khi nó hết việc phải làm) và sau đó sẽ tự động chuyển CPU sang làm việc trên một luồng khác. 
Ví dụ: một CPU có thể đang xử lý AI trò chơi trong khi một luồng khác đang tính toán đầu ra đồ họa.

# Cách sử dụng đơn giản

## Ví dụ về pthread Hello world
Để sử dụng pthread, bạn sẽ cần include `pthread.h` VÀ bạn cần biên dịch với tùy chọn trình biên dịch `-pthread` (hoặc `-lpthread`). Tùy chọn này cho trình biên dịch biết rằng chương trình của bạn yêu cầu hỗ trợ luồng

Để tạo một luồng, hãy sử dụng hàm `pthread_create`. Hàm này nhận bốn đối số:
```C
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
```
* Đầu tiên là một con trỏ đến một biến sẽ chứa id của luồng mới được tạo.
* Thứ hai là một con trỏ đến các thuộc tính mà chúng ta có thể sử dụng để tinh chỉnh và điều chỉnh một số tính năng nâng cao của pthread.
* Thứ ba là một con trỏ đến một hàm mà chúng ta muốn chạy
* Thứ tư là một con trỏ sẽ được cung cấp cho hàm của chúng ta

Đối số `void *(*start_routine) (void *)` rất khó đọc! Nó có nghĩa là một con trỏ nhận một con trỏ `void *` và trả về một con trỏ `void *`. Nó trông giống như một khai báo hàm ngoại trừ tên của hàm được bao bọc bởi `(* .... )`

Đây là ví dụ đơn giản nhất:
```C
#include <stdio.h>
#include <pthread.h>
// hãy nhớ đặt tùy chọn biên dịch -pthread

void *busy(void *ptr) {
// ptr sẽ trỏ đến "Hi"
    puts("Hello World");
    return NULL;
}
int main() {
    pthread_t id;
    pthread_create(&id, NULL, busy, "Hi");
    while (1) {} // Lặp lại mãi mãi
}
```
Nếu chúng ta muốn đợi luồng của mình kết thúc, hãy sử dụng `pthread_join`
```C
void *result;
pthread_join(id, &result);
```
Trong ví dụ trên, `result` sẽ là `null` vì hàm busy trả về `null`.
Chúng ta cần chuyển địa chỉ của result vì `pthread_join` sẽ ghi vào nội dung của con trỏ của chúng ta.

Xem [Pthreads Phần 2](https://github.com/angrave/SystemProgramming/wiki/Pthreads%2C-Part-2%3A-Usage-in-Practice)