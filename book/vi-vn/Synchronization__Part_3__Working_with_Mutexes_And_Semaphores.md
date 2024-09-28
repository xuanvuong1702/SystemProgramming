# Ngăn xếp an toàn cho luồng

## Thao tác nguyên tử là gì?

Để diễn giải Wikipedia,
> Một thao tác (hoặc tập hợp các thao tác) là nguyên tử hoặc không thể bị gián đoạn nếu nó xuất hiện đối với phần còn lại của hệ thống xảy ra ngay lập tức.
Nếu không có khóa, chỉ các lệnh CPU đơn giản ("đọc byte này từ bộ nhớ") là nguyên tử (không thể chia cắt). Trên một hệ thống CPU đơn, người ta có thể tạm thời tắt ngắt (vì vậy một chuỗi các thao tác không thể bị gián đoạn) nhưng trong thực tế, tính nguyên tử đạt được bằng cách sử dụng các nguyên thủy đồng bộ hóa, thường là khóa mutex.

Việc tăng một biến (`i++`) _không_ phải là nguyên tử vì nó yêu cầu ba bước riêng biệt: Sao chép mẫu bit từ bộ nhớ vào CPU; thực hiện phép tính bằng cách sử dụng các thanh ghi của CPU; sao chép mẫu bit trở lại bộ nhớ. Trong chuỗi tăng này, một luồng hoặc tiến trình khác vẫn có thể đọc giá trị cũ và các ghi khác vào cùng một bộ nhớ cũng sẽ bị ghi đè khi chuỗi tăng hoàn thành.

## Làm cách nào để sử dụng khóa mutex để làm cho cấu trúc dữ liệu của tôi an toàn cho luồng?

Lưu ý, đây chỉ là phần giới thiệu - việc viết các cấu trúc dữ liệu an toàn cho luồng hiệu suất cao cần một cuốn sách riêng! Dưới đây là một cấu trúc dữ liệu đơn giản (ngăn xếp) không an toàn cho luồng:
```C
// Một ngăn xếp có kích thước cố định đơn giản (phiên bản 1)
#define STACK_SIZE 20
int count;
double values[STACK_SIZE];

void push(double v) { 
    values[count++] = v; 
}

double pop() {
    return values[--count];
}

int is_empty() {
    return count == 0;
}
```
Phiên bản 1 của ngăn xếp không an toàn cho luồng vì nếu hai luồng gọi push hoặc pop cùng một lúc thì kết quả hoặc ngăn xếp có thể không nhất quán. Ví dụ: hãy tưởng tượng nếu hai luồng gọi pop cùng một lúc thì cả hai luồng có thể đọc cùng một giá trị, cả hai có thể đọc giá trị đếm ban đầu.

Để biến điều này thành một cấu trúc dữ liệu an toàn cho luồng, chúng ta cần xác định các _đoạn mã nguy hiểm_ của mã của chúng ta, tức là đoạn (các) đoạn nào của mã chỉ có thể có một luồng tại một thời điểm. Trong ví dụ trên, các hàm `push`, `pop` và `is_empty` truy cập cùng một biến (tức là bộ nhớ) và tất cả các đoạn mã nguy hiểm cho ngăn xếp.

Trong khi `push` (và `pop`) đang thực thi, cấu trúc dữ liệu ở trạng thái không nhất quán (ví dụ: số lượng có thể chưa được ghi vào, vì vậy có thể vẫn chứa giá trị ban đầu). Bằng cách bao bọc các phương thức này bằng mutex, chúng ta có thể đảm bảo rằng chỉ có một luồng tại một thời điểm có thể cập nhật (hoặc đọc) ngăn xếp.

Một 'giải pháp' ứng viên được hiển thị bên dưới. Nó có đúng không? Nếu không, nó sẽ thất bại như thế nào?
```C
// Nỗ lực tạo ngăn xếp an toàn cho luồng (phiên bản 2)
#define STACK_SIZE 20
int count;
double values[STACK_SIZE];

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void push(double v) { 
    pthread_mutex_lock(&m1);
    values[count++] = v;
    pthread_mutex_unlock(&m1);
}

double pop() {
    pthread_mutex_lock(&m2);
    double v = values[--count];
    pthread_mutex_unlock(&m2);

    return v;
}

int is_empty() {
    pthread_mutex_lock(&m1);
    return count == 0;
    pthread_mutex_unlock(&m1);
}

```
Mã trên ('phiên bản 2') chứa ít nhất một lỗi. Hãy dành một chút thời gian để xem bạn có thể tìm ra lỗi (các lỗi) và tìm ra hậu quả (các hậu quả). **Mã này có hai lỗi. Đầu tiên, việc sử dụng hai mutex khác nhau (`m1` và `m2`) cho `push` và `pop` không ngăn được race condition khi một luồng đang đẩy và một luồng khác đang pop cùng một lúc. Thứ hai, trong hàm `is_empty`, lệnh `pthread_mutex_unlock(&m1)` được đặt sau lệnh `return`, có nghĩa là mutex sẽ không bao giờ được mở khóa.**

