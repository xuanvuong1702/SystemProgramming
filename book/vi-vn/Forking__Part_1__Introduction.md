##  Đang thi công

## Một lời cảnh báo ##
Phân nhánh tiến trình (process forking) là một công cụ rất mạnh (và rất nguy hiểm). Nếu bạn làm rối và gây ra bom fork (giải thích sau trên trang này), **bạn có thể làm sập toàn bộ hệ thống**. Để giảm nguy cơ xảy ra điều này, hãy giới hạn số lượng tiến trình tối đa của bạn ở một số nhỏ, ví dụ:
 40 bằng cách gõ ```ulimit -u 40``` vào dòng lệnh. Lưu ý rằng giới hạn này chỉ dành cho người dùng, có nghĩa là nếu bạn fork bom, thì bạn sẽ không thể kill tất cả các tiến trình mà bạn vừa tạo vì việc gọi ```killall``` yêu cầu shell của bạn `fork()` ... [trớ trêu thay](https://imgur.com/gallery/wefaH) phải không? Vậy chúng ta có thể làm gì về điều này. Một giải pháp là tạo ra một phiên bản shell khác với tư cách là người dùng khác (ví dụ: root) trước và kill các tiến trình từ đó. Một cách khác là sử dụng lệnh `exec` tích hợp sẵn để kill tất cả các tiến trình của người dùng (cẩn thận, bạn chỉ có một lần thử). Cuối cùng, bạn có thể khởi động lại hệ thống :)

Khi kiểm tra mã `fork()`, hãy đảm bảo rằng bạn có quyền root và/hoặc quyền truy cập vật lý vào máy liên quan. Nếu bạn phải làm việc trên mã `fork()` từ xa, hãy nhớ rằng **kill -9 -1** sẽ cứu bạn trong trường hợp khẩn cấp.

**TL;DR:** Fork có thể **cực kỳ** nguy hiểm nếu bạn không chuẩn bị cho nó. **Bạn đã được cảnh báo.**

# Giới thiệu về Fork

## Fork làm gì?

Lệnh gọi hệ thống `fork` nhân bản tiến trình hiện tại để tạo một tiến trình mới. Nó tạo một tiến trình mới (tiến trình con) bằng cách sao chép trạng thái của tiến trình hiện có với một số khác biệt nhỏ (được thảo luận bên dưới). Tiến trình con không bắt đầu từ `main`. Thay vào đó, nó trả về từ `fork()` giống như tiến trình cha.

Như một nhận xét bên lề, trong các hệ thống UNIX cũ, toàn bộ không gian địa chỉ của tiến trình cha đã được sao chép trực tiếp (bất kể tài nguyên có bị sửa đổi hay không). Ngày nay, kernel thực hiện [copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write), giúp tiết kiệm rất nhiều tài nguyên, đồng thời rất hiệu quả về thời gian.

## Ví dụ `fork()` đơn giản nhất là gì?
Đây là một ví dụ rất đơn giản...
```C
printf("Tôi được in một lần!\n");
fork();
// Bây giờ có hai tiến trình đang chạy
// và mỗi tiến trình sẽ in ra dòng tiếp theo.
printf("Bạn thấy dòng này hai lần!\n");
```

## Tại sao ví dụ này lại in 42 hai lần?
Chương trình sau in ra 42 hai lần - nhưng `fork()` nằm sau `printf`!? Tại sao?
```C
#include <unistd.h> /*fork được khai báo ở đây*/
#include <stdio.h> /* printf được khai báo ở đây*/
int main() {
   int answer = 84 >> 1;
   printf("Đáp án: %d", answer);
   fork();
   return 0;
}
```
Dòng `printf` **được** thực thi chỉ một lần, tuy nhiên lưu ý rằng nội dung được in không được xả ra đầu ra tiêu chuẩn (không có dòng mới được in, chúng ta không gọi `fflush` hoặc thay đổi chế độ đệm).
Do đó, văn bản đầu ra vẫn đang chờ trong bộ nhớ tiến trình để được gửi đi.
Khi `fork()` được thực thi, toàn bộ bộ nhớ tiến trình được sao chép bao gồm cả bộ đệm. Do đó, tiến trình con bắt đầu với một bộ đệm đầu ra không trống, bộ đệm này sẽ được xả khi chương trình thoát.

## Làm thế nào để bạn viết mã khác nhau cho tiến trình cha và tiến trình con?

Kiểm tra giá trị trả về của `fork()`. 

Nếu `fork()` trả về -1, điều đó ngụ ý rằng có điều gì đó không ổn trong quá trình tạo một tiến trình con mới. Bạn nên kiểm tra giá trị được lưu trữ trong `errno` để xác định loại lỗi đã xảy ra; các lỗi thường gặp bao gồm `EAGAIN` và `ENOMEM` (kiểm tra [trang này](http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html) để có mô tả về các lỗi).

Tương tự, giá trị trả về là 0 cho biết rằng chúng ta đang ở trong tiến trình con, trong khi một số nguyên dương cho thấy rằng chúng ta đang ở trong tiến trình cha. Giá trị dương được trả về bởi `fork()` cho biết id tiến trình (PID) của tiến trình con.

Đây là một cách để ghi nhớ cái nào là cái nào:

Tiến trình con có thể tìm thấy tiến trình cha của nó - tiến trình ban đầu đã được sao chép - bằng cách gọi `getppid()`, do đó không cần bất kỳ thông tin trả về bổ sung nào từ `fork()`. Tuy nhiên, tiến trình cha chỉ có thể tìm ra id của tiến trình con mới từ giá trị trả về của `fork`:
```C
pid_t id = fork();
if (id == -1) exit(1); // fork thất bại 
if (id > 0)
{ 
// Tôi là tiến trình cha ban đầu và 
// Tôi vừa tạo một tiến trình con với id 'id'
// Sử dụng waitpid để đợi tiến trình con kết thúc
} else { // trả về 0
// Tôi phải là tiến trình con mới được tạo
}
```

## Bom fork là gì?
'Bom fork' là khi bạn cố gắng tạo một số lượng tiến trình vô hạn. Một ví dụ đơn giản được hiển thị bên dưới:
```C
while (1) fork();
```
Điều này thường sẽ khiến hệ thống gần như bị đình trệ khi nó cố gắng phân bổ thời gian CPU và bộ nhớ cho một số lượng rất lớn các tiến trình đã sẵn sàng để chạy. Nhận xét: Quản trị viên hệ thống không thích bom fork và có thể đặt giới hạn trên cho số lượng tiến trình mà mỗi người dùng có thể có hoặc có thể thu hồi quyền đăng nhập vì nó tạo ra sự xáo trộn tài nguyên cho các chương trình của người dùng khác. Bạn cũng có thể giới hạn số lượng tiến trình con được tạo bằng cách sử dụng `setrlimit()`.

Bom fork không nhất thiết phải là ác ý - đôi khi chúng xảy ra do lỗi mã hóa của sinh viên.

Angrave gợi ý rằng bộ ba phim Ma trận, nơi máy móc và con người cuối cùng hợp tác để đánh bại Agent-Smith đang nhân lên, là một cốt truyện điện ảnh dựa trên bom fork do AI điều khiển.

# Chờ đợi và Thực thi

## Tiến trình cha đợi tiến trình con kết thúc như thế nào?
Sử dụng `waitpid` (hoặc `wait`).

```C
pid_t child_id = fork();
if (child_id == -1) { perror("fork"); exit(EXIT_FAILURE);}
if (child_id > 0) { 
  // Chúng ta có một đứa con! Lấy mã thoát của nó
  int status; 
  waitpid( child_id, &status, 0 );
  // mã không được hiển thị để lấy trạng thái thoát từ ​​tiến trình con
} else { // Trong tiến trình con ...
  // bắt đầu tính toán
  exit(123);
}
```

## Tôi có thể làm cho tiến trình con thực thi một chương trình khác không?
Có. Sử dụng một trong các hàm [`exec`](http://man7.org/linux/man-pages/man3/exec.3.html) sau khi phân nhánh. Tập hợp các hàm `exec` thay thế hình ảnh tiến trình bằng hình ảnh tiến trình của những gì đang được gọi. Điều này có nghĩa là bất kỳ dòng mã nào sau lệnh gọi `exec` đều bị thay thế. Bất kỳ công việc nào khác mà bạn muốn tiến trình con thực hiện nên được thực hiện trước lệnh gọi `exec`.  

[Bài viết trên Wikipedia](https://en.wikipedia.org/wiki/Exec_(system_call)#C_language_prototypes) thực hiện một công việc tuyệt vời giúp bạn hiểu ý nghĩa của các tên trong họ exec.

Các lược đồ đặt tên có thể được rút ngắn như thế này:

> Cơ sở của mỗi cái là `exec` (thực thi), theo sau là một hoặc nhiều chữ cái:
>
> * `e`: Một mảng con trỏ đến các biến môi trường được truyền rõ ràng đến hình ảnh tiến trình mới.
> * `l`: Các đối số dòng lệnh được truyền riêng lẻ (một danh sách) cho hàm.
> * `p`: Sử dụng biến môi trường `PATH` để tìm tệp được đặt tên trong đối số tệp sẽ được thực thi.
> * `v`: Các đối số dòng lệnh được truyền cho hàm dưới dạng một mảng (vector) con trỏ.

```C
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char**argv) {
  pid_t child = fork();
  if (child == -1) return EXIT_FAILURE;
  if (child) { /* Tôi có một đứa con! */
    int status;
    waitpid(child , &status ,0);
    return EXIT_SUCCESS;

  } else { /* Tôi là đứa con */
    // Các phiên bản khác của exec truyền đối số dưới dạng mảng
    // Hãy nhớ rằng đối số đầu tiên là tên chương trình
    // Đối số cuối cùng phải là một con trỏ char trỏ đến NULL

    execl("/bin/ls", "ls","-alh", (char *) NULL);

    // Nếu chúng ta đến dòng này, có điều gì đó không ổn!
    perror("exec thất bại!");
  }
}
```
## Một cách đơn giản hơn để thực thi một chương trình khác
Sử dụng `system`. Đây là cách sử dụng nó:
```C

#include <unistd.h>
#include <stdlib.h>

int main(int argc, char**argv) {
  system("ls");
  return 0;
}
```
Lệnh gọi `system` sẽ fork, thực thi lệnh được truyền bởi tham số và tiến trình cha ban đầu sẽ đợi điều này kết thúc. Điều này cũng có nghĩa là `system` là một lệnh gọi chặn: Tiến trình cha không thể tiếp tục cho đến khi tiến trình được khởi động bởi `system` thoát. Điều này có thể hữu ích hoặc không. Ngoài ra, `system` thực sự tạo ra một shell, sau đó được cung cấp chuỗi, điều này có nhiều chi phí hơn so với việc chỉ sử dụng `exec` trực tiếp. Shell tiêu chuẩn sẽ sử dụng biến môi trường `PATH` để tìm kiếm một tên tệp khớp với lệnh. Sử dụng `system` thường sẽ đủ cho nhiều vấn đề "chạy-lệnh-này" đơn giản nhưng có thể nhanh chóng trở nên hạn chế đối với các vấn đề phức tạp hoặc tinh tế hơn, và nó ẩn đi cơ chế của mẫu fork-exec-wait, vì vậy chúng tôi khuyến khích bạn học và sử dụng `fork`, `exec` và `waitpid` thay thế.
 
## Ví dụ về fork ngớ ngẩn nhất là gì?
Một ví dụ hơi ngớ ngẩn được hiển thị bên dưới. Nó sẽ in gì? Hãy thử nó với nhiều đối số cho chương trình của bạn.
```C
#include <unistd.h>
#include <stdio.h>
int main(int argc, char **argv) {
  pid_t id;
  int status; 
  while (--argc && (id=fork())) {
    waitpid(id,&status,0); /* Chờ đợi tiến trình con*/
  }
  printf("%d:%s\n", argc, argv[argc]);
  return 0;
}
```

Thuật toán sắp xếp song song đáng kinh ngạc `sleepsort` với độ phức tạp O(N) rõ ràng là người chiến thắng ngớ ngẩn ngày nay. Lần đầu tiên được xuất bản trên [4chan vào năm 2011 ](https://dis.4chan.org/read/prog/1295544154). Một phiên bản của thuật toán sắp xếp khủng khiếp nhưng thú vị này được hiển thị bên dưới.
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

Lưu ý: Thuật toán không thực sự là O(N) vì cách bộ lập lịch hệ thống hoạt động. Mặc dù có các thuật toán song song chạy trong O(log(N)) trên mỗi tiến trình, nhưng thật đáng buồn đây không phải là một trong số đó.

## Điều gì khác biệt trong tiến trình con so với tiến trình cha?
Sự khác biệt chính bao gồm:
* ID tiến trình được trả về bởi `getpid()`. ID tiến trình cha được trả về bởi `getppid()`.
* Tiến trình cha được thông báo qua một tín hiệu, `SIGCHLD`, khi tiến trình con kết thúc nhưng không phải ngược lại.
* Tiến trình con không kế thừa các tín hiệu đang chờ xử lý hoặc báo thức hẹn giờ.
Để biết danh sách đầy đủ, hãy xem [trang hướng dẫn (`man page`) của `fork`](http://man7.org/linux/man-pages/man2/fork.2.html).

# Các tiến trình con có chia sẻ bộ xử lý tệp đang mở không?
Có! Trên thực tế, cả hai tiến trình đều sử dụng cùng một bộ mô tả tệp kernel cơ bản. Ví dụ: nếu một tiến trình tua lại vị trí truy cập ngẫu nhiên về đầu tệp, thì cả hai tiến trình đều bị ảnh hưởng.

Cả tiến trình con và tiến trình cha đều nên `close` (hoặc `fclose`) bộ mô tả tệp hoặc bộ xử lý tệp của chúng tương ứng.

## Làm cách nào để tôi tìm hiểu thêm?
Đọc các trang hướng dẫn sử dụng!
* [`fork`](http://man7.org/linux/man-pages/man2/fork.2.html)
* [`exec`](http://man7.org/linux/man-pages/man3/exec.3.html)
* [`wait`](http://man7.org/linux/man-pages/man2/wait.2.html)

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction">
Trở lại: Tiến trình, Phần 1: Giới thiệu
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-2%3A-Fork%2C-Exec%2C-Wait">
Tiếp theo: Phân nhánh, Phần 2: Fork, Exec, Wait
</a>
</div>



