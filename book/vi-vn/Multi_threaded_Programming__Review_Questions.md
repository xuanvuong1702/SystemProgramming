> Cảnh báo - số câu hỏi có thể thay đổi

## Q1
Đoạn mã sau đây có **an toàn cho luồng** không? Thiết kế lại đoạn mã sau để **an toàn cho luồng**. Gợi ý: **Mutex** là không cần thiết nếu bộ nhớ tin nhắn là duy nhất cho mỗi lần gọi.

````C
static char message[20];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *format(int v) {
  pthread_mutex_lock(&mutex);
  sprintf(message, ":%d:" ,v);
  pthread_mutex_unlock(&mutex);
  return message;
}
````
## Q2
Cái nào sau đây **không** khiến một **tiến trình** thoát?
* Trả về từ hàm bắt đầu của **pthread** trong luồng đang chạy cuối cùng.
* Luồng gốc trả về từ hàm `main`.
* Bất kỳ luồng nào gây ra lỗi phân đoạn.
* Bất kỳ luồng nào gọi `exit`.
* Gọi `pthread_exit` trong luồng chính với các luồng khác vẫn đang chạy.


## Q3
Viết một biểu thức toán học cho số lượng ký tự "W" sẽ được in bởi chương trình sau. Giả sử a, b, c, d là các số nguyên dương nhỏ. Câu trả lời của bạn có thể sử dụng hàm 'min' trả về đối số có giá trị thấp nhất của nó.

````C
unsigned int a=...,b=...,c=...,d=...;

void* func(void* ptr) {
  char m = * (char*)ptr;
  if(m == 'P') sem_post(s);
  if(m == 'W') sem_wait(s);
  putchar(m);
  return NULL;
}

int main(int argv, char** argc) {
  sem_init(s,0, a);
  while(b--) pthread_create(&tid, NULL, func, "W"); 
  while(c--) pthread_create(&tid, NULL, func, "P"); 
  while(d--) pthread_create(&tid, NULL, func, "W"); 
  pthread_exit(NULL); 
  /*Tiến trình sẽ kết thúc khi tất cả các luồng đã thoát*/
}
````

## Q4
Hoàn thành đoạn mã sau. Đoạn mã sau được cho là in xen kẽ `A` và `B`. Nó đại diện cho hai luồng luân phiên thực thi.  Thêm các lệnh gọi **biến điều kiện** vào `func` để luồng đang chờ không cần phải liên tục kiểm tra biến `turn`. Hỏi: `pthread_cond_broadcast` có cần thiết hay `pthread_cond_signal` là đủ?
````C
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* turn;

void* func(void* mesg) {
  while(1) {
// Thêm khóa mutex và các lệnh gọi biến điều kiện ...

    while(turn == mesg) { 
        /* thăm dò lại ... Thay đổi tôi - Vòng lặp bận này làm hao pin CPU! */ 
    }

    /* Làm việc trên luồng này */
    puts( (char*) mesg);
    turn = mesg;
    
  }
  return 0;
}

int main(int argc, char** argv){
  pthread_t tid1;
  pthread_create(&tid1, NULL, func, "A");
  func("B"); // không cần tạo một luồng khác - chỉ cần sử dụng luồng chính
  return 0;
}
````

## Q5
Xác định các **vùng tranh chấp** trong mã đã cho. Thêm **khóa mutex** để làm cho mã an toàn cho luồng. Thêm các lệnh gọi biến điều kiện để `total` không bao giờ trở thành âm hoặc trên 1000. Thay vào đó, lệnh gọi sẽ bị **chặn** cho đến khi an toàn để tiếp tục. Giải thích tại sao `pthread_cond_broadcast` là cần thiết.
````C
int total;
void add(int value) {
 if(value < 1) return;
 total += value;
}
void sub(int value) {
 if(value < 1) return;
 total -= value;
}
````

## Q6
Cấu trúc dữ liệu không an toàn cho luồng có các phương thức `size()` `enq` và `deq`. Sử dụng biến điều kiện và khóa mutex để hoàn thành các phiên bản **chặn**, an toàn cho luồng.
````C
void enqueue(void* data) {
  // nên chặn nếu size() sẽ lớn hơn 256
  enq(data);
}
void* dequeue() {
  // nên chặn nếu size() là 0
  return deq();
}
````

## Q7
Khởi nghiệp của bạn cung cấp tính năng lập kế hoạch đường dẫn bằng cách sử dụng thông tin giao thông mới nhất. Thực tập sinh được trả lương cao của bạn đã tạo ra một cấu trúc dữ liệu không an toàn cho luồng với hai hàm: `shortest` (sử dụng nhưng không sửa đổi đồ thị) và `set_edge` (sửa đổi đồ thị).
````C
graph_t* create_graph(char* filename); // được gọi một lần

// trả về một đối tượng heap mới là đường dẫn ngắn nhất từ ​​đỉnh i đến j
path_t* shortest(graph_t* graph, int i, int j); 

// cập nhật cạnh từ đỉnh i đến j
void set_edge(graph_t* graph, int i, int j, double time); 
  
````
Để có hiệu suất, nhiều luồng phải có thể gọi `shortest` cùng một lúc nhưng đồ thị chỉ có thể được sửa đổi bởi một luồng khi không có luồng nào khác đang thực thi bên trong `shortest` hoặc `set_edge`.
 
Sử dụng khóa mutex và biến điều kiện để triển khai giải pháp **người đọc-người viết**. Một nỗ lực chưa hoàn thành được hiển thị bên dưới. Mặc dù nỗ lực này an toàn cho luồng (do đó đủ cho ngày demo!), nhưng nó không cho phép nhiều luồng tính toán đường dẫn `ngắn nhất` cùng một lúc và sẽ không có đủ **thông lượng**.
````C
path_t* shortest_safe(graph_t* graph, int i, int j) {
  pthread_mutex_lock(&m);
  path_t* path = shortest(graph, i, j);
  pthread_mutex_unlock(&m);
  return path;
}
void set_edge_safe(graph_t* graph, int i, int j, double dist) {
  pthread_mutex_lock(&m);
  set_edge(graph, i, j, dist);
  pthread_mutex_unlock(&m);
}
````



