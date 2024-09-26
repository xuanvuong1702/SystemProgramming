# Các hàm pthread khác

## Làm thế nào để tạo một pthread?
Xem [Pthreads Phần 1](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction) giới thiệu `pthread_create` và `pthread_join`

## Nếu tôi gọi `pthread_create` hai lần, quá trình của tôi có bao nhiêu ngăn xếp?
Quá trình của bạn sẽ chứa ba ngăn xếp - một cho mỗi luồng. Luồng đầu tiên được tạo khi quá trình bắt đầu, và bạn đã tạo thêm hai luồng nữa. Thực tế có thể có nhiều ngăn xếp hơn thế, nhưng hãy bỏ qua sự phức tạp đó cho tới khi nào. Ý tưởng quan trọng là mỗi luồng đều yêu cầu một ngăn xếp vì ngăn xếp chứa các biến tự động và đăng ký PC CPU cũ, để nó có thể quay lại thực thi hàm gọi sau khi hàm kết thúc.

## Sự khác biệt giữa một quá trình đầy đủ và một luồng là gì?
Ngoài ra, không giống như các quá trình, các luồng trong cùng một quá trình có thể chia sẻ cùng một bộ nhớ toàn cục (phân đoạn dữ liệu và heap).

## `pthread_cancel` làm gì?
Dừng một luồng. Lưu ý rằng luồng có thể không thực sự bị dừng ngay lập tức. Ví dụ, nó có thể bị kết thúc khi luồng thực hiện một lệnh hệ điều hành (ví dụ: `write`).

Trên thực tế, `pthread_cancel` ít khi được sử dụng vì nó không cho luồng cơ hội để dọn dẹp sau chính nó (ví dụ, nó có thể đã mở một số tệp).
Một cách triển khai thay thế là sử dụng một biến boolean (int) mà giá trị của nó được sử dụng để thông báo cho các luồng khác rằng họ nên kết thúc và dọn dẹp.

## Sự khác biệt giữa `exit` và `pthread_exit` là gì?
`exit(42)` thoát khỏi toàn bộ quá trình và đặt giá trị thoát của quá trình. Điều này tương đương với `return 42` trong phương thức main. Tất cả các luồng bên trong quá trình đều bị dừng.

`pthread_exit(void *)` chỉ dừng luồng gọi tức là luồng không bao giờ trả lại sau khi gọi `pthread_exit`. Thư viện pthread sẽ tự động kết thúc quá trình nếu không còn luồng nào đang chạy. `pthread_exit(...)` tương đương với việc trả về từ hàm của luồng; cả hai đều kết thúc luồng và cũng đặt giá trị trả về (con trỏ void *) cho luồng.

Gọi `pthread_exit` trong luồng `main` là một cách phổ biến cho các chương trình đơn giản để đảm bảo rằng tất cả các luồng kết thúc. Ví dụ, trong chương trình sau, các luồng `myfunc` có thể sẽ không có thời gian để bắt đầu.
```C
int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, myfunc, "Jabberwocky");
    pthread_create(&tid2, NULL, myfunc, "Vorpel");
    exit(42); //hoặc return 42;

    // Không có mã nào được chạy sau exit
}
```
Hai chương trình sau sẽ chờ các luồng mới kết thúc-
```C
int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, myfunc, "Jabberwocky");
    pthread_create(&tid2, NULL, myfunc, "Vorpel");
    pthread_exit(NULL); 

    // Không có mã nào được chạy sau pthread_exit
    // Tuy nhiên, quá trình sẽ tiếp tục tồn tại cho đến khi cả hai luồng đều đã kết thúc
}
```
Hoặc, chúng ta join vào mỗi luồng (tức là chờ nó kết thúc) trước khi chúng ta trả về từ main (hoặc gọi exit).
```C
int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, myfunc, "Jabberwocky");
    pthread_create(&tid2, NULL, myfunc, "Vorpel");
    // chờ cả hai luồng kết thúc :
    void* result;
    pthread_join(tid1, &result);
    pthread_join(tid2, &result); 
    return 42;
}
```
Lưu ý rằng phiên bản pthread_exit tạo ra các luồng zombie, tuy nhiên, đây không phải là các quá trình chạy lâu, vì vậy chúng ta không quan tâm.

## Làm thế nào để kết thúc một luồng?
* Trả về từ hàm luồng
* Gọi `pthread_exit`
* Hủy luồng bằng `pthread_cancel`
* Kết thúc quá trình (ví dụ: SIGTERM); exit(); trả về từ `main`

