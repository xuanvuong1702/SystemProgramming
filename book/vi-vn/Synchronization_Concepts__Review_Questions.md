> Lưu ý các vấn đề đồng bộ hóa lập trình luồng nằm trên một trang wiki riêng biệt. Trang này tập trung vào các chủ đề khái niệm.
> Số câu hỏi có thể thay đổi
 
## Câu hỏi 1
Mỗi điều kiện Coffman có nghĩa là gì? (ví dụ: bạn có thể cung cấp định nghĩa cho từng điều kiện)
* Giữ và chờ
* Chờ đợi vòng tròn
* Không chiếm quyền ưu tiên
* Loại trừ lẫn nhau

## Câu hỏi 2
Hãy cho một ví dụ thực tế về việc phá vỡ từng điều kiện Coffman. Một tình huống cần xem xét: Thợ sơn, sơn và cọ sơn.
Giữ và chờ
Chờ đợi vòng tròn
Không chiếm quyền ưu tiên
Loại trừ lẫn nhau

## Câu hỏi 3
Xác định khi nào mã Triết gia ăn tối gây ra deadlock (hoặc không). Ví dụ: nếu bạn thấy đoạn mã sau, điều kiện Coffman nào không được thỏa mãn?

```C
// Nhận cả hai khóa hoặc không có khóa nào.
pthread_mutex_lock(a);
if (pthread_mutex_trylock(b)) { /* thất bại */
    pthread_mutex_unlock(a);
    ...
}
```

## Câu hỏi 4
Có bao nhiêu tiến trình bị chặn?

* P1 chiếm R1
* P2 chiếm R2
* P1 chiếm R3
* P2 chờ R3
* P3 chiếm R5
* P1 chiếm R4
* P3 chờ R1
* P4 chờ R5
* P5 chờ R1

## Câu hỏi 5 
Có bao nhiêu câu sau đây đúng với bài toán người đọc-người viết?

* Có thể có nhiều người đọc đang hoạt động
* Có thể có nhiều người viết đang hoạt động
* Khi có một người viết đang hoạt động, số lượng người đọc đang hoạt động phải bằng không
* Nếu có một người đọc đang hoạt động, số lượng người viết đang hoạt động phải bằng không
* Người viết phải đợi cho đến khi những người đọc đang hoạt động hiện tại kết thúc




