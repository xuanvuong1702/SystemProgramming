---
title: "Chương 01: Học Ngôn Ngữ Lập Trình C, Phần 2: Nhập và Xuất Văn Bản"
type: book-vi-vn
order: 2
---

# In ra Luồng

## Làm thế nào để in chuỗi, số nguyên, ký tự ra luồng xuất chuẩn?
Sử dụng `printf`. Tham số đầu tiên là một chuỗi định dạng bao gồm các trình giữ chỗ cho dữ liệu cần in. Các chỉ định định dạng phổ biến là `%s` (xử lý đối số như một con trỏ chuỗi C, tiếp tục in tất cả các ký tự cho đến khi gặp ký tự NULL), `%d` (in đối số dưới dạng số nguyên), `%p` (in đối số dưới dạng địa chỉ bộ nhớ).

Một ví dụ đơn giản được hiển thị dưới đây:

```C
char *name = ... ; int score = ...;
printf("Hello %s, your result is %d\n", name, score);
printf("Debug: Chuỗi và số nguyên được lưu trữ tại: %p và %p\n", name, &score );
// name đã là một con trỏ char và trỏ đến phần đầu của mảng.
// Chúng ta cần "&" để lấy địa chỉ của biến kiểu int
```

Mặc định, vì hiệu suất, `printf` không thực sự ghi bất cứ thứ gì ra (bằng cách gọi `write`) cho đến khi bộ đệm của nó đầy hoặc một dòng mới được in ra.

## Có cách nào khác để in chuỗi và ký tự đơn không?
Sử dụng `puts( name )` (để in chuỗi) và `putchar( c )` (để in ký tự)  trong đó `name` là một con trỏ đến một chuỗi C và `c` là một `char`.

## Làm thế nào để in ra các luồng tệp khác?
Sử dụng `fprintf( _file_ , "Hello %s, score: %d", name, score);`
Trong đó `_file_` là một trong các luồng được định nghĩa trước `stdout`, `stderr` hoặc một con trỏ `FILE` được trả về bởi `fopen` hoặc `fdopen`.

## Tôi có thể sử dụng bộ mô tả tệp (file descriptor) không?
Có chứ! Bạn chỉ cần sử dụng `dprintf(int fd, char* format_string, ...);`. 
Chỉ cần nhớ rằng luồng có thể được đệm, vì vậy bạn sẽ cần đảm bảo rằng dữ liệu được ghi vào bộ mô tả tệp.

