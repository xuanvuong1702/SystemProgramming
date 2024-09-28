# Thêm các hàm pthread

## Làm cách nào để tạo một pthread?
Xem [Pthreads Phần 1](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction) giới thiệu `pthread_create` và `pthread_join`

## Nếu tôi gọi `pthread_create` hai lần, tiến trình của tôi có bao nhiêu ngăn xếp?
Tiến trình của bạn sẽ chứa ba ngăn xếp - một cho mỗi luồng. Luồng đầu tiên được tạo khi tiến trình bắt đầu và bạn đã tạo thêm hai luồng nữa. Trên thực tế, có thể có nhiều ngăn xếp hơn thế này, nhưng hãy bỏ qua sự phức tạp đó ngay bây giờ. Ý tưởng quan trọng là mỗi luồng yêu cầu một ngăn xếp vì ngăn xếp chứa các biến tự động và thanh ghi PC CPU cũ, để nó có thể quay lại thực thi hàm gọi sau khi hàm kết thúc.

## Sự khác biệt giữa một tiến trình đầy đủ và một luồng là gì?
Ngoài ra, không giống như các tiến trình, các luồng trong cùng một tiến trình có thể chia sẻ cùng một bộ nhớ toàn cục (phân đoạn dữ liệu và heap).

## `pthread_cancel` làm gì?
Dừng một luồng. Lưu ý rằng luồng có thể không thực sự bị dừng ngay lập tức. Ví dụ: nó có thể bị chấm dứt khi luồng thực hiện lệnh gọi hệ điều hành (ví dụ: `write`).

Trong thực tế, `pthread_cancel` hiếm khi được sử dụng vì nó không cho luồng cơ hội dọn dẹp sau khi tự kết thúc (ví dụ: nó có thể đã mở một số tệp).
Một cách triển khai thay thế là sử dụng một biến boolean (int) có giá trị được sử dụng để thông báo cho các luồng khác rằng chúng nên kết thúc và dọn dẹp.

## Sự khác biệt giữa `exit` và `pthread_exit` là gì?
`exit(42)` thoát toàn bộ tiến trình và đặt giá trị thoát của tiến trình. Điều này tương đương với `return 42` trong hàm `main`. Tất cả các luồng bên trong tiến trình đều bị dừng.

`pthread_exit(void *)` chỉ dừng luồng gọi, tức là luồng không bao giờ trả về sau khi gọi `pthread_exit`. Thư viện pthread sẽ tự động kết thúc tiến trình nếu không có luồng nào khác đang chạy. `pthread_exit(...)` tương đương với việc trả về từ hàm của luồng; cả hai đều kết thúc luồng và cũng đặt giá trị trả về (con trỏ void *) cho luồng.

Gọi `pthread_exit` trong luồng `main` là một cách phổ biến để các chương trình đơn giản đảm bảo rằng tất cả các luồng đều kết thúc. Ví dụ: trong chương trình sau, các luồng `myfunc` có thể sẽ không có thời gian để bắt đầu.
```C
int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, myfunc, "Jabberwocky");
  pthread_create(&tid2, NULL, myfunc, "Vorpel");
  exit(42); //hoặc return 42;

  // Không có mã nào được chạy sau exit
}
```
Hai chương trình tiếp theo sẽ đợi các luồng mới kết thúc -
```C
int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, myfunc, "Jabberwocky");
  pthread_create(&tid2, NULL, myfunc, "Vorpel");
  pthread_exit(NULL); 

  // Không có mã nào được chạy sau pthread_exit
  // Tuy nhiên, tiến trình sẽ tiếp tục tồn tại cho đến khi cả hai luồng đã kết thúc
}
```
Ngoài ra, chúng ta tham gia vào mỗi luồng (tức là đợi nó kết thúc) trước khi chúng ta trả về từ main (hoặc gọi exit).
```C
int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, myfunc, "Jabberwocky");
  pthread_create(&tid2, NULL, myfunc, "Vorpel");
  // đợi cả hai luồng kết thúc :
  void* result;
  pthread_join(tid1, &result);
  pthread_join(tid2, &result); 
  return 42;
}
```
Lưu ý rằng phiên bản `pthread_exit` tạo ra các luồng zombie, tuy nhiên đây không phải là các tiến trình chạy lâu, vì vậy chúng ta không quan tâm.

## Làm cách nào để chấm dứt một luồng?
* Trả về từ hàm luồng
* Gọi `pthread_exit`
* Hủy luồng bằng `pthread_cancel`
* Chấm dứt tiến trình (ví dụ: `SIGTERM`); `exit()`; trả về từ `main`

## Mục đích của `pthread_join` là gì?
* Chờ một luồng kết thúc
* Dọn dẹp tài nguyên luồng
* Lấy giá trị trả về của luồng

