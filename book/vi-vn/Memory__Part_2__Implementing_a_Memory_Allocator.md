# Hướng dẫn cấp phát bộ nhớ

Bộ cấp phát bộ nhớ cần theo dõi byte nào hiện đang được cấp phát và byte nào có sẵn để sử dụng. Trang này giới thiệu **cách triển khai** và **chi tiết khái niệm** về việc xây dựng **bộ cấp phát**, tức là mã thực tế triển khai `malloc` và `free`.

## Trang này nói về các liên kết của các khối - tôi có malloc bộ nhớ cho chúng thay thế không?
Mặc dù về mặt khái niệm, chúng ta đang nghĩ về việc tạo **danh sách liên kết** và danh sách các khối, nhưng chúng ta không cần "malloc bộ nhớ" để tạo chúng! Thay vào đó, chúng ta đang ghi các số nguyên và **con trỏ** vào bộ nhớ mà chúng ta đã kiểm soát để sau này chúng ta có thể nhất quán nhảy từ địa chỉ này sang địa chỉ tiếp theo. **Thông tin nội bộ** này đại diện cho một số chi phí. Vì vậy, ngay cả khi chúng tôi đã yêu cầu 1024 KB bộ nhớ liền kề từ hệ thống, chúng tôi sẽ không thể cung cấp tất cả cho chương trình đang chạy.

## Suy nghĩ theo khối
Chúng ta có thể coi bộ nhớ **heap** của mình như một danh sách các **khối**, trong đó mỗi khối được cấp phát hoặc chưa được cấp phát.
Thay vì lưu trữ một danh sách con trỏ rõ ràng, chúng tôi lưu trữ thông tin về kích thước của khối _như một phần của khối_. Do đó, về mặt khái niệm có một danh sách các khối trống, nhưng nó là ngầm định, tức là ở dạng thông tin kích thước khối mà chúng tôi lưu trữ như một phần của mỗi khối.

Chúng ta có thể điều hướng từ khối này sang khối tiếp theo chỉ bằng cách thêm kích thước của khối. Ví dụ: nếu bạn có một con trỏ `p` trỏ đến đầu của một khối, thì `next_block` sẽ ở `((char *)p) +  *(size_t *) p`, nếu bạn đang lưu trữ kích thước của các khối tính bằng byte. Việc **ép kiểu** thành `char *` đảm bảo rằng **số học con trỏ** được tính bằng byte. Việc ép kiểu thành `size_t *` đảm bảo rằng bộ nhớ tại `p` được đọc dưới dạng giá trị kích thước và sẽ cần thiết nếu `p` là kiểu `void *` hoặc `char *`.

Chương trình gọi không bao giờ nhìn thấy các giá trị này; chúng là **nội bộ** đối với việc triển khai bộ cấp phát bộ nhớ. 

Ví dụ: giả sử bộ cấp phát của bạn được yêu cầu dành 80 byte (`malloc(80)`) và yêu cầu 8 byte dữ liệu tiêu đề nội bộ. Bộ cấp phát sẽ cần tìm một không gian chưa được phân bổ ít nhất 88 byte. Sau khi cập nhật dữ liệu heap, nó sẽ trả về một con trỏ đến khối. Tuy nhiên, con trỏ được trả về không trỏ đến đầu khối vì đó là nơi lưu trữ **dữ liệu kích thước** nội bộ! Thay vào đó, chúng tôi sẽ trả về đầu khối + 8 byte.
Trong quá trình triển khai, hãy nhớ rằng số học con trỏ phụ thuộc vào kiểu. Ví dụ: `p += 8` cộng thêm `8 * sizeof(p)`, không nhất thiết là 8 byte!

## Triển khai malloc
Triển khai đơn giản nhất sử dụng **vừa đầu tiên**: Bắt đầu từ khối đầu tiên, giả sử nó tồn tại, và lặp lại cho đến khi tìm thấy một khối đại diện cho không gian chưa được phân bổ có đủ kích thước hoặc chúng ta đã kiểm tra tất cả các khối.

Nếu không tìm thấy khối phù hợp, đã đến lúc gọi `sbrk()` một lần nữa để mở rộng đủ kích thước của heap. Một triển khai nhanh có thể mở rộng nó một lượng đáng kể để chúng ta sẽ không cần yêu cầu thêm bộ nhớ heap trong tương lai gần.

Khi tìm thấy một khối trống, nó có thể lớn hơn không gian chúng ta cần. Nếu vậy, chúng tôi sẽ tạo hai mục nhập trong danh sách ngầm định của mình. Mục nhập đầu tiên là khối được phân bổ, mục nhập thứ hai là không gian còn lại.

