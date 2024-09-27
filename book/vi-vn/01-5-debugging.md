---
title: "Chương 01: Học Ngữ Lập Trình C, Phần 5: Gỡ lỗi"
type: book-vi-vn
order: 5
---

# Hướng dẫn Hitchhiker về Gỡ lỗi Chương trình C

Đây sẽ là một hướng dẫn toàn diện để giúp bạn gỡ lỗi chương trình C. Có nhiều cấp độ khác nhau để kiểm tra lỗi và chúng tôi sẽ đi qua hầu hết chúng. Hãy thoải mái bổ sung bất cứ điều gì bạn thấy hữu ích trong việc gỡ lỗi chương trình C, bao gồm nhưng không giới hạn ở việc sử dụng trình gỡ lỗi, nhận biết các loại lỗi phổ biến, các lỗi thường gặp và mẹo tìm kiếm Google hiệu quả.

# Gỡ lỗi Trong Mã Nguồn

## Mã nguồn sạch

Hãy viết mã nguồn theo phong cách **modular** bằng cách sử dụng các **hàm hỗ trợ** (helper function). Nếu có một tác vụ lặp lại (ví dụ: lấy con trỏ đến các khối liền kề trong bài tập malloc), hãy tạo chúng thành các hàm hỗ trợ. Và đảm bảo rằng mỗi hàm thực hiện **một** nhiệm vụ cụ thể một cách chính xác, để bạn không phải gỡ lỗi hai lần.

Giả sử chúng ta đang thực hiện thuật toán sắp xếp chọn bằng cách tìm phần tử nhỏ nhất trong mỗi lần lặp như sau:

```C
void selection_sort(int *a, long len){
     for(long i = len-1; i > 0; --i){
         long max_index = i;
         for(long j = len-1; j >= 0; --j){
             if(a[max_index] < a[j]){
                  max_index = j;
             }
         }
         int temp = a[i];
         a[i] = a[max_index];
         a[max_index] = temp;
     }

}
```

Nhiều người có thể nhìn thấy lỗi trong đoạn mã trên, nhưng việc tái cấu trúc nó thành các hàm sau đây sẽ giúp ích cho việc gỡ lỗi:

```C
long max_index(int *a, long start, long end);
void swap(int *a, long idx1, long idx2);
void selection_sort(int *a, long len);
```

Bằng cách này, lỗi sẽ được khoanh vùng trong một hàm cụ thể.

Mặc dù khóa học này không tập trung vào việc tái cấu trúc/gỡ lỗi mã nguồn, và thực tế là hầu hết mã hệ thống đều rất khó đọc, nhưng việc áp dụng một số kỹ thuật lập trình tốt sẽ giúp bạn tiết kiệm rất nhiều thời gian và công sức trong quá trình gỡ lỗi.

## Khẳng định (Assertion)

Sử dụng **khẳng định** (`assert`) để đảm bảo rằng mã nguồn của bạn hoạt động đúng đến một điểm nhất định - và quan trọng hơn là để đảm bảo rằng bạn không vô tình làm hỏng nó sau này. 

Ví dụ: nếu cấu trúc dữ liệu của bạn là danh sách liên kết đôi, bạn có thể sử dụng `assert(node->size == node->next->prev->size)` để khẳng định rằng nút tiếp theo có một con trỏ trỏ đến nút hiện tại. Bạn cũng có thể kiểm tra xem con trỏ có trỏ đến một phạm vi địa chỉ bộ nhớ hợp lệ, khác `NULL`, `->size` có giá trị hợp lý, v.v.

