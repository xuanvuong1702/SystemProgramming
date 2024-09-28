# In ra Luồng

## Làm cách nào để in chuỗi, số nguyên, ký tự ra luồng xuất chuẩn? 
Sử dụng `printf`. Tham số đầu tiên là một chuỗi định dạng bao gồm các trình giữ chỗ cho dữ liệu cần in. Các trình chỉ định định dạng phổ biến là `%s` coi đối số là một con trỏ chuỗi C, tiếp tục in tất cả các ký tự cho đến khi gặp ký tự NULL; `%d` in đối số dưới dạng số nguyên; `%p` in đối số dưới dạng địa chỉ bộ nhớ. 

Một ví dụ đơn giản được hiển thị bên dưới:
```C
char *name = ... ; int score = ...;
printf("Hello %s, your result is %d\n", name, score);
printf("Debug: Chuỗi và số nguyên được lưu trữ tại: %p và %p\n", name, &score );
// name đã là một con trỏ char và trỏ đến đầu mảng. 
// Chúng ta cần "&" để lấy địa chỉ của biến int
```

Theo mặc định, vì hiệu suất, `printf` không thực sự ghi bất cứ thứ gì ra (bằng cách gọi `write`) cho đến khi bộ đệm của nó đầy hoặc một ký tự xuống dòng được in. 

## Tôi còn có thể in chuỗi và ký tự đơn bằng cách nào khác?
Sử dụng `puts( name )` và `putchar( c )`  trong đó `name` là một con trỏ đến một chuỗi C và `c` chỉ là một `char`

## Làm cách nào để in ra các luồng tệp khác?
Sử dụng `fprintf( _file_ , "Hello %s, score: %d", name, score);`
Trong đó `_file_` là `stdout` `stderr` được xác định trước hoặc một con trỏ FILE được trả về bởi `fopen` hoặc `fdopen`

## Tôi có thể sử dụng bộ mô tả tệp không?
Có! Chỉ cần sử dụng `dprintf(int fd, char* format_string, ...);` Chỉ cần nhớ rằng luồng có thể được đệm, vì vậy bạn sẽ cần đảm bảo rằng dữ liệu được ghi vào bộ mô tả tệp.

