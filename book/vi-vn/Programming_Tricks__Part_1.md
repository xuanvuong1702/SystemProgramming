## Sử dụng `cat` làm IDE của bạn
Ai cần trình soạn thảo? IDE? Chúng ta chỉ cần sử dụng `cat`!
Bạn đã thấy `cat` được sử dụng để đọc nội dung của các tệp nhưng nó cũng có thể được sử dụng để đọc đầu vào chuẩn và gửi lại tới đầu ra chuẩn.
```
$ cat
HELLO
HELLO
```
Để kết thúc việc đọc từ luồng đầu vào, hãy đóng luồng đầu vào bằng cách nhấn `CTRL-D`

Hãy sử dụng `cat` để gửi đầu vào chuẩn đến một tệp. Chúng ta sẽ sử dụng '>' để chuyển hướng đầu ra của nó đến một tệp:
```
$ cat > myprog.c
#include <stdio.h>
int main() {printf("Hi!");return 0;}
```
(Hãy cẩn thận! Không được phép xóa và hoàn tác...)
Nhấn `CTRL-D` khi hoàn tất.

## Chỉnh sửa mã của bạn bằng biểu thức chính quy `perl` (còn gọi là "hãy nhớ perl pie của bạn")
Một mẹo hữu ích nếu bạn có một số tệp văn bản (ví dụ: mã nguồn) cần thay đổi là sử dụng biểu thức chính quy.
`perl` giúp việc chỉnh sửa các tệp tại chỗ trở nên rất dễ dàng.
Chỉ cần nhớ 'perl pie' và tìm kiếm trên web...

Một ví dụ. Giả sử chúng ta muốn thay đổi chuỗi "Hi" thành "Bye" trong tất cả các tệp .c trong thư mục hiện tại. Sau đó, chúng ta có thể viết một mẫu thay thế đơn giản sẽ được thực thi trên mỗi dòng tại một thời điểm trong tất cả các tệp:
```
$ perl -p -i -e 's/Hi/Bye/' *.c
```
(Đừng hoảng sợ nếu bạn làm sai, các tệp gốc vẫn còn đó; chúng chỉ có phần mở rộng .bak)
Rõ ràng là có nhiều điều bạn có thể làm với biểu thức chính quy hơn là thay đổi Hi thành Bye.

## Sử dụng `!!` trong shell của bạn
Để chạy lại lệnh cuối cùng, chỉ cần gõ `!!` và nhấn `return`
Để chạy lại lệnh cuối cùng bắt đầu bằng g, hãy gõ `!g` và nhấn `return`

## Sử dụng `&&` trong shell của bạn
Bạn đã chán ngấy việc chạy `make` hoặc `gcc` và sau đó chạy chương trình nếu nó được biên dịch OK? Thay vào đó, hãy sử dụng && để xâu chuỗi các lệnh này lại với nhau

```
$ gcc program.c && ./a.out
```
## Make có thể làm nhiều hơn là make
Bạn cũng có thể thử đặt một dòng trong Makefile của mình để biên dịch và sau đó chạy chương trình của bạn.
```
run : $(program)
        ./$(program)
```
Sau đó chạy
```
$ make run
```
sẽ đảm bảo rằng mọi thay đổi bạn đã thực hiện đều được biên dịch và chạy chương trình của bạn cùng một lúc. Cũng tốt cho việc kiểm tra nhiều đầu vào cùng một lúc. Mặc dù bạn có thể muốn viết một tập lệnh shell thông thường cho việc đó.

## Hàng xóm của bạn có quá năng suất không? Bộ tiền xử lý C để giải cứu!
Sử dụng bộ tiền xử lý C để định nghĩa lại các từ khóa phổ biến, ví dụ:
```C
#define if while
```
Mẹo: Đặt dòng này bên trong một trong các include tiêu chuẩn, ví dụ: /usr/include/stdio.h

## Ai cần các hàm khi bạn có bộ tiền xử lý C

Được rồi, đây giống như một cái bẫy hơn. Hãy cẩn thận khi sử dụng macro trông giống như các hàm...
```C
#define min(a,b) a<b?a:b
```
Một định nghĩa hoàn toàn hợp lý về giá trị nhỏ nhất của a và b. Tuy nhiên, bộ tiền xử lý chỉ là một trình xử lý văn bản đơn giản, vì vậy thứ tự ưu tiên có thể gây hại cho bạn:

```C
int value = -min(2,3); // Nên là -2?
```
Được mở rộng thành
```
int value = -2<3 ? 2 :3; // Ôi... kết quả sẽ là 2
```
Một cách khắc phục một phần là bao bọc mọi đối số bằng `()` và cũng bao bọc toàn bộ biểu thức bằng ():
```C
#define min(a,b) (  (a) < (b) ?(a):(b) )
```
Tuy nhiên, đây vẫn _không phải_ là một hàm! Ví dụ: bạn có thể thấy tại sao `min(i++,10)` có thể tăng i một hoặc hai lần không!?