Có hai cách đơn giản để lưu ý xem một khối đang được sử dụng hay có sẵn. Cách đầu tiên là lưu trữ nó dưới dạng một byte trong thông tin tiêu đề cùng với kích thước và cách thứ hai để mã hóa nó dưới dạng **bit thấp nhất** trong kích thước!
Do đó, thông tin kích thước khối sẽ bị giới hạn ở chỉ các giá trị chẵn:
```C
// Giả sử p là một kiểu con trỏ hợp lý, ví dụ: 'size_t *'.
isallocated = (*p) & 1;
realsize = (*p) & ~1;  // che đi bit thấp nhất
```

## Cân nhắc về căn chỉnh và làm tròn

Nhiều kiến ​​trúc mong đợi các **nguyên thủy đa byte** được căn chỉnh theo bội số nào đó của 2^n. Ví dụ: thông thường yêu cầu các kiểu 4 byte được căn chỉnh theo ranh giới 4 byte (và kiểu 8 byte trên ranh giới 8 byte). Nếu các nguyên thủy đa byte không được lưu trữ trên một ranh giới hợp lý (ví dụ: bắt đầu từ một địa chỉ lẻ) thì hiệu suất có thể bị ảnh hưởng đáng kể vì nó có thể yêu cầu hai yêu cầu đọc bộ nhớ thay vì một. Trên một số kiến ​​trúc, hình phạt thậm chí còn lớn hơn - chương trình sẽ bị lỗi với [lỗi bus](http://en.wikipedia.org/wiki/Bus_error#Unaligned_access).

Vì `malloc` không biết người dùng sẽ sử dụng bộ nhớ được phân bổ như thế nào (mảng số đôi? mảng ký tự?), nên con trỏ được trả về cho chương trình cần được **căn chỉnh** cho trường hợp xấu nhất, tùy thuộc vào kiến ​​trúc.

Từ tài liệu glibc, `malloc` glibc sử dụng phương pháp phỏng đoán sau:
"    Khối mà malloc cung cấp cho bạn được đảm bảo được căn chỉnh để nó có thể chứa bất kỳ loại dữ liệu nào. Trên các hệ thống GNU, địa chỉ luôn là bội số của tám trên hầu hết các hệ thống và là bội số của 16 trên hệ thống 64-bit."

Ví dụ: nếu bạn cần tính toán có bao nhiêu đơn vị 16 byte được yêu cầu, đừng quên **làm tròn lên** -
```C
int s = (requested_bytes + tag_overhead_bytes + 15) / 16
```
Hằng số bổ sung đảm bảo các đơn vị không hoàn chỉnh được làm tròn lên. Lưu ý, mã thực tế có nhiều khả năng biểu tượng kích thước, ví dụ: `sizeof(x) - 1`, thay vì mã hóa hằng số 15.

[Đây là một bài viết tuyệt vời về căn chỉnh bộ nhớ, nếu bạn quan tâm hơn nữa](http://www.ibm.com/developerworks/library/pa-dalign/)
## Một lưu ý về phân mảnh nội bộ

**Phân mảnh nội bộ** xảy ra khi khối bạn cung cấp cho họ lớn hơn kích thước phân bổ của họ. Giả sử rằng chúng ta có một khối trống có kích thước 16B (không bao gồm siêu dữ liệu). Nếu họ phân bổ 7 byte, bạn có thể muốn làm tròn lên 16B và chỉ trả về toàn bộ khối.

Điều này trở nên rất nham hiểm khi bạn triển khai hợp nhất và chia nhỏ (phần tiếp theo). Nếu bạn không triển khai một trong hai, thì bạn có thể kết thúc việc trả về một khối có kích thước 64B cho phân bổ 7B! Có _rất nhiều_ chi phí cho việc phân bổ đó mà chúng ta đang cố gắng tránh.

## Triển khai free
Khi `free` được gọi, chúng ta cần áp dụng lại độ lệch để quay lại điểm bắt đầu 'thực' của khối (hãy nhớ rằng chúng ta đã không cung cấp cho người dùng một con trỏ đến điểm bắt đầu thực tế của khối?), Tức là đến nơi chúng ta lưu trữ thông tin kích thước.

Một triển khai ngây thơ sẽ chỉ đơn giản là đánh dấu khối là **không sử dụng**. Nếu chúng ta đang lưu trữ trạng thái phân bổ khối trong bit kích thước thấp nhất, thì chúng ta chỉ cần xóa bit:
```C
*p = (*p) & ~1; // Xóa bit thấp nhất 
```
Tuy nhiên, chúng ta còn một chút việc phải làm: Nếu khối hiện tại và khối tiếp theo (nếu có) đều trống, chúng ta cần **hợp nhất** các khối này thành một khối duy nhất.
Tương tự, chúng ta cũng cần kiểm tra khối trước đó. Nếu điều đó tồn tại và đại diện cho một bộ nhớ chưa được phân bổ, thì chúng ta cần hợp nhất các khối thành một khối lớn duy nhất.

Để có thể hợp nhất một khối trống với một khối trống trước đó, chúng ta cũng sẽ cần tìm khối trước đó, vì vậy chúng ta cũng lưu trữ kích thước của khối ở cuối khối. Chúng được gọi là "**thẻ ranh giới**" (tham khảo Knuth73). Vì các khối liền kề nhau, phần cuối của một khối nằm ngay bên cạnh phần đầu của khối tiếp theo. Vì vậy, khối hiện tại (ngoài khối đầu tiên) có thể nhìn xa hơn một vài byte để tra cứu kích thước của khối trước đó. Với thông tin này, giờ đây bạn có thể nhảy ngược lại!

## Hiệu suất
Với mô tả ở trên, có thể xây dựng một bộ cấp phát bộ nhớ. Ưu điểm chính của nó là **đơn giản** - ít nhất là đơn giản so với các bộ cấp phát khác!
**Cấp phát bộ nhớ** là thao tác **thời gian tuyến tính** trường hợp xấu nhất (tìm kiếm danh sách liên kết cho một khối trống đủ lớn) và **giải phóng** là **thời gian không đổi** (không quá 3 khối sẽ cần được hợp nhất thành một khối duy nhất). Sử dụng bộ cấp phát này, có thể thử nghiệm các chiến lược sắp xếp khác nhau. Ví dụ: bạn có thể bắt đầu tìm kiếm từ nơi bạn giải phóng khối lần cuối hoặc nơi bạn phân bổ lần cuối. Nếu bạn lưu trữ con trỏ đến các khối, bạn cần phải rất cẩn thận rằng chúng luôn **hợp lệ** (ví dụ: khi hợp nhất các khối hoặc các lệnh gọi `malloc` hoặc `free` khác thay đổi cấu trúc heap)

## Bộ cấp phát Danh sách Trống Rõ ràng

Hiệu suất tốt hơn có thể đạt được bằng cách triển khai một **danh sách liên kết kép rõ ràng** các nút trống. Trong trường hợp đó, chúng ta có thể ngay lập tức di chuyển đến khối trống tiếp theo và khối trống trước đó. Điều này có thể giảm một nửa thời gian tìm kiếm, vì danh sách liên kết chỉ bao gồm các khối chưa được phân bổ.

Ưu điểm thứ hai là bây giờ chúng ta có một số quyền kiểm soát **thứ tự** của danh sách liên kết. Ví dụ: khi một khối được giải phóng, chúng ta có thể chọn chèn nó vào đầu danh sách liên kết thay vì luôn luôn ở giữa các lân cận của nó. Điều này được thảo luận bên dưới.

Chúng ta lưu trữ con trỏ của danh sách liên kết của chúng ta ở đâu? Một thủ thuật đơn giản là nhận ra rằng bản thân khối không được sử dụng và lưu trữ con trỏ tiếp theo và trước đó như một phần của khối (mặc dù bây giờ bạn phải đảm bảo rằng các khối trống luôn đủ lớn để chứa hai con trỏ).

Chúng ta vẫn cần triển khai **Thẻ Ranh giới** (tức là danh sách ngầm sử dụng kích thước), để chúng ta có thể giải phóng chính xác các khối và hợp nhất chúng với hai lân cận của chúng. Do đó, **danh sách trống rõ ràng** yêu cầu nhiều mã và độ phức tạp hơn.

Với danh sách liên kết rõ ràng, một thuật toán 'Tìm-đầu tiên' nhanh chóng và đơn giản được sử dụng để tìm liên kết đủ lớn đầu tiên. Tuy nhiên, vì thứ tự liên kết có thể được sửa đổi, nên điều này tương ứng với các chiến lược sắp xếp khác nhau. Ví dụ: nếu các liên kết được duy trì từ lớn nhất đến nhỏ nhất, thì điều này tạo ra chiến lược sắp xếp 'Vừa kém nhất'.

### Chính sách chèn danh sách liên kết rõ ràng
Khối mới được giải phóng có thể được chèn dễ dàng vào hai vị trí có thể: ở đầu hoặc theo thứ tự địa chỉ (bằng cách sử dụng các thẻ ranh giới để tìm các lân cận trước tiên).

Việc chèn vào đầu tạo ra một chính sách **LIFO** (vào sau, ra trước): Các không gian được giải phóng gần đây nhất sẽ được sử dụng lại. Các nghiên cứu cho thấy sự phân mảnh tồi tệ hơn so với sử dụng thứ tự địa chỉ.

Việc chèn theo thứ tự địa chỉ ("Chính sách thứ tự địa chỉ") chèn các khối được giải phóng để các khối được truy cập theo **thứ tự địa chỉ** tăng dần. Chính sách này yêu cầu nhiều thời gian hơn để giải phóng một khối vì các thẻ ranh giới (dữ liệu kích thước) phải được sử dụng để tìm các khối chưa được phân bổ tiếp theo và trước đó. Tuy nhiên, có ít phân mảnh hơn.

# Nghiên cứu tình huống: Bộ cấp phát Buddy (một ví dụ về danh sách phân đoạn)

Bộ cấp phát **phân đoạn** là bộ cấp phát chia heap thành các khu vực khác nhau được xử lý bởi các **bộ cấp phát con** khác nhau tùy thuộc vào kích thước của yêu cầu phân bổ. Kích thước được nhóm thành các lớp (ví dụ: **lũy thừa của hai**) và mỗi kích thước được xử lý bởi một bộ cấp phát con khác nhau và mỗi kích thước duy trì danh sách trống của riêng nó.

Một bộ cấp phát nổi tiếng thuộc loại này là bộ cấp phát **buddy**. Chúng tôi sẽ thảo luận về bộ cấp phát nhị phân buddy chia phân bổ thành các khối có kích thước 2 ^ n (n = 1, 2, 3, ...) lần một số đơn vị cơ sở byte, nhưng những bộ cấp phát khác cũng tồn tại (ví dụ: **phân chia Fibonacci** - bạn có thể xem tại sao nó được đặt tên như vậy không?). Khái niệm cơ bản rất đơn giản: Nếu không có khối trống có kích thước 2 ^ n, hãy chuyển đến cấp độ tiếp theo và lấy khối đó và chia nó thành hai. Nếu hai khối lân cận có cùng kích thước trở nên không được phân bổ, chúng có thể được **hợp nhất** lại với nhau thành một khối lớn duy nhất có kích thước gấp đôi.

Bộ cấp phát Buddy rất nhanh vì các khối lân cận để hợp nhất có thể được tính toán từ địa chỉ của khối được giải phóng, thay vì duyệt qua các **thẻ kích thước**. Hiệu suất tối ưu thường yêu cầu một lượng nhỏ mã **hợp ngữ** để sử dụng một lệnh CPU chuyên biệt để tìm bit khác không thấp nhất. 

Nhược điểm chính của bộ cấp phát Buddy là chúng bị *phân mảnh nội bộ*, bởi vì các phân bổ được **làm tròn** lên đến kích thước khối gần nhất. Ví dụ: một phân bổ 68 byte sẽ yêu cầu một khối 128 byte.



### Đọc thêm và Tài liệu tham khảo
* Xem [Hội nghị Khoa học Máy tính Lý thuyết và Công nghệ Phần mềm năm 1999](http://books.google.com/books?id=0uHME7EfjQEC&lpg=PP1&pg=PA85#v=onepage&q&f=false) (Google books, trang 85)
* Bài giảng ThanksForTheMemory UIUC ([pptx](https://subversion.ews.illinois.edu/svn/sp18-cs241/_shared/wikifiles/CS241-05-ThanksForTheMemorySlides.pptx)) ([pdf](https://subversion.ews.illinois.edu/svn/sp18-cs241/_shared/wikifiles/CS241-05-ThanksForTheMemorySlides.pdf))
và 
* [Trang phân bổ bộ nhớ buddy của Wikipedia](http://en.wikipedia.org/wiki/Buddy_memory_allocation)

# Các bộ cấp phát khác
Có rất nhiều lược đồ phân bổ khác. Ví dụ: [SLUB](http://en.wikipedia.org/wiki/SLUB_%28software%29) (wikipedia) - một trong ba bộ cấp phát được sử dụng nội bộ bởi Linux Kernel.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-1%3A-Heap-Memory-Introduction">
Quay lại: Bộ nhớ, Phần 1: Giới thiệu về Bộ nhớ Heap
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-3%3A-Smashing-the-Stack-Example">
Tiếp theo: Bộ nhớ, Phần 3: Ví dụ về Phá hủy Ngăn xếp
</a>
</div>





