### Đừng lãng phí thời gian chờ đợi

Thông thường, khi bạn gọi `read()`, nếu dữ liệu chưa có sẵn, nó sẽ đợi cho đến khi dữ liệu sẵn sàng trước khi hàm trả về.  Khi bạn đang đọc dữ liệu từ đĩa, độ trễ đó có thể không lâu, nhưng khi bạn đang đọc từ kết nối mạng chậm, có thể mất rất nhiều thời gian để dữ liệu đó đến, nếu nó đến.  

POSIX cho phép bạn đặt một **cờ** trên một **bộ mô tả tệp** sao cho bất kỳ lệnh gọi nào đến `read()` trên bộ mô tả tệp đó sẽ trả về ngay lập tức, cho dù nó đã hoàn thành hay chưa.  Với bộ mô tả tệp của bạn ở chế độ này, lệnh gọi `read()` của bạn sẽ bắt đầu
hoạt động đọc, và trong khi nó đang hoạt động, bạn có thể thực hiện các công việc hữu ích khác.  Điều này được gọi là chế độ **"không chặn"**,
vì lệnh gọi đến `read()` không bị **chặn**.

Để đặt một bộ mô tả tệp không bị chặn:
```C
    // fd là bộ mô tả tệp của tôi
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
```
Đối với **socket**, bạn có thể tạo nó ở chế độ không chặn bằng cách thêm `SOCK_NONBLOCK` vào đối số thứ hai của `socket()`:
```C
    fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
```
Khi một tệp ở chế độ không chặn và bạn gọi `read()`, nó sẽ trả về ngay lập tức với bất kỳ byte nào có sẵn.
Giả sử 100 byte đã đến từ máy chủ ở đầu kia của socket của bạn và bạn gọi `read(fd, buf, 150)`.
Read sẽ trả về ngay lập tức với giá trị 100, nghĩa là nó đã đọc được 100 trong số 150 byte bạn yêu cầu.
Giả sử bạn đã cố gắng đọc dữ liệu còn lại bằng một lệnh gọi đến `read(fd, buf+100, 50)`, nhưng 50 byte cuối cùng vẫn chưa
đến.  `read()` sẽ trả về -1 và đặt biến lỗi toàn cục **errno** thành 
`EAGAIN` hoặc `EWOULDBLOCK`.  Đó là cách hệ thống cho bạn biết rằng dữ liệu chưa sẵn sàng.

`write()` cũng hoạt động ở chế độ không chặn.  Giả sử bạn muốn gửi 40.000 byte đến một máy chủ từ xa bằng cách sử dụng socket.
Hệ thống chỉ có thể gửi một số byte nhất định tại một thời điểm. Các hệ thống thông thường có thể gửi khoảng 23.000 byte tại một thời điểm. Ở chế độ không chặn, `write(fd, buf, 40000)` sẽ trả về số byte mà nó có thể
gửi ngay lập tức, hoặc khoảng 23.000.  Nếu bạn gọi `write()` ngay lập tức một lần nữa, nó sẽ trả về -1 và đặt `errno` thành
`EAGAIN` hoặc `EWOULDBLOCK`. Đó là cách hệ thống cho bạn biết rằng nó vẫn đang bận gửi khối dữ liệu cuối cùng,
và chưa sẵn sàng để gửi thêm.

### Làm cách nào để kiểm tra khi I/O đã hoàn thành?

Có một vài cách.  Hãy xem cách thực hiện bằng cách sử dụng *select* và *epoll*.

#### select
```C
    int select(int nfds, 
               fd_set *readfds, 
               fd_set *writefds,
               fd_set *exceptfds, 
               struct timeval *timeout);
```
Với ba tập hợp các **bộ mô tả tệp**, `select()` sẽ đợi bất kỳ bộ mô tả tệp nào trong số đó trở nên 'sẵn sàng'.
* `readfds` - một bộ mô tả tệp trong `readfds` đã sẵn sàng khi có dữ liệu có thể đọc được hoặc **EOF** đã được đạt đến.
* `writefds` - một bộ mô tả tệp trong `writefds` đã sẵn sàng khi một lệnh gọi đến `write()` sẽ thành công.
* `exceptfds` - dành riêng cho hệ thống, không được xác định rõ.  Chỉ cần chuyển `NULL` cho cái này.

`select()` trả về tổng số bộ mô tả tệp đã sẵn sàng.  Nếu không có bộ mô tả nào trở nên
sẵn sàng trong thời gian được xác định bởi *timeout*, nó sẽ trả về 0.  Sau khi `select()` trả về, 
người gọi sẽ cần lặp qua các bộ mô tả tệp trong `readfds` và/hoặc `writefds` để xem cái nào
đã sẵn sàng. Vì `readfds` và `writefds` hoạt động như cả tham số đầu vào và đầu ra, khi `select()`
cho biết rằng có các bộ mô tả tệp đã sẵn sàng, nó sẽ ghi đè chúng để
chỉ phản ánh các bộ mô tả tệp đã sẵn sàng. Trừ khi người gọi có ý định gọi
`select()` chỉ một lần, sẽ là một ý tưởng tốt để lưu một bản sao của `readfds` và `writefds` trước
khi gọi nó.

