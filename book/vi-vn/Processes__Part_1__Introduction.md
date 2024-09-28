# Tổng quan

Một tiến trình là một phiên bản của một chương trình đang chạy. Các tiến trình có rất nhiều thứ tùy ý sử dụng. Khi bắt đầu mỗi chương trình, bạn nhận được một tiến trình, nhưng mỗi chương trình có thể tạo ra nhiều tiến trình hơn. Trên thực tế, hệ điều hành của bạn chỉ khởi động với một tiến trình và tất cả các tiến trình khác được phân tách (fork) từ đó -- tất cả những điều đó được thực hiện ngầm khi khởi động.

## Được rồi, nhưng chương trình là gì?

Các chương trình thường chứa những điều sau:
* Định dạng nhị phân: Điều này cho hệ điều hành biết tập hợp các bit nào trong tệp nhị phân là gì -- phần nào có thể thực thi, phần nào là hằng số, thư viện nào cần bao gồm, v.v.
* Một tập hợp các lệnh máy
* Một số biểu thị lệnh nào để bắt đầu từ đó
* Hằng số
* Thư viện để liên kết và nơi điền địa chỉ của các thư viện đó

## Ban đầu

Khi hệ điều hành của bạn khởi động trên máy Linux, có một tiến trình có tên là `init` (hoặc `systemd` trên các hệ thống mới hơn) được tạo ra. Tiến trình đó là một tiến trình đặc biệt xử lý các tín hiệu, ngắt và mô-đun duy trì cho các phần tử kernel nhất định. Bất cứ khi nào bạn muốn tạo một tiến trình mới, bạn gọi `fork` (sẽ được thảo luận trong phần sau) và sử dụng một hàm khác để tải một chương trình khác.


## Cách ly tiến trình

Các tiến trình rất mạnh mẽ nhưng chúng bị cô lập! Điều đó có nghĩa là theo mặc định, không có tiến trình nào có thể giao tiếp với tiến trình khác. Điều này rất quan trọng vì nếu bạn có một hệ thống lớn (giả sử AWS), thì bạn muốn một số tiến trình có đặc quyền cao hơn (giám sát, quản trị) so với người dùng trung bình của bạn, và chắc chắn không ai muốn người dùng trung bình có thể làm sập toàn bộ hệ thống, dù là cố ý hay vô tình bằng cách sửa đổi một tiến trình.

Nếu tôi chạy đoạn mã sau,

```C
int secrets; //có thể được định nghĩa trong kernel hoặc ở nơi khác
secrets++;
printf("%d\n", secrets);
```