## Mục đích của pthread_join là gì?
* Chờ một luồng kết thúc
* Dọn dẹp tài nguyên luồng
* Lấy giá trị trả về của luồng

## Điều gì xảy ra nếu bạn không gọi `pthread_join`?
Các luồng đã hoàn thành sẽ tiếp tục tiêu thụ tài nguyên. Cuối cùng, nếu tạo đủ nhiều luồng, `pthread_create` sẽ thất bại.
Trên thực tế, điều này chỉ là vấn đề đối với các quá trình chạy lâu nhưng không phải là vấn đề đối với các quá trình đơn giản, ngắn hạn vì tất cả tài nguyên luồng đều được tự động giải phóng khi quá trình kết thúc.

## Tôi nên sử dụng `pthread_exit` hay `pthread_join`?
Cả `pthread_exit` và `pthread_join` đều cho phép các luồng khác kết thúc theo cách của chúng (ngay cả khi được gọi trong luồng chính). Tuy nhiên, chỉ `pthread_join` sẽ trả về cho bạn khi luồng được chỉ định kết thúc. `pthread_exit` không chờ đợi và sẽ ngay lập tức kết thúc luồng của bạn và không cho bạn cơ hội để tiếp tục thực thi.

## Bạn có thể truyền con trỏ đến các biến ngăn xếp từ một luồng sang một luồng khác không?
Có. Tuy nhiên, bạn cần phải rất cẩn thận về tuổi thọ của các biến ngăn xếp.
```C
pthread_t start_threads() {
    int start = 42;
    pthread_t tid;
    pthread_create(&tid, 0, myfunc, &start); // LỖI!
    return tid;
}
```
Mã trên không hợp lệ vì hàm `start_threads` có thể sẽ trả về trước khi `myfunc` bắt đầu. Hàm truyền địa chỉ của `start`, tuy nhiên vào thời điểm `myfunc` được thực thi, `start` không còn trong phạm vi và địa chỉ của nó sẽ được sử dụng lại cho một biến khác.

Mã sau đây là hợp lệ vì tuổi thọ của biến ngăn xếp dài hơn luồng nền.

```C
void start_threads() {
    int start = 42;
    void *result;
    pthread_t tid;
    pthread_create(&tid, 0, myfunc, &start); // OK - start sẽ hợp lệ!
    pthread_join(tid, &result);
}
```

# Giới thiệu về Điều kiện Đua

## Làm thế nào để tạo ra mười luồng với các giá trị khởi đầu khác nhau.
Đoạn mã sau đây được dự định để khởi động mười luồng với các giá trị 0,1,2,3,...9
Tuy nhiên, khi chạy nó in ra `1 7 8 8 8 8 8 8 8 10`! Bạn có thể thấy lý do tại sao không?
```C
#include <pthread.h>
void* myfunc(void* ptr) {
    int i = *((int *) ptr);
    printf("%d ", i);
    return NULL;
}

int main() {
    // Mỗi luồng nhận một giá trị khác nhau của i để xử lý
    int i;
    pthread_t tid;
    for(i =0; i < 10; i++) {
        pthread_create(&tid, NULL, myfunc, &i); // LỖI
    }
    pthread_exit(NULL);
}
```
Mã trên gặp phải một `điều kiện đua` - giá trị của i đang thay đổi. Các luồng mới bắt đầu muộn hơn (trong đầu ra của ví dụ, luồng cuối cùng bắt đầu sau khi vòng lặp kết thúc).

Để khắc phục điều kiện đua này, chúng ta sẽ cung cấp cho mỗi luồng một con trỏ đến khu vực dữ liệu riêng của nó. Ví dụ, cho mỗi luồng, chúng ta có thể muốn lưu trữ id, một giá trị khởi đầu và một giá trị đầu ra:
```C
struct T {
    pthread_t id;
    int start;
    char result[100];
};
```
Các giá trị này có thể được lưu trữ trong một mảng -
```C
struct T *info = calloc(10 , sizeof(struct T)); // dự trữ đủ byte cho mười cấu trúc T
```
Và mỗi phần tử mảng được truyền vào mỗi luồng -
```C
pthread_create(&info[i].id, NULL, func, &info[i]);
```

