## IPC là gì?

Giao tiếp liên tiến trình (Inter-process communication - IPC) là bất kỳ cách nào để một tiến trình trao đổi thông tin với một tiến trình khác. Bạn đã thấy một hình thức của nó trong bộ nhớ ảo! Một phần của bộ nhớ ảo có thể được chia sẻ giữa tiến trình cha và tiến trình con, dẫn đến sự giao tiếp. Bạn có thể muốn bao bọc bộ nhớ đó trong mutex `pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);` (hoặc mutex trên toàn tiến trình) để ngăn chặn tình trạng race condition.

Có nhiều cách IPC tiêu chuẩn hơn, chẳng hạn như pipe! Hãy xem xét nếu bạn gõ dòng sau vào terminal:

```bash
$ ls -1 | cut -d'.' -f1 | uniq | sort | tee dir_contents
```

Đoạn mã sau làm gì (Nó không thực sự quan trọng nên bạn có thể bỏ qua phần này nếu muốn)? Nó sẽ liệt kê (`ls`) thư mục hiện tại (tham số `-1` nghĩa là nó xuất ra một mục trên mỗi dòng). Lệnh `cut` sau đó lấy mọi thứ trước dấu chấm đầu tiên. `uniq` đảm bảo tất cả các dòng là duy nhất, `sort` sắp xếp chúng và `tee` xuất ra một tệp.

Phần quan trọng là bash tạo ra **5 tiến trình riêng biệt** và kết nối các stdout/stdin của chúng với các pipe theo sơ đồ sau:

(0) ls (1)------>(0) cut (1)------->(0) uniq (1)------>(0) sort (1)------>(0) tee (1)

Các số trong pipe là các bộ mô tả tệp (file descriptor) cho mỗi tiến trình và mũi tên biểu thị hướng chuyển hướng hoặc nơi xuất ra của pipe.

## Pipe là gì?

Một pipe POSIX gần giống như phiên bản thực tế của nó - bạn có thể đưa các byte vào một đầu và chúng sẽ xuất hiện ở đầu kia theo cùng thứ tự. Tuy nhiên, không giống như các pipe thực, luồng luôn theo cùng một hướng, một bộ mô tả tệp được sử dụng để đọc và bộ mô tả tệp kia để ghi. Lệnh gọi hệ thống `pipe` được sử dụng để tạo pipe.

```C
int filedes[2];
pipe (filedes);
printf("read from %d, write to %d\n", filedes[0], filedes[1]);
```

Các bộ mô tả tệp này có thể được sử dụng với `read`:

```C
// Để đọc...
char buffer[80];
int bytesread = read(filedes[0], buffer, sizeof(buffer));
```

Và `write`:

```C
write(filedes[1], "Go!", 4);
```

## Làm cách nào tôi có thể sử dụng pipe để giao tiếp với một tiến trình con?

Một phương pháp phổ biến để sử dụng pipe là tạo pipe trước khi fork.

```C
int filedes[2];
pipe (filedes);
pid_t child = fork();
if (child > 0) { /* Tôi chắc chắn là tiến trình cha */
    char buffer[80];
    int bytesread = read(filedes[0], buffer, sizeof(buffer));
    // làm gì đó với các byte đã đọc    
}
```

Tiến trình con sau đó có thể gửi tin nhắn trở lại tiến trình cha:

```C
if (child == 0) {
   write(filedes[1], "done", 4);
}
```

## Tôi có thể sử dụng pipe bên trong một tiến trình duy nhất không?

Câu trả lời ngắn gọn: Có, nhưng tôi không chắc tại sao bạn lại muốn làm vậy LOL!

Đây là một chương trình ví dụ gửi tin nhắn cho chính nó:

```C
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int fh[2];
    pipe(fh);
    FILE *reader = fdopen(fh[0], "r");
    FILE *writer = fdopen(fh[1], "w");
    // Hoan hô, bây giờ tôi có thể sử dụng printf thay vì sử dụng read() write() cấp thấp
    printf("Writing...\n");
    fprintf(writer,"%d %d %d\n", 10, 20, 30);
    fflush(writer);
    
    printf("Reading...\n");
    int results[3];
    int ok = fscanf(reader,"%d %d %d", results, results + 1, results + 2);
    printf("%d values parsed: %d %d %d\n", ok, results[0], results[1], results[2]);
    
    return 0;
}
```

Vấn đề với việc sử dụng pipe theo cách này là ghi vào pipe có thể bị chặn, tức là pipe chỉ có dung lượng bộ đệm hạn chế. Nếu pipe đầy, tiến trình ghi sẽ bị chặn! Kích thước tối đa của bộ đệm phụ thuộc vào hệ thống; các giá trị điển hình từ 4KB đến 128KB.

```C
int main() {
    int fh[2];
    pipe(fh);
    int b = 0;
    #define MESG "..............................."
    while(1) {
        printf("%d\n",b);
        write(fh[1], MESG, sizeof(MESG))
        b+=sizeof(MESG);
    }
    return 0;
}
```

Xem [[Pipes, Phần 2: Bí mật lập trình Pipe]]


