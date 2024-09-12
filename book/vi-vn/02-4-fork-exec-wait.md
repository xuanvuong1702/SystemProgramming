# Mô hình

## Ví dụ 'exec' sau đây làm gì?
```C
#include <unistd.h>
#include <fcntl.h> // O_CREAT, O_APPEND etc. được định nghĩa ở đây

int main() {
    close(1); // đóng standard out
    open("log.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    puts("Captain's log");
    chdir("/usr/include");
    // execl( executable,  arguments for executable including program name and NULL at the end)

    execl("/bin/ls", /* Các mục còn lại được gửi đến ls*/ "/bin/ls", ".", (char *) NULL); // "ls ."
    perror("exec failed");
    return 0; // Không mong đợi
}
```
## Lỗi tinh vi của forkbomb

Có gì sai với đoạn mã này

```C
#include <unistd.h>
#define HELLO_NUMBER 10

int main(){
    pid_t children[HELLO_NUMBER];
    int i;
    for(i = 0; i < HELLO_NUMBER; i++){
        pid_t child = fork();
        if(child == -1){
            break;
        }
        if(child == 0){ // Tôi là quy trình con
             execlp("ehco", "echo", "hello", NULL);
        }
        else{
            children[i] = child;
        }
    }

    int j;
    for(j = 0; j < i; j++){
        waitpid(children[j], NULL, 0);
    }
    return 0;
}
```

Chúng tôi đã viết sai `ehco`, vì vậy chúng tôi không thể `exec` nó. Điều này có nghĩa là gì? Thay vì tạo ra 10 quy trình, chúng tôi đã tạo ra 2**10 quy trình, làm bom máy tính của chúng tôi bằng cách fork. Làm thế nào chúng tôi có thể ngăn chặn điều này? Đặt một lệnh thoát ngay sau exec để trong trường hợp exec thất bại, chúng tôi sẽ không kết thúc việc bom máy tính của chúng tôi bằng cách fork.

## Quy trình con thừa hưởng gì từ quy trình cha?
* Các tay cầm tệp mở. Nếu sau này cha tìm kiếm, ví dụ, về phía sau đến đầu tệp thì điều này sẽ ảnh hưởng đến quy trình con (và ngược lại).
* Trình xử lý tín hiệu
* Thư mục làm việc hiện tại
* Biến môi trường

