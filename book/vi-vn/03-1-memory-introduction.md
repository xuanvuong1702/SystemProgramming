---
title: "Chương 03: Bộ nhớ"
type: book-vi-vn
order: 6
---

# Cấp phát bộ nhớ động trong C

## Điều gì xảy ra khi tôi gọi malloc?
Hàm `malloc` là một lệnh thư viện C và được sử dụng để dành một khối bộ nhớ liên tục. Khác với bộ nhớ stack, bộ nhớ này vẫn được cấp phát cho đến khi `free` được gọi với con trỏ giống nhau. Cũng có `calloc` và `realloc` được thảo luận bên dưới.

## Malloc có thể thất bại không?
Nếu `malloc` không thể dành thêm bất kỳ bộ nhớ nào thì nó sẽ trả về `NULL`. Các chương trình mạnh mẽ nên kiểm tra giá trị trả về. Nếu mã của bạn giả định `malloc` thành công và nó không thành công, thì chương trình của bạn sẽ có khả năng bị lỗi (segfault) khi nó cố gắng ghi vào địa chỉ 0.

## Heap nằm ở đâu và kích thước của nó là bao nhiêu?
Heap là một phần của bộ nhớ quá trình và nó không có kích thước cố định. Việc cấp phát bộ nhớ heap được thực hiện bởi thư viện C khi bạn gọi `malloc` (`calloc`, `realloc`) và `free`.

Trước hết, hãy xem lại về bộ nhớ quá trình: Một quá trình là một phiên bản đang chạy của chương trình của bạn. Mỗi quá trình có không gian địa chỉ riêng của nó. Ví dụ, trên một máy 32 bit, quá trình của bạn nhận được khoảng 4 tỷ địa chỉ để sử dụng, tuy nhiên không phải tất cả đều hợp lệ hoặc thậm chí được ánh xạ đến bộ nhớ vật lý thực sự (RAM). Bên trong bộ nhớ của quá trình, bạn sẽ tìm thấy mã thực thi, không gian cho stack, biến môi trường, biến toàn cục (static) và heap.

Bằng cách gọi `sbrk`, thư viện C có thể tăng kích thước của heap khi chương trình của bạn yêu cầu thêm bộ nhớ heap. Khi heap và stack (một cho mỗi thread) cần phải phát triển, chúng ta đặt chúng ở hai đầu không gian địa chỉ. Vì vậy, đối với các kiến trúc thông thường, heap sẽ phát triển lên trên và stack phát triển xuống dưới.

Sự thật: Các bộ cấp phát bộ nhớ hệ điều hành hiện đại không còn cần `sbrk` - thay vào đó, chúng có thể yêu cầu các vùng bộ nhớ ảo độc lập và duy trì nhiều vùng bộ nhớ. Ví dụ, các yêu cầu gigabyte có thể được đặt trong một vùng bộ nhớ khác so với các yêu cầu cấp phát nhỏ. Tuy nhiên, chi tiết này là một sự phức tạp không mong muốn: Các vấn đề về độ phân mảnh và cấp phát bộ nhớ hiệu quả vẫn còn, vì vậy chúng tôi sẽ bỏ qua sự tinh tế trong việc thực hiện ở đây và sẽ viết như thể heap là một vùng duy nhất.

Nếu chúng ta viết một chương trình đa luồng (sẽ nói thêm về điều này sau), chúng ta sẽ cần nhiều stack (một cho mỗi luồng) nhưng chỉ có một heap.

Trên các kiến trúc thông thường, heap là một phần của `Data segment` và bắt đầu ngay phía trên mã và biến toàn cục. 

## Các chương trình cần gọi brk hoặc sbrk không?
Không thường xuyên (mặc dù gọi `sbrk(0)` có thể thú vị vì nó cho bạn biết heap của bạn hiện đang kết thúc ở đâu). Thay vào đó, các chương trình sử dụng `malloc, calloc, realloc` và `free` là một phần của thư viện C. Các hàm thực hiện nội bộ của những hàm này sẽ gọi `sbrk` khi cần thêm bộ nhớ heap.
```C
void *top_of_heap = sbrk(0);
malloc(16384);
void *top_of_heap2 = sbrk(0);
printf("The top of heap went from %p to %p \n", top_of_heap, top_of_heap2);
```
Ví dụ về đầu ra: `The top of heap went from 0x4000 to 0xa000`

