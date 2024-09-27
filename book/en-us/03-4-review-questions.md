# Các chủ đề
* Phù hợp nhất
* Phù hợp tồi nhất
* Phù hợp đầu tiên
* Bộ cấp phát Buddy
* Độ phân mảnh nội bộ
* Độ phân mảnh ngoại bộ
* sbrk
* Căn chỉnh tự nhiên
* Thẻ biên
* Hợp nhất
* Phân chia
* Cấp phát Slab/Bể bộ nhớ

# Câu hỏi/Bài tập

* Độ phân mảnh nội bộ là gì? Khi nào nó trở thành vấn đề?
    - Answer: 
        + Độ phân mảnh nội bộ trong lập trình C xảy ra khi bộ nhớ được cấp phát cho một biến hoặc một đối tượng nhưng không được sử dụng hết. Điều này thường xảy ra khi kích thước của khối bộ nhớ được cấp phát lớn hơn kích thước thực sự cần thiết. 
        +  Độ phân mảnh nội bộ trở thành vấn đề khi nó dẫn đến việc lãng phí bộ nhớ, làm giảm hiệu suất và hiệu quả của chương trình. Điều này đặc biệt quan trọng trong các hệ thống có bộ nhớ hạn chế hoặc khi chương trình cần phải chạy trong thời gian dài mà không có sự can thiệp của người dùng.


* Độ phân mảnh ngoại bộ là gì? Khi nào nó trở thành vấn đề?

    - Answer:
        + Độ phân mảnh ngoại bộ trong lập trình C xảy ra khi bộ nhớ được cấp phát và giải phóng theo cách mà các khối bộ nhớ trống không liên tục trên bộ nhớ heap. Điều này có thể dẫn đến việc không thể cấp phát một khối bộ nhớ liên tục lớn, mặc dù có đủ bộ nhớ tổng cộng.

        + Độ phân mảnh ngoại bộ trở thành vấn đề khi nó ngăn chặn việc cấp phát bộ nhớ cho các đối tượng lớn. Điều này có thể làm giảm hiệu suất và hiệu quả của chương trình, đặc biệt là trong các ứng dụng yêu cầu nhiều bộ nhớ liên tục như xử lý đồ họa hoặc xử lý dữ liệu lớn.

* Chiến lược đặt phù hợp nhất là gì? Nó như thế nào với độ phân mảnh ngoại bộ? Độ phức tạp thời gian?

    - Answer:
        + Chiến lược đặt phù hợp nhất (Best Fit) là một chiến lược quản lý bộ nhớ trong đó hệ thống tìm kiếm toàn bộ danh sách các khối bộ nhớ trống và chọn khối nhỏ nhất đủ để đáp ứng yêu cầu cấp phát bộ nhớ.

        + Về mặt độ phân mảnh ngoại bộ, chiến lược Best Fit có thể giảm thiểu độ phân mảnh ngoại bộ bằng cách sử dụng các khối bộ nhớ trống nhỏ nhất có thể để đáp ứng yêu cầu cấp phát. Tuy nhiên, nó có thể tạo ra nhiều khối bộ nhớ trống nhỏ, dẫn đến độ phân mảnh nội bộ.

        + Về độ phức tạp thời gian, chiến lược Best Fit có thể yêu cầu thời gian tìm kiếm lớn nếu có nhiều khối bộ nhớ trống, vì hệ thống phải tìm kiếm toàn bộ danh sách để tìm khối nhỏ nhất phù hợp. Độ phức tạp thời gian trung bình là O(n), với n là số lượng khối bộ nhớ trống.


* Chiến lược đặt phù hợp tồi nhất là gì? Nó có tốt hơn với độ phân mảnh ngoại bộ không? Độ phức tạp thời gian?

    - Answer:
        + Chiến lược đặt phù hợp tồi nhất (Worst Fit) là một chiến lược quản lý bộ nhớ trong đó hệ thống tìm kiếm toàn bộ danh sách các khối bộ nhớ trống và chọn khối lớn nhất để đáp ứng yêu cầu cấp phát bộ nhớ.

        + Về mặt độ phân mảnh ngoại bộ, chiến lược Worst Fit có thể tạo ra nhiều khối bộ nhớ trống nhỏ hơn, dẫn đến tăng độ phân mảnh ngoại bộ. Điều này xảy ra vì nó luôn chọn khối bộ nhớ trống lớn nhất, để lại nhiều khối nhỏ hơn không được sử dụng.

        + Về độ phức tạp thời gian, chiến lược Worst Fit cũng yêu cầu thời gian tìm kiếm lớn nếu có nhiều khối bộ nhớ trống, vì hệ thống phải tìm kiếm toàn bộ danh sách để tìm khối lớn nhất phù hợp. Độ phức tạp thời gian trung bình là O(n), với n là số lượng khối bộ nhớ trống.

* Chiến lược đặt phù hợp đầu tiên là gì? Nó hơi tốt hơn với độ phân mảnh, phải không? Độ phức tạp thời gian dự kiến?
* Giả sử rằng chúng ta đang sử dụng bộ cấp phát buddy với một slab mới 64kb. Làm thế nào nó cấp phát 1.5kb?
* Khi nào thì cài đặt `sbrk` 5 dòng của malloc có ích?
* Căn chỉnh tự nhiên là gì?
* Hợp nhất/Phân chia là gì? Làm thế nào chúng tăng/giảm độ phân mảnh? Khi nào bạn có thể hợp nhất hoặc phân chia?
* Các thẻ biên hoạt động như thế nào? Làm thế nào chúng có thể được sử dụng để hợp nhất hoặc phân chia?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-3%3A-Smashing-the-Stack-Example">
Quay lại: Bộ nhớ, Phần 3: Ví dụ về Smashing the Stack
</a>
</div>