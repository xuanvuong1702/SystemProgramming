## Câu hỏi về tín hiệu

## Hãy đưa ra tên của hai tín hiệu thường được tạo ra bởi kernel

Hai tín hiệu thường được tạo ra bởi kernel là:

* **SIGSEGV:** Tín hiệu này được tạo ra khi một chương trình cố gắng truy cập vào một vùng bộ nhớ không hợp lệ (lỗi segmentation fault).
* **SIGCHLD:** Tín hiệu này được tạo ra khi một tiến trình con kết thúc.


## Hãy đưa ra tên của một tín hiệu không thể bị bắt bởi một signal handler

Một tín hiệu không thể bị bắt bởi một signal handler là **SIGKILL**. Tín hiệu này được sử dụng để chấm dứt một tiến trình một cách bắt buộc, không thể bị chặn hay bỏ qua.

## Tại sao việc gọi bất kỳ hàm nào (một thứ không an toàn với trình xử lý tín hiệu) trong một signal handler là không an toàn?

Việc gọi các hàm không an toàn với trình xử lý tín hiệu trong một signal handler là không an toàn bởi vì:

* **Signal handler có thể ngắt bất cứ lúc nào:** Signal handler có thể được gọi bất cứ lúc nào trong quá trình thực thi chương trình, ngay cả khi chương trình đang ở giữa một thao tác quan trọng.
* **Hàm không an toàn có thể sử dụng tài nguyên được chia sẻ:** Nhiều hàm thư viện C sử dụng các tài nguyên được chia sẻ, chẳng hạn như bộ nhớ heap hoặc các biến toàn cục. Nếu một signal handler gọi một hàm không an toàn trong khi chương trình chính đang sửa đổi cùng một tài nguyên, điều này có thể dẫn đến race condition và lỗi không mong muốn.
* **Hàm không an toàn có thể không được thiết kế để xử lý việc bị ngắt:** Một số hàm không được thiết kế để xử lý việc bị ngắt bởi một signal handler. Việc gọi các hàm này từ một signal handler có thể dẫn đến hành vi không xác định.

## Câu hỏi về mã

Viết một đoạn mã ngắn sử dụng `SIGACTION` và `SIGNALSET` để tạo một `SIGALRM` handler.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigalrm_handler(int signum) {
  printf("Tín hiệu SIGALRM đã được bắt!\n");
}

int main() {
  struct sigaction sa;

  // Khởi tạo cấu trúc sigaction.
  sa.sa_handler = sigalrm_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  // Cài đặt trình xử lý SIGALRM bằng sigaction.
  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Đặt báo thức cho 3 giây.
  alarm(3);

  // Chờ tín hiệu báo thức.
  pause();

  return 0;
}
```

**Giải thích mã:**

* Đầu tiên, ta khai báo một hàm `sigalrm_handler` để xử lý tín hiệu `SIGALRM`. Hàm này đơn giản in ra một thông báo.
* Trong hàm `main`, ta khai báo một cấu trúc `sigaction` và gán hàm xử lý `sigalrm_handler` cho `sa.sa_handler`.
* Ta sử dụng `sigemptyset` để khởi tạo mặt nạ tín hiệu `sa.sa_mask` thành tập rỗng. Điều này có nghĩa là không có tín hiệu nào bị chặn trong khi xử lý tín hiệu `SIGALRM`.
* Ta đặt `sa.sa_flags` thành 0, không sử dụng cờ đặc biệt nào.
* Sau đó, ta sử dụng `sigaction` để cài đặt trình xử lý `SIGALRM`.
* Ta sử dụng `alarm(3)` để đặt báo thức cho 3 giây.
* Cuối cùng, ta sử dụng `pause()` để chờ tín hiệu báo thức. Khi tín hiệu báo thức được gửi đến, signal handler sẽ được gọi và in ra thông báo.

**Lưu ý:**

* `perror` được sử dụng để in ra thông báo lỗi nếu `sigaction` thất bại.
* `exit(1)` được sử dụng để thoát chương trình với mã lỗi 1 nếu `sigaction` thất bại.



