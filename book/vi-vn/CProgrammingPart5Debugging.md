# Cẩm nang về Gỡ lỗi Chương trình C

Đây sẽ là một hướng dẫn đồ sộ để giúp bạn gỡ lỗi các chương trình C của mình. Có nhiều cấp độ khác nhau mà bạn có thể kiểm tra lỗi và chúng tôi sẽ xem xét hầu hết chúng. Hãy thoải mái thêm bất cứ điều gì bạn thấy hữu ích trong việc gỡ lỗi các chương trình C bao gồm nhưng không giới hạn ở việc sử dụng trình gỡ lỗi, nhận biết các loại lỗi thường gặp, các lỗi phổ biến và các mẹo tìm kiếm hiệu quả trên Google.


# Gỡ lỗi trong mã

## Mã sạch

Làm cho mã của bạn trở nên mô-đun bằng cách sử dụng các hàm hỗ trợ. Nếu có một tác vụ lặp lại (ví dụ: lấy con trỏ đến các khối liền kề trong malloc MP), hãy biến chúng thành các hàm hỗ trợ. Và đảm bảo rằng mỗi hàm thực hiện một việc rất tốt, để bạn không phải gỡ lỗi hai lần.

Giả sử rằng chúng ta đang thực hiện sắp xếp chọn bằng cách tìm phần tử nhỏ nhất trong mỗi lần lặp như sau,

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

Nhiều người có thể thấy lỗi trong mã, nhưng nó có thể giúp ích cho việc tái cấu trúc phương thức trên thành

```C
long max_index(int *a, long start, long end);
void swap(int *a, long idx1, long idx2);
void selection_sort(int *a, long len);
```

Và lỗi cụ thể là ở một hàm.

Cuối cùng, chúng tôi không phải là một lớp học về tái cấu trúc/gỡ lỗi mã của bạn. Trên thực tế, hầu hết các mã hệ thống đều rất tệ đến mức bạn không muốn đọc nó. Nhưng vì lợi ích của việc gỡ lỗi, về lâu dài, bạn có thể áp dụng một số phương pháp hay.

## Khẳng định!

Sử dụng các khẳng định để đảm bảo rằng mã của bạn hoạt động đến một điểm nhất định -- và quan trọng là để đảm bảo rằng bạn không làm hỏng nó sau này. Ví dụ: nếu cấu trúc dữ liệu của bạn là danh sách liên kết kép, bạn có thể làm điều gì đó như `assert(node->size == node->next->prev->size)` để khẳng định rằng nút tiếp theo có một con trỏ đến nút hiện tại. Bạn cũng có thể kiểm tra con trỏ đang trỏ đến một phạm vi địa chỉ bộ nhớ mong đợi, không phải là null, ->size là hợp lý, v.v.
Macro `NDEBUG` sẽ vô hiệu hóa tất cả các khẳng định, vì vậy đừng quên đặt macro đó sau khi bạn hoàn tất việc gỡ lỗi. http://www.cplusplus.com/reference/cassert/assert/

Đây là một ví dụ nhanh với `assert`. Giả sử rằng tôi đang viết mã sử dụng `memcpy`

```C
assert(!(src < dest+n && dest < src+n)); //Kiểm tra chồng lấp
memcpy(dest, src, n);
```

Kiểm tra này có thể bị tắt trong thời gian biên dịch, nhưng sẽ giúp bạn tiết kiệm **rất nhiều** rắc rối khi gỡ lỗi!

## `printf`

Khi mọi cách khác đều thất bại, hãy in ra liên tục! Mỗi hàm của bạn cần có mục tiêu rõ ràng về những gì nó sẽ thực hiện (ví dụ: hàm find_min cần phải tìm ra phần tử nhỏ nhất). Bạn cần kiểm tra xem mỗi hàm có đang thực hiện đúng nhiệm vụ đề ra hay không và xác định chính xác vị trí mã của bạn gặp lỗi. Trong trường hợp xảy ra điều kiện tranh chấp (race condition), tsan có thể hỗ trợ, nhưng việc để mỗi luồng in dữ liệu tại các thời điểm nhất định có thể giúp bạn phát hiện ra điều kiện tranh chấp.

# Valgrind


Valgrind là một bộ công cụ được thiết kế để cung cấp các công cụ gỡ lỗi và cấu hình để làm cho chương trình của bạn chính xác hơn và phát hiện một số vấn đề trong thời gian chạy. Công cụ được sử dụng nhiều nhất trong số này là Memcheck, có thể phát hiện nhiều lỗi liên quan đến bộ nhớ thường gặp trong các chương trình C và C++ và có thể dẫn đến sự cố và hành vi không thể đoán trước (ví dụ: bộ đệm bộ nhớ không được giải phóng).