## Calloc là gì?
Khác với `malloc`, `calloc` khởi tạo nội dung bộ nhớ thành zero và cũng nhận hai đối số (số lượng mục và kích thước bằng byte của mỗi mục). Một cách thực hiện `calloc` dễ hiểu nhưng hơi ngây thơ như sau:

```C
void *calloc(size_t n, size_t size)
{
    size_t total = n * size; // Không kiểm tra tràn số!
    void *result = malloc(total);
    
    if (!result) return NULL;
    
// Nếu chúng ta đang sử dụng các trang bộ nhớ mới
// vừa được cấp phát từ hệ thống bằng cách gọi sbrk
// thì chúng sẽ là zero nên việc đặt lại thành zero là không cần thiết,

    memset(result, 0, total);
    return result; 
}
```
Một cuộc thảo luận nâng cao về những hạn chế này có thể xem [ở đây](http://locklessinc.com/articles/calloc/).

Các lập trình viên thường sử dụng `calloc` thay vì gọi `memset` sau `malloc`, để đặt nội dung bộ nhớ thành zero. Lưu ý `calloc(x,y)` giống hệt với `calloc(y,x)`, nhưng bạn nên tuân theo quy ước của hướng dẫn sử dụng.

```C
// Đảm bảo rằng bộ nhớ của chúng ta được khởi tạo thành zero
link_t *link  = malloc(256);
memset(link, 0, 256); // Giả định rằng malloc trả về một địa chỉ hợp lệ!

link_t *link = calloc(1, 256); // an toàn hơn: calloc(1, sizeof(link_t));
```
## Tại sao bộ nhớ đầu tiên được trả về bởi sbrk được khởi tạo thành zero?
Nếu hệ điều hành không đặt nội dung của RAM vật lý thành zero, có thể một quá trình sẽ biết về bộ nhớ của một quá trình khác đã sử dụng bộ nhớ trước đó. Điều này sẽ là một lỗ hổng bảo mật.

Rất tiếc, điều này có nghĩa là cho các yêu cầu `malloc` trước khi bất kỳ bộ nhớ nào đã được giải phóng và các chương trình đơn giản (sử dụng bộ nhớ mới được dành riêng từ hệ thống) thì bộ nhớ _thường_ là zero. Sau đó, các lập trình viên viết các chương trình C giả định rằng bộ nhớ malloc'd sẽ _luôn_ là zero.

```C
char* ptr = malloc(300);
// nội dung có lẽ là zero vì chúng ta nhận được bộ nhớ hoàn toàn mới
// vì vậy các chương trình cho người mới học có vẻ hoạt động!
// strcpy(ptr, "Some data"); // làm việc với dữ liệu
free(ptr);
// sau
char *ptr2 = malloc(308); // Nội dung có thể bây giờ chứa dữ liệu hiện có và có lẽ không phải là zero
```

## Tại sao malloc không luôn khởi tạo bộ nhớ thành zero?
Hiệu suất! Chúng ta muốn malloc nhanh nhất có thể. Việc đặt bộ nhớ thành zero có thể không cần thiết.

## Realloc là gì và khi nào bạn sử dụng nó?
`realloc` cho phép bạn thay đổi kích thước của một cấp phát bộ nhớ hiện có đã được cấp phát trước đó trên heap (qua malloc, calloc hoặc realloc). Việc sử dụng phổ biến nhất của realloc là thay đổi kích thước bộ nhớ được sử dụng để chứa một mảng giá trị. Một phiên bản realloc dễ hiểu nhưng hơi ngây thơ được đề xuất dưới đây
```C
void * realloc(void * ptr, size_t newsize) {
    // Cách thực hiện đơn giản luôn đặt thêm bộ nhớ
    // và không có kiểm tra lỗi
    void *result = malloc(newsize); 
    size_t oldsize =  ... //(phụ thuộc vào cấu trúc dữ liệu nội bộ của bộ cấp phát)
    if (ptr) memcpy(result, ptr, newsize < oldsize ? newsize : oldsize);
    free(ptr);
    return result;
}
```
Một cách sử dụng SAI của realloc được hiển thị dưới đây:
```C
int *array = malloc(sizeof(int) * 2);
array[0] = 10; array[1] = 20;
// Ooops cần một mảng lớn hơn - vì vậy sử dụng realloc..
realloc (array, 3); // LỖI!
array[2] = 30; 
```

Đoạn mã trên chứa hai lỗi. Đầu tiên, chúng ta cần 3*sizeof(int) byte chứ không phải 3 byte.
Thứ hai, realloc có thể cần di chuyển nội dung hiện có của bộ nhớ đến một vị trí mới. Ví dụ, có thể không có đủ không gian vì các byte lân cận đã được cấp phát. Một cách sử dụng đúng của realloc được hiển thị dưới đây.
```C
array = realloc(array, 3 * sizeof(int));
// Nếu mảng được sao chép đến một vị trí mới thì cấp phát cũ sẽ được giải phóng.
```
Một phiên bản mạnh mẽ hơn cũng sẽ kiểm tra giá trị trả về `NULL`. Lưu ý `realloc` có thể được sử dụng để tăng và giảm cấp phát.

## Tôi có thể đọc thêm ở đâu?
Xem [trang hướng dẫn](http://man7.org/linux/man-pages/man3/malloc.3.html)!

## Việc cấp phát bộ nhớ nhanh quan trọng như thế nào?
Rất quan trọng! Việc cấp phát và giải phóng bộ nhớ heap là một hoạt động phổ biến trong hầu hết các ứng dụng.

# Giới thiệu về việc Cấp phát

## Cách thực hiện malloc và free ngớ ngẩn nhất là gì và điều gì không đúng với nó?

```C
void* malloc(size_t size)
// Hỏi hệ thống thêm nhiều byte bằng cách mở rộng không gian heap.
// sbrk trả về -1 khi thất bại
     void *p = sbrk(size); 
     if(p == (void *) -1) return NULL; // Không còn không gian
     return p;
}
void free() {/* Không làm gì */}
```
Cách thực hiện trên gặp hai hạn chế lớn:
* Các lệnh hệ thống chậm (so với các lệnh thư viện). Chúng ta nên dành một lượng lớn bộ nhớ và chỉ đôi khi yêu cầu thêm từ hệ thống.
* Không tái sử dụng bộ nhớ đã giải phóng. Chương trình của chúng ta không bao giờ tái sử dụng bộ nhớ heap - nó chỉ tiếp tục yêu cầu một heap lớn hơn.

Nếu trình cấp phát này được sử dụng trong một chương trình thông thường, quá trình sẽ nhanh chóng tiêu hao hết tất cả bộ nhớ có sẵn.
Thay vào đó, chúng ta cần một trình cấp phát có thể sử dụng hiệu quả không gian heap và chỉ yêu cầu thêm bộ nhớ khi cần thiết.

## Các chiến lược đặt là gì?
Trong quá trình thực hiện chương trình, bộ nhớ được cấp phát và giải phóng (giải phóng), vì vậy sẽ có các khoảng trống (lỗ) trong bộ nhớ heap có thể được tái sử dụng cho các yêu cầu bộ nhớ sau này. Trình cấp phát bộ nhớ cần theo dõi các phần nào của heap hiện đang được cấp phát và các phần nào đang có sẵn.

Giả sử kích thước heap hiện tại của chúng ta là 64K, mặc dù không phải tất cả đều đang được sử dụng vì một số bộ nhớ đã được cấp phát trước đó đã được chương trình giải phóng:

16KB trống | 10KB đã cấp phát | 1KB trống | 1KB đã cấp phát | 30KB trống | 4KB đã cấp phát | 2KB trống 
---|---|---|---|---|---|---

Nếu một yêu cầu malloc mới cho 2KB được thực hiện (`malloc(2048)`), `malloc` nên dành bộ nhớ ở đâu? Nó có thể sử dụng lỗ 2KB cuối cùng (ngẫu nhiên có kích thước hoàn hảo!) hoặc nó có thể chia một trong hai lỗ trống khác. Những lựa chọn này đại diện cho các chiến lược đặt khác nhau.

Bất kể lỗ nào được chọn, trình cấp phát sẽ cần chia lỗ thành hai: Không gian mới được cấp phát (sẽ được trả về cho chương trình) và một lỗ nhỏ hơn (nếu còn dư không gian).

Chiến lược phù hợp hoàn hảo tìm lỗ nhỏ nhất có kích thước đủ lớn (ít nhất 2KB):

16KB trống | 10KB đã cấp phát | 1KB trống | 1KB đã cấp phát | 30KB trống | 4KB đã cấp phát | `2KB Ở ĐÂY!`
---|---|---|---|---|---|---

Chiến lược phù hợp tồi nhất tìm lỗ lớn nhất có kích thước đủ lớn (vì vậy chia lỗ 30KB thành hai):

16KB trống | 10KB đã cấp phát | 1KB trống | 1KB đã cấp phát | `2KB Ở ĐÂY!` | `28KB trống` | 4KB đã cấp phát | 2KB trống 
---|---|---|---|---|---|---|---

Chiến lược phù hợp đầu tiên tìm lỗ đầu tiên có sẵn có kích thước đủ lớn (chia lỗ 16KB thành hai):

`2KB Ở ĐÂY!` | `14KB trống` | 10KB đã cấp phát | 1KB trống | 1KB đã cấp phát | 30KB trống | 4KB đã cấp phát | 2KB trống 
---|---|---|---|---|---|---|---

## Hiện tượng mảnh vỡ bên ngoài là gì?
Trong ví dụ dưới đây, trong tổng số 64KB bộ nhớ heap, 17KB được cấp phát, và 47KB còn trống. Tuy nhiên, khối lớn nhất có sẵn chỉ là 30KB vì bộ nhớ heap chưa được cấp phát của chúng ta bị mảnh vỡ thành các mảnh nhỏ hơn.

 `16KB trống` | 10KB đã cấp phát | `1KB trống` | 1KB đã cấp phát | `30KB trống` | 4KB đã cấp phát | 2KB đã cấp phát 
---|---|---|---|---|---|---

## Chiến lược đặt có ảnh hưởng như thế nào đến hiện tượng mảnh vỡ bên ngoài và hiệu suất?
Các chiến lược khác nhau ảnh hưởng đến việc mảnh vỡ bộ nhớ heap theo những cách không rõ ràng, chỉ được phát hiện thông qua phân tích toán học hoặc mô phỏng cẩn thận trong điều kiện thực tế (ví dụ: mô phỏng các yêu cầu cấp phát bộ nhớ của cơ sở dữ liệu hoặc máy chủ web).
Ví dụ, best-fit (phù hợp tốt nhất) dường như là một chiến lược xuất sắc, tuy nhiên, nếu chúng ta không thể tìm thấy một lỗ có kích thước hoàn hảo thì việc đặt này tạo ra nhiều lỗ nhỏ không thể sử dụng, dẫn đến mảnh vỡ cao. Nó cũng yêu cầu quét tất cả các lỗ có thể.

First fit (phù hợp đầu tiên) có lợi thế là nó sẽ không đánh giá tất cả các vị trí có thể và do đó sẽ nhanh hơn.

Vì Worst-fit (phù hợp tồi nhất) nhắm vào không gian chưa được cấp phát lớn nhất, nó là một lựa chọn kém nếu cần cấp phát lớn.

Trên thực tế, first-fit và next-fit (không được thảo luận ở đây) thường là chiến lược đặt phổ biến. Các phương pháp kết hợp và nhiều lựa chọn khác tồn tại (xem trang triển khai trình cấp phát bộ nhớ).

## Những thách thức khi viết trình cấp phát heap là gì?
Những thách thức chính là,
* Cần giảm thiểu mảnh vỡ (tức là tối đa hóa sử dụng bộ nhớ)
* Cần hiệu suất cao
* Triển khai tinh vi (nhiều thao tác con trỏ sử dụng danh sách liên kết và phép toán con trỏ)

Một số nhận xét bổ sung:

Cả mảnh vỡ và hiệu suất phụ thuộc vào hồ sơ cấp phát của ứng dụng, có thể được đánh giá nhưng không thể dự đoán và trên thực tế, dưới điều kiện sử dụng cụ thể, trình cấp phát đặc biệt thường vượt trội hơn so với triển khai mục đích chung.

Trình cấp phát không biết các yêu cầu cấp phát bộ nhớ của chương trình trước. Ngay cả khi chúng ta biết, đây là vấn đề [Knapsack](http://en.wikipedia.org/wiki/Knapsack_problem) được biết đến là NP khó!


## Làm thế nào để triển khai một trình cấp phát bộ nhớ?
Câu hỏi tốt. Xem phần tiếp theo để biết cách triển khai.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator">
Tiếp theo: Bộ nhớ, Phần 2: Triển khai một trình cấp phát bộ nhớ
</a>
</div>