## Điều gì xảy ra nếu bạn không gọi `pthread_join`?
Các luồng đã kết thúc sẽ tiếp tục tiêu thụ tài nguyên. Cuối cùng, nếu tạo đủ luồng, `pthread_create` sẽ thất bại.
Trong thực tế, đây chỉ là vấn đề đối với các tiến trình chạy lâu nhưng không phải là vấn đề đối với các tiến trình đơn giản, tồn tại trong thời gian ngắn vì tất cả tài nguyên luồng sẽ tự động được giải phóng khi tiến trình thoát.

## Tôi nên sử dụng `pthread_exit` hay `pthread_join`?
Cả `pthread_exit` và `pthread_join` sẽ cho phép các luồng khác tự kết thúc (ngay cả khi được gọi trong luồng chính). Tuy nhiên, chỉ `pthread_join` sẽ trả về cho bạn khi luồng được chỉ định kết thúc. `pthread_exit` không đợi và sẽ ngay lập tức kết thúc luồng của bạn và không cho bạn cơ hội tiếp tục thực thi.

## Bạn có thể chuyển con trỏ đến các biến ngăn xếp từ luồng này sang luồng khác không?
Có thể. Tuy nhiên, bạn cần phải rất cẩn thận về thời gian tồn tại của các biến ngăn xếp.
```C
pthread_t start_threads() {
  int start = 42;
  pthread_t tid;
  pthread_create(&tid, 0, myfunc, &start); // LỖI!
  return tid;
}
```
Mã trên không hợp lệ vì hàm `start_threads` có thể sẽ trả về trước khi `myfunc` bắt đầu. Hàm truyền địa chỉ của `start`, tuy nhiên khi `myfunc` được thực thi, `start` không còn trong phạm vi và địa chỉ của nó sẽ được sử dụng lại cho một biến khác.

Mã sau hợp lệ vì thời gian tồn tại của biến ngăn xếp dài hơn luồng nền.

```C
void start_threads() {
  int start = 42;
  void *result;
  pthread_t tid;
  pthread_create(&tid, 0, myfunc, &start); // OK - start sẽ hợp lệ!
  pthread_join(tid, &result);
}
```

# Giới thiệu về Race Condition

## Làm cách nào để tạo mười luồng với các giá trị bắt đầu khác nhau?
Mã sau được cho là để bắt đầu mười luồng với các giá trị 0,1,2,3,...9
Tuy nhiên, khi chạy, nó in ra `1 7 8 8 8 8 8 8 8 10`! Bạn có thể thấy tại sao?
```C
#include <pthread.h>
void* myfunc(void* ptr) {
    int i = *((int *) ptr);
    printf("%d ", i);
    return NULL;
}

int main() {
    // Mỗi luồng nhận được một giá trị khác nhau của i để xử lý
    int i;
    pthread_t tid;
    for (i = 0; i < 10; i++) {
        pthread_create(&tid, NULL, myfunc, &i); // LỖI
    }
    pthread_exit(NULL);
}
```
Mã trên bị `race condition` - giá trị của i đang thay đổi. Các luồng mới bắt đầu muộn hơn (trong đầu ra ví dụ, luồng cuối cùng bắt đầu sau khi vòng lặp kết thúc).

Để khắc phục race condition này, chúng ta sẽ cung cấp cho mỗi luồng một con trỏ đến vùng dữ liệu riêng của nó. Ví dụ: đối với mỗi luồng, chúng ta có thể muốn lưu trữ id, giá trị bắt đầu và giá trị đầu ra:
```C
struct T {
  pthread_t id;
  int start;
  char result[100];
};
```
Chúng có thể được lưu trữ trong một mảng - 
```C
struct T *info = calloc(10 , sizeof(struct T)); // dành đủ byte cho mười cấu trúc T
```
Và mỗi phần tử mảng được truyền cho mỗi luồng - 
```C
pthread_create(&info[i].id, NULL, func, &info[i]);
```

## Tại sao một số hàm, ví dụ: `asctime`, `getenv`, `strtok`, `strerror` không an toàn cho luồng? 
Để trả lời điều này, hãy xem xét một hàm đơn giản cũng không 'an toàn cho luồng':
```C
char *to_message(int num) {
    static char result[256];
    if (num < 10) 
        sprintf(result, "%d : blah blah", num);
    else 
        strcpy(result, "Unknown");
    return result;
}
```
Trong mã trên, bộ đệm kết quả được lưu trữ trong bộ nhớ toàn cục. Điều này là tốt - chúng ta sẽ không muốn trả về một con trỏ đến một địa chỉ không hợp lệ trên ngăn xếp, nhưng chỉ có một bộ đệm kết quả trong toàn bộ bộ nhớ. Nếu hai luồng cùng sử dụng nó cùng một lúc thì luồng này sẽ làm hỏng luồng kia:


