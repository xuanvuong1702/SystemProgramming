# Chủ đề
* Biểu diễn chuỗi C
* Chuỗi C như con trỏ
* `char p[]` so với `char* p`
* Các hàm chuỗi C đơn giản (`strcmp`, `strcat`, `strcpy`)
* `sizeof char`
* `sizeof x` so với `x*`
* Vòng đời bộ nhớ Heap
* Các lệnh gọi cấp phát heap
* Giải tham chiếu con trỏ
* Toán tử lấy địa chỉ
* Số học con trỏ
* Nhân bản chuỗi
* Cắt ngắn chuỗi
* Lỗi double-free
* Chuỗi ký tự
* Định dạng in.
* Lỗi bộ nhớ vượt quá giới hạn
* Bộ nhớ tĩnh
* File I/O POSIX so với thư viện C
* C I/O `fprintf` và `printf`
* POSIX file I/O (`read`, `write`, `open`)
* Bộ đệm của `stdout`

# Câu hỏi / Bài tập

* Đoạn mã sau in ra gì
```C
int main(){
    fprintf(stderr, "Hello ");
    fprintf(stdout, "It's a small ");
    fprintf(stderr, "World\n");
    fprintf(stdout, "place\n");
    return 0;
}
```
* Sự khác biệt giữa hai khai báo sau là gì? `sizeof` trả về gì cho mỗi khai báo?
```C
char str1[] = "another";
char *str2 = "another one";
```
* Chuỗi trong C là gì?
* Viết mã một hàm `my_strcmp` đơn giản. Còn `my_strcat`, `my_strcpy` hoặc `my_strdup` thì sao? Phần thưởng: Viết mã các hàm trong khi chỉ duyệt qua các chuỗi _một lần_.
* Thông thường những câu lệnh sau sẽ trả về gì?
```C
int *ptr;
sizeof(ptr);
sizeof(*ptr);
```
* `malloc` là gì? Nó khác với `calloc` như thế nào. Khi bộ nhớ được cấp phát bởi `malloc`, làm cách nào tôi có thể sử dụng `realloc`?
* Toán tử `&` là gì? Còn `*` thì sao?
* Số học con trỏ. Giả sử các địa chỉ sau. Các phép dịch chuyển sau đây là gì?
```C
char** ptr = malloc(10); //0x100
ptr[0] = malloc(20); //0x200
ptr[1] = malloc(20); //0x300
```
     * `ptr + 2`
     * `ptr + 4`
     * `ptr[0] + 4`
     * `ptr[1] + 2000`
     * `*((int*)(ptr + 1)) + 3`
* Làm cách nào để chúng ta ngăn chặn lỗi double free?
* Trình chỉ định định dạng của `printf` để in chuỗi, `int` hoặc `char` là gì?
* Đoạn mã sau có hợp lệ không? Nếu có, tại sao? `output` nằm ở đâu?
```C
char *foo(int var){
    static char output[20];
    snprintf(output, 20, "%d", var);
    return output;
}
```
* Viết một hàm nhận một chuỗi và mở tệp đó, in ra tệp 40 byte mỗi lần nhưng mỗi lần in khác đảo ngược chuỗi (hãy thử sử dụng POSIX API cho việc này).
* Một số điểm khác biệt giữa mô hình bộ mô tả tệp POSIX và `FILE*` của C là gì (tức là những lệnh gọi hàm nào được sử dụng và lệnh nào được đệm)? POSIX có sử dụng `FILE*` của C bên trong hay ngược lại?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Trở lại: Lập trình C, Phần 5: Gỡ lỗi
</a>
</div>


