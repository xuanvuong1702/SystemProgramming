# Giới thiệu về Luồng (Thread)

## Luồng là gì?
Luồng là viết tắt của 'thread-of-execution' (chuỗi thực thi). Nó đại diện cho chuỗi các lệnh mà CPU đã và sẽ thực thi. Để nhớ cách trở về từ các lời gọi hàm, và để lưu trữ giá trị của các biến tự động và tham số, một luồng sử dụng một ngăn xếp.

## Quy trình nhẹ (LWP) là gì? Nó liên quan như thế nào đến luồng?

Về cơ bản, một luồng là một quy trình (nghĩa là việc tạo một luồng tương tự như `fork`) ngoại trừ việc **không có sao chép** nghĩa là không có sao chép khi ghi. Điều này cho phép một quy trình chia sẻ cùng không gian địa chỉ, biến, vùng heap, bộ mô tả tệp và v.v.

Lệnh gọi hệ thống thực tế để tạo một luồng tương tự như `fork`; đó là `clone`. Chúng ta sẽ không đi sâu vào chi tiết nhưng bạn có thể đọc [trang hướng dẫn](http://man7.org/linux/man-pages/man2/clone.2.html) nhớ rằng nó nằm ngoài phạm vi trực tiếp của khóa học này.

LWP hoặc luồng được ưa chuộng hơn fork trong nhiều kịch bản vì chúng tốn ít chi phí hơn khi tạo. Nhưng trong một số trường hợp (đáng chú ý là Python sử dụng cách này) đa xử lý là cách làm cho mã của bạn nhanh hơn.

## Ngăn xếp của luồng hoạt động như thế nào?
Hàm main của bạn (và các hàm khác mà bạn có thể gọi) có các biến tự động. Chúng ta sẽ lưu trữ chúng trong bộ nhớ bằng cách sử dụng một ngăn xếp và theo dõi kích thước của ngăn xếp bằng cách sử dụng một con trỏ đơn giản (con trỏ ngăn xếp). Nếu luồng gọi một hàm khác, chúng ta di chuyển con trỏ ngăn xếp xuống, để có thêm không gian cho các tham số và biến tự động. Khi nó trở về từ một hàm, chúng ta có thể di chuyển con trỏ ngăn xếp trở lại giá trị trước đó của nó. Chúng ta giữ một bản sao của giá trị con trỏ ngăn xếp cũ - trên ngăn xếp! Đây là lý do tại sao việc trở về từ một hàm rất nhanh - rất dễ để 'giải phóng' bộ nhớ được sử dụng bởi các biến tự động - chúng ta chỉ cần thay đổi con trỏ ngăn xếp.

Trong một chương trình đa luồng, có nhiều ngăn xếp nhưng chỉ có một không gian địa chỉ. Thư viện pthread cấp phát một số không gian ngăn xếp (hoặc trong heap hoặc sử dụng một phần của ngăn xếp của chương trình chính) và sử dụng lời gọi hàm `clone` để bắt đầu luồng tại địa chỉ ngăn xếp đó. Tổng không gian địa chỉ có thể trông giống như sau.

![](https://i.imgur.com/ac2QDwu.png)

## Quá trình của tôi có thể có bao nhiêu luồng?
Bạn có thể có nhiều hơn một luồng chạy bên trong một quá trình. Bạn nhận được luồng đầu tiên miễn phí! Nó chạy mã bạn viết bên trong 'main'. Nếu bạn cần thêm luồng, bạn có thể gọi `pthread_create` để tạo một luồng mới bằng cách sử dụng thư viện pthread. Bạn sẽ cần truyền một con trỏ đến một hàm để luồng biết nơi bắt đầu.

Các luồng bạn tạo đều sống bên trong cùng một bộ nhớ ảo vì chúng là một phần của cùng một quá trình. Do đó, chúng có thể nhìn thấy heap, các biến toàn cục và mã chương trình, v.v. Do đó, bạn có thể có hai (hoặc nhiều hơn) CPU làm việc trên chương trình của bạn cùng một lúc và bên trong cùng một quá trình. Việc gán các luồng cho CPU là do hệ điều hành quyết định. Nếu bạn có nhiều luồng hoạt động hơn số CPU, thì kernel sẽ gán luồng cho một CPU trong một thời gian ngắn (hoặc cho đến khi nó hết việc để làm) và sau đó sẽ tự động chuyển CPU để làm việc trên một luồng khác. 
Ví dụ, một CPU có thể đang xử lý trí tuệ nhân tạo của trò chơi trong khi một luồng khác đang tính toán đầu ra đồ họa.

# Sử dụng đơn giản

## Ví dụ Hello world với pthread
Để sử dụng pthreads, bạn sẽ cần bao gồm `pthread.h` VÀ bạn cần biên dịch với tùy chọn trình biên dịch `-pthread` (hoặc `-lpthread`). Tùy chọn này cho trình biên dịch biết rằng chương trình của bạn yêu cầu hỗ trợ đa luồng

Để tạo một luồng, sử dụng hàm `pthread_create`. Hàm này nhận bốn đối số:
```C
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);
```
* Đầu tiên là một con trỏ đến một biến sẽ giữ id của luồng mới tạo.
* Thứ hai là một con trỏ đến các thuộc tính mà chúng ta có thể sử dụng để điều chỉnh và tinh chỉnh một số tính năng nâng cao của pthreads.
* Thứ ba là một con trỏ đến một hàm mà chúng ta muốn chạy
* Thứ tư là một con trỏ sẽ được trao cho hàm của chúng ta

Đối số `void *(*start_routine) (void *)` khá khó đọc! Nó có nghĩa là một con trỏ nhận một con trỏ `void *` và trả về một con trỏ `void *`. Nó giống như một khai báo hàm ngoại trừ việc tên của hàm được bao quanh bởi `(* .... )`

Dưới đây là ví dụ đơn giản nhất:
```C
#include <stdio.h>
#include <pthread.h>
// nhớ đặt tùy chọn biên dịch -pthread

void *busy(void *ptr) {
// ptr sẽ trỏ đến "Hi"
    puts("Hello World");
    return NULL;
}
int main() {
    pthread_t id;
    pthread_create(&id, NULL, busy, "Hi");
    while (1) {} // Vòng lặp vô hạn
}
```
Nếu chúng ta muốn chờ luồng của mình hoàn thành, hãy sử dụng `pthread_join`
```C
void *result;
pthread_join(id, &result);
```
Trong ví dụ trên, `result` sẽ là `null` vì hàm busy trả về `null`.
Chúng ta cần truyền địa chỉ của result vì `pthread_join` sẽ ghi vào nội dung của con trỏ của chúng ta.

Xem thêm [Pthreads Phần 2](https://github.com/angrave/SystemProgramming/wiki/Pthreads%2C-Part-2%3A-Usage-in-Practice)