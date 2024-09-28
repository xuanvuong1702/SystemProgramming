# Bối cảnh
<img src="https://upload.wikimedia.org/wikipedia/commons/7/7b/An_illustration_of_the_dining_philosophers_problem.png" height="500px" width="500px">

Vì vậy, bạn có các triết gia của mình ngồi quanh một bàn, tất cả đều muốn ăn một ít mì ống (hoặc bất cứ thứ gì đó) và họ thực sự đói. Về cơ bản, mỗi triết gia đều giống nhau, có nghĩa là mỗi triết gia có cùng một tập lệnh dựa trên triết gia khác (tức là; bạn không thể bảo mọi triết gia có số chẵn làm một việc và mọi triết gia có số lẻ làm một việc khác).

# Các giải pháp thất bại
## Deadlock trái-phải
Chúng ta làm gì? Hãy thử một giải pháp đơn giản

````C
void* philosopher(void* forks){
     info phil_info = forks;
     pthread_mutex_t* left_fork = phil_info->left_fork;
     pthread_mutex_t* right_fork = phil_info->right_fork;
     while(phil_info->simulation){
          pthread_mutex_lock(left_fork);
          pthread_mutex_lock(right_fork);
          eat(left_fork, right_fork);
          pthread_mutex_unlock(left_fork);
          pthread_mutex_unlock(right_fork);
     }
}
````

Nhưng điều này gặp phải một vấn đề! Điều gì sẽ xảy ra nếu mọi người nhặt dĩa bên trái của họ và đang đợi dĩa bên phải của họ? Chúng ta đã rơi vào tình trạng deadlock (bế tắc) chương trình. Điều quan trọng cần lưu ý là deadlock không xảy ra mọi lúc và xác suất deadlock của giải pháp này giảm xuống khi số lượng triết gia tăng lên. Điều thực sự quan trọng cần lưu ý là cuối cùng giải pháp này sẽ bị deadlock, khiến các luồng bị bỏ đói, điều này là xấu.

## Trylock? Giống livelock hơn
Vì vậy, bây giờ bạn đang nghĩ đến việc phá vỡ một trong các điều kiện Coffman. Chúng ta có
- Loại trừ lẫn nhau (Mutual Exclusion)
- Không chiếm đoạt (No Preemption)
- Giữ và chờ đợi (Hold and Wait)
- Chờ đợi vòng (Circular Wait)

Chà, chúng ta không thể để hai triết gia sử dụng một cái dĩa cùng một lúc, loại trừ lẫn nhau là điều không thể. Trong mô hình đơn giản hiện tại của chúng ta, chúng ta không thể để triết gia buông khóa mutex sau khi anh ta/cô ta đã giữ nó, vì vậy chúng ta sẽ loại bỏ giải pháp này ngay bây giờ -- có một số ghi chú ở cuối trang về giải pháp này. Hãy phá vỡ Giữ và chờ đợi!
````C
void* philosopher(void* forks){
     info phil_info = forks;
     pthread_mutex_t* left_fork = phil_info->left_fork;
     pthread_mutex_t* right_fork = phil_info->right_fork;
     while(phil_info->simulation){
          pthread_mutex_lock(left_fork);
          int failed = pthread_mutex_trylock(right_fork);
          if(!failed){
               eat(left_fork, right_fork);
               pthread_mutex_unlock(right_fork);
          }
          pthread_mutex_unlock(left_fork);
     }
}
````

Bây giờ triết gia của chúng ta nhặt cái dĩa bên trái và cố gắng lấy cái bên phải. Nếu nó có sẵn, họ ăn. Nếu nó không có sẵn, họ đặt cái dĩa bên trái xuống và thử lại. Không có deadlock!

Nhưng, có một vấn đề. Điều gì sẽ xảy ra nếu tất cả các triết gia nhặt dĩa bên trái của họ cùng một lúc, cố gắng lấy dĩa bên phải, đặt dĩa bên trái xuống, nhặt dĩa bên trái, cố gắng lấy dĩa bên phải .... Bây giờ chúng ta đã bị livelock (khóa sống) trong giải pháp của mình! Các triết gia tội nghiệp của chúng ta vẫn đang chết đói, vì vậy hãy cho họ một số giải pháp thích hợp.

# Các giải pháp khả thi

## Trọng tài (Ngây thơ và Nâng cao).

