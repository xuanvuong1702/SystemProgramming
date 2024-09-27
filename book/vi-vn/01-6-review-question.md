---
title: "Chương 01: Học Ngữ Lập Trình C, Câu hỏi ôn tập"
type: book-vi-vn
order: 6
---

# Câu hỏi/Bài tập

1. Đoạn code sau in ra gì?
```C
int main(){
    fprintf(stderr, "Hello ");
    fprintf(stdout, "It's a small ");
    fprintf(stderr, "World\n");
    fprintf(stdout, "place\n");
    return 0;
}
```
2.  Sự khác biệt giữa hai khai báo sau là gì? `sizeof` trả về gì cho mỗi khai báo?
```C
char str1[] = "another";
char *str2 = "another one";
```
3. Chuỗi trong C là gì?
4. Hãy viết một hàm `my_strcmp` đơn giản. Còn `my_strcat`, `my_strcpy`, hoặc `my_strdup` thì sao? 

**Bonus:** Viết các hàm mà chỉ duyệt qua chuỗi _một lần_.

5. Các biểu thức sau thường trả về gì?
```C
int *ptr;
sizeof(ptr);
sizeof(*ptr);
```
6. `malloc` là gì? Nó khác `calloc` như thế nào? Khi bộ nhớ đã được `malloc`, làm thế nào tôi có thể sử dụng `realloc`?
7. Toán tử `&` là gì? Còn `*` thì sao?
8. Số học con trỏ. Giả sử có các địa chỉ sau. Các biểu thức sau sẽ có giá trị là gì?
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
     
9. Làm thế nào để ngăn chặn lỗi giải phóng bộ nhớ kép (double free)?
10. Chỉ định định dạng (format specifier) trong `printf` để in một chuỗi, `int`, hoặc `char` là gì?
11. Đoạn code sau có hợp lệ không? Nếu có, tại sao? `output` được đặt ở đâu?
```C
char *foo(int var){
    static char output[20];
    snprintf(output, 20, "%d", var);
    return output;
}
```
12. Viết một hàm nhận một chuỗi đường dẫn đến tệp và mở tệp đó, in ra nội dung của tệp 40 byte mỗi lần, nhưng mỗi lần in thứ hai sẽ đảo ngược chuỗi (hãy thử sử dụng API POSIX cho bài tập này).
13. Những điểm khác biệt giữa mô hình bộ mô tả tệp (file descriptor) của POSIX và `FILE*` của C là gì (ví dụ: các hàm nào được sử dụng và cơ chế đệm)? POSIX có sử dụng `FILE*` của C bên trong hay ngược lại?

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-5%3A-Debugging">
Quay lại: Học Ngữ Lập Trình C, Phần 5: Gỡ lỗi
</a>
</div>