Để chạy Valgrind trên chương trình của bạn: 

```
valgrind --leak-check=yes myprogram arg1 arg2
```
hoặc 

```
valgrind ./myprogram
```

Các đối số là tùy chọn và công cụ mặc định sẽ chạy là Memcheck. Đầu ra sẽ được trình bày dưới dạng 
số lượng phân bổ, số lượng phân bổ được giải phóng và số lượng lỗi.

**Ví dụ**

![Ví dụ về Valgrind](https://i.imgur.com/ZdBWDvh.png)

Đây là một ví dụ để giúp bạn diễn giải các kết quả ở trên. Giả sử chúng ta có một chương trình đơn giản như thế này: 
```C
  #include <stdlib.h>

  void dummy_function()
  {
     int* x = malloc(10 * sizeof(int));
     x[10] = 0;        // lỗi 1: như bạn có thể thấy ở đây, chúng ta ghi vào một địa chỉ bộ nhớ ngoài giới hạn
  }                    // lỗi 2: rò rỉ bộ nhớ x được phân bổ không được giải phóng

  int main(void)
  {
     dummy_function();
     return 0;
  }
```

Hãy xem Valgrind sẽ xuất ra gì (chương trình này được biên dịch và chạy mà không có lỗi).
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
**Ghi không hợp lệ**: Nó đã phát hiện ra **lỗi tràn bộ đệm heap** của chúng ta (ghi ra bên ngoài khối được phân bổ)

**Chắc chắn bị mất**: Rò rỉ bộ nhớ—có thể bạn đã quên giải phóng một khối bộ nhớ

Valgrind là một công cụ rất hiệu quả để kiểm tra lỗi trong thời gian chạy. C rất đặc biệt khi nói đến hành vi như vậy, vì vậy sau khi biên dịch chương trình của bạn, bạn có thể sử dụng Valgrind để sửa các lỗi mà trình biên dịch của bạn có thể không phát hiện ra và thường xảy ra khi chương trình của bạn đang chạy.

Để biết thêm thông tin, bạn có thể tham khảo [trang web chính thức](http://valgrind.org/docs/manual/quick-start.html).

# Tsan

ThreadSanitizer (TSan) là một công cụ từ Google, được tích hợp trong clang (và gcc), để giúp bạn phát hiện các điều kiện cuộc đua trong mã của mình. Để biết thêm thông tin về công cụ, hãy xem wiki Github.

Lưu ý rằng chạy với TSan sẽ làm chậm mã của bạn một chút.

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
// biên dịch với gcc -fsanitize=thread -pie -fPIC -ltsan -g simple_race.c
```

Chúng ta có thể thấy rằng có một điều kiện cuộc đua trên biến `Global`. Cả luồng chính và luồng được tạo bằng `pthread_create` sẽ cố gắng thay đổi giá trị cùng một lúc. Nhưng, ThreadSanitizer có bắt được nó không?

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

Nếu chúng ta biên dịch với cờ gỡ lỗi, thì nó cũng sẽ cung cấp cho chúng ta tên biến.

# GDB

Giới thiệu: http://www.cs.cmu.edu/~gilpin/tutorial/

#### Đặt điểm dừng theo chương trình

Một thủ thuật rất hữu ích khi gỡ lỗi các chương trình C phức tạp với GDB là đặt các điểm dừng trong mã nguồn.

```c
int main() {
    int val = 1;
    val = 42;
    asm("int $3"); // đặt điểm dừng ở đây
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

http://www.delorie.com/gnu/docs/gdb/gdb_56.html

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

Ở đây, bằng cách sử dụng lệnh `x` với tham số `16xb`, chúng ta có thể thấy rằng bắt đầu từ địa chỉ bộ nhớ `0x7fff5fbff9c` (giá trị của `bad_string`), `printf` sẽ thực sự thấy chuỗi byte sau dưới dạng chuỗi vì chúng ta đã cung cấp một chuỗi bị lỗi mà không có **ký tự kết thúc null**.

```0x63 0x61 0x74 0xe0 0xf9 0xbf 0x5f 0xff 0x7f 0x00```

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-4:-Strings-and-Structs">
Trở lại: Lập trình C, Phần 4: Chuỗi và Cấu trúc
</a>
| <a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Review-Questions">
Tiếp theo: Lập trình C, Câu hỏi Ôn tập
</a>
</div>

