# Kernel

**Kernel** của hệ điều hành là một phần mềm đặc biệt. Đây là phần mềm được tải lên trước khi tất cả các chương trình khác của bạn thậm chí cân nhắc việc **khởi động**. Việc kernel làm là như sau, được viết tắt

1. Hệ điều hành thực thi **ROM** hoặc mã chỉ đọc
2. Hệ điều hành sau đó thực thi một `boot_loader` hoặc tiện ích mở rộng `EFI` ngày nay
3. **Boot loader** tải kernel của bạn
4. Kernel của bạn thực thi `init` để [khởi động](https://en.wikipedia.org/wiki/Bootstrapping) chính nó từ không có gì
5. Kernel thực thi các **tập lệnh khởi động**
6. Kernel thực thi các tập lệnh **không gian người dùng** và bạn có thể sử dụng máy tính của mình!

Bạn không cần phải biết cụ thể về **quy trình khởi động**, nhưng nó ở đó. Khi bạn đang thực thi trong **không gian người dùng**, kernel cung cấp một số thao tác quan trọng mà các chương trình không phải lo lắng.
* **Lập lịch** các tiến trình và **luồng**; Ngoài ra, xử lý các **nguyên thủy đồng bộ hóa**
* Cung cấp các **lệnh gọi hệ thống** như `write` hoặc `read`
* Quản lý **bộ nhớ ảo** và các thiết bị nhị phân cấp thấp như trình điều khiển `usb`
* Xử lý việc đọc và hiểu **hệ thống tệp**
* Xử lý giao tiếp qua **mạng**
* Xử lý giao tiếp với các **tiến trình** khác
* **Liên kết động** các **thư viện**

Kernel xử lý tất cả những thứ này ở chế độ kernel. **Chế độ Kernel** mang lại cho bạn sức mạnh lớn hơn, như thực thi các lệnh CPU bổ sung nhưng với cái giá của một lỗi làm hỏng toàn bộ máy tính của bạn -- ouch. Đó là những gì bạn sẽ tương tác trong lớp này.

### Bộ mô tả tệp

Một trong những điều mà bạn đã quen thuộc là kernel cung cấp cho bạn các **bộ mô tả tệp** khi bạn mở các tệp văn bản. Đây là một **zine** từ Julia Evans mô tả chi tiết một chút về nó.

![Fds](https://drawings.jvns.ca/drawings/filedescriptors.jpeg)

Như zine nhỏ cho thấy, Kernel theo dõi các bộ mô tả tệp và những gì chúng trỏ đến. Chúng ta sẽ thấy sau này rằng các bộ mô tả tệp không cần phải trỏ đến các tệp thực tế và hệ điều hành sẽ theo dõi chúng cho bạn. Ngoài ra, hãy lưu ý rằng giữa các tiến trình, các bộ mô tả tệp có thể được sử dụng lại nhưng bên trong một tiến trình, chúng là duy nhất.

Bộ mô tả tệp cũng có khái niệm về **vị trí**. Bạn có thể đọc hoàn toàn một tệp trên đĩa vì hệ điều hành theo dõi vị trí trong tệp và điều đó cũng thuộc về tiến trình của bạn.

## Tuyệt, vậy shell là gì?

**Shell** thực sự là cách bạn sẽ tương tác với kernel. Trước Hệ điều hành Thân thiện với người dùng, khi máy tính khởi động, tất cả những gì bạn có quyền truy cập là một shell. Điều này có nghĩa là tất cả các lệnh và chỉnh sửa của bạn phải được thực hiện theo cách này. Ngày nay, máy tính của chúng tôi khởi động ở **chế độ máy tính để bàn**, nhưng người ta vẫn có thể truy cập shell bằng cách sử dụng **thiết bị đầu cuối**. Khi bạn mở một cái, bạn sẽ thấy một cái gì đó như thế này

```
(Thông tin) $
```

Nó đã sẵn sàng cho lệnh tiếp theo của bạn! Bạn có thể nhập rất nhiều tiện ích **unix** như `ls`, `echo Hello` và shell sẽ thực thi chúng và cung cấp cho bạn kết quả. Một số trong số này được gọi là `shell-builtins` có nghĩa là mã nằm trong chính chương trình shell. Một số trong số này là các chương trình được **biên dịch** mà bạn chạy. Shell chỉ xem xét một biến đặc biệt được gọi là **đường dẫn**, chứa danh sách các đường dẫn được phân tách bằng `:` để tìm kiếm một tệp **thực thi** có tên của bạn, đây là một ví dụ về đường dẫn.

```
$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
```

Vì vậy, khi shell thực thi `ls`, nó sẽ xem xét tất cả các thư mục đó, tìm `/bin/ls` và thực thi nó.

```
$ ls
...
$ /bin/ls
```

Bạn luôn có thể gọi thông qua **đường dẫn đầy đủ**. Đó là lý do tại sao trong các lớp học trước đây, nếu bạn muốn chạy một thứ gì đó trên thiết bị đầu cuối, bạn phải làm `./exe` vì thông thường thư mục mà bạn đang làm việc không nằm trong biến `PATH`. `.` mở rộng đến **thư mục hiện tại** của bạn và shell của bạn thực thi `<current_dir>/exe` là một lệnh hợp lệ.

### Mẹo và thủ thuật về Shell
* Mũi tên lên sẽ đưa bạn đến lệnh gần đây nhất của bạn
* `ctrl-r` sẽ tìm kiếm các lệnh mà bạn đã chạy trước đó
* `ctrl-c` sẽ làm **gián đoạn** tiến trình shell của bạn
* Thêm nữa!

## Được rồi, vậy thiết bị đầu cuối là gì?

Thiết bị đầu cuối chỉ là một ứng dụng hiển thị đầu ra từ shell. Bạn có thể có thiết bị đầu cuối mặc định của mình, thiết bị đầu cuối dựa trên quake, **terminator**, các tùy chọn là vô tận!