Giải pháp trọng tài ngây thơ là có một trọng tài (ví dụ: mutex). Yêu cầu mỗi triết gia xin phép trọng tài để được ăn. Giải pháp này cho phép một triết gia ăn tại một thời điểm. Khi họ ăn xong, một triết gia khác có thể xin phép để được ăn.

Điều này ngăn chặn deadlock vì không có chờ đợi vòng quanh! Không có triết gia nào phải chờ đợi bất kỳ triết gia nào khác.

Giải pháp trọng tài nâng cao là triển khai một lớp xác định xem dĩa của triết gia có thuộc quyền sở hữu của trọng tài hay không. Nếu có, họ đưa chúng cho triết gia, để anh ta ăn và lấy lại dĩa. Điều này có thêm lợi ích là có thể cho nhiều triết gia ăn cùng một lúc.

### Vấn đề:
- Các giải pháp này chậm
- Chúng có một điểm lỗi duy nhất - trọng tài - có thể gây ra tắc nghẽn
- Trọng tài cũng cần phải công bằng và có thể xác định deadlock trong giải pháp thứ hai
- Trong các hệ thống thực tế, trọng tài có xu hướng đưa dĩa lặp đi lặp lại cho các triết gia vừa ăn xong vì lịch trình xử lý

## Rời khỏi bàn (Giải pháp của Stallings)
Tại sao giải pháp đầu tiên bị deadlock? Chà, có n triết gia và n chiếc dĩa. Điều gì sẽ xảy ra nếu chỉ có 1 triết gia tại bàn? Chúng ta có thể bị deadlock không? Không.

Còn 2 triết gia thì sao? 3? ... Bạn có thể thấy điều này sẽ đi đến đâu. Giải pháp của Stallings nói rằng hãy loại bỏ các triết gia khỏi bàn cho đến khi không thể xảy ra deadlock -- hãy nghĩ về con số kỳ diệu của các triết gia tại bàn là bao nhiêu. Cách để làm điều này trong hệ thống thực tế là thông qua semaphore và cho phép một số lượng triết gia nhất định đi qua.

### Vấn đề:
- Giải pháp yêu cầu rất nhiều chuyển đổi ngữ cảnh, điều này rất tốn kém cho CPU
- Bạn cần biết về số lượng tài nguyên trước để chỉ cho phép số lượng triết gia đó
- Một lần nữa, ưu tiên được dành cho các tiến trình đã ăn xong.

## Thứ tự bán phần (Giải pháp của Dijkstra)
Đây là giải pháp của Dijkstra (ông là người đã đề xuất vấn đề này trong một kỳ thi). Tại sao giải pháp đầu tiên bị deadlock? Dijkstra nghĩ rằng triết gia cuối cùng nhặt dĩa bên trái của mình (gây ra deadlock cho giải pháp) nên nhặt dĩa bên phải của mình. Ông thực hiện điều này bằng cách đánh số các dĩa từ 1 ... n và bảo mỗi triết gia nhặt dĩa có số thấp hơn của mình.

Hãy chạy lại qua điều kiện deadlock. Mọi người cố gắng nhặt cái dĩa có số thấp hơn của họ trước. Triết gia 1 nhận được dĩa 1, Triết gia 2 nhận được dĩa 2, v.v. cho đến khi chúng ta đến Triết gia n. Họ phải chọn giữa dĩa 1 và n. dĩa 1 đã được triết gia 1 giữ, vì vậy họ không thể nhặt dĩa đó, có nghĩa là anh ta sẽ không nhặt dĩa n. Chúng ta đã phá vỡ chờ đợi vòng quanh! Có nghĩa là deadlock là không thể.

### Vấn đề:
- Triết gia cần biết tập hợp các tài nguyên theo thứ tự trước khi lấy bất kỳ tài nguyên nào.
- Bạn cần xác định một thứ tự bán phần cho tất cả các tài nguyên.
- Ưu tiên triết gia đã ăn xong.

## Các giải pháp nâng cao

Có nhiều giải pháp nâng cao hơn mà danh sách không đầy đủ bao gồm
- Dĩa sạch/bẩn (Giải pháp Chandra/Misra)
- Mô hình Actor (các mô hình truyền tin nhắn khác)
- Trọng tài siêu cấp (Đường ống phức tạp)
