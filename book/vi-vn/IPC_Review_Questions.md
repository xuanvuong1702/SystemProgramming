# Chủ đề
* Bộ nhớ ảo
* Bảng trang
* MMU/TLB
* Dịch địa chỉ
* Lỗi trang
* Khung/Trang
* Bảng trang một cấp so với nhiều cấp
* Tính toán độ lệch cho bảng trang nhiều cấp
* Ống (Pipe)
* Đầu đọc ghi của ống
* Ghi vào ống không có tiến trình đọc
* Đọc từ ống không có tiến trình ghi
* Ống có tên và Ống không tên
* Kích thước bộ đệm / Tính nguyên tử
* Thuật toán lập lịch
* Thước đo hiệu quả

# Câu hỏi
* Bộ nhớ ảo là gì?
* Những điều sau đây là gì và mục đích của chúng là gì?
    * Bộ đệm tra cứu dịch (Translation Lookaside Buffer)
    * Địa chỉ vật lý
    * Đơn vị quản lý bộ nhớ (Memory Management Unit). Bảng trang nhiều cấp. Số khung. Số trang và độ lệch trang.
    * Bit bẩn (Dirty bit)
    * Bit NX (NX Bit)
* Bảng trang là gì? Còn khung vật lý là gì? Một trang có luôn cần trỏ đến một khung vật lý không?
* Lỗi trang là gì? Các loại là gì? Khi nào nó dẫn đến **lỗi phân đoạn** (segfault)?
* Ưu điểm của bảng trang một cấp là gì? Nhược điểm? Còn bảng nhiều cấp thì sao?
* Bảng nhiều cấp trông như thế nào trong bộ nhớ?
* Làm cách nào để bạn xác định có bao nhiêu bit được sử dụng trong **độ lệch trang**?
* Cho không gian địa chỉ 64 bit, các trang và khung 4kb và bảng trang 3 cấp, số bit của Số trang ảo 1 (VPN1), VPN2, VPN3 và độ lệch là bao nhiêu?
* Ống là gì? Làm cách nào để tạo một ống?
* Khi nào SIGPIPE được gửi đến một tiến trình?
* Trong điều kiện nào, việc gọi `read()` trên một ống sẽ bị chặn? Trong điều kiện nào `read()` sẽ trả về 0 ngay lập tức
* Sự khác biệt giữa ống có tên và ống không tên là gì?
* Ống có an toàn cho luồng không?
* Viết một hàm sử dụng `fseek` và `ftell` để thay thế ký tự ở giữa của một tệp bằng 'X'
* Viết một hàm tạo một ống và sử dụng `write` để gửi 5 byte, "HELLO" đến ống. Trả về **bộ mô tả tệp** đọc của ống.
* Điều gì xảy ra khi bạn ánh xạ bộ nhớ một tệp?
* Tại sao không nên lấy kích thước tệp bằng `ftell`? Bạn nên làm điều đó như thế nào thay thế?
* Lập lịch là gì?
* Thời gian quay vòng là gì? Thời gian phản hồi? Thời gian chờ đợi?
* **Hiệu ứng đoàn tàu** là gì?
* Thuật toán nào có thời gian quay vòng / phản hồi / chờ đợi trung bình tốt nhất

