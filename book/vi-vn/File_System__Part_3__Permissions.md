# Nhắc tôi nhớ quyền có nghĩa là gì?

Mọi tệp và thư mục đều có một tập hợp 9 bit quyền và một trường kiểu:
* **r**, quyền đọc tệp
* **w**, quyền ghi vào tệp
* **x**, quyền thực thi tệp

`chmod 777 <file>`

|chmod | 7 |  7  |  7  |
|----|-----|-----|-----|
| 01 | 111 | 111 | 111 |
| d | rwx | rwx | rwx |
| 1 | 2   |  3  |  4  |

1. Loại tệp<br />
2. Quyền của chủ sở hữu<br />
3. Quyền của nhóm<br />
4. Quyền của mọi người khác<br />

`mknod` thay đổi trường đầu tiên, loại tệp.
`chmod` nhận một số và một tệp và thay đổi các bit quyền.

Tệp có một chủ sở hữu. Nếu tiến trình của bạn có cùng ID người dùng với chủ sở hữu (hoặc root) thì các quyền trong bộ ba đầu tiên được áp dụng cho bạn. Nếu bạn ở trong cùng một nhóm với tệp (tất cả các tệp cũng thuộc sở hữu của một nhóm) thì tập hợp các bit quyền tiếp theo được áp dụng cho bạn. Nếu không có điều nào ở trên được áp dụng, thì bộ ba cuối cùng được áp dụng cho bạn.

## Làm cách nào để thay đổi quyền trên một tệp?
Sử dụng `chmod` (viết tắt của "thay đổi các bit chế độ tệp").

Có một lệnh gọi hệ thống, `int chmod(const char *path, mode_t mode);` nhưng chúng ta sẽ tập trung vào lệnh shell. Có hai cách phổ biến để sử dụng `chmod`; với một giá trị bát phân hoặc với một chuỗi ký hiệu:
```
$ chmod 644 file1
$ chmod 755 file2
$ chmod 700 file3
$ chmod ugo-w file4
$ chmod o-rx file4
```
Các chữ số cơ số 8 ('bát phân') mô tả các quyền cho mỗi vai trò: Người dùng sở hữu tệp, nhóm và mọi người khác. Số bát phân là tổng của ba giá trị được gán cho ba loại quyền: đọc (4), ghi (2), thực thi (1).

Ví dụ: `chmod 755 myfile`
* r + w + x = chữ số
* người dùng có 4+2+1, quyền đầy đủ (đọc, ghi, thực thi)
* nhóm có 4+0+1, quyền đọc và thực thi
* tất cả người dùng có 4+0+1, quyền đọc và thực thi

## Làm cách nào để đọc chuỗi quyền từ `ls`?
Sử dụng `ls -l`. 
Lưu ý rằng các quyền sẽ xuất ra ở định dạng 'drwxrwxrwx'. Ký tự đầu tiên cho biết loại tệp. 
Các giá trị có thể có cho ký tự đầu tiên:
* (-) tệp thông thường
* (d) thư mục
* (c) tệp thiết bị ký tự
* (l) liên kết tượng trưng
* (p) pipe
* (b) thiết bị khối
* (s) socket

## `sudo` là gì?
Sử dụng `sudo` để trở thành quản trị viên trên máy.
Ví dụ: Thông thường (trừ khi được chỉ định rõ ràng trong tệp '/etc/fstab', bạn cần quyền truy cập root để gắn kết hệ thống tệp). `sudo` có thể được sử dụng để tạm thời chạy lệnh với tư cách root (miễn là người dùng có quyền sudo).

```
$ sudo mount /dev/sda2 /stuff/mydisk
$ sudo adduser fred
```

## Làm cách nào để thay đổi quyền sở hữu của một tệp?
Sử dụng `chown username filename`.

## Làm cách nào để đặt quyền từ mã?

Sử dụng hàm `chmod(const char *path, mode_t mode);`.

## Tại sao một số tệp lại là 'setuid'? Điều này có nghĩa là gì?
Bit set-user-ID-on-execution (setuid) thay đổi người dùng được liên kết với tiến trình khi tệp được chạy. Điều này thường được sử dụng cho các lệnh cần chạy với tư cách root nhưng được thực thi bởi những người dùng không phải root. Ví dụ về điều này là `sudo`.

Set-group-ID-on-execution thay đổi nhóm mà tiến trình được chạy.

## Tại sao chúng hữu ích?
Trường hợp sử dụng phổ biến nhất là để người dùng có thể có quyền truy cập root (quản trị viên) trong suốt thời gian chương trình chạy.

## `sudo` chạy với quyền gì?
```
$ ls -l /usr/bin/sudo
-r-s--x--x  1 root  wheel  327920 Oct 24 09:04 /usr/bin/sudo
```
Bit 's' có nghĩa là thực thi và set-uid; ID người dùng hiệu quả của tiến trình sẽ khác với tiến trình cha. Trong ví dụ này, nó sẽ là root.

## Sự khác biệt giữa `getuid()` và `geteuid()` là gì?
* `getuid` trả về ID người dùng thực (bằng 0 nếu đăng nhập với tư cách root).
* `geteuid` trả về ID người dùng hiệu quả (bằng 0 nếu hoạt động với tư cách root, ví dụ: do cờ setuid được đặt trên chương trình).

## Làm cách nào để đảm bảo chỉ những người dùng có đặc quyền mới có thể chạy mã của tôi?
* Kiểm tra quyền hiệu quả của người dùng bằng cách gọi `geteuid()`. Giá trị trả về bằng 0 có nghĩa là chương trình đang chạy hiệu quả với tư cách root.

[Chuyển đến Hệ thống tệp: Phần 4](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-4:-Working-with-directories)



