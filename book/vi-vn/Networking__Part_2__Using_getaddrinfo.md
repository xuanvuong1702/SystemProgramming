## Làm cách nào để sử dụng `getaddrinfo` để chuyển đổi tên máy chủ thành địa chỉ IP?

Hàm `getaddrinfo` có thể chuyển đổi **tên miền** có thể đọc được của con người (ví dụ: `www.illinois.edu`) thành địa chỉ IPv4 và IPv6. Trên thực tế, nó sẽ trả về một danh sách liên kết của các cấu trúc `addrinfo`:
```C
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    socklen_t        ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};
```

Nó rất dễ sử dụng. Ví dụ: giả sử bạn muốn tìm ra địa chỉ IPv4 số của máy chủ web tại www.bbc.com. Chúng tôi thực hiện việc này theo hai giai đoạn. Đầu tiên, sử dụng `getaddrinfo` để xây dựng một danh sách liên kết các kết nối có thể. Thứ hai, sử dụng `getnameinfo` để chuyển đổi địa chỉ nhị phân thành một dạng có thể đọc được.

```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo hints, *infoptr; // Vì vậy, không cần sử dụng biến toàn cục memset

int main() {
    hints.ai_family = AF_INET; // AF_INET có nghĩa là chỉ địa chỉ IPv4

    int result = getaddrinfo("www.bbc.com", NULL, &hints, &infoptr);
    if (result) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(result));
        exit(1);
    }

    struct addrinfo *p;
    char host[256];

    for (p = infoptr; p != NULL; p = p->ai_next) {

        getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof (host), NULL, 0, NI_NUMERICHOST);
        puts(host);
    }

    freeaddrinfo(infoptr);
    return 0;
}
```
Đầu ra điển hình:
```
212.58.244.70
212.58.244.71
```

## Làm cách nào để www.cs.illinois.edu được chuyển đổi thành địa chỉ IP?

Phép thuật! Không, nghiêm túc mà nói, một hệ thống được gọi là "**DNS**" (Dịch vụ Tên miền) được sử dụng. Nếu một máy không chứa câu trả lời **cục bộ**, thì nó sẽ gửi một gói **UDP** đến máy chủ DNS cục bộ. Máy chủ này đến lượt nó có thể truy vấn các máy chủ DNS **ngược dòng** khác.

## DNS có an toàn không?
Bản thân DNS nhanh nhưng không an toàn. Các yêu cầu DNS không được **mã hóa** và dễ bị tấn công '**man-in-the-middle**'. Ví dụ: kết nối internet của quán cà phê có thể dễ dàng phá hoại các yêu cầu DNS của bạn và gửi lại các địa chỉ IP khác nhau cho một **miền** cụ thể.

## Làm cách nào để kết nối với máy chủ TCP (ví dụ: máy chủ web?)

Có ba **lệnh gọi hệ thống** cơ bản bạn cần để kết nối với máy từ xa:
```
getaddrinfo -- Xác định địa chỉ từ xa của máy chủ từ xa
socket      -- Tạo một socket
connect     -- Kết nối với máy chủ từ xa bằng cách sử dụng socket và thông tin địa chỉ
```
Lệnh gọi `getaddrinfo` nếu thành công, sẽ tạo một **danh sách liên kết** của các cấu trúc `addrinfo` và đặt con trỏ đã cho trỏ đến cấu trúc đầu tiên.


Lệnh gọi **socket** tạo một socket **gửi đi** và trả về một **bộ mô tả** (đôi khi được gọi là '**bộ mô tả tệp**') có thể được sử dụng với `read` và `write` v.v. Theo nghĩa này, nó là tương tự mạng của `open` mở **luồng tệp** - ngoại trừ việc chúng ta chưa kết nối socket với bất cứ thứ gì!

Cuối cùng, lệnh gọi **kết nối** cố gắng kết nối với máy từ xa. Chúng ta chuyển bộ mô tả socket gốc và cả thông tin địa chỉ socket được lưu trữ bên trong cấu trúc `addrinfo`. Có nhiều loại cấu trúc địa chỉ socket khác nhau (ví dụ: IPv4 so với IPv6) có thể yêu cầu nhiều bộ nhớ hơn. Vì vậy, ngoài việc chuyển con trỏ, kích thước của cấu trúc cũng được chuyển:

```C
// Rút thông tin địa chỉ socket từ cấu trúc addrinfo:
connect(sockfd, p->ai_addr, p->ai_addrlen);
```

## Làm cách nào để giải phóng bộ nhớ được phân bổ cho danh sách liên kết của các cấu trúc addrinfo?

Là một phần của mã **dọn dẹp**, hãy gọi `freeaddrinfo` trên cấu trúc `addrinfo` trên cùng:
```C
void freeaddrinfo(struct addrinfo *ai);
```

## Nếu getaddrinfo thất bại, tôi có thể sử dụng `strerror` để in ra lỗi không?
Không. Xử lý lỗi với `getaddrinfo` hơi khác một chút:
* Giá trị trả về _là_ **mã lỗi** (tức là không sử dụng `errno`)
* Sử dụng `gai_strerror` để lấy văn bản lỗi tiếng Anh ngắn gọn tương đương:

```C
int result = getaddrinfo(...);
if (result) { 
    const char *mesg = gai_strerror(result); 
    ...
}
```

## Tôi có thể chỉ yêu cầu kết nối IPv4 hoặc IPv6 không? Chỉ TCP?
Có! Sử dụng cấu trúc `addrinfo` được truyền vào `getaddrinfo` để xác định loại kết nối bạn muốn.

Ví dụ: để chỉ định các giao thức **dựa trên luồng** qua IPv6:
```C
struct addrinfo hints;
memset(&hints, 0, sizeof (hints));

hints.ai_family = AF_INET6; // Chỉ muốn IPv6 (sử dụng AF_INET cho IPv4)
hints.ai_socktype = SOCK_STREAM; // Chỉ muốn kết nối dựa trên luồng
```

## Còn các ví dụ mã sử dụng `gethostbyname` thì sao?

Hàm cũ `gethostbyname` **không được dùng nữa**; đó là cách cũ để chuyển đổi tên máy chủ thành địa chỉ IP. Địa chỉ cổng vẫn cần được đặt thủ công bằng cách sử dụng hàm `htons`. Viết mã để hỗ trợ IPv4 VÀ IPv6 bằng cách sử dụng `getaddrinfo` mới hơn sẽ dễ dàng hơn nhiều

## Có dễ dàng như vậy không!?
Có và không. Việc tạo một **máy khách TCP** đơn giản rất dễ dàng - tuy nhiên, giao tiếp mạng cung cấp nhiều **cấp độ trừu tượng hóa** khác nhau và một số thuộc tính và tùy chọn có thể được đặt ở mỗi cấp độ trừu tượng hóa (ví dụ: chúng ta chưa nói về `setsockopt` có thể thao tác các tùy chọn cho socket).

## Đọc thêm
Để biết thêm thông tin, hãy xem [hướng dẫn của Beej](https://beej.us/guide/bgnet/html/multi/index.html).

