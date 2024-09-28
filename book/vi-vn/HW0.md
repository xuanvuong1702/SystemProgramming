# Chào mừng!
```C
// Đầu tiên, bạn có thể đoán lời bài hát nào đã được chuyển thành mã hệ thống giống C này không?
char q[] = "Bạn có muốn xây dựng một chương trình C99 không?";
#define or "hoặc gỡ lỗi với gdb?"
static unsigned int i = sizeof(or) != strlen(or);
char* ptr = "lathe"; size_t come = fprintf(stdout,"%s door", ptr+2);
int away = ! (int) * "";

int* shared = mmap(NULL, sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
munmap(shared,sizeof(int*));

if(!fork()) { execlp("man","man","-3","ftell", (char*)0); perror("failed"); }
if(!fork()) { execlp("make","make", "snowman", (char*)0); execlp("make","make", (char*)0)); }

exit(0);
```

## Bạn có muốn thành thạo Lập trình Hệ thống? Và nhận điểm cao hơn B?
```C
int main(int argc, char** argv) {
	puts("Tuyệt vời! Chúng tôi có rất nhiều tài nguyên hữu ích dành cho bạn, nhưng bạn phải");
	puts(" là một người học tích cực và học cách giải quyết vấn đề và gỡ lỗi mã.");
	puts("Mang câu trả lời gần như đã hoàn thành của bạn cho các vấn đề bên dưới");
	puts(" đến buổi lab đầu tiên để chứng minh rằng bạn đã làm việc về điều này.");
	printf("Một vài \"không biết\" hoặc \"không chắc chắn\" là tốt cho buổi lab 1.\n"); 
	puts("Cảnh báo: bạn và các bạn cùng lớp sẽ phải làm việc chăm chỉ trong lớp học này.");
	puts("Đây không phải là CS225; bạn sẽ bị thúc đẩy nhiều hơn để");
	puts(" tự mình giải quyết mọi việc.");
	fprintf(stdout,"Bài tập về nhà này là bước đệm cho tất cả các bài tập trong tương lai.\n");
	char p[] = "Vì vậy, bạn sẽ muốn làm rõ bất kỳ sự nhầm lẫn hoặc quan niệm sai lầm nào.\n";
	write(1, p, strlen(p) );
	char buffer[1024];
	sprintf(buffer,"Để chấm điểm, bài tập về nhà 0 này sẽ được chấm điểm như một phần bài lab %d của bạn.\n", 1);
	write(1, buffer, strlen(buffer));
	printf("Nhấn Enter để tiếp tục\n");
	read(0, buffer, sizeof(buffer));
	return 0;
}
```
## Xem video và viết câu trả lời của bạn cho các câu hỏi sau

**Quan trọng!**

Máy ảo trong trình duyệt của bạn và video bạn cần cho HW0 ở đây:

http://cs-education.github.io/sys/

Sách wiki của khóa học:

https://github.com/angrave/SystemProgramming/wiki

Câu hỏi? Nhận xét? Sử dụng Piazza:
https://piazza.com/illinois/fall2018/cs241

Máy ảo trong trình duyệt chạy hoàn toàn bằng Javascript và nhanh nhất trong Chrome. Lưu ý rằng VM và bất kỳ mã nào bạn viết sẽ được đặt lại khi bạn tải lại trang, *vì vậy hãy sao chép mã của bạn vào một tài liệu riêng.* Các thử thách sau video (ví dụ: bài thơ Haiku) không phải là một phần của bài tập về nhà 0 nhưng bạn học được nhiều nhất bằng cách thực hành (thay vì chỉ xem một cách thụ động) - vì vậy chúng tôi khuyên bạn nên vui vẻ với mỗi thử thách kết thúc video.

Các câu hỏi HW0 ở bên dưới. Sao chép câu trả lời của bạn vào một tài liệu văn bản sd, bạn sẽ cần gửi chúng sau này trong khóa học.

## Chương 1

Trong đó anh hùng gan dạ của chúng ta chiến đấu với **đầu ra tiêu chuẩn**, **lỗi tiêu chuẩn**, **bộ mô tả tệp** và ghi vào tệp

