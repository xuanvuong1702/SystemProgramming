# Các chủ đề
* Vòng đời pthread
* Mỗi luồng có một ngăn xếp
* Lấy giá trị trả về từ một luồng
* Sử dụng `pthread_join`
* Sử dụng `pthread_create`
* Sử dụng `pthread_exit`
* Trong những điều kiện nào thì một quá trình sẽ thoát

# Câu hỏi
* Điều gì xảy ra khi một pthread được tạo? (bạn không cần đi vào chi tiết quá mức)
* Ngăn xếp của mỗi luồng ở đâu?
* Làm thế nào để lấy một giá trị trả về từ một `pthread_t`? Những cách nào mà một luồng có thể đặt giá trị trả về đó? Điều gì xảy ra nếu bạn loại bỏ giá trị trả về?
* Tại sao `pthread_join` quan trọng (nghĩ về không gian ngăn xếp, các thanh ghi, giá trị trả về)?
* `pthread_exit` làm gì trong những trường hợp bình thường (tức là bạn không phải là luồng cuối cùng)? Những hàm nào khác được gọi khi bạn gọi pthread_exit?
* Cho tôi ba điều kiện mà một quá trình đa luồng sẽ thoát. Bạn có thể nghĩ ra thêm điều kiện nào khác không?
* Vấn đề song song một cách xấu hổ là gì?