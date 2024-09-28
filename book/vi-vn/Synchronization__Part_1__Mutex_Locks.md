# Giải quyết đoạn mã quan trọng

## Đoạn mã quan trọng là gì?
Đoạn mã quan trọng là một đoạn mã chỉ có thể được thực thi bởi một luồng tại một thời điểm, nếu chương trình hoạt động chính xác (chính xác, có nghĩa là đưa ra kết quả đúng, hiện tại). Nếu hai luồng (hoặc tiến trình) thực thi mã bên trong đoạn mã quan trọng cùng một lúc thì có khả năng chương trình có thể không còn hoạt động chính xác.

## Việc chỉ tăng một biến có phải là một đoạn mã quan trọng không?
Có thể. Việc tăng một biến (`i++`) được thực hiện trong ba bước riêng biệt: Sao chép nội dung bộ nhớ vào thanh ghi CPU. Tăng giá trị trong CPU. Lưu giá trị mới vào bộ nhớ. Nếu vị trí bộ nhớ chỉ có thể được truy cập bởi một luồng (ví dụ: biến tự động `i` bên dưới) thì không có khả năng xảy ra race condition và không có Đoạn mã quan trọng nào được liên kết với `i`. Tuy nhiên, biến `sum` là một biến toàn cục và được truy cập bởi hai luồng. Có thể hai luồng cố gắng tăng biến cùng một lúc.
```C
#include <stdio.h>
#include <pthread.h>
// Biên dịch với -pthread
// gcc -o ex1 ex1.c  -lpthread
// ./ex1

int sum = 0; //chia sẻ

void *countgold(void *param)
{
    int i; //cục bộ cho mỗi luồng
    for (i = 0; i < 10000000; i++)
    {
        sum += 1;
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, countgold, NULL);
    pthread_create(&tid2, NULL, countgold, NULL);

    //Chờ cả hai luồng kết thúc:
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("ARRRRG sum là %d\n", sum);
    return 0;
}

```
Đầu ra điển hình của mã trên là `ARGGGH sum là 8140268`
Một đầu ra khác nhau được in mỗi khi chương trình được chạy vì có race condition; mã không ngăn hai luồng đọc-ghi `sum` cùng một lúc. Ví dụ: cả hai luồng đều sao chép giá trị hiện tại của `sum` vào CPU chạy mỗi luồng (giả sử chọn 123). Cả hai luồng đều tăng một vào bản sao của riêng chúng. Cả hai luồng đều ghi lại giá trị (124). Nếu các luồng đã truy cập tổng vào những thời điểm khác nhau thì số lượng sẽ là 125.
* Chúng ta có thể cung cấp giới hạn trên và giới hạn dưới cho đầu ra của chương trình trên không?

## Làm cách nào để đảm bảo chỉ có một luồng tại một thời điểm có thể truy cập biến toàn cục?
Ý bạn là, "Cứu tôi - Tôi cần một mutex!"
Nếu một luồng hiện đang ở bên trong một đoạn mã quan trọng, chúng tôi muốn luồng khác đợi cho đến khi luồng đầu tiên hoàn thành. Vì mục đích này, chúng ta có thể sử dụng mutex (viết tắt của Mutual Exclusion - Loại trừ lẫn nhau).

Đối với các ví dụ đơn giản, lượng mã nhỏ nhất mà chúng ta cần thêm chỉ là ba dòng:
```C
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER; // biến toàn cục
pthread_mutex_lock(&m);   // bắt đầu Đoạn mã quan trọng
pthread_mutex_unlock(&m); // kết thúc Đoạn mã quan trọng
```
Khi chúng ta hoàn thành với mutex, chúng ta cũng nên gọi `pthread_mutex_destroy(&m)`. Lưu ý, _bạn chỉ có thể hủy một mutex đã được mở khóa._ Việc gọi destroy trên một khóa đã bị hủy, khởi tạo một khóa đã được khởi tạo, khóa một khóa đã bị khóa, mở khóa một khóa đã được mở khóa, v.v. là không được hỗ trợ (ít nhất là đối với mutex mặc định) và thường dẫn đến hành vi không xác định.

## Nếu tôi khóa một mutex, nó có dừng tất cả các luồng khác không?
Không, các luồng khác sẽ tiếp tục. Chỉ khi một luồng cố gắng khóa một mutex đã bị khóa, luồng đó mới phải đợi. Ngay sau khi luồng ban đầu mở khóa mutex, luồng thứ hai (đang chờ) sẽ có được khóa và có thể tiếp tục.

## Có cách nào khác để tạo mutex không?
Có. Bạn có thể sử dụng macro `PTHREAD_MUTEX_INITIALIZER` chỉ cho các biến toàn cục ('tĩnh').
`m = PTHREAD_MUTEX_INITIALIZER` tương đương với mục đích chung hơn
`pthread_mutex_init(&m,NULL)`. Phiên bản init bao gồm các tùy chọn để _trao đổi hiệu suất_ để kiểm tra lỗi bổ sung và các tùy chọn chia sẻ nâng cao.

