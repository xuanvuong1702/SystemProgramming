# Bộ nhớ ảo là gì?

Trong các hệ thống nhúng rất đơn giản và các máy tính ban đầu, các tiến trình truy cập trực tiếp vào bộ nhớ, tức là "Địa chỉ 1234" tương ứng với một byte cụ thể được lưu trữ trong một phần cụ thể của bộ nhớ vật lý.
Trong các hệ thống hiện đại, điều này không còn đúng nữa. Thay vào đó, mỗi tiến trình được cách ly; và có một quy trình dịch giữa địa chỉ của một lệnh CPU cụ thể hoặc một phần dữ liệu của một tiến trình và byte thực tế của bộ nhớ vật lý ("RAM"). Các địa chỉ bộ nhớ không còn là địa chỉ 'thật'; tiến trình chạy bên trong bộ nhớ ảo. Bộ nhớ ảo không chỉ giữ cho các tiến trình an toàn (vì một tiến trình không thể trực tiếp đọc hoặc sửa đổi bộ nhớ của một tiến trình khác) mà còn cho phép hệ thống phân bổ và phân bổ lại hiệu quả các phần bộ nhớ cho các tiến trình khác nhau.

## MMU là gì?

Đơn vị Quản lý Bộ nhớ (MMU) là một phần của CPU. Nó chuyển đổi địa chỉ bộ nhớ ảo thành địa chỉ vật lý. MMU cũng có thể làm gián đoạn CPU nếu hiện tại không có ánh xạ từ một địa chỉ ảo cụ thể đến một địa chỉ vật lý hoặc nếu lệnh CPU hiện tại cố gắng ghi vào vị trí mà tiến trình chỉ có quyền truy cập đọc.

## Vậy làm cách nào để chuyển đổi địa chỉ ảo thành địa chỉ vật lý?

Hãy tưởng tượng bạn có một máy 32 bit. Con trỏ có thể chứa 32 bit, tức là chúng có thể định địa chỉ 2^32 vị trí khác nhau, tức là 4GB bộ nhớ (chúng ta sẽ tuân theo quy ước tiêu chuẩn là một địa chỉ có thể chứa một byte).

Hãy tưởng tượng chúng ta có một bảng lớn - đây là phần thông minh - được lưu trữ trong bộ nhớ! Đối với mọi địa chỉ có thể có (tất cả 4 tỷ địa chỉ trong số đó), chúng ta sẽ lưu trữ 'thực', tức là địa chỉ vật lý. Mỗi địa chỉ vật lý sẽ cần 4 byte (để chứa 32 bit).
Sơ đồ này sẽ yêu cầu 16 tỷ byte để lưu trữ tất cả các mục. Ồ - sơ đồ tra cứu của chúng ta sẽ tiêu thụ hết bộ nhớ mà chúng ta có thể mua cho máy 4GB của mình.
Chúng ta cần phải làm tốt hơn thế này. Bảng tra cứu của chúng ta tốt hơn là nhỏ hơn bộ nhớ mà chúng ta có, nếu không chúng ta sẽ không còn dung lượng cho các chương trình thực tế và dữ liệu hệ điều hành của mình.
Giải pháp là chia nhỏ bộ nhớ thành các vùng nhỏ gọi là 'trang' và 'khung' và sử dụng bảng tra cứu cho mỗi trang.

# Trang là gì? Có bao nhiêu trang?

Trang là một khối bộ nhớ ảo. Kích thước khối điển hình trên hệ điều hành Linux là 4KB (tức là 2^12 địa chỉ), mặc dù bạn có thể tìm thấy các ví dụ về khối lớn hơn.

Vì vậy, thay vì nói về các byte riêng lẻ, chúng ta có thể nói về các khối 4KB, mỗi khối được gọi là một trang. Chúng ta cũng có thể đánh số các trang của mình ("Trang 0" "Trang 1" v.v.)

## Ví dụ: Có bao nhiêu trang trong một máy 32 bit (giả sử kích thước trang là 4KB)?

Trả lời: 2^32 địa chỉ / 2^12 = 2^20 trang.

Hãy nhớ rằng 2^10 là 1024, vì vậy 2^20 lớn hơn một triệu một chút.

Đối với máy 64 bit, 2^64 / 2^12 = 2^52, tức là khoảng 10^15 trang.

## Khung là gì?

Khung (hoặc đôi khi được gọi là 'khung trang') là một khối _bộ nhớ vật lý_ hoặc RAM (= Bộ nhớ Truy cập Ngẫu nhiên). Loại bộ nhớ này đôi khi được gọi là 'bộ nhớ chính' (và trái ngược với bộ nhớ thứ cấp chậm hơn, chẳng hạn như đĩa quay có thời gian truy cập thấp hơn)

