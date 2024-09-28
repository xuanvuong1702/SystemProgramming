# Tín hiệu chuyên sâu

## Làm cách nào để tìm hiểu thêm về tín hiệu?

Các trang hướng dẫn của Linux thảo luận về các lệnh gọi hệ thống tín hiệu trong phần 2. Ngoài ra còn có một bài viết dài hơn trong phần 7 (mặc dù không có trong OSX/BSD):
```
man -s7 signal
```

## Thuật ngữ tín hiệu
* **Generated (Đã tạo):** Tín hiệu đang được tạo trong kernel bằng lệnh gọi hệ thống kill.
* **Pending (Đang chờ xử lý):** Chưa được gửi nhưng sắp được gửi.
* **Blocked (Bị chặn):** Không được gửi vì không có cách xử lý tín hiệu nào cho phép tín hiệu được gửi.
* **Delivered (Đã gửi):** Đã gửi đến tiến trình, hành động được mô tả đang được thực hiện.
* **Caught (Đã bắt):** Khi tiến trình ngăn chặn tín hiệu phá hủy nó và thay vào đó làm điều gì đó khác với nó.

## Cách xử lý tín hiệu của một tiến trình là gì?
Đối với mỗi tiến trình, mỗi tín hiệu có một cách xử lý, có nghĩa là hành động nào sẽ xảy ra khi tín hiệu được gửi đến tiến trình. Ví dụ: cách xử lý mặc định của `SIGINT` là chấm dứt nó. Cách xử lý tín hiệu có thể được thay đổi bằng cách gọi `signal()` (đơn giản nhưng không di động vì có những biến thể tinh tế trong việc triển khai trên các kiến ​​trúc POSIX khác nhau và cũng không được khuyến nghị cho các chương trình đa luồng) hoặc `sigaction` (được thảo luận sau). Bạn có thể tưởng tượng cách xử lý của các tiến trình đối với tất cả các tín hiệu có thể có như một bảng các mục con trỏ hàm (một cho mỗi tín hiệu có thể có).

Cách xử lý mặc định cho tín hiệu có thể là bỏ qua tín hiệu, dừng tiến trình, tiếp tục tiến trình đã dừng, chấm dứt tiến trình hoặc chấm dứt tiến trình và cũng xuất ra một tệp 'core'. Lưu ý rằng tệp 'core' là một biểu diễn của trạng thái bộ nhớ của tiến trình có thể được kiểm tra bằng cách sử dụng trình gỡ lỗi.

## Nhiều tín hiệu có thể được xếp hàng đợi không?

Không - tuy nhiên có thể có các tín hiệu đang ở trạng thái chờ xử lý. Nếu một tín hiệu đang chờ xử lý, điều đó có nghĩa là nó chưa được gửi đến tiến trình. Lý do phổ biến nhất khiến tín hiệu bị chờ xử lý là tiến trình (hoặc luồng) hiện đã chặn tín hiệu cụ thể đó.

Nếu một tín hiệu cụ thể, ví dụ: `SIGINT`, đang chờ xử lý thì không thể xếp hàng đợi lại cùng một tín hiệu đó.

_Có thể_ có nhiều hơn một tín hiệu thuộc loại khác nhau ở trạng thái chờ xử lý. Ví dụ: tín hiệu `SIGINT` và `SIGTERM` có thể đang chờ xử lý (tức là chưa được gửi đến tiến trình đích).

## Làm cách nào để chặn tín hiệu?
Tín hiệu có thể bị chặn (có nghĩa là chúng sẽ ở trạng thái chờ xử lý) bằng cách đặt mặt nạ tín hiệu của tiến trình hoặc, khi bạn đang viết một chương trình đa luồng, mặt nạ tín hiệu của luồng.

# Cách xử lý trong Luồng/Tiến trình Con

## Điều gì xảy ra khi tạo một luồng mới?
Luồng mới kế thừa một bản sao của mặt nạ của luồng gọi.
```C
pthread_sigmask( ... ); // đặt mặt nạ của tôi để chặn việc gửi một số tín hiệu
pthread_create( ... ); // luồng mới sẽ bắt đầu với một bản sao của cùng một mặt nạ
```

## Điều gì xảy ra khi forking?

Tiến trình con kế thừa một bản sao của cách xử lý tín hiệu của tiến trình cha. Nói cách khác, nếu bạn đã cài đặt trình xử lý `SIGINT` trước khi forking, thì tiến trình con cũng sẽ gọi trình xử lý nếu `SIGINT` được gửi đến tiến trình con.

