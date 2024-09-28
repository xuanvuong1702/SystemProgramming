# Cấp phát bộ nhớ động C

## Điều gì xảy ra khi tôi gọi malloc?
Hàm `malloc` là một lệnh gọi thư viện C và được sử dụng để **dành** một khối bộ nhớ **liền kề**. Không giống như **bộ nhớ ngăn xếp**, bộ nhớ vẫn được phân bổ cho đến khi `free` được gọi với cùng một **con trỏ**. Ngoài ra còn có `calloc` và `realloc` được thảo luận bên dưới.

## Malloc có thể thất bại không?
Nếu `malloc` không thể dành thêm bộ nhớ thì nó sẽ trả về `NULL`. Các chương trình **mạnh mẽ** nên kiểm tra giá trị trả về. Nếu mã của bạn giả định `malloc` thành công và nó không thành công, thì chương trình của bạn có thể sẽ bị **lỗi phân đoạn** (segfault) khi cố gắng ghi vào địa chỉ 0.

## Heap ở đâu và nó lớn đến mức nào? 
**Heap** là một phần của bộ nhớ tiến trình và nó không có kích thước cố định. Cấp phát bộ nhớ heap được thực hiện bởi thư viện C khi bạn gọi `malloc` (`calloc`, `realloc`) và `free`.

Trước tiên, hãy xem lại nhanh về bộ nhớ tiến trình: **Tiến trình** là một phiên bản đang chạy của chương trình của bạn. Mỗi tiến trình có **không gian địa chỉ** riêng. Ví dụ: trên máy 32 bit, tiến trình của bạn nhận được khoảng 4 tỷ địa chỉ để sử dụng, tuy nhiên không phải tất cả chúng đều hợp lệ hoặc thậm chí được ánh xạ tới bộ nhớ vật lý (**RAM**) thực tế. Bên trong bộ nhớ của tiến trình, bạn sẽ tìm thấy mã thực thi, không gian cho **ngăn xếp**, **biến môi trường**, biến **toàn cục** (tĩnh) và heap.

Bằng cách gọi `sbrk`, thư viện C có thể tăng kích thước của heap khi chương trình của bạn yêu cầu nhiều bộ nhớ heap hơn. Vì heap và ngăn xếp (một cho mỗi **luồng**) cần phát triển, chúng tôi đặt chúng ở hai đầu đối diện của không gian địa chỉ. Vì vậy, đối với các kiến ​​trúc điển hình, heap sẽ phát triển lên trên và ngăn xếp phát triển xuống dưới. 

Sự thật: **Bộ cấp phát bộ nhớ** hệ điều hành hiện đại không còn cần `sbrk` - thay vào đó, chúng có thể yêu cầu các vùng độc lập của **bộ nhớ ảo** và duy trì nhiều vùng bộ nhớ. Ví dụ: các yêu cầu gigabyte có thể được đặt trong một vùng bộ nhớ khác với các yêu cầu phân bổ nhỏ. Tuy nhiên, chi tiết này là một sự phức tạp không mong muốn: Các vấn đề về phân mảnh và phân bổ bộ nhớ hiệu quả vẫn được áp dụng, vì vậy chúng tôi sẽ bỏ qua sự tinh tế về triển khai này ở đây và sẽ viết như thể heap là một vùng duy nhất.

Nếu chúng ta viết một chương trình đa luồng (sẽ nói thêm về điều đó sau), chúng ta sẽ cần nhiều ngăn xếp (một cho mỗi luồng) nhưng chỉ có một heap.

Trên các kiến ​​trúc điển hình, heap là một phần của `Phân đoạn dữ liệu` và bắt đầu ngay phía trên mã và các biến toàn cục. 

## Các chương trình có cần gọi brk hoặc sbrk không?
Thông thường là không (mặc dù việc gọi `sbrk(0)` có thể thú vị vì nó cho bạn biết heap của bạn hiện đang kết thúc ở đâu). Thay vào đó, các chương trình sử dụng `malloc, calloc, realloc` và `free` là một phần của thư viện C. Việc triển khai nội bộ các hàm này sẽ gọi `sbrk` khi cần thêm bộ nhớ heap.
```C
void *top_of_heap = sbrk(0);
malloc(16384);
void *top_of_heap2 = sbrk(0);
printf("Đỉnh của heap đã đi từ %p đến %p \n", top_of_heap, top_of_heap2);
```
Ví dụ đầu ra: `Đỉnh của heap đã đi từ 0x4000 đến 0xa000`


