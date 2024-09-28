# Những điều cần lưu ý về Pipe

## Ví dụ hoàn chỉnh không hoạt động!

Đây là một ví dụ hoàn chỉnh nhưng không hoạt động! Tiến trình con đọc từng byte một từ pipe và in ra - nhưng chúng ta không bao giờ thấy tin nhắn! Bạn có biết tại sao không?

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    int fd[2];
    pipe(fd);
    // Bạn phải đọc từ fd[0] và ghi vào fd[1]
    printf("Reading from %d, writing to %d\n", fd[0], fd[1]);

    pid_t p = fork();
    if (p > 0) {
        /* Tôi có một tiến trình con, do đó tôi là tiến trình cha*/
        write(fd[1],"Hi Child!",9);

        /*đừng quên tiến trình con của bạn*/
        wait(NULL);
    } else {
        char buf;
        int bytesread;
        // đọc một byte tại một thời điểm.
        while ((bytesread = read(fd[0], &buf, 1)) > 0) {
            putchar(buf);
        }
    }
    return 0;
}

```

Tiến trình cha gửi các byte `H,i,(dấu cách),C...!` vào pipe (điều này có thể bị chặn nếu pipe đầy).
Tiến trình con bắt đầu đọc pipe từng byte một. Trong trường hợp trên, tiến trình con sẽ đọc và in từng ký tự. Tuy nhiên, nó không bao giờ thoát khỏi vòng lặp while! Khi không còn ký tự nào để đọc, nó chỉ đơn giản là bị chặn và chờ thêm.

Lệnh gọi `putchar` ghi các ký tự ra nhưng chúng ta không bao giờ xóa bộ đệm `stdout`. Tức là, chúng ta đã chuyển tin nhắn từ tiến trình này sang tiến trình khác nhưng nó vẫn chưa được in ra. Để xem tin nhắn, chúng ta có thể xóa bộ đệm, ví dụ: `fflush(stdout)` (hoặc `printf("\n")` nếu đầu ra đang được chuyển đến terminal). Một giải pháp tốt hơn cũng sẽ thoát khỏi vòng lặp bằng cách kiểm tra dấu hiệu kết thúc tin nhắn,

```C
        while ((bytesread = read(fd[0], &buf, 1)) > 0) {
            putchar(buf);
            if (buf == '!') break; /* Kết thúc tin nhắn */
        }
```

Và tin nhắn sẽ được xóa vào terminal khi tiến trình con thoát.


## Muốn sử dụng pipe với printf và scanf? Sử dụng fdopen!

Các bộ mô tả tệp POSIX là các số nguyên đơn giản 0,1,2,3...
Ở cấp độ thư viện C, C bao bọc chúng bằng bộ đệm và các hàm hữu ích như printf và scanf, để chúng ta có thể dễ dàng in hoặc phân tích cú pháp các số nguyên, chuỗi, v.v.
Nếu bạn đã có bộ mô tả tệp, thì bạn có thể tự 'bao bọc' nó thành con trỏ FILE bằng cách sử dụng `fdopen`:


```C
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    char *name = "Fred";
    int score = 123;
    int filedes = open("mydata.txt", O_CREAT, S_IWUSR | S_IRUSR);

    FILE *f = fdopen(filedes, "w");
    fprintf(f, "Name:%s Score:%d\n", name, score);
    fclose(f);