## Làm cách nào để in dữ liệu vào một chuỗi C?
Sử dụng `sprintf` hoặc tốt hơn là `snprintf`.
```C
char result[200];
int len = snprintf(result, sizeof(result), "%s:%d", name, score);
```
`snprintf` trả về số lượng ký tự sẽ được ghi nếu có đủ không gian, không bao gồm byte NULL kết thúc.
```C
char x[5];
int size = snprintf(x, 5, "%s%s%s", "12", "34", "56"); // ghi "1234" + NULL
printf("%d\n", size); // xuất ra 6
```
Nguồn: [bài đăng StackOverflow này](https://stackoverflow.com/questions/12746885/why-use-asprintf) và trang hướng dẫn.

## Điều gì sẽ xảy ra nếu tôi thực sự muốn `printf` gọi `write` mà không có ký tự xuống dòng?

Sử dụng `fflush( FILE* inp )`. Nội dung của tệp sẽ được ghi. Nếu tôi muốn viết "Hello World" mà không có ký tự xuống dòng, tôi có thể viết nó như thế này.

```C
int main(){
    fprintf(stdout, "Hello World");
    fflush(stdout);
    return 0;
}
```

## `perror` hữu ích như thế nào?
Giả sử rằng bạn có một lệnh gọi hàm vừa thất bại (vì bạn đã kiểm tra trang hướng dẫn và đó là mã trả về lỗi). `perror(const char* message)` sẽ in phiên bản tiếng Anh của lỗi ra `stderr`
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

# Phân tích cú pháp Đầu vào

## Làm cách nào để phân tích cú pháp số từ chuỗi?

Sử dụng `long int strtol(const char *nptr, char **endptr, int base);` hoặc `long long int strtoll(const char *nptr, char **endptr, int base);`.

Những hàm này làm gì là lấy con trỏ đến chuỗi của bạn `*nptr` và một `base` (tức là nhị phân, bát phân, thập phân, thập lục phân, v.v.) và một con trỏ tùy chọn `endptr` và trả về một giá trị được phân tích cú pháp.

```C
int main(){
    const char *nptr = "1A2436";
    char* endptr;
    long int result = strtol(nptr, &endptr, 16);
    return 0;
}
```

Tuy nhiên, hãy cẩn thận! Xử lý lỗi rất phức tạp vì hàm sẽ không trả về mã lỗi. Nếu bạn cung cấp cho nó một chuỗi không phải là số, nó sẽ trả về 0. Điều này có nghĩa là bạn không thể phân biệt giữa "0" hợp lệ và chuỗi không hợp lệ. Xem trang hướng dẫn để biết thêm chi tiết về hành vi của `strtol` với các giá trị không hợp lệ và nằm ngoài giới hạn. Một cách thay thế an toàn hơn là sử dụng `sscanf` (và kiểm tra giá trị trả về).

```C
int main(){
    const char *input = "0"; // hoặc "!##@" hoặc ""
    char* endptr;
    long int parsed = strtol(input, &endptr, 10);
    if(parsed == 0){
        // Chuỗi đầu vào không phải là số ở cơ số 10 hợp lệ hoặc nó thực sự bằng 0!

    }
    return 0;
}
```

## Làm cách nào để phân tích cú pháp đầu vào bằng `scanf` thành các tham số?
Sử dụng `scanf` (hoặc `fscanf` hoặc `sscanf`) để nhận đầu vào tương ứng từ luồng nhập chuẩn, một luồng tệp tùy ý hoặc một chuỗi C.
Nên kiểm tra giá trị trả về để xem có bao nhiêu mục đã được phân tích cú pháp.
Các hàm `scanf` yêu cầu con trỏ hợp lệ. Việc truyền một giá trị con trỏ không chính xác là một nguồn lỗi phổ biến. Ví dụ:
```C
int *data = (int *) malloc(sizeof(int));
char *line = "v 10";
char type;
// Thực hành tốt: Kiểm tra scanf đã phân tích cú pháp dòng và đọc hai giá trị:
int ok = 2 == sscanf(line, "%c %d", &type, data); // lỗi con trỏ
```
Chúng tôi muốn ghi giá trị ký tự vào `type` và giá trị số nguyên vào bộ nhớ được cấp phát bởi `malloc`.
Tuy nhiên, chúng tôi đã truyền địa chỉ của con trỏ dữ liệu, chứ không phải thứ mà con trỏ đang trỏ đến! Vì vậy, `sscanf` sẽ thay đổi chính con trỏ. tức là con trỏ bây giờ sẽ trỏ đến địa chỉ 10 nên mã này sau đó sẽ thất bại, ví dụ: khi `free(data)` được gọi.
 
## Làm cách nào để ngăn `scanf` gây ra tràn bộ đệm?
Mã sau giả định rằng `scanf` sẽ không đọc quá 10 ký tự (bao gồm cả byte kết thúc) vào bộ đệm.
```C
char buffer[10];
scanf("%s",buffer);
```
Bạn có thể bao gồm một số nguyên tùy chọn để chỉ định số lượng ký tự KHÔNG BAO GỒM byte kết thúc:
```C
char buffer[10];
scanf("%9s", buffer); // đọc tối đa 9 ký tự từ đầu vào (chừa chỗ cho byte thứ 10 là byte kết thúc)
```

## Tại sao `gets` nguy hiểm? Tôi nên sử dụng gì thay thế?
Mã sau dễ bị tấn công tràn bộ đệm. Nó giả định hoặc tin tưởng rằng dòng đầu vào sẽ không quá 10 ký tự, bao gồm cả byte kết thúc.
```C
char buf[10];
gets(buf); // Hãy nhớ rằng tên mảng có nghĩa là byte đầu tiên của mảng
``` 
`gets` không được dùng nữa trong tiêu chuẩn C99 và đã bị xóa khỏi tiêu chuẩn C mới nhất (C11). Các chương trình nên sử dụng `fgets` hoặc `getline` thay thế. 

Trong đó mỗi cái có cấu trúc sau tương ứng:
```C 
char *fgets (char *str, int num, FILE *stream); 

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
```

Đây là một cách đơn giản, an toàn để đọc một dòng. Các dòng dài hơn 9 ký tự sẽ bị cắt bớt:
```C
char buffer[10];
char *result = fgets(buffer, sizeof(buffer), stdin);
```
Kết quả là NULL nếu có lỗi hoặc gặp kết thúc tệp.
Lưu ý, không giống như `gets`,  `fgets` sao chép ký tự xuống dòng vào bộ đệm, mà bạn có thể muốn loại bỏ-
```C
if (!result) { return; /* không có dữ liệu - không đọc nội dung bộ đệm */}

int i = strlen(buffer) - 1;
if (buffer[i] == '\n') 
    buffer[i] = '\0';
```

## Làm cách nào để sử dụng `getline`?
Một trong những lợi thế của `getline` là nó sẽ tự động (phân bổ lại) một bộ đệm trên heap có kích thước đủ.

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
// Bộ đệm hiện có sẽ được sử dụng lại hoặc nếu cần,
// Nó sẽ được `free` và một bộ đệm lớn hơn mới sẽ được `malloc`
chars = getline(&buffer, &size, stdin);

// Sau đó... đừng quên giải phóng bộ đệm!
free(buffer);
```

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-1:-Introduction">
Trở lại: Lập trình C, Phần 1: Giới thiệu
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-3%3A-Common-Gotchas">
Tiếp theo: Lập trình C, Phần 3: Các lỗi thường gặp
</a>
</div>