Khung có cùng số byte với một trang ảo. Nếu một máy 32 bit có 2^32 (4GB) RAM, thì sẽ có cùng số lượng trong không gian địa chỉ của máy. Không chắc rằng một máy 64 bit sẽ bao giờ có 2^64 byte RAM - bạn có thể thấy tại sao?

## Bảng trang là gì và nó lớn như thế nào?

Bảng trang là một ánh xạ giữa một trang và một khung.
Ví dụ: Trang 1 có thể được ánh xạ tới khung 45, trang 2 được ánh xạ tới khung 30. Các khung khác có thể hiện đang không được sử dụng hoặc được gán cho các tiến trình đang chạy khác hoặc được hệ điều hành sử dụng nội bộ.

Một bảng trang đơn giản chỉ là một mảng, `int frame = table[ page_num ];`

Đối với một máy 32 bit có các trang 4KB, mỗi mục cần chứa một số khung - tức là 20 bit vì chúng tôi đã tính toán có 2^20 khung. Đó là 2,5 byte cho mỗi mục! Trong thực tế, chúng ta sẽ làm tròn lên 4 byte cho mỗi mục và tìm cách sử dụng cho những bit dự phòng đó. Với 4 byte cho mỗi mục x 2^20 mục = 4 MB bộ nhớ vật lý được yêu cầu để chứa bảng trang.

Đối với một máy 64 bit có các trang 4KB, mỗi mục cần 52 bit. Hãy làm tròn lên 64 bit (8 byte) cho mỗi mục. Với 2^52 mục, đó là 2^55 byte (khoảng 40 petabyte...) Oops bảng trang của chúng ta quá lớn.

Trong kiến trúc 64 bit, địa chỉ bộ nhớ thưa thớt, vì vậy chúng ta cần một cơ chế để giảm kích thước bảng trang, do hầu hết các mục sẽ không bao giờ được sử dụng.