Xem [trang man của fork](http://linux.die.net/man/2/fork) để biết thêm chi tiết.

## Điều gì khác biệt trong quy trình con so với quy trình cha?
ID quy trình là khác nhau. Trong quy trình con, gọi `getppid()` (chú ý có hai chữ 'p') sẽ cho kết quả giống như gọi getpid() trong quy trình cha. Xem trang man của fork để biết thêm chi tiết.

## Làm thế nào tôi chờ quy trình con hoàn thành?
Sử dụng `waitpid` hoặc `wait`. Quy trình cha sẽ tạm dừng cho đến khi `wait` (hoặc `waitpid`) trả về. Lưu ý rằng giải thích này bỏ qua cuộc thảo luận về việc khởi động lại.

## Mô hình fork-exec-wait là gì

Một mô hình lập trình phổ biến là gọi `fork` theo sau là `exec` và `wait`. Quy trình gốc gọi fork, tạo ra một quy trình con. Quy trình con sau đó sử dụng exec để bắt đầu thực thi một chương trình mới. Trong khi đó, quy trình cha sử dụng `wait` (hoặc `waitpid`) để chờ quy trình con hoàn thành.
Xem dưới đây để biết một ví dụ mã hoàn chỉnh.

## Làm thế nào tôi bắt đầu một quy trình nền chạy cùng một lúc?
Đừng chờ chúng! Quy trình cha của bạn có thể tiếp tục thực thi mã mà không cần phải chờ quy trình con. Lưu ý rằng trong thực tế, các quy trình nền cũng có thể được ngắt kết nối khỏi các luồng nhập và xuất của quy trình cha bằng cách gọi `close` trên các tay cầm tệp mở trước khi gọi exec.

Tuy nhiên, các quy trình con hoàn thành trước khi quy trình cha hoàn thành có thể trở thành zombies. Xem trang zombie để biết thêm thông tin.

# Zombies

## Cha mẹ tốt không để con cái của họ trở thành zombies!
Lưu ý, từ 'zombie' trong trường hợp này giúp làm sáng tỏ về những gì chúng thực sự đại diện. Khi một quy trình con hoàn thành (hoặc kết thúc), nó vẫn chiếm một vị trí trong bảng quy trình kernel. Hơn nữa, chúng vẫn chứa thông tin về quy trình đã bị kết thúc, như id quy trình, trạng thái thoát, v.v. (tức là một bộ khung của quy trình gốc vẫn còn).

Chỉ khi quy trình con đã được 'đợi' thì vị trí sẽ trở nên khả dụng và thông tin còn lại có thể được truy cập bởi quy trình cha.

Một chương trình chạy lâu có thể tạo ra nhiều zombies bằng cách liên tục tạo quy trình và không bao giờ `wait` cho chúng.

## Hiệu ứng của quá nhiều zombies sẽ là gì?

Cuối cùng sẽ không đủ không gian trong bảng quy trình kernel để tạo ra quy trình mới. Do đó, `fork()` sẽ thất bại và có thể khiến hệ thống khó / không thể sử dụng - ví dụ: chỉ việc đăng nhập cũng yêu cầu một quy trình mới!

## Hệ thống làm gì để ngăn chặn zombies?
Một khi quy trình hoàn thành, bất kỳ quy trình con nào của nó sẽ được gán cho "init" - quy trình đầu tiên với pid là 1. Do đó, những quy trình con này sẽ thấy getppid() trả về giá trị 1. Những đứa trẻ mồ côi này sẽ cuối cùng hoàn thành và trong một thời gian ngắn trở thành zombie. May mắn thay, quy trình init tự động đợi tất cả quy trình con của nó, do đó loại bỏ những zombies này khỏi hệ thống.

## Làm thế nào tôi ngăn chặn zombies? (Cảnh báo: Câu trả lời đơn giản)
Đợi quy trình con của bạn!
```C
waitpid(child, &status, 0); // Dọn dẹp và đợi quy trình con của tôi hoàn thành.
```
Lưu ý rằng chúng tôi giả định rằng lý do duy nhất để nhận được sự kiện SIGCHLD là một quy trình con đã hoàn thành (điều này không hoàn toàn đúng - xem trang man để biết thêm chi tiết).

Một cách thực hiện mạnh mẽ hơn sẽ kiểm tra trạng thái bị gián đoạn và bao gồm phần trên trong một vòng lặp.
Đọc tiếp để thảo luận về cách thực hiện mạnh mẽ hơn.

## Làm thế nào tôi có thể chờ đợi quy trình con của tôi một cách bất đồng bộ bằng cách sử dụng SIGCHLD? (NÂNG CAO)

Cảnh báo: Phần này sử dụng tín hiệu mà chúng tôi chưa giới thiệu đầy đủ.
Quy trình cha nhận tín hiệu SIGCHLD khi một quy trình con hoàn thành, vì vậy trình xử lý tín hiệu có thể chờ quy trình. Một phiên bản hơi đơn giản hơn được hiển thị dưới đây.
```C
pid_t child;

void cleanup(int signal) {
  int status;
  waitpid(child, &status, 0);
  write(1,"cleanup!\n",9);
}
int main() {
    // Đăng ký trình xử lý tín hiệu TRƯỚC khi quy trình con có thể hoàn thành
    signal(SIGCHLD, cleanup); // hoặc tốt hơn - sigaction
    child = fork();
    if (child == -1) { exit(EXIT_FAILURE);}

    if (child == 0) { /* Tôi là quy trình con!*/
      // Thực hiện các công việc nền, ví dụ: gọi exec   
    } else { /* Tôi là quy trình cha! */
        sleep(4); // để chúng ta có thể thấy việc dọn dẹp
        puts("Quy trình cha đã hoàn thành");
    }
    return 0;
} 
```

Tuy nhiên, ví dụ trên đã bỏ qua một số điểm tinh vi:
* Có thể có nhiều hơn một quy trình con đã hoàn thành nhưng quy trình cha chỉ nhận được một tín hiệu SIGCHLD (tín hiệu không được xếp hàng)
* Tín hiệu SIGCHLD có thể được gửi vì các lý do khác (ví dụ: một quy trình con tạm thời bị dừng)

Một đoạn mã mạnh mẽ hơn để thu dọn zombies được hiển thị dưới đây.
```C
void cleanup(int signal) {
    int status;
    while (waitpid((pid_t) (-1), 0, WNOHANG) > 0) {}
}
```

## Vậy biến môi trường là gì?

Biến môi trường là các biến mà hệ thống giữ cho tất cả các quy trình sử dụng. Hệ thống của bạn đã thiết lập những biến này ngay bây giờ! Trong Bash, bạn có thể kiểm tra một số biến này


```
$ echo $HOME
/home/user
$ echo $PATH
/usr/local/sbin:/usr/bin:...
```

Làm thế nào để bạn lấy những biến này trong C/C++? Bạn có thể sử dụng hàm `getenv` và `setenv`

```C
char* home = getenv("HOME"); // Sẽ trả về /home/user
setenv("HOME", "/home/user", 1 /*đặt ghi đè thành true*/ );
```

## Vậy, những biến môi trường này có ý nghĩa gì đối với quy trình cha/con?

Mỗi quy trình nhận được từ điển riêng của nó về các biến môi trường mà được sao chép sang quy trình con. Nghĩa là, nếu quy trình cha thay đổi các biến môi trường của mình, nó sẽ không được chuyển sang quy trình con và ngược lại. Điều này quan trọng trong bộ ba fork-exec-wait nếu bạn muốn thực thi một chương trình với các biến môi trường khác so với quy trình cha của bạn (hoặc bất kỳ quy trình nào khác).

Ví dụ, bạn có thể viết một chương trình C lặp qua tất cả các múi giờ và thực thi lệnh `date` để in ra ngày và giờ ở tất cả các địa phương. Các biến môi trường được sử dụng cho tất cả các loại chương trình nên việc chỉnh sửa chúng là quan trọng.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-1:-Introduction">
Trở lại: Forking, Phần 1: Giới thiệu
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals">
Tiếp theo: Kiểm soát Quy trình, Phần 1: Wait macros, sử dụng tín hiệu
</a>
</div>