## Calloc là gì?
Không giống như `malloc`, `calloc` **khởi tạo** nội dung bộ nhớ thành 0 và cũng nhận hai đối số (số lượng mục và kích thước tính bằng byte của mỗi mục). Một triển khai ngây thơ nhưng dễ đọc của `calloc` trông như thế này:
```C
void *calloc(size_t n, size_t size)
{
	size_t total = n * size; // Không kiểm tra tràn!
	void *result = malloc(total);
	
	if (!result) return NULL;
	
// Nếu chúng ta đang sử dụng các trang bộ nhớ mới 
// vừa được phân bổ từ hệ thống bằng cách gọi sbrk
// thì chúng sẽ bằng 0 nên việc đưa về 0 là không cần thiết,

	memset(result, 0, total);
	return result; 
}
```
Một cuộc thảo luận nâng cao về những hạn chế này là [ở đây](http://locklessinc.com/articles/calloc/).

Các lập trình viên thường sử dụng `calloc` thay vì gọi `memset` một cách rõ ràng sau `malloc`, để đặt nội dung bộ nhớ thành 0. Lưu ý `calloc(x,y)` giống hệt với `calloc(y,x)`, nhưng bạn nên tuân theo các quy ước của hướng dẫn sử dụng.

```C
// Đảm bảo bộ nhớ của chúng tôi được khởi tạo thành 0
link_t *link  = malloc(256);
memset(link, 0, 256); // Giả định malloc trả về một địa chỉ hợp lệ!

link_t *link = calloc(1, 256); // an toàn hơn: calloc(1, sizeof(link_t));
```
## Tại sao bộ nhớ được trả về lần đầu tiên bởi sbrk được khởi tạo thành 0?
Nếu hệ điều hành không đặt nội dung của RAM vật lý về 0, một tiến trình có thể biết về bộ nhớ của một tiến trình khác đã sử dụng bộ nhớ trước đó. Đây sẽ là một **lỗ hổng bảo mật**.

Thật không may, điều này có nghĩa là đối với các yêu cầu `malloc` trước khi bất kỳ bộ nhớ nào được giải phóng và các chương trình đơn giản (cuối cùng sử dụng bộ nhớ mới được **dành riêng** từ hệ thống), bộ nhớ _thường_ bằng 0. Sau đó, các lập trình viên viết sai các chương trình C giả định bộ nhớ được `malloc` sẽ _luôn_ bằng 0.

```C
char* ptr = malloc(300);
// nội dung có thể bằng 0 vì chúng tôi nhận được bộ nhớ hoàn toàn mới
// vì vậy các chương trình dành cho người mới bắt đầu có vẻ hoạt động!
// strcpy(ptr, "Một số dữ liệu"); // làm việc với dữ liệu
free(ptr);
// sau đó
char *ptr2 = malloc(308); // Nội dung bây giờ có thể chứa dữ liệu hiện có và có thể không bằng 0
```

## Tại sao malloc không phải lúc nào cũng khởi tạo bộ nhớ thành 0?
Hiệu suất! Chúng tôi muốn malloc càng nhanh càng tốt. Việc đưa bộ nhớ về 0 có thể là không cần thiết.

## Realloc là gì và khi nào bạn sẽ sử dụng nó?
`realloc` cho phép bạn thay đổi kích thước của một phân bổ bộ nhớ hiện có đã được phân bổ trước đó trên heap (thông qua `malloc`, `calloc` hoặc `realloc`). Cách sử dụng phổ biến nhất của `realloc` là thay đổi kích thước bộ nhớ được sử dụng để chứa một **mảng** giá trị. Một phiên bản ngây thơ nhưng dễ đọc của `realloc` được đề xuất bên dưới
```C
void * realloc(void * ptr, size_t newsize) {
  // Triển khai đơn giản luôn dành nhiều bộ nhớ hơn
  // và không có kiểm tra lỗi
  void *result = malloc(newsize); 
  size_t oldsize =  ... //(phụ thuộc vào cấu trúc dữ liệu nội bộ của bộ cấp phát)
  if (ptr) memcpy(result, ptr, newsize < oldsize ? newsize : oldsize);
  free(ptr);
  return result;
}
```
Cách sử dụng `realloc` SAI được hiển thị bên dưới:
```C
int *array = malloc(sizeof(int) * 2);
array[0] = 10; array[1] = 20;
// Ôi trời cần một mảng lớn hơn - vì vậy hãy sử dụng realloc ..
realloc (array, 3); // LỖI!
array[2] = 30; 
```

Mã trên chứa hai lỗi. Đầu tiên, chúng ta cần 3 * sizeof(int) byte chứ không phải 3 byte.
Thứ hai, `realloc` có thể cần phải di chuyển nội dung hiện có của bộ nhớ đến một vị trí mới. Ví dụ: có thể không có đủ dung lượng vì các byte lân cận đã được phân bổ. Cách sử dụng `realloc` chính xác được hiển thị bên dưới.
```C
array = realloc(array, 3 * sizeof(int));
// Nếu mảng được sao chép đến một vị trí mới thì phân bổ cũ sẽ được giải phóng.
```
Một phiên bản mạnh mẽ cũng sẽ kiểm tra giá trị trả về `NULL`. Lưu ý `realloc` có thể được sử dụng để tăng và giảm phân bổ. 

## Tôi có thể đọc thêm ở đâu?
Xem [trang hướng dẫn sử dụng](http://man7.org/linux/man-pages/man3/malloc.3.html)!

## Việc cấp phát bộ nhớ nhanh quan trọng như thế nào?
Rất! Việc phân bổ và **giải phóng** bộ nhớ heap là một thao tác phổ biến trong hầu hết các ứng dụng.

# Giới thiệu về Cấp phát

## Triển khai malloc và free ngớ ngẩn nhất là gì và có gì sai với nó?

```C
void* malloc(size_t size)
// Yêu cầu hệ thống cấp thêm byte bằng cách mở rộng không gian heap. 
// sbrk Trả về -1 khi thất bại
   void *p = sbrk(size); 
   if(p == (void *) -1) return NULL; // Hết dung lượng
   return p;
}
void free() {/* Không làm gì cả */}
```
Triển khai ở trên có hai nhược điểm chính:
* Các **lệnh gọi hệ thống** chậm (so với các lệnh gọi thư viện). Chúng ta nên dành một lượng lớn bộ nhớ và chỉ thỉnh thoảng yêu cầu thêm từ hệ thống.
* Không **tái sử dụng** bộ nhớ đã giải phóng. Chương trình của chúng tôi không bao giờ sử dụng lại bộ nhớ heap - nó chỉ liên tục yêu cầu một heap lớn hơn.

Nếu bộ cấp phát này được sử dụng trong một chương trình điển hình, tiến trình sẽ nhanh chóng cạn kiệt tất cả bộ nhớ khả dụng.
Thay vào đó, chúng ta cần một bộ cấp phát có thể sử dụng hiệu quả không gian heap và chỉ yêu cầu thêm bộ nhớ khi cần thiết.

## Chiến lược sắp xếp là gì?
Trong quá trình thực thi chương trình, bộ nhớ được phân bổ và giải phóng (**giải phóng**), vì vậy sẽ có các **khoảng trống** (lỗ) trong bộ nhớ heap có thể được sử dụng lại cho các yêu cầu bộ nhớ trong tương lai. Bộ cấp phát bộ nhớ cần theo dõi phần nào của heap hiện đang được phân bổ và phần nào có sẵn.

Giả sử kích thước heap hiện tại của chúng ta là 64K, mặc dù không phải tất cả đều được sử dụng vì một số bộ nhớ được malloc trước đó đã được giải phóng bởi chương trình: 

16KB trống | 10KB được phân bổ | 1KB trống | 1KB được phân bổ | 30KB trống | 4KB được phân bổ | 2KB trống 
---|---|---|---|---|---|---

Nếu một yêu cầu `malloc` mới cho 2KB được thực thi (`malloc(2048)`), thì `malloc` nên dành bộ nhớ ở đâu? Nó có thể sử dụng **lỗ** 2KB cuối cùng (vừa đúng kích thước!) hoặc nó có thể chia một trong hai lỗ trống còn lại. Những lựa chọn này đại diện cho các **chiến lược sắp xếp** khác nhau.

Cho dù lỗ nào được chọn, bộ cấp phát sẽ cần chia lỗ thành hai phần: Không gian mới được phân bổ (sẽ được trả về cho chương trình) và một lỗ nhỏ hơn (nếu còn dung lượng trống).

Chiến lược **Vừa khít nhất** tìm thấy lỗ nhỏ nhất có đủ kích thước (ít nhất là 2KB):

16KB trống | 10KB được phân bổ | 1KB trống | 1KB được phân bổ | 30KB trống | 4KB được phân bổ | `2KB Ở ĐÂY!`
---|---|---|---|---|---|---

Chiến lược **Vừa kém nhất** tìm thấy lỗ lớn nhất có đủ kích thước (vì vậy hãy chia lỗ 30KB thành hai phần):

16KB trống | 10KB được phân bổ | 1KB trống | 1KB được phân bổ | `2KB Ở ĐÂY!` | `28KB trống` | 4KB được phân bổ | 2KB trống 
---|---|---|---|---|---|---|---

Chiến lược **Vừa đầu tiên** tìm thấy lỗ trống đầu tiên có đủ kích thước (chia lỗ 16KB thành hai phần):

`2KB Ở ĐÂY!` | `14KB trống` | 10KB được phân bổ | 1KB trống | 1KB được phân bổ | 30KB trống | 4KB được phân bổ | 2KB trống 
---|---|---|---|---|---|---|---


## Phân mảnh ngoài là gì?
Trong ví dụ bên dưới, trong số 64KB bộ nhớ heap, 17KB được phân bổ và 47KB trống. Tuy nhiên, khối khả dụng lớn nhất chỉ là 30KB vì bộ nhớ heap chưa được phân bổ khả dụng của chúng tôi bị **phân mảnh** thành các phần nhỏ hơn. 

 `16KB trống` | 10KB được phân bổ | `1KB trống` | 1KB được phân bổ | `30KB trống` | 4KB được phân bổ | 2KB được phân bổ 
---|---|---|---|---|---|---

## Chiến lược sắp xếp có ảnh hưởng gì đến phân mảnh ngoài và hiệu suất?
Các chiến lược khác nhau ảnh hưởng đến sự phân mảnh của bộ nhớ heap theo những cách không rõ ràng, điều này chỉ được phát hiện bằng cách phân tích toán học hoặc **mô phỏng** cẩn thận trong các điều kiện thực tế (ví dụ: mô phỏng các yêu cầu phân bổ bộ nhớ của cơ sở dữ liệu hoặc máy chủ web).
Ví dụ: thoạt nhìn, Vừa khít nhất có vẻ là một chiến lược tuyệt vời, tuy nhiên, nếu chúng ta không thể tìm thấy một lỗ có kích thước hoàn hảo thì vị trí này sẽ tạo ra nhiều lỗ nhỏ không thể sử dụng được, dẫn đến phân mảnh cao. Nó cũng yêu cầu quét tất cả các lỗ có thể.

Vừa đầu tiên có ưu điểm là nó sẽ không đánh giá tất cả các vị trí có thể và do đó sẽ nhanh hơn. 

Vì Vừa kém nhất nhắm mục tiêu vào không gian chưa được phân bổ lớn nhất nên đó là một lựa chọn kém nếu yêu cầu phân bổ lớn.

Trong thực tế, vừa đầu tiên và vừa tiếp theo (không được thảo luận ở đây) thường là chiến lược sắp xếp phổ biến. Các phương pháp tiếp cận lai và nhiều lựa chọn thay thế khác tồn tại (xem trang triển khai **bộ cấp phát bộ nhớ**).
 
## Những thách thức khi viết bộ cấp phát heap là gì?
Những thách thức chính là,
* Cần giảm thiểu phân mảnh (tức là tối đa hóa việc sử dụng bộ nhớ)
* Cần hiệu suất cao
* Triển khai phức tạp (nhiều thao tác con trỏ bằng cách sử dụng danh sách liên kết và số học con trỏ)

Một số nhận xét bổ sung:

Cả phân mảnh và hiệu suất đều phụ thuộc vào **hồ sơ phân bổ** ứng dụng, có thể được đánh giá nhưng không thể dự đoán và trong thực tế, trong các điều kiện sử dụng cụ thể, bộ cấp phát mục đích đặc biệt thường có thể hoạt động tốt hơn triển khai mục đích chung.

Bộ cấp phát không biết trước các yêu cầu phân bổ bộ nhớ của chương trình. Ngay cả khi chúng tôi đã làm, đây là [Vấn đề Ba lô](http://en.wikipedia.org/wiki/Knapsack_problem) được biết là khó NP!


## Làm cách nào để bạn triển khai bộ cấp phát bộ nhớ?
Câu hỏi hay. Xem phần tiếp theo để biết cách triển khai.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator">
Tiếp theo: Bộ nhớ, Phần 2: Triển khai Bộ cấp phát Bộ nhớ
</a>
</div>

