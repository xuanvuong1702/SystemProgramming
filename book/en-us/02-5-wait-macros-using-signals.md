# Các Macro Wait

## Tôi có thể tìm ra giá trị thoát của quy trình con của tôi không?

Bạn có thể tìm thấy 8 bit thấp nhất của giá trị thoát của quy trình con (giá trị trả về của `main()` hoặc giá trị bao gồm trong `exit()`): Sử dụng "Các macro Wait" - thường bạn sẽ sử dụng "WIFEXITED" và "WEXITSTATUS". Xem trang man của `wait`/`waitpid` để biết thêm thông tin).
```C
int status;
pid_t child = fork();
if (child == -1) return 1; // Thất bại
if (child > 0) { /* Tôi là quy trình cha - chờ quy trình con hoàn thành */
    pid_t pid = waitpid(child, &status, 0);
    if (pid != -1 && WIFEXITED(status)) {
        int low8bits = WEXITSTATUS(status);
        printf("Quy trình %d trả về %d" , pid, low8bits);
    }
} else { /* Tôi là quy trình con */
    // làm điều gì đó thú vị
    execl("/bin/ls", "/bin/ls", ".", (char *) NULL); // "ls ."
}
```

Một quy trình chỉ có thể có 256 giá trị trả về, phần còn lại của các bit là thông tin.

## Dịch bit

Lưu ý rằng không cần phải ghi nhớ điều này, đây chỉ là một cái nhìn tổng quan về cách thông tin được lưu trữ bên trong các biến trạng thái

Từ mã nguồn Android:
```C
/* Nếu WIFEXITED(STATUS), 8 bit thấp nhất của trạng thái. */

#define __WEXITSTATUS(status) (((status) & 0xff00) >> 8)

/* Nếu WIFSIGNALED(STATUS), tín hiệu kết thúc. */

#define __WTERMSIG(status) ((status) & 0x7f)

/* Nếu WIFSTOPPED(STATUS), tín hiệu đã dừng quy trình con. */

#define __WSTOPSIG(status) __WEXITSTATUS(status)

/* Khác không nếu STATUS chỉ ra việc kết thúc bình thường. */

#define __WIFEXITED(status) (__WTERMSIG(status) == 0)
```

Hạt nhân có một cách nội bộ để theo dõi việc được tín hiệu, thoát ra, hoặc dừng lại. API này được trừu tượng hóa để các nhà phát triển hạt nhân có thể tự do thay đổi theo ý muốn.

## Cẩn thận.

Hãy nhớ rằng các macro chỉ có ý nghĩa nếu điều kiện tiên quyết được đáp ứng. Nghĩa là trạng thái thoát của một quy trình sẽ không được xác định nếu quy trình đó được tín hiệu. Các macro sẽ không kiểm tra cho bạn, vì vậy nó phụ thuộc vào người lập trình để đảm bảo rằng logic kiểm tra ra.

# Tín hiệu

## Tín hiệu là gì?

Tín hiệu là một cấu trúc được cung cấp cho chúng ta bởi hạt nhân. Nó cho phép một quy trình gửi một tín hiệu (nghĩ đến một tin nhắn) đến một quy trình khác một cách không đồng bộ. Nếu quy trình đó muốn chấp nhận tín hiệu, nó có thể, và sau đó, đối với hầu hết các tín hiệu, có thể quyết định làm gì với tín hiệu đó. Dưới đây là một danh sách ngắn (không toàn diện) về các tín hiệu.

|   Tên   |             Hành động Mặc định             | Trường hợp Sử dụng Thông thường |
|----------|----------------------------------------|--------------------------------|
| SIGINT   | Kết thúc Quy trình (Có thể bắt được)      | Yêu cầu quy trình dừng lại một cách tốt đẹp |
| SIGQUIT  | Kết thúc Quy trình (Có thể bắt được)      | Yêu cầu quy trình dừng lại một cách mạnh mẽ |
| SIGSTOP  | Dừng Quy trình (Không thể bắt được)        | Dừng quy trình để tiếp tục |
| SIGCONT  | Tiếp tục một Quy trình                    | Tiếp tục chạy quy trình |
| SIGKILL  | Kết thúc Quy trình (Không thể bắt được)   | Bạn muốn quy trình của mình biến mất |

## Khi nào tín hiệu được tạo ra?

* Khi người dùng gửi một tín hiệu. Ví dụ, bạn đang ở terminal, và bạn gửi `CTRL-C`
* Khi một sự kiện hệ thống xảy ra. Ví dụ, bạn nhận được một `SIGCHILD` sau khi fork để nhận biết khi một trong những quy trình con của bạn đã thoát.
* Khi một chương trình khác gửi nó. Ví dụ, khi bạn thực hiện `kill -9 PID`, nó gửi `SIGKILL`
* Khi một ngắt phần cứng phù hợp được kích hoạt. Ví dụ, nếu bạn truy cập vào một trang mà bạn không được phép, phần cứng tạo ra một ngắt segfault mà được chặn bởi hạt nhân. Hạt nhân tìm quy trình đã gây ra điều này và gửi một tín hiệu ngắt phần mềm `SIGSEGV`.

## Tôi có thể tạm dừng quy trình con của mình không?

Có! Bạn có thể tạm thời tạm dừng một quy trình đang chạy bằng cách gửi cho nó một tín hiệu SIGSTOP.
Nếu thành công, nó sẽ đóng băng quy trình; tức là, quy trình sẽ không được cấp thêm thời gian CPU nữa.

Để cho phép một quy trình tiếp tục thực thi, gửi cho nó tín hiệu SIGCONT.