Lưu ý rằng các tín hiệu đang chờ xử lý cho tiến trình con _không_ được kế thừa trong quá trình forking.

## Điều gì xảy ra trong quá trình exec?
Cả mặt nạ tín hiệu và cách xử lý tín hiệu đều được chuyển sang chương trình được exec. [https://www.gnu.org/software/libc/manual/html_node/Executing-a-File.html#Executing-a-File](Nguồn) Các tín hiệu đang chờ xử lý cũng được giữ nguyên. **Các trình xử lý tín hiệu được đặt lại**, vì mã trình xử lý ban đầu đã biến mất cùng với tiến trình cũ.

## Làm cách nào để chặn tín hiệu trong một chương trình đơn luồng?
Sử dụng `sigprocmask`! Với `sigprocmask`, bạn có thể đặt mặt nạ mới, thêm các tín hiệu mới sẽ bị chặn vào mặt nạ tiến trình và bỏ chặn các tín hiệu hiện đang bị chặn. Bạn cũng có thể xác định mặt nạ hiện có (và sử dụng nó cho sau này) bằng cách chuyển một giá trị khác null cho `oldset`.

```C
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);`
```

Từ trang hướng dẫn Linux của `sigprocmask`:
```
SIG_BLOCK: Tập hợp các tín hiệu bị chặn là hợp của tập hợp hiện tại và tập hợp đối số.
SIG_UNBLOCK: Các tín hiệu trong tập hợp được xóa khỏi tập hợp các tín hiệu bị chặn hiện tại. Có thể thử bỏ chặn tín hiệu không bị chặn.
SIG_SETMASK: Tập hợp các tín hiệu bị chặn được đặt thành tập hợp đối số.
```
Kiểu `sigset` hoạt động như một bitmap, ngoại trừ các hàm được sử dụng thay vì đặt và bỏ đặt bit một cách rõ ràng bằng cách sử dụng `&` và `|`. 

Một lỗi phổ biến là quên khởi tạo tập hợp tín hiệu trước khi sửa đổi một bit. Ví dụ:
```C
sigset_t set, oldset;
sigaddset(&set, SIGINT); // Ôi!
sigprocmask(SIG_SETMASK, &set, &oldset)
```
Mã đúng khởi tạo tập hợp để tất cả bật hoặc tất cả tắt. Ví dụ:
```C
sigfillset(&set); // tất cả các tín hiệu
sigprocmask(SIG_SETMASK, &set, NULL); // Chặn tất cả các tín hiệu!
// (Thực ra SIGKILL hoặc SIGSTOP không thể bị chặn...)

sigemptyset(&set); // không có tín hiệu 
sigprocmask(SIG_SETMASK, &set, NULL); // đặt lại mặt nạ thành trống
```

## Làm cách nào để chặn tín hiệu trong một chương trình đa luồng?
Việc chặn tín hiệu trong các chương trình đa luồng tương tự như các chương trình đơn luồng:
* Sử dụng `pthread_sigmask` thay vì `sigprocmask`
* Chặn tín hiệu trong tất cả các luồng để ngăn việc gửi không đồng bộ

Phương pháp dễ nhất để đảm bảo tín hiệu bị chặn trong tất cả các luồng là đặt mặt nạ tín hiệu trong luồng chính trước khi các luồng mới được tạo.

```C
sigemptyset(&set);
sigaddset(&set, SIGQUIT);
sigaddset(&set, SIGINT);
pthread_sigmask(SIG_BLOCK, &set, NULL);

// luồng này và luồng mới sẽ chặn SIGQUIT và SIGINT
pthread_create(&thread_id, NULL, myfunc, funcparam);
```

Cũng giống như chúng ta đã thấy với `sigprocmask`, `pthread_sigmask` bao gồm một tham số 'how' xác định cách sử dụng tập hợp tín hiệu:
```C
pthread_sigmask(SIG_SETMASK, &set, NULL); // thay thế mặt nạ của luồng bằng tập hợp tín hiệu đã cho
pthread_sigmask(SIG_BLOCK, &set, NULL);   // thêm tập hợp tín hiệu vào mặt nạ của luồng
pthread_sigmask(SIG_UNBLOCK, &set, NULL); // xóa tập hợp tín hiệu khỏi mặt nạ của luồng
```

## Các tín hiệu đang chờ xử lý được gửi như thế nào trong một chương trình đa luồng?
Tín hiệu được gửi đến bất kỳ luồng nào **không chặn** tín hiệu đó.

Nếu hai hoặc nhiều luồng có thể nhận tín hiệu thì luồng nào sẽ bị gián đoạn là **ngẫu nhiên**!

