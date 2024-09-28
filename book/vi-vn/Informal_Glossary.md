Cảnh báo: Không giống như bảng chú giải thuật ngữ đầy đủ, bảng chú giải thuật ngữ không chính thức này bỏ qua các chi tiết và cung cấp giải thích đơn giản và dễ hiểu cho từng thuật ngữ. Để biết thêm thông tin và chi tiết, hãy sử dụng công cụ tìm kiếm web yêu thích của bạn.

## Kernel là gì?
Kernel là phần trung tâm của hệ điều hành, quản lý các **tiến trình**, tài nguyên (bao gồm cả **bộ nhớ**) và các thiết bị **nhập/xuất** phần cứng. Các chương trình người dùng tương tác với kernel bằng cách thực hiện các **lệnh gọi hệ thống**.

Tìm hiểu thêm:
[[http://en.wikipedia.org/wiki/Kernel_%28operating_system%29]]

## Tiến trình là gì?

Tiến trình là một phiên bản của chương trình đang chạy trên máy. Có thể có nhiều tiến trình của cùng một chương trình. Ví dụ, bạn và tôi có thể đều đang chạy 'cat' hoặc 'gnuchess'

Một tiến trình chứa mã chương trình và thông tin trạng thái có thể sửa đổi như **biến**, **tín hiệu**, **bộ mô tả tệp** đang mở cho các tệp, kết nối mạng và các tài nguyên hệ thống khác được lưu trữ bên trong bộ nhớ của tiến trình. Hệ điều hành cũng lưu trữ **siêu thông tin** về tiến trình được hệ thống sử dụng để quản lý và giám sát hoạt động và mức sử dụng tài nguyên của tiến trình.

Tìm hiểu thêm:
[[http://en.wikipedia.org/wiki/Process_%28computing%29]]

## Bộ nhớ ảo là gì?
Các tiến trình chạy trên điện thoại thông minh và máy tính xách tay của bạn sử dụng **bộ nhớ ảo**: Mỗi tiến trình được **cách ly** với các tiến trình khác và dường như có quyền truy cập đầy đủ vào tất cả các **địa chỉ bộ nhớ** có thể! Trên thực tế, chỉ một phần nhỏ **không gian địa chỉ** của tiến trình được **ánh xạ** tới bộ nhớ vật lý và lượng bộ nhớ vật lý thực tế được phân bổ cho một tiến trình có thể thay đổi theo thời gian và được **phân trang** ra đĩa, ánh xạ lại và chia sẻ an toàn với các tiến trình khác. Bộ nhớ ảo cung cấp những lợi ích đáng kể bao gồm khả năng cách ly tiến trình mạnh mẽ (bảo mật), lợi ích về tài nguyên và hiệu suất (sử dụng bộ nhớ vật lý đơn giản và hiệu quả) mà chúng ta sẽ thảo luận sau. 

Tìm hiểu thêm:
[[http://en.wikipedia.org/wiki/Virtual_memory]]