```C
pthread_mutex_t *lock = malloc(sizeof(pthread_mutex_t)); 
pthread_mutex_init(lock, NULL);
//sau đó 
// chúng ta phải có số lượng mở khóa và khóa bằng nhau trong một lần thực thi
pthread_mutex_destroy(lock);
free(lock);
```
Những điều cần lưu ý về `init` và `destroy`:
* Nhiều luồng thực hiện init/destroy có hành vi không xác định
* Hủy mutex đã khóa có hành vi không xác định
* Về cơ bản, hãy cố gắng giữ nguyên mẫu của một luồng khởi tạo mutex và một và chỉ một luồng hủy mutex.

# Mutex Gotchas

## Vậy `pthread_mutex_lock` có dừng các luồng khác khi chúng đọc cùng một biến không?
Không. Mutex không thông minh như vậy - nó hoạt động với mã (luồng), không phải dữ liệu. Chỉ khi một luồng khác gọi `lock` trên mutex đã khóa, luồng khác mới cần đợi cho đến khi mutex được mở khóa.

Xem xét
```C
int a;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

// sau đó
// Luồng 1
pthread_mutex_lock(&m1);
a++;
pthread_mutex_unlock(&m1);

// Luồng 2
pthread_mutex_lock(&m2);
a++;
pthread_mutex_unlock(&m2);
```

Vẫn sẽ gây ra race condition.


## Tôi có thể tạo mutex trước khi forking không?
Có - tuy nhiên, tiến trình con và tiến trình cha sẽ không chia sẻ bộ nhớ ảo và mỗi tiến trình sẽ có một mutex độc lập với tiến trình kia.

