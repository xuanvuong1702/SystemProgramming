# Tổng quan

Một quy trình là một thể hiện của một chương trình đang chạy. Các quy trình có rất nhiều thứ để sử dụng. Ở đầu mỗi chương trình, bạn nhận được một quy trình, nhưng mỗi chương trình có thể tạo thêm nhiều quy trình. Thực tế, hệ điều hành của bạn khởi động với chỉ một quy trình và tất cả các quy trình khác đều được tách ra từ quy trình đó - tất cả những điều đó được thực hiện dưới nắp khi khởi động.

## Okay, vậy chương trình là gì?

Chương trình thường bao gồm những điều sau
* Một định dạng nhị phân: Điều này cho hệ điều hành biết tập hợp các bit nào trong nhị phân là gì - phần nào có thể thực thi, phần nào là hằng số, thư viện nào cần bao gồm, v.v.
* Một tập hợp các hướng dẫn máy
* Một số biểu thị hướng dẫn nào để bắt đầu từ
* Hằng số
* Thư viện để liên kết và nơi để điền vào địa chỉ của những thư viện đó

## Ban đầu

Khi hệ điều hành của bạn khởi động trên một máy linux, có một quy trình gọi là `init.d` được tạo ra. Quy trình đó là một quy trình đặc biệt xử lý tín hiệu, gián đoạn, và một mô-đun kiên trì cho một số yếu tố nhân. Bất cứ khi nào bạn muốn tạo một quy trình mới, bạn gọi `fork` (sẽ được thảo luận trong một phần sau) và sử dụng một hàm khác để tải một chương trình khác.


## Cô lập Quy trình

Quy trình rất mạnh mẽ nhưng chúng được cô lập! Điều đó có nghĩa là theo mặc định, không có quy trình nào có thể giao tiếp với quy trình khác. Điều này rất quan trọng bởi vì nếu bạn có một hệ thống lớn (hãy nói như AWS) thì bạn muốn một số quy trình có quyền cao hơn (giám sát, quản trị) so với người dùng trung bình, và chắc chắn không ai muốn người dùng trung bình có thể làm sập toàn bộ hệ thống, dù cố ý hay vô tình, bằng cách sửa đổi một quy trình.

Nếu tôi chạy đoạn mã sau,

```c
int secrets; //có thể được định nghĩa trong kernel hoặc ở đâu đó
secrets++;
printf("%d\n", secrets);
```

Trên hai terminal khác nhau, như bạn có thể đoán, cả hai đều sẽ in ra số 1 chứ không phải số 2. Ngay cả khi chúng ta thay đổi mã để làm điều gì đó thực sự hacky (ngoại trừ việc đọc trực tiếp bộ nhớ), sẽ không có cách nào để thay đổi trạng thái của một quy trình khác (có thể [đây](https://en.wikipedia.org/wiki/Dirty_COW) nhưng đó là đi sâu quá).

# Nội dung Quy trình

## Bố cục Bộ nhớ
<div>
<div style="display: table;margin: 0 auto;">
<img src="https://i.imgur.com/pl6K5cF.png" width=600 style="display: block;margin: 0 auto">
</div>
</div>

Khi một quy trình bắt đầu, nó nhận được không gian địa chỉ riêng của mình. Điều đó có nghĩa là mỗi quy trình nhận được:
* **Một Stack**. Stack là nơi lưu trữ biến tự động và địa chỉ trả về của lời gọi hàm. Mỗi khi một biến mới được khai báo, chương trình di chuyển con trỏ stack xuống để dành chỗ cho biến. Phân đoạn này của stack có thể ghi nhưng không thể thực thi. Nếu stack tăng quá nhiều - có nghĩa là nó tăng hơn một ranh giới được đặt trước hoặc giao nhau với heap - bạn sẽ nhận được một stackoverflow rất có thể dẫn đến một SEGFAULT hoặc tương tự. **Stack được cấp phát tĩnh theo mặc định có nghĩa là chỉ có một lượng không gian nhất định mà người ta có thể ghi**
* **Một Heap**. Heap là một vùng bộ nhớ mở rộng. Nếu bạn muốn cấp phát một đối tượng lớn, nó đi vào đây. Heap bắt đầu từ đầu phân đoạn văn bản và tăng lên (có nghĩa là đôi khi khi bạn gọi `malloc` nó yêu cầu hệ điều hành đẩy ranh giới heap lên). Khu vực này cũng có thể ghi nhưng không thể thực thi. Người ta có thể hết bộ nhớ heap nếu hệ thống bị hạn chế hoặc nếu bạn hết địa chỉ (thông thường hơn trên hệ thống 32bit).
* **Một Phân đoạn Dữ liệu** Điều này chứa tất cả các biến toàn cục của bạn. Phần này bắt đầu từ cuối phân đoạn văn bản và có kích thước tĩnh vì số lượng biến toàn cục được biết ở thời gian biên dịch. Có hai khu vực cho dữ liệu thường là **IBSS** và **UBSS** tương ứng là tập dịch vụ cơ bản đã khởi tạo và phân đoạn dữ liệu chưa khởi tạo. Phần này có thể ghi nhưng không thể thực thi và không có gì quá phức tạp ở đây.
* **Một Phân đoạn Văn bản**. Đây, có lẽ, là phần quan trọng nhất của địa chỉ. Đây là nơi lưu trữ tất cả mã của bạn. Vì assembly biên dịch thành 1's và 0's, đây là nơi lưu trữ các số 1's và 0's. Bộ đếm chương trình di chuyển qua phân đoạn này thực hiện các hướng dẫn và di chuyển xuống hướng dẫn tiếp theo. Điều quan trọng cần lưu ý là đây là phần duy nhất của mã có thể thực thi. Nếu bạn cố gắng thay đổi mã trong khi nó đang chạy, rất có thể bạn sẽ gặp lỗi segfault (có cách vượt qua nhưng hãy giả sử rằng nó gây ra lỗi segfault).
* Tại sao nó không bắt đầu từ số không? Điều này nằm ngoài [phạm vi](https://en.wikipedia.org/wiki/Address_space_layout_randomization) của lớp học này nhưng nó là vì lý do bảo mật.

## ID Quy trình (PID)

Để theo dõi tất cả các quy trình này, hệ điều hành của bạn cung cấp cho mỗi quy trình một số và quy trình đó được gọi là PID, ID quy trình. Các quy trình cũng có một `ppid` viết tắt của parent process id (ID quy trình cha). Mỗi quy trình đều có một quy trình cha, quy trình cha đó có thể là `init.d`

Các quy trình cũng có thể chứa
* Trạng thái Chạy - Cho dù một quy trình đang chuẩn bị, đang chạy, đã dừng, đã kết thúc, v.v.
* Mô tả Tệp - Danh sách các ánh xạ từ số nguyên đến các thiết bị thực (tệp, ổ USB, socket)
* Quyền - Quy trình đang chạy trên `user` nào và thuộc `group` nào. Quy trình chỉ có thể thực hiện những gì được phép cho `user` hoặc `group` như mở một tệp mà `user` đã tạo độc quyền. Có những mẹo để làm cho một chương trình không phải là người dùng đã bắt đầu chương trình, tức là `sudo` lấy một chương trình mà `user` bắt đầu và thực thi nó như `root`.
* Đối số - một danh sách các chuỗi cho biết chương trình của bạn chạy dưới những tham số nào
* Danh sách Môi trường - một danh sách các chuỗi theo dạng `NAME=VALUE` mà người dùng có thể chỉnh sửa.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction">
Forking, Phần 1: Giới thiệu
</a>
</div>