![](http://www.cs.odu.edu/~cs471w/spring12/lectures/MainMemory_files/image028.jpg)

Một ví dụ trực quan về bảng trang ở đây. Hãy tưởng tượng việc truy cập một mảng và lấy các phần tử mảng.

## Độ lệch là gì và nó được sử dụng như thế nào?

Hãy nhớ rằng bảng trang của chúng ta ánh xạ các trang tới các khung, nhưng mỗi trang là một khối các địa chỉ liền kề. Làm cách nào để tính toán byte cụ thể nào sẽ sử dụng bên trong một khung cụ thể? Giải pháp là sử dụng lại trực tiếp các bit thấp nhất của địa chỉ bộ nhớ ảo. Ví dụ: giả sử tiến trình của chúng ta đang đọc địa chỉ sau-
```Địa chỉ ảo = 11110000111100001111000010101010 (nhị phân)```

Trên một máy có kích thước trang là 256 byte, thì 8 bit thấp nhất (10101010) sẽ được sử dụng làm độ lệch.
Các bit cao hơn còn lại sẽ là số trang (111100001111000011110000).


## Bảng trang đa cấp

Các bảng trang đa cấp là một giải pháp cho vấn đề kích thước bảng trang cho kiến trúc 64 bit. Chúng ta sẽ xem xét triển khai đơn giản nhất - bảng trang hai cấp. Mỗi bảng là một danh sách các con trỏ trỏ đến bảng cấp tiếp theo, không phải tất cả các bảng con đều cần tồn tại. Một ví dụ, bảng trang hai cấp cho kiến trúc 32 bit được hiển thị bên dưới-

```
Địa chỉ ảo = 11110000111111110000000010101010 (nhị phân)
                 |_Chỉ mục 1_||        ||          | Chỉ mục thư mục 10 bit
                           |_Chỉ mục 2_||          | Chỉ mục bảng con 10 bit
                                     |__________| Độ lệch 12 bit (được chuyển trực tiếp đến RAM)
```
Trong sơ đồ trên, việc xác định số khung yêu cầu hai lần đọc bộ nhớ: 10 bit trên cùng được sử dụng trong một thư mục của các bảng trang. Nếu 2 byte được sử dụng cho mỗi mục, chúng ta chỉ cần 2KB để lưu trữ toàn bộ thư mục này. Mỗi bảng con sẽ trỏ đến các khung vật lý (tức là cần 4 byte để lưu trữ 20 bit). Tuy nhiên, đối với các tiến trình chỉ có nhu cầu bộ nhớ nhỏ, chúng ta chỉ cần chỉ định các mục nhập cho địa chỉ bộ nhớ thấp (cho heap và mã chương trình) và địa chỉ bộ nhớ cao (cho stack). Mỗi bảng con là 1024 mục x 4 byte, tức là 4KB cho mỗi bảng con.

Do đó, tổng chi phí bộ nhớ cho bảng trang đa cấp của chúng ta đã giảm từ 4MB (đối với triển khai một cấp) xuống còn 3 khung bộ nhớ (12KB)! Đây là lý do: Chúng ta cần ít nhất một khung cho thư mục cấp cao và hai khung cho chỉ hai bảng con. Một bảng con là cần thiết cho các địa chỉ thấp (mã chương trình, hằng số và có thể là một heap nhỏ), bảng con kia là dành cho các địa chỉ cao hơn được sử dụng bởi môi trường và stack. Trong thực tế, các chương trình thực có thể sẽ cần nhiều mục nhập bảng con hơn, vì mỗi bảng con chỉ có thể tham chiếu 1024 * 4KB = 4MB không gian địa chỉ nhưng điểm chính vẫn là - chúng ta đã giảm đáng kể chi phí bộ nhớ cần thiết để thực hiện tra cứu bảng trang.


## Bảng trang có làm cho việc truy cập bộ nhớ chậm hơn không? (Và TLB là gì?)

Có - Đáng kể! (Nhưng nhờ phần cứng thông minh, thường là không...)
So với việc đọc hoặc ghi bộ nhớ trực tiếp.
Đối với một bảng trang duy nhất, máy của chúng ta hiện chậm gấp đôi! (Cần hai lần truy cập bộ nhớ)
Đối với bảng trang hai cấp, việc truy cập bộ nhớ hiện chậm gấp ba lần. (Cần ba lần truy cập bộ nhớ)

Để khắc phục chi phí này, MMU bao gồm một bộ nhớ đệm kết hợp các tra cứu ảo-trang-khung được sử dụng gần đây. Bộ nhớ đệm này được gọi là TLB ("Translation Lookaside Buffer"). Mỗi khi cần dịch địa chỉ ảo thành vị trí bộ nhớ vật lý, TLB sẽ được truy vấn song song với bảng trang. Đối với hầu hết các lần truy cập bộ nhớ của hầu hết các chương trình, có khả năng cao là TLB đã lưu trữ kết quả. Tuy nhiên, nếu một chương trình không có tính nhất quán bộ nhớ cache tốt (ví dụ: đang đọc từ các vị trí bộ nhớ ngẫu nhiên của nhiều trang khác nhau) thì TLB sẽ không có kết quả được lưu trữ và lúc này MMU phải sử dụng bảng trang chậm hơn nhiều để xác định khung vật lý.

![](https://upload.wikimedia.org/wikipedia/commons/thumb/8/8e/X86_Paging_4K.svg/440px-X86_Paging_4K.svg.png)

Đây có thể là cách chia nhỏ một bảng trang đa cấp.

# Khung nâng cao và Bảo vệ trang

## Các khung có thể được chia sẻ giữa các tiến trình không? Chúng có thể được chuyên biệt hóa không?

Có! Ngoài việc lưu trữ số khung, bảng trang có thể được sử dụng để lưu trữ thông tin về việc tiến trình có thể ghi hay chỉ đọc một khung cụ thể hay không. Sau đó, các khung chỉ đọc có thể được chia sẻ an toàn giữa nhiều tiến trình. Ví dụ: mã lệnh thư viện C có thể được chia sẻ giữa tất cả các tiến trình tải mã động vào bộ nhớ tiến trình. Mỗi tiến trình chỉ có thể đọc bộ nhớ đó. Có nghĩa là nếu bạn cố gắng ghi vào một trang chỉ đọc trong bộ nhớ, bạn sẽ nhận được lỗi `SEGFAULT`. Đó là lý do tại sao đôi khi truy cập bộ nhớ bị segfault và đôi khi không, tất cả phụ thuộc vào việc phần cứng của bạn có cho phép bạn truy cập hay không.

Ngoài ra, các tiến trình có thể chia sẻ một trang với tiến trình con bằng cách sử dụng lệnh gọi hệ thống `mmap`. `mmap` là một lệnh gọi thú vị vì thay vì gắn mỗi địa chỉ ảo vào một khung vật lý, nó gắn nó vào một thứ khác. Thứ gì đó khác có thể là một tệp, một đơn vị GPU hoặc bất kỳ thao tác ánh xạ bộ nhớ nào khác mà bạn có thể nghĩ ra! Việc ghi vào địa chỉ bộ nhớ có thể ghi xuyên suốt vào thiết bị hoặc việc ghi có thể bị hệ điều hành tạm dừng nhưng đây là một sự trừu tượng hóa rất mạnh mẽ vì thông thường hệ điều hành có thể thực hiện tối ưu hóa (nhiều tiến trình ánh xạ bộ nhớ cùng một tệp có thể khiến kernel tạo một ánh xạ).

## Những thông tin gì khác được lưu trữ trong bảng trang và tại sao?

Ngoài bit chỉ đọc và thống kê sử dụng được thảo luận ở trên, thông tin chỉ đọc, sửa đổi và thực thi thường được lưu trữ.

## Lỗi trang là gì?

Lỗi trang xảy ra khi một chương trình đang chạy cố gắng truy cập vào một số bộ nhớ ảo trong không gian địa chỉ của nó mà không được ánh xạ tới bộ nhớ vật lý. Lỗi trang cũng sẽ xảy ra trong các tình huống khác.

Có ba loại Lỗi trang:

**Lỗi trang nhỏ**  xảy ra khi chưa có ánh xạ cho trang, nhưng đó là địa chỉ hợp lệ. Đây có thể là bộ nhớ được yêu cầu bởi `sbrk(2)` nhưng chưa được ghi vào, có nghĩa là hệ điều hành có thể đợi lần ghi đầu tiên trước khi cấp phát không gian. Hệ điều hành chỉ đơn giản là tạo trang, tải nó vào bộ nhớ và chuyển sang.

**Lỗi trang lớn** xảy ra khi ánh xạ tới trang không có trong bộ nhớ mà nằm trên đĩa. Điều này sẽ làm là hoán đổi trang vào bộ nhớ và hoán đổi một trang khác ra. Nếu điều này xảy ra đủ thường xuyên, chương trình của bạn được cho là đang _thrashing_ MMU.

**Lỗi trang không hợp lệ** xảy ra khi bạn cố gắng ghi vào địa chỉ bộ nhớ không thể ghi hoặc đọc vào địa chỉ bộ nhớ không thể đọc. MMU tạo ra lỗi không hợp lệ và hệ điều hành thường sẽ tạo ra `SIGSEGV` có nghĩa là vi phạm phân đoạn, có nghĩa là bạn đã ghi ra ngoài phân đoạn mà bạn có thể ghi.

### Bit chỉ đọc

Bit chỉ đọc đánh dấu trang là chỉ đọc. Nỗ lực ghi vào trang sẽ gây ra lỗi trang. Lỗi trang sau đó sẽ được Kernel xử lý. Hai ví dụ về trang chỉ đọc bao gồm chia sẻ thư viện thời gian chạy C giữa nhiều tiến trình (vì lý do bảo mật, bạn sẽ không muốn cho phép một tiến trình sửa đổi thư viện); và Sao chép khi ghi, trong đó chi phí sao chép một trang có thể bị trì hoãn cho đến khi lần ghi đầu tiên xảy ra.

### Bit bẩn

http://en.wikipedia.org/wiki/Page_table#Page_table_data
> Bit bẩn cho phép tối ưu hóa hiệu suất. Một trang trên đĩa được phân trang vào bộ nhớ vật lý, sau đó được đọc từ và sau đó được phân trang ra một lần nữa không cần phải được ghi lại vào đĩa, vì trang chưa thay đổi. Tuy nhiên, nếu trang đã được ghi vào sau khi được phân trang, bit bẩn của nó sẽ được đặt, cho biết rằng trang phải được ghi lại vào bộ nhớ dự phòng. Chiến lược này yêu cầu bộ nhớ dự phòng giữ lại một bản sao của trang sau khi nó được phân trang vào bộ nhớ. Khi bit bẩn không được sử dụng, bộ nhớ dự phòng chỉ cần có kích thước bằng tổng kích thước tức thời của tất cả các trang được phân trang ra tại bất kỳ thời điểm nào. Khi bit bẩn được sử dụng, mọi lúc một số trang sẽ tồn tại trong cả bộ nhớ vật lý và bộ nhớ dự phòng.

### Bit thực thi

Bit thực thi xác định xem các byte trong một trang có thể được thực thi như các lệnh CPU hay không. Bằng cách vô hiệu hóa một trang, nó ngăn mã được lưu trữ độc hại trong bộ nhớ tiến trình (ví dụ: do tràn ngăn xếp) dễ dàng bị thực thi. (đọc thêm: http://en.wikipedia.org/wiki/NX_bit#Hardware_background)


### Tìm hiểu thêm

Một cuộc thảo luận cấp thấp hơn và kỹ thuật hơn về phân trang và bit trang trên nền tảng x86 được thảo luận tại [http://wiki.osdev.org/Paging].


