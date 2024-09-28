# Lời nói đầu

Chào mừng bạn đến với cuốn sách wiki Lập trình Hệ thống do cộng đồng đóng góp của Angrave!
Wiki này đã được tích cực xây dựng và duy trì từ năm 2014-2018 bởi sinh viên và giảng viên từ Đại học Illinois. Đó là một thử nghiệm viết tác giả từ cộng đồng bởi Lawrence Angrave từ CS @ Illinois và đại diện cho nội dung khóa học CS241 (nay là CS341) "Giới thiệu về Lập trình Hệ thống" của Angrave từ thời điểm đó. Wiki này sẽ luôn có sẵn cho mọi người để tìm hiểu thêm về lập trình hệ thống. Vui lòng cho tôi biết (angrave @ illinois) nếu bạn thấy nó hữu ích!

Sinh viên Đại học Illinois nên sử dụng sách giáo khoa dành riêng cho CS341 -

http://cs341.cs.illinois.edu/coursebook/index.html

# Nội dung

## 0. Giới thiệu
* [[Bảng thuật ngữ không chính thức]]
* [[Thủ thuật lập trình, Phần 1]]
* [[Truyện ngắn và Bài hát về Lập trình Hệ thống]]
* [[Lỗi Lập trình Hệ thống trong Công nghiệp]]

