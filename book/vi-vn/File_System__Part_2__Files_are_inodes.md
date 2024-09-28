Ý tưởng lớn: Quên tên tệp: 'inode' chính là tệp.  

Người ta thường nghĩ rằng tên tệp là tệp 'thực tế'. Không phải vậy! Thay vào đó, hãy coi **inode** là tệp. Inode chứa siêu thông tin (lần truy cập cuối cùng, quyền sở hữu, kích thước) và trỏ đến các khối đĩa được sử dụng để chứa nội dung tệp.

## Vậy ... Làm cách nào để triển khai một thư mục?

Thư mục chỉ là một ánh xạ tên tới số inode.
POSIX cung cấp một tập hợp nhỏ các hàm để đọc tên tệp và số inode cho mỗi mục nhập (xem bên dưới).

Hãy nghĩ về nó trông như thế nào trong hệ thống tệp thực tế. Về mặt lý thuyết, thư mục cũng giống như các tệp thực tế. Các khối đĩa sẽ chứa _các mục nhập thư mục_ hoặc _dirents_. Điều đó có nghĩa là khối đĩa của chúng ta có thể trông như thế này:

| inode_num | name |
|-----------|------|
| 2043567   | hi.txt |
...

Mỗi mục nhập thư mục có thể có kích thước cố định hoặc là một chuỗi C có độ dài thay đổi. Nó phụ thuộc vào cách hệ thống tệp cụ thể triển khai nó ở cấp độ thấp hơn.

## Làm cách nào để tìm số inode của một tệp?
Từ shell, hãy sử dụng `ls` với tùy chọn `-i`:

```
$ ls -i
12983989 dirlist.c		12984068 sandwich.c
```

Từ C, hãy gọi một trong các hàm `stat` (được giới thiệu bên dưới).

## Làm cách nào để tìm hiểu siêu thông tin về một tệp (hoặc thư mục)?

Sử dụng các lệnh gọi `stat`. Ví dụ: để tìm ra thời điểm tệp 'notes.txt' của tôi được truy cập lần cuối:

```c
struct stat s;
stat("notes.txt", &s);
printf("Lần truy cập cuối cùng %s", ctime(&s.st_atime));
```
Thực tế có ba phiên bản của `stat`;

```c
int stat(const char *path, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *path, struct stat *buf);
```

Ví dụ: bạn có thể sử dụng `fstat` để tìm hiểu siêu thông tin về một tệp nếu bạn đã có một bộ mô tả tệp được liên kết với tệp đó:

```c
FILE *file = fopen("notes.txt", "r");
int fd = fileno(file); /* Chỉ để cho vui - trích xuất bộ mô tả tệp từ struct FILE của C */
struct stat s;
fstat(fd, & s);
printf("Lần truy cập cuối cùng %s", ctime(&s.st_atime));
```

Chúng ta sẽ thảo luận về lệnh gọi thứ ba 'lstat' khi giới thiệu liên kết tượng trưng.

Ngoài thời gian truy cập, tạo và sửa đổi, cấu trúc `stat` còn bao gồm số inode, độ dài của tệp và thông tin chủ sở hữu.

```c
struct stat {
    dev_t     st_dev;     /* ID của thiết bị chứa tệp */
    ino_t     st_ino;     /* số inode */
    mode_t    st_mode;    /* quyền */
    nlink_t   st_nlink;   /* số lượng liên kết cứng */
    uid_t     st_uid;     /* ID người dùng của chủ sở hữu */
    gid_t     st_gid;     /* ID nhóm của chủ sở hữu */
    dev_t     st_rdev;    /* ID thiết bị (nếu là tệp đặc biệt) */
    off_t     st_size;    /* tổng kích thước, tính bằng byte */
    blksize_t st_blksize; /* kích thước khối cho I/O hệ thống tệp */
    blkcnt_t  st_blocks;  /* số lượng khối 512B được phân bổ */
    time_t    st_atime;   /* thời gian truy cập lần cuối */
    time_t    st_mtime;   /* thời gian sửa đổi lần cuối */
    time_t    st_ctime;   /* thời gian thay đổi trạng thái lần cuối */
};
```

## Làm cách nào để liệt kê nội dung của một thư mục?

Hãy viết phiên bản 'ls' của riêng chúng ta để liệt kê nội dung của một thư mục.

```c
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    if(argc == 1) {
        printf("Cách sử dụng: %s [thư mục]\n", *argv);
        exit(0);
    }
    struct dirent *dp;
    DIR *dirp = opendir(argv[1]);
    while ((dp = readdir(dirp)) != NULL) {
        puts(dp->d_name);
    }

    closedir(dirp);
    return 0;
}
```

## Làm cách nào để đọc nội dung của một thư mục?

