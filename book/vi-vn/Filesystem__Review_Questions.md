> Số câu hỏi có thể thay đổi

## Q1
Viết một hàm sử dụng `fseek` và `ftell` để thay thế ký tự ở giữa của một tệp bằng 'X'
````C
void xout(char* filename) {
  FILE *f = fopen(filename, ____ );
  


}
````
## Q2
Trong hệ thống tệp `ext2`, có bao nhiêu inode được đọc từ đĩa để truy cập byte đầu tiên của tệp `/dir1/subdirA/notes.txt`? Giả sử tên thư mục và số inode trong thư mục gốc (nhưng không phải bản thân inode) đã có trong bộ nhớ.

## Q3
Trong hệ thống tệp `ext2`, số lượng khối đĩa tối thiểu phải được đọc từ đĩa để truy cập byte đầu tiên của tệp `/dir1/subdirA/notes.txt` là bao nhiêu? Giả sử tên thư mục, số inode trong thư mục gốc và tất cả inode đã có trong bộ nhớ.

## Q4
Trong một hệ thống tệp `ext2` với địa chỉ 32 bit và khối đĩa 4KB, các inode có thể lưu trữ 10 số khối đĩa trực tiếp. Kích thước tệp tối thiểu cần thiết để yêu cầu i) một bảng gián tiếp đơn? ii) một bảng gián tiếp kép là bao nhiêu?

## Q5
Sửa lệnh shell `chmod` bên dưới để đặt quyền của tệp `secret.txt` sao cho chủ sở hữu có quyền đọc, ghi và thực thi, nhóm có quyền đọc và mọi người khác không có quyền truy cập.
````
chmod 000 secret.txt
```

