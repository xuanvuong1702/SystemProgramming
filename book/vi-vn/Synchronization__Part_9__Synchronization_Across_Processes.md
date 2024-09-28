# Đồng bộ hóa tiến trình

Bạn nghĩ rằng bạn đang sử dụng các tiến trình khác nhau, vì vậy bạn không phải đồng bộ hóa? Hãy nghĩ lại! Bạn có thể không gặp phải race condition trong một tiến trình nhưng điều gì sẽ xảy ra nếu tiến trình của bạn cần tương tác với hệ thống xung quanh nó? Hãy xem xét một ví dụ tạo động lực

```C
void write_string(const char *data) {
    int fd = open("my_file.txt", O_WRONLY);
    write(fd, data, strlen(data));
    close(fd);
}

int main() {
    if(!fork()) {
        write_string("key1: value1");
        wait(NULL);
    } else {
        write_string("key2: value2");
    }
    return 0;
}
```

Khi chương trình được biên dịch và chạy, nếu không có lệnh gọi hệ thống nào thất bại thì chúng ta sẽ nhận được một cái gì đó trông giống như thế này (với điều kiện tệp trống lúc đầu).
```
key1: value1
key2: value2
```

hoặc

```
key2: value2
key1: value1
```

## Gián đoạn

Nhưng, có một sắc thái ẩn. Hầu hết các lệnh gọi hệ thống có thể bị `gián đoạn` có nghĩa là hệ điều hành có thể dừng một lệnh gọi hệ thống đang diễn ra vì nó cần dừng tiến trình. Vì vậy, trừ khi `fork` `wait` `open` và `close` thất bại -- chúng thường đi đến hoàn thành -- điều gì sẽ xảy ra nếu `write` thất bại? Nếu ghi thất bại và không có byte nào được ghi, chúng ta có thể nhận được một cái gì đó giống như `key1: value1` hoặc `key2: value2`. Đây là mất dữ liệu là không chính xác nhưng sẽ không làm hỏng tệp. Điều gì sẽ xảy ra nếu ghi bị gián đoạn sau một ghi một phần? Chúng ta nhận được tất cả các loại điên rồ. Ví dụ,

```
key2: key1: value1
```

## Giải pháp

Vậy chúng ta nên làm gì? Chúng ta nên sử dụng mutex dùng chung! Hãy xem xét đoạn mã sau.
```C
pthread_mutex_t * mutex = NULL;
pthread_mutexattr_t attr;

void write_string(const char *data) {
    pthread_mutex_lock(mutex);
    int fd = open("my_file.txt", O_WRONLY);
    int bytes_to_write = strlen(data), written = 0;
    while(written < bytes_to_write) {
        written += write(fd, data + written, bytes_to_write - written);
    }
    close(fd);
    pthread_mutex_unlock(mutex);
}

int main() {
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pmutex = mmap (NULL, sizeof(pthread_mutex_t), 
                PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    pthread_mutex_init(pmutex, &attrmutex);
    if(!fork()) {
        write_string("key1: value1");
        wait(NULL);
        pthread_mutex_destroy(pmutex);
        pthread_mutexattr_destroy(&attrmutex); 
        munmap((void *)pmutex, sizeof(*pmutex));
    } else {
        write_string("key2: value2");
    }
    return 0;
}
```

Những gì mã thực hiện trong main là khởi tạo mutex được chia sẻ tiến trình bằng cách sử dụng một đoạn bộ nhớ `dùng chung`. Bạn sẽ tìm hiểu xem lệnh gọi `mmap` này làm gì sau này -- chỉ cần giả sử bây giờ nó tạo ra bộ nhớ được chia sẻ giữa các tiến trình. Chúng ta có thể khởi tạo `pthread_mutex_t` trong đoạn bộ nhớ đặc biệt đó và sử dụng nó như bình thường. Để chống lại `ghi` thất bại, chúng ta đã đặt lệnh gọi `ghi` bên trong vòng lặp while tiếp tục ghi miễn là còn byte để ghi. Bây giờ nếu tất cả các lệnh gọi hệ thống khác hoạt động, sẽ không còn race condition nữa.

Hầu hết các chương trình cố gắng tránh hoàn toàn vấn đề này bằng cách ghi vào các tệp riêng biệt, nhưng điều tốt là biết rằng có mutex giữa các tiến trình và chúng rất hữu ích.

## Bạn có thể làm gì khác?

Bạn có thể sử dụng tất cả các nguyên thủy mà bạn đã được dạy trước đó! Rào cản, semaphore và biến điều kiện đều có thể được khởi tạo trên một đoạn bộ nhớ dùng chung và được sử dụng theo cách tương tự như các đối tác đa luồng của chúng.

## Được rồi, vậy khi nào tôi sẽ sử dụng cái này?

* Bạn không phải lo lắng về việc các địa chỉ bộ nhớ tùy ý trở thành ứng cử viên race condition. Điều này có nghĩa là chỉ những khu vực mà bạn đặc biệt `mmap` hoặc bên ngoài tài nguyên hệ thống như tệp mới có nguy cơ.
* Bạn nhận được sự cô lập tốt đẹp của một tiến trình, vì vậy nếu một tiến trình thất bại, hệ thống có thể duy trì nguyên vẹn
* Khi bạn có nhiều luồng, việc tạo một tiến trình có thể giảm tải cho hệ thống

## Tôi có cần ghi nhớ các chi tiết cụ thể không?

Không, bạn chỉ cần biết rằng mutex và các nguyên thủy đồng bộ hóa khác có thể được chia sẻ giữa các tiến trình.

