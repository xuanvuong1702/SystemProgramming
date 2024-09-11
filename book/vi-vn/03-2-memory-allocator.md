# Hướng dẫn Trình cấp phát Bộ nhớ
Một trình cấp phát bộ nhớ cần theo dõi những byte nào đang được cấp phát và những byte nào đang sẵn sàng để sử dụng. Trang này giới thiệu về việc triển khai và chi tiết khái niệm của việc xây dựng một trình cấp phát, tức là mã thực tế thực hiện `malloc` và `free`.

## Trang này nói về các liên kết của các khối - tôi có cấp phát bộ nhớ cho chúng không?
Mặc dù về mặt khái niệm, chúng ta đang nghĩ về việc tạo danh sách liên kết và danh sách các khối, nhưng chúng ta không cần "cấp phát bộ nhớ" để tạo chúng! Thay vào đó, chúng ta đang ghi số nguyên và con trỏ vào bộ nhớ mà chúng ta đã kiểm soát để sau này chúng ta có thể nhảy một cách nhất quán từ một địa chỉ này sang địa chỉ khác. Thông tin nội bộ này đại diện cho một số overhead. Vì vậy, ngay cả khi chúng ta đã yêu cầu 1024 KB bộ nhớ liên tục từ hệ thống, chúng ta sẽ không thể cung cấp tất cả nó cho chương trình đang chạy.

## Suy nghĩ về các khối
Chúng ta có thể coi bộ nhớ heap của mình như một danh sách các khối, trong đó mỗi khối hoặc được cấp phát hoặc không được cấp phát.
Thay vì lưu trữ một danh sách rõ ràng của các con trỏ, chúng ta lưu trữ thông tin về kích thước của khối _là một phần của khối_. Do đó, có một danh sách khái niệm về các khối trống, nhưng nó là ngầm định, tức là dưới dạng thông tin kích thước khối mà chúng ta lưu trữ như một phần của mỗi khối.

Chúng ta có thể điều hướng từ một khối này sang khối tiếp theo chỉ bằng cách cộng thêm kích thước của khối. Ví dụ, nếu bạn có một con trỏ `p` trỏ đến đầu của một khối, thì `next_block` sẽ ở `((char *)p) +  *(size_t *) p`, nếu bạn đang lưu trữ kích thước của các khối theo byte. Việc ép kiểu thành `char *` đảm bảo rằng phép toán con trỏ được tính bằng byte. Việc ép kiểu thành `size_t *` đảm bảo rằng bộ nhớ tại `p` được đọc như một giá trị kích thước và sẽ cần thiết nếu `p` là kiểu `void *` hoặc `char *`.

Chương trình gọi không bao giờ thấy những giá trị này; chúng chỉ nội bộ đối với việc triển khai trình cấp phát bộ nhớ.

Ví dụ, giả sử trình cấp phát của bạn được yêu cầu dành 80 byte (`malloc(80)`) và cần 8 byte dữ liệu tiêu đề nội bộ. Trình cấp phát sẽ cần tìm một không gian chưa được cấp phát ít nhất 88 byte. Sau khi cập nhật dữ liệu heap, nó sẽ trả về một con trỏ đến khối. Tuy nhiên, con trỏ trả về không trỏ đến đầu của khối vì đó là nơi lưu trữ dữ liệu kích thước nội bộ! Thay vào đó, chúng ta sẽ trả về đầu của khối + 8 byte.
Trong việc triển khai, hãy nhớ rằng phép toán con trỏ phụ thuộc vào kiểu. Ví dụ, `p += 8` thêm `8 * sizeof(p)`, không nhất thiết là 8 byte!

## Triển khai malloc
Triển khai đơn giản nhất sử dụng phương pháp first fit: Bắt đầu từ khối đầu tiên, giả sử nó tồn tại, và lặp lại cho đến khi tìm thấy một khối đại diện cho không gian chưa được cấp phát với kích thước đủ lớn, hoặc chúng ta đã kiểm tra tất cả các khối.

Nếu không tìm thấy khối phù hợp, đến lúc gọi `sbrk()` một lần nữa để mở rộng đủ kích thước của heap. Một triển khai nhanh chóng có thể mở rộng một lượng đáng kể để chúng ta sẽ không cần yêu cầu thêm bộ nhớ heap trong tương lai gần.

Khi tìm thấy một khối trống, nó có thể lớn hơn không gian chúng ta cần. Nếu vậy, chúng ta sẽ tạo hai mục trong danh sách ngầm định của mình. Mục đầu tiên là khối đã cấp phát, mục thứ hai là không gian còn lại.

