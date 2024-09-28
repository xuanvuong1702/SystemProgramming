## Macro Chờ

### Tôi có thể tìm ra giá trị thoát của tiến trình con không?

Bạn có thể tìm thấy 8 bit thấp nhất của giá trị thoát của tiến trình con (giá trị trả về của hàm `main()` hoặc giá trị được bao gồm trong hàm `exit()`): Sử dụng "macro Wait" - thông thường bạn sẽ sử dụng "WIFEXITED" và "WEXITSTATUS". Xem trang hướng dẫn `wait`/`waitpid` để biết thêm thông tin).

```C
int status;
pid_t child = fork();
if (child == -1) return 1; // Thất bại
if (child > 0) { /* Tôi là tiến trình cha - chờ tiến trình con kết thúc */
    pid_t pid = waitpid(child, &status, 0);
    if (pid != -1 && WIFEXITED(status)) {
        int low8bits = WEXITSTATUS(status);
        printf("Tiến trình %d trả về %d" , pid, low8bits);
    }
} else { /* Tôi là tiến trình con */
    // làm điều gì đó thú vị
    execl("/bin/ls", "/bin/ls", ".", (char *) NULL); // "ls ."
}
```

Một tiến trình chỉ có thể có 256 giá trị trả về, các bit còn lại mang thông tin bổ sung.

## Dịch Bit

Lưu ý rằng không cần phải ghi nhớ điều này, đây chỉ là tổng quan cấp cao về cách thông tin được lưu trữ bên trong các biến trạng thái.

Từ mã nguồn Android:
```C
/* Nếu WIFEXITED(STATUS), 8 bit thấp nhất của trạng thái. */

#define __WEXITSTATUS(status) (((status) & 0xff00) >> 8)

/* Nếu WIFSIGNALED(STATUS), tín hiệu kết thúc. */

#define __WTERMSIG(status) ((status) & 0x7f)

/* Nếu WIFSTOPPED(STATUS), tín hiệu đã dừng tiến trình con. */

#define __WSTOPSIG(status) __WEXITSTATUS(status)

/* Khác không nếu STATUS cho biết kết thúc bình thường. */

#define __WIFEXITED(status) (__WTERMSIG(status) == 0)
```

Kernel có cách thức nội bộ để theo dõi trạng thái đã được báo hiệu, đã thoát hoặc đã dừng. API đó được trừu tượng hóa để các nhà phát triển kernel có thể tự do thay đổi theo ý muốn.

## Hãy cẩn thận.

Hãy nhớ rằng các macro chỉ có ý nghĩa nếu điều kiện tiên quyết được đáp ứng. Có nghĩa là trạng thái thoát của một tiến trình sẽ không được xác định nếu tiến trình đó nhận được tín hiệu. Các macro sẽ không thực hiện việc kiểm tra cho bạn, vì vậy tùy thuộc vào lập trình viên để đảm bảo logic kiểm tra được thực hiện.

# Tín hiệu

## Tín hiệu là gì?

Tín hiệu là một cấu trúc được cung cấp cho chúng ta bởi kernel. Nó cho phép một tiến trình gửi một tín hiệu không đồng bộ (giống như một tin nhắn) đến một tiến trình khác. Nếu tiến trình đó muốn chấp nhận tín hiệu, nó có thể, và sau đó, đối với hầu hết các tín hiệu, có thể quyết định phải làm gì với tín hiệu đó. Dưới đây là danh sách ngắn (không đầy đủ) các tín hiệu.

|   Tên   |             Hành động mặc định             | Trường hợp sử dụng thông thường |
|----------|----------------------------------------|--------------------------------|
| SIGINT   | Kết thúc tiến trình (Có thể bắt được)      | Yêu cầu tiến trình dừng lại một cách nhẹ nhàng |
| SIGQUIT  | Kết thúc tiến trình (Có thể bắt được)      | Yêu cầu tiến trình dừng lại một cách mạnh mẽ |
| SIGSTOP  | Dừng tiến trình (Không thể bắt được)        | Dừng tiến trình để tiếp tục sau đó |
| SIGCONT  | Tiếp tục một tiến trình                    | Tiếp tục chạy tiến trình |
| SIGKILL  | Kết thúc tiến trình (Không thể bắt được)   | Bạn muốn tiến trình của mình biến mất |

