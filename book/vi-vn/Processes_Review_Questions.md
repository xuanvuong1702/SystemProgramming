## Chủ đề

* Sử dụng đúng fork, exec và waitpid
* Sử dụng exec với đường dẫn
* Hiểu fork, exec và waitpid làm gì. Ví dụ: cách sử dụng giá trị trả về của chúng.
* SIGKILL vs SIGSTOP vs SIGINT.
* Tín hiệu nào được gửi khi bạn nhấn CTRL-C
* Sử dụng kill từ shell hoặc lệnh gọi POSIX kill.
* Cách ly bộ nhớ tiến trình.
* Bố cục bộ nhớ tiến trình (heap, stack, v.v. ở đâu; địa chỉ bộ nhớ không hợp lệ).
* Fork bomb, zombie và orphan là gì? Cách tạo/xóa chúng.
* getpid vs getppid
* Cách sử dụng các macro trạng thái thoát WAIT WIFEXITED, v.v.


# Câu hỏi/Bài tập

* Sự khác biệt giữa các hàm exec có p và không có p là gì? Hệ điều hành làm gì 
* Làm cách nào để truyền đối số dòng lệnh vào `execl*`? Còn `execv*` thì sao? Theo quy ước, đối số dòng lệnh đầu tiên nên là gì?
* Làm cách nào để biết `exec` hoặc `fork` có thất bại hay không?
* Con trỏ `int *status` được truyền vào wait là gì? Khi nào wait thất bại?
* Một số điểm khác biệt giữa `SIGKILL`, `SIGSTOP`, `SIGCONT`, `SIGINT` là gì? Các hành vi mặc định là gì? Bạn có thể thiết lập trình xử lý tín hiệu cho những tín hiệu nào?
* Tín hiệu nào được gửi khi bạn nhấn `CTRL-C`?
* Terminal của tôi được neo vào PID = 1337 và vừa không phản hồi. Viết cho tôi lệnh terminal và mã C để gửi `SIGQUIT` đến nó.
* Một tiến trình có thể thay đổi bộ nhớ của tiến trình khác bằng các phương tiện thông thường không? Tại sao?
* Heap, stack, data và text segment ở đâu? Bạn có thể ghi vào những segment nào? Địa chỉ bộ nhớ không hợp lệ là gì?
* Viết cho tôi một fork bomb bằng C (vui lòng đừng chạy nó).
* Orphan là gì? Làm thế nào nó trở thành zombie? Làm thế nào để trở thành một tiến trình cha tốt?
* Bạn có ghét khi cha mẹ nói với bạn rằng bạn không thể làm điều gì đó không? Viết cho tôi một chương trình gửi `SIGSTOP` đến tiến trình cha của bạn.
* Viết một hàm fork exec chờ một tệp thực thi và sử dụng các macro wait cho tôi biết liệu tiến trình đã thoát bình thường hay bị tín hiệu. Nếu tiến trình thoát bình thường, thì hãy in ra điều đó với giá trị trả về. Nếu không, thì hãy in ra số tín hiệu khiến tiến trình kết thúc.
<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Process-Control,-Part-1:-Wait-macros,-using-signals">
Quay lại: Kiểm soát tiến trình, Phần 1: Macro Wait, sử dụng tín hiệu
</a>
</div>
