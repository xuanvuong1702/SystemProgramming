# Chủ đề
Điều kiện Coffman
Biểu đồ phân bổ tài nguyên
Bài toán Triết gia ăn tối
* Các giải pháp Triết gia ăn tối thất bại
* Các giải pháp Triết gia ăn tối bị Livelock
* Các giải pháp Triết gia ăn tối hoạt động: Ưu điểm / Nhược điểm

# Câu hỏi
* Các điều kiện Coffman là gì?
* Mỗi điều kiện Coffman có nghĩa là gì? (ví dụ: bạn có thể cung cấp định nghĩa cho từng điều kiện)
* Đưa ra ví dụ thực tế về việc phá vỡ từng điều kiện Coffman. Một tình huống cần xem xét: Thợ sơn, Sơn, Cọ sơn, v.v. Làm thế nào để bạn đảm bảo rằng công việc sẽ được hoàn thành?
* Biết cách xác định khi nào mã Triết gia ăn tối gây ra deadlock (hay không).
Ví dụ: nếu bạn thấy đoạn mã sau đây mà điều kiện Coffman nào không được thỏa mãn?
```C
// Lấy cả hai khóa hoặc không có khóa nào.
pthread_mutex_lock( a );
if( pthread_mutex_trylock( b ) ) { /*thất bại*/
   pthread_mutex_unlock( a );
   ...
}
```


* Nếu một luồng gọi
```C
  pthread_mutex_lock(m1) // thành công
  pthread_mutex_lock(m2) // bị chặn
```
và một luồng khác gọi
```C
  pthread_mutex_lock(m2) // thành công
  pthread_mutex_lock(m1) // bị chặn
```
Điều gì xảy ra và tại sao? Điều gì xảy ra nếu luồng thứ ba gọi `pthread_mutex_lock(m1)`?

* Có bao nhiêu tiến trình bị chặn? Như thường lệ, giả sử rằng một tiến trình có thể hoàn thành nếu nó có thể giành được tất cả các tài nguyên được liệt kê bên dưới.
     * P1 giành được R1
     * P2 giành được R2
     * P1 giành được R3
     * P2 đợi R3
     * P3 giành được R5
     * P1 giành được R4
     * P3 đợi R1
     * P4 đợi R5
     * P5 đợi R1

(Vẽ biểu đồ tài nguyên!)