Nếu ba luồng gọi `push()` cùng một lúc, khóa `m1` đảm bảo rằng chỉ có một luồng tại một thời điểm thao tác ngăn xếp (hai luồng sẽ cần đợi cho đến khi luồng đầu tiên hoàn thành (gọi unlock), sau đó luồng thứ hai sẽ được phép tiếp tục vào phần nguy hiểm và cuối cùng luồng thứ ba sẽ được phép tiếp tục sau khi luồng thứ hai đã hoàn thành).

Một đối số tương tự được áp dụng cho các lệnh gọi đồng thời (lệnh gọi cùng một lúc) đến `pop`. Tuy nhiên, phiên bản 2 không ngăn push và pop chạy cùng một lúc vì `push` và `pop` sử dụng hai khóa mutex khác nhau.

Cách khắc phục rất đơn giản trong trường hợp này - hãy sử dụng cùng một khóa mutex cho cả hai hàm push và pop.

Mã có lỗi thứ hai; `is_empty` trả về sau khi so sánh và sẽ không mở khóa mutex. Tuy nhiên, lỗi sẽ không được phát hiện ngay lập tức. Ví dụ: giả sử một luồng gọi `is_empty` và một luồng thứ hai sau đó gọi `push`. Luồng này sẽ dừng một cách bí ẩn. Sử dụng trình gỡ lỗi, bạn có thể phát hiện ra rằng luồng bị kẹt tại phương thức `lock()` bên trong phương thức `push` vì khóa chưa bao giờ được mở khóa bởi lệnh gọi `is_empty` trước đó. Do đó, một sơ suất trong một luồng đã dẫn đến các vấn đề muộn hơn nhiều trong một luồng tùy ý khác.

Một phiên bản tốt hơn được hiển thị bên dưới -
```C
// Nỗ lực tạo ngăn xếp an toàn cho luồng (phiên bản 3)
int count;
double values[count];
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void push(double v) { 
  pthread_mutex_lock(&m); 
  values[count++] = v;
  pthread_mutex_unlock(&m);
}
double pop() {
  pthread_mutex_lock(&m);
  double v = values[--count];
  pthread_mutex_unlock(&m);
  return v;
}
int is_empty() {
  pthread_mutex_lock(&m);
  int result = count == 0;
  pthread_mutex_unlock(&m);
  return result;
}
```
Phiên bản 3 an toàn cho luồng (chúng tôi đã đảm bảo loại trừ lẫn nhau cho tất cả các phần nguy hiểm) tuy nhiên có hai điểm cần lưu ý:
* `is_empty` an toàn cho luồng nhưng kết quả của nó có thể đã lỗi thời, tức là ngăn xếp có thể không còn trống vào thời điểm luồng nhận được kết quả!
* Không có biện pháp bảo vệ chống lại hiện tượng tràn dưới (lấy ra khỏi ngăn xếp trống) hoặc tràn trên (đẩy vào ngăn xếp đã đầy).

Điểm sau có thể được khắc phục bằng cách sử dụng semaphore đếm.

Triển khai giả định một ngăn xếp duy nhất. Phiên bản mục đích chung hơn có thể bao gồm mutex như một phần của cấu trúc bộ nhớ và sử dụng `pthread_mutex_init` để khởi tạo mutex. Ví dụ:

```C
// Hỗ trợ nhiều ngăn xếp (mỗi ngăn xếp có một mutex)
typedef struct stack {
  int count;
  pthread_mutex_t m; 
  double *values;
} stack_t;

stack_t* stack_create(int capacity) {
  stack_t *result = malloc(sizeof(stack_t));
  result->count = 0;
  result->values = malloc(sizeof(double) * capacity);
  pthread_mutex_init(&result->m, NULL);
  return result;
}
void stack_destroy(stack_t *s) {
  free(s->values);
  pthread_mutex_destroy(&s->m);
  free(s);
}
// Cảnh báo không có kiểm tra tràn dưới hoặc tràn trên!

void push(stack_t *s, double v) { 
  pthread_mutex_lock(&s->m); 
  s->values[(s->count)++] = v; 
  pthread_mutex_unlock(&s->m); 
}

double pop(stack_t *s) { 
  pthread_mutex_lock(&s->m); 
  double v = s->values[--(s->count)]; 
  pthread_mutex_unlock(&s->m); 
  return v;
}

int is_empty(stack_t *s) { 
  pthread_mutex_lock(&s->m); 
  int result = s->count == 0; 
  pthread_mutex_unlock(&s->m);
  return result;
}
```
Ví dụ sử dụng:
```C
int main() {
    stack_t *s1 = stack_create(10 /* Dung lượng tối đa*/);
    stack_t *s2 = stack_create(10);
    push(s1, 3.141);
    push(s2, pop(s1));
    stack_destroy(s2);
    stack_destroy(s1);
}
```
# Semaphore ngăn xếp

## Làm cách nào để buộc các luồng của tôi đợi nếu ngăn xếp trống hoặc đầy?