## Làm thế nào để in dữ liệu vào một chuỗi C?
Sử dụng `sprintf` hoặc tốt hơn là `snprintf`.
```C
char result[200];
int len = snprintf(result, sizeof(result), "%s:%d", name, score);
```
`snprintf` trả về số lượng ký tự mà **có thể** đã được ghi nếu có đủ không gian, không bao gồm byte NULL kết thúc.
```C
char x[5];
int size = snprintf(x, 5, "%s%s%s", "12", "34", "56"); // ghi "1234" + NULL
printf("%d\n", size); // xuất ra 6
```
Nguồn: [bài viết StackOverflow này](https://stackoverflow.com/questions/12746885/why-use-asprintf) và trang hướng dẫn (man page).

## Nếu tôi thực sự muốn `printf` gọi `write` mà không cần dòng mới thì sao?

Sử dụng `fflush( FILE* inp )`. Nội dung của tệp sẽ được ghi. Nếu tôi muốn viết "Hello World" mà không cần dòng mới, tôi có thể viết nó như thế này.

```C
int main(){
    fprintf(stdout, "Hello World");
    fflush(stdout);
    return 0;
}
```
## `perror` hữu ích như thế nào?
Hãy tưởng tượng bạn có một lời gọi hàm vừa thất bại (bởi vì bạn đã kiểm tra trang hướng dẫn và đó là một mã trả về lỗi). `perror(const char* message)` sẽ in mô tả lỗi ra `stderr`.
```C
int main(){
    int ret = open("IDoNotExist.txt", O_RDONLY);
    if(ret < 0){
        perror("Mở IDoNotExist:");
    }
    //...
    return 0;
}
```

# Phân tích cú pháp đầu vào

## Làm thế nào để phân tích số nguyên từ chuỗi?

Sử dụng `long int strtol(const char *nptr, char **endptr, int base);` hoặc `long long int strtoll(const char *nptr, char **endptr, int base);`.

Những hàm này nhận một con trỏ đến chuỗi của bạn `*nptr`, một cơ số `base` (ví dụ: nhị phân, bát phân, thập phân, thập lục phân, v.v.) và một con trỏ tùy chọn `endptr`, sau đó trả về một giá trị số nguyên đã được phân tích cú pháp.

```C
int main(){
    const char *nptr = "1A2436";
    char* endptr;
    long int result = strtol(nptr, &endptr, 16);
    return 0;
}
```

Tuy nhiên, hãy cẩn thận! Xử lý lỗi là một việc khá phức tạp bởi vì hàm không trả về một mã lỗi. Nếu bạn đưa cho nó một chuỗi không phải là số, nó sẽ trả về 0. Điều này có nghĩa là bạn không thể phân biệt giữa một "0" hợp lệ và một chuỗi không hợp lệ. Hãy xem trang hướng dẫn để biết thêm chi tiết về hành vi của `strtol` với các giá trị không hợp lệ và ngoài giới hạn. Một lựa chọn an toàn hơn là sử dụng `sscanf` (và kiểm tra giá trị trả về).

```C
int main(){
    const char *input = "0"; // hoặc "!##@" hoặc ""
    char* endptr;
    long int parsed = strtol(input, &endptr, 10);
    if(parsed == 0){
        // Hoặc chuỗi đầu vào không phải là một số hợp lệ cơ sở 10, hoặc nó thực sự là số không!

    }
    return 0;
}
```

## Làm thế nào để phân tích số thực từ chuỗi?

Sử dụng `double strtod(const char *nptr, char **endptr);` hoặc `float strtof(const char *nptr, char **endptr);` hoặc `long double strtold(const char *nptr, char **endptr);`.

Những hàm này hoạt động giống như `strtol` và `strtoll`, nhưng chúng trả về một giá trị số thực.

```C
int main(){
    const char *nptr = "3.14159";
    char* endptr;
    double result = strtod(nptr, &endptr);
    return 0;
}
```

## Làm thế nào để phân tích dữ liệu đầu vào sử dụng `scanf` vào các tham số?
Sử dụng `scanf` (hoặc `fscanf` hoặc `sscanf`) để lấy dữ liệu đầu vào từ luồng đầu vào mặc định, một luồng tệp tùy ý hoặc một chuỗi C tương ứng.
Nên kiểm tra giá trị trả về để xem có bao nhiêu mục đã được phân tích cú pháp.
Các hàm `scanf` yêu cầu các con trỏ hợp lệ. Đây là một nguồn lỗi phổ biến khi truyền vào một giá trị con trỏ không chính xác. Ví dụ:
```C
int *data = (int *) malloc(sizeof(int));
char *line = "v 10";
char type;
// Thực hành tốt: Kiểm tra scanf đã phân tích dòng và đọc hai giá trị:
int ok = 2 == sscanf(line, "%c %d", &type, &data); // lỗi con trỏ
```
Chúng ta muốn ghi giá trị ký tự vào `type` và giá trị số nguyên vào vùng nhớ được cấp phát bởi `malloc`. Tuy nhiên, chúng ta đã truyền địa chỉ của con trỏ `data`, chứ không phải là vùng nhớ mà con trỏ đang trỏ tới! 
Vì vậy, `sscanf` sẽ thay đổi chính con trỏ. 
Tức là con trỏ bây giờ sẽ trỏ đến địa chỉ 10, 
vì vậy đoạn mã này sẽ bị lỗi sau đó, ví dụ: khi `free(data)` được gọi.

## Làm thế nào để ngăn `scanf` gây ra lỗi tràn bộ đệm?
Đoạn mã sau đây giả định rằng `scanf` sẽ không đọc quá 10 ký tự (bao gồm cả byte kết thúc) vào bộ đệm.
```C
char buffer[10];
scanf("%s",buffer);
```
Tuy nhiên, nếu người dùng nhập vào nhiều hơn 9 ký tự (vì cần một byte cho ký tự kết thúc chuỗi), `scanf` sẽ ghi vào bộ nhớ mà nó không được phép, dẫn đến lỗi tràn bộ đệm. 
Để tránh điều này, bạn có thể chỉ định số lượng ký tự tối đa mà `scanf` nên đọc:

```C
char buffer[10];
scanf("%9s", buffer); // đọc tối đa 9 ký tự từ đầu vào (để dành chỗ cho byte thứ 10 là byte kết thúc)
```

## Tại sao `gets` nguy hiểm? Tôi nên sử dụng gì thay thế?
Đoạn mã sau đây dễ bị lỗi tràn bộ đệm. Nó giả định hoặc tin rằng dòng đầu vào sẽ không quá 10 ký tự, bao gồm cả byte kết thúc.
```C
char buf[10];
gets(buf); // Hãy nhớ rằng tên mảng nghĩa là byte đầu tiên của mảng
``` 
`gets` đã bị khai tử trong tiêu chuẩn C99 và đã bị loại bỏ khỏi tiêu chuẩn C mới nhất (C11). Các chương trình nên sử dụng `fgets` hoặc `getline` thay thế.

Mỗi hàm có cấu trúc như sau:
```C
char *fgets (char *str, int num, FILE *stream); 

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```
Đây là một cách đơn giản, an toàn để đọc một dòng. Các dòng dài hơn 9 ký tự sẽ bị cắt bớt:
```C
char buffer[10];
char *result = fgets(buffer, sizeof(buffer), stdin);
```
Kết quả sẽ là `NULL` nếu có lỗi hoặc đạt đến cuối tệp.
Lưu ý, không giống như `gets`, `fgets` sẽ sao chép ký tự xuống dòng vào bộ đệm, điều này có thể bạn muốn loại bỏ:
```C
if (!result) { return; /* không có dữ liệu - không đọc nội dung bộ đệm */}

int i = strlen(buffer) - 1;
if (buffer[i] == '\n') 
    buffer[i] = '\0';
```

## Làm thế nào để sử dụng `getline`?
Một trong những lợi ích của `getline` là nó sẽ tự động cấp phát (hoặc tái cấp phát) một bộ đệm trên heap có kích thước đủ lớn.

```C
// ssize_t getline(char **lineptr, size_t *n, FILE *stream);

/* đặt bộ đệm và kích thước thành 0; chúng sẽ được thay đổi bởi getline */
char *buffer = NULL;
size_t size = 0;

ssize_t chars = getline(&buffer, &size, stdin);

// Loại bỏ ký tự xuống dòng nếu có,
if (chars > 0 && buffer[chars-1] == '\n') 
    buffer[chars-1] = '\0';

// Đọc một dòng khác.
// Bộ đệm hiện có sẽ được sử dụng lại, hoặc, nếu cần thiết,
// Nó sẽ được `free` và một bộ đệm lớn hơn mới sẽ được `malloc`
chars = getline(&buffer, &size, stdin);

// Sau này... đừng quên giải phóng bộ đệm!
free(buffer);
```
<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-1:-Introduction">
Trở lại: Học Ngữ Lập Trình C, Phần 1: Giới Thiệu
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas">
Tiếp theo: Học Ngữ Lập Trình C, Phần 3: Các Lỗi Thường Gặp
</a>
</div>
