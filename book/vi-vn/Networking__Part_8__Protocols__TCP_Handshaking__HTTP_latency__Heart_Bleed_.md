# Giao thức

## TCP khởi tạo kênh giao tiếp giữa các socket như thế nào?

## Lệnh gọi POSIX nào khiến gói SYN đầu tiên được gửi đến máy chủ?

Gói gửi đi đầu tiên được gửi bởi **máy khách** khi máy khách gọi `connect`

```C
socket(...) <- Một socket được tạo nhưng chưa có kết nối thực tế nào được thực hiện.
connect(fd,...) <- Khởi tạo kết nối
```

## Nghiên cứu tình huống phá hoại giao thức: Từ chối dịch vụ
**Tấn công lũ lụt SYN**
**Tấn công từ chối dịch vụ phân tán (DDoS)**
**Tấn công từ chối dịch vụ Internet vạn vật (IoT DDoS)**

## Độ trễ TCP

Cứu phi hành gia! Mặt trăng cách xa 1,3 giây ánh sáng. Máy khách TCP ở trên Trái đất và bảng điều khiển mặt trăng chạy máy chủ TCP. Giả sử mỗi lần cần có một kết nối TCP mới. Bao nhiêu giây trôi qua giữa việc muốn gửi thông báo CLOSE-AIRLOCK và máy chủ nhận được dữ liệu?

```C
fd=socket(...)
connect(fd,...,...)
write(fd,"CLOSE-AIRLOCK!",14);
```
Trả lời: 3,9 giây.
TODO: Giải thích tại sao. Tạo sơ đồ

Bao nhiêu giây trôi qua giữa việc yêu cầu dữ liệu từ máy chủ và nhận được kết quả?

```C
fd= socket(...)
connect(fd,...,...)
write(fd,"READ-TEMP!",10);
bytes= read(fd,buffer,256);
```
Trả lời: 5,2 giây. Todo Giải thích tại sao.

## Hiệu suất TCP và HTTP/1.0

Nếu **thời gian khứ hồi** của máy khách-máy chủ là 10 mili giây, thì thời gian tối thiểu cần thiết để hiển thị một trang web có hình ảnh là bao nhiêu? Giả sử HTTP/1.0 và hình ảnh yêu cầu một yêu cầu HTTP riêng biệt.


## Cải thiện hiệu suất HTTP/1.1

## Cải thiện hiệu suất HTTP/2.0

## Giao thức QUIC