## 1. Học C
* [[Lập trình C, Phần 1: Giới thiệu]]
* [[Lập trình C, Phần 2: Nhập và Xuất Văn bản]]
   * [In ra / Đọc từ Luồng](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output#printing-to-streams)
   * [Phân tích cú pháp Đầu vào](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output#parsing-input)
* [[Lập trình C, Phần 3: Các lỗi thường gặp]]
   * [Lỗi về Bộ nhớ](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#memory-mistakes)
   * [Lỗi Logic / Luồng Chương trình](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#logic-and-program-flow-mistakes)
   * [Các lỗi thường gặp khác](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#other-gotchas)
* [[Lập trình C, Phần 4: Chuỗi và Cấu trúc]]
   * [Chuỗi](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs#strings-structs-and-gotchas)
   * [Cấu trúc](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs#so-whats-a-struct)
* [[Lập trình C, Phần 5: Gỡ lỗi]]
   * [Gỡ lỗi trong Mã](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#in-code-debugging)
   * [Valgrind](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#valgrind)
   * [Tsan](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#tsan)
   * [GDB](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#gdb)
* [[Lập trình C, Câu hỏi Ôn tập]]

## 2. Tiến trình
* [[Kernel, Shell, Thiết bị đầu cuối Ôi chao!]]
* [[Tiến trình, Phần 1: Giới thiệu]]
   * [Tổng quan](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#overview)
   * [Nội dung Tiến trình](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#process-contents)
   * [Phần thưởng: Thêm nội dung](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#process-functionslimitations-bonus)
* [[Phân nhánh, Phần 1: Giới thiệu]]
   * [Giới thiệu](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction#intro-to-fork)
   * [Chờ đợi và Thực thi](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction#waiting-and-execing)
* [[Phân nhánh, Phần 2: Fork, Exec, Wait]]
   * [Mẫu](https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-2:-Fork,-Exec,-Wait#the-pattern)
   * [Zombie](https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-2:-Fork,-Exec,-Wait#zombies)
* [[Kiểm soát Tiến trình, Phần 1: Macro đợi, sử dụng tín hiệu]]
   * [Macro đợi](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#wait-macros)
   * [Tín hiệu](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#signals)
* [[Câu hỏi Ôn tập về Tiến trình]]

## 3. Bộ nhớ và Bộ cấp phát
* [[Bộ nhớ, Phần 1: Giới thiệu về Bộ nhớ Heap]]
   * [Cấp phát Bộ nhớ động C](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction#c-dynamic-memory-allocation)
   * [Giới thiệu về Cấp phát](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction#intro-to-allocating)
* [[Bộ nhớ, Phần 2: Triển khai Bộ cấp phát Bộ nhớ]]
   * [Hướng dẫn về Bộ cấp phát Bộ nhớ](https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator#memory-allocator-tutorial)
* [[Bộ nhớ, Phần 3: Ví dụ về Phá hủy Ngăn xếp]]
* [[Câu hỏi Ôn tập về Bộ nhớ]]

## 4. Giới thiệu về Pthreads
* [[Pthreads, Phần 1: Giới thiệu]]
   * [Giới thiệu về Luồng](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction#intro-to-threads)
   * [Sử dụng Pthreads đơn giản](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction#simple-usage)
* [[Pthreads, Phần 2: Sử dụng trong Thực tế]]
   * [Thêm các Hàm pthread](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-2:-Usage-in-Practice#more-pthread-functions)
   * [Giới thiệu về Điều kiện Cuộc đua](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-2:-Usage-in-Practice#intro-to-race-conditions)
* [[Pthreads, Phần 3: Các Vấn đề Song song (Phần thưởng)]]
* [[Câu hỏi Ôn tập về Pthread]]

## 5. Đồng bộ hóa
* [[Đồng bộ hóa, Phần 1: Khóa Mutex]]
    * [Giải quyết Vùng tranh chấp](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-1:-Mutex-Locks#solving-critical-sections)
    * [Các lỗi thường gặp của Mutex](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-1:-Mutex-Locks#mutex-gotchas)
* [[Đồng bộ hóa, Phần 2: Semaphore đếm]]
* [[Đồng bộ hóa, Phần 3: Làm việc với Mutex và Semaphore]]
    * [Ngăn xếp An toàn cho Luồng](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-3%3A-Working-with-Mutexes-And-Semaphores#thread-safe-stack)
    * [Semaphore Ngăn xếp](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-3%3A-Working-with-Mutexes-And-Semaphores#stack-semaphores)
* [[Đồng bộ hóa, Phần 4: Vấn đề Vùng tranh chấp]]
    * [Các Giải pháp Ứng cử viên](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#candidate-solutions)
    * [Các Giải pháp Hoạt động](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#working-solutions)
    * [Các Giải pháp Phần cứng](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#hardware-solutions)
* [[Đồng bộ hóa, Phần 5: Biến Điều kiện]]
    * [Giới thiệu về Biến Điều kiện](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-5%3A-Condition-Variables#intro-to-condition-variables)
    * [Triển khai Semaphore Đếm](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-5%3A-Condition-Variables#implementing-counting-semphore)
* [[Đồng bộ hóa, Phần 6: Triển khai rào cản]]
* [[Đồng bộ hóa, Phần 7: Bài toán Người đọc Người viết]]
* [[Đồng bộ hóa, Phần 8: Ví dụ về Bộ đệm vòng]]
* [[Đồng bộ hóa, Phần 9: Đồng bộ hóa giữa các Tiến trình]]
* [[Câu hỏi Ôn tập về Đồng bộ hóa]]

## 6. Deadlock
* [[Deadlock, Phần 1: Biểu đồ phân bổ tài nguyên]]
* [[Deadlock, Phần 2: Điều kiện Deadlock]]
* [[Deadlock, Phần 3: Triết gia ăn tối]]
    * [Các Giải pháp Thất bại](https://github.com/angrave/SystemProgramming/wiki/Deadlock,-Part-3:-Dining-Philosophers#failed-solutions)
    * [Các Giải pháp Khả thi](https://github.com/angrave/SystemProgramming/wiki/Deadlock,-Part-3:-Dining-Philosophers#viable-solutions)
* [[Câu hỏi Ôn tập về Deadlock]]

## 7. Giao tiếp Liên tiến trình & Lập lịch
* [[Bộ nhớ ảo, Phần 1: Giới thiệu về Bộ nhớ ảo]]
    * [Bộ nhớ ảo là gì?](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory#what-is-virtual-memory)
    * [Khung Nâng cao và Bảo vệ Trang](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory#advanced-frames-and-page-protections)
* [[Đường ống, Phần 1: Giới thiệu về đường ống]]
* [[Đường ống, Phần 2: Bí mật lập trình đường ống]]
    * [Các lỗi thường gặp của Đường ống](https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#pipe-gotchas)
    * [Đường ống có tên](https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#named-pipes)
* [[ Tệp, Phần 1: Làm việc với tệp]]
* [[ Lập lịch, Phần 1: Lập lịch Tiến trình ]]
    * [Suy nghĩ về Lập lịch](https://github.com/angrave/SystemProgramming/wiki/Scheduling%2C-Part-1%3A-Scheduling-Processes#thinking-about-scheduling)
    * [Thước đo Hiệu quả](https://github.com/angrave/SystemProgramming/wiki/Scheduling%2C-Part-1%3A-Scheduling-Processes#measures-of-efficiency)
* [[ Lập lịch, Phần 2: Lập lịch Tiến trình: Thuật toán ]]
* [[ Câu hỏi Ôn tập về IPC]]

## 8. Mạng
* [[ POSIX, Phần 1: Xử lý lỗi]]
* [[ Mạng, Phần 1: Giới thiệu]]
* [[ Mạng, Phần 2: Sử dụng getaddrinfo ]]
* [[ Mạng, Phần 3: Xây dựng Máy khách TCP đơn giản ]]
* [[ Mạng, Phần 4: Xây dựng Máy chủ TCP đơn giản ]]
* [[ Mạng, Phần 5: Tắt cổng, sử dụng lại cổng và các thủ thuật khác ]]
* [[ Mạng, Phần 6: Tạo máy chủ UDP ]]
* [[ Mạng, Phần 7: I/O không chặn, select () và epoll ]]
* [[ Mạng, Phần 8: Giao thức (Bắt tay TCP, Độ trễ HTTP, Heartbleed) ]]
* [[ RPC, Phần 1: Giới thiệu về Gọi Thủ tục Từ xa ]]
* [[ Câu hỏi Ôn tập về Mạng ]]

## 9. Hệ thống tệp
* [[ Hệ thống tệp, Phần 1: Giới thiệu ]]
    * [Điều hướng / Thuật ngữ](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction#navigationterminology)
    * [Hệ thống tệp là gì?](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction#so-whats-a-file-system)
* [[ Hệ thống tệp, Phần 2: Tệp là inode]]
* [[ Hệ thống tệp, Phần 3: Quyền ]]
* [[ Hệ thống tệp, Phần 4: Làm việc với thư mục ]]
* [[ Hệ thống tệp, Phần 5: Hệ thống tệp ảo ]]
* [[ Hệ thống tệp, Phần 6: Tệp được ánh xạ bộ nhớ và Bộ nhớ dùng chung ]]
* [[ Hệ thống tệp, Phần 7: Hệ thống tệp có thể mở rộng và đáng tin cậy ]]
    * [Độ tin cậy với một đĩa đơn](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-7%3A-Scalable-and-Reliable-Filesystems#reliable-single-disk-filesystems)
    * [Dư thừa](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-7%3A-Scalable-and-Reliable-Filesystems#redundancy)
* [[ Hệ thống tệp, Phần 8: Loại bỏ phần mềm độc hại được cài đặt sẵn khỏi thiết bị Android ]]
* [[ Hệ thống tệp, Phần 9: Ví dụ về khối đĩa ]]
* [[ Câu hỏi Ôn tập về Hệ thống tệp ]]

## 10. Tín hiệu
* [[Kiểm soát Tiến trình, Phần 1: Macro đợi, sử dụng tín hiệu]]
   * [Macro đợi](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#wait-macros)
   * [Tín hiệu](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#signals)
* [[ Tín hiệu, Phần 2: Tín hiệu đang chờ xử lý và Mặt nạ tín hiệu ]]
   * [Tín hiệu chuyên sâu](https://github.com/angrave/SystemProgramming/wiki/Signals,-Part-2:-Pending-Signals-and-Signal-Masks#signals-in-depth)
   * [Xử lý trong Luồng/Tiến trình con](https://github.com/angrave/SystemProgramming/wiki/Signals,-Part-2:-Pending-Signals-and-Signal-Masks#disposition-in-threadschildren)
* [[ Tín hiệu, Phần 3: Nâng cao tín hiệu ]]
* [[ Tín hiệu, Phần 4: Sigaction ]]
* [[ Câu hỏi Ôn tập về Tín hiệu ]]

## Thực hành thi và Câu hỏi Ôn tập
Cảnh báo, đây là những bài thực hành tốt nhưng không đầy đủ.

* [[Chủ đề thi]]
* [[Lập trình C: Câu hỏi Ôn tập]]
* [[Lập trình Đa luồng: Câu hỏi Ôn tập]]
* [[Khái niệm Đồng bộ hóa: Câu hỏi Ôn tập]]
* [[Bộ nhớ: Câu hỏi Ôn tập]]
* [[Đường ống: Câu hỏi Ôn tập]]
* [[Hệ thống tệp: Câu hỏi Ôn tập]]
* [[Mạng: Câu hỏi Ôn tập]]
* [[Tín hiệu: Câu hỏi Ôn tập]]
* [[Trò đùa về Lập trình Hệ thống]]



