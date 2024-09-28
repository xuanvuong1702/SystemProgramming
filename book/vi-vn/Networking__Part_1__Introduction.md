Lưu ý: Rõ ràng là trang này *không phải* là mô tả đầy đủ về IP, UDP hoặc TCP! Thay vào đó, nó là một phần giới thiệu ngắn gọn và đủ để chúng ta có thể xây dựng dựa trên những khái niệm này trong các bài giảng sau.

## "IP4" "IP6" là gì?
Sau đây là phần giới thiệu "30 giây" về **giao thức internet** (IP) - là cách chính để gửi các **gói tin** ("datagram") thông tin từ máy này sang máy khác.

"IP4", hay chính xác hơn là "IPv4" là phiên bản 4 của Giao thức Internet, mô tả cách gửi các gói tin thông tin qua mạng từ máy này sang máy khác. Khoảng 95% tổng số gói tin trên Internet ngày nay là gói IPv4. Một hạn chế đáng kể của IPv4 là địa chỉ nguồn và địa chỉ đích bị giới hạn ở 32 bit (IPv4 được thiết kế vào thời điểm mà ý tưởng về 4 tỷ thiết bị được kết nối với cùng một mạng là điều không tưởng - hoặc ít nhất là không đáng để tăng kích thước gói tin) 

Mỗi gói IPv4 bao gồm một **tiêu đề** rất nhỏ - thường là 20 byte (chính xác hơn là "octets"), bao gồm địa chỉ nguồn và địa chỉ đích.

Về mặt khái niệm, địa chỉ nguồn và địa chỉ đích có thể được chia thành hai phần: **số mạng** (các bit trên) và các bit dưới đại diện cho một **số máy chủ** cụ thể trên mạng đó.

Một giao thức gói tin mới hơn là "IPv6" giải quyết nhiều hạn chế của IPv4 (ví dụ: làm cho **bảng định tuyến** đơn giản hơn và địa chỉ 128 bit) tuy nhiên, chưa đến 5% lưu lượng truy cập web dựa trên IPv6.

Một máy có thể có địa chỉ IPv6 và địa chỉ IPv4.

## "Không có nơi nào giống như 127.0.0.1"!
Một địa chỉ IPv4 đặc biệt là `127.0.0.1` còn được gọi là **localhost**. Các gói tin được gửi đến 127.0.0.1 sẽ không bao giờ rời khỏi máy; địa chỉ được chỉ định là cùng một máy.

Lưu ý rằng địa chỉ 32 bit được chia thành 4 **octet**, tức là mỗi số trong **ký hiệu chấm** có thể là từ 0-255 bao gồm cả. Tuy nhiên, địa chỉ IPv4 cũng có thể được viết dưới dạng số nguyên.

## ... và ... "Không có nơi nào giống như 0:0:0:0:0:0:0:1?"
Địa chỉ localhost 128bit trong IPv6 là `0:0:0:0:0:0:0:1` có thể được viết ở dạng rút gọn là `::1`

## Cổng là gì?
Để gửi một **datagram** (gói tin) đến một máy chủ trên Internet bằng IPv4 (hoặc IPv6), bạn cần chỉ định địa chỉ máy chủ và **cổng**. Cổng là một số 16 bit không dấu (tức là số cổng tối đa là 65535).

Một **tiến trình** có thể **lắng nghe** các gói tin đến trên một cổng cụ thể. Tuy nhiên, chỉ các tiến trình có quyền truy cập **siêu người dùng** (root) mới có thể lắng nghe trên các cổng < 1024. Bất kỳ tiến trình nào cũng có thể lắng nghe trên các cổng 1024 trở lên.

Một cổng thường được sử dụng là cổng 80: Cổng 80 được sử dụng cho các yêu cầu http không được mã hóa (tức là các trang web).
Ví dụ: nếu **trình duyệt web** kết nối với http://www.bbc.com/, thì nó sẽ kết nối với cổng 80.

## UDP là gì? Khi nào nó được sử dụng?
**UDP** là một giao thức **không kết nối** được xây dựng trên IPv4 và IPv6. Nó rất đơn giản để sử dụng: Quyết định địa chỉ đích và cổng và gửi gói dữ liệu của bạn! Tuy nhiên, mạng không đảm bảo về việc các gói tin có đến được hay không.
Các gói (hay còn gọi là Datagram) có thể bị loại bỏ nếu mạng bị **tắc nghẽn**. Các gói có thể bị trùng lặp hoặc đến không theo thứ tự.

Giữa hai **trung tâm dữ liệu** ở xa, điển hình là thấy 3% **mất gói**.

Một trường hợp sử dụng điển hình cho UDP là khi nhận dữ liệu cập nhật quan trọng hơn nhận tất cả dữ liệu. Ví dụ: một trò chơi có thể gửi các bản cập nhật liên tục về vị trí của người chơi. Tín hiệu video trực tuyến có thể gửi các bản cập nhật hình ảnh bằng UDP

## TCP là gì? Khi nào nó được sử dụng?
**TCP** là một giao thức **dựa trên kết nối** được xây dựng trên IPv4 và IPv6 (và do đó có thể được mô tả là "TCP/IP" hoặc "TCP over IP"). TCP tạo một _đường ống_ giữa hai máy và **trừu tượng hóa** bản chất cấp gói thấp của Internet: Do đó, trong hầu hết các điều kiện, các byte được gửi từ máy này cuối cùng sẽ đến máy kia mà không bị trùng lặp hoặc mất dữ liệu. 

TCP sẽ tự động quản lý việc gửi lại các gói, bỏ qua các gói trùng lặp, sắp xếp lại các gói không theo thứ tự và thay đổi tốc độ gửi gói.

**Bắt tay ba chiều** của TCP được gọi là SYN, SYN-ACK và ACK. Sơ đồ trên trang này giúp hiểu bắt tay TCP. [Bắt tay TCP](http://www.inetdaemon.com/tutorials/internet/tcp/3-way_handshake.shtml)
 
Hầu hết các **dịch vụ** trên Internet ngày nay (ví dụ: **dịch vụ web**) sử dụng TCP vì nó ẩn đi sự phức tạp của bản chất cấp thấp hơn, cấp gói của Internet.



