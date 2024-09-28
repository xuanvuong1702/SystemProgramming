## Hệ điều hành tải tiến trình và thư viện của tôi vào bộ nhớ như thế nào?
Nó ánh xạ nội dung của tệp thực thi và tệp thư viện vào không gian địa chỉ của tiến trình.
Nếu nhiều chương trình chỉ cần quyền truy cập đọc vào cùng một tệp (ví dụ: `/bin/bash`, thư viện C), thì cùng một bộ nhớ vật lý có thể được chia sẻ giữa nhiều tiến trình.

Cơ chế tương tự có thể được sử dụng bởi các chương trình để ánh xạ trực tiếp các tệp vào bộ nhớ.

## Làm cách nào để ánh xạ một tệp vào bộ nhớ?
Dưới đây là một chương trình đơn giản để ánh xạ một tệp vào bộ nhớ. Các điểm chính cần lưu ý là:
* `mmap` yêu cầu một bộ mô tả tệp, vì vậy chúng ta cần `open` tệp trước
* `mmap` chỉ hoạt động trên các bộ mô tả tệp có thể `lseek` (tìm kiếm), tức là các tệp "thực", không phải pipe hoặc socket
* Chúng ta tìm kiếm đến kích thước mong muốn và ghi một byte để đảm bảo rằng tệp có đủ độ dài (không làm như vậy sẽ khiến chương trình của bạn nhận được tín hiệu `SIGBUS` khi cố gắng truy cập tệp). Hàm `ftruncate` cũng có thể được sử dụng cho việc này.
* Khi hoàn tất, chúng ta gọi `munmap` để hủy ánh xạ tệp khỏi bộ nhớ.

Ví dụ này cũng cho thấy các hằng số tiền xử lý `__LINE__` và `__FILE__`, chứa số dòng hiện tại và tên tệp của tệp hiện đang được biên dịch.
```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int fail(char *filename, int linenumber) { 
  fprintf(stderr, "%s:%d %s\n", filename, linenumber, strerror(errno)); 
  exit(1);
  return 0; /* Làm cho trình biên dịch hài lòng */
}
#define QUIT fail(__FILE__, __LINE__ )

int main() {
  // Chúng tôi muốn một tệp đủ lớn để chứa 10 số nguyên, 
  // mặc dù ánh xạ thực sự được làm tròn lên bội số của kích thước trang (4kB trên x86 Linux)
  int size = sizeof(int) * 10;

  int fd = open("data", O_RDWR | O_CREAT | O_TRUNC, 0600); // 6 = quyền đọc + ghi cho tôi!

  lseek(fd, size, SEEK_SET);
  write(fd, "A", 1);
  
  void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("Được ánh xạ tại %p\n", addr);
  if (addr == (void*) -1 ) QUIT;
  
  int *array = addr;
  array[0] = 0x12345678;
  array[1] = 0xdeadc0de;

  munmap(addr, size);
  close(fd);
  return 0;
  
}
```
Nội dung của tệp nhị phân của chúng ta có thể được liệt kê bằng cách sử dụng `hexdump`:
```bash
$ hexdump data
0000000 78 56 34 12 de c0 ad de 00 00 00 00 00 00 00 00
0000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
0000020 00 00 00 00 00 00 00 00 41   
```
Người đọc cẩn thận có thể nhận thấy rằng các số nguyên của chúng ta được ghi ở định dạng byte ít quan trọng nhất (vì đó là endianness của CPU) và chúng ta đã phân bổ một tệp nhiều hơn một byte!

Các tùy chọn `PROT_READ | PROT_WRITE` chỉ định quyền bảo vệ bộ nhớ ảo. Tùy chọn `PROT_EXEC` (không được sử dụng ở đây) có thể được đặt để cho phép CPU thực thi các lệnh trong bộ nhớ (ví dụ: điều này sẽ hữu ích nếu bạn ánh xạ một tệp thực thi hoặc thư viện).

## Sự khác biệt giữa `read` + `write` và `mmap`

Trên Linux, có nhiều điểm tương đồng giữa các cách tiếp cận này hơn là sự khác biệt. Trong cả hai trường hợp, tệp thực sự được tải vào bộ đệm đĩa trước khi quá trình đọc hoặc ghi xảy ra và các thao tác ghi được thực hiện trực tiếp trên các trang trong bộ đệm đĩa chứ không phải trên đĩa cứng. Hệ điều hành cuối cùng sẽ đồng bộ hóa bộ đệm đĩa với đĩa; bạn có thể sử dụng `sync` (hoặc các biến thể của nó) để yêu cầu nó làm như vậy ngay lập tức.

