# Giới thiệu về Biến điều kiện

## Khởi động

Hãy đặt tên cho các thuộc tính này!
* "Chỉ một tiến trình (/ luồng) có thể ở trong CS tại một thời điểm"
* "Nếu đang chờ, thì một tiến trình khác chỉ có thể vào CS một số lần hữu hạn"
* "Nếu không có tiến trình nào khác ở trong CS thì tiến trình có thể vào CS ngay lập tức"

Xem [[Đồng bộ hóa, Phần 4: Vấn đề đoạn mã quan trọng]] để biết câu trả lời.

## Biến điều kiện là gì? Làm thế nào để bạn sử dụng chúng? Thức tỉnh giả là gì?

* Biến điều kiện cho phép một tập hợp các luồng ngủ cho đến khi được đánh thức! Bạn có thể đánh thức một luồng hoặc tất cả các luồng đang ngủ. Nếu bạn chỉ đánh thức một luồng, hệ điều hành sẽ quyết định luồng nào sẽ đánh thức. Bạn không đánh thức trực tiếp các luồng mà thay vào đó bạn 'báo hiệu' biến điều kiện, sau đó sẽ đánh thức một (hoặc tất cả) các luồng đang ngủ bên trong biến điều kiện.

* Biến điều kiện được sử dụng với mutex và với vòng lặp (để kiểm tra điều kiện).

* Đôi khi, một luồng đang chờ có thể xuất hiện để thức dậy mà không có lý do (điều này được gọi là _thức tỉnh giả_)! Đây không phải là vấn đề vì bạn luôn sử dụng `wait` bên trong một vòng lặp kiểm tra một điều kiện phải đúng để tiếp tục.

* Các luồng đang ngủ bên trong một biến điều kiện được đánh thức bằng cách gọi `pthread_cond_broadcast` (đánh thức tất cả) hoặc `pthread_cond_signal` (đánh thức một). Lưu ý mặc dù tên hàm là vậy, nhưng điều này không liên quan gì đến các tín hiệu POSIX `signal`!

## `pthread_cond_wait` làm gì?

Lệnh gọi `pthread_cond_wait` thực hiện ba hành động:
1. mở khóa mutex
2. chờ (ngủ cho đến khi `pthread_cond_signal` được gọi trên cùng một biến điều kiện). Nó thực hiện 1 và 2 một cách nguyên tử.
3. Trước khi trả về, khóa mutex

## (Chủ đề nâng cao) Tại sao Biến điều kiện cũng cần một mutex?

Biến điều kiện cần một mutex vì ba lý do. Điều dễ hiểu nhất là nó ngăn thông báo đánh thức sớm (các hàm `signal` hoặc `broadcast`) bị 'mất'. Hãy tưởng tượng chuỗi sự kiện sau (thời gian chạy xuống trang) trong đó điều kiện được thỏa mãn _ngay trước_ khi `pthread_cond_wait` được gọi. Trong ví dụ này, tín hiệu đánh thức bị mất!

Luồng 1 | Luồng 2
-------------------------|---------
`while (answer < 42) {` | &nbsp;
&nbsp; | `answer++`
&nbsp; | `p_cond_signal(cv)`
`p_cond_wait(cv, m) ` | &nbsp;

Nếu cả hai luồng đều đã khóa một mutex, tín hiệu không thể được gửi cho đến _sau_ khi `pthread_cond_wait(cv, m)` được gọi (sau đó nội bộ mở khóa mutex)

Một lý do phổ biến thứ hai là việc cập nhật trạng thái chương trình (biến `answer`) thường yêu cầu loại trừ lẫn nhau - ví dụ: nhiều luồng có thể đang cập nhật giá trị của `answer`.

