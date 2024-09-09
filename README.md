# Lời Mở Đầu

Chào mừng đến với wiki-book về Lập Trình Hệ Thống do cộng đồng cung cấp của Angrave!
Wiki này được xây dựng và duy trì tích cực từ 2014-2018 bởi sinh viên và giảng viên từ Đại học Illinois. Đây là một thí nghiệm tạo nội dung do cộng đồng thực hiện bởi Lawrence Angrave từ CS @ Illinois và đại diện cho nội dung khóa học CS241 (giờ là CS341) "Giới Thiệu về Lập Trình Hệ Thống" từ thời điểm đó. Wiki này sẽ luôn mở cửa cho mọi người để tìm hiểu thêm về lập trình hệ thống. Xin vui lòng cho tôi biết (angrave @ illinois) nếu bạn thấy nó hữu ích!

Sinh viên Đại học Illinois nên sử dụng sách giáo trình cụ thể cho CS341 -

http://cs341.cs.illinois.edu/coursebook/index.html

# Mục Lục

## 0. Giới Thiệu
* [[Bảng Thuật Ngữ Thông Thường]]
* [[Mẹo Lập Trình, Phần 1]]
* [[Câu Chuyện Ngắn và Bài Hát về Lập Trình Hệ Thống]]
* [[Lỗi Lập Trình Hệ Thống trong Ngành Công Nghiệp]]

