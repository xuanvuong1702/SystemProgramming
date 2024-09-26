## Chủ đề
* Sử dụng đúng fork, exec và waitpid
* Sử dụng exec với một đường dẫn
* Hiểu những gì fork, exec và waitpid làm. Ví dụ: cách sử dụng giá trị trả về của chúng.
* SIGKILL so với SIGSTOP so với SIGINT.
* Tín hiệu nào được gửi khi bạn nhấn CTRL-C
* Sử dụng kill từ shell hoặc lệnh kill POSIX.
* Cô lập bộ nhớ quy trình.
* Bố cục bộ nhớ quy trình (vị trí của heap, stack, v.v.; địa chỉ bộ nhớ không hợp lệ).
* Fork bomb, zombie và orphan là gì? Cách tạo/xóa chúng.
* getpid so với getppid
* Cách sử dụng các macro trạng thái thoát WAIT như WIFEXITED, v.v.

# Câu hỏi/Bài tập

* Sự khác biệt giữa execs có p và không có p là gì? Hệ điều hành là gì?
* Làm thế nào để truyền các đối số dòng lệnh vào `execl*`? Còn `execv*`? Đối số dòng lệnh đầu tiên theo quy ước nên là gì?
* Làm thế nào để biết nếu `exec` hoặc `fork` thất bại?
* `int *status` trỏ vào wait là gì? Khi nào wait thất bại?
* Một số khác biệt giữa `SIGKILL`, `SIGSTOP`, `SIGCONT`, `SIGINT` là gì? Những hành vi mặc định là gì? Bạn có thể thiết lập trình xử lý tín hiệu cho những tín hiệu nào?
* Tín hiệu nào được gửi khi bạn nhấn `CTRL-C`?
* Terminal của tôi được gắn vào PID = 1337 và vừa trở nên không phản hồi. Viết cho tôi lệnh terminal và mã C để gửi `SIGQUIT` cho nó.
* Một quy trình có thể thay đổi bộ nhớ của quy trình khác thông qua phương tiện bình thường không? Tại sao?
* Heap, stack, dữ liệu, và phân đoạn văn bản ở đâu? Bạn có thể ghi vào phân đoạn nào? Địa chỉ bộ nhớ không hợp lệ là gì?
* Viết cho tôi một fork bomb bằng C (xin đừng chạy nó).
* Orphan là gì? Làm thế nào nó trở thành zombie? Làm thế nào tôi có thể làm một bậc cha mẹ tốt?
* Bạn có ghét khi cha mẹ bạn nói rằng bạn không thể làm điều gì đó không? Viết cho tôi một chương trình gửi `SIGSTOP` cho cha mẹ của bạn.
* Viết một hàm fork exec chờ một chương trình thực thi, và sử dụng các macro wait cho tôi biết nếu quy trình đã thoát bình thường hay nếu nó đã được gửi tín hiệu. Nếu quy trình thoát bình thường, thì in điều đó với giá trị trả về. Nếu không, thì in số tín hiệu đã khiến quy trình kết thúc.
<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Process-Control,-Part-1:-Wait-macros,-using-signals">
Quay lại: Kiểm soát quy trình, Phần 1: Macro Wait, sử dụng tín hiệu
</a>
</div>