Thời gian | Luồng 1 | Luồng 2 | Bình luận 
-----|----------| --------|------
 1   | `to_m(5)` |         |
 2   |          | `to_m(99)` | Bây giờ cả hai luồng sẽ thấy "Unknown" được lưu trữ trong bộ đệm kết quả


## Biến điều kiện, semaphore, mutex là gì?
Đây là những khóa đồng bộ hóa được sử dụng để ngăn chặn race condition và đảm bảo đồng bộ hóa thích hợp giữa các luồng chạy trong cùng một chương trình. Ngoài ra, những khóa này về mặt khái niệm giống hệt với các nguyên thủy được sử dụng bên trong kernel.


## Có bất kỳ lợi thế nào khi sử dụng luồng hơn so với fork tiến trình không?
Có! Việc chia sẻ thông tin giữa các luồng rất dễ dàng vì các luồng (của cùng một tiến trình) nằm trong cùng một không gian bộ nhớ ảo.
Ngoài ra, việc tạo một luồng nhanh hơn đáng kể so với việc tạo (forking) một tiến trình.

## Có bất kỳ nhược điểm nào khi sử dụng luồng hơn so với fork tiến trình không?
Có! Không có sự cô lập! Vì các luồng nằm trong cùng một tiến trình, một luồng có quyền truy cập vào cùng một bộ nhớ ảo như các luồng khác. Một luồng duy nhất có thể chấm dứt toàn bộ tiến trình (ví dụ: bằng cách cố gắng đọc địa chỉ 0).

## Bạn có thể fork một tiến trình với nhiều luồng không?
Có thể! Tuy nhiên, tiến trình con chỉ có một luồng duy nhất (là bản sao của luồng đã gọi `fork`). Chúng ta có thể thấy điều này như một ví dụ đơn giản, trong đó các luồng nền không bao giờ in ra thông báo thứ hai trong tiến trình con.

```C
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pid_t child = -2;

void *sleepnprint(void *arg) {
  printf("%d:%s starting up...\n", getpid(), (char *) arg);

  while (child == -2) {sleep(1);} /* Sau này chúng ta sẽ sử dụng các biến điều kiện */

  printf("%d:%s finishing...\n",getpid(), (char*)arg);

  return NULL;  
}
int main() {
  pthread_t tid1, tid2;
  pthread_create(&tid1,NULL, sleepnprint, "New Thread One");
  pthread_create(&tid2,NULL, sleepnprint, "New Thread Two");
  
  child = fork();
  printf("%d:%s\n",getpid(), "fork()ing complete");
  sleep(3);
    
  printf("%d:%s\n",getpid(), "Main thread finished");
  
  pthread_exit(NULL);
  return 0; /* Không bao giờ thực thi */
}
```

```
8970:New Thread One starting up...
8970:fork()ing complete
8973:fork()ing complete
8970:New Thread Two starting up...
8970:New Thread Two finishing...
8970:New Thread One finishing...
8970:Main thread finished
8973:Main thread finished
```

Trong thực tế, việc tạo luồng trước khi fork có thể dẫn đến các lỗi không mong muốn vì (như đã trình bày ở trên) các luồng khác sẽ bị chấm dứt ngay lập tức khi fork. Một luồng khác có thể vừa khóa một mutex (ví dụ: bằng cách gọi `malloc`) và không bao giờ mở khóa nó nữa. Người dùng nâng cao có thể thấy `pthread_atfork` hữu ích, tuy nhiên, chúng tôi khuyên bạn nên cố gắng tránh tạo luồng trước khi forking trừ khi bạn hiểu đầy đủ các hạn chế và khó khăn của phương pháp này.

## Có những lý do nào khác mà `fork` có thể được ưu tiên hơn so với việc tạo một luồng không?
Việc tạo các tiến trình riêng biệt rất hữu ích:
* Khi mong muốn bảo mật hơn (ví dụ: trình duyệt Chrome sử dụng các tiến trình khác nhau cho các tab khác nhau)
* Khi chạy một chương trình hiện có và hoàn chỉnh thì cần một tiến trình mới (ví dụ: khởi động 'gcc')
* Khi bạn đang gặp phải các nguyên thủy đồng bộ hóa và mỗi tiến trình đang hoạt động trên một thứ gì đó trong hệ thống
 
## Làm cách nào để tôi có thể tìm hiểu thêm?
Xem ví dụ đầy đủ trong [trang hướng dẫn](http://man7.org/linux/man-pages/man3/pthread_create.3.html)
Và [hướng dẫn tham khảo pthread](http://man7.org/linux/man-pages/man7/pthreads.7.html)
CŨNG: [Mã mẫu của bên thứ ba ngắn gọn giải thích tạo, tham gia và thoát](http://www.thegeekstuff.com/2012/04/terminate-c-thread/)