## Khi nào tín hiệu được tạo ra?

* Khi người dùng gửi tín hiệu. Ví dụ: bạn đang ở terminal và bạn gửi `CTRL-C`
* Khi một sự kiện hệ thống xảy ra. Ví dụ: bạn nhận được `SIGCHILD` sau khi fork để nhận biết khi một trong các tiến trình con của bạn đã thoát.
* Khi một chương trình khác gửi nó. Ví dụ: khi bạn thực thi `kill -9 PID`, nó sẽ gửi `SIGKILL`
* Khi một ngắt phần cứng thích hợp được kích hoạt. Ví dụ: nếu bạn truy cập một trang mà bạn không được phép, phần cứng sẽ tạo ra ngắt lỗi phân đoạn được kernel chặn lại. Kernel tìm thấy tiến trình gây ra lỗi này và gửi tín hiệu ngắt phần mềm `SIGSEGV`.

## Tôi có thể tạm dừng tiến trình con của mình không?

Có! Bạn có thể tạm dừng tạm thời một tiến trình đang chạy bằng cách gửi cho nó tín hiệu SIGSTOP.
Nếu thành công, nó sẽ đóng băng một tiến trình; tức là tiến trình sẽ không được cấp thêm thời gian CPU nữa.

Để cho phép một tiến trình tiếp tục thực thi, hãy gửi cho nó tín hiệu SIGCONT.

Ví dụ:
Đây là chương trình in chậm một dấu chấm mỗi giây, tối đa 59 dấu chấm.
```C
#include <unistd.h>
#include <stdio.h>
int main() {
    printf("My pid is %d\n", getpid());
    int i = 60;
    while (--i) { 
        write(1, ".", 1);
        sleep(1);
    }
    write(1, "Done!", 5);
    return 0;
}
```
Đầu tiên, chúng ta sẽ bắt đầu tiến trình trong nền (lưu ý dấu & ở cuối).
Sau đó, gửi cho nó một tín hiệu từ tiến trình shell bằng cách sử dụng lệnh kill.
```
>./program &
My pid is 403
...
>kill -SIGSTOP 403
>kill -SIGCONT 403
```

## Làm cách nào để kill/stop/suspend tiến trình con của tôi từ C?

Trong C, gửi tín hiệu đến tiến trình con bằng cách sử dụng lệnh gọi POSIX `kill`,
```C
kill(child, SIGUSR1); // Gửi tín hiệu do người dùng định nghĩa
kill(child, SIGSTOP); // Dừng tiến trình con (tiến trình con không thể ngăn chặn điều này)
kill(child, SIGTERM); // Kết thúc tiến trình con (tiến trình con có thể ngăn chặn điều này)
kill(child, SIGINT); // Tương đương với CTRL-C (theo mặc định sẽ đóng tiến trình)
```

Như chúng ta đã thấy ở trên, cũng có một lệnh kill có sẵn trong shell,
ví dụ: lấy danh sách các tiến trình đang chạy và sau đó kết thúc tiến trình 45 và tiến trình 46.
```
ps
kill -l 
kill -9 45
kill -s TERM 46
```
## Làm cách nào tôi có thể phát hiện "CTRL-C" và dọn dẹp một cách duyên dáng?

Chúng ta sẽ quay lại tín hiệu sau - đây chỉ là phần giới thiệu ngắn. Trên hệ thống Linux, hãy xem `man 7 signal` nếu bạn muốn tìm hiểu thêm (ví dụ: danh sách các lệnh gọi hệ thống và thư viện an toàn với tín hiệu không đồng bộ).

