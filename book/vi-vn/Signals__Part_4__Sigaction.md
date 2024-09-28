## Cách và lý do sử dụng `sigaction`?

Bạn nên sử dụng `sigaction` thay vì `signal` vì nó có ngữ nghĩa được xác định rõ hơn. `signal` trên các hệ điều hành khác nhau thực hiện những việc khác nhau, điều này **không tốt**. `sigaction` di động hơn và được xác định rõ hơn cho các luồng nếu cần.

Để thay đổi "cách xử lý tín hiệu" của một tiến trình - tức là điều gì xảy ra khi một tín hiệu được gửi đến tiến trình của bạn - hãy sử dụng `sigaction`

Bạn có thể sử dụng lệnh gọi hệ thống `sigaction` để đặt trình xử lý hiện tại cho một tín hiệu hoặc đọc trình xử lý tín hiệu hiện tại cho một tín hiệu cụ thể.

```C
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
```
Cấu trúc sigaction bao gồm hai hàm callback (chúng ta sẽ chỉ xem xét phiên bản 'handler'), một mặt nạ tín hiệu và một trường flags -
```C
struct sigaction {
    void     (*sa_handler)(int);
    void     (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
}; 
```
## Làm cách nào để chuyển đổi lệnh gọi `signal` thành lệnh gọi `sigaction` tương đương?

Giả sử bạn đã cài đặt trình xử lý tín hiệu cho tín hiệu báo thức,
```C
signal(SIGALRM, myhandler);
```

Mã `sigaction` tương đương là:
```C
struct sigaction sa; 
sa.sa_handler = myhandler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = 0; 
sigaction(SIGALRM, &sa, NULL)
```

Tuy nhiên, chúng ta thường cũng có thể đặt trường mask và flags. Mask là một mặt nạ tín hiệu tạm thời được sử dụng trong quá trình thực thi trình xử lý tín hiệu. Cờ SA_RESTART sẽ tự động khởi động lại một số (nhưng không phải tất cả) các lệnh gọi hệ thống mà nếu không sẽ trả về sớm (với lỗi EINTR). Điều sau có nghĩa là chúng ta có thể đơn giản hóa phần còn lại của mã phần nào vì vòng lặp khởi động lại có thể không còn cần thiết nữa.

```C
sigfillset(&sa.sa_mask);
sa.sa_flags = SA_RESTART; /* Khởi động lại các hàm nếu bị gián đoạn bởi trình xử lý */     
```

## Làm cách nào để sử dụng sigwait?

Sigwait có thể được sử dụng để đọc một tín hiệu đang chờ xử lý tại một thời điểm. `sigwait` được sử dụng để đợi tín hiệu đồng bộ, thay vì xử lý chúng trong một callback. Cách sử dụng sigwait điển hình trong một chương trình đa luồng được hiển thị bên dưới. Lưu ý rằng mặt nạ tín hiệu luồng được đặt trước (và sẽ được kế thừa bởi các luồng mới). Điều này ngăn các tín hiệu được _gửi_ nên chúng sẽ ở trạng thái chờ xử lý cho đến khi sigwait được gọi. Cũng lưu ý rằng cùng một biến sigset_t được sử dụng bởi sigwait - ngoại trừ thay vì đặt tập hợp các tín hiệu bị chặn, nó đang được sử dụng làm tập hợp các tín hiệu mà sigwait có thể bắt và trả về.

Một ưu điểm của việc viết một luồng xử lý tín hiệu tùy chỉnh (chẳng hạn như ví dụ bên dưới) thay vì hàm callback là bạn có thể sử dụng nhiều hàm thư viện C và hàm hệ thống hơn mà nếu không sẽ không thể sử dụng an toàn trong trình xử lý tín hiệu vì chúng không an toàn tín hiệu không đồng bộ.
 
Dựa trên `http://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_sigmask.html`
```C
static sigset_t   signal_mask;  /* tín hiệu để chặn         */

int main (int argc, char *argv[]) {
    pthread_t sig_thr_id;      /* ID luồng trình xử lý tín hiệu */
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask, SIGINT);
    sigaddset(&signal_mask, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);

    /* Các luồng mới sẽ kế thừa mặt nạ của luồng này */
    pthread_create(&sig_thr_id, NULL, signal_thread, NULL);

    /* MÃ ỨNG DỤNG */
    ...
}

void *signal_thread (void *arg) {
    int       sig_caught;    /* tín hiệu đã bắt       */

    /* Sử dụng cùng một mặt nạ như tập hợp các tín hiệu mà chúng tôi muốn biết! */
    sigwait(&signal_mask, &sig_caught);
    switch (sig_caught) {
    case SIGINT:     /* xử lý SIGINT  */
        ...
        break;
    case SIGTERM:    /* xử lý SIGTERM */
        ...
        break;
    default:         /* thông thường không nên xảy ra */
        fprintf(stderr, "\nTín hiệu không mong muốn %d\n", sig_caught);
        break;
    }
}
```