Lý do thứ ba và tinh tế là để đáp ứng các mối quan tâm về lập lịch thời gian thực mà chúng tôi chỉ nêu ra ở đây: Trong một ứng dụng quan trọng về thời gian, luồng đang chờ có _mức độ ưu tiên cao nhất_ nên được phép tiếp tục trước. Để đáp ứng yêu cầu này, mutex cũng phải được khóa trước khi gọi `pthread_cond_signal` hoặc `pthread_cond_broadcast`. Đối với những người tò mò, một cuộc thảo luận dài hơn và mang tính lịch sử là [ở đây](https://groups.google.com/forum/?hl=ky#!msg/comp.programming.threads/wEUgPq541v8/ZByyyS8acqMJ).

## Tại sao lại tồn tại thức tỉnh giả?

Vì hiệu suất. Trên các hệ thống đa CPU, có thể một race condition có thể khiến yêu cầu đánh thức (tín hiệu) không được chú ý. Kernel có thể không phát hiện ra lệnh gọi đánh thức bị mất này nhưng có thể phát hiện khi nào nó có thể xảy ra. Để tránh tín hiệu bị mất tiềm ẩn, luồng được đánh thức để mã chương trình có thể kiểm tra lại điều kiện.

## Ví dụ

Biến điều kiện _luôn_ được sử dụng với khóa mutex.

Trước khi gọi _wait_, khóa mutex phải được khóa và _wait_ phải được bao bọc bằng một vòng lặp.
```C
pthread_cond_t cv;
pthread_mutex_t m;
int count;

// Khởi tạo
pthread_cond_init(&cv, NULL);
pthread_mutex_init(&m, NULL);
count = 0;

pthread_mutex_lock(&m);
while (count < 10) {
    pthread_cond_wait(&cv, &m); 
    /* Hãy nhớ rằng cond_wait mở khóa mutex trước khi chặn (chờ đợi)! */
    /* Sau khi mở khóa, các luồng khác có thể yêu cầu mutex. */
    /* Khi luồng này sau đó được đánh thức, nó sẽ */
    /* khóa lại mutex trước khi trả về */
}
pthread_mutex_unlock(&m);

// sau đó dọn dẹp bằng pthread_cond_destroy(&cv); và mutex_destroy 


// Trong một luồng khác, tăng số lượng:
while (1) {
    pthread_mutex_lock(&m);
    count++;
    pthread_cond_signal(&cv);
    /* Mặc dù luồng kia được đánh thức nhưng nó không thể trả về */
    /* từ pthread_cond_wait cho đến khi chúng ta đã mở khóa mutex. Điều này là */
    /* một điều tốt! Trên thực tế, thông thường cách tốt nhất là gọi */
    /* cond_signal hoặc cond_broadcast trước khi mở khóa mutex */
    pthread_mutex_unlock(&m);
}
```
# Triển khai Semaphore đếm

* Chúng ta có thể triển khai semaphore đếm bằng cách sử dụng biến điều kiện.
* Mỗi semaphore cần một số lượng, một biến điều kiện và một mutex
```C
typedef struct sem_t {
    int count; 
    pthread_mutex_t m;
    pthread_condition_t cv;
} sem_t;
```

Triển khai `sem_init` để khởi tạo mutex và biến điều kiện
```C
int sem_init(sem_t *s, int pshared, int value) {
    if (pshared) { errno = ENOSYS /* 'Chưa được triển khai'*/; return -1; }

    s->count = value;
    pthread_mutex_init(&s->m, NULL);
    pthread_cond_init(&s->cv, NULL);
    return 0;
}
```

Triển khai `sem_post` của chúng ta cần tăng số lượng.
Chúng ta cũng sẽ đánh thức bất kỳ luồng nào đang ngủ bên trong biến điều kiện.
Lưu ý rằng chúng ta khóa và mở khóa mutex để chỉ một luồng có thể ở bên trong phần quan trọng tại một thời điểm.
```C
sem_post(sem_t *s) {
    pthread_mutex_lock(&s->m);
    s->count++;
    pthread_cond_signal(&s->cv); /* Xem ghi chú */
    /* Một luồng được đánh thức phải có được khóa, vì vậy nó cũng sẽ phải đợi cho đến khi chúng ta gọi unlock */

    pthread_mutex_unlock(&s->m);
}
```
Triển khai `sem_wait` của chúng ta có thể cần phải ngủ nếu số lượng semaphore là 0.
Giống như `sem_post`, chúng ta bao bọc phần quan trọng bằng cách sử dụng khóa (vì vậy chỉ có một luồng có thể thực thi mã của chúng ta tại một thời điểm). Lưu ý rằng nếu luồng cần đợi thì mutex sẽ được mở khóa, cho phép một luồng khác vào `sem_post` và đánh thức chúng ta khỏi giấc ngủ!

Lưu ý rằng ngay cả khi một luồng được đánh thức, trước khi nó trả về từ `pthread_cond_wait`, nó phải lấy lại khóa, vì vậy nó sẽ phải đợi thêm một chút (ví dụ: cho đến khi sem_post kết thúc).
```C
sem_wait(sem_t *s) {
    pthread_mutex_lock(&s->m);
    while (s->count == 0) {
        pthread_cond_wait(&s->cv, &s->m); /*mở khóa mutex, chờ, khóa lại mutex */
    }
    s->count--;
    pthread_mutex_unlock(&s->m);
}
```
**Chờ `sem_post` tiếp tục gọi `pthread_cond_signal` sẽ không phá vỡ sem_wait?**
Trả lời: Không! Chúng ta không thể vượt qua vòng lặp cho đến khi số lượng khác 0. Trong thực tế, điều này có nghĩa là `sem_post` sẽ gọi `pthread_cond_signal` một cách không cần thiết ngay cả khi không có luồng nào đang chờ. Một triển khai hiệu quả hơn sẽ chỉ gọi `pthread_cond_signal` khi cần thiết, tức là
```C
    /* Chúng ta có tăng từ 0 lên 1 không - thời gian để báo hiệu một luồng đang ngủ bên trong sem_post */
    if (s->count == 1) /* Đánh thức một luồng đang chờ! */
        pthread_cond_signal(&s->cv);
```

## Các cân nhắc khác về semaphore

* Việc triển khai semaphore thực bao gồm hàng đợi và các mối quan tâm về lập lịch để đảm bảo tính công bằng và ưu tiên, ví dụ: đánh thức luồng ngủ lâu nhất có mức độ ưu tiên cao nhất.
* Ngoài ra, việc sử dụng nâng cao `sem_init` cho phép semaphore được chia sẻ giữa các tiến trình. Triển khai của chúng ta chỉ hoạt động cho các luồng bên trong cùng một tiến trình.




