# Điều hướng / Thuật ngữ

## Thiết kế một hệ thống tệp! Mục tiêu thiết kế của bạn là gì?
Việc thiết kế một hệ thống tệp là một vấn đề khó khăn bởi vì có rất nhiều mục tiêu thiết kế cấp cao mà chúng ta muốn đáp ứng. Danh sách chưa đầy đủ các mục tiêu lý tưởng bao gồm:

* Đáng tin cậy và mạnh mẽ (ngay cả khi có lỗi phần cứng hoặc ghi không hoàn chỉnh do mất điện)
* Kiểm soát truy cập (bảo mật)
* Theo dõi và hạn ngạch
* Lập chỉ mục và tìm kiếm
* Khả năng sao lưu và phiên bản
* Mã hóa
* Nén tự động
* Hiệu suất cao (ví dụ: Bộ nhớ đệm trong bộ nhớ)
* Sử dụng bộ nhớ hiệu quả, chống trùng lặp dữ liệu

Không phải tất cả các hệ thống tệp đều hỗ trợ tất cả các mục tiêu này. Ví dụ: nhiều hệ thống tệp không tự động nén các tệp hiếm khi được sử dụng.

## `.` , `..` và `...` là gì?
Trong hệ thống tệp Unix tiêu chuẩn: 
* `.` đại diện cho thư mục hiện tại.
* `..` đại diện cho thư mục cha.
* `...` **KHÔNG** phải là đại diện hợp lệ của bất kỳ thư mục nào (đây không phải là thư mục ông bà). Tuy nhiên, nó _có thể_ là tên của một tệp trên đĩa.

## Đường dẫn tuyệt đối và đường dẫn tương đối là gì?
Đường dẫn tuyệt đối là đường dẫn bắt đầu từ 'nút gốc' của cây thư mục của bạn. Đường dẫn tương đối là đường dẫn bắt đầu từ vị trí hiện tại của bạn trong cây.

## Một số ví dụ về đường dẫn tương đối và tuyệt đối là gì?
Nếu bạn bắt đầu trong thư mục chính của mình ("~" viết tắt), thì `Desktop/cs241` sẽ là một đường dẫn tương đối. Đường dẫn tuyệt đối tương ứng của nó có thể là `/Users/[yourname]/Desktop/cs241`.

## Làm cách nào để đơn giản hóa `a/b/../c/./`?
Hãy nhớ rằng `..` có nghĩa là 'thư mục cha' và `.` có nghĩa là 'thư mục hiện tại'.

Ví dụ: `a/b/../c/./`
- Bước 1: `cd a` (trong a)
- Bước 2: `cd b` (trong a/b)
- Bước 3: `cd ..` (trong a, vì .. đại diện cho 'thư mục cha')
- Bước 4: `cd c` (trong a/c)
- Bước 5: `cd .` (trong a/c, vì . đại diện cho 'thư mục hiện tại')

Do đó, đường dẫn này có thể được đơn giản hóa thành `a/c`.

# Vậy hệ thống tệp là gì?

Hệ thống tệp là cách thông tin được tổ chức trên đĩa. Bất cứ khi nào bạn muốn truy cập một tệp, hệ thống tệp sẽ quyết định cách đọc tệp. Đây là hình ảnh mẫu của một hệ thống tệp.