Có những hạn chế nghiêm ngặt đối với mã thực thi bên trong trình xử lý tín hiệu. Hầu hết các lệnh gọi thư viện và hệ thống không 'an toàn với tín hiệu không đồng bộ' - chúng có thể không được sử dụng bên trong trình xử lý tín hiệu vì chúng không an toàn khi tái nhập. Trong một chương trình đơn luồng, việc xử lý tín hiệu tạm thời làm gián đoạn việc thực thi chương trình để thực thi mã trình xử lý tín hiệu thay thế. Giả sử chương trình ban đầu của bạn bị gián đoạn trong khi thực thi mã thư viện của `malloc`; các cấu trúc bộ nhớ được sử dụng bởi malloc sẽ không ở trạng thái nhất quán. Gọi `printf` (sử dụng `malloc`) như một phần của trình xử lý tín hiệu là không an toàn và sẽ dẫn đến "hành vi không xác định", tức là nó không còn là một chương trình hữu ích, có thể đoán trước được. Trong thực tế, chương trình của bạn có thể bị sập, tính toán hoặc tạo ra kết quả không chính xác hoặc ngừng hoạt động ("deadlock"), tùy thuộc vào chính xác những gì chương trình của bạn đang thực thi khi nó bị gián đoạn để thực thi mã trình xử lý tín hiệu.


Một cách sử dụng phổ biến của trình xử lý tín hiệu là đặt cờ boolean được thăm dò (đọc) thường xuyên như một phần của hoạt động bình thường của chương trình. Ví dụ:
```C
int pleaseStop; // Xem ghi chú về lý do tại sao "volatile sig_atomic_t" tốt hơn

void handle_sigint(int signal) {
    pleaseStop = 1;
}

int main() {
    signal(SIGINT, handle_sigint);
    pleaseStop = 0;
    while (! pleaseStop) { 
        /* logic ứng dụng ở đây */ 
    }
    /* mã dọn dẹp ở đây */
}
```

Mã trên có vẻ chính xác trên lý thuyết. Tuy nhiên, chúng ta cần cung cấp gợi ý cho trình biên dịch và cho lõi CPU sẽ thực thi vòng lặp `main()`. Chúng ta cần ngăn chặn tối ưu hóa trình biên dịch: Biểu thức `! pleaseStop` dường như là một vòng lặp bất biến, tức là luôn đúng, vì vậy có thể được đơn giản hóa thành `true`. Thứ hai, chúng ta cần đảm bảo rằng giá trị của `pleaseStop` không được lưu trong bộ nhớ cache bằng cách sử dụng thanh ghi CPU và thay vào đó luôn được đọc từ và ghi vào bộ nhớ chính. Kiểu `sig_atomic_t` ngụ ý rằng tất cả các bit của biến có thể được đọc hoặc sửa đổi như một "hoạt động nguyên tử" - một hoạt động duy nhất không thể bị gián đoạn. Không thể đọc một giá trị được tạo thành từ một số giá trị bit mới và giá trị bit cũ.

Bằng cách chỉ định `pleaseStop` với kiểu chính xác `volatile sig_atomic_t`, chúng ta có thể viết mã di động trong đó vòng lặp chính sẽ thoát sau khi trình xử lý tín hiệu trả về. Kiểu `sig_atomic_t` có thể lớn bằng `int` trên hầu hết các nền tảng hiện đại nhưng trên các hệ thống nhúng có thể nhỏ bằng `char` và chỉ có thể biểu diễn các giá trị (-127 đến 127).

```C
volatile sig_atomic_t pleaseStop;
```
Hai ví dụ về mẫu này có thể được tìm thấy trong "COMP", một máy tính 4 bit 1Hz dựa trên terminal (https://github.com/gto76/comp-cpp/blob/1bf9a77eaf8f57f7358a316e5bbada97f2dc8987/src/output.c#L121).
Hai cờ boolean được sử dụng. Một để đánh dấu việc gửi `SIGINT` (CTRL-C) và tắt chương trình một cách duyên dáng, và cờ kia để đánh dấu tín hiệu `SIGWINCH` để phát hiện thay đổi kích thước terminal và vẽ lại toàn bộ màn hình. 

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait/_edit">
Trở lại: Forking, Phần 2: Fork, Exec, Wait
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Processes-Review-Questions">
Tiếp theo: Câu hỏi ôn tập về Tiến trình
</a>
</div>
