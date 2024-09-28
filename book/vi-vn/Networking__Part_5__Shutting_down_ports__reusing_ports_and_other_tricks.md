## Sự khác biệt giữa shutdown và close là gì?

Sử dụng lệnh gọi `shutdown` khi bạn không còn cần đọc thêm bất kỳ dữ liệu nào từ **socket**, ghi thêm dữ liệu hoặc đã hoàn tất cả hai.
Khi bạn **tắt** một socket để ghi (hoặc đọc) thêm, thông tin đó cũng được gửi đến đầu kia của kết nối. Ví dụ: nếu bạn tắt socket để ghi thêm ở phía máy chủ, thì một lúc sau, lệnh gọi `read` bị chặn có thể trả về 0 để cho biết rằng không còn byte nào được mong đợi.

Sử dụng `close` khi tiến trình của bạn không còn cần **bộ mô tả tệp** socket. 

Nếu bạn đã `fork` sau khi tạo bộ mô tả tệp socket, tất cả các tiến trình cần **đóng socket** trước khi tài nguyên socket có thể được sử dụng lại.  Nếu bạn tắt một socket để đọc thêm thì tất cả các tiến trình sẽ bị ảnh hưởng vì bạn đã thay đổi socket chứ không chỉ bộ mô tả tệp.

Mã được viết tốt sẽ `shutdown` một socket trước khi gọi `close` nó.

## Khi tôi chạy lại mã máy chủ của mình, nó không hoạt động! Tại sao?

Theo mặc định, sau khi socket bị đóng, cổng sẽ chuyển sang trạng thái **hết thời gian chờ**, trong thời gian đó, nó không thể được sử dụng lại ('liên kết với một socket mới').

Hành vi này có thể bị vô hiệu hóa bằng cách đặt tùy chọn socket `REUSEPORT` trước khi **liên kết** với một cổng:

```C
    int optval = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    bind(sock_fd, ...);
```

## Máy khách TCP có thể liên kết với một cổng cụ thể không?

Có! Trên thực tế, các kết nối TCP **gửi đi** được tự động liên kết với một cổng không sử dụng trên máy khách. Thông thường, việc đặt cổng một cách rõ ràng trên máy khách là không cần thiết vì hệ thống sẽ tìm một cách thông minh một cổng không sử dụng trên một giao diện hợp lý (ví dụ: **thẻ không dây**, nếu hiện đang được kết nối bằng kết nối **WiFi**). Tuy nhiên, nó có thể hữu ích nếu bạn cần chọn cụ thể một **thẻ ethernet** cụ thể hoặc nếu **tường lửa** chỉ cho phép các kết nối gửi đi từ một phạm vi giá trị cổng cụ thể.

Để liên kết rõ ràng với một giao diện và cổng ethernet, hãy gọi `bind` trước `connect`

## Tôi đã xây dựng một máy khách hoặc máy chủ TCP đơn giản nhưng tiến trình của tôi đôi khi chỉ thoát! Tại sao?

Nếu tiến trình của bạn ghi vào một socket đã bị **tắt** bởi đầu kia của kết nối TCP, thì tiến trình của bạn sẽ được gửi một tín hiệu **SIGPIPE**. Từ cuộc thảo luận trước đó của chúng tôi về **các đường ống**, bạn có thể nhớ hành động mặc định là tắt tiến trình. Một cách giải quyết cho việc này là bỏ qua các tín hiệu SIGPIPE hoặc triển khai **trình xử lý tín hiệu** của riêng bạn.

```C
void handle_sigpipe(int signal) {
  char mesg[1000];
  sprintf(mesg, "\n****** SIGPIPE  - không ai đang lắng nghe :-( ******\n");
  write(1, mesg, strlen(mesg));
}
```
Và đăng ký trình xử lý tín hiệu bằng cách sử dụng `signal` (hoặc `sigaction` hoặc `pthread_sigmask` mới hơn) ...

```C
signal(SIGPIPE,handle_sigpipe)
```
## Ai đã kết nối với máy chủ của tôi?

