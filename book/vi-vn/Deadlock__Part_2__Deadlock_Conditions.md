## Điều kiện Coffman
Có bốn điều kiện **cần** và **đủ** để xảy ra deadlock. Chúng được gọi là điều kiện Coffman.

* Loại trừ lẫn nhau (Mutual Exclusion)
* Chờ đợi vòng (Circular Wait)
* Giữ và chờ đợi (Hold and Wait)
* Không chiếm đoạt (No Preemption)

Nếu bạn phá vỡ bất kỳ điều kiện nào trong số chúng, bạn không thể gặp deadlock!

Tất cả các điều kiện này đều cần thiết cho deadlock, vì vậy hãy thảo luận lần lượt từng điều kiện. Đầu tiên là những điều kiện dễ hiểu-
* **Loại trừ lẫn nhau**: Tài nguyên không thể được chia sẻ.
* **Chờ đợi vòng**: Tồn tại một chu kỳ trong Biểu đồ phân bổ tài nguyên. Tồn tại một tập hợp các tiến trình {P1, P2, ...} sao cho P1 đang chờ tài nguyên được giữ bởi P2, P2 đang chờ P3, ..., P3 đang chờ P1.
* **Giữ và chờ đợi**: Một tiến trình thu nhận một tập hợp tài nguyên chưa hoàn chỉnh và giữ chúng trong khi chờ các tài nguyên khác.
* **Không chiếm đoạt**: Khi một tiến trình đã thu nhận một tài nguyên, tài nguyên đó không thể bị lấy đi khỏi tiến trình và tiến trình sẽ không tự nguyện từ bỏ tài nguyên.

## Phá vỡ điều kiện Coffman

Hai sinh viên cần một cây bút và một tờ giấy:
* Các sinh viên chia sẻ một cây bút và một tờ giấy. Deadlock được tránh vì **Loại trừ lẫn nhau** không được yêu cầu.
* Cả hai sinh viên đồng ý lấy bút trước khi lấy giấy. Deadlock được tránh vì không thể có **Chờ đợi vòng**.
* Các sinh viên lấy cả bút và giấy trong một thao tác ("Lấy cả hai hoặc không lấy gì cả"). Deadlock được tránh vì không có **Giữ và chờ đợi**.
* Các sinh viên là bạn bè và sẽ yêu cầu nhau từ bỏ một tài nguyên đang giữ. Deadlock được tránh vì cho phép **chiếm đoạt**.


## Livelock
Livelock **không phải** là deadlock.

Hãy xem xét 'giải pháp' sau đây:
* Các sinh viên sẽ đặt xuống một tài nguyên đang giữ nếu họ không thể nhặt được tài nguyên khác trong vòng 10 giây. Giải pháp này tránh được deadlock tuy nhiên nó có thể bị livelock.

Livelock xảy ra khi một tiến trình tiếp tục thực thi nhưng không thể tiến triển.
Trong thực tế, Livelock có thể xảy ra bởi vì lập trình viên đã thực hiện các bước để tránh deadlock. Trong ví dụ trên, trong một hệ thống bận rộn, sinh viên sẽ liên tục giải phóng tài nguyên đầu tiên vì họ không bao giờ có thể thu nhận được tài nguyên thứ hai. Hệ thống không bị deadlock (tiến trình sinh viên vẫn đang thực thi) tuy nhiên nó cũng không đạt được bất kỳ tiến triển nào.

## Ngăn chặn / Tránh Deadlock so với Phát hiện Deadlock

**Ngăn chặn deadlock** là đảm bảo rằng deadlock không thể xảy ra, nghĩa là bạn phá vỡ một điều kiện Coffman. Điều này hoạt động tốt nhất bên trong một chương trình duy nhất và kỹ sư phần mềm đưa ra lựa chọn để phá vỡ một điều kiện Coffman nhất định. Hãy xem xét [Thuật toán Banker](https://en.wikipedia.org/wiki/Banker's_algorithm). Nó là một thuật toán khác để tránh deadlock. Toàn bộ việc triển khai nằm ngoài phạm vi của lớp này, chỉ cần biết rằng có nhiều thuật toán tổng quát hơn cho hệ điều hành.

**Phát hiện deadlock**, mặt khác, cho phép hệ thống đi vào trạng thái deadlock. Sau khi vào, hệ thống sử dụng thông tin mà nó có để phá vỡ deadlock. Ví dụ, hãy xem xét nhiều tiến trình truy cập tệp. Hệ điều hành có thể theo dõi tất cả các tệp/tài nguyên thông qua bộ mô tả tệp ở một số cấp độ (bất kể là thông qua API hay trực tiếp). Nếu hệ điều hành phát hiện một chu kỳ có hướng trong bảng bộ mô tả tệp của hệ điều hành, nó có thể phá vỡ quyền giữ của một tiến trình (ví dụ: thông qua lập lịch) và để hệ thống tiếp tục.

## Bài toán Triết gia ăn tối

Bài toán Triết gia ăn tối là một bài toán đồng bộ hóa cổ điển. Hãy tưởng tượng tôi mời N (giả sử 5) triết gia đến dự bữa ăn. Chúng tôi sẽ cho họ ngồi vào một bàn với 5 chiếc đũa (mỗi triết gia có một chiếc). Một triết gia luân phiên giữa việc muốn ăn hoặc suy nghĩ. Để ăn, triết gia phải nhặt hai chiếc đũa ở hai bên vị trí của họ (bài toán ban đầu yêu cầu mỗi triết gia phải có hai chiếc dĩa). Tuy nhiên, những chiếc đũa này được chia sẻ với người hàng xóm của anh ta.

![5DiningPhilosophers](https://raw.githubusercontent.com/wiki/angrave/SystemProgramming/5DiningPhilosophers.png)

Có thể thiết kế một giải pháp hiệu quả sao cho tất cả các triết gia đều được ăn không? Hay, một số triết gia sẽ chết đói, không bao giờ có được chiếc đũa thứ hai? Hay tất cả họ sẽ bị deadlock? Ví dụ: hãy tưởng tượng mỗi vị khách nhặt chiếc đũa bên trái của họ và sau đó chờ chiếc đũa bên phải của họ được giải phóng. Ôi - các triết gia của chúng ta đã bị deadlock!