Trả lời: Sử dụng `opendir`, `readdir`, `closedir`.  
Ví dụ: đây là một triển khai rất đơn giản của `ls` để liệt kê nội dung của một thư mục.

```c
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    if(argc ==1) {
        printf("Cách sử dụng: %s [thư mục]\n", *argv);
        exit(0);
    }
    struct dirent *dp;
    DIR *dirp = opendir(argv[1]);
    while ((dp = readdir(dirp)) != NULL) {
        printf("%s %lu\n", dp-> d_name, (unsigned long)dp-> d_ino );
    }

    closedir(dirp);
    return 0;
}
```

Lưu ý: sau khi gọi `fork()`, tiến trình cha hoặc tiến trình con (XOR) có thể sử dụng `readdir()`, `rewinddir()` hoặc `seekdir()`. Nếu cả tiến trình cha và tiến trình con đều sử dụng các hàm trên, hành vi sẽ không được xác định.

## Làm cách nào để kiểm tra xem một tệp có trong thư mục hiện tại hay không?
Ví dụ: để xem một thư mục cụ thể có bao gồm tệp (hoặc tên tệp) 'name' hay không, chúng ta có thể viết đoạn mã sau. (Gợi ý: Bạn có thể phát hiện ra lỗi không?)

```c
int exists(char *directory, char *name)  {
    struct dirent *dp;
    DIR *dirp = opendir(directory);
    while ((dp = readdir(dirp)) != NULL) {
        puts(dp->d_name);
        if (!strcmp(dp->d_name, name)) {
 	    return 1; /* Đã tìm thấy */
        }
    }
    closedir(dirp);
    return 0; /* Không tìm thấy */
}
```

Đoạn mã trên có một lỗi tinh tế: Nó bị rò rỉ tài nguyên! Nếu tìm thấy tên tệp phù hợp thì `closedir` không bao giờ được gọi như một phần của lệnh `return` sớm. Bất kỳ bộ mô tả tệp nào được mở và bất kỳ bộ nhớ nào được phân bổ bởi `opendir` đều không bao giờ được giải phóng. Điều này có nghĩa là cuối cùng tiến trình sẽ hết tài nguyên và lệnh gọi `open` hoặc `opendir` sẽ thất bại.

Cách khắc phục là đảm bảo chúng ta giải phóng tài nguyên trong mọi đường dẫn mã có thể. Trong mã trên, điều này có nghĩa là gọi `closedir` trước `return 1`. Việc quên giải phóng tài nguyên là một lỗi lập trình C phổ biến vì không có hỗ trợ trong ngôn ngữ C để đảm bảo tài nguyên luôn được giải phóng với tất cả các đường dẫn mã.

## Những điểm cần lưu ý khi sử dụng `readdir` là gì? Ví dụ: để tìm kiếm đệ quy các thư mục?
Có hai điểm cần lưu ý chính và một lưu ý:
Hàm `readdir` trả về "." (thư mục hiện tại) và ".." (thư mục cha). Nếu bạn đang tìm kiếm các thư mục con, bạn cần loại trừ rõ ràng các thư mục này.

Đối với nhiều ứng dụng, việc kiểm tra thư mục hiện tại trước khi tìm kiếm đệ quy các thư mục con là hợp lý. Điều này có thể đạt được bằng cách lưu trữ kết quả trong danh sách liên kết hoặc đặt lại cấu trúc thư mục để khởi động lại từ đầu.

Một lưu ý cuối cùng: `readdir` không an toàn cho luồng! Đối với tìm kiếm đa luồng, hãy sử dụng `readdir_r` yêu cầu người gọi chuyển vào địa chỉ của struct `dirent` hiện có.

Xem trang hướng dẫn sử dụng của `readdir` để biết thêm chi tiết.

## Làm cách nào để xác định xem một mục nhập thư mục có phải là thư mục hay không?
Trả lời: Sử dụng `S_ISDIR` để kiểm tra các bit chế độ được lưu trữ trong cấu trúc `stat`.

Và để kiểm tra xem một tệp có phải là tệp thông thường hay không, hãy sử dụng `S_ISREG`,

```c
struct stat s;
if (0 == stat(name, &s)) {
    printf("%s ", name);
    if (S_ISDIR( s.st_mode)) puts("là một thư mục");
    if (S_ISREG( s.st_mode)) puts("là một tệp thông thường");
} else {
    perror("stat failed - bạn có chắc là tôi có thể đọc siêu dữ liệu của tệp này không?");
}
```
## Thư mục có inode không?
Có! Mặc dù cách tốt hơn để nghĩ về điều này là một thư mục (như một tệp) _là_ một inode (với một số dữ liệu - tên thư mục và nội dung inode). Nó chỉ là một loại inode đặc biệt.