```

Đối với việc ghi vào tệp, điều này là không cần thiết - chỉ cần sử dụng `fopen` làm giống như `open` và `fdopen`
Tuy nhiên, đối với pipe, chúng ta đã có bộ mô tả tệp - vì vậy đây là thời điểm tuyệt vời để sử dụng `fdopen`!

## Ví dụ hoàn chỉnh sử dụng pipe gần như hoạt động!

Đây là một ví dụ hoàn chỉnh sử dụng pipe gần như hoạt động! Bạn có thể phát hiện ra lỗi không? Gợi ý: Tiến trình cha không bao giờ in bất cứ thứ gì!

```C
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int fh[2];
    pipe(fh);
    FILE *reader = fdopen(fh[0], "r");
    FILE *writer = fdopen(fh[1], "w");
    pid_t p = fork();
    if (p > 0) {
        int score;
        fscanf(reader, "Score %d", &score);
        printf("Tiến trình con cho biết điểm số là %d\n", score);
    } else {
        fprintf(writer, "Score %d", 10 + 10);
        fflush(writer);
    }
    return 0;
}
```

Lưu ý rằng tài nguyên pipe (không tên) sẽ biến mất sau khi cả tiến trình con và tiến trình cha đã thoát. Trong ví dụ trên, tiến trình con sẽ gửi các byte và tiến trình cha sẽ nhận các byte từ pipe. Tuy nhiên, không có ký tự xuống dòng nào được gửi, vì vậy `fscanf` sẽ tiếp tục yêu cầu các byte vì nó đang chờ kết thúc dòng, tức là nó sẽ chờ mãi mãi! Cách khắc phục là đảm bảo chúng ta gửi ký tự xuống dòng, để `fscanf` sẽ trả về.

```C
thay đổi:   fprintf(writer, "Score %d", 10 + 10);
thành:       fprintf(writer, "Score %d\n", 10 + 10);
```

## Vậy chúng ta có cần `fflush` không?

Có, nếu bạn muốn các byte của mình được gửi đến pipe ngay lập tức! Vào đầu khóa học này, chúng ta đã giả định rằng các luồng tệp luôn được _đệm dòng_, tức là thư viện C sẽ xóa bộ đệm của nó mỗi khi bạn gửi ký tự xuống dòng. Trên thực tế, điều này chỉ đúng với các luồng terminal - đối với các luồng tệp khác, thư viện C cố gắng cải thiện hiệu suất bằng cách chỉ xóa khi bộ đệm nội bộ của nó đầy hoặc tệp bị đóng.


## Khi nào tôi cần hai pipe?

Nếu bạn cần gửi dữ liệu đến và từ một tiến trình con một cách không đồng bộ, thì cần có hai pipe (một cho mỗi hướng).
Nếu không, tiến trình con sẽ cố gắng đọc dữ liệu của chính nó dành cho tiến trình cha (và ngược lại)!

## Lưu ý khi đóng pipe

Các tiến trình nhận được tín hiệu SIGPIPE khi không có tiến trình nào đang lắng nghe! Từ trang hướng dẫn pipe(2) - 
```
Nếu tất cả các bộ mô tả tệp tham chiếu đến đầu đọc của pipe đã bị đóng, 
thì write(2) sẽ khiến tín hiệu SIGPIPE được tạo ra cho tiến trình gọi. 
```

Mẹo: Lưu ý chỉ người ghi (không phải người đọc) mới có thể sử dụng tín hiệu này.
Để thông báo cho người đọc rằng người ghi đang đóng đầu pipe của họ, bạn có thể ghi byte đặc biệt của riêng mình (ví dụ: 0xff) hoặc một tin nhắn (`"Bye!"`)

## Ví dụ về việc bắt tín hiệu này không hoạt động!

Đây là một ví dụ về việc bắt tín hiệu này không hoạt động! Bạn có biết tại sao không?

```C
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void no_one_listening(int signal) {
    write(1, "Không có ai đang lắng nghe!\n", 27);
}

