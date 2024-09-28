## Làm thế nào để tôi tìm hiểu xem tệp (inode) là tệp thông thường hay thư mục?

Sử dụng macro `S_ISDIR` để kiểm tra các bit chế độ trong struct `stat`:

```C
struct stat s;
stat("/tmp", &s);
if (S_ISDIR(s.st_mode)) { ... 
```
Lưu ý, sau này chúng ta sẽ viết mã mạnh mẽ để xác minh rằng lệnh gọi `stat` thành công (trả về 0); nếu lệnh gọi `stat` thất bại, chúng ta nên giả sử nội dung của struct `stat` là tùy ý.

## Làm thế nào để tôi đệ quy vào các thư mục con?

Trước tiên, một câu đố - bạn có thể tìm thấy bao nhiêu lỗi trong đoạn mã sau?
```C
void dirlist(char *path) {
  
  struct dirent *dp;
  DIR *dirp = opendir(path);
  while ((dp = readdir(dirp)) != NULL) {
     char newpath[strlen(path) + strlen(dp->d_name) + 1];
     sprintf(newpath,"%s/%s", newpath, dp->d_name);
     printf("%s\n", dp->d_name);
     dirlist(newpath);
  }
}

int main(int argc, char **argv) { dirlist(argv[1]); return 0; }
```
Bạn đã tìm thấy tất cả 5 lỗi chưa?
```C
// Kiểm tra kết quả opendir (có thể người dùng đã cung cấp cho chúng ta một đường dẫn không thể mở dưới dạng thư mục)
if (!dirp) { perror("Không thể mở thư mục"); return; }
// +2 vì chúng ta cần chỗ cho / và ký tự null kết thúc chuỗi
char newpath[strlen(path) + strlen(dp->d_name) + 2]; 
// Tham số chính xác
sprintf(newpath,"%s/%s", path, dp->d_name); 
// Thực hiện kiểm tra stat (và xác minh) trước khi đệ quy
if (0 == stat(newpath,&s) && S_ISDIR(s.st_mode)) dirlist(newpath);
// Rò rỉ tài nguyên: bộ xử lý tệp thư mục không được đóng sau vòng lặp while
closedir(dirp);
```


## Liên kết tượng trưng là gì? Chúng hoạt động như thế nào? Làm cách nào để tạo một liên kết?

**Liên kết tượng trưng** (hay **liên kết mềm**) là các tệp đặc biệt chứa đường dẫn đến một tệp hoặc thư mục khác. 

Để tạo liên kết tượng trưng trong shell, sử dụng lệnh `ln -s <đích> <liên_kết>`:

```bash
ln -s /đường/dẫn/tới/tệp_gốc /đường/dẫn/tới/liên_kết
```

Để đọc nội dung của liên kết (đường dẫn tới tệp gốc), hãy sử dụng `readlink`:
```bash
$ readlink myfile.txt
../../dir1/notes.txt
```

Để đọc siêu thông tin (stat) của một liên kết tượng trưng, hãy sử dụng `lstat` chứ không phải `stat`:
```C
struct stat s1, s2;
stat("myfile.txt", &s1); // stat thông tin về tệp notes.txt
lstat("myfile.txt", &s2); // stat thông tin về liên kết tượng trưng
```

Trong C, bạn có thể sử dụng hàm `symlink` để tạo liên kết tượng trưng:

```C
symlink(const char *target, const char *symlink);
```

## Ưu điểm của liên kết tượng trưng
* Có thể tham chiếu đến các tệp chưa tồn tại.
* Không giống như liên kết cứng, có thể tham chiếu đến thư mục cũng như tệp thông thường.
* Có thể tham chiếu đến các tệp (và thư mục) tồn tại bên ngoài hệ thống tệp hiện tại.

**Nhược điểm chính:** Chậm hơn so với tệp và thư mục thông thường. Khi nội dung liên kết được đọc, chúng phải được diễn giải thành đường dẫn mới đến tệp đích.

## `/dev/null` là gì và khi nào nó được sử dụng?

Tệp `/dev/null` là một nơi tuyệt vời để "vứt bỏ" dữ liệu mà bạn không bao giờ cần đọc!
Các byte được gửi đến `/dev/null` sẽ bị loại bỏ ngay lập tức. Một cách sử dụng phổ biến của `/dev/null` là loại bỏ đầu ra tiêu chuẩn. Ví dụ:
```bash
$ ls . >/dev/null
```

## Tại sao tôi muốn đặt sticky bit của thư mục?

Khi sticky bit của thư mục được đặt, chỉ chủ sở hữu tệp, chủ sở hữu thư mục và người dùng root mới có thể đổi tên (hoặc xóa) tệp. Điều này hữu ích khi nhiều người dùng có quyền ghi vào một thư mục chung.

Một cách sử dụng phổ biến của sticky bit là cho thư mục `/tmp` được chia sẻ và có thể ghi.