```C
    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    for (int i=0; i < read_fd_count; i++)
      FD_SET(my_read_fds[i], &readfds);
    for (int i=0; i < write_fd_count; i++)
      FD_SET(my_write_fds[i], &writefds);

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    int num_ready = select(FD_SETSIZE, &readfds, &writefds, NULL, &timeout);

    if (num_ready < 0) {
      perror("lỗi trong select()");
    } else if (num_ready == 0) {
      printf("hết thời gian chờ\n");
    } else {
      for (int i=0; i < read_fd_count; i++)
        if (FD_ISSET(my_read_fds[i], &readfds))
          printf("fd %d đã sẵn sàng để đọc\n", my_read_fds[i]);
      for (int i=0; i < write_fd_count; i++)
        if (FD_ISSET(my_write_fds[i], &writefds))
          printf("fd %d đã sẵn sàng để ghi\n", my_write_fds[i]);
    }
```

[Để biết thêm thông tin về select()](http://pubs.opengroup.org/onlinepubs/9699919799/functions/select.html)

## epoll

*epoll* không phải là một phần của POSIX, nhưng nó được Linux hỗ trợ.  Nó là một cách hiệu quả hơn để đợi nhiều
bộ mô tả tệp.  Nó sẽ cho bạn biết chính xác những bộ mô tả nào đã sẵn sàng. Nó thậm chí còn cung cấp cho bạn một cách để lưu trữ
một lượng nhỏ dữ liệu với mỗi bộ mô tả, như chỉ số mảng hoặc con trỏ, giúp bạn dễ dàng truy cập
dữ liệu của mình được liên kết với bộ mô tả đó hơn.

Để sử dụng **epoll**, trước tiên bạn phải tạo một bộ mô tả tệp đặc biệt với [epoll_create()](http://linux.die.net/man/2/epoll_create).  Bạn sẽ không đọc hoặc ghi vào tệp này
bộ mô tả; bạn sẽ chỉ chuyển nó cho các hàm `epoll_xxx` khác và gọi
`close()` trên nó vào cuối.
```C
    epfd = epoll_create(1);
```
Đối với mỗi bộ mô tả tệp bạn muốn theo dõi bằng epoll, bạn sẽ cần thêm nó 
vào các **cấu trúc dữ liệu epoll** 
bằng cách sử dụng [epoll_ctl()](http://linux.die.net/man/2/epoll_ctl) với tùy chọn `EPOLL_CTL_ADD`.  Bạn có thể thêm bất kỳ
số lượng bộ mô tả tệp nào vào nó.
```C
    struct epoll_event event;
    event.events = EPOLLOUT;  // EPOLLIN == đọc, EPOLLOUT == ghi
    event.data.ptr = mypointer;
    epoll_ctl(epfd, EPOLL_CTL_ADD, mypointer->fd, &event)
```
Để đợi một số bộ mô tả tệp trở nên sẵn sàng, hãy sử dụng [epoll_wait()](http://linux.die.net/man/2/epoll_wait).
Cấu trúc `epoll_event` mà nó điền sẽ chứa dữ liệu bạn đã cung cấp trong `event.data` khi bạn
đã thêm bộ mô tả tệp này. Điều này giúp bạn dễ dàng tra cứu dữ liệu của riêng mình được liên kết
với bộ mô tả tệp này.
```C
    int num_ready = epoll_wait(epfd, &event, 1, timeout_milliseconds);
    if (num_ready > 0) {
      MyData *mypointer = (MyData*) event.data.ptr;
      printf("sẵn sàng để ghi vào %d\n", mypointer->fd);
    }
```
Giả sử bạn đang chờ để ghi dữ liệu vào một bộ mô tả tệp, nhưng bây giờ bạn muốn đợi để đọc dữ liệu từ nó.
Chỉ cần sử dụng `epoll_ctl()` với tùy chọn `EPOLL_CTL_MOD` để thay đổi loại hoạt động bạn đang theo dõi.
```C
    event.events = EPOLLIN;
    event.data.ptr = mypointer;
    epoll_ctl(epfd, EPOLL_CTL_MOD, mypointer->fd, &event);
```
Để hủy đăng ký một bộ mô tả tệp khỏi epoll trong khi để các bộ mô tả khác hoạt động, hãy sử dụng `epoll_ctl()` với tùy chọn `EPOLL_CTL_DEL`.
```C
    epoll_ctl(epfd, EPOLL_CTL_DEL, mypointer->fd, NULL);
```
Để tắt một phiên bản epoll, hãy đóng bộ mô tả tệp của nó.
```C
    close(epfd);
```
Ngoài `read()` và `write()` không chặn, bất kỳ lệnh gọi nào đến `connect()` trên một socket không chặn cũng sẽ
không chặn. Để đợi kết nối hoàn tất, hãy sử dụng `select()` hoặc epoll để đợi socket có thể ghi.


## Bài đăng blog thú vị về các trường hợp cạnh với select

https://idea.popcount.org/2017-01-06-select-is-fundamentally-broken/






