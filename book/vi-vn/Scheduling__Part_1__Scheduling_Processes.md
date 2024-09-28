# Suy nghĩ về lập lịch

[Lập lịch CPU](https://en.wikipedia.org/wiki/Scheduling_(computing)) là bài toán lựa chọn hiệu quả tiến trình nào để chạy trên các lõi CPU của hệ thống. Trong một hệ thống bận rộn, sẽ có nhiều tiến trình sẵn sàng chạy hơn số lõi CPU, vì vậy kernel hệ thống phải đánh giá tiến trình nào nên được lên lịch để chạy trên CPU và tiến trình nào nên được đặt trong hàng đợi sẵn sàng để được thực thi sau.

Độ phức tạp bổ sung của đa luồng và nhiều lõi CPU được coi là yếu tố gây xao nhãng cho phần trình bày ban đầu này nên bị bỏ qua ở đây.

Một điểm cần lưu ý khác cho những người không phải là người bản ngữ là ý nghĩa kép của "Thời gian": Từ "Thời gian" có thể được sử dụng trong cả ngữ cảnh đồng hồ và khoảng thời gian đã trôi qua. Ví dụ: "Thời gian đến của tiến trình đầu tiên là 9:00 sáng" và "Thời gian chạy của thuật toán là 3 giây".

## Lập lịch được đo lường như thế nào và bộ lập lịch nào là tốt nhất?

Lập lịch ảnh hưởng đến hiệu suất của hệ thống, cụ thể là *độ trễ* và *thông lượng* của hệ thống. Thông lượng có thể được đo bằng một giá trị hệ thống, ví dụ: thông lượng I/O - số byte được ghi mỗi giây hoặc số lượng các tiến trình nhỏ có thể hoàn thành trên một đơn vị thời gian hoặc sử dụng mức độ trừu tượng hóa cao hơn, ví dụ: số lượng bản ghi khách hàng được xử lý mỗi phút. Độ trễ có thể được đo bằng thời gian phản hồi (khoảng thời gian trôi qua trước khi một tiến trình có thể bắt đầu gửi phản hồi) hoặc thời gian chờ hoặc thời gian quay vòng (khoảng thời gian trôi qua để hoàn thành một nhiệm vụ). Các bộ lập lịch khác nhau cung cấp các sự đánh đổi tối ưu hóa khác nhau có thể hoặc không phù hợp với mục đích sử dụng mong muốn - không có bộ lập lịch tối ưu cho tất cả các môi trường và mục tiêu có thể có. Ví dụ: 'công việc ngắn nhất trước' (Shortest Job First - SJF) sẽ giảm thiểu tổng thời gian chờ trên tất cả các công việc nhưng trong môi trường tương tác (UI), sẽ tốt hơn nếu giảm thiểu thời gian phản hồi (bằng cách đánh đổi một số thông lượng), trong khi FCFS (First Come First Served - đến trước phục vụ trước) có vẻ trực quan công bằng và dễ dàng triển khai nhưng lại gặp phải Hiệu ứng đoàn tàu.

## Thời gian đến là gì?

Thời điểm mà một tiến trình lần đầu tiên đến hàng đợi sẵn sàng và sẵn sàng bắt đầu thực thi. Nếu CPU đang rảnh, thời gian đến cũng sẽ là thời gian bắt đầu thực thi.

## Tiền chiếm (Preemption) là gì?

Nếu không có tiền chiếm, các tiến trình sẽ chạy cho đến khi chúng không thể sử dụng CPU thêm nữa. Ví dụ: các điều kiện sau sẽ loại bỏ một tiến trình khỏi CPU và CPU sẽ có sẵn để được lên lịch cho các tiến trình khác: Tiến trình kết thúc do tín hiệu, bị chặn chờ nguyên thủy đồng thời hoặc thoát bình thường.
Do đó, một khi một tiến trình được lên lịch, nó sẽ tiếp tục ngay cả khi một tiến trình khác có mức độ ưu tiên cao hơn (ví dụ: công việc ngắn hơn) xuất hiện trên hàng đợi sẵn sàng.

Với tiền chiếm, các tiến trình hiện có có thể bị xóa ngay lập tức nếu một tiến trình thích hợp hơn được thêm vào hàng đợi sẵn sàng. Ví dụ: giả sử tại t = 0 với bộ lập lịch Công việc ngắn nhất trước, có hai tiến trình (P1 P2) với thời gian thực thi lần lượt là 10 và 20 ms. P1 được lên lịch. P1 ngay lập tức tạo một tiến trình mới P3, với thời gian thực thi là 5 ms, được thêm vào hàng đợi sẵn sàng. Nếu không có tiền chiếm, P3 sẽ chạy sau 10ms (sau khi P1 đã hoàn thành). Với tiền chiếm, P1 sẽ bị đuổi khỏi CPU ngay lập tức và thay vào đó được đặt trở lại hàng đợi sẵn sàng và P3 sẽ được CPU thực thi thay thế.

## Bộ lập lịch nào bị chết đói (starvation)?
Bất kỳ bộ lập lịch nào sử dụng hình thức ưu tiên đều có thể dẫn đến chết đói vì các tiến trình trước đó có thể không bao giờ được lên lịch để chạy (được gán CPU). Ví dụ: với SJF, các công việc dài hơn có thể không bao giờ được lên lịch nếu hệ thống tiếp tục có nhiều công việc ngắn để lên lịch. Tất cả phụ thuộc vào [loại bộ lập lịch](https://en.wikipedia.org/wiki/Scheduling_(computing)#Types_of_operating_system_schedulers).

## Tại sao một tiến trình (hoặc luồng) có thể được đặt vào hàng đợi sẵn sàng?

Một tiến trình được đặt vào hàng đợi sẵn sàng khi nó có thể sử dụng CPU. Một số ví dụ bao gồm:
* Một tiến trình đã bị chặn đang chờ `read` từ bộ nhớ hoặc socket hoàn thành và dữ liệu hiện đã có sẵn.
* Một tiến trình mới đã được tạo và sẵn sàng để bắt đầu.
* Một luồng tiến trình đã bị chặn trên một nguyên thủy đồng bộ hóa (biến điều kiện, semaphore, khóa mutex) nhưng bây giờ có thể tiếp tục.
* Một tiến trình bị chặn đang chờ lệnh gọi hệ thống hoàn thành nhưng một tín hiệu đã được gửi và trình xử lý tín hiệu cần chạy.

Các ví dụ tương tự có thể được tạo khi xem xét các luồng.

# Các thước đo hiệu quả

`start_time` là thời gian bắt đầu theo đồng hồ treo tường của tiến trình (CPU bắt đầu làm việc trên đó)
`end_time` là thời gian kết thúc theo đồng hồ treo tường của tiến trình (CPU kết thúc tiến trình)
`run_time` là tổng thời gian CPU cần
`arrival_time` là thời gian tiến trình vào bộ lập lịch (CPU có thể không bắt đầu làm việc trên đó)

## 'Thời gian quay vòng' là gì?

Tổng thời gian từ khi tiến trình đến cho đến khi nó kết thúc.

`turnaround_time = end_time - arrival_time`

## 'Thời gian phản hồi' là gì?

Tổng độ trễ (thời gian) mất từ ​​khi tiến trình đến cho đến khi CPU thực sự bắt đầu làm việc trên đó.

`response_time = start_time - arrival_time`

## 'Thời gian chờ' là gì?

Thời gian chờ là thời gian chờ *tổng cộng*, tức là tổng thời gian mà một tiến trình nằm trong hàng đợi sẵn sàng. Một sai lầm phổ biến là tin rằng đó chỉ là thời gian chờ ban đầu trong hàng đợi sẵn sàng.

Nếu một tiến trình sử dụng nhiều CPU mà không có I/O mất 7 phút thời gian CPU để hoàn thành nhưng cần 9 phút thời gian đồng hồ treo tường để hoàn thành, chúng ta có thể kết luận rằng nó đã được đặt trong hàng đợi sẵn sàng trong 2 phút. Trong 2 phút đó, tiến trình đã sẵn sàng để chạy nhưng không có CPU nào được gán. Không quan trọng khi nào công việc đang chờ, thời gian chờ là 2 phút.

`wait_time  = (end_time - arrival_time) - run_time`


## Hiệu ứng đoàn tàu (Convoy Effect) là gì?

"Hiệu ứng đoàn tàu là nơi các tiến trình sử dụng nhiều I/O liên tục bị sao lưu, chờ đợi các tiến trình sử dụng nhiều CPU chiếm dụng CPU. Điều này dẫn đến hiệu suất I/O kém, ngay cả đối với các tiến trình có nhu cầu CPU nhỏ."

Giả sử CPU hiện đang được gán cho một tác vụ sử dụng nhiều CPU và có một tập hợp các tiến trình sử dụng nhiều I/O đang ở trong hàng đợi sẵn sàng. Các tiến trình này chỉ yêu cầu một lượng nhỏ thời gian CPU nhưng chúng không thể tiếp tục vì chúng đang chờ tác vụ sử dụng nhiều CPU được gỡ bỏ khỏi bộ xử lý. Các tiến trình này bị bỏ đói cho đến khi tiến trình bị ràng buộc CPU giải phóng CPU. Nhưng CPU sẽ hiếm khi được giải phóng (ví dụ: trong trường hợp của bộ lập lịch FCFS, chúng ta phải đợi cho đến khi tiến trình bị chặn do yêu cầu I/O). Các tiến trình sử dụng nhiều I/O cuối cùng cũng có thể đáp ứng nhu cầu CPU của chúng, mà chúng có thể thực hiện nhanh chóng vì nhu cầu CPU của chúng nhỏ và CPU được gán lại cho tiến trình sử dụng nhiều CPU một lần nữa. Do đó, hiệu suất I/O của toàn bộ hệ thống bị ảnh hưởng thông qua tác động gián tiếp của việc bỏ đói nhu cầu CPU của tất cả các tiến trình.

Hiệu ứng này thường được thảo luận trong bối cảnh bộ lập lịch FCFS, tuy nhiên, bộ lập lịch Round Robin (vòng tròn) cũng có thể thể hiện Hiệu ứng đoàn tàu trong thời gian lượng tử dài.


## Lập lịch Linux
Kể từ tháng 2 năm 2016, Linux theo mặc định sử dụng *Bộ lập lịch hoàn toàn công bằng (Completely Fair Scheduler - CFS)* để lập lịch CPU và Lập lịch công bằng ngân sách "BFQ" để lập lịch I/O. Lập lịch thích hợp có thể có tác động đáng kể đến thông lượng và độ trễ. Độ trễ đặc biệt quan trọng đối với các ứng dụng tương tác và thời gian thực mềm như phát trực tuyến âm thanh và video. Xem thảo luận và điểm chuẩn so sánh [tại đây](https://lkml.org/lkml/2014/5/27/314) để biết thêm thông tin.

Dưới đây là cách CFS lên lịch:

* CPU tạo một cây Đỏ-Đen với thời gian chạy ảo của tiến trình (thời gian chạy / nice\_value) và độ công bằng của tiến trình đang ngủ (nếu tiến trình đang chờ một thứ gì đó, hãy cung cấp cho nó CPU khi nó hoàn thành việc chờ đợi).
* (Giá trị Nice là cách kernel ưu tiên cho một số tiến trình nhất định, giá trị Nice càng thấp thì mức độ ưu tiên càng cao)
* Kernel chọn giá trị thấp nhất dựa trên số liệu này và lên lịch cho tiến trình đó chạy tiếp theo, lấy nó ra khỏi hàng đợi. Vì cây đỏ-đen là tự cân bằng nên thao tác này được đảm bảo $O(log(n))$ (việc chọn tiến trình tối thiểu có cùng thời gian chạy).

Mặc dù nó được gọi là Bộ lập lịch công bằng nhưng có một số vấn đề công bằng.

* Các nhóm tiến trình được lên lịch có thể có tải không cân bằng, vì vậy bộ lập lịch phân phối tải gần đúng. Khi một CPU khác được giải phóng, nó chỉ có thể xem xét tải trung bình của một lịch trình nhóm chứ không phải các lõi riêng lẻ. Vì vậy, CPU miễn phí có thể không nhận công việc từ CPU đang hoạt động quá lâu miễn là mức trung bình ổn.
* Nếu một nhóm tiến trình đang chạy trên các lõi không liền kề thì có lỗi. Nếu hai lõi cách nhau hơn một bước nhảy, thuật toán cân bằng tải sẽ không xem xét lõi đó. Có nghĩa là nếu một CPU miễn phí và một CPU đang thực hiện nhiều công việc hơn cách nhau hơn một bước nhảy, nó sẽ không nhận công việc (có thể đã được vá).
* Sau khi một luồng đi ngủ trên một tập hợp con của các lõi, khi nó thức dậy, nó chỉ có thể được lên lịch trên các lõi mà nó đang ngủ. Nếu các lõi đó hiện đang bận, luồng sẽ phải đợi trên chúng, lãng phí cơ hội sử dụng các lõi nhàn rỗi khác.
* Để đọc thêm về các vấn đề của Bộ lập lịch công bằng, hãy đọc [tại đây](https://blog.acolyer.org/2016/04/26/the-linux-scheduler-a-decade-of-wasted-cores).

