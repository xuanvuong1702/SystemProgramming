## Làm cách nào để gửi tín hiệu đến một tiến trình từ shell?
Bạn đã biết một cách để gửi `SIG_INT` chỉ cần gõ `CTRL-C`
Từ shell, bạn có thể sử dụng `kill` (nếu bạn biết ID tiến trình) và `killall` (nếu bạn biết tên tiến trình)
```sh
# Đầu tiên, hãy sử dụng ps và grep để tìm tiến trình mà chúng ta muốn gửi tín hiệu
$ ps au | grep myprogram
angrave  4409   0.0  0.0  2434892    512 s004  R+    2:42PM   0:00.00 myprogram 1 2 3

# Gửi tín hiệu SIGINT đến tiến trình 4409 (tương đương với `CTRL-C`)
$ kill -SIGINT 4409

# Gửi SIGKILL (chấm dứt tiến trình)
$ kill -SIGKILL 4409
$ kill -9 4409
```

`killall` tương tự ngoại trừ việc nó khớp theo tên chương trình. Hai ví dụ tiếp theo, gửi `SIGINT` và sau đó `SIGKILL` để chấm dứt các tiến trình đang chạy `myprogram`
```sh
# Gửi SIGINT (SIGINT có thể bị bỏ qua)
$ killall -SIGINT myprogram

# SIGKILL (-9) không thể bị bỏ qua! 
$ killall -9 myprogram
```
## Làm cách nào để gửi tín hiệu đến một tiến trình từ chương trình C đang chạy?
Sử dụng `raise` hoặc `kill`
```C
int raise(int sig); // Gửi tín hiệu cho chính tôi!
int kill(pid_t pid, int sig); // Gửi tín hiệu đến một tiến trình khác
```
Đối với các tiến trình không phải root, tín hiệu chỉ có thể được gửi đến các tiến trình của cùng một người dùng, tức là bạn không thể chỉ SIGKILL các tiến trình của tôi! Xem kill(2), tức là man -s2 để biết thêm chi tiết.
 

## Làm cách nào để gửi tín hiệu đến một luồng cụ thể?
Sử dụng `pthread_kill`
```C
int pthread_kill(pthread_t thread, int sig)
```

Trong ví dụ bên dưới, luồng mới được tạo đang thực thi `func` sẽ bị gián đoạn bởi `SIGINT`

```C
pthread_create(&tid, NULL, func, args);
pthread_kill(tid, SIGINT);
pthread_kill(pthread_self(), SIGKILL); // gửi SIGKILL cho chính tôi
```

## `pthread_kill(threadid, SIGKILL)` có giết chết tiến trình hay luồng không?
Nó sẽ giết chết toàn bộ tiến trình. Mặc dù các luồng riêng lẻ có thể đặt mặt nạ tín hiệu, nhưng cách xử lý tín hiệu (bảng các trình xử lý/hành động được thực hiện cho mỗi tín hiệu) là *cho mỗi tiến trình*, không phải *cho mỗi luồng*. Điều này có nghĩa là
`sigaction` có thể được gọi từ bất kỳ luồng nào vì bạn sẽ đặt trình xử lý tín hiệu cho tất cả các luồng trong tiến trình.

## Làm cách nào để bắt (xử lý) một tín hiệu?
Bạn có thể chọn xử lý các tín hiệu đang chờ xử lý một cách không đồng bộ hoặc đồng bộ.

Cài đặt trình xử lý tín hiệu để xử lý tín hiệu không đồng bộ, hãy sử dụng `sigaction` (hoặc, đối với các ví dụ đơn giản, `signal`).

Để bắt tín hiệu đang chờ xử lý một cách đồng bộ, hãy sử dụng `sigwait` (chặn cho đến khi tín hiệu được gửi) hoặc `signalfd` (cũng chặn và cung cấp một bộ mô tả tệp có thể được `read()` để truy xuất các tín hiệu đang chờ xử lý).

Xem `Signals, Phần 4` để biết ví dụ về cách sử dụng `sigwait`

