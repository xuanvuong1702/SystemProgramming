## Hai loại tệp

Trên Linux, có hai khái niệm trừu tượng về tệp. Khái niệm trừu tượng đầu tiên là cấp `fd` (bộ mô tả tệp) của Linux, có nghĩa là bạn có thể sử dụng
* `open`
* `read`
* `write`
* `close`
* `lseek`
* `fcntl`
...

Và vân vân. Giao diện Linux rất mạnh mẽ và biểu cảm, nhưng đôi khi chúng ta cần tính di động (ví dụ: nếu chúng ta đang viết cho macOS hoặc Windows). Đây là lúc khái niệm trừu tượng của C phát huy tác dụng. Trên các hệ điều hành khác nhau, C sử dụng các hàm cấp thấp để tạo một trình bao bọc xung quanh các tệp mà bạn có thể sử dụng ở mọi nơi, nghĩa là C trên Linux sử dụng các lệnh gọi ở trên. C có một vài lệnh gọi sau:
* `fopen`
* `fread` hoặc `fgetc/fgets` hoặc `fscanf`
* `fwrite` hoặc `fprintf`
* `fclose`
* `fflush`

Nhưng bạn không có được sự biểu cảm mà Linux cung cấp cho bạn với các lệnh gọi hệ thống, bạn có thể chuyển đổi qua lại giữa chúng bằng `int fileno(FILE* stream)` và `FILE* fdopen(int fd...)`.

Một khía cạnh quan trọng khác cần lưu ý là các tệp C được **đệm**, nghĩa là nội dung của chúng có thể không được ghi ngay lập tức theo mặc định. Bạn có thể thay đổi điều đó bằng các tùy chọn C.

## Làm cách nào để biết một tệp có dung lượng bao nhiêu?
Đối với các tệp nhỏ hơn kích thước của kiểu dữ liệu `long`, việc sử dụng `fseek` và `ftell` là một cách đơn giản để thực hiện việc này:

Di chuyển đến cuối tệp và tìm vị trí hiện tại.
```C
fseek(f, 0, SEEK_END);
long pos = ftell(f);
```
Điều này cho chúng ta biết vị trí hiện tại trong tệp tính bằng byte - tức là độ dài của tệp!

`fseek` cũng có thể được sử dụng để đặt vị trí tuyệt đối.
```C
fseek(f, 0, SEEK_SET); // Di chuyển đến đầu tệp 
fseek(f, posn, SEEK_SET);  // Di chuyển đến 'posn' trong tệp.
```
Tất cả các lần đọc và ghi trong tương lai trong tiến trình cha hoặc con sẽ tuân theo vị trí này.
Lưu ý rằng việc ghi hoặc đọc từ tệp sẽ thay đổi vị trí hiện tại.

Xem các trang hướng dẫn (`man page`) cho `fseek` và `ftell` để biết thêm thông tin.

## Nhưng cố gắng đừng làm điều này
**Lưu ý: Điều này không được khuyến khích trong trường hợp thông thường vì một điểm kỳ quặc với ngôn ngữ C**. Điểm kỳ quặc đó là `long` chỉ cần có **kích thước 4 Byte**, nghĩa là kích thước tối đa mà `ftell` có thể trả về là nhỏ hơn 2 Gigabyte một chút (mà chúng ta biết ngày nay các tệp của chúng ta có thể là hàng trăm gigabyte hoặc thậm chí terabyte trên hệ thống tệp phân tán). Chúng ta nên làm gì thay thế? Sử dụng `stat`! Chúng tôi sẽ đề cập đến `stat` trong phần sau nhưng đây là một số mã sẽ cho bạn biết kích thước của tệp
```C
struct stat buf;
if(stat(filename, &buf) == -1){
	return -1;
}
return (ssize_t)buf.st_size;
```
`buf.st_size` thuộc loại `off_t` đủ lớn cho các tệp lớn **khổng lồ**.

## Điều gì xảy ra nếu tiến trình con đóng luồng tệp bằng `fclose` hoặc `close`?
Việc đóng một luồng tệp là duy nhất cho mỗi tiến trình. Các tiến trình khác có thể tiếp tục sử dụng bộ xử lý tệp của riêng chúng. Hãy nhớ rằng mọi thứ đều được sao chép khi một tiến trình con được tạo, ngay cả vị trí tương đối của các tệp.

## Còn `mmap` cho tệp thì sao?

Một trong những cách sử dụng chung cho `mmap` là ánh xạ một tệp vào bộ nhớ. Điều này không có nghĩa là tệp được `malloc` vào bộ nhớ ngay lập tức. Lấy ví dụ đoạn mã sau.

```C
int fd = open(...); //Tệp có 2 trang
char* addr = mmap(..fd..);
addr[0] = 'l';
```
Hạt nhân có thể nói, "okay, tôi thấy rằng bạn muốn ánh xạ tệp vào bộ nhớ, vì vậy tôi sẽ dành một số không gian trong không gian địa chỉ của bạn có độ dài bằng tệp". Điều đó có nghĩa là khi bạn ghi vào `addr[0]`, bạn thực sự đang ghi vào byte đầu tiên của tệp. Hạt nhân thực sự có thể thực hiện một số tối ưu hóa. Thay vì tải toàn bộ tệp vào bộ nhớ, nó có thể chỉ tải từng trang một vì nếu tệp có 1024 trang; bạn có thể chỉ truy cập 3 hoặc 4 trang khiến việc tải toàn bộ tệp là lãng phí thời gian (đó là lý do tại sao lỗi trang rất mạnh! Chúng cho phép hệ điều hành kiểm soát mức độ bạn sử dụng tệp của mình).

## Đối với mỗi `mmap`

Hãy nhớ rằng sau khi bạn hoàn thành `mmap`, bạn phải gọi `munmap` để thông báo cho hệ điều hành rằng bạn không còn sử dụng các trang được phân bổ, do đó, hệ điều hành có thể ghi nó trở lại đĩa và trả lại địa chỉ cho bạn trong trường hợp bạn cần `malloc` sau đó.


