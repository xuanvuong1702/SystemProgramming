# Các Chủ Đề
* Biểu diễn chuỗi C
* Chuỗi C dưới dạng con trỏ
* char p[] so với char* p
* Các hàm chuỗi C đơn giản (strcmp, strcat, strcpy)
* sizeof char
* sizeof x so với x*
* Thời gian sống của bộ nhớ Heap
* Các lời gọi cấp phát bộ nhớ Heap
* Tham chiếu con trỏ
* Toán tử lấy địa chỉ
* Toán học con trỏ
* Sao chép chuỗi
* Cắt ngắn chuỗi
* Lỗi double-free
* Chuỗi ký tự hằng số
* Định dạng in.
* Lỗi bộ nhớ vượt ra ngoài giới hạn
* Bộ nhớ tĩnh
* fileio POSIX so với thư viện C
* C io fprintf và printf
* IO file POSIX (đọc, viết, mở)
* Đệm của stdout
# Câu hỏi/Bài tập

* Đoạn code sau in ra gì
```C
int main(){
    fprintf(stderr, "Hello ");
    fprintf(stdout, "It's a small ");
    fprintf(stderr, "World\n");
    fprintf(stdout, "place\n");
    return 0;
}
```
* Những khác biệt giữa hai khai báo sau là gì? `sizeof` trả về gì cho một trong số chúng?
```C
char str1[] = "another";
char *str2 = "another one";
```
* Chuỗi trong C là gì?
* Hãy viết một hàm `my_strcmp` đơn giản. Còn `my_strcat`, `my_strcpy`, hoặc `my_strdup` thì sao? 

Thêm: Viết các hàm mà chỉ đi qua chuỗi _một lần_.

* Những dòng sau thường trả về gì?
```C
int *ptr;
sizeof(ptr);
sizeof(*ptr);
```
* `malloc` là gì? Nó khác `calloc` như thế nào. Khi bộ nhớ đã được `malloc`, làm thế nào tôi có thể sử dụng `realloc`?
* Toán tử `&` là gì? Còn `*` thì sao?
* Toán học con trỏ. Giả sử có các địa chỉ sau. Những dịch chuyển sau là gì?
```C
char** ptr = malloc(10); //0x100
ptr[0] = malloc(20); //0x200
ptr[1] = malloc(20); //0x300
```
     * `ptr + 2`
     * `ptr + 4`
     * `ptr[0] + 4`
     * `ptr[1] + 2000`
     * `*((int)(ptr + 1)) + 3`
* Làm thế nào để ngăn chặn lỗi giải phóng bộ nhớ kép?
* Định dạng printf để in một chuỗi, `int`, hoặc `char` là gì?
* Đoạn mã sau có hợp lệ không? Nếu có, tại sao? `output` được đặt ở đâu?
```C
char *foo(int var){
    static char output[20];
    snprintf(output, 20, "%d", var);
    return output;
}
```
* Viết một hàm nhận một chuỗi và mở tệp đó, in ra tệp 40 byte một lần nhưng mỗi lần in thứ hai sẽ đảo ngược chuỗi (hãy thử sử dụng API POSIX cho việc này).
* Những khác biệt nào giữa mô hình filedescriptor POSIX và `FILE*` của C (tức là những lời gọi hàm nào được sử dụng và cái nào được đệm)? POSIX có sử dụng `FILE*` của C bên trong hay ngược lại không?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Quay lại: Lập trình C, Phần 5: Gỡ lỗi
</a>
</div>