Tuy nhiên, `mmap` cho phép tiến trình của bạn truy cập trực tiếp vào bộ nhớ vật lý được lưu trữ trong bộ đệm trang. Nó thực hiện điều này bằng cách ánh xạ các mục nhập bảng trang tiến trình đến cùng các khung trang được tìm thấy trong bộ đệm đĩa. Mặt khác, `read` phải tải dữ liệu từ đĩa vào bộ đệm trang trước, _sau đó sao chép dữ liệu_ từ bộ đệm trang vào bộ đệm không gian người dùng (`write` làm tương tự, nhưng theo hướng ngược lại).

Mặt khác, việc tạo ánh xạ này giữa không gian địa chỉ tiến trình và bộ đệm trang yêu cầu tiến trình phải tạo nhiều bảng trang mới và dẫn đến các lỗi trang bổ sung (nhỏ) khi truy cập lần đầu vào trang tệp.

## Ưu điểm của việc ánh xạ bộ nhớ một tệp là gì

Đối với nhiều ứng dụng, ưu điểm chính là:  
- Mã hóa đơn giản - dữ liệu tệp có sẵn ngay lập tức như thể tất cả đều nằm trong bộ nhớ chính. Không cần phân tích cú pháp dữ liệu đến theo từng khối và lưu trữ nó trong các cấu trúc bộ nhớ mới như bộ đệm.
- Chia sẻ tệp - các tệp được ánh xạ bộ nhớ đặc biệt thuận tiện khi cùng một dữ liệu được chia sẻ giữa nhiều tiến trình.
- Áp lực bộ nhớ thấp hơn - khi tài nguyên eo hẹp, kernel có thể dễ dàng loại bỏ dữ liệu khỏi bộ đệm đĩa ra khỏi bộ nhớ chính và vào bộ nhớ đĩa dự phòng (các trang sạch thậm chí không cần phải được ghi trở lại đĩa). Mặt khác, việc tải các tệp lớn vào bộ đệm cuối cùng sẽ buộc kernel phải lưu trữ các trang đó (ngay cả những trang sạch) trong không gian hoán đổi, có thể bị hết.

Lưu ý: đối với xử lý tuần tự đơn giản, các tệp được ánh xạ bộ nhớ không nhất thiết phải nhanh hơn so với các phương pháp tiếp cận 'dựa trên luồng' tiêu chuẩn của `read`, `fscanf`, v.v., chủ yếu là do các lỗi trang nhỏ và thực tế là tải dữ liệu từ đĩa vào bộ đệm trang là một nút thắt hiệu suất chặt chẽ hơn nhiều so với việc sao chép từ bộ đệm này sang bộ đệm khác.

## Làm cách nào để chia sẻ bộ nhớ giữa tiến trình cha và tiến trình con?

Dễ dàng -  Sử dụng `mmap` mà không cần tệp - chỉ cần chỉ định các tùy chọn `MAP_ANONYMOUS` và `MAP_SHARED`!

```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() được định nghĩa trong tiêu đề này */
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
  
  int size = 100 * sizeof(int);  
  void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  printf("Được ánh xạ tại %p\n", addr);
  
  int *shared = addr;
  pid_t mychild = fork();
  if (mychild > 0) {
    shared[0] = 10;
    shared[1] = 20;
  } else {
    sleep(1); // Chúng ta sẽ nói về đồng bộ hóa sau
    printf("%d\n", shared[1] + shared[0]);
  }

  munmap(addr,size);
  return 0;
}
```

## Tôi có thể sử dụng bộ nhớ dùng chung cho IPC không?

Có! Ví dụ đơn giản, bạn có thể chỉ dành một vài byte và thay đổi giá trị trong bộ nhớ dùng chung khi bạn muốn tiến trình con thoát. Chia sẻ bộ nhớ ẩn danh là một hình thức giao tiếp giữa các tiến trình (IPC) rất hiệu quả vì không có chi phí sao chép, lệnh gọi hệ thống hoặc truy cập đĩa - hai tiến trình chia sẻ theo nghĩa đen cùng một khung _vật lý_ của bộ nhớ chính.

Mặt khác, bộ nhớ dùng chung, giống như đa luồng, tạo ra chỗ cho các cuộc đua dữ liệu. Các tiến trình chia sẻ bộ nhớ có thể ghi có thể cần sử dụng các nguyên thủy đồng bộ hóa như mutex để ngăn chặn điều này xảy ra.

[Chuyển đến Hệ thống tệp: Phần 7](https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-7:-Scalable-and-Reliable-Filesystems)
