## `socket`

`int socket(int domain, int socket_type, int protocol);`

`Socket` tạo một socket với **miền** (ví dụ: `AF_INET` cho IPv4 hoặc `AF_INET6` cho IPv6), `socket_type` là sử dụng **UDP** hay **TCP** hoặc loại socket khác, `protocol` là tùy chọn cấu hình giao thức (đối với ví dụ của chúng ta, chúng ta có thể để trống như 0 cho mặc định). Lệnh gọi này tạo một **đối tượng socket** trong kernel mà qua đó người ta có thể giao tiếp với thế giới/mạng bên ngoài. 
Bạn có thể sử dụng kết quả của `getaddressinfo` để điền vào các tham số `socket` hoặc cung cấp chúng theo cách thủ công.

Lệnh gọi `socket` trả về một số nguyên - một **bộ mô tả tệp** - và, đối với **máy khách TCP**, bạn có thể sử dụng nó giống như một bộ mô tả tệp thông thường, tức là bạn có thể sử dụng `read` và `write` để nhận hoặc gửi gói.

Socket TCP tương tự như `pipe` ngoại trừ việc chúng cho phép **giao tiếp song công** hoàn toàn, tức là bạn có thể gửi và nhận dữ liệu theo cả hai hướng một cách độc lập.

## `getaddrinfo`

Chúng ta đã thấy điều này trong phần trước! Bạn là chuyên gia về điều này. 

## `connect`

`int connectok = connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`

Truyền cho `connect` bộ mô tả tệp socket, **địa chỉ** bạn muốn kết nối đến và độ dài tính bằng byte của cấu trúc địa chỉ. Để giúp xác định lỗi và sai lầm, bạn nên kiểm tra giá trị trả về của tất cả các lệnh gọi mạng, bao gồm cả `connect`

## `read`/`write`

Khi chúng ta có kết nối thành công, chúng ta có thể đọc hoặc ghi giống như bất kỳ bộ mô tả tệp cũ nào. Hãy nhớ rằng nếu bạn được kết nối với một trang web, bạn muốn tuân thủ đặc tả **giao thức HTTP** để nhận được bất kỳ loại kết quả có ý nghĩa nào. Có những thư viện để làm điều này, thông thường bạn không kết nối ở cấp socket vì có những thư viện hoặc gói khác xung quanh nó.

Số lượng byte được đọc hoặc ghi có thể nhỏ hơn dự kiến. Vì vậy, điều quan trọng là phải kiểm tra giá trị trả về của `read` và `write`. 

## Ví dụ đầy đủ về máy khách TCP đơn giản

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_INET; /* Chỉ IPv4 */
    hints.ai_socktype = SOCK_STREAM; /* TCP */

    s = getaddrinfo("www.illinois.edu", "80", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    if (connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1) {
        perror("connect");
        exit(2);
    }

    char *buffer = "GET / HTTP/1.0\r\n\r\n";
    printf("ĐANG GỬI: %s", buffer);
    printf("===\n");

    // Đối với bản demo tầm thường này, chỉ cần giả sử write() gửi tất cả các byte trong một lần và không bị gián đoạn

    write(sock_fd, buffer, strlen(buffer));

    char resp[1000];
    int len = read(sock_fd, resp, 999);
    resp[len] = '\0';
    printf("%s\n", resp);

    return 0;
}
```

Ví dụ đầu ra:
```
ĐANG GỬI: GET / HTTP/1.0

===
HTTP/1.1 200 OK
Date: Mon, 27 Oct 2014 19:19:05 GMT
Server: Apache/2.2.15 (Red Hat) mod_ssl/2.2.15 OpenSSL/1.0.1e-fips mod_jk/1.2.32
Last-Modified: Fri, 03 Feb 2012 16:51:10 GMT
ETag: "401b0-49-4b8121ea69b80"
Accept-Ranges: bytes
Content-Length: 73
Connection: close
Content-Type: text/html

Được cung cấp bởi Dịch vụ Web tại Vụ Công cộng tại Đại học Illinois
```

## Bình luận về yêu cầu và phản hồi HTTP
Ví dụ trên minh họa một yêu cầu đến máy chủ bằng cách sử dụng **Giao thức truyền siêu văn bản (HTTP)**.
Một trang web (hoặc các tài nguyên khác) được yêu cầu bằng cách sử dụng yêu cầu sau:
```
GET / HTTP/1.0

```
Có bốn phần (**phương thức**, ví dụ: `GET`, `POST`, ...); **tài nguyên** (ví dụ: `/`, `/index.html`, `/image.png`); **giao thức** "HTTP/1.0" và hai dòng mới (`\r\n\r\n`)


Dòng phản hồi đầu tiên của máy chủ mô tả phiên bản HTTP được sử dụng và liệu yêu cầu có thành công hay không bằng **mã phản hồi** 3 chữ số:
```
HTTP/1.1 200 OK
```
Nếu máy khách đã yêu cầu một tệp không tồn tại, ví dụ: `GET /nosuchfile.html HTTP/1.0`
Thì dòng đầu tiên bao gồm mã phản hồi là mã phản hồi `404` nổi tiếng:
```
HTTP/1.1 404 Not Found
```