### Xin chào, Thế giới! (kiểu gọi hệ thống)
1. Viết một chương trình sử dụng `write()` để in ra "Xin chào! Tên tôi là `<Tên của bạn>`".
### Xin chào, Luồng Lỗi Tiêu chuẩn!
2. Viết một hàm để in ra một hình tam giác có chiều cao `n` ra **lỗi tiêu chuẩn**.
   - Hàm của bạn phải có chữ ký `void write_triangle(int n)` và phải sử dụng `write()`.
   - Hình tam giác sẽ trông như thế này, với n = 3:
   ```C
   *
   **
   ***
   ```
### Ghi vào tệp
3. Lấy chương trình của bạn từ "Xin chào, Thế giới!" sửa đổi nó để ghi vào một tệp có tên là `hello_world.txt`.
   - Đảm bảo sử dụng đúng cờ và **chế độ** chính xác cho `open()` (`man 2 open` là bạn của bạn).
### Không phải mọi thứ đều là lệnh gọi hệ thống
4. Lấy chương trình của bạn từ "Ghi vào tệp" và thay thế `write()` bằng `printf()`.
   - Đảm bảo in vào tệp thay vì đầu ra tiêu chuẩn!
5. Một số điểm khác biệt giữa `write()` và `printf()` là gì?

## Chương 2

Xác định kích thước các kiểu C và giới hạn của chúng, mảng `int` và `char`, và tăng con trỏ

### Không phải tất cả các byte đều là 8 bit?
1. Có bao nhiêu bit trong một byte?
2. Có bao nhiêu byte trong một `char`?
3. Các mục sau đây trên máy của bạn có bao nhiêu byte?
   - `int`, `double`, `float`, `long` và `long long`
### Theo con trỏ int
4. Trên máy có số nguyên 8 byte:
```C
int main(){
    int data[8];
} 
```
Nếu địa chỉ của `data` là `0x7fbd9d40`, thì địa chỉ của `data+2` là gì?

5. `data[3]` tương đương với gì trong C?
   - Gợi ý: C chuyển đổi `data[3]` thành gì trước khi **giải tham chiếu** địa chỉ?

### Mảng ký tự `sizeof`, tăng con trỏ
  
Hãy nhớ rằng, kiểu của **hằng số chuỗi** `"abc"` là một mảng.

6. Tại sao điều này lại gây ra lỗi **segfault**?
```C
char *ptr = "hello";
*ptr = 'J';
```
7. `sizeof("Hello\0World")` trả về gì?
8. `strlen("Hello\0World")` trả về gì?
9. Đưa ra ví dụ về X sao cho `sizeof(X)` là 3.
10. Đưa ra ví dụ về Y sao cho `sizeof(Y)` có thể là 4 hoặc 8 tùy thuộc vào máy.

## Chương 3

Đối số chương trình, biến môi trường và làm việc với mảng ký tự (chuỗi)

### Đối số chương trình, `argc`, `argv`
1. Hai cách để tìm độ dài của `argv` là gì?
2. `argv[0]` đại diện cho gì?
### Biến môi trường
3. Con trỏ đến các biến môi trường được lưu trữ ở đâu (trên **ngăn xếp**, **heap**, ở đâu đó)?
### Tìm kiếm chuỗi (chuỗi chỉ là mảng ký tự)
4. Trên máy mà con trỏ là 8 byte và với đoạn mã sau:
```C
char *ptr = "Hello";
char array[] = "Hello";
```
Giá trị của `sizeof(ptr)` và `sizeof(array)` là gì? Tại sao?

### Vòng đời của các biến tự động

5. Cấu trúc dữ liệu nào quản lý vòng đời của các **biến tự động**?

## Chương 4

**Bộ nhớ heap** và **ngăn xếp** và làm việc với **cấu trúc**

