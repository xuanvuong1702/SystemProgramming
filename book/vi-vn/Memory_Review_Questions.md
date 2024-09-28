# Chủ đề
* Vừa khít nhất (Best Fit)
* Vừa kém nhất (Worst Fit)
* Vừa đầu tiên (First Fit)
* Bộ cấp phát Buddy
* Phân mảnh nội bộ
* Phân mảnh ngoài
* sbrk
* Căn chỉnh tự nhiên
* Thẻ ranh giới
* Hợp nhất
* Tách
* Cấp phát Slab / Pool bộ nhớ

# Câu hỏi / Bài tập
* Phân mảnh nội bộ là gì? Khi nào nó trở thành vấn đề?
* Phân mảnh ngoài là gì? Khi nào nó trở thành vấn đề?
* Chiến lược sắp xếp Vừa khít nhất là gì? Nó như thế nào với phân mảnh ngoài? Độ phức tạp thời gian?
* Chiến lược sắp xếp Vừa kém nhất là gì? Nó có tốt hơn với phân mảnh ngoài không? Độ phức tạp thời gian?
* Chiến lược sắp xếp Vừa đầu tiên là gì? Nó tốt hơn một chút với phân mảnh, phải không? Độ phức tạp thời gian dự kiến?
* Giả sử rằng chúng ta đang sử dụng bộ cấp phát Buddy với một **slab** mới là 64kb. Làm cách nào để phân bổ 1,5kb?
* Việc triển khai malloc `sbrk` 5 dòng khi nào được sử dụng?
* **Căn chỉnh tự nhiên** là gì?
* **Hợp nhất / Tách** là gì? Làm thế nào chúng làm tăng / giảm phân mảnh? Khi nào bạn có thể hợp nhất hoặc tách?
* **Thẻ ranh giới** hoạt động như thế nào? Làm thế nào chúng có thể được sử dụng để hợp nhất hoặc tách?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-3%3A-Smashing-the-Stack-Example">
Quay lại: Bộ nhớ, Phần 3: Ví dụ về Phá hủy Ngăn xếp
</a>
</div>