## Tại sao một số hàm như asctime, getenv, strtok, strerror không an toàn khi sử dụng trong luồng?
Để trả lời câu hỏi này, hãy xem xét một hàm đơn giản khác cũng không 'an toàn khi sử dụng trong luồng'
```C
char *to_message(int num) {
    char static result [256];
    if (num < 10) sprintf(result, "%d : blah blah" , num);
    else strcpy(result, "Unknown");
    return result;
}
```
Trong đoạn mã trên, bộ đệm kết quả được lưu trữ trong bộ nhớ toàn cục. Điều này tốt - chúng ta không muốn trả về một con trỏ đến một địa chỉ không hợp lệ trên ngăn xếp, nhưng chỉ có một bộ đệm kết quả trong toàn bộ bộ nhớ. Nếu hai luồng sử dụng nó cùng một lúc thì một luồng sẽ làm hỏng luồng khác:

Thời gian | Luồng 1 | Luồng 2 | Bình luận
-----|----------| --------|------
 1   | to_m(5 ) |         |
 2   |          | to_m(99)| Bây giờ cả hai luồng đều thấy "Unknown" được lưu trữ trong bộ đệm kết quả

## Biến điều kiện, semaphore, mutex là gì?
Đây là các khóa đồng bộ hóa được sử dụng để ngăn chặn điều kiện đua và đảm bảo đồng bộ hóa đúng giữa các luồng chạy trong cùng một chương trình. Ngoài ra, các khóa này về mặt khái niệm giống hệt với các nguyên tắc cơ bản được sử dụng bên trong kernel.

## Có lợi ích nào khi sử dụng luồng so với việc tạo ra các quá trình?
Có! Việc chia sẻ thông tin giữa các luồng dễ dàng vì các luồng (cùng một quá trình) sống trong cùng không gian bộ nhớ ảo.
Ngoài ra, việc tạo một luồng nhanh hơn nhiều so với việc tạo (tách) một quá trình.

## Có nhược điểm nào khi sử dụng luồng so với việc tạo ra các quá trình?
Có! Không có sự cô lập! Vì các luồng sống trong cùng một quá trình, một luồng có quyền truy cập vào cùng một không gian bộ nhớ ảo như các luồng khác. Một luồng duy nhất có thể kết thúc toàn bộ quá trình (ví dụ: bằng cách cố gắng đọc địa chỉ zero).

## Bạn có thể tạo một quá trình với nhiều luồng không?
Có! Tuy nhiên, quá trình con chỉ có một luồng duy nhất (là một bản sao của luồng đã gọi `fork`. Chúng ta có thể thấy điều này qua một ví dụ đơn giản, nơi các luồng nền không bao giờ in ra một thông điệp thứ hai trong quá trình con.
```C
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static pid_t child = -2;

void *sleepnprint(void *arg) {
    printf("%d:%s starting up...\n", getpid(), (char *) arg);

    while (child == -2) {sleep(1);} /* Later we will use condition variables */

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
    return 0; /* Never executes */
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
Trên thực tế, việc tạo luồng trước khi fork có thể dẫn đến các lỗi không mong muốn vì (như đã trình bày ở trên) các luồng khác sẽ ngay lập tức bị kết thúc khi fork. Một luồng khác có thể vừa khóa mutex (ví dụ: bằng cách gọi malloc) và không bao giờ mở khóa nó nữa. Người dùng cao cấp có thể thấy `pthread_atfork` hữu ích tuy nhiên chúng tôi khuyên bạn nên tránh tạo luồng trước khi fork trừ khi bạn hiểu rõ hạn chế và khó khăn của cách tiếp cận này.

## Có lý do khác nào mà `fork` có thể được ưu tiên hơn việc tạo một luồng.
Việc tạo các quá trình riêng biệt là hữu ích
* Khi muốn tăng cường bảo mật (ví dụ, trình duyệt Chrome sử dụng các quá trình khác nhau cho các tab khác nhau)
* Khi chạy một chương trình hiện có và hoàn chỉnh thì cần một quá trình mới (ví dụ: khởi động 'gcc')
* Khi bạn gặp phải các nguyên tắc đồng bộ hóa và mỗi quá trình đang hoạt động trên một thứ gì đó trong hệ thống

## Làm thế nào để tìm hiểu thêm?
Xem ví dụ đầy đủ trong [trang man](http://man7.org/linux/man-pages/man3/pthread_create.3.html)
Và [hướng dẫn tham chiếu pthread](http://man7.org/linux/man-pages/man7/pthreads.7.html)
CŨNG NHƯ: [Mã mẫu của bên thứ ba giải thích tạo, tham gia và thoát](http://www.thegeekstuff.com/2012/04/terminate-c-thread/)