int main() {
    signal(SIGPIPE, no_one_listening);
    int filedes[2];
    
    pipe(filedes);
    pid_t child = fork();
    if (child > 0) { 
        /* Tôi chắc chắn là tiến trình cha. Đóng đầu nghe của pipe */
        /* Tôi không nghe nữa!*/
        close(filedes[0]);
    } else {
        /* Tiến trình con ghi tin nhắn vào pipe */
        write(filedes[1], "One", 3);
        sleep(2);
        // Ghi này có tạo ra SIGPIPE không?
        write(filedes[1], "Two", 3);
        write(1, "Done\n", 5);
    }
    return 0;
}
```

Lỗi trong mã trên là vẫn còn một trình đọc cho pipe! Tiến trình con vẫn mở bộ mô tả tệp đầu tiên của pipe và hãy nhớ đặc tả? Tất cả trình đọc phải được đóng lại.

Khi forking, _Thực hành phổ biến_ là đóng đầu không cần thiết (không sử dụng) của mỗi pipe trong tiến trình con và tiến trình cha. Ví dụ: tiến trình cha có thể đóng đầu đọc và tiến trình con có thể đóng đầu ghi (và ngược lại nếu bạn có hai pipe)

## Cái gì đang lấp đầy pipe? Điều gì xảy ra khi pipe đầy?

Pipe bị lấp đầy khi người ghi ghi quá nhiều vào pipe mà người đọc không đọc bất kỳ phần nào của nó. Khi pipe đầy, tất cả các hoạt động ghi sẽ thất bại cho đến khi có hoạt động đọc xảy ra. Ngay cả khi đó, hoạt động ghi có thể bị lỗi một phần nếu pipe còn một chút dung lượng trống nhưng không đủ cho toàn bộ tin nhắn.

Để tránh điều này, thường làm hai việc. Hoặc tăng kích thước của pipe. Hoặc phổ biến hơn, hãy sửa thiết kế chương trình của bạn để pipe liên tục được đọc.

## Pipe có an toàn cho tiến trình không?

Có! Ghi pipe là nguyên tử cho đến kích thước của pipe. Có nghĩa là nếu hai tiến trình cố gắng ghi vào cùng một pipe, kernel có mutex nội bộ với pipe mà nó sẽ khóa, thực hiện ghi và trả về. Điều duy nhất cần lưu ý là khi pipe sắp đầy. Nếu hai tiến trình đang cố gắng ghi và pipe chỉ có thể đáp ứng một phần ghi, thì ghi pipe đó không phải là nguyên tử -- hãy cẩn thận về điều đó!

## Vòng đời của pipe

Pipe không tên (loại chúng ta đã thấy cho đến thời điểm này) sống trong bộ nhớ (không chiếm bất kỳ dung lượng đĩa nào) và là một hình thức giao tiếp liên tiến trình (IPC) đơn giản và hiệu quả, hữu ích cho việc truyền dữ liệu và tin nhắn đơn giản. Khi tất cả các tiến trình đã đóng, tài nguyên pipe được giải phóng.

Một lựa chọn thay thế cho pipe _không tên_ là pipe _có tên_ được tạo bằng cách sử dụng `mkfifo`.

# Pipe có tên

## Làm cách nào để tạo pipe có tên?

Từ dòng lệnh: `mkfifo`
Từ C: `int mkfifo(const char *pathname, mode_t mode);`

Bạn cung cấp cho nó tên đường dẫn và chế độ hoạt động, nó sẽ sẵn sàng hoạt động! Pipe có tên không chiếm dung lượng trên đĩa. Về cơ bản, hệ điều hành đang nói với bạn khi bạn có một pipe có tên là nó sẽ tạo một pipe không tên tham chiếu đến pipe có tên, và thế là xong! Không có phép thuật bổ sung nào. Điều này chỉ để thuận tiện cho việc lập trình nếu các tiến trình được bắt đầu mà không cần forking (có nghĩa là sẽ không có cách nào để lấy bộ mô tả tệp cho tiến trình con cho pipe không tên)

## Tại sao pipe của tôi bị treo?

Đọc và ghi bị treo trên Pipe có tên cho đến khi có ít nhất một trình đọc và một trình ghi, hãy xem ví dụ này

```bash
1$ mkfifo fifo
1$ echo Hello > fifo
# Điều này sẽ bị treo cho đến khi tôi thực hiện việc này trên một terminal khác hoặc một tiến trình khác
2$ cat fifo
Hello
```

Bất kỳ `open` nào được gọi trên pipe có tên, kernel sẽ chặn cho đến khi một tiến trình khác gọi open ngược lại. Có nghĩa là, echo gọi `open(.., O_WRONLY)` nhưng điều đó bị chặn cho đến khi cat gọi `open(.., O_RDONLY)`, sau đó các chương trình được phép tiếp tục.

## Race condition với pipe có tên.

Có gì sai với chương trình sau?

```C
// Chương trình 1

int main() {
    int fd = open("fifo", O_RDWR | O_TRUNC);
    write(fd, "Hello!", 6);
    close(fd);
    return 0;
}

//Chương trình 2
int main() {
    char buffer[7];
    int fd = open("fifo", O_RDONLY);
    read(fd, buffer, 6);
    buffer[6] = '\0';
    printf("%s\n", buffer);
    return 0;
}
```

Điều này có thể không bao giờ in ra hello vì race condition. Vì bạn đã mở pipe trong tiến trình đầu tiên theo cả hai quyền, open sẽ không chờ trình đọc vì bạn đã nói với hệ điều hành rằng bạn là trình đọc! Đôi khi có vẻ như nó hoạt động vì việc thực thi mã trông giống như thế này.

| Tiến trình 1 | Tiến trình 2 |
|-----------|-----------|
|  open(O_RDWR) & write()  |           |
|           |   open(O_RDONLY) & read()  |
|  close() & exit()   |           |
|           | print() & exit() |


Đôi khi nó sẽ không hoạt động

| Tiến trình 1 | Tiến trình 2 |
|-----------|-----------|
|  open(O_RDWR) & write()  |           |
|  close() & exit()   |  (Pipe có tên bị hủy)  |
|   (Bị chặn vô thời hạn)        |    open(O_RDONLY)       |