## Tại sao các chương trình shell và script lại bắt đầu bằng `#!/usr/bin/env python`?
Trả lời: Để có tính di động!
Mặc dù có thể viết đường dẫn đầy đủ đến trình thông dịch Python hoặc Perl, nhưng cách tiếp cận này không có tính di động vì bạn có thể đã cài đặt Python trong một thư mục khác.

Để khắc phục điều này, tiện ích `env` được sử dụng để tìm và thực thi chương trình trên đường dẫn của người dùng.
Bản thân tiện ích `env` trong lịch sử đã được lưu trữ trong `/usr/bin` - và nó phải được chỉ định với một đường dẫn tuyệt đối.


## Làm cách nào để tạo các tệp 'ẩn', tức là không được liệt kê bởi "ls"? Làm cách nào để liệt kê chúng?
Dễ dàng! Tạo các tệp (hoặc thư mục) bắt đầu bằng "." - sau đó (theo mặc định) chúng không được hiển thị bởi các công cụ và tiện ích tiêu chuẩn.

Điều này thường được sử dụng để ẩn các tệp cấu hình bên trong thư mục chính của người dùng.
Ví dụ: `ssh` lưu trữ các tùy chọn của nó bên trong một thư mục có tên là `.sshd`.

Để liệt kê tất cả các tệp, bao gồm cả các mục nhập thường bị ẩn, hãy sử dụng `ls` với tùy chọn `-a`:
```bash
$ ls -a
.			a.c			myls
..			a.out			other.txt
.secret	
```



## Điều gì xảy ra nếu tôi tắt bit thực thi trên thư mục?
Bit thực thi cho một thư mục được sử dụng để kiểm soát xem nội dung thư mục có thể được liệt kê hay không.

```bash
$ chmod ugo-x dir1
$ ls -l
drw-r--r--   3 angrave  staff   102 Nov 10 11:22 dir1
```

Tuy nhiên, khi cố gắng liệt kê nội dung của thư mục:
```bash
$ ls dir1
ls: dir1: Permission denied
```
Nói cách khác, bản thân thư mục có thể được khám phá nhưng nội dung của nó không thể được liệt kê.


## Globbing tệp là gì (và ai thực hiện nó)?
Trước khi thực thi chương trình, shell sẽ mở rộng các tham số thành các tên tệp phù hợp. Ví dụ: nếu thư mục hiện tại có ba tên tệp bắt đầu bằng `my` ( `my1.txt` `mytext.txt` `myomy`), thì:
```bash
$ echo my*
```
Sẽ được mở rộng thành:
```bash
$ echo my1.txt mytext.txt myomy
```
Điều này được gọi là **globbing tệp** và được xử lý trước khi lệnh được thực thi.
Tức là các tham số của lệnh giống hệt với việc nhập thủ công mọi tên tệp phù hợp.


## Tạo thư mục an toàn
Giả sử bạn đã tạo thư mục của riêng mình trong `/tmp` và sau đó đặt quyền sao cho chỉ bạn mới có thể sử dụng thư mục (xem bên dưới). Điều này có an toàn không? 
```bash
$ mkdir /tmp/mystuff
$ chmod 700 /tmp/mystuff
```
Có một khoảng thời gian giữa khi thư mục được tạo và khi quyền của nó được thay đổi. Điều này dẫn đến một số lỗ hổng dựa trên điều kiện cuộc đua (trong đó kẻ tấn công sửa đổi thư mục theo một cách nào đó trước khi các đặc quyền bị xóa). Một số ví dụ bao gồm:

Một người dùng khác thay thế `mystuff` bằng một liên kết cứng đến một tệp hoặc thư mục hiện có thuộc sở hữu của người dùng thứ hai, sau đó họ sẽ có thể đọc và kiểm soát nội dung của thư mục `mystuff`. Ôi không - bí mật của chúng ta không còn bí mật nữa!

Tuy nhiên, trong ví dụ cụ thể này, thư mục `/tmp` có sticky bit được đặt, vì vậy những người dùng khác có thể không xóa thư mục `mystuff` và kịch bản tấn công đơn giản được mô tả ở trên là không thể. Điều này không có nghĩa là việc tạo thư mục và sau đó làm cho thư mục trở nên riêng tư là an toàn! Phiên bản tốt hơn là tạo nguyên tử thư mục với các quyền chính xác ngay từ đầu:
```bash
$ mkdir -m 700 /tmp/mystuff
```

## Làm cách nào để tự động tạo thư mục cha?
Sử dụng tùy chọn `-p` với lệnh `mkdir`:
```bash
$ mkdir -p d1/d2/d3
```
Lệnh này sẽ tự động tạo `d1` và `d2` nếu chúng không tồn tại.

## Mặt nạ umask mặc định của tôi là 022; điều này có nghĩa là gì?

