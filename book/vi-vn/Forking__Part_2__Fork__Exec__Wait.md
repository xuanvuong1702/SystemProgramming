# Mẫu

## Ví dụ 'exec' sau đây làm gì?
```C
#include <unistd.h>
#include <fcntl.h> // O_CREAT, O_APPEND etc. được định nghĩa ở đây

int main() {
   close(1); // đóng đầu ra tiêu chuẩn
   open("log.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
   puts("Nhật ký của thuyền trưởng");
   chdir("/usr/include");
   // execl( tệp thực thi,  đối số cho tệp thực thi bao gồm tên chương trình và NULL ở cuối)

   execl("/bin/ls", /* Các mục còn lại được gửi đến ls*/ "/bin/ls", ".", (char *) NULL); // "ls ."
   perror("exec thất bại");
   return 0; // Không mong đợi
}
```
Không có kiểm tra lỗi trong mã trên (chúng tôi giả sử `close`, `open`, `chdir` etc hoạt động như mong đợi).
* `open`: sẽ sử dụng **bộ mô tả tệp** khả dụng thấp nhất (tức là 1); vì vậy **đầu ra tiêu chuẩn** bây giờ sẽ chuyển đến tệp nhật ký.
* `chdir`: Thay đổi **thư mục làm việc hiện tại** thành `/usr/include`
* `execl`: Thay thế **hình ảnh tiến trình** bằng `/bin/ls` và gọi phương thức `main()` của nó
* `perror`: Chúng tôi không mong đợi sẽ đến đây - nếu chúng tôi làm vậy thì `exec` đã thất bại.

## Lỗi bom fork tinh tế

