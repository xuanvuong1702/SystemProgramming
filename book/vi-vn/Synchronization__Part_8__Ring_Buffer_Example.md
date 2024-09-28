## Bộ đệm vòng là gì?

Bộ đệm vòng là một cơ chế lưu trữ đơn giản, thường có kích thước cố định, trong đó bộ nhớ liền kề được coi như thể nó là hình tròn, và hai bộ đếm chỉ mục theo dõi điểm bắt đầu và kết thúc hiện tại của hàng đợi. Vì việc lập chỉ mục mảng không phải là hình tròn, nên các bộ đếm chỉ mục phải quay trở về 0 khi di chuyển qua phần cuối của mảng.
Khi dữ liệu được thêm vào (enqueue) vào đầu hàng đợi hoặc bị xóa (dequeue) khỏi đuôi của hàng đợi, các mục hiện tại trong bộ đệm tạo thành một đoàn tàu dường như quay vòng quanh đường ray
![RingBuffer](https://raw.githubusercontent.com/wiki/angrave/SystemProgramming/RingBuffer-Angrave2014-1024x768.png)
Một triển khai đơn giản (đơn luồng) được hiển thị bên dưới. Lưu ý enqueue và dequeue không bảo vệ chống tràn bộ nhớ dưới hoặc tràn bộ nhớ trên - có thể thêm một mục khi hàng đợi đã đầy và có thể xóa một mục khi hàng đợi trống. Ví dụ: nếu chúng ta thêm 20 số nguyên (1,2,3...) vào hàng đợi và không dequeue bất kỳ mục nào thì các giá trị `17,18,19,20` sẽ ghi đè `1,2,3,4`. Chúng ta sẽ không khắc phục sự cố này ngay bây giờ, thay vào đó khi chúng ta tạo phiên bản đa luồng, chúng ta sẽ đảm bảo các luồng đang enqueue và dequeue bị chặn trong khi bộ đệm vòng lần lượt đầy hoặc trống.

```C
void *buffer[16];
int in = 0, out = 0;

void enqueue(void *value) { /* Thêm một mục vào đầu hàng đợi */
  buffer[in] = value;
  in++; /* Tăng chỉ mục cho lần sau */
  if (in == 16) in = 0; /* Quay vòng! */
}

void *dequeue() { /* Xóa một mục khỏi cuối hàng đợi. */
  void *result = buffer[out];
  out++;
  if (out == 16) out = 0;
  return result;
}
```

## Những cạm bẫy khi triển khai Bộ đệm vòng là gì?

Rất dễ viết phương thức enqueue hoặc dequeue ở dạng rút gọn sau (N là dung lượng của bộ đệm, ví dụ: 16):
```C
void enqueue(void *value)
  b[ (in++) % N ] = value;
}
```
Phương thức này dường như hoạt động (vượt qua các bài kiểm tra đơn giản, v.v.) nhưng chứa một lỗi tinh vi. Với đủ thao tác enqueue (hơn hai tỷ một chút), giá trị int của `in` sẽ tràn và trở thành âm! Toán tử modulo (hoặc 'phần dư') `%` giữ nguyên dấu. Do đó, bạn có thể sẽ viết vào `b[-14]` chẳng hạn!

Một dạng rút gọn chính xác sử dụng mặt nạ bit với điều kiện N là 2^x (16,32,64,...)
```C
b[ (in++) & (N-1) ] = value;
```

Bộ đệm này vẫn chưa ngăn chặn tràn bộ nhớ dưới hoặc tràn bộ nhớ trên bộ đệm. Đối với điều đó, chúng ta sẽ chuyển sang nỗ lực đa luồng của mình, luồng này sẽ chặn một luồng cho đến khi có khoảng trống hoặc có ít nhất một mục để xóa.

## Kiểm tra tính chính xác của triển khai đa luồng (Ví dụ 1)

Mã sau đây là một triển khai không chính xác. Điều gì sẽ xảy ra? `enqueue` và/hoặc `dequeue` có bị chặn không? Loại trừ lẫn nhau có được thỏa mãn không? Bộ đệm có thể tràn bộ nhớ dưới không? Bộ đệm có thể tràn bộ nhớ trên không?
Để rõ ràng, `pthread_mutex` được rút ngắn thành `p_m` và chúng ta giả sử sem_wait không thể bị gián đoạn.

```C
#define N 16
void *b[N]
int in = 0, out = 0
p_m_t lock
sem_t s1,s2
void init() { 
    p_m_init(&lock, NULL)
    sem_init(&s1, 0, 16)
    sem_init(&s2, 0, 0)
}

enqueue(void *value) {
    p_m_lock(&lock)

    // Gợi ý: Chờ trong khi bằng không. Giảm và trả về
    sem_wait( &s1 ) 
 
    b[ (in++) & (N-1) ] = value

    // Gợi ý: Tăng. Sẽ đánh thức một luồng đang chờ 
    sem_post(&s1) 
    p_m_unlock(&lock)
}
void *dequeue(){
    p_m_lock(&lock)
    sem_wait(&s2)
    void *result = b[(out++) & (N-1) ]
    sem_post(&s2)
    p_m_unlock(&lock)
    return result
}
```
## Phân tích
Trước khi đọc tiếp, hãy xem bạn có thể tìm thấy bao nhiêu lỗi. Sau đó, xác định điều gì sẽ xảy ra nếu các luồng gọi các phương thức enqueue và dequeue.

* Phương thức enqueue đợi và đăng trên cùng một semaphore (s1) và tương tự với dequeue và (s2), tức là chúng ta giảm giá trị và sau đó tăng giá trị ngay lập tức, vì vậy vào cuối hàm, giá trị semaphore không thay đổi!
* Giá trị ban đầu của s1 là 16, vì vậy semaphore sẽ không bao giờ bị giảm xuống 0 - enqueue sẽ không bị chặn nếu bộ đệm vòng đầy - vì vậy có thể xảy ra tràn bộ nhớ trên.
* Giá trị ban đầu của s2 là 0, vì vậy các lệnh gọi dequeue sẽ luôn bị chặn và không bao giờ trả về!
* Thứ tự khóa mutex và sem_wait sẽ cần phải được hoán đổi (tuy nhiên ví dụ này bị hỏng đến mức lỗi này không có tác dụng!)
## Kiểm tra tính chính xác của triển khai đa luồng (Ví dụ 2)

Mã sau đây là một triển khai không chính xác. Điều gì sẽ xảy ra? `enqueue` và/hoặc `dequeue` có bị chặn không? Loại trừ lẫn nhau có được thỏa mãn không? Bộ đệm có thể tràn bộ nhớ dưới không? Bộ đệm có thể tràn bộ nhớ trên không?
Để rõ ràng, `pthread_mutex` được rút ngắn thành `p_m` và chúng ta giả sử sem_wait không thể bị gián đoạn.

```C
void *b[16]
int in = 0, out = 0
p_m_t lock
sem_t s1, s2
void init() {
    sem_init(&s1,0,16)
    sem_init(&s2,0,0)
}

enqueue(void *value){

 sem_wait(&s2)
 p_m_lock(&lock)

 b[ (in++) & (N-1) ] = value

 p_m_unlock(&lock)
 sem_post(&s1)
}

void *dequeue(){
  sem_wait(&s1)
  p_m_lock(&lock)
  void *result = b[(out++) & 15]
  p_m_unlock(&lock)
  sem_post(&s2)

  return result;
}
```

### Phân tích
* Giá trị ban đầu của s2 là 0. Do đó, enqueue sẽ bị chặn ở lần gọi đầu tiên đến sem_wait mặc dù bộ đệm trống!
* Giá trị ban đầu của s1 là 16. Do đó, dequeue sẽ không bị chặn ở lần gọi đầu tiên đến sem_wait mặc dù bộ đệm trống - oops Tràn bộ nhớ dưới! Phương thức dequeue sẽ trả về dữ liệu không hợp lệ.
* Mã không thỏa mãn Loại trừ lẫn nhau; hai luồng có thể sửa đổi `in` hoặc `out` cùng một lúc! Mã dường như sử dụng khóa mutex. Thật không may, khóa chưa bao giờ được khởi tạo với `pthread_mutex_init()` hoặc `PTHREAD_MUTEX_INITIALIZER` - vì vậy khóa có thể không hoạt động (`pthread_mutex_lock` có thể chỉ đơn giản là không làm gì)

## Triển khai chính xác bộ đệm vòng

Mã giả (`pthread_mutex` được rút ngắn thành `p_m`, v.v.) được hiển thị bên dưới.

Vì khóa mutex được lưu trữ trong bộ nhớ toàn cục (tĩnh) nên nó có thể được khởi tạo với  `PTHREAD_MUTEX_INITIALIZER`. Nếu chúng ta đã cấp phát không gian cho mutex trên heap, thì chúng ta sẽ sử dụng `pthread_mutex_init(ptr, NULL)`

```C
#include <pthread.h>
#include <semaphore.h>
// N phải là 2^i
#define N (16)

void *b[N]
int in = 0, out = 0
p_m_t lock = PTHREAD_MUTEX_INITIALIZER
sem_t countsem, spacesem

void init() {
  sem_init(&countsem, 0, 0)
  sem_init(&spacesem, 0, 16)
}
```

Phương thức enqueue được hiển thị bên dưới. Chú ý:
* Khóa chỉ được giữ trong đoạn mã găng (truy cập vào cấu trúc dữ liệu).
* Một triển khai hoàn chỉnh sẽ cần phải bảo vệ chống lại việc trả về sớm từ `sem_wait` do tín hiệu POSIX.

```C
enqueue(void *value){
 // đợi nếu không còn chỗ trống:
 sem_wait( &spacesem )

 p_m_lock(&lock)
 b[ (in++) & (N-1) ] = value
 p_m_unlock(&lock)

 // tăng bộ đếm số lượng các mục
 sem_post(&countsem)
}
```

Triển khai `dequeue` được hiển thị bên dưới. Lưu ý tính đối xứng của các lệnh gọi đồng bộ hóa đến `enqueue`. Trong cả hai trường hợp, các hàm đầu tiên đợi nếu số lượng khoảng trống hoặc số lượng mục là 0.

```C
void *dequeue(){
  // Chờ nếu không có mục nào trong bộ đệm
  sem_wait(&countsem)

  p_m_lock(&lock)
  void *result = b[(out++) & (N-1)]
  p_m_unlock(&lock)

  // Tăng bộ đếm số lượng khoảng trống
  sem_post(&spacesem)

  return result
}
```

## Suy nghĩ thêm
* Điều gì sẽ xảy ra nếu thứ tự của các lệnh gọi `pthread_mutex_unlock` và `sem_post` bị hoán đổi?
* Điều gì sẽ xảy ra nếu thứ tự của các lệnh gọi `sem_wait` và `pthread_mutex_lock` bị hoán đổi?

