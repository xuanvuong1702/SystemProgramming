## Làm cách nào để tạo máy chủ UDP?
Có nhiều lệnh gọi hàm khả dụng để gửi **socket UDP**. Chúng tôi sẽ sử dụng `getaddrinfo` mới hơn để giúp thiết lập cấu trúc socket.

Hãy nhớ rằng **UDP** là một giao thức dựa trên **gói tin** ('datagram') đơn giản; không có kết nối nào được thiết lập giữa hai máy chủ.

Đầu tiên, hãy khởi tạo cấu trúc `addrinfo` hints để yêu cầu một socket **datagram thụ động** IPv6.
```C
memset(&hints, 0, sizeof(hints));
hints.ai_family = AF_INET6; // sử dụng AF_INET thay thế cho IPv4
hints.ai_socktype =  SOCK_DGRAM;
hints.ai_flags =  AI_PASSIVE;
```

Tiếp theo, sử dụng `getaddrinfo` để chỉ định số **cổng** (chúng ta không cần chỉ định máy chủ vì chúng ta đang tạo socket máy chủ, không gửi gói tin đến máy chủ từ xa).
```C
getaddrinfo(NULL, "300", &hints, &res);

sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
bind(sockfd, res->ai_addr, res->ai_addrlen);
```
Số cổng là <1024, vì vậy chương trình sẽ cần quyền `root`. Chúng ta cũng có thể chỉ định **tên dịch vụ** thay vì giá trị cổng số.

Cho đến nay, các lệnh gọi tương tự như máy chủ TCP. Đối với một dịch vụ dựa trên **luồng**, chúng ta sẽ gọi `listen` và `accept`. Đối với **máy chủ UDP** của chúng tôi, chúng ta có thể bắt đầu chờ gói tin đến trên **socket** -

```C
struct sockaddr_storage addr;
int addrlen = sizeof(addr);

// ssize_t recvfrom(int socket, void* buffer, size_t buflen, int flags, struct sockaddr *addr, socklen_t * address_len);

byte_count = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &addr, &addrlen);
```

Cấu trúc `addr` sẽ chứa thông tin **người gửi** (nguồn) về gói tin đến.
Lưu ý kiểu `sockaddr_storage` đủ lớn để chứa tất cả các loại địa chỉ socket có thể (ví dụ: IPv4, IPv6 và các loại socket khác).

## Mã đầy đủ

```C
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    int s;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6; // INET cho IPv4
    hints.ai_socktype =  SOCK_DGRAM;
    hints.ai_flags =  AI_PASSIVE;

    getaddrinfo(NULL, "300", &hints, &res);

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) != 0) {
        perror("bind()");
        exit(1);
    }
    struct sockaddr_storage addr;
    int addrlen = sizeof(addr);

    while(1){
        char buf[1024];
        ssize_t byte_count = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *) &addr, &addrlen);
        if (byte_count == -1) {
            perror("recvfrom");
        } else {
            buf[byte_count] = '\0';

            printf("Đọc %zd ký tự\n", byte_count);
            printf("===\n");
            printf("%s\n", buf);
        }
    }

    return 0;
}
```




