# Chủ đề
* Vòng đời pthread
* Mỗi luồng có một ngăn xếp
* Nắm bắt các giá trị trả về từ một luồng
* Sử dụng `pthread_join`
* Sử dụng `pthread_create`
* Sử dụng `pthread_exit`
* Trong những điều kiện nào thì một tiến trình sẽ thoát

# Câu hỏi
* Điều gì xảy ra khi một pthread được tạo? (bạn không cần phải đi sâu vào chi tiết cụ thể)
* Ngăn xếp của mỗi luồng ở đâu?
* Làm thế nào để bạn nhận được giá trị trả về cho một `pthread_t`? Các cách để một luồng có thể đặt giá trị trả về đó là gì? Điều gì xảy ra nếu bạn loại bỏ giá trị trả về?
* Tại sao `pthread_join` lại quan trọng (hãy nghĩ đến không gian ngăn xếp, thanh ghi, giá trị trả về)?
* `pthread_exit` làm gì trong trường hợp bình thường (tức là bạn không phải là luồng cuối cùng)? Những hàm nào khác được gọi khi bạn gọi pthread_exit?
* Cho tôi ba điều kiện mà theo đó một tiến trình đa luồng sẽ thoát. Bạn có thể nghĩ ra thêm điều kiện nào không?
* Bài toán song song dễ dàng (embarrassingly parallel problem) là gì?