Sử dụng semaphore đếm! Sử dụng semaphore đếm để theo dõi số lượng khoảng trống còn lại và semaphore khác để theo dõi số lượng mục trong ngăn xếp. Chúng ta sẽ gọi hai semaphore này là 'sremain' và 'sitems'. Hãy nhớ rằng `sem_wait` sẽ đợi nếu số lượng semaphore đã giảm xuống 0 (bởi một luồng khác gọi `sem_post`).

```C
// Phác thảo #1

sem_t sitems;
sem_t sremain;
void stack_init(){
  sem_init(&sitems, 0, 0);
  sem_init(&sremain, 0, 10);
}


double pop() {
  // Chờ cho đến khi có ít nhất một mục
  sem_wait(&sitems);
  ...

void push(double v) {
  // Chờ cho đến khi có ít nhất một khoảng trống
  sem_wait(&sremain);
  ...
```
Phác thảo #2 đã triển khai `post` quá sớm. Một luồng khác đang chờ trong push có thể cố gắng ghi vào ngăn xếp đầy một cách sai lầm (và tương tự, một luồng đang chờ trong `pop()` được phép tiếp tục quá sớm).

```C
// Phác thảo #2 (Lỗi!)
double pop() {
  // Chờ cho đến khi có ít nhất một mục
  sem_wait(&sitems);
  sem_post(&sremain); // lỗi! đánh thức luồng pushing() quá sớm
  return values[--count];
}
void push(double v) {
  // Chờ cho đến khi có ít nhất một khoảng trống
  sem_wait(&sremain);
  sem_post(&sitems); // lỗi! đánh thức luồng popping() quá sớm
  values[count++] = v;
}
```

Phác thảo 3 triển khai logic semaphore chính xác nhưng bạn có thể phát hiện ra lỗi không?
```C
// Phác thảo #3 (Lỗi!)
double pop() {
  // Chờ cho đến khi có ít nhất một mục
  sem_wait(&sitems);
  double v= values[--count];
  sem_post(&sremain);
  return v;
}

void push(double v) {
  // Chờ cho đến khi có ít nhất một khoảng trống
  sem_wait(&sremain);
  values[count++] = v;
  sem_post(&sitems); 
}
```
Phác thảo 3 thực thi chính xác các điều kiện bộ đệm đầy và bộ đệm trống bằng cách sử dụng semaphore. Tuy nhiên, không có _loại trừ lẫn nhau_: Hai luồng có thể ở trong _phần quan trọng_ cùng một lúc, điều này sẽ làm hỏng cấu trúc dữ liệu (hoặc ít nhất dẫn đến mất dữ liệu). Cách khắc phục là bao bọc mutex xung quanh phần quan trọng:

```C
// Ngăn xếp đơn giản - xem ví dụ ở trên về cách chuyển đổi điều này thành nhiều ngăn xếp.
// Ngoài ra, một triển khai POSIX mạnh mẽ sẽ kiểm tra EINTR và mã lỗi của sem_wait.

// PTHREAD_MUTEX_INITIALIZER cho tĩnh (sử dụng pthread_mutex_init() cho bộ nhớ stack/heap)

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; 
int count = 0;
double values[10];
sem_t sitems, sremain;

void init() {
  sem_init(&sitems, 0, 0);
  sem_init(&sremain, 0, 10); // 10 khoảng trống
}

double pop() {
  // Chờ cho đến khi có ít nhất một mục
  sem_wait(&sitems);

  pthread_mutex_lock(&m); // PHẦN QUAN TRỌNG
  double v= values[--count];
  pthread_mutex_unlock(&m);

  sem_post(&sremain); // Này thế giới, có ít nhất một khoảng trống
  return v;
}

void push(double v) {
  // Chờ cho đến khi có ít nhất một khoảng trống
  sem_wait(&sremain);

  pthread_mutex_lock(&m); // PHẦN QUAN TRỌNG
  values[count++] = v;
  pthread_mutex_unlock(&m);

  sem_post(&sitems); // Này thế giới, có ít nhất một mục
}
// Lưu ý một giải pháp mạnh mẽ sẽ cần kiểm tra kết quả của sem_wait cho EINTR (sẽ nói thêm về điều này sau)
```


## Những lỗi Mutex phổ biến là gì?

* Khóa/mở khóa mutex sai (do lỗi đánh máy ngớ ngẩn).
* Không mở khóa mutex (do giả sử trả về sớm trong điều kiện lỗi).
* Rò rỉ tài nguyên (không gọi `pthread_mutex_destroy`).
* Sử dụng mutex chưa được khởi tạo (hoặc sử dụng mutex đã bị hủy).
* Khóa mutex hai lần trên một luồng (mà không mở khóa trước).
* Deadlock và Đảo ngược ưu tiên (chúng ta sẽ nói về những điều này sau).
* Giả sử `sem_wait`, `sem_trywait`, `sem_timedwait` đã thành công thay vì kiểm tra mã trả về cho `EINTR` hoặc `ETIMEDOUT`.

