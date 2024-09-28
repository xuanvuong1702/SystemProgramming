# Một số thuật toán lập lịch nổi tiếng là gì?

Đối với tất cả các ví dụ,

Tiến trình 1: Thời gian chạy 1000ms

Tiến trình 2: Thời gian chạy 2000ms

Tiến trình 3: Thời gian chạy 3000ms

Tiến trình 4: Thời gian chạy 4000ms

Tiến trình 5: Thời gian chạy 5000ms

# Công việc ngắn nhất trước (Shortest Job First - SJF)

![](http://i.imgur.com/jGLvjqT.png)

* P1 Đến: 0ms
* P2 Đến: 0ms
* P3 Đến: 0ms
* P4 Đến: 0ms
* P5 Đến: 0ms

Tất cả các tiến trình đều đến lúc bắt đầu và bộ lập lịch lên lịch cho công việc có tổng thời gian CPU ngắn nhất. Vấn đề rõ ràng là bộ lập lịch này cần biết chương trình này sẽ chạy trong bao lâu theo thời gian **trước khi chạy chương trình**.

**Lưu ý kỹ thuật:** Triển khai SJF thực tế sẽ không sử dụng tổng thời gian thực thi của tiến trình mà là thời gian **bùng nổ** (tổng thời gian CPU bao gồm cả việc thực thi tính toán trong tương lai trước khi tiến trình không còn sẵn sàng để chạy). Thời gian bùng nổ dự kiến ​​có thể được ước tính bằng cách sử dụng trung bình động có trọng số giảm theo cấp số nhân dựa trên thời gian bùng nổ trước đó nhưng đối với phần trình bày này, chúng tôi sẽ đơn giản hóa cuộc thảo luận này để sử dụng tổng thời gian chạy của tiến trình làm đại diện cho thời gian bùng nổ.

**Ưu điểm**
* Các công việc ngắn hơn có xu hướng được chạy trước

**Nhược điểm**
* Cần thuật toán phải **biết trước** thời gian chạy của mỗi tiến trình.

# Công việc ngắn nhất trước có tiền chiếm (Preemptive Shortest Job First - PSJF)

Công việc ngắn nhất trước có tiền chiếm giống như công việc ngắn nhất trước nhưng nếu một công việc mới xuất hiện với thời gian chạy ngắn hơn tổng thời gian chạy của công việc hiện tại, thì nó sẽ được chạy thay thế. (Nếu nó bằng nhau như ví dụ của chúng tôi, thuật toán của chúng tôi có thể chọn). Bộ lập lịch sử dụng thời gian chạy _tổng_ của tiến trình. Nếu bạn muốn thời gian _còn lại_ ngắn nhất, thì đó là một biến thể của PSJF được gọi là Thời gian còn lại ngắn nhất trước (Shortest Remaining Time First - SRTF).

![](http://i.imgur.com/QvoX7Ia.png)

* P2 lúc 0ms
* P1 lúc 1000ms
* P5 lúc 3000ms
* P4 lúc 4000ms
* P3 lúc 5000ms

Đây là những gì thuật toán của chúng tôi thực hiện. Nó chạy P2 vì nó là thứ duy nhất để chạy. Sau đó, P1 xuất hiện lúc 1000ms, P2 chạy trong 2000ms, vì vậy bộ lập lịch của chúng tôi **chủ động** dừng P2 và cho phép P1 chạy hết (điều này hoàn toàn phụ thuộc vào thuật toán vì thời gian là bằng nhau). Sau đó, P5 xuất hiện -- vì không có tiến trình nào đang chạy, bộ lập lịch sẽ chạy tiến trình 5. P4 xuất hiện và vì thời gian chạy bằng P5, bộ lập lịch dừng P5 và chạy P4. Cuối cùng P3 xuất hiện, **chiếm quyền ưu tiên** của P4 và chạy đến khi hoàn thành. Sau đó P4 chạy, sau đó P5 chạy.

**Ưu điểm**
* Đảm bảo các công việc ngắn hơn được chạy trước

**Nhược điểm**
* Cần biết thời gian chạy của mỗi tiến trình.

# Đến trước phục vụ trước (First Come First Served - FCFS)

![](http://i.imgur.com/lcMpUZz.png)

* P2 lúc 0ms
* P1 lúc 1000ms
* P5 lúc 3000ms
* P4 lúc 4000ms
* P3 lúc 5000ms

Các tiến trình được lên lịch theo thứ tự đến. Một ưu điểm của FCFS là thuật toán lập lịch rất đơn giản: hàng đợi sẵn sàng chỉ là một hàng đợi FIFO (vào trước ra trước).
FCFS bị ảnh hưởng bởi hiệu ứng đoàn tàu.

Ở đây P2 đến, sau đó P1 đến, sau đó P5, sau đó P4, sau đó P3. Bạn có thể thấy hiệu ứng đoàn tàu cho P5.

**Ưu điểm**
* Triển khai đơn giản

**Nhược điểm**
* Các tiến trình chạy dài có thể chặn tất cả các tiến trình khác

# Vòng tròn (Round Robin - RR)

Các tiến trình được lên lịch theo thứ tự đến của chúng trong hàng đợi sẵn sàng. Tuy nhiên, sau một **lượng thời gian** nhỏ, một tiến trình đang chạy sẽ bị buộc phải xóa khỏi trạng thái đang chạy và được đặt trở lại hàng đợi sẵn sàng. Điều này đảm bảo rằng một tiến trình chạy dài không thể bỏ đói tất cả các tiến trình khác khỏi việc chạy.
Khoảng thời gian tối đa mà một tiến trình có thể thực thi trước khi được trả về hàng đợi sẵn sàng được gọi là **lượng tử thời gian**. Trong giới hạn của lượng tử thời gian lớn (trong đó lượng tử thời gian dài hơn thời gian chạy của tất cả các tiến trình), vòng tròn sẽ tương đương với FCFS.

![](http://i.imgur.com/AlBYi0Y.png)

* P1 Đến: 0ms
* P2 Đến: 0ms
* P3 Đến: 0ms
* P4 Đến: 0ms
* P5 Đến: 0ms

Lượng tử = 1000ms

Ở đây, tất cả các tiến trình đều đến cùng một lúc. P1 được chạy trong 1 lượng tử và kết thúc. P2 trong một lượng tử; sau đó, nó bị dừng lại cho P3. Sau khi tất cả các tiến trình khác chạy trong một lượng tử, chúng ta quay trở lại P2 cho đến khi tất cả các tiến trình kết thúc.

**Ưu điểm**
* Đảm bảo một số khái niệm về sự công bằng

**Nhược điểm**
* Số lượng lớn các tiến trình = Rất nhiều chuyển đổi ngữ cảnh

# Ưu tiên

Các tiến trình được lên lịch theo thứ tự giá trị ưu tiên. Ví dụ: một tiến trình điều hướng có thể quan trọng hơn để thực thi hơn một tiến trình ghi nhật ký.