Lệnh gọi hệ thống `accept` có thể tùy chọn cung cấp thông tin về máy khách từ xa, bằng cách truyền vào cấu trúc `sockaddr`. Các giao thức khác nhau có các biến thể khác nhau của  `struct sockaddr`, có kích thước khác nhau. Cấu trúc đơn giản nhất để sử dụng là `sockaddr_storage` đủ lớn để đại diện cho tất cả các loại sockaddr có thể có. Lưu ý rằng C không có bất kỳ mô hình kế thừa nào. Do đó, chúng ta cần phải ép kiểu cấu trúc của mình một cách rõ ràng thành cấu trúc '**kiểu cơ sở**' `sockaddr`.

```C
    struct sockaddr_storage clientaddr;
    socklen_t clientaddrsize = sizeof(clientaddr);
    int client_id = accept(passive_socket,
            (struct sockaddr *) &clientaddr,
             &clientaddrsize);
```

Chúng tôi đã thấy `getaddrinfo` có thể xây dựng danh sách liên kết các mục nhập `addrinfo` (và mỗi mục nhập này có thể bao gồm dữ liệu cấu hình socket). Điều gì sẽ xảy ra nếu chúng ta muốn biến dữ liệu socket thành địa chỉ IP và cổng? Nhập `getnameinfo` có thể được sử dụng để chuyển đổi thông tin socket cục bộ hoặc từ xa thành **tên miền** hoặc **IP số**. Tương tự, số cổng có thể được biểu diễn dưới dạng **tên dịch vụ** (ví dụ: "http" cho cổng 80). Trong ví dụ bên dưới, chúng tôi yêu cầu các phiên bản số cho địa chỉ IP máy khách và số cổng máy khách.

```C
    socklen_t clientaddrsize = sizeof(clientaddr);
    int client_id = accept(sock_id, (struct sockaddr *) &clientaddr, &clientaddrsize);
    char host[256], port[256];
    getnameinfo((struct sockaddr *) &clientaddr,
          clientaddrsize, host, sizeof(host), port, sizeof(port),
          NI_NUMERICHOST | NI_NUMERICSERV);
```
Việc cần làm: Thảo luận về `NI_MAXHOST` và `NI_MAXSERV` và `NI_NUMERICHOST` 

## Ví dụ getnameinfo: Địa chỉ IP của tôi là gì?

Để lấy danh sách liên kết các địa chỉ IP của máy hiện tại, hãy sử dụng `getifaddrs` sẽ trả về danh sách liên kết các địa chỉ IP IPv4 và IPv6 (và có khả năng là cả các **giao diện** khác nữa). Chúng ta có thể kiểm tra từng mục nhập và sử dụng `getnameinfo` để in địa chỉ IP của máy chủ.
Cấu trúc `ifaddrs` bao gồm họ nhưng không bao gồm kích thước của cấu trúc. Do đó, chúng ta cần xác định thủ công kích thước cấu trúc dựa trên họ (IPv4 v IPv6)
```C
 (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6)
```
Mã đầy đủ được hiển thị bên dưới.
```C
    int required_family = AF_INET; // Thay đổi thành AF_INET6 cho IPv6
    struct ifaddrs *myaddrs, *ifa;
    getifaddrs(&myaddrs);
    char host[256], port[256];
    for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {
        int family = ifa->ifa_addr->sa_family;
        if (family == required_family && ifa->ifa_addr) {
            if (0 == getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                sizeof(struct sockaddr_in6),
                                host, sizeof(host), port, sizeof(port)
                                 , NI_NUMERICHOST | NI_NUMERICSERV  ))
                puts(host);
            }
        }
```
## Địa chỉ IP máy của tôi là gì (phiên bản shell)

Trả lời: sử dụng `ifconfig` (hoặc `ipconfig` của Windows)
Tuy nhiên, lệnh này tạo ra rất nhiều đầu ra cho mỗi giao diện, vì vậy chúng ta có thể lọc đầu ra bằng cách sử dụng `grep`
```
ifconfig | grep inet

Ví dụ đầu ra:
	inet6 fe80::1%lo0 prefixlen 64 scopeid 0x1 
	inet 127.0.0.1 netmask 0xff000000 
	inet6 ::1 prefixlen 128 
	inet6 fe80::7256:81ff:fe9a:9141%en1 prefixlen 64 scopeid 0x5 
	inet 192.168.1.100 netmask 0xffffff00 broadcast 192.168.1.255
```



