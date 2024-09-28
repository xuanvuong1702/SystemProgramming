## Những lỗi thường gặp mà lập trình viên C mắc phải?

# Lỗi về bộ nhớ

## Hằng số chuỗi là chỉ đọc
```C
char array[] = "Hi!"; // array chứa một bản sao có thể thay đổi 
strcpy(array, "OK");

char *ptr = "Can't change me"; // ptr trỏ đến một vùng bộ nhớ bất biến
strcpy(ptr, "Will not work");
```
Các **chuỗi ký tự** là các mảng ký tự được lưu trữ trong phân đoạn mã của chương trình, là bất biến. Hai chuỗi ký tự có thể chia sẻ cùng một không gian trong bộ nhớ. Một ví dụ như sau:

```C
char *str1 = "Brandon Chong là trợ giảng tốt nhất";
char *str2 = "Brandon Chong là trợ giảng tốt nhất";
str1 == str2; // đúng
```
Các chuỗi được trỏ đến bởi `str1` và `str2` thực sự có thể nằm ở cùng một vị trí trong bộ nhớ.

Tuy nhiên, các mảng ký tự chứa các bản sao sâu của các chuỗi ký tự được gán cho chúng và các mảng khác nhau đại diện cho các vị trí bộ nhớ khác nhau. Các mảng ký tự sau đây không nằm ở cùng một vị trí trong bộ nhớ.

```C
char arr1[] = "Brandon Chong không viết cái này";
char arr2[] = "Brandon Chong không viết cái này";
arr1 == arr2;         // sai
&arr1[0] == &arr2[0]; // sai
```

## Tràn / Thiếu bộ đệm
```C
int i = 10, array[10];
for (; i >= 0; i--) array[i] = i;
```
C không thực hiện kiểm tra giới hạn khi truy cập mảng. Ví dụ trên ghi vào `array[10]` nằm ngoài giới hạn mảng. Điều này có thể làm hỏng các biến khác trên **ngăn xếp** cũng như chính việc triển khai **ngăn xếp cuộc gọi**, khiến chương trình của bạn bị tấn công bởi tin tặc. Trong thực tế, những tràn bộ nhớ này thường là kết quả của việc sử dụng các lệnh gọi thư viện không an toàn hoặc đặt giới hạn kích thước sai trên một lệnh gọi thư viện an toàn hơn.

```C
gets(array); // Hy vọng rằng đầu vào ngắn hơn mảng của tôi! (KHÔNG BAO GIỜ sử dụng gets)
fgets(array, 4096, stdin); // Ôi trời 
```

## Xử lý con trỏ đến các biến tự động ngoài phạm vi
```C
int *f() {
    int result = 42;
    static int imok;
    int *p;
    {
      int x = result;
      p = &x;
    }
    //imok = *p;      // Không OK: x đã ra khỏi phạm vi
    //return &result; // Không OK: result sẽ ra khỏi phạm vi sau khi hàm trả về 
    return &imok;     // OK - các biến tĩnh không nằm trên ngăn xếp
}
```
Các biến tự động chỉ được liên kết với bộ nhớ ngăn xếp chừng nào chúng còn nằm trong phạm vi. Sau khi chúng ra khỏi phạm vi, dữ liệu được lưu trữ tại địa chỉ bộ nhớ của chúng trở nên không xác định. Các biến tĩnh nằm trong phân đoạn dữ liệu, có thể truy cập an toàn ngay cả khi các biến đó không nằm trong phạm vi.

## `sizeof(type *)` so với `sizeof(type)` 
```C
struct User {
   char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t *));
```
Trong ví dụ trên, chúng ta cần cấp phát đủ byte cho struct. Thay vào đó, chúng ta đã cấp phát đủ byte để chứa một con trỏ. Việc ghi vào con trỏ người dùng có thể làm hỏng **heap**. Mã chính xác được hiển thị bên dưới.
```C
struct User {
   char name[100];
};
typedef struct User user_t;

user_t *user = (user_t *) malloc(sizeof (user_t));
```

## Chuỗi yêu cầu `strlen(s)+1` Byte

Mọi chuỗi phải có một byte null sau ký tự thực tế cuối cùng. Để lưu trữ chuỗi <code>"Hi"</code> cần 3 byte: <code>[H] [i] [\0]</code>.

```C
char *strdup(const char *input) {     /* trả về một bản sao của 'input' */
    char *copy;
    copy = malloc(sizeof (char *));   /* không! điều này cấp phát không gian cho một con trỏ, không phải một chuỗi */
    copy = malloc(strlen(input));     /* Gần như...nhưng còn bộ kết thúc null thì sao? */
    copy = malloc(strlen(input) + 1); /* Đúng rồi. */
    strcpy(copy, input);              /* strcpy sẽ cung cấp bộ kết thúc null */
    return copy;
}
```