## 1. Học C
* [[Lập Trình C, Phần 1: Giới Thiệu]]
* [[Lập Trình C, Phần 2: Nhập và Xuất Văn Bản]]
   * [In vào/Đọc từ Luồng](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output#printing-to-streams)
   * [Phân Tích Đầu Vào](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-2%3A-Text-Input-And-Output#parsing-input)
* [[Lập Trình C, Phần 3: Những Lỗi Thông Thường]]
   * [Lỗi Bộ Nhớ](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#memory-mistakes)
   * [Lỗi Logic/Dòng Chương Trình](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#logic-and-program-flow-mistakes)
   * [Những Lỗi Khác](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas#other-gotchas)
* [[Lập Trình C, Phần 4: Chuỗi và Cấu Trúc]]
   * [Chuỗi](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs#strings-structs-and-gotchas)
   * [Cấu Trúc](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs#so-whats-a-struct)
* [[Lập Trình C, Phần 5: Gỡ Lỗi]]
   * [Gỡ Lỗi Trong Code](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#in-code-debugging)
   * [Valgrind](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#valgrind)
   * [Tsan](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#tsan)
   * [GDB](https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging#gdb)
* [[Lập Trình C, Câu Hỏi Ôn Tập]]

## 2. Quá Trình
* [[Hạt Nhân, Shell, Terminal, Ôi Chao!]]
* [[Quá Trình, Phần 1: Giới Thiệu]]
   * [Tổng Quan](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#overview)
   * [Nội Dung Quá Trình](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#process-contents)
   * [Phần Thưởng: Thêm Nội Dung](https://github.com/angrave/SystemProgramming/wiki/Processes,-Part-1:-Introduction#process-functionslimitations-bonus)
* [[Forking, Phần 1: Giới Thiệu]]
   * [Giới Thiệu](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction#intro-to-fork)
   * [Chờ và Thực Thi](https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction#waiting-and-execing)
* [[Forking, Phần 2: Fork, Exec, Wait]]
   * [Mô Hình](https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-2:-Fork,-Exec,-Wait#the-pattern)
   * [Zombies](https://github.com/angrave/SystemProgramming/wiki/Forking,-Part-2:-Fork,-Exec,-Wait#zombies)
* [[Kiểm Soát Quá Trình, Phần 1: Wait Macros, Sử Dụng Tín Hiệu]]
   * [Wait Macros](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#wait-macros)
   * [Tín Hiệu](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#signals)
* [[Câu Hỏi Ôn Tập về Quá Trình]]

## 3. Bộ Nhớ và Bộ Cấp Phát
* [[Bộ Nhớ, Phần 1: Giới Thiệu về Bộ Nhớ Heap]]
   * [Cấp Phát Bộ Nhớ Động C](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction#c-dynamic-memory-allocation)
   * [Giới Thiệu về Việc Cấp Phát](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction#intro-to-allocating)
* [[Bộ Nhớ, Phần 2: Triển Khai Bộ Cấp Phát Bộ Nhớ]]
   * [Hướng Dẫn Bộ Cấp Phát Bộ Nhớ](https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator#memory-allocator-tutorial)
* [[Bộ Nhớ, Phần 3: Ví Dụ về Làm Vỡ Stack]]
* [[Câu Hỏi Ôn Tập về Bộ Nhớ]]

## 4. Giới Thiệu về Pthreads
* [[Pthreads, Phần 1: Giới Thiệu]]
   * [Giới Thiệu về Threads](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction#intro-to-threads)
   * [Pthreads Đơn Giản](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-1:-Introduction#simple-usage)
* [[Pthreads, Phần 2: Sử Dụng Trong Thực Tế]]
   * [Thêm Các Hàm pthread](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-2:-Usage-in-Practice#more-pthread-functions)
   * [Giới Thiệu về Điều Kiện Đua](https://github.com/angrave/SystemProgramming/wiki/Pthreads,-Part-2:-Usage-in-Practice#intro-to-race-conditions)
* [[Pthreads, Phần 3: Vấn Đề Song Song (Phần Thưởng)]]
* [[Câu Hỏi Ôn Tập về Pthread]]
## 5. Đồng bộ hóa
* [[Đồng bộ hóa, Phần 1: Khóa Mutex]]
   * [Giải quyết các phần Critical](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-1:-Mutex-Locks#solving-critical-sections)
   * [Mutex Gotchas](https://github.com/angrave/SystemProgramming/wiki/Synchronization,-Part-1:-Mutex-Locks#mutex-gotchas)
* [[Đồng bộ hóa, Phần 2: Semaphore Đếm]]
* [[Đồng bộ hóa, Phần 3: Làm việc với Mutexes Và Semaphores]]
   * [Stack An toàn cho Thread](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-3%3A-Working-with-Mutexes-And-Semaphores#thread-safe-stack)
   * [Stack Semaphores](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-3%3A-Working-with-Mutexes-And-Semaphores#stack-semaphores)
* [[Đồng bộ hóa, Phần 4: Vấn đề Phần Critical]]
   * [Giải pháp Ứng viên](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#candidate-solutions)
   * [Giải pháp hoạt động](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#working-solutions)
   * [Giải pháp phần cứng](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-4%3A-The-Critical-Section-Problem#hardware-solutions)
* [[Đồng bộ hóa, Phần 5: Biến điều kiện]]
   * [Giới thiệu về Biến điều kiện](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-5%3A-Condition-Variables#intro-to-condition-variables)
   * [Triển khai Semaphore Đếm](https://github.com/angrave/SystemProgramming/wiki/Synchronization%2C-Part-5%3A-Condition-Variables#implementing-counting-semphore)
* [[Đồng bộ hóa, Phần 6: Triển khai một rào cản]]
* [[Đồng bộ hóa, Phần 7: Vấn đề Đọc Ghi]]
* [[Đồng bộ hóa, Phần 8: Ví dụ về Bộ đệm vòng]]
* [[Đồng bộ hóa, Phần 9: Đồng bộ hóa giữa các quá trình]]
* [[Câu hỏi ôn tập về Đồng bộ hóa]]

## 6. Deadlock
* [[Deadlock, Phần 1: Đồ thị phân bổ tài nguyên]]
* [[Deadlock, Phần 2: Điều kiện Deadlock]]
* [[Deadlock, Phần 3: Triết gia ăn tối]]
   * [Giải pháp thất bại](https://github.com/angrave/SystemProgramming/wiki/Deadlock,-Part-3:-Dining-Philosophers#failed-solutions)
   * [Giải pháp khả thi](https://github.com/angrave/SystemProgramming/wiki/Deadlock,-Part-3:-Dining-Philosophers#viable-solutions)
* [[Câu hỏi ôn tập về Deadlock]]

## 7. Giao tiếp giữa các quá trình & Lập lịch
* [[Bộ nhớ ảo, Phần 1: Giới thiệu về Bộ nhớ ảo]]
   * [Bộ nhớ ảo là gì?](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory#what-is-virtual-memory)
   * [Khung nâng cao và Bảo vệ trang](https://github.com/angrave/SystemProgramming/wiki/Virtual-Memory%2C-Part-1%3A-Introduction-to-Virtual-Memory#advanced-frames-and-page-protections)
* [[Pipes, Phần 1: Giới thiệu về pipes]]
* [[Pipes, Phần 2: Bí mật lập trình Pipe]]
   * [Pipe Gotchas](https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#pipe-gotchas)
   * [Pipes Đặt tên](https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#named-pipes)
* [[ Files, Phần 1: Làm việc với files]]
* [[ Lập lịch, Phần 1: Lập lịch quá trình ]]
   * [Suy nghĩ về Lập lịch](https://github.com/angrave/SystemProgramming/wiki/Scheduling%2C-Part-1%3A-Scheduling-Processes#thinking-about-scheduling)
   * [Biện pháp hiệu quả](https://github.com/angrave/SystemProgramming/wiki/Scheduling%2C-Part-1%3A-Scheduling-Processes#measures-of-efficiency)
* [[ Lập lịch, Phần 2: Lập lịch quá trình: Thuật toán ]]
* [[ Câu hỏi ôn tập về IPC]]
## 8. Mạng
* [[ POSIX, Phần 1: Xử lý lỗi]]
* [[ Mạng, Phần 1: Giới thiệu]]
* [[ Mạng, Phần 2: Sử dụng getaddrinfo ]]
* [[ Mạng, Phần 3: Xây dựng một Client TCP đơn giản ]]
* [[ Mạng, Phần 4: Xây dựng một Server TCP đơn giản ]]
* [[ Mạng, Phần 5: Tắt cổng, tái sử dụng cổng và các thủ thuật khác ]]
* [[ Mạng, Phần 6: Tạo một server UDP ]]
* [[ Mạng, Phần 7: Nonblocking I O, select(), và epoll ]]
* [[ Mạng, Phần 8: Các giao thức (TCP Handshaking, HTTP latency, Heart Bleed) ]]
* [[ RPC, Phần 1: Giới thiệu về Remote Procedure Calls ]]
* [[ Câu hỏi ôn tập về Mạng ]]

## 9. Hệ thống tệp
* [[ Hệ thống tệp, Phần 1: Giới thiệu ]]
   * [Điều hướng/Terminology](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction#navigationterminology)
   * [Hệ thống tệp là gì?](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction#so-whats-a-file-system)
* [[ Hệ thống tệp, Phần 2: Tệp là inodes]]
* [[ Hệ thống tệp, Phần 3: Quyền hạn ]]
* [[ Hệ thống tệp, Phần 4: Làm việc với thư mục ]]
* [[ Hệ thống tệp, Phần 5: Hệ thống tệp ảo ]]
* [[ Hệ thống tệp, Phần 6: Tệp ánh xạ bộ nhớ và Bộ nhớ chia sẻ ]]
* [[ Hệ thống tệp, Phần 7: Hệ thống tệp có thể mở rộng và đáng tin cậy ]]
   * [Độ tin cậy với một Đĩa duy nhất](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-7%3A-Scalable-and-Reliable-Filesystems#reliable-single-disk-filesystems)
   * [Dự phòng](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-7%3A-Scalable-and-Reliable-Filesystems#redundancy)
* [[ Hệ thống tệp, Phần 8: Loại bỏ phần mềm độc hại được cài đặt sẵn từ một thiết bị Android ]]
* [[ Hệ thống tệp, Phần 9: Ví dụ về khối đĩa ]]
* [[ Câu hỏi ôn tập về Hệ thống tệp ]]

## 10. Tín hiệu
* [[Kiểm soát Quá trình, Phần 1: Wait macros, sử dụng tín hiệu]]
   * [Wait Macros](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#wait-macros)
   * [Tín hiệu](https://github.com/angrave/SystemProgramming/wiki/Process-Control%2C-Part-1%3A-Wait-macros%2C-using-signals#signals)
* [[ Tín hiệu, Phần 2: Tín hiệu đang chờ và Mặt nạ tín hiệu ]]
   * [Tín hiệu sâu hơn](https://github.com/angrave/SystemProgramming/wiki/Signals,-Part-2:-Pending-Signals-and-Signal-Masks#signals-in-depth)
   * [Disposition trong Threads/Children](https://github.com/angrave/SystemProgramming/wiki/Signals,-Part-2:-Pending-Signals-and-Signal-Masks#disposition-in-threadschildren)
* [[ Tín hiệu, Phần 3: Tạo tín hiệu ]]
* [[ Tín hiệu, Phần 4: Sigaction ]]
* [[ Câu hỏi ôn tập về Tín hiệu ]]
## Ôn tập và Câu hỏi ôn thi
Lưu ý rằng đây là những bài tập ôn tập tốt nhưng không toàn diện.

* [[Chủ đề ôn thi]]
* [[Lập trình C: Câu hỏi ôn tập]]
* [[Lập trình đa luồng: Câu hỏi ôn tập]]
* [[Khái niệm Đồng bộ hóa: Câu hỏi ôn tập]]
* [[Bộ nhớ: Câu hỏi ôn tập]]
* [[Pipe: Câu hỏi ôn tập]]
* [[Hệ thống tệp: Câu hỏi ôn tập]]
* [[Mạng: Câu hỏi ôn tập]]
* [[Tín hiệu: Câu hỏi ôn tập]]
* [[Truyện cười về Lập trình hệ thống]]