Trên hai terminal khác nhau, như bạn đoán, cả hai sẽ in ra 1 chứ không phải 2. Ngay cả khi chúng ta thay đổi mã để làm điều gì đó thực sự hack (ngoài việc đọc trực tiếp bộ nhớ) thì sẽ không có cách nào để thay đổi trạng thái của tiến trình khác (được rồi, có thể [lỗ hổng Dirty COW](https://en.wikipedia.org/wiki/Dirty_COW) nhưng điều đó đang đi quá sâu một chút).

# Nội dung tiến trình

## Bố cục bộ nhớ
<div>
<div style="display: table;margin: 0 auto;">
<img src="https://i.imgur.com/pl6K5cF.png" width=600 style="display: block;margin: 0 auto">
</div>
</div>

Khi một tiến trình bắt đầu, nó sẽ nhận được không gian địa chỉ riêng. Có nghĩa là mỗi tiến trình nhận được:
* **Stack**. Stack là nơi lưu trữ các biến tự động và địa chỉ trả về của lệnh gọi hàm. Mỗi khi một biến mới được khai báo, chương trình sẽ di chuyển con trỏ stack xuống để dành chỗ cho biến đó. Đoạn stack này có thể ghi nhưng không thể thực thi. Nếu stack phát triển quá xa -- nghĩa là nó phát triển vượt quá ranh giới đặt trước hoặc giao nhau với heap -- bạn sẽ gặp phải lỗi tràn stack, rất có thể dẫn đến SEGFAULT hoặc lỗi tương tự. **Stack được phân bổ tĩnh theo mặc định, nghĩa là chỉ có một lượng không gian nhất định mà người ta có thể ghi vào**
* **Heap**. Heap là một vùng bộ nhớ mở rộng. Nếu bạn muốn cấp phát một đối tượng lớn, nó sẽ được lưu trữ ở đây. Heap bắt đầu ở phía trên segment văn bản và phát triển lên trên (có nghĩa là đôi khi khi bạn gọi `malloc`, nó sẽ yêu cầu hệ điều hành đẩy ranh giới heap lên trên). Vùng này cũng có thể ghi nhưng không thể thực thi. Người ta có thể hết bộ nhớ heap nếu hệ thống bị hạn chế hoặc nếu bạn hết địa chỉ (phổ biến hơn trên hệ thống 32 bit).
* **Phân đoạn dữ liệu**. Phân đoạn này chứa tất cả các biến toàn cục của bạn. Phần này bắt đầu ở cuối segment văn bản và có kích thước tĩnh vì số lượng biến toàn cục được biết tại thời điểm biên dịch. Phân đoạn dữ liệu thường có hai vùng là **BSS** và **Data**, lần lượt là vùng chưa được khởi tạo và vùng đã được khởi tạo. Phần này có thể ghi nhưng không thể thực thi và không có gì quá đặc biệt ở đây.
* **Phân đoạn mã**. Đây được cho là phần quan trọng nhất của địa chỉ. Đây là nơi lưu trữ tất cả mã của bạn. Vì hợp ngữ được biên dịch thành 1 và 0, nên đây là nơi lưu trữ các số 1 và 0. Bộ đếm chương trình di chuyển qua phân đoạn này, thực thi các lệnh và di chuyển xuống lệnh tiếp theo. Điều quan trọng cần lưu ý là đây là phần duy nhất có thể thực thi của mã. Nếu bạn cố gắng thay đổi mã trong khi nó đang chạy, rất có thể bạn sẽ gặp lỗi segfault (có nhiều cách để giải quyết nhưng cứ giả sử rằng nó bị lỗi segfault).
* Tại sao nó không bắt đầu từ 0? Nó nằm ngoài [phạm vi](https://en.wikipedia.org/wiki/Address_space_layout_randomization) của lớp này nhưng nó dành cho bảo mật.

## ID tiến trình (PID)

Để theo dõi tất cả các tiến trình này, hệ điều hành của bạn gán cho mỗi tiến trình một số và tiến trình đó được gọi là PID, ID tiến trình. Các tiến trình cũng có `ppid`, viết tắt của parent process id (ID tiến trình cha). Mọi tiến trình đều có cha, cha đó có thể là `init` (hoặc `systemd`).

Các tiến trình cũng có thể chứa
* Trạng thái đang chạy - Cho dù tiến trình đang sẵn sàng, đang chạy, đã dừng, đã kết thúc, v.v.
* Bộ mô tả tệp - Danh sách các ánh xạ từ số nguyên đến các thiết bị thực (tệp, USB, socket)
* Quyền - Tiến trình đang chạy trên `người dùng` nào và thuộc `nhóm` nào. Sau đó, tiến trình chỉ có thể thực hiện những gì được phép cho `người dùng` hoặc `nhóm` như mở một tệp mà `người dùng` đã đặt độc quyền. Có những thủ thuật để làm cho một chương trình không phải là người dùng đã khởi động chương trình, tức là `sudo` lấy một chương trình mà `người dùng` khởi động và thực thi nó với tư cách là `root`.
* Đối số - danh sách các chuỗi cho biết chương trình của bạn các tham số để chạy
* Danh sách môi trường - danh sách các chuỗi ở dạng `NAME=VALUE` mà người ta có thể sửa đổi.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Forking%2C-Part-1%3A-Introduction">
Phân nhánh, Phần 1: Giới thiệu
</a>
</div>