(Lưu ý nâng cao: Có các tùy chọn nâng cao sử dụng bộ nhớ dùng chung cho phép tiến trình con và tiến trình cha chia sẻ mutex nếu nó được tạo với các tùy chọn chính xác và sử dụng đoạn bộ nhớ dùng chung. Xem [ví dụ stackoverflow](http://stackoverflow.com/questions/19172541/procs-fork-and-mutexes))

## Nếu một luồng khóa mutex, thì luồng khác có thể mở khóa nó không?
Không. Cùng một luồng phải mở khóa nó.

## Tôi có thể sử dụng hai hoặc nhiều khóa mutex không?
Có! Trên thực tế, việc có một khóa cho mỗi cấu trúc dữ liệu mà bạn cần cập nhật là phổ biến.

Nếu bạn chỉ có một khóa, thì có thể có sự tranh chấp đáng kể cho khóa giữa hai luồng là không cần thiết. Ví dụ: nếu hai luồng đang cập nhật hai bộ đếm khác nhau, thì có thể không cần thiết phải sử dụng cùng một khóa.
 
Tuy nhiên, chỉ đơn giản là tạo nhiều khóa là không đủ: Điều quan trọng là phải có khả năng lý giải về các đoạn mã quan trọng, ví dụ: điều quan trọng là một luồng không thể đọc hai cấu trúc dữ liệu trong khi chúng đang được cập nhật và tạm thời ở trạng thái không nhất quán.

## Có bất kỳ chi phí nào trong việc gọi khóa và mở khóa không?
Có một lượng nhỏ chi phí khi gọi `pthread_mutex_lock` và `_unlock`; tuy nhiên đây là cái giá bạn phải trả cho các chương trình hoạt động chính xác!

## Ví dụ đầy đủ đơn giản nhất?
Một ví dụ đầy đủ được hiển thị bên dưới
```C
#include <stdio.h>
#include <pthread.h>

// Biên dịch với -pthread
// Tạo mutex đã sẵn sàng để bị khóa!
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int sum = 0;

void *countgold(void *param) {
    int i;
    
    // Cùng một luồng khóa mutex phải mở khóa nó
    // Đoạn mã quan trọng chỉ là 'sum += 1'
    // Tuy nhiên, việc khóa và mở khóa một triệu lần
    // có chi phí đáng kể trong câu trả lời đơn giản này
    
    pthread_mutex_lock(&m);

    // Các luồng khác gọi khóa sẽ phải đợi cho đến khi chúng ta gọi mở khóa

    for (i = 0; i < 10000000; i++) {
	sum += 1;
    }
    pthread_mutex_unlock(&m);
    return NULL;
}

int main() {
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, countgold, NULL);
    pthread_create(&tid2, NULL, countgold, NULL);

    //Chờ cả hai luồng kết thúc:
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("ARRRRG sum là %d\n", sum);
    return 0;
}
```

Trong mã trên, luồng nhận được khóa vào nhà đếm trước khi vào. Đoạn mã quan trọng chỉ là `sum += 1` nên phiên bản sau cũng chính xác nhưng chậm hơn - 
```C
    for (i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&m);
        sum += 1;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}
```
Quá trình này chạy chậm hơn vì chúng ta khóa và mở khóa mutex một triệu lần, điều này rất tốn kém - ít nhất là so với việc tăng một biến. (Và trong ví dụ đơn giản này, chúng ta không thực sự cần các luồng - chúng ta có thể đã cộng hai lần!) Một ví dụ đa luồng nhanh hơn sẽ là cộng một triệu bằng cách sử dụng biến tự động (cục bộ) và chỉ sau đó thêm nó vào tổng được chia sẻ sau khi vòng lặp tính toán đã hoàn thành:
```C
    int local = 0;
    for (i = 0; i < 10000000; i++) {
       local += 1;
    }

    pthread_mutex_lock(&m);
    sum += local;
    pthread_mutex_unlock(&m);

    return NULL;
}
```

## Điều gì xảy ra nếu tôi quên mở khóa?

Deadlock! Chúng ta sẽ nói về deadlock sau một chút nhưng vấn đề với vòng lặp này là gì nếu được gọi bởi nhiều luồng.

```C
while (not_stop) {
    // stdin có thể không an toàn cho luồng
    pthread_mutex_lock(&m);
    char *line = getline(...);
    if (rand() % 2) { /* bỏ qua các dòng một cách ngẫu nhiên */
         continue;
    }
    pthread_mutex_unlock(&m);
    
    process_line(line);
}
```

## Khi nào tôi có thể hủy mutex?
Bạn chỉ có thể hủy mutex đã được mở khóa.

## Tôi có thể sao chép pthread_mutex_t sang một vị trí bộ nhớ mới không?
Không, việc sao chép các byte của mutex sang một vị trí bộ nhớ mới và sau đó sử dụng bản sao là _không_ được hỗ trợ.

## Triển khai đơn giản của mutex sẽ trông như thế nào?

Một đề xuất đơn giản (nhưng không chính xác!) được hiển thị bên dưới. Hàm `unlock` chỉ đơn giản là mở khóa mutex và trả về. Hàm khóa trước tiên kiểm tra xem khóa đã bị khóa chưa. Nếu hiện tại nó đang bị khóa, nó sẽ tiếp tục kiểm tra lại cho đến khi một luồng khác đã mở khóa mutex.
```C
// Phiên bản 1 (Không chính xác!)
// giả sử rằng biến `locked` là `bool`
// hành vi này sẽ khác nhau như thế nào trên máy uni processor so với máy multiprocessor? 

void lock(mutex_t *m) {
    while(m->locked) { /*Đã khóa? Không sao - chỉ cần lặp lại và kiểm tra lại!*/ }
    // hành vi sẽ như thế nào nếu chúng ta đặt một kiểm tra khác như 
    // trên một CPU ?? trên nhiều CPU ?? 
    if ( m->locked != 0 ) 
    {
        m->locked = 1;  
    }
    // hoặc 
    // m->locked = true;
}

void unlock(mutex_t *m) {
    m->locked = 0;
    // hoặc
    // m->locked = false;

}
```
Phiên bản 1 sử dụng 'busy-waiting' (lãng phí tài nguyên CPU một cách không cần thiết) tuy nhiên có một vấn đề nghiêm trọng hơn: Chúng ta có race-condition! 

Nếu hai luồng đều gọi `lock` đồng thời thì có thể cả hai luồng đều đọc 'm_locked' là 0. Do đó, cả hai luồng sẽ tin rằng chúng có quyền truy cập độc quyền vào khóa và cả hai luồng sẽ tiếp tục. Ôi!

Điều gì sẽ xảy ra nếu một trong nhiều luồng thực sự có thể lấy khóa, gọi `unlock` và hành vi của các luồng khác muốn `lock` sẽ như thế nào?  


Chúng ta có thể cố gắng giảm chi phí CPU một chút bằng cách gọi, [pthread_yield](http://man7.org/linux/man-pages/man3/pthread_yield.3.html) bên trong vòng lặp  - pthread_yield gợi ý cho hệ điều hành rằng luồng không sử dụng CPU trong một thời gian ngắn, vì vậy CPU có thể được gán cho các luồng đang chờ chạy. Nhưng không khắc phục được race-condition.

Tại sao nó không khắc phục được race-condition?
Nó thậm chí không phải là một nỗ lực để khắc phục nó, nó là một nỗ lực để làm cho mã chạy nhanh hơn. 

Chúng ta cần một triển khai tốt hơn - bạn có thể làm việc để ngăn chặn race-condition không?


## Làm cách nào để tôi tìm hiểu thêm?
[Chơi!](http://cs-education.github.io/sys) Đọc trang hướng dẫn!
* [Trang hướng dẫn pthread_mutex_lock](http://linux.die.net/man/3/pthread_mutex_lock)
* [Trang hướng dẫn pthread_mutex_unlock](http://linux.die.net/man/3/pthread_mutex_unlock)
* [Trang hướng dẫn pthread_mutex_init](http://linux.die.net/man/3/pthread_mutex_init)
* [Trang hướng dẫn pthread_mutex_destroy](http://linux.die.net/man/3/pthread_mutex_destroy)


