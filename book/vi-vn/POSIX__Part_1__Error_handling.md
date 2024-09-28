# Xử lý lỗi POSIX là gì?

Trong các ngôn ngữ khác, bạn có thể thấy việc xử lý lỗi được triển khai bằng các ngoại lệ (exception). Mặc dù về mặt kỹ thuật, bạn có thể sử dụng chúng trong C -- Bạn giữ một ngăn xếp các khối try/catch và sử dụng `setjmp` và `longjmp` để chuyển đến các khối đó tương ứng -- việc xử lý lỗi trong C thường được thực hiện bằng cách xử lý lỗi POSIX, mã thường trông như thế này.

```C
int ret = some_system_call()
if(ret == ERROR_CODE){
switch(errno){
// Thực hiện các việc khác nhau dựa trên số errno.
}
}

```

Trong kernel, việc sử dụng `goto` được sử dụng rất nhiều để dọn dẹp các phần khác nhau của ứng dụng. **Bạn không nên sử dụng goto** vì chúng làm cho mã khó đọc hơn. Goto trong kernel tồn tại là do sự cần thiết, vì vậy đừng học theo.

## `errno` là gì và khi nào nó được đặt?
	
POSIX định nghĩa một số nguyên đặc biệt `errno` được đặt khi một lệnh gọi hệ thống thất bại.
Giá trị ban đầu của `errno` là 0 (tức là không có lỗi).
Khi một lệnh gọi hệ thống thất bại, nó thường sẽ trả về -1 để biểu thị lỗi và đặt `errno`

## Còn nhiều luồng thì sao?

Mỗi luồng có bản sao `errno` riêng. Điều này rất hữu ích; nếu không, lỗi trong một luồng sẽ ảnh hưởng đến trạng thái lỗi của luồng khác.

## Khi nào `errno` được đặt lại về 0?

Nó không được đặt lại trừ khi bạn đặc biệt đặt lại về 0! Khi các lệnh gọi hệ thống thành công, chúng _không_ đặt lại giá trị của `errno`.

Điều này có nghĩa là bạn chỉ nên dựa vào giá trị của errno nếu bạn biết một lệnh gọi hệ thống đã thất bại (ví dụ: nó trả về -1).

## Những điều cần lưu ý và thực tiễn tốt nhất khi sử dụng `errno` là gì?

Hãy cẩn thận khi xử lý lỗi phức tạp, sử dụng các lệnh gọi thư viện hoặc lệnh gọi hệ thống có thể thay đổi giá trị của `errno`. Trong thực tế, việc sao chép giá trị của errno vào một biến int sẽ an toàn hơn:

```C
// Không an toàn - fprintf đầu tiên có thể thay đổi giá trị của errno trước khi chúng ta sử dụng nó!
if (-1 == sem_wait(&s)) {
   fprintf(stderr, "An error occurred!");
   fprintf(stderr, "The error value is %d\n", errno);
}
// Tốt hơn, sao chép giá trị trước khi thực hiện nhiều lệnh gọi hệ thống và thư viện hơn
if (-1 == sem_wait(&s)) {
   int errno_saved = errno;
   fprintf(stderr, "An error occurred!");
   fprintf(stderr, "The error value is %d\n", errno_saved);
}
```

Tương tự, nếu trình xử lý tín hiệu của bạn thực hiện bất kỳ lệnh gọi hệ thống hoặc thư viện nào, thì nên lưu giá trị ban đầu của errno và khôi phục giá trị trước khi trả về:

```C
void handler(int signal) {
   int errno_saved = errno;

   // thực hiện các lệnh gọi hệ thống có thể thay đổi errno

   errno = errno_saved;
}
```

## Làm cách nào bạn có thể in ra thông báo chuỗi được liên kết với một số lỗi cụ thể?

Sử dụng `strerror` để lấy mô tả ngắn gọn (bằng tiếng Anh) về giá trị lỗi

```C
char *mesg = strerror(errno);
fprintf(stderr, "An error occurred (errno=%d): %s", errno, mesg);
```

## `perror` và `strerror` liên quan với nhau như thế nào?

Trong các trang trước, chúng ta đã sử dụng perror để in ra lỗi vào lỗi tiêu chuẩn. Sử dụng `strerror`, giờ đây chúng ta có thể viết một triển khai đơn giản của `perror`:

```C
void perror(char *what) {
   fprintf(stderr, "%s: %s\n", what, strerror(errno));
}
```

## Những điều cần lưu ý khi sử dụng `strerror` là gì?

Thật không may, `strerror` không an toàn cho luồng. Nói cách khác, hai luồng không thể gọi nó cùng một lúc!

Có hai cách giải quyết: Đầu tiên, chúng ta có thể sử dụng khóa mutex để xác định một phần quan trọng và một bộ đệm cục bộ. Cùng một mutex nên được sử dụng bởi tất cả các luồng ở tất cả các nơi gọi `strerror`

```C
pthread_mutex_lock(&m);
char *result = strerror(errno);
char *message = malloc(strlen(result) + 1);
strcpy(message, result);
pthread_mutex_unlock(&m);
fprintf(stderr, "An error occurred (errno=%d): %s", errno, message);
free(message);
```

