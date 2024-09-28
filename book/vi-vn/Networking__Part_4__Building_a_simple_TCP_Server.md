## `htons` là gì và khi nào nó được sử dụng?

Số nguyên có thể được biểu diễn ở dạng **byte có trọng số thấp nhất trước** hoặc **byte có trọng số cao nhất trước**. Cách tiếp cận nào cũng hợp lý miễn là bản thân máy tính nhất quán nội bộ. Đối với **giao tiếp mạng**, chúng ta cần tiêu chuẩn hóa định dạng đã thống nhất.

`htons(xyz)` trả về giá trị 'short' số nguyên không dấu 16 bit `xyz` theo **thứ tự byte mạng**.
`htonl(xyz)` trả về giá trị 'long' số nguyên không dấu 32 bit `xyz` theo thứ tự byte mạng.

Các hàm này được đọc là 'máy chủ sang mạng'; các hàm **nghịch đảo** (ntohs, ntohl) chuyển đổi các giá trị byte được sắp xếp theo mạng thành thứ tự được sắp xếp theo máy chủ. Vậy, **thứ tự máy chủ** là **little-endian** hay **big-endian**? Câu trả lời là - nó phụ thuộc vào máy của bạn! Nó phụ thuộc vào **kiến ​​trúc** thực tế của máy chủ đang chạy mã. Nếu kiến ​​trúc tình cờ giống với thứ tự mạng thì kết quả của các hàm này chỉ là đối số. Đối với máy x86, thứ tự máy chủ và mạng _là_ khác nhau.

Tóm tắt: Bất cứ khi nào bạn đọc hoặc ghi các cấu trúc mạng C cấp thấp (ví dụ: thông tin cổng và địa chỉ), hãy nhớ sử dụng các hàm trên để đảm bảo chuyển đổi chính xác sang / từ **định dạng máy**. Nếu không, giá trị được hiển thị hoặc chỉ định có thể không chính xác.

## 4 lệnh gọi mạng 'lớn' được sử dụng để tạo máy chủ là gì?

Bốn **lệnh gọi hệ thống** cần thiết để tạo **máy chủ TCP** là: `socket`, `bind`, `listen` và `accept`. Mỗi lệnh đều có một mục đích cụ thể và nên được gọi theo thứ tự trên.

Thông tin cổng (được sử dụng bởi `bind`) có thể được đặt thủ công (nhiều ví dụ mã C chỉ dành cho IPv4 cũ hơn làm điều này) hoặc được tạo bằng cách sử dụng `getaddrinfo`.

Chúng ta cũng thấy các ví dụ về `setsockopt` sau này.

## Mục đích của việc gọi `socket` là gì?

Để tạo một **điểm cuối** để giao tiếp mạng. Một socket mới tự nó không đặc biệt hữu ích; mặc dù chúng tôi đã chỉ định kết nối dựa trên gói hoặc luồng, nó không bị ràng buộc với một giao diện hoặc cổng mạng cụ thể nào. Thay vào đó, socket trả về một **bộ mô tả mạng** có thể được sử dụng với các lệnh gọi sau đến `bind`, `listen` và `accept`.

## Mục đích của việc gọi `bind` là gì?

Lệnh gọi `bind` **liên kết** một socket trừu tượng với một **giao diện** và cổng mạng thực tế. Có thể gọi bind trên **máy khách TCP**, tuy nhiên, việc chỉ định **cổng gửi đi** thường là không cần thiết.

## Mục đích của việc gọi `listen` là gì?

Lệnh gọi `listen` chỉ định kích thước **hàng đợi** cho số lượng kết nối đến, chưa được xử lý, tức là chưa được `accept` gán **bộ mô tả mạng**.
Các giá trị điển hình cho một máy chủ hiệu suất cao là 128 trở lên.

## Tại sao socket máy chủ lại bị động?

**Socket máy chủ** không chủ động cố gắng kết nối với máy chủ khác; thay vào đó, chúng **đợi** các kết nối đến. Ngoài ra, socket máy chủ không bị đóng khi ngang hàng ngắt kết nối. Thay vào đó, máy khách giao tiếp với một socket đang hoạt động riêng biệt trên máy chủ dành riêng cho kết nối đó.