**Umask** *trừ* (giảm) các bit quyền từ 777 và được sử dụng khi các tệp mới và thư mục mới được tạo bởi `open`, `mkdir`, v.v. Do đó, `022` (bát phân) có nghĩa là quyền của nhóm và những người khác sẽ không bao gồm bit cho phép ghi. Mỗi tiến trình (bao gồm cả shell) đều có giá trị umask hiện tại. Khi phân nhánh, tiến trình con sẽ kế thừa giá trị umask của tiến trình cha.

Ví dụ: bằng cách đặt umask thành 077 trong shell, đảm bảo rằng việc tạo tệp và thư mục trong tương lai sẽ chỉ có thể truy cập được đối với người dùng hiện tại:
```bash
$ umask 077
$ mkdir secretdir
```

Ví dụ về mã, giả sử một tệp mới được tạo bằng `open()` và các bit chế độ `666` (các bit ghi và đọc cho người dùng, nhóm và những người khác):
```C
open("myfile", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
```
Nếu umask là bát phân 022, thì quyền của tệp được tạo sẽ là `0666 & ~022`,
tức là:
```C
           S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
```

## Làm cách nào để sao chép byte từ tệp này sang tệp khác?
Sử dụng  lệnh `dd` linh hoạt. Ví dụ: lệnh sau sao chép 1 MB dữ liệu từ tệp `/dev/urandom` sang tệp `/dev/null`. Dữ liệu được sao chép dưới dạng 1024 khối có kích thước khối 1024 byte.
```bash
$ dd if=/dev/urandom of=/dev/null bs=1k count=1024
```
Cả tệp đầu vào và đầu ra trong ví dụ trên đều là ảo - chúng không tồn tại trên đĩa. Điều này có nghĩa là tốc độ truyền không bị ảnh hưởng bởi sức mạnh phần cứng. Thay vào đó, chúng là một phần của hệ thống tệp `dev`, là hệ thống tệp ảo do kernel cung cấp.
Tệp ảo `/dev/urandom` cung cấp một luồng byte ngẫu nhiên vô hạn, trong khi tệp ảo `/dev/null` bỏ qua tất cả các byte được ghi vào nó. Một cách sử dụng phổ biến của `/dev/null` là loại bỏ đầu ra của lệnh, ví dụ:
```bash
$ myverboseexecutable > /dev/null
```

Một tệp ảo `/dev` thường được sử dụng khác là `/dev/zero` cung cấp một luồng byte 0 vô hạn.
Ví dụ: chúng ta có thể đánh giá hiệu suất của hệ điều hành khi đọc luồng byte 0 trong kernel vào bộ nhớ tiến trình và ghi các byte trở lại kernel mà không có bất kỳ I/O đĩa nào. Lưu ý thông lượng (~20GB/s) phụ thuộc nhiều vào kích thước khối. Đối với kích thước khối nhỏ, chi phí của các lệnh gọi hệ thống `read` và `write` bổ sung sẽ chiếm ưu thế.

```bash
$ dd if=/dev/zero of=/dev/null bs=1M count=1024
1024+0 records in
1024+0 records out
1073741824 bytes (1.1 GB) copied, 0.0539153 s, 19.9 GB/s
```

## Điều gì xảy ra khi tôi chạm vào một tệp?
Tệp thực thi `touch` tạo tệp nếu nó không tồn tại và cũng cập nhật thời gian sửa đổi lần cuối của tệp thành thời gian hiện tại. Ví dụ: chúng ta có thể tạo một tệp riêng tư mới với thời gian hiện tại:
```bash
$ umask 077       # tất cả các tệp mới trong tương lai sẽ che tất cả các bit r,w,x để truy cập nhóm và khác
$ touch file123   # tạo một tệp nếu nó không tồn tại và cập nhật thời gian sửa đổi của nó
$ stat file123
  Tệp: `file123'
  Kích thước: 0         	Khối: 0          Khối IO: 65536  tệp trống thông thường
Thiết bị: 21h/33d	Inode: 226148      Liên kết: 1
Truy cập: (0600/-rw-------)  Uid: (395606/ angrave)   Gid: (61019/     ews)
Truy cập: 2014-11-12 13:42:06.000000000 -0600
Sửa đổi: 2014-11-12 13:42:06.001787000 -0600
Thay đổi: 2014-11-12 13:42:06.001787000 -0600
```

Một ví dụ sử dụng `touch` là buộc `make` biên dịch lại một tệp không thay đổi sau khi sửa đổi các tùy chọn trình biên dịch bên trong makefile. Hãy nhớ rằng `make` là 'lười biếng' - nó sẽ so sánh thời gian sửa đổi của tệp nguồn với tệp đầu ra tương ứng để xem tệp có cần được biên dịch lại hay không:
```bash
$ touch myprogram.c   # buộc tệp nguồn của tôi được biên dịch lại
$ make
```

[Chuyển đến Hệ thống tệp: Phần 5](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-5:-Virtual-file-systems)