## Không khởi tạo bộ nhớ
```C
void myfunct() {
    char array[10];
    char *p = malloc(10);
    printf("%s %s\n", array, p);
}
```
Các biến tự động (ngăn xếp) và bộ nhớ heap thu được bằng `malloc` không được khởi tạo thành 0 theo mặc định. Hàm trên dẫn đến hành vi không xác định.

## Giải phóng kép
```C
char *p = malloc(10);
free(p);
//  .. sau đó ...
free(p); 
```
Giải phóng cùng một bộ nhớ heap hai lần là một lỗi.

## Con trỏ lơ lửng
```C
char *p = malloc(10);
strcpy(p, "Hello");
free(p);
//  .. sau đó ...
strcpy(p,"World"); 
```
Truy cập bộ nhớ đã được giải phóng dẫn đến hành vi không xác định. Một thực hành lập trình phòng thủ là đặt con trỏ thành NULL ngay khi bộ nhớ được giải phóng, vì không có cách nào khác để kiểm tra một cách đáng tin cậy xem con trỏ có trỏ đến một địa chỉ hợp lệ hay không. Macro sau đây thực hiện điều này.

```C
#define safer_free(p) {free(p); (p) = NULL;}
```

## Quên sao chép bộ đệm `getline`
```C
#include <stdio.h>
  
int main(void){
  char *line = NULL;
  size_t linecap = 0;
  char *strings[3];

  // giả sử stdin chứa "1\n2\n\3\n"
  for (size_t i = 0; i < 3; ++i)
    strings[i] = getline(&line, &linecap, stdin) >= 0 ? line : "";

  // điều này in ra "3\n3\n\3n" thay vì "3\n\2\n1\n"
  for (size_t i = 3; i--;) // i=2,1,0
    printf("%s", strings[i]);
}
```
Vì `getline` sử dụng lại bộ đệm, tất cả các con trỏ trong mảng `strings` thực sự đang trỏ đến cùng một bộ nhớ. Chúng ta có thể khắc phục điều này bằng cách đặt phép gán của `strings[i]` thành một bản sao sâu của bộ đệm.
```C
   strings[i] = getline(&line, &linecap, stdin) >= 0 ? strdup(line) : "";
```
Sự thật thú vị: việc cung cấp "1\n123456789abcdef\n3\n" cho phiên bản bị hỏng của chương trình có thể khiến nó in ra "3\n3\n1\n" thay vì "3\n3\n3\n". Tại sao lại như vậy? 
*Gợi ý: sử dụng Valgrind*

# Lỗi logic và luồng chương trình
## Quên `break` sau `case` 
```C
int flag = 1; // Sẽ in cả ba dòng.
switch (flag) {
case 1: printf("Tôi được in\n");
case 2: printf("Tôi cũng vậy\n");
case 3: printf("Tôi ba\n");
}
```
Các câu lệnh `case` không có `break` sẽ chỉ tiếp tục sang mã của câu lệnh `case` tiếp theo. Mã chính xác được hiển thị bên dưới. `break` cho câu lệnh cuối cùng là không cần thiết vì không còn `case` nào được thực thi sau câu lệnh cuối cùng.
```C
int flag = 1; // Sẽ chỉ in "Tôi được in\n"
switch (flag) {
case 1: 
    printf("Tôi được in\n");
    break;
case 2: 
    printf("Tôi cũng vậy\n");
    break;
case 3: 
    printf("Tôi ba\n");
    break; //không cần thiết
}
```

## Phép gán so với Kiểm tra bằng
```C
int answer = 3; // Sẽ in ra câu trả lời.
if (answer = 42) { printf("Tôi đã giải quyết được câu trả lời! Đó là %d", answer); }
```
Trình biên dịch thường sẽ cảnh báo bạn về lỗi này. Nếu bạn thực sự muốn thực hiện phép gán, hãy thêm một cặp dấu ngoặc đơn bổ sung để chặn các cảnh báo này.
```C
ssize_t x;
if ( (x = read(somefd, somebuf, somenum)) ){
  // làm gì đó
}
```

## Các hàm không được khai báo hoặc được khai báo nguyên mẫu không chính xác

```C
#include <stdio.h>
int main(void){
  int start = time();
  printf("%d\n", start);
}
```
Lệnh gọi thư viện `time` thực sự nhận một tham số (một con trỏ đến một số bộ nhớ có thể nhận cấu trúc `time_t`). Trình biên dịch có thể không phát hiện ra lỗi này vì lập trình viên đã không cung cấp nguyên mẫu hàm hợp lệ bằng cách bao gồm `time.h`. Vì lý do này, việc gọi các hàm không được khai báo là bất hợp pháp trong C99 trở lên.