### Cấp phát bộ nhớ bằng `malloc`, heap và thời gian
1. Nếu tôi muốn sử dụng dữ liệu sau khi **vòng đời** của hàm mà nó được tạo ra kết thúc, tôi nên đặt nó ở đâu? Làm cách nào để đặt nó ở đó?
2. Sự khác biệt giữa bộ nhớ heap và ngăn xếp là gì?
3. Có những loại bộ nhớ khác trong một tiến trình không?
4. Điền vào chỗ trống: "Trong một chương trình C tốt, đối với mỗi `malloc`, sẽ có một ___".
### Các lỗi thường gặp khi cấp phát heap
5. Một lý do khiến `malloc` có thể thất bại là gì?
6. Một số điểm khác biệt giữa `time()` và `ctime()` là gì?
7. Điều gì sai với đoạn mã này?
```C
free(ptr);
free(ptr);
```
8. Điều gì sai với đoạn mã này?
```C
free(ptr);
printf("%s\n", ptr);
```
9. Làm thế nào người ta có thể tránh hai sai lầm trước đó? 
### `struct`, `typedef` và danh sách liên kết
10. Tạo một `struct` đại diện cho `Person`. Sau đó, tạo một `typedef`, sao cho `struct Person` có thể được thay thế bằng một từ duy nhất. Một người nên chứa các thông tin sau: tên của họ (một chuỗi), tuổi của họ (một số nguyên) và danh sách bạn bè của họ (được lưu trữ dưới dạng con trỏ đến một mảng con trỏ đến `Person`).
11. Bây giờ, hãy tạo hai người trên heap, "Agent Smith" và "Sonny Moore", lần lượt là 128 và 256 tuổi và là bạn của nhau.
### Nhân bản chuỗi, cấp phát và giải phóng bộ nhớ của cấu trúc
Tạo các hàm để tạo và hủy một Người (Người và tên của họ phải nằm trên heap).
12. `create()` nên lấy tên và tuổi. Tên nên được sao chép vào heap. Sử dụng `malloc` để dành đủ bộ nhớ cho mọi người có tối đa mười người bạn. Hãy chắc chắn **khởi tạo** tất cả các trường (tại sao?).
13. `destroy()` nên giải phóng không chỉ bộ nhớ của struct person, mà còn giải phóng tất cả các thuộc tính của nó được lưu trữ trên heap. Việc hủy một người không nên hủy bất kỳ người nào khác.

## Chương 5 

Nhập và xuất văn bản và phân tích cú pháp bằng cách sử dụng `getchar`, `gets` và `getline`.

### Đọc ký tự, sự cố với gets
1. Những hàm nào có thể được sử dụng để lấy ký tự từ `stdin` và ghi chúng vào `stdout`?
2. Nêu một vấn đề với `gets()`.
### Giới thiệu `sscanf` và những người bạn
3. Viết mã phân tích cú pháp chuỗi "Hello 5 World" và khởi tạo 3 biến thành "Hello", 5 và "World".
### `getline` rất hữu ích
4. Người ta cần khai báo gì trước khi sử dụng `getline()`?
5. Viết một chương trình C để in ra nội dung của một tệp theo từng dòng bằng cách sử dụng `getline()`.

## Phát triển C

Đây là những mẹo chung để biên dịch và phát triển bằng cách sử dụng trình biên dịch và git. Một số tìm kiếm trên web sẽ hữu ích ở đây

1. Cờ **trình biên dịch** nào được sử dụng để tạo **bản dựng gỡ lỗi**?
2. Bạn sửa đổi **Makefile** để tạo bản dựng gỡ lỗi và gõ `make` một lần nữa. Giải thích tại sao điều này là không đủ để tạo một bản dựng mới.
3. Tab hay dấu cách được sử dụng để thụt lề các lệnh sau quy tắc trong Makefile?
4. `git commit` làm gì? `sha` trong ngữ cảnh của git là gì?
5. `git log` cho bạn thấy gì?
6. `git status` cho bạn biết điều gì và nội dung của `.gitignore` sẽ thay đổi đầu ra của nó như thế nào?
7. `git push` làm gì? Tại sao không đủ để chỉ **commit** với `git commit -m 'đã sửa tất cả các lỗi'`?
8. Lỗi **không chuyển tiếp nhanh** `git push` bị từ chối có nghĩa là gì? Cách phổ biến nhất để giải quyết vấn đề này là gì?

## Tùy chọn (Chỉ để cho vui)
- Chuyển đổi lời bài hát của bạn thành Lập trình Hệ thống và mã C được đề cập trong cuốn sách wiki này và chia sẻ trên Piazza.
- Tìm, theo ý kiến ​​của bạn, mã C tốt nhất và tệ nhất trên web và đăng liên kết lên Piazza.
- Viết một chương trình C ngắn với một lỗi C tinh tế có chủ ý và đăng nó lên Piazza để xem những người khác có thể phát hiện ra lỗi của bạn hay không.
- Bạn có biết lỗi lập trình hệ thống thú vị / tai hại nào mà bạn đã nghe nói đến không? Hãy thoải mái chia sẻ với các đồng nghiệp của bạn và nhân viên khóa học trên piazza.



