* [Wiki w/Interactive MC Questions](http://angrave.github.io/SystemProgramming/networkingreviewquestions.html)
* Xem [Câu hỏi lập trình](#coding-questions)
* Xem [Câu hỏi trả lời ngắn](#short-answer-questions)
* Xem [MP Wearables](https://courses.engr.illinois.edu/cs241/mps/mp7/) Câu hỏi để suy ngẫm về Thực phẩm


# Câu hỏi trả lời ngắn
## Q1
**Socket** là gì?

## Q2 

Điều gì đặc biệt khi lắng nghe trên cổng 1000 so với cổng 2000?

- Cổng 2000 chậm gấp đôi cổng 1000
- Cổng 2000 nhanh gấp đôi cổng 1000
- Cổng 1000 yêu cầu quyền **root**
- Không có gì


## Q3 
Mô tả một điểm khác biệt đáng kể giữa IPv4 và IPv6

## Q4
Khi nào và tại sao bạn sử dụng `ntohs`?

## Q5
Nếu địa chỉ máy chủ là 32 bit thì tôi có khả năng sử dụng lược đồ IP nào nhất? 128 bit?

## Q6
Giao thức mạng phổ biến nào dựa trên **gói tin** và có thể không gửi dữ liệu thành công?

## Q7
Giao thức phổ biến nào **dựa trên luồng** và sẽ gửi lại dữ liệu nếu gói tin bị mất?

## Q8
**Bắt tay ba bước** SYN, SYN-ACK, ACK là gì?

## Q9
Cái nào sau đây **KHÔNG** phải là tính năng của TCP? 
- Sắp xếp lại gói tin
- **Kiểm soát luồng**
- Truyền lại gói tin
- Phát hiện lỗi đơn giản
- **Mã hóa**

## Q10
Giao thức nào sử dụng **số thứ tự**? Giá trị ban đầu của chúng là gì? Và tại sao?

## Q11
Các **lệnh gọi mạng** tối thiểu cần thiết để xây dựng **máy chủ TCP** là gì? Thứ tự chính xác của chúng là gì?

## Q12
Các lệnh gọi mạng tối thiểu cần thiết để xây dựng **máy khách TCP** là gì? Thứ tự chính xác của chúng là gì?

## Q13
Khi nào bạn sẽ gọi `bind` trên máy khách TCP?

## Q14 
Mục đích của
`socket`,
`bind`,
`listen`,
`accept`
là gì?

## Q15
Lệnh gọi nào ở trên có thể **chặn**, chờ máy khách mới kết nối?

## Q16
**DNS** là gì? Nó làm gì cho bạn? Lệnh gọi mạng CS241 nào sẽ sử dụng nó cho bạn?

## Q17
Đối với `getaddrinfo`, làm cách nào để bạn chỉ định **socket máy chủ**?

## Q18
Tại sao `getaddrinfo` có thể tạo ra các **gói mạng**?

## Q19
Lệnh gọi mạng nào chỉ định kích thước của **backlog** được phép?

## Q20
Lệnh gọi mạng nào trả về một **bộ mô tả tệp** mới?

## Q21
Khi nào **socket thụ động** được sử dụng?

## Q22
Khi nào `epoll` là lựa chọn tốt hơn so với `select`? Khi nào `select` là lựa chọn tốt hơn so với `epoll`?

## Q23
`write(fd, data, 5000)`  có luôn gửi 5000 byte dữ liệu không? Khi nào nó có thể thất bại?

## Q24 
**Dịch địa chỉ mạng (NAT)** hoạt động như thế nào? 


## Q25
@MCQ
Giả sử một mạng có **thời gian truyền** 20ms giữa Máy khách và Máy chủ, sẽ mất bao nhiêu thời gian để thiết lập kết nối TCP?
20 ms
40 ms 
100 ms
60 ms @ANS
**Bắt tay 3 chiều** @EXP
@END


## Q26
Một số điểm khác biệt giữa HTTP 1.0 và HTTP 1.1 là gì? Sẽ mất bao nhiêu ms để truyền 3 tệp từ máy chủ đến máy khách nếu mạng có thời gian truyền 20ms? Thời gian thực hiện khác nhau như thế nào giữa HTTP 1.0 và HTTP 1.1?


# Câu hỏi lập trình
## Q 2.1

Viết vào một **socket mạng** có thể không gửi tất cả các byte và có thể bị gián đoạn do **tín hiệu**. Kiểm tra giá trị trả về của `write` để triển khai `write_all` sẽ gọi `write` lặp đi lặp lại với bất kỳ dữ liệu còn lại nào. Nếu `write` trả về -1 thì trả về -1 ngay lập tức trừ khi `errno` là `EINTR` - trong trường hợp đó, hãy lặp lại nỗ lực `write` cuối cùng. Bạn sẽ cần phải sử dụng **số học con trỏ**.
````C
// Trả về -1 nếu ghi không thành công (trừ khi EINTR trong trường hợp đó, nó sẽ gọi lại write)
// Lặp lại các lệnh gọi write cho đến khi tất cả bộ đệm được ghi.
ssize_t write_all(int fd, const char *buf, size_t nbyte) {
  ssize_t nb = write(fd, buf, nbyte);
  return nb;
}
````

## Q 2.2
Triển khai máy chủ TCP **đa luồng** lắng nghe trên cổng 2000. Mỗi **luồng** sẽ đọc 128 byte từ bộ mô tả tệp của máy khách và **phản hồi** lại cho máy khách, trước khi đóng kết nối và kết thúc luồng.

## Q 2.3
Triển khai máy chủ UDP lắng nghe trên cổng 2000. Dành một bộ đệm 200 byte. **Lắng nghe** một gói tin đến. Các gói tin hợp lệ có kích thước tối đa 200 byte và bắt đầu bằng bốn byte 0x65 0x66 0x67 0x68. Bỏ qua các gói tin không hợp lệ. Đối với các gói tin hợp lệ, hãy thêm giá trị của byte thứ năm dưới dạng giá trị **không dấu** vào tổng đang chạy và in tổng cho đến nay. Nếu tổng đang chạy lớn hơn 255 thì thoát.