Ví dụ,
Dưới đây là chương trình in ra một dấu chấm mỗi giây, lên đến 59 dấu chấm.

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
Chúng ta sẽ bắt đầu quy trình ở chế độ nền (chú ý dấu & ở cuối).
Sau đó gửi cho nó một tín hiệu từ quy trình shell bằng cách sử dụng lệnh kill.
```
>./program &
My pid is 403
...
>kill -SIGSTOP 403
>kill -SIGCONT 403
```
## Làm thế nào để tôi giết/dừng/tạm dừng quy trình con từ C?
Trong C, gửi một tín hiệu cho quy trình con bằng cách sử dụng lệnh `kill` của POSIX,
```C
kill(child, SIGUSR1); // Gửi một tín hiệu do người dùng định nghĩa
kill(child, SIGSTOP); // Dừng quy trình con (quy trình con không thể ngăn chặn điều này)
kill(child, SIGTERM); // Kết thúc quy trình con (quy trình con có thể ngăn chặn điều này)
kill(child, SIGINT); // Tương đương với CTRL-C (mặc định là đóng quy trình)
```
Như chúng ta đã thấy ở trên, cũng có một lệnh kill có sẵn trong shell
ví dụ: lấy danh sách các quy trình đang chạy và sau đó kết thúc quy trình 45 và quy trình 46
```
ps
kill -l 
kill -9 45
kill -s TERM 46
```
## Làm thế nào tôi có thể phát hiện "CTRL-C" và dọn dẹp một cách nhẹ nhàng?

Chúng ta sẽ quay lại với tín hiệu sau - đây chỉ là một sự giới thiệu ngắn gọn. Trên hệ thống Linux, xem `man -s7 signal` nếu bạn quan tâm tìm hiểu thêm (ví dụ danh sách các lệnh hệ thống và thư viện là an toàn với tín hiệu không đồng bộ).

Có những hạn chế nghiêm ngặt về mã thực thi bên trong trình xử lý tín hiệu. Hầu hết các lệnh thư viện và hệ thống không 'an toàn với tín hiệu không đồng bộ' - chúng không được sử dụng bên trong trình xử lý tín hiệu vì chúng không an toàn khi tái nhập. Trong một chương trình đơn luồng, việc xử lý tín hiệu tạm thời gián đoạn việc thực thi chương trình để thực thi mã trình xử lý tín hiệu thay thế. Giả sử chương trình gốc của bạn bị gián đoạn trong khi thực thi mã thư viện của `malloc`; các cấu trúc bộ nhớ được sử dụng bởi malloc sẽ không ở trong một trạng thái nhất quán. Gọi `printf` (sử dụng `malloc`) như một phần của trình xử lý tín hiệu không an toàn và sẽ dẫn đến "hành vi không xác định" tức là nó không còn là một chương trình hữu ích, dự đoán được. Trên thực tế, chương trình của bạn có thể bị sập, tính toán hoặc tạo ra kết quả không chính xác hoặc ngừng hoạt động ("deadlock"), tùy thuộc vào chính xác những gì chương trình của bạn đang thực thi khi nó bị gián đoạn để thực thi mã trình xử lý tín hiệu.

Một ứng dụng phổ biến của trình xử lý tín hiệu là đặt một cờ boolean mà thỉnh thoảng được kiểm tra (đọc) như một phần của quá trình chạy bình thường của chương trình. Ví dụ,
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
Mã trên có thể trông đúng trên giấy. Tuy nhiên, chúng ta cần cung cấp một gợi ý cho trình biên dịch và lõi CPU sẽ thực thi vòng lặp `main()`. Chúng ta cần ngăn chặn một tối ưu hóa của trình biên dịch: Biểu thức `! pleaseStop` dường như là một bất biến vòng lặp, tức là đúng mãi mãi, vì vậy có thể được đơn giản hóa thành `true`. Thứ hai, chúng ta cần đảm bảo rằng giá trị của `pleaseStop` không được lưu trữ tạm thời bằng một thanh ghi CPU và thay vào đó luôn được đọc từ và ghi vào bộ nhớ chính. Kiểu `sig_atomic_t` ngụ ý rằng tất cả các bit của biến có thể được đọc hoặc sửa đổi như một "hoạt động nguyên tử" - một hoạt động không thể gián đoạn. Không thể đọc một giá trị được tạo thành từ một số giá trị bit mới và giá trị bit cũ.

Bằng cách chỉ định `pleaseStop` với kiểu đúng `volatile sig_atomic_t`, chúng ta có thể viết mã di động nơi vòng lặp chính sẽ được thoát sau khi trình xử lý tín hiệu trả về. Kiểu `sig_atomic_t` có thể lớn như một `int` trên hầu hết các nền tảng hiện đại nhưng trên các hệ thống nhúng có thể nhỏ như một `char` và chỉ có thể biểu diễn giá trị từ (-127 đến 127).

```C
volatile sig_atomic_t pleaseStop;
```
Hai ví dụ về mô hình này có thể được tìm thấy trong "COMP", một máy tính 4 bit 1Hz dựa trên terminal (https://github.com/gto76/comp-cpp/blob/1bf9a77eaf8f57f7358a316e5bbada97f2dc8987/src/output.c#L121).
Hai cờ boolean được sử dụng. Một để đánh dấu việc giao hàng của `SIGINT` (CTRL-C), và tắt chương trình một cách nhẹ nhàng, và còn lại để đánh dấu tín hiệu `SIGWINCH` để phát hiện thay đổi kích thước terminal và vẽ lại toàn bộ màn hình.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait/_edit">
Quay lại: Forking, Phần 2: Fork, Exec, Wait
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Processes-Review-Questions">
Tiếp theo: Câu hỏi ôn tập về Quy trình
</a>
</div>