Ngoài ra, hãy sử dụng `strerror_r` ít di động hơn nhưng an toàn cho luồng. `perror` an toàn cho luồng, đó là lý do tại sao nó được ưu tiên trong môi trường đa luồng nếu có thể.

## `EINTR` là gì? Nó có ý nghĩa gì đối với `sem_wait`? `read`? `write`?

Một số lệnh gọi hệ thống có thể bị gián đoạn khi một tín hiệu (ví dụ: SIGCHLD, SIGPIPE,...) được gửi đến tiến trình. Tại thời điểm này, lệnh gọi hệ thống có thể trả về mà không thực hiện bất kỳ hành động nào! Ví dụ: các byte có thể chưa được đọc/ghi, semaphore wait có thể chưa được chờ đợi.

Sự gián đoạn này có thể được phát hiện bằng cách kiểm tra giá trị trả về và nếu `errno` là `EINTR`. Trong trường hợp đó, lệnh gọi hệ thống nên được thử lại. Thường thấy loại vòng lặp sau bao bọc một lệnh gọi hệ thống (chẳng hạn như sem_wait).

```C
while ((-1 == systemcall(...)) && (errno == EINTR)) { /* lặp lại! */}
```

Hãy cẩn thận để viết `== EINTR`, không phải `= EINTR`.

Hoặc, nếu giá trị kết quả cần được sử dụng sau này...

```C
while ((-1 == (result = systemcall(...))) && (errno == EINTR)) { /* lặp lại! */}
```

Trên Linux, việc gọi `read` và `write` vào đĩa cục bộ thường sẽ không trả về EINTR (thay vào đó, hàm sẽ tự động được khởi động lại cho bạn). Tuy nhiên, việc gọi `read` và `write` trên bộ mô tả tệp tương ứng với luồng mạng _có thể_ trả về EINTR.

## Lệnh gọi hệ thống nào có thể bị gián đoạn và cần được bao bọc?

Sử dụng trang hướng dẫn! Trang hướng dẫn bao gồm danh sách các lỗi (tức là giá trị errno) có thể được đặt bởi lệnh gọi hệ thống. Nguyên tắc chung là các lệnh gọi 'chậm' (chặn) (ví dụ: ghi vào socket) có thể bị gián đoạn nhưng các lệnh gọi không chặn nhanh (ví dụ: pthread_mutex_lock) sẽ không bị gián đoạn.

Từ trang hướng dẫn tín hiệu 7 của linux:

"Nếu một trình xử lý tín hiệu được gọi trong khi một lệnh gọi hệ thống hoặc lệnh gọi hàm thư viện bị chặn, thì:
* lệnh gọi được tự động khởi động lại sau khi trình xử lý tín hiệu trả về; hoặc
* lệnh gọi thất bại với lỗi EINTR.
Hành vi nào trong hai hành vi này xảy ra phụ thuộc vào giao diện và liệu trình xử lý tín hiệu có được thiết lập bằng cờ SA_RESTART hay không (xem sigaction(2)). Các chi tiết khác nhau giữa các hệ thống UNIX; dưới đây, các chi tiết dành cho Linux.

Nếu một lệnh gọi bị chặn đến một trong các giao diện sau bị gián đoạn bởi trình xử lý tín hiệu, thì lệnh gọi sẽ tự động được khởi động lại sau khi trình xử lý tín hiệu trả về nếu cờ SA_RESTART được sử dụng; nếu không, lệnh gọi sẽ thất bại với lỗi EINTR:

* các lệnh gọi read(2), readv(2), write(2), writev(2) và ioctl(2) trên các thiết bị "chậm". Thiết bị "chậm" là thiết bị mà lệnh gọi I/O có thể bị chặn trong một thời gian không xác định, ví dụ: terminal, pipe hoặc socket. (Đĩa không phải là thiết bị chậm theo định nghĩa này.) Nếu một lệnh gọi I/O trên thiết bị chậm đã truyền một số dữ liệu vào thời điểm nó bị gián đoạn bởi trình xử lý tín hiệu, thì lệnh gọi sẽ trả về trạng thái thành công (thường là số byte được truyền).
"

Lưu ý, thật dễ dàng để tin rằng việc đặt cờ 'SA_RESTART' là đủ để làm cho toàn bộ vấn đề này biến mất. Thật không may, điều đó không đúng: vẫn có các lệnh gọi hệ thống có thể trả về sớm và đặt `EINTR`! Xem [signal(7)](https://cs-education.github.io/sysassets/man_pages/html/man7/signal.7.html) để biết chi tiết. 

## Ngoại lệ Errno?

Có một số tiện ích POSIX có số lỗi riêng và không sử dụng `errno`. Một ví dụ là khi bạn gọi `getaddrinfo` trả về mã lỗi làm kết quả của nó. Hàm để kiểm tra chuyển đổi số lỗi được trả về thành chuỗi là [gai_strerr](https://linux.die.net/man/3/gai_strerror).