Macro `NDEBUG` sẽ vô hiệu hóa tất cả các khẳng định, vì vậy đừng quên định nghĩa nó sau khi bạn hoàn tất việc gỡ lỗi. [http://www.cplusplus.com/reference/cassert/assert/](http://www.cplusplus.com/reference/cassert/assert/)

Dưới đây là một ví dụ nhanh với `assert`. Giả sử bạn đang viết mã nguồn sử dụng `memcpy`:

```C
assert(!(src < dest + n && dest < src + n)); //Kiểm tra vùng nhớ chồng chéo
memcpy(dest, src, n);
```

Kiểm tra này có thể được tắt khi biên dịch, nhưng sẽ giúp bạn tiết kiệm **rất nhiều** thời gian gỡ lỗi!

## In ra màn hình (`printf`)

Khi mọi cách khác đều thất bại, hãy in ra dữ liệu như điên! Mỗi hàm của bạn nên có một ý tưởng rõ ràng về những gì nó sẽ làm (ví dụ: hàm `find_min` phải tìm ra phần tử nhỏ nhất). Bạn muốn kiểm tra xem mỗi hàm có đang làm những gì nó được thiết kế để làm hay không, và xác định chính xác vị trí mã nguồn bị lỗi. Trong trường hợp xảy ra **điều kiện đua** (race condition), `tsan` có thể giúp ích, nhưng việc mỗi luồng in ra dữ liệu tại một số thời điểm nhất định cũng có thể giúp bạn xác định điều kiện đua.

# Valgrind

Valgrind là một bộ công cụ được thiết kế để cung cấp các công cụ gỡ lỗi và profiling để làm cho chương trình của bạn chính xác hơn và phát hiện một số vấn đề trong thời gian chạy. Công cụ được sử dụng nhiều nhất trong số này là Memcheck, có thể phát hiện nhiều lỗi liên quan đến bộ nhớ phổ biến trong các chương trình C và C++ và có thể dẫn đến sự cố và hành vi không thể đoán trước (ví dụ: bộ đệm bộ nhớ không được giải phóng).

Để chạy Valgrind trên chương trình của bạn:

```bash
valgrind --leak-check=yes myprogram arg1 arg2
```

hoặc

```bash
valgrind ./myprogram
```

Các đối số là tùy chọn và công cụ mặc định sẽ chạy là Memcheck. Kết quả sẽ được trình bày dưới dạng số lượng cấp phát, số lượng cấp phát đã được giải phóng và số lượng lỗi.

**Ví dụ**

![Ví dụ Valgrind](https://i.imgur.com/ZdBWDvh.png)

Dưới đây là một ví dụ để giúp bạn hiểu kết quả trên. Giả sử chúng ta có một chương trình đơn giản như sau:

```C
#include <stdlib.h>

void dummy_function()
{
    int* x = malloc(10 * sizeof(int));
    x[10] = 0;        // lỗi 1: ghi vào một địa chỉ bộ nhớ nằm ngoài phạm vi
}                    // lỗi 2: rò rỉ bộ nhớ, x được cấp phát không được giải phóng

int main(void)
{
    dummy_function();
    return 0;
}
```

Hãy xem Valgrind sẽ xuất ra gì (chương trình này biên dịch và chạy không có lỗi):

```
==29515== Memcheck, a memory error detector
==29515== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==29515== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==29515== Command: ./a
==29515== 
==29515== Invalid write of size 4
==29515==    at 0x400544: dummy_function (in /home/rafi/projects/exocpp/a)
==29515==    by 0x40055A: main (in /home/rafi/projects/exocpp/a)
==29515==  Address 0x5203068 is 0 bytes after a block of size 40 alloc'd
==29515==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==29515==    by 0x400537: dummy_function (in /home/rafi/projects/exocpp/a)
==29515==    by 0x40055A: main (in /home/rafi/projects/exocpp/a)
==29515== 
==29515== 
==29515== HEAP SUMMARY:
==29515==     in use at exit: 40 bytes in 1 blocks
==29515==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
==29515== 
==29515== LEAK SUMMARY:
==29515==    definitely lost: 40 bytes in 1 blocks
==29515==    indirectly lost: 0 bytes in 0 blocks
==29515==      possibly lost: 0 bytes in 0 blocks
==29515==    still reachable: 0 bytes in 0 blocks
==29515==         suppressed: 0 bytes in 0 blocks
==29515== Rerun with --leak-check=full to see details of leaked memory
==29515== 
==29515== For counts of detected and suppressed errors, rerun with: -v
==29515== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

* **Invalid write**: Valgrind đã phát hiện ra lỗi ghi vượt quá giới hạn khối heap (ghi ra ngoài khối đã được cấp phát).
* **Definitely lost**: Rò rỉ bộ nhớ - bạn có thể đã quên giải phóng một khối bộ nhớ.

Valgrind là một công cụ rất hiệu quả để kiểm tra lỗi trong thời gian chạy. Ngôn ngữ C rất đặc biệt khi nói đến các hành vi như vậy, vì vậy sau khi biên dịch chương trình, bạn có thể sử dụng Valgrind để sửa các lỗi mà trình biên dịch có thể không phát hiện ra, và thường xảy ra khi chương trình của bạn đang chạy.

Để biết thêm thông tin, bạn có thể tham khảo [trang web chính thức](http://valgrind.org/docs/manual/quick-start.html).

# ThreadSanitizer (Tsan)

**ThreadSanitizer** (Tsan) là một công cụ từ Google, được tích hợp vào trình biên dịch `clang` (và `gcc`), để giúp bạn phát hiện các **điều kiện đua** trong mã nguồn. Để biết thêm thông tin về công cụ này, hãy xem wiki trên Github.

Lưu ý rằng chạy chương trình với `tsan` sẽ làm chậm code của bạn một chút.

```C
#include <pthread.h>
#include <stdio.h>

int Global;

void *Thread1(void *x) {
    Global++;
    return NULL;
}

int main() {
    pthread_t t[2];
    pthread_create(&t[0], NULL, Thread1, NULL);
    Global = 100;
    pthread_join(t[0], NULL);
}
// biên dịch với: gcc -fsanitize=thread -pie -fPIC -ltsan -g simple_race.c
```

Chúng ta có thể thấy rằng có một điều kiện đua trên biến `Global`. Cả luồng chính và luồng được tạo bằng `pthread_create` sẽ cố gắng thay đổi giá trị của biến cùng một lúc. Vậy, ThreadSanitizer có phát hiện ra lỗi này không?

```
$ ./a.out
==================
WARNING: ThreadSanitizer: data race (pid=28888)
  Read of size 4 at 0x7f73ed91c078 by thread T1:
    #0 Thread1 /home/zmick2/simple_race.c:7 (exe+0x000000000a50)
    #1  :0 (libtsan.so.0+0x00000001b459)

  Previous write of size 4 at 0x7f73ed91c078 by main thread:
    #0 main /home/zmick2/simple_race.c:14 (exe+0x000000000ac8)

  Thread T1 (tid=28889, running) created by main thread at:
    #0  :0 (libtsan.so.0+0x00000001f6ab)
    #1 main /home/zmick2/simple_race.c:13 (exe+0x000000000ab8)

SUMMARY: ThreadSanitizer: data race /home/zmick2/simple_race.c:7 Thread1
==================
ThreadSanitizer: reported 1 warnings
```

Nếu chúng ta biên dịch với cờ debug (`-g`), Tsan sẽ cung cấp cho chúng ta cả tên biến.

# GDB (GNU Debugger)

Giới thiệu: [http://www.cs.cmu.edu/~gilpin/tutorial/](http://www.cs.cmu.edu/~gilpin/tutorial/)

#### Đặt điểm dừng (breakpoint) trong mã nguồn

Một mẹo rất hữu ích khi gỡ lỗi các chương trình C phức tạp với GDB là đặt điểm dừng trực tiếp trong mã nguồn.

```c
int main() {
    int val = 1;
    val = 42;
    asm("int $3"); // đặt một điểm dừng ở đây
    val = 7;
}
```

```sh
$ gcc main.c -g -o main && ./main
(gdb) r
[...]
Program received signal SIGTRAP, Trace/breakpoint trap.
main () at main.c:6
6	    val = 7;
(gdb) p val
$1 = 42
```

#### Kiểm tra nội dung bộ nhớ

[http://www.delorie.com/gnu/docs/gdb/gdb_56.html](http://www.delorie.com/gnu/docs/gdb/gdb_56.html)

Ví dụ:

```c
int main() {
    char bad_string[3] = {'C', 'a', 't'};
    printf("%s", bad_string);
}
```

```sh
$ gcc main.c -g -o main && ./main
$ Cat ZVQ� $
```

```sh
(gdb) l
1	#include <stdio.h>
2	int main() {
3	    char bad_string[3] = {'C', 'a', 't'};
4	    printf("%s", bad_string);
5	}
(gdb) b 4
Breakpoint 1 at 0x100000f57: file main.c, line 4.
(gdb) r
[...]
Breakpoint 1, main () at main.c:4
4	    printf("%s", bad_string);
(gdb) x/16xb bad_string
0x7fff5fbff9cd:	0x63	0x61	0x74	0xe0	0xf9	0xbf	0x5f	0xff
0x7fff5fbff9d5:	0x7f	0x00	0x00	0xfd	0xb5	0x23	0x89	0xff

(gdb)
```

Ở đây, bằng cách sử dụng lệnh `x` với tham số `16xb`, chúng ta có thể xem 16 byte trong bộ nhớ, bắt đầu từ địa chỉ `0x7fff5fbff9c` (giá trị của `bad_string`), dưới dạng thập lục phân. `printf` sẽ đọc chuỗi byte này cho đến khi gặp byte null (`0x00`). Do đó, kết quả in ra sẽ không như mong muốn vì chuỗi `bad_string` không được kết thúc bằng byte null.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-4:-Strings-and-Structs">
Quay lại: Học Ngữ Lập Trình C, Phần 4: Chuỗi và Cấu trúc
</a>
| <a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Review-Questions">
Tiếp theo: Học Ngữ Lập Trình C, Câu hỏi ôn tập
</a>
</div>


