## RPC là gì?

RPC là viết tắt của Remote Procedure Call (Gọi thủ tục từ xa). RPC là ý tưởng rằng chúng ta có thể thực thi một thủ tục (hàm) trên một máy tính khác. Trong thực tế, thủ tục có thể thực thi trên cùng một máy, tuy nhiên nó có thể ở trong một ngữ cảnh khác - ví dụ như dưới một người dùng khác với các quyền khác nhau và vòng đời khác nhau.

## Phân tách đặc quyền (Privilege Separation) là gì?

Mã từ xa sẽ thực thi dưới một người dùng khác và với các đặc quyền khác với người gọi. Trong thực tế, lệnh gọi từ xa có thể thực thi với nhiều hoặc ít đặc quyền hơn người gọi. Về nguyên tắc, điều này có thể được sử dụng để cải thiện bảo mật của hệ thống (bằng cách đảm bảo các thành phần hoạt động với đặc quyền tối thiểu). Thật không may, các mối quan tâm bảo mật cần được đánh giá cẩn thận để đảm bảo rằng các cơ chế RPC không thể bị phá hoại để thực hiện các hành động không mong muốn. Ví dụ, việc triển khai RPC có thể ngầm tin tưởng bất kỳ máy khách được kết nối nào để thực hiện bất kỳ hành động nào, thay vì một tập hợp con các hành động trên một tập hợp con của dữ liệu.

## Mã stub là gì? Marshalling là gì?

Mã stub là mã cần thiết để ẩn đi sự phức tạp của việc thực hiện một lệnh gọi thủ tục từ xa. Một trong những vai trò của mã stub là **marshalling** dữ liệu cần thiết thành một định dạng có thể được gửi dưới dạng luồng byte đến một máy chủ từ xa.

````C
// Bên ngoài, 'getHiscore' trông giống như một lệnh gọi hàm bình thường
// Bên trong, mã stub thực hiện tất cả công việc để gửi và nhận dữ liệu đến và từ máy từ xa.

int getHiscore(char* game) {
  // Marshall yêu cầu thành một chuỗi byte:
  char* buffer;
  asprintf(&buffer, "getHiscore(%s)!", game);

  // Gửi qua mạng (chúng ta không gửi byte 0; dấu '!' biểu thị kết thúc của thông báo)
  write(fd, buffer, strlen(buffer));

  // Chờ máy chủ gửi phản hồi
  ssize_t bytesread = read(fd, buffer, 8); // chúng ta biết bộ đệm sẽ có ít nhất 8 byte và điều đó là đủ.

  // Không kiểm tra lỗi trong ví dụ này!
  // Ví dụ: unmarshall các byte nhận được từ văn bản thành một int
  buffer[bytesread] = 0; // Biến kết quả thành một chuỗi C

  int score = atoi(buffer);
  free(buffer);
  return score;
}
````

## Mã stub máy chủ là gì? Unmarshalling là gì?
Mã stub máy chủ sẽ nhận yêu cầu, **unmarshall** yêu cầu thành dữ liệu hợp lệ trong bộ nhớ, gọi triển khai bên dưới và gửi kết quả trở lại cho người gọi.

## Làm cách nào để bạn gửi một int? float? một struct? Một danh sách liên kết? Một đồ thị?
Để triển khai RPC, bạn cần quyết định (và ghi lại) các quy ước mà bạn sẽ sử dụng để tuần tự hóa dữ liệu thành một chuỗi byte. Ngay cả một số nguyên đơn giản cũng có một số lựa chọn phổ biến:
* Có dấu hay không dấu?
* ASCII
* Số byte cố định hay thay đổi tùy thuộc vào độ lớn
* Định dạng nhị phân Little endian hay Big endian?

Để marshall một struct, hãy quyết định trường nào cần được tuần tự hóa. Có thể không cần thiết phải gửi tất cả các mục dữ liệu (ví dụ: một số mục có thể không liên quan đến RPC cụ thể hoặc có thể được máy chủ tính toán lại từ các mục dữ liệu khác hiện có).

Để marshall một danh sách liên kết, không cần thiết phải gửi các con trỏ liên kết - chỉ cần truyền các giá trị. Là một phần của unmarshalling, máy chủ có thể tạo lại cấu trúc danh sách liên kết từ chuỗi byte.

Bằng cách bắt đầu tại nút/đỉnh đầu, một cây đơn giản có thể được truy cập đệ quy để tạo phiên bản tuần tự hóa của dữ liệu. Một đồ thị tuần hoàn thường sẽ yêu cầu thêm bộ nhớ để đảm bảo rằng mỗi cạnh và đỉnh được xử lý chính xác một lần.

## IDL (Interface Description Language) là gì?

Viết mã stub bằng tay rất khó khăn, tẻ nhạt, dễ xảy ra lỗi, khó bảo trì và khó thiết kế ngược giao thức mạng từ mã đã triển khai. Một cách tiếp cận tốt hơn là chỉ định các đối tượng dữ liệu, thông báo và dịch vụ và tự động tạo mã máy khách và máy chủ.

Một ví dụ hiện đại về Ngôn ngữ Mô tả Giao diện là các tệp .proto của Google Protocol Buffer.

## Độ phức tạp và thách thức của RPC so với các lệnh gọi cục bộ?

Các lệnh gọi thủ tục từ xa chậm hơn đáng kể (10x đến 100x) và phức tạp hơn các lệnh gọi cục bộ. RPC phải marshall dữ liệu thành định dạng tương thích với mạng. Điều này có thể yêu cầu nhiều lần truyền qua cấu trúc dữ liệu, phân bổ bộ nhớ tạm thời và chuyển đổi biểu diễn dữ liệu.

Mã stub RPC mạnh mẽ phải xử lý thông minh các lỗi mạng và phiên bản. Ví dụ: máy chủ có thể phải xử lý các yêu cầu từ máy khách vẫn đang chạy phiên bản đầu của mã stub.

RPC an toàn sẽ cần triển khai các kiểm tra bảo mật bổ sung (bao gồm xác thực và ủy quyền), xác thực dữ liệu và mã hóa giao tiếp giữa máy khách và máy chủ.

## Chuyển một lượng lớn dữ liệu có cấu trúc

Hãy xem xét ba phương pháp chuyển dữ liệu bằng cách sử dụng 3 định dạng khác nhau - JSON, XML và Google Protocol Buffers. JSON và XML là các giao thức dựa trên văn bản. Ví dụ về các thông báo JSON và XML ở bên dưới.
````xml
<ticket><price currency='dollar'>10</price><vendor>travelocity</vendor></ticket>
````

````javascript
{ "currency": "dollar", "vendor": "travelocity", "price": "10" }
````

Google Protocol Buffers là một giao thức nhị phân hiệu quả mã nguồn mở, đặt trọng tâm mạnh mẽ vào thông lượng cao với chi phí CPU thấp và sao chép bộ nhớ tối thiểu. Các triển khai tồn tại cho nhiều ngôn ngữ bao gồm Go, Python, C++ và C. Điều này có nghĩa là mã stub máy khách và máy chủ bằng nhiều ngôn ngữ có thể được tạo từ tệp đặc tả .proto để marshall dữ liệu đến và từ luồng nhị phân.

Google Protocol Buffers giảm vấn đề phiên bản bằng cách bỏ qua các trường không xác định hiện có trong một thông báo. Xem phần giới thiệu về Protocol Buffers để biết thêm thông tin.

[[https://developers.google.com/protocol-buffers/docs/overview]]