Điều gì sai với mã này

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
        if(child == 0){ //Tôi là tiến trình con
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

Chúng ta đã viết sai chính tả `ehco`, vì vậy chúng ta không thể `exec` nó. Điều này có nghĩa là gì? Thay vì tạo 10 tiến trình, chúng ta chỉ tạo ra 2^10 tiến trình, **bom fork** máy của chúng ta. Làm thế nào chúng ta có thể ngăn chặn điều này? Thêm một lệnh `exit` ngay sau `exec` để trong trường hợp `exec` thất bại, chúng ta sẽ không kết thúc việc bom fork máy của chúng ta.

## Tiến trình con kế thừa những gì từ tiến trình cha?
* **Bộ xử lý tệp** đang mở. Nếu tiến trình cha sau đó tìm kiếm, chẳng hạn, trở lại đầu tệp thì điều này cũng sẽ ảnh hưởng đến tiến trình con (và ngược lại). 
* **Trình xử lý tín hiệu**
* **Thư mục làm việc hiện tại**
* **Biến môi trường**

Xem [trang hướng dẫn sử dụng fork](http://linux.die.net/man/2/fork) để biết thêm chi tiết.

## Điều gì khác biệt trong tiến trình con so với tiến trình cha?
**ID tiến trình** là khác nhau. Trong tiến trình con, việc gọi `getppid()` (lưu ý hai chữ 'p') sẽ cho kết quả giống như việc gọi `getpid()` trong tiến trình cha. Xem trang hướng dẫn sử dụng fork để biết thêm chi tiết.

## Làm cách nào để tôi đợi tiến trình con của tôi kết thúc?
Sử dụng `waitpid` hoặc `wait`. Tiến trình cha sẽ **tạm dừng** cho đến khi `wait` (hoặc `waitpid`) trả về. Lưu ý giải thích này bỏ qua cuộc thảo luận về việc khởi động lại.

## Mẫu fork-exec-wait là gì

Một mẫu lập trình phổ biến là gọi `fork` theo sau là `exec` và `wait`. Tiến trình ban đầu gọi fork, tạo ra một tiến trình con. Tiến trình con sau đó sử dụng `exec` để bắt đầu thực thi một chương trình mới. Trong khi đó, tiến trình cha sử dụng `wait` (hoặc `waitpid`) để đợi tiến trình con kết thúc.
Xem bên dưới để biết ví dụ mã đầy đủ.


## Làm cách nào để bắt đầu một tiến trình nền chạy cùng lúc?
Đừng đợi họ! Tiến trình cha của bạn có thể tiếp tục thực thi mã mà không cần phải đợi tiến trình con. Lưu ý trong thực tế, các **tiến trình nền** cũng có thể bị ngắt kết nối khỏi các luồng đầu vào và đầu ra của tiến trình cha bằng cách gọi `close` trên các **bộ mô tả tệp** đang mở trước khi gọi `exec`.

Tuy nhiên, các tiến trình con kết thúc trước khi tiến trình cha kết thúc có thể trở thành **zombie**. Xem trang zombie để biết thêm thông tin.

# Zombie

## Cha mẹ tốt không để con mình trở thành zombie!
Lưu ý, từ 'zombie' trong trường hợp này cho thấy một số điều sáng tỏ về những gì chúng thực sự đại diện. Khi một tiến trình con kết thúc (hoặc **chấm dứt**), nó vẫn chiếm một vị trí trong **bảng tiến trình kernel**. Hơn nữa, chúng vẫn chứa thông tin về tiến trình đã bị chấm dứt, chẳng hạn như id tiến trình, **trạng thái thoát**, v.v. (tức là một bộ xương của tiến trình ban đầu vẫn còn).
Chỉ khi tiến trình con đã được 'chờ đợi', vị trí đó mới khả dụng và thông tin còn lại có thể được truy cập bởi tiến trình cha.

Một chương trình chạy lâu có thể tạo ra nhiều zombie bằng cách liên tục tạo các tiến trình và không bao giờ `wait`-ing cho chúng.

## Tác động của quá nhiều zombie là gì?

Cuối cùng, sẽ không có đủ dung lượng trong bảng tiến trình kernel để tạo các tiến trình mới. Do đó `fork()` sẽ thất bại và có thể khiến hệ thống khó / không thể sử dụng - ví dụ: chỉ cần đăng nhập yêu cầu một tiến trình mới!

## Hệ thống làm gì để giúp ngăn chặn zombie?
Khi một tiến trình hoàn thành, bất kỳ tiến trình con nào của nó sẽ được gán cho "init" - tiến trình đầu tiên có **pid** là 1. Do đó, những tiến trình con này sẽ thấy `getppid()` trả về giá trị 1. Những **tiến trình mồ côi** này cuối cùng sẽ kết thúc và trong một thời gian ngắn trở thành zombie. May mắn thay, tiến trình init tự động chờ tất cả các tiến trình con của nó, do đó loại bỏ những zombie này khỏi hệ thống.

## Làm cách nào để ngăn chặn zombie? (Cảnh báo: Câu trả lời đơn giản hóa)
Chờ đợi con của bạn!
```C
waitpid(child, &status, 0); // Dọn dẹp và đợi tiến trình con của tôi kết thúc.
```
Lưu ý rằng chúng tôi giả định rằng lý do duy nhất để nhận được sự kiện SIGCHLD là một tiến trình con đã kết thúc (điều này không hoàn toàn đúng - xem trang hướng dẫn sử dụng để biết thêm chi tiết).

Một triển khai mạnh mẽ cũng sẽ kiểm tra **trạng thái bị gián đoạn** và bao gồm điều kiện trên trong một vòng lặp.
Đọc tiếp để thảo luận về việc triển khai mạnh mẽ hơn.

## Làm cách nào để tôi có thể đợi tiến trình con của mình một cách không đồng bộ bằng cách sử dụng SIGCHLD? (NÂNG CAO)

Cảnh báo: Phần này sử dụng các tín hiệu mà chúng tôi chưa giới thiệu đầy đủ.
Tiến trình cha nhận được tín hiệu SIGCHLD khi tiến trình con hoàn thành, vì vậy **trình xử lý tín hiệu** có thể đợi trên tiến trình. Một phiên bản hơi đơn giản hóa được hiển thị bên dưới.
```C
pid_t child;

void cleanup(int signal) {
  int status;
  waitpid(child, &status, 0);
  write(1,"cleanup!\n",9);
}
int main() {
   // Đăng ký trình xử lý tín hiệu TRƯỚC KHI tiến trình con có thể kết thúc
   signal(SIGCHLD, cleanup); // hoặc tốt hơn - sigaction
   child = fork();
   if (child == -1) { exit(EXIT_FAILURE);}

   if (child == 0) { /* Tôi là đứa con!*/
     // Làm những việc nền, ví dụ: gọi exec   
   } else { /* Tôi là cha mẹ! */
      sleep(4); // để chúng ta có thể thấy phần dọn dẹp
      puts("Cha mẹ đã xong việc");
   }
   return 0;
} 
```

Tuy nhiên, ví dụ trên bỏ lỡ một vài điểm tinh tế:
* Nhiều hơn một tiến trình con có thể đã kết thúc nhưng tiến trình cha sẽ chỉ nhận được một tín hiệu SIGCHLD (các tín hiệu không được **xếp hàng**)
* Tín hiệu SIGCHLD có thể được gửi vì những lý do khác (ví dụ: một tiến trình con bị dừng tạm thời)

Một mã mạnh mẽ hơn để thu thập zombie được hiển thị bên dưới.
```C
void cleanup(int signal) {
  int status;
  while (waitpid((pid_t) (-1), &status, WNOHANG) > 0) {}
}
```

## Vậy biến môi trường là gì?

**Biến môi trường** là các biến mà hệ thống giữ cho tất cả các tiến trình sử dụng. Hệ thống của bạn đã thiết lập những thứ này ngay bây giờ! Trong Bash, bạn có thể kiểm tra một số trong số này

```
$ echo $HOME
/home/user
$ echo $PATH
/usr/local/sbin:/usr/bin:...
```

Làm thế nào để bạn nhận được những thứ này trong C/C++? Bạn có thể sử dụng hàm `getenv` và `setenv`

```C
char* home = getenv("HOME"); // Sẽ trả về /home/user
setenv("HOME", "/home/user", 1 /*đặt ghi đè thành true*/ );
```

## Đúng vậy, vậy những biến môi trường này có ý nghĩa gì đối với cha mẹ/con cái?

Chà, mỗi tiến trình nhận được **từ điển biến môi trường** riêng của nó được sao chép cho tiến trình con. Có nghĩa là, nếu tiến trình cha thay đổi các biến môi trường của nó, nó sẽ không được chuyển đến tiến trình con và ngược lại. Điều này rất quan trọng trong **bộ ba fork-exec-wait** nếu bạn muốn exec một chương trình với các biến môi trường khác với tiến trình cha của bạn (hoặc bất kỳ tiến trình nào khác).

Ví dụ: bạn có thể viết một chương trình C lặp qua tất cả các **múi giờ** và thực thi lệnh `date` để in ra ngày và giờ theo tất cả các địa phương. Các biến môi trường được sử dụng cho tất cả các loại chương trình, vì vậy việc sửa đổi chúng rất quan trọng.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-1:-Introduction">
Trở lại: Phân nhánh, Phần 1: Giới thiệu
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals">
Tiếp theo: Kiểm soát tiến trình, Phần 1: Macro đợi, sử dụng tín hiệu
</a>
</div>