![](http://tinf2.vub.ac.be/~dvermeir/manual/uintro/disk.gif)

Whoa, có rất nhiều thứ, hãy chia nhỏ nó ra
* **Superblock**: Khối này chứa siêu dữ liệu về hệ thống tệp, kích thước, thời gian sửa đổi lần cuối, nhật ký, số lượng inode và inode bắt đầu đầu tiên, số lượng khối dữ liệu và khối dữ liệu bắt đầu đầu tiên.
* **Inode**: Đây là khái niệm trừu tượng chính. Một inode là một tệp. 
* **Khối đĩa**: Đây là nơi dữ liệu được lưu trữ. Nội dung thực tế của tệp.

## Inode lưu trữ nội dung tệp như thế nào?
![](https://classes.soe.ucsc.edu/cmps111/Fall08/inode_with_signatures.jpg)

Từ [Wikipedia](http://en.wikipedia.org/wiki/Inode):

> *Trong hệ thống tệp kiểu Unix, một nút chỉ mục, được gọi một cách không chính thức là inode, là một cấu trúc dữ liệu được sử dụng để biểu diễn một đối tượng hệ thống tệp, có thể là một trong những thứ khác nhau bao gồm tệp hoặc thư mục. Mỗi inode lưu trữ các thuộc tính và vị trí khối đĩa của dữ liệu của đối tượng hệ thống tệp. Các thuộc tính đối tượng hệ thống tệp có thể bao gồm siêu dữ liệu thao tác (ví dụ: thời gian thay đổi, truy cập, sửa đổi), cũng như dữ liệu chủ sở hữu và quyền (ví dụ: id nhóm, id người dùng, quyền).*

Để đọc một vài byte đầu tiên của tệp, hãy theo con trỏ khối trực tiếp đầu tiên đến khối trực tiếp đầu tiên và đọc một vài byte đầu tiên, việc ghi cũng theo quy trình tương tự. Nếu bạn muốn đọc toàn bộ tệp, hãy tiếp tục đọc các khối trực tiếp cho đến khi hết kích thước (chúng ta sẽ nói về các khối gián tiếp sau).

> "Tất cả các vấn đề trong khoa học máy tính đều có thể được giải quyết bằng một cấp độ gián tiếp khác." - David Wheeler

## Tại sao lại tạo khối đĩa có cùng kích thước với các trang bộ nhớ?
Để hỗ trợ bộ nhớ ảo, do đó chúng ta có thể phân trang mọi thứ vào và ra khỏi bộ nhớ.

## Chúng ta muốn lưu trữ thông tin gì cho mỗi tệp?
* Tên tệp
* Kích thước tệp
* Thời gian tạo, sửa đổi lần cuối, truy cập lần cuối
* Quyền
* Đường dẫn tệp
* Tổng kiểm tra
* Dữ liệu tệp (inode)

## Các quyền truyền thống là gì: quyền người dùng - nhóm - khác cho một tệp?
Một số quyền tệp phổ biến bao gồm:
* 755: `rwx r-x r-x`

  người dùng: `rwx`, nhóm: `r-x`, những người khác: `r-x`

  Người dùng có thể đọc, ghi và thực thi. Nhóm và những người khác chỉ có thể đọc và thực thi.
* 644: `rw- r-- r--`

  người dùng: `rw-`, nhóm: `r--`, những người khác: `r--`

  Người dùng có thể đọc và ghi. Nhóm và những người khác chỉ có thể đọc.

## 3 bit quyền cho một tệp thông thường cho mỗi vai trò là gì?
* Đọc (bit quan trọng nhất)  
* Ghi (bit thứ 2)  
* Thực thi (bit ít quan trọng nhất)

## "644" "755" có nghĩa là gì?
Đây là những ví dụ về quyền ở định dạng bát phân (cơ số 8). Mỗi chữ số bát phân tương ứng với một vai trò khác nhau (người dùng, nhóm, mọi người).

Chúng ta có thể đọc quyền ở định dạng bát phân như sau:  
* 644 - Quyền người dùng R/W (Đọc/Ghi), quyền nhóm R (Đọc), quyền mọi người R (Đọc)
* 755 - Quyền người dùng R/W/X (Đọc/Ghi/Thực thi), quyền nhóm R/X (Đọc/Thực thi), quyền mọi người R/X (Đọc/Thực thi)

## Bạn có thể lưu trữ bao nhiêu con trỏ trong mỗi bảng gián tiếp? 
Ví dụ, giả sử chúng ta chia đĩa thành các khối 4KB và chúng ta muốn đánh địa chỉ tối đa 2^32 khối.

Kích thước đĩa tối đa là 4KB * 2^32 = 16TB (hãy nhớ 2^10 = 1024)

Một khối đĩa có thể lưu trữ 4KB / 4B (mỗi con trỏ cần 32 bit) = 1024 con trỏ. Mỗi con trỏ tham chiếu đến một khối đĩa 4KB - vì vậy bạn có thể tham chiếu tối đa 1024 * 4KB = 4MB dữ liệu.

Đối với cùng một cấu hình đĩa, một khối gián tiếp kép lưu trữ 1024 con trỏ đến 1024 bảng gián tiếp. Do đó, một khối gián tiếp kép có thể tham chiếu tối đa 1024 * 4MB = 4GB dữ liệu.

Tương tự, một khối gián tiếp ba có thể tham chiếu tối đa 4TB dữ liệu.

[Chuyển đến Hệ thống tệp: Phần 2](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-2:-Files-are-inodes)

