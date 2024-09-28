## Làm cách nào để đợi N luồng đến một điểm nhất định trước khi tiếp tục bước tiếp theo?

Giả sử chúng ta muốn thực hiện một phép tính đa luồng có hai giai đoạn, nhưng chúng ta không muốn chuyển sang giai đoạn hai cho đến khi giai đoạn đầu tiên hoàn thành.

Chúng ta có thể sử dụng một phương thức đồng bộ hóa được gọi là **rào cản**. Khi một luồng đến rào cản, nó sẽ đợi tại rào cản cho đến khi tất cả các luồng đến rào cản và sau đó tất cả chúng sẽ tiếp tục cùng nhau.

Hãy coi nó giống như đi bộ đường dài với một số người bạn. Bạn đồng ý đợi nhau ở đỉnh mỗi ngọn đồi (và bạn ghi nhớ tinh thần có bao nhiêu người trong nhóm của bạn). Giả sử bạn là người đầu tiên lên đến đỉnh ngọn đồi đầu tiên. Bạn sẽ đợi ở đó trên đỉnh cho bạn bè của bạn. Từng người một, họ sẽ đến đỉnh, nhưng không ai sẽ tiếp tục cho đến khi người cuối cùng trong nhóm của bạn đến. Khi họ đến, tất cả các bạn sẽ tiếp tục.

Pthreads có một hàm `pthread_barrier_wait()` để triển khai điều này. Bạn sẽ cần khai báo một biến `pthread_barrier_t` và khởi tạo nó với `pthread_barrier_init()`. `pthread_barrier_init()` nhận số lượng luồng sẽ tham gia vào rào cản làm đối số. [Đây là một ví dụ.](https://github.com/angrave/SystemProgramming/wiki/Sample-program-using-pthread-barriers)


Bây giờ chúng ta hãy triển khai rào cản của riêng mình và sử dụng nó để giữ cho tất cả các luồng đồng bộ trong một phép tính lớn.

```C
double data[256][8192]

1 Các luồng thực hiện phép tính đầu tiên (sử dụng và thay đổi các giá trị trong dữ liệu)

2 Rào cản! Chờ tất cả các luồng hoàn thành phép tính đầu tiên trước khi tiếp tục

3 Các luồng thực hiện phép tính thứ hai (sử dụng và thay đổi các giá trị trong dữ liệu)
```

Hàm luồng có bốn phần chính-
```C
void *calc(void *arg) {
    /* Thực hiện phần của tôi trong phép tính đầu tiên */
    /* Tôi có phải là luồng cuối cùng kết thúc không? Nếu vậy, hãy đánh thức tất cả các luồng khác! */
    /* Nếu không, hãy đợi cho đến khi các luồng khác đã hoàn thành phần một */
    /* Thực hiện phần của tôi trong phép tính thứ hai */
}
```

Luồng chính của chúng ta sẽ tạo ra 16 luồng và chúng ta sẽ chia mỗi phép tính thành 16 phần riêng biệt. Mỗi luồng sẽ được cung cấp một giá trị duy nhất (0,1,2,..15), để nó có thể hoạt động trên khối riêng của mình.
Vì kiểu (void*) có thể chứa các số nguyên nhỏ, chúng ta sẽ chuyển giá trị của `i` bằng cách ép kiểu nó thành một con trỏ void.
```C
#define N (16)
double data[256][8192];
int main() {
    pthread_t ids[N];
    for (int i = 0; i < N; i++)  
        pthread_create(&ids[i], NULL, calc, (void *) i);
```
  
Lưu ý, chúng ta sẽ không bao giờ hủy tham chiếu giá trị con trỏ này như một vị trí bộ nhớ thực tế - chúng ta sẽ chỉ ép kiểu nó trực tiếp trở lại một số nguyên:
```C
void *calc(void *ptr) {
// Luồng 0 sẽ làm việc trên các hàng 0..15, luồng 1 trên các hàng 16..31
    int x, y, start = N * (int) ptr;
    int end = start + N; 
    for (x = start; x < end; x++) for (y = 0; y < 8192; y++) { /* thực hiện phép tính #1 */ }
```

Sau khi phép tính 1 hoàn thành, chúng ta cần đợi các luồng chậm hơn (trừ khi chúng ta là luồng cuối cùng!).
Vì vậy, hãy theo dõi số lượng luồng đã đến rào cản của chúng ta hay còn gọi là 'điểm kiểm tra':
```C
// Toàn cục:
int remain = N;


// Sau mã tính toán #1:
remain--; // Chúng tôi đã hoàn thành
if (remain == 0) { /*Tôi là người cuối cùng! - Đã đến lúc mọi người thức dậy! */ }
else {
    while (remain != 0) { /* spin spin spin*/ }
}
```
Tuy nhiên, mã trên có một race condition (hai luồng có thể cố gắng giảm `remain`) và vòng lặp là một vòng lặp bận. Chúng ta có thể làm tốt hơn! Hãy sử dụng biến điều kiện và sau đó chúng ta sẽ sử dụng hàm broadcast/signal để đánh thức các luồng đang ngủ.

Nhắc nhở, biến điều kiện giống như một ngôi nhà! Các luồng đến đó để ngủ (`pthread_cond_wait`). Bạn có thể chọn đánh thức một luồng (`pthread_cond_signal`) hoặc tất cả chúng (`pthread_cond_broadcast`). Nếu hiện tại không có luồng nào đang chờ thì hai lệnh gọi này không có tác dụng.

Phiên bản biến điều kiện thường rất giống với giải pháp không chính xác vòng lặp bận - như chúng ta sẽ hiển thị tiếp theo. Đầu tiên, hãy thêm một mutex và các biến toàn cục điều kiện và đừng quên khởi tạo chúng trong `main` ...

```C
//biến toàn cục
pthread_mutex_t m;
pthread_cond_t cv;

int main() {
    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&cv, NULL);
```

Chúng ta sẽ sử dụng mutex để đảm bảo rằng chỉ có một luồng sửa đổi `remain` tại một thời điểm.
Luồng đến cuối cùng cần đánh thức _tất cả_ các luồng đang ngủ - vì vậy chúng ta sẽ sử dụng `pthread_cond_broadcast(&cv)` chứ không phải `pthread_cond_signal`

```C
pthread_mutex_lock(&m);
remain--; 
if (remain == 0) { pthread_cond_broadcast(&cv); }
else {
    while (remain != 0) { pthread_cond_wait(&cv, &m); }
}
pthread_mutex_unlock(&m);
```
Khi một luồng vào `pthread_cond_wait`, nó sẽ giải phóng mutex và ngủ. Tại một số thời điểm trong tương lai, nó sẽ được đánh thức. Khi chúng ta đưa một luồng trở lại từ trạng thái ngủ, trước khi trả về, nó phải đợi cho đến khi nó có thể khóa mutex. Lưu ý rằng ngay cả khi một luồng đang ngủ thức dậy sớm, nó sẽ kiểm tra điều kiện vòng lặp while và nhập lại wait nếu cần.

**Rào cản trên không thể tái sử dụng** Có nghĩa là nếu chúng ta gắn nó vào bất kỳ vòng lặp tính toán cũ nào, rất có thể mã sẽ gặp phải điều kiện trong đó rào cản bị deadlock hoặc một luồng chạy nhanh hơn một lần lặp. Hãy suy nghĩ về cách bạn có thể làm cho rào cản trên có thể tái sử dụng, nghĩa là nếu nhiều luồng gọi `barrier_wait` trong một vòng lặp thì có thể đảm bảo rằng chúng đang ở trên cùng một lần lặp.