Từ [Wikipedia](http://en.wikipedia.org/wiki/Inode):
> Thư mục Unix là danh sách các cấu trúc liên kết, mỗi cấu trúc chứa một tên tệp và một số inode.

Hãy nhớ rằng, inode không chứa tên tệp--chỉ chứa siêu dữ liệu tệp khác.

## Làm cách nào để cùng một tệp xuất hiện ở hai vị trí khác nhau trong hệ thống tệp của tôi?
Đầu tiên, hãy nhớ rằng tên tệp != tệp. Hãy coi inode là 'tệp' và thư mục chỉ là danh sách các tên với mỗi tên được ánh xạ tới một số inode. Một số inode đó có thể là inode tệp thông thường, một số khác có thể là inode thư mục.

Nếu chúng ta đã có một tệp trên hệ thống tệp, chúng ta có thể tạo một liên kết khác đến cùng một inode bằng lệnh 'ln':

```
$ ln file1.txt blip.txt
```
Tuy nhiên, `blip.txt` _là_ cùng một tệp; nếu tôi chỉnh sửa `blip`, tôi đang chỉnh sửa cùng một tệp với 'file1.txt'!
Chúng ta có thể chứng minh điều này bằng cách chỉ ra rằng cả hai tên tệp đều tham chiếu đến cùng một inode:
```
$ ls -i file1.txt blip.txt
134235 file1.txt
134235 blip.txt
```

Những loại liên kết này (hay còn gọi là mục nhập thư mục) được gọi là 'liên kết cứng'.

Lệnh gọi C tương đương là `link`:
```C
link(const char *path1, const char *path2);

link("file1.txt", "blip.txt");
```

Để đơn giản, các ví dụ trên đã tạo các liên kết cứng bên trong cùng một thư mục, tuy nhiên các liên kết cứng có thể được tạo ở bất kỳ đâu bên trong cùng một hệ thống tệp.

## Điều gì xảy ra khi tôi `rm` (xóa) một tệp?
Khi bạn xóa một tệp (sử dụng `rm` hoặc `unlink`), bạn đang xóa một tham chiếu inode khỏi một thư mục.
Tuy nhiên, inode vẫn có thể được tham chiếu từ các thư mục khác. Để xác định xem nội dung của tệp có còn cần thiết hay không, mỗi inode giữ một số tham chiếu được cập nhật bất cứ khi nào một liên kết mới được tạo hoặc bị hủy.

## Nghiên cứu trường hợp: Phần mềm sao lưu giảm thiểu trùng lặp tệp
Một ví dụ sử dụng liên kết cứng là tạo hiệu quả nhiều bản lưu trữ của một hệ thống tệp tại các thời điểm khác nhau. Khi khu vực lưu trữ có bản sao của một tệp cụ thể, thì các bản lưu trữ trong tương lai có thể sử dụng lại các tệp lưu trữ này thay vì tạo một tệp trùng lặp. Phần mềm "Time Machine" của Apple làm điều này.

## Tôi có thể tạo liên kết cứng đến thư mục cũng như tệp thông thường không?
Không. Chà có. Không hẳn ... Thực ra bạn không thực sự muốn làm điều này, phải không?
Tiêu chuẩn POSIX nói rằng bạn không được phép! Lệnh `ln` sẽ chỉ cho phép root thực hiện việc này và chỉ khi bạn cung cấp tùy chọn `-d`. Tuy nhiên, ngay cả root cũng có thể không thể thực hiện việc này vì hầu hết các hệ thống tệp đều ngăn chặn điều đó! 

## Tại sao POSIX ngăn chặn các liên kết cứng đến thư mục?

Tính toàn vẹn của hệ thống tệp giả định rằng cấu trúc thư mục (không bao gồm các liên kết mềm mà chúng ta sẽ nói sau) là một cây không tuần hoàn có thể truy cập được từ thư mục gốc. Việc thực thi hoặc xác minh ràng buộc này sẽ trở nên tốn kém nếu cho phép liên kết thư mục. Việc phá vỡ những giả định này có thể khiến các công cụ toàn vẹn tệp không thể sửa chữa hệ thống tệp. Tìm kiếm đệ quy có khả năng không bao giờ kết thúc và các thư mục có thể có nhiều hơn một cha mẹ nhưng ".." chỉ có thể tham chiếu đến một cha mẹ duy nhất. Nói chung, đó là một ý tưởng tồi.

---
<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-1%3A-Introduction">
Trở lại: Hệ thống tệp, Phần 1
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/File-System,-Part-3:-Permissions">
Tiếp theo: Hệ thống tệp, Phần 3
</a>
</div>
