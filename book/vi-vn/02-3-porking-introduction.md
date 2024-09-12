## Một lời cảnh báo ##
Tạo quy trình con là một công cụ rất mạnh mẽ (và rất nguy hiểm). Nếu bạn mắc lỗi và gây ra một quả bom tạo quy trình con (sẽ giải thích sau trong trang này), **bạn có thể làm sụp đổ toàn bộ hệ thống**. Để giảm khả năng này, hãy giới hạn số lượng quy trình tối đa của bạn ở một số nhỏ, ví dụ 40 bằng cách gõ ```ulimit -u 40``` vào dòng lệnh. Lưu ý rằng giới hạn này chỉ dành cho người dùng, có nghĩa là nếu bạn tạo bom quy trình con, thì bạn sẽ không thể giết tất cả các quy trình bạn vừa tạo vì gọi ```killall``` yêu cầu shell của bạn tạo quy trình con()... [mỉa mai](https://imgur.com/gallery/wefaH) phải không? Vậy chúng ta có thể làm gì về điều này. Một giải pháp là tạo một thể hiện shell khác dưới dạng người dùng khác (ví dụ: root) trước và giết các quy trình từ đó. Một cách khác là sử dụng lệnh ```exec``` tích hợp để giết tất cả các quy trình của người dùng (hãy cẩn thận vì bạn chỉ có một cú sút duy nhất). Cuối cùng, bạn có thể khởi động lại hệ thống :)

Khi kiểm tra mã tạo quy trình con(), hãy đảm bảo rằng bạn có quyền root và/hoặc quyền truy cập vật lý vào máy đang liên quan. Nếu bạn phải làm việc với mã tạo quy trình con() từ xa, hãy nhớ rằng **kill -9 -1** sẽ cứu bạn trong trường hợp khẩn cấp.

TL;DR: Tạo quy trình con có thể **cực kỳ** nguy hiểm nếu bạn không chuẩn bị cho nó. **Bạn đã được cảnh báo.**

# Giới thiệu về Fork

## Fork làm gì?

Lệnh hệ thống `fork` sao chép quy trình hiện tại để tạo một quy trình mới. Nó tạo một quy trình mới (quy trình con) bằng cách sao chép trạng thái của quy trình hiện tại với một số khác biệt nhỏ (được thảo luận dưới đây). Quy trình con không bắt đầu từ main. Thay vào đó, nó trả về từ `fork()` giống như quy trình cha.

Chỉ là một nhận xét nhỏ, trong các hệ thống UNIX cũ, không gian địa chỉ toàn bộ của quy trình cha được sao chép trực tiếp (bất kể tài nguyên có được sửa đổi hay không). Ngày nay, kernel thực hiện [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write), giúp tiết kiệm rất nhiều tài nguyên, đồng thời rất hiệu quả về thời gian.
## Ví dụ đơn giản nhất về `fork()` là gì?
Dưới đây là một ví dụ rất đơn giản...
```C
printf("Tôi chỉ được in ra một lần!\n");
fork();
// Bây giờ có hai quy trình đang chạy
// và mỗi quy trình sẽ in ra dòng tiếp theo.
printf("Bạn sẽ thấy dòng này hai lần!\n");
```

## Tại sao ví dụ này in ra số 42 hai lần?
Chương trình sau đây in ra số 42 hai lần - nhưng `fork()` lại nằm sau `printf`!? Tại sao?
```C
#include <unistd.h> /*fork được khai báo ở đây*/
#include <stdio.h> /* printf được khai báo ở đây*/
int main() {
    int answer = 84 >> 1;
    printf("Answer: %d", answer);
    fork();
    return 0;
}
```
Dòng `printf` _được_ thực thi chỉ một lần tuy nhiên hãy chú ý rằng nội dung được in không được đẩy ra chuẩn ra (không có dòng mới được in, chúng ta không gọi `fflush`, hoặc thay đổi chế độ đệm).
Vì vậy, văn bản đầu ra vẫn còn trong bộ nhớ quy trình đang chờ để được gửi.
Khi `fork()` được thực thi, toàn bộ bộ nhớ quy trình được nhân đôi bao gồm cả bộ đệm. Do đó, quy trình con bắt đầu với một bộ đệm đầu ra không trống, sẽ được đẩy khi chương trình thoát.

## Làm thế nào để viết mã khác nhau cho quy trình cha và quy trình con?

Kiểm tra giá trị trả về của `fork()`.

Nếu fork() trả về -1, điều đó ngụ ý rằng có điều gì đó không ổn trong quá trình tạo một quy trình con mới. Người ta nên kiểm tra giá trị được lưu trong _errno_ để xác định loại lỗi nào đã xảy ra; những lỗi phổ biến bao gồm EAGAIN và ENOMEM (kiểm tra [trang này](http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html) để có mô tả về các lỗi).

Tương tự, giá trị trả về 0 cho biết chúng ta đang ở trong quy trình con, trong khi một số nguyên dương cho thấy chúng ta đang ở trong quy trình cha. Giá trị dương được trả về bởi fork() cho chúng ta biết id quy trình (_pid_) của quy trình con.

Dưới đây là một cách để nhớ cái nào là cái nào:

Quy trình con có thể tìm thấy quy trình cha của mình - quy trình gốc đã được nhân đôi - bằng cách gọi `getppid()` - vì vậy không cần thêm thông tin trả về từ `fork()`. Tuy nhiên, quy trình cha chỉ có thể tìm ra id của quy trình con mới từ giá trị trả về của `fork`:
```C
pid_t id = fork();
if (id == -1) exit(1); // fork thất bại
if (id > 0)
{ 
// Tôi là quy trình cha gốc và
// Tôi vừa tạo một quy trình con với id 'id'
// Sử dụng waitpid để chờ quy trình con hoàn thành
} else { // trả về zero
// Tôi phải là quy trình con mới tạo
}
```

## Fork bomb là gì?
'Fork bomb' là khi bạn cố gắng tạo ra một số lượng vô hạn các quy trình. Một ví dụ đơn giản được hiển thị dưới đây:
```C
while (1) fork();
```
Điều này thường khiến hệ thống gần như tê liệt khi nó cố gắng phân bổ thời gian CPU và bộ nhớ cho một số lượng rất lớn các quy trình sẵn sàng để chạy. Nhận xét: Các quản trị viên hệ thống không thích fork-bombs và có thể đặt giới hạn trên cho số lượng quy trình mà mỗi người dùng có thể có hoặc có thể thu hồi quyền đăng nhập vì nó tạo ra sự xáo lộn trong lực lượng cho các chương trình của người dùng khác. Bạn cũng có thể giới hạn số lượng quy trình con được tạo bằng cách sử dụng `setrlimit()`.

Fork bombs không nhất thiết phải là độc hại - chúng đôi khi xảy ra do lỗi mã hóa của sinh viên.

Angrave đề xuất rằng bộ ba phim Matrix, nơi máy móc và con người cuối cùng làm việc cùng nhau để đánh bại Agent-Smith đang nhân lên, là một cốt truyện điện ảnh dựa trên một fork-bomb do AI điều khiển.

# Chờ và Thực thi

## Quy trình cha làm thế nào để chờ quy trình con hoàn thành?
Sử dụng `waitpid` (hoặc `wait`).

```C
pid_t child_id = fork();
if (child_id == -1) { perror("fork"); exit(EXIT_FAILURE);}
if (child_id > 0) { 
    // Chúng ta có một quy trình con! Lấy mã thoát của họ
    int status; 
    waitpid( child_id, &status, 0 );
    // mã không được hiển thị để lấy trạng thái thoát từ quy trình con
} else { // Trong quy trình con ...
    // bắt đầu tính toán
    exit(123);
}
```

## Tôi có thể làm cho quy trình con thực thi chương trình khác không?
Có. Sử dụng một trong các hàm [`exec`](http://man7.org/linux/man-pages/man3/exec.3.html) sau khi fork. Tập hàm `exec` thay thế hình ảnh quy trình bằng hình ảnh quy trình của thứ đang được gọi. Điều này có nghĩa là bất kỳ dòng mã nào sau lệnh `exec` đều bị thay thế. Bất kỳ công việc nào bạn muốn quy trình con thực hiện nên được thực hiện trước lệnh `exec`.

[Bài viết Wikipedia](https://en.wikipedia.org/wiki/Exec_(system_call)#C_language_prototypes) giúp bạn hiểu rõ hơn về tên của gia đình exec.

Các lược đồ đặt tên có thể được rút ngắn như sau

> Cơ sở của mỗi cái là exec (thực thi), theo sau là một hoặc nhiều chữ cái:
>
> e – Một mảng con trỏ đến các biến môi trường được truyền một cách rõ ràng đến hình ảnh quy trình mới.
>
> l – Các đối số dòng lệnh được truyền riêng lẻ (một danh sách) đến hàm.
>
> p – Sử dụng biến môi trường PATH để tìm tệp được đặt tên trong đối số tệp để thực thi.
>
> v – Các đối số dòng lệnh được truyền đến hàm dưới dạng một mảng (vector) con trỏ.

```C
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char**argv) {
    pid_t child = fork();
    if (child == -1) return EXIT_FAILURE;
    if (child) { /* Tôi có một quy trình con! */
        int status;
        waitpid(child , &status ,0);
        return EXIT_SUCCESS;

    } else { /* Tôi là quy trình con */
        // Các phiên bản khác của exec truyền đối số dưới dạng mảng
        // Nhớ rằng đối số đầu tiên là tên chương trình
        // Đối số cuối cùng phải là con trỏ char đến NULL

        execl("/bin/ls", "ls","-alh", (char *) NULL);

        // Nếu chúng ta đến dòng này, có điều gì đó không ổn!
        perror("exec failed!");
    }
}
```
## Một cách đơn giản hơn để thực thi một chương trình khác
Sử dụng `system`. Dưới đây là cách sử dụng nó:
```C
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char**argv) {
    system("ls");
    return 0;
}
```
Lệnh `system` sẽ fork, thực thi lệnh được truyền qua tham số và quy trình cha gốc sẽ chờ cho đến khi nó hoàn thành. Điều này cũng có nghĩa là `system` là một lệnh chặn: Quy trình cha không thể tiếp tục cho đến khi quy trình được bắt đầu bởi `system` thoát. Điều này có thể hữu ích hoặc không. Ngoài ra, `system` thực sự tạo ra một shell mà sau đó được cung cấp chuỗi, đây là nhiều overhead hơn so với việc chỉ sử dụng `exec` trực tiếp. Shell chuẩn sẽ sử dụng biến môi trường `PATH` để tìm kiếm một tên tệp phù hợp với lệnh. Sử dụng system thường đủ cho nhiều vấn đề đơn giản nhưng có thể trở nên hạn chế cho các vấn đề phức tạp hơn, và nó che giấu cơ chế của mô hình fork-exec-wait nên chúng tôi khuyến khích bạn học và sử dụng `fork` `exec` và `waitpid` thay thế.

## Ví dụ về fork ngớ ngẩn nhất là gì?
Một ví dụ hơi ngớ ngẩn được hiển thị dưới đây. Nó sẽ in gì? Hãy thử với nhiều đối số cho chương trình của bạn.
```C
#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv) {
  pid_t id;
  int status; 
  while (--argc && (id=fork())) {
    waitpid(id,&status,0); /* Wait for child*/
  }
  printf("%d:%s\n", argc, argv[argc]);
  return 0;
}
```

Thuật toán sắp xếp _sleepsort_ song song tuyệt vời với độ phức tạp thời gian hiển nhiên là O(N) là người chiến thắng ngớ ngẩn hôm nay. Đầu tiên được công bố trên [4chan vào năm 2011](https://dis.4chan.org/read/prog/1295544154). Một phiên bản của thuật toán sắp xếp kinh khủng nhưng thú vị này được hiển thị dưới đây.
```C
int main(int c, char **v)
{
    while (--c > 1 && !fork());
    int val  = atoi(v[c]);
    sleep(val);
    printf("%d\n", val);
    return 0;
}
```

Lưu ý: Thuật toán thực sự không phải là O(N) vì cách hoạt động của bộ lập lịch hệ thống. Mặc dù có các thuật toán song song chạy trong O(log(N)) cho mỗi quy trình, đáng tiếc đây không phải là một trong số đó.

## Điều gì khác biệt trong quy trình con so với quy trình cha?
Các khác biệt chính bao gồm:
* ID quy trình được trả về bởi `getpid()`. ID quy trình cha được trả về bởi `getppid()`.
* Cha được thông báo qua một tín hiệu, SIGCHLD, khi quy trình con kết thúc nhưng không ngược lại.
* Quy trình con không kế thừa các tín hiệu đang chờ hoặc báo thức hẹn giờ.
Để xem danh sách đầy đủ, hãy xem [trang man của fork](http://man7.org/linux/man-pages/man2/fork.2.html)

# Các quy trình con có chia sẻ các filehandle mở không?
Có! Thực tế cả hai quy trình đều sử dụng cùng một bộ mô tả tệp nhân hạt. Ví dụ, nếu một quy trình rebob lại vị trí truy cập ngẫu nhiên về đầu tệp, thì cả hai quy trình đều bị ảnh hưởng.

Cả quy trình con và cha nên `close` (hoặc `fclose`) các bộ mô tả tệp hoặc tay cầm tệp tương ứng của họ.

## Làm thế nào tôi có thể tìm hiểu thêm?
Đọc các trang man!
* [fork](http://man7.org/linux/man-pages/man2/fork.2.html)
* [exec](http://man7.org/linux/man-pages/man3/exec.3.html)
* [wait](http://man7.org/linux/man-pages/man2/wait.2.html)

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction">
Quay lại: Quy trình, Phần 1: Giới thiệu
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait">
Tiếp theo: Forking, Phần 2: Fork, Exec, Wait
</a>
</div>