## Dấu chấm phẩy thừa

Dấu chấm phẩy sau các câu lệnh `for` và `while` sẽ khiến chúng được hiểu là "vòng lặp rỗng".

```C
int i;
for (i = 0; i < 5; i++);{
  printf("Tôi được in một lần\n");
}
while (i < 10); // chương trình đi vào vòng lặp vô hạn
  i++;          // đoạn mã này không bao giờ được thực thi
```

Việc thêm dấu chấm phẩy liền kề nhau trong các khối mã là hoàn toàn hợp pháp và bị trình biên dịch bỏ qua. Bạn cũng có thể bỏ qua các thành phần của câu lệnh `for` bằng cách không đặt bất cứ thứ gì giữa các dấu chấm phẩy hoặc giữa dấu chấm phẩy và dấu ngoặc đơn.
```C
int i = 0;
for (; i++ < 5;) { // i=1,2,3,4,5
    printf("%d\n", i);;;;;;;;;;;;;
}
```

# Các lỗi khác
## Macro bộ tiền xử lý C và độ ưu tiên

Tiền xử lý là một thao tác được thực hiện **trước** khi chương trình thực sự được biên dịch. Nó không là gì ngoài việc thay thế văn bản, tức là sao chép và dán. Xem xét đoạn mã sau.
```C
#define BADD(x,y) x+y
char buffer[BADD(5,5)*2];
```

Sau khi tiền xử lý, mã sẽ trông giống hệt như thế này.
```C
char buffer[5+5*2];
```

Lưu ý rằng bộ đệm chiếm 15 byte thay vì 20, vì phép nhân có độ ưu tiên cao hơn phép cộng. Để ngăn chặn loại hành vi này, hãy đảm bảo bao quanh tất cả các biến macro cũng như toàn bộ biểu thức macro bằng dấu ngoặc đơn.
```C
#define ADD(x,y) ((x)+(y))
```

## Macro bộ tiền xử lý C và hiệu ứng phụ
```C
#define min(a,b) ((a)<(b) ? (a) : (b))
int x = 4;
if (min(x++, 100)) printf("%d là sáu", x);
```
Biểu thức điều kiện được mở rộng thành `x++ < 100 ? x++ : 100`, dẫn đến việc `x` được tăng lên hai lần. Không có cách nào tốt để ngăn chặn những hiệu ứng phụ này khi sử dụng macro C tiêu chuẩn. Nhưng [GNU C](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html) cung cấp một số cách giải quyết (chỉ hoạt động khi sử dụng GCC).

## `sizeof(type[])` so với `sizeof(type *)`
```C
#define ARRAY_LENGTH(A) (sizeof((A)) / sizeof((A)[0]))
int fixed_length_array[10]; // ARRAY_LENGTH(fixed_length_array) = 10
int *dynamic_array = malloc(10); // ARRAY_LENGTH(dynamic_array) = 2 hoặc 1
```
Nếu chúng ta có một biến mảng được khai báo như `fixed_length_array`, toán tử `sizeof` sẽ mang lại số byte mà mảng chiếm dụng và chia kích thước này cho kích thước của phần tử đầu tiên sẽ mang lại số lượng phần tử trong mảng. Thật không may, kích thước của một con trỏ luôn giống nhau (8 hoặc 4 byte), bất kể kích thước hoặc kiểu của mảng mà nó trỏ đến. Chỉ các biến mảng được khai báo trong thời gian biên dịch (và [mảng có độ dài biến C99](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html)) sẽ hiển thị kích thước mảng thực thông qua `sizeof`.

## `sizeof` và hiệu ứng phụ

```C
int a = 0;
size_t size = sizeof(a++);
printf("size: %lu, a: %d", size, a);
```
Mã này in ra như sau.
```C
size: 4, a: 0
```
Biểu thức được truyền vào `sizeof` không thực sự được đánh giá trong thời gian chạy trong hầu hết các trường hợp, vì kiểu (do đó là kích thước) của biểu thức có thể được tính toán trong thời gian biên dịch. Mặc dù có những ngoại lệ trong trường hợp [mảng có độ dài biến C99](http://port70.net/~nsz/c/c11/n1570.html#6.5.3.4p2), vì kích thước của chúng được xác định trong thời gian chạy.

<div align="center">
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming,-Part-2:-Text-Input-And-Output">
Trở lại: Lập trình C, Phần 2: Nhập và Xuất Văn bản
</a> |
<a href="https://github.com/angrave/SystemProgramming/wiki/C-Programming%2C-Part-4%3A-Strings-and-Structs">
Tiếp theo: Lập trình C, Phần 4: Chuỗi và Cấu trúc
</a>
</div>