Có hai cách đơn giản để ghi chú nếu một khối đang được sử dụng hoặc có sẵn. Cách đầu tiên là lưu trữ nó dưới dạng byte trong thông tin tiêu đề cùng với kích thước và cách thứ hai là mã hóa nó dưới dạng bit thấp nhất trong kích thước!
Vì vậy, thông tin kích thước khối sẽ chỉ giới hạn ở các giá trị chẵn:
```c
// Giả sử p là một kiểu con trỏ hợp lý, ví dụ 'size_t *'.
isallocated = (*p) & 1;
realsize = (*p) & ~1;  // che mặt nạ bit thấp nhất
```

## Xem xét về căn chỉnh và làm tròn lên

Nhiều kiến trúc mong đợi các kiểu dữ liệu đa byte được căn chỉnh theo một số bội số của 2^n. Ví dụ, thông thường yêu cầu các kiểu 4-byte được căn chỉnh theo biên giới 4-byte (và các kiểu 8-byte trên biên giới 8-byte). Nếu các kiểu dữ liệu đa byte không được lưu trữ trên một biên giới hợp lý (ví dụ bắt đầu tại một địa chỉ lẻ) thì hiệu suất có thể bị ảnh hưởng đáng kể vì nó có thể yêu cầu hai yêu cầu đọc bộ nhớ thay vì một. Trên một số kiến trúc, hình phạt còn lớn hơn - chương trình sẽ bị crash với một [lỗi bus](http://en.wikipedia.org/wiki/Bus_error#Unaligned_access).

Vì `malloc` không biết người dùng sẽ sử dụng bộ nhớ đã cấp phát như thế nào (mảng của doubles? mảng của chars?), con trỏ trả về cho chương trình cần được căn chỉnh cho trường hợp xấu nhất, phụ thuộc vào kiến trúc.

Từ tài liệu glibc, `malloc` của glibc sử dụng phương pháp thực nghiệm sau:
"    Khối mà malloc cung cấp cho bạn được đảm bảo đã được căn chỉnh để nó có thể chứa bất kỳ loại dữ liệu nào. Trên các hệ thống GNU, địa chỉ luôn là bội số của tám trên hầu hết các hệ thống, và bội số của 16 trên các hệ thống 64-bit."

Ví dụ, nếu bạn cần tính toán bao nhiêu đơn vị 16 byte được yêu cầu, đừng quên làm tròn lên -
```c
int s = (requested_bytes + tag_overhead_bytes + 15) / 16
```
Hằng số bổ sung đảm bảo các đơn vị không hoàn chỉnh được làm tròn lên. Lưu ý, mã thực sự có khả năng sử dụng kích thước biểu tượng như `sizeof(x) - 1`, thay vì mã hóa hằng số số 15.

[Đây là một bài viết tuyệt vời về căn chỉnh bộ nhớ, nếu bạn quan tâm thêm](http://www.ibm.com/developerworks/library/pa-dalign/)
## Một lưu ý về đoạn nội bộ

Đoạn nội bộ xảy ra khi khối bạn cung cấp cho họ lớn hơn kích thước cấp phát của họ. Giả sử chúng ta có một khối trống kích thước 16B (không bao gồm metadata). Nếu họ cấp phát 7 byte, bạn có thể muốn làm tròn lên thành 16B và chỉ trả lại toàn bộ khối.

Điều này trở nên rất xấu khi bạn thực hiện việc hợp nhất và chia (phần tiếp theo). Nếu bạn không thực hiện cả hai, thì bạn có thể kết thúc việc trả lại một khối kích thước 64B cho một cấp phát 7B! Có rất nhiều chi phí phát sinh cho cấp phát đó mà chúng ta đang cố gắng tránh.

## Thực hiện free
Khi `free` được gọi, chúng ta cần áp dụng lại offset để quay lại 'thực sự' bắt đầu của khối (nhớ rằng chúng ta không cung cấp cho người dùng con trỏ đến thực sự bắt đầu của khối?), tức là, đến nơi chúng ta lưu trữ thông tin kích thước.

Một cách thực hiện ngây thơ đơn giản là đánh dấu khối là không được sử dụng. Nếu chúng ta đang lưu trữ trạng thái cấp phát khối trong bit kích thước thấp nhất, thì chúng ta chỉ cần xóa bit:
```C
*p = (*p) & ~1; // Clear lowest bit
```
Tuy nhiên, chúng ta cần phải làm thêm một chút công việc: Nếu khối hiện tại và khối tiếp theo (nếu tồn tại) đều không được sử dụng, chúng ta cần hợp nhất những khối này thành một khối duy nhất.
Tương tự, chúng ta cũng cần kiểm tra khối trước đó. Nếu khối đó tồn tại và đại diện cho bộ nhớ chưa được cấp phát, thì chúng ta cần hợp nhất các khối thành một khối lớn duy nhất.

Để có thể hợp nhất một khối trống với khối trống trước đó, chúng ta cũng cần tìm khối trước đó, vì vậy chúng ta lưu kích thước của khối ở cuối khối. Những thứ này được gọi là "thẻ biên" (tham khảo Knuth73). Vì các khối liền kề, cuối một khối ngồi ngay cạnh đầu của khối tiếp theo. Vì vậy, khối hiện tại (ngoại trừ khối đầu tiên) có thể nhìn vài byte phía sau để tra cứu kích thước của khối trước đó. Với thông tin này, bạn có thể nhảy lùi!

## Hiệu suất
Với mô tả trên, có thể xây dựng một bộ cấp phát bộ nhớ. Ưu điểm chính của nó là đơn giản - ít nhất là đơn giản hơn so với các bộ cấp phát khác!
Cấp phát bộ nhớ là một hoạt động tuyến tính trong trường hợp xấu nhất (tìm kiếm trong danh sách liên kết để tìm một khối trống đủ lớn) và giải phóng bộ nhớ là thời gian hằng số (không quá 3 khối sẽ cần được hợp nhất thành một khối duy nhất). Sử dụng bộ cấp phát này, có thể thử nghiệm với các chiến lược đặt khác nhau. Ví dụ, bạn có thể bắt đầu tìm kiếm từ nơi bạn vừa giải phóng một khối, hoặc từ nơi bạn vừa cấp phát. Nếu bạn lưu con trỏ đến các khối, bạn cần rất cẩn thận để chúng luôn luôn hợp lệ (ví dụ: khi hợp nhất các khối hoặc các lời gọi malloc hoặc free khác thay đổi cấu trúc heap)

## Bộ cấp phát Danh sách Rõ ràng

Hiệu suất tốt hơn có thể đạt được bằng cách triển khai một danh sách liên kết kép rõ ràng của các nút trống. Trong trường hợp đó, chúng ta có thể di chuyển ngay lập tức đến khối trống tiếp theo và khối trống trước đó. Điều này có thể cắt đôi thời gian tìm kiếm, bởi vì danh sách liên kết chỉ bao gồm các khối chưa được cấp phát.

Một lợi ích thứ hai là bây giờ chúng ta có một số kiểm soát về thứ tự của danh sách liên kết. Ví dụ, khi một khối được giải phóng, chúng ta có thể chọn chèn nó vào đầu danh sách liên kết thay vì luôn luôn giữa các hàng xóm của nó. Điều này sẽ được thảo luận dưới đây.

Chúng ta lưu trữ con trỏ của danh sách liên kết ở đâu? Một mẹo đơn giản là nhận ra rằng khối đó không được sử dụng và lưu trữ con trỏ tiếp theo và trước đó như một phần của khối (tuy nhiên, bây giờ bạn phải đảm bảo rằng các khối trống luôn đủ lớn để chứa hai con trỏ).

Chúng ta vẫn cần triển khai Thẻ Biên (tức là một danh sách ngầm sử dụng kích thước), để chúng ta có thể giải phóng khối một cách chính xác và hợp nhất chúng với hai hàng xóm của chúng. Do đó, danh sách rõ ràng yêu cầu nhiều mã và độ phức tạp hơn.

Với danh sách liên kết rõ ràng, một thuật toán 'Tìm-Đầu-Tiên' nhanh chóng và đơn giản được sử dụng để tìm liên kết đủ lớn đầu tiên. Tuy nhiên, vì thứ tự liên kết có thể được sửa đổi, điều này tương ứng với các chiến lược đặt khác nhau. Ví dụ, nếu các liên kết được duy trì từ lớn nhất đến nhỏ nhất, thì điều này tạo ra một chiến lược đặt 'Phù hợp nhất'.

### Chính sách chèn danh sách liên kết rõ ràng
Khối vừa được giải phóng có thể được chèn dễ dàng vào hai vị trí có thể: ở đầu hoặc theo thứ tự địa chỉ (bằng cách sử dụng các thẻ biên để tìm hàng xóm đầu tiên).

Chèn vào đầu tạo ra một chính sách LIFO (last-in, first-out): Không gian vừa được giải phóng sẽ được tái sử dụng. Các nghiên cứu cho thấy độ phân mảnh tồi tệ hơn so với sử dụng thứ tự địa chỉ.

Chèn theo thứ tự địa chỉ ("Chính sách theo thứ tự địa chỉ") chèn các khối đã giải phóng sao cho các khối được truy cập theo thứ tự địa chỉ tăng dần. Chính sách này yêu cầu nhiều thời gian hơn để giải phóng một khối vì các thẻ biên (dữ liệu kích thước) phải được sử dụng để tìm các khối chưa được cấp phát tiếp theo và trước đó. Tuy nhiên, có ít độ phân mảnh hơn.

# Nghiên cứu trường hợp: Bộ cấp phát Buddy (một ví dụ về danh sách phân biệt)

Một bộ cấp phát phân biệt là một bộ cấp phát chia heap thành các khu vực khác nhau được xử lý bởi các bộ cấp phát con khác nhau tùy thuộc vào kích thước yêu cầu cấp phát. Các kích thước được nhóm thành các lớp (ví dụ: lũy thừa của hai) và mỗi kích thước được xử lý bởi một bộ cấp phát con khác nhau và mỗi kích thước duy trì danh sách trống của riêng mình.

Một bộ cấp phát nổi tiếng của loại này là bộ cấp phát buddy. Chúng ta sẽ thảo luận về bộ cấp phát buddy nhị phân, nó chia cấp phát thành các khối có kích thước 2^n (n = 1, 2, 3, ...) lần một số lượng đơn vị cơ sở byte, nhưng cũng có những bộ cấp phát khác (ví dụ: phân chia Fibonacci - bạn có thể thấy tại sao nó được đặt tên như vậy không?). Ý tưởng cơ bản rất đơn giản: Nếu không có khối trống nào có kích thước 2^n, hãy chuyển sang cấp độ tiếp theo và lấy khối đó và chia nó thành hai. Nếu hai khối lân cận cùng kích thước trở thành không được cấp phát, chúng có thể được hợp nhất trở lại thành một khối lớn duy nhất có kích thước gấp đôi.

Bộ cấp phát Buddy nhanh chóng vì các khối lân cận để hợp nhất có thể được tính toán từ địa chỉ của khối đã giải phóng, thay vì duyệt qua các thẻ kích thước. Hiệu suất tối đa thường yêu cầu một lượng nhỏ mã lệnh để sử dụng một lệnh CPU chuyên biệt để tìm bit không phải là zero thấp nhất.

Nhược điểm chính của bộ cấp phát Buddy là chúng phải chịu *độ phân mảnh nội bộ*, vì cấp phát được làm tròn lên đến kích thước khối gần nhất. Ví dụ, một cấp phát 68 byte sẽ yêu cầu một khối 128 byte.

### Đọc thêm và Tham khảo
* Xem [Foundations of Software Technology and Theoretical Computer Science 1999 proceedings](http://books.google.com/books?id=0uHME7EfjQEC&lpg=PP1&pg=PA85#v=onepage&q&f=false) (Google books, trang 85)
* ThanksForTheMemory UIUC lecture Slides ([pptx](https://subversion.ews.illinois.edu/svn/sp18-cs241/_shared/wikifiles/CS241-05-ThanksForTheMemorySlides.pptx)) ([pdf](https://subversion.ews.illinois.edu/svn/sp18-cs241/_shared/wikifiles/CS241-05-ThanksForTheMemorySlides.pdf))
và 
* [Trang cấp phát bộ nhớ buddy của Wikipedia](http://en.wikipedia.org/wiki/Buddy_memory_allocation)

# Các bộ cấp phát khác
Có nhiều phương pháp cấp phát khác. Ví dụ [SLUB](http://en.wikipedia.org/wiki/SLUB_%28software%29) (wikipedia) - một trong ba bộ cấp phát được sử dụng nội bộ bởi Kernel Linux.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-1%3A-Heap-Memory-Introduction">
Quay lại: Bộ nhớ, Phần 1: Giới thiệu về Bộ nhớ Heap
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-3%3A-Smashing-the-Stack-Example">
Tiếp theo: Bộ nhớ, Phần 3: Ví dụ về Smashing the Stack
</a>
</div>