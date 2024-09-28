## Semaphore đếm là gì?

Semaphore đếm chứa một giá trị [không âm] và hỗ trợ hai thao tác "wait" và "post". **Post** tăng semaphore và trả về ngay lập tức. "**Wait**" sẽ _chờ_ nếu số lượng là 0. Nếu số lượng khác 0, lệnh gọi _wait_ sẽ giảm số lượng và trả về ngay lập tức.

Tại sao chúng được đặt tên là "wait" & "post"?

Một phép so sánh là số lượng bánh quy trong hộp bánh quy (hoặc đồng vàng trong rương kho báu). Trước khi lấy bánh quy, hãy gọi 'wait'. Nếu không còn bánh quy, thì `wait` sẽ không trả về: Nó sẽ `wait` cho đến khi một luồng khác tăng semaphore bằng cách gọi `post`.

Tóm lại, `post` tăng và trả về ngay lập tức trong khi `wait` sẽ chờ nếu số lượng là 0. Trước khi trả về, nó sẽ giảm số lượng, nghĩa là, có ít hơn một bánh quy.

## Làm cách nào để tạo một semaphore?

Trang này giới thiệu semaphore không tên. Thật không may, Mac OS X chưa hỗ trợ những thứ này.

Trước tiên, hãy quyết định xem giá trị ban đầu nên là 0 hay một giá trị khác (ví dụ: số lượng khoảng trống còn lại trong một mảng).
Không giống như mutex pthread, không có phím tắt nào để tạo semaphore - hãy sử dụng `sem_init`:
```C
#include <semaphore.h>

sem_t s;
int main() {
  sem_init(&s, 0, 10); // tối đa mười bánh quy // trả về -1 (=FAILED) trên OS X 
  sem_wait(&s); // Có thể làm điều này 10 lần mà không bị chặn
  sem_post(&s); // Thông báo rằng chúng ta đã hoàn thành (và có thêm một mục tài nguyên khả dụng; tăng số lượng)
  sem_destroy(&s); // giải phóng tài nguyên của semaphore
}
```

## Tôi có thể gọi `wait` và `post` từ các luồng khác nhau không?

Có! Không giống như mutex, việc tăng và giảm có thể từ các luồng khác nhau.
**Có thể có semaphore không toàn cục không? Sử dụng của chúng sẽ là gì? Semaphore không toàn cục có thể được sử dụng để đồng bộ hóa các luồng trong cùng một tiến trình, nhưng không nhất thiết phải được chia sẻ giữa các tiến trình. Chúng có thể hữu ích cho các tác vụ như quản lý tài nguyên trong một tiến trình duy nhất.**

## Tôi có thể sử dụng semaphore thay cho mutex không?

Có - mặc dù chi phí của semaphore lớn hơn. Để sử dụng semaphore:
* Khởi tạo semaphore với số lượng là một. **[Đây là giá trị ban đầu]**
* Thay thế `...lock` bằng `sem_wait`
* Thay thế `...unlock` bằng `sem_post`

**Tại sao chi phí của các hoạt động semaphore lại cao hơn? Mutex được thiết kế đặc biệt cho loại trừ lẫn nhau, trong khi semaphore là một cơ chế đồng bộ hóa tổng quát hơn. Mutex thường được triển khai hiệu quả hơn vì chúng chỉ cần theo dõi xem khóa có được giữ hay không, trong khi semaphore phải duy trì một bộ đếm.**

**Mutex là một semaphore _được khởi tạo_ luôn `wait` trước khi nó `post`.
Nếu ai đó làm điều ngược lại, họ sẽ rơi vào tình huống nào? Deadlock? Race condition? Nếu một semaphore được khởi tạo với số lượng lớn hơn 1 và `post` được gọi trước `wait`, thì nhiều luồng có thể vào đoạn mã quan trọng cùng một lúc, dẫn đến race condition.**

```C
sem_t s;
sem_init(&s, 0, 1);

sem_wait(&s);
// Đoạn mã quan trọng
sem_post(&s);
```

## Tôi có thể sử dụng `sem_post` bên trong trình xử lý tín hiệu không?
## Tôi có thể tăng số lượng của bộ đếm trong semaphore bên trong trình xử lý tín hiệu không?

Có! `sem_post` là một trong số ít các hàm có thể được sử dụng chính xác bên trong trình xử lý tín hiệu.
**_Điều này có nghĩa là chúng ta có thể giải phóng một luồng đang chờ, luồng này bây giờ có thể thực hiện tất cả các lệnh gọi mà chúng ta không được phép gọi bên trong chính trình xử lý tín hiệu (ví dụ: `printf`)._**

```C
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

sem_t s;

void handler(int signal)
{
    sem_post(&s); /* Giải phóng Kraken! */
}

void *singsong(void *param)
{
    sem_wait(&s); // xem giá trị của giá trị semaphore được sử dụng để khởi tạo nó
    printf("Tôi phải đợi cho đến khi tín hiệu của bạn giải phóng tôi!\n");
}

int main()
{
    int ok = sem_init(&s, 0, 0 /* Giá trị ban đầu là 0 */); 
    if (ok == -1) {
       perror("Không thể tạo semaphore không tên");
       return 1;
    }
    signal(SIGINT, handler); // Quá đơn giản! Xem ghi chú bên dưới

    pthread_t tid;
    pthread_create(&tid, NULL, singsong, NULL);
    pthread_exit(NULL); /* Tiến trình sẽ thoát khi không còn luồng nào nữa */
}
```
Lưu ý rằng các chương trình mạnh mẽ không sử dụng `signal()` trong một chương trình đa luồng ("Hiệu ứng của signal() trong một tiến trình đa luồng là không xác định." - trang hướng dẫn tín hiệu); một chương trình chính xác hơn sẽ cần sử dụng `sigaction`.

## Làm cách nào để tôi tìm hiểu thêm?

Đọc các trang hướng dẫn:
* [sem_init](http://man7.org/linux/man-pages/man3/sem_init.3.html)
* [sem_wait](http://man7.org/linux/man-pages/man3/sem_wait.3.html)
* [sem_post](http://man7.org/linux/man-pages/man3/sem_post.3.html)
* [sem_destroy](http://man7.org/linux/man-pages/man3/sem_destroy.3.html)