Các kết nối TCP duy nhất được xác định bởi bộ `(ip nguồn, cổng nguồn, ip đích, cổng đích)`
Có thể có nhiều kết nối từ trình duyệt web đến cùng một cổng máy chủ (ví dụ: cổng 80) vì cổng nguồn trên mỗi gói đến là duy nhất. tức là Đối với một cổng máy chủ cụ thể (ví dụ: cổng 80), có thể có một socket máy chủ **thụ động** nhưng nhiều socket đang **hoạt động** (một cho mỗi kết nối hiện đang mở) và hệ điều hành của máy chủ duy trì một **bảng tra cứu** liên kết một bộ duy nhất với các socket đang hoạt động, vì vậy các gói đến có thể được **định tuyến** chính xác đến socket chính xác.

## Mục đích của việc gọi `accept` là gì?

Khi socket máy chủ đã được khởi tạo, máy chủ gọi `accept` để đợi các kết nối mới. Không giống như `socket` `bind` và `listen`, lệnh gọi này sẽ **chặn**. tức là nếu không có kết nối mới, lệnh gọi này sẽ chặn và chỉ trả về khi một máy khách mới kết nối. Socket TCP được trả về được liên kết với một bộ cụ thể `(IP máy khách, cổng máy khách, IP máy chủ, cổng máy chủ)` và sẽ được sử dụng cho tất cả các gói TCP đến và đi trong tương lai khớp với bộ này. 

Lưu ý lệnh gọi `accept` trả về một bộ mô tả tệp mới. Bộ mô tả tệp này dành riêng cho một máy khách cụ thể. **Lỗi lập trình** phổ biến là sử dụng bộ mô tả socket máy chủ gốc cho I/O máy chủ và sau đó tự hỏi tại sao mã mạng lại bị lỗi.

## Những cạm bẫy khi tạo máy chủ TCP là gì?

+ Sử dụng bộ mô tả socket của socket máy chủ thụ động (được mô tả ở trên)
+ Không chỉ định yêu cầu `SOCK_STREAM` cho `getaddrinfo`
+ Không thể sử dụng lại một cổng hiện có.
+ Không khởi tạo các mục nhập cấu trúc không sử dụng
+ Lệnh gọi `bind` sẽ thất bại nếu cổng hiện đang được sử dụng

Lưu ý, các cổng là trên mỗi máy - không phải trên mỗi tiến trình hoặc trên mỗi người dùng. Nói cách khác, bạn không thể sử dụng cổng 1234 trong khi một tiến trình khác đang sử dụng cổng đó. Tệ hơn, các cổng theo mặc định bị 'buộc' sau khi một tiến trình kết thúc.


## Ví dụ mã máy chủ

Một ví dụ máy chủ đơn giản hoạt động được hiển thị bên dưới. Lưu ý rằng ví dụ này chưa hoàn chỉnh - ví dụ: nó không đóng bộ mô tả socket hoặc giải phóng bộ nhớ được tạo bởi `getaddrinfo`

```C
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, "1234", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
        perror("bind()");
        exit(1);
    }

    if (listen(sock_fd, 10) != 0) {
        perror("listen()");
        exit(1);
    }
    
    struct sockaddr_in *result_addr = (struct sockaddr_in *) result->ai_addr;
    printf("Đang lắng nghe trên bộ mô tả tệp %d, cổng %d\n", sock_fd, ntohs(result_addr->sin_port));

    printf("Đang chờ kết nối...\n");
    int client_fd = accept(sock_fd, NULL, NULL);
    printf("Đã kết nối: client_fd=%d\n", client_fd);

    char buffer[1000];
    int len = read(client_fd, buffer, sizeof (buffer) - 1);
    buffer[len] = '\0';

    printf("Đã đọc %d ký tự\n", len);
    printf("===\n");
    printf("%s\n", buffer);

    return 0;
}
```

## Tại sao máy chủ của tôi không thể sử dụng lại cổng?

Theo mặc định, cổng không được giải phóng ngay lập tức khi socket máy chủ bị đóng. Thay vào đó, cổng chuyển sang trạng thái **"TIMED-WAIT"**. Điều này có thể dẫn đến nhầm lẫn đáng kể trong quá trình phát triển vì **hết thời gian chờ** có thể khiến mã mạng hợp lệ có vẻ bị lỗi.

 Để có thể sử dụng lại cổng ngay lập tức, hãy chỉ định `SO_REUSEPORT` trước khi liên kết với cổng.
```C
int optval = 1;
setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof (optval));

bind(....
```

Đây là [một cuộc thảo luận giới thiệu mở rộng trên stackoverflow về `SO_REUSEPORT`](http://stackoverflow.com/questions/14388706/socket-options-so-reuseaddr-and-so-reuseport-how-do-they-differ-do-they-mean-t).



