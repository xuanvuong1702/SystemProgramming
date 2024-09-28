## Cảnh báo - số câu hỏi có thể thay đổi
# Bộ nhớ và Chuỗi
## Q1.1
Trong ví dụ bên dưới, biến nào được đảm bảo in ra giá trị bằng không?
````C
int a;
static int b;

void func() {
   static int c;
   int d;
   printf("%d %d %d %d\n",a,b,c,d);
}
````
## Q 1.2
Trong ví dụ bên dưới, biến nào được đảm bảo in ra giá trị bằng không?
````C
void func() {
   int* ptr1 = malloc( sizeof(int) );
   int* ptr2 = realloc(NULL, sizeof(int) );
   int* ptr3 = calloc( 1, sizeof(int) );
   int* ptr4 = calloc( sizeof(int) , 1);
   
   printf("%d %d %d %d\n",*ptr1,*ptr2,*ptr3,*ptr4);
}
````
## Q 1.3
Giải thích lỗi trong nỗ lực sao chép chuỗi sau.
````C
char* copy(char*src) {
 char*result = malloc( strlen(src) ); 
 strcpy(result, src); 
 return result;
}
````
## Q 1.4
Tại sao nỗ lực sao chép chuỗi sau đây đôi khi hoạt động và đôi khi thất bại?

````C
char* copy(char*src) {
 char*result = malloc( strlen(src) +1 ); 
 strcat(result, src); 
 return result;
}
````
## Q 1.5
Giải thích hai lỗi trong đoạn mã sau đây cố gắng sao chép một chuỗi.
````C
char* copy(char*src) {
 char result[sizeof(src)]; 
 strcpy(result, src); 
 return result;
}
````
## Q 1.6
Câu lệnh nào sau đây là hợp lệ?
````C
char a[] = "Hello"; strcpy(a, "World");
char b[] = "Hello"; strcpy(b, "World12345", b);
char* c = "Hello"; strcpy(c, "World");
````

## Q 1.7
Hoàn thành typedef con trỏ hàm để khai báo một con trỏ đến một hàm nhận một đối số `void*` và trả về `void*`. Đặt tên cho kiểu của bạn là `pthread_callback`
````C
typedef ______________________ pthread_callback;
````
## Q 1.8
Ngoài các đối số hàm, những gì khác được lưu trữ trên ngăn xếp của một luồng?

## Q 1.9
Triển khai một phiên bản của `char* strcat(char*dest, const char*src)` chỉ sử dụng `strcpy` `strlen` và số học con trỏ
````C
char* mystrcat(char*dest, const char*src) {

  // Sử dụng strcpy strlen ở đây

  return dest;
}
````
## Q 1.10
Triển khai phiên bản của `size_t strlen(const char*)` sử dụng một vòng lặp và không có lệnh gọi hàm.
````C
size_t mystrlen(const char*s) {

}
````
## Q 1.11
Xác định ba lỗi trong triển khai `strcpy` sau.
````C
char* strcpy(const char* dest, const char* src) {
  while(*src) { *dest++ = *src++; }
  return dest;
}
````



# In
## Q 2.1
Phát hiện hai lỗi!
````C
fprintf("Bạn đã ghi được 100%");
````
# Định dạng và In ra tệp
## Q 3.1
Hoàn thành đoạn mã sau để in ra tệp. In tên, dấu phẩy và điểm số vào tệp 'result.txt'
````C
char* name = .....;
int score = ......;
FILE *f = fopen("result.txt",_____);
if(f) {
    _____
}
fclose(f);
````
# In ra một chuỗi
## Q 4.1

Làm cách nào để bạn in các giá trị của các biến `a`, `mesg`, `val` và `ptr` vào một chuỗi? In `a` dưới dạng số nguyên, `mesg` dưới dạng chuỗi C, `val` dưới dạng double `val` và `ptr` dưới dạng con trỏ thập lục phân. Bạn có thể giả định rằng `mesg` trỏ đến một chuỗi C ngắn (<50 ký tự).
Phần thưởng: Làm cách nào để bạn làm cho mã này mạnh mẽ hơn hoặc có thể đối phó với...?
```C
char* toString(int a, char*mesg, double val, void* ptr) {
   char* result = malloc( strlen(mesg) + 50);
    _____
   return result;
}
```

# Phân tích cú pháp đầu vào
## Q 5.1
Tại sao bạn nên kiểm tra giá trị trả về của `sscanf` và `scanf`?
## Q 5.2
Tại sao `gets` nguy hiểm?

## Q 5.3
Viết một chương trình hoàn chỉnh sử dụng `getline`. Đảm bảo chương trình của bạn không bị rò rỉ bộ nhớ.

## Bộ nhớ heap
Khi nào bạn sẽ sử dụng `calloc` thay vì `malloc`? 
Khi nào `realloc` sẽ hữu ích?

## Q 5.4
Lập trình viên đã mắc lỗi gì trong đoạn mã sau? Có thể sửa nó i) sử dụng bộ nhớ heap? ii) sử dụng bộ nhớ toàn cục (tĩnh)?
```C
static int id;

char* next_ticket() {
  id ++;
  char result[20];
  sprintf(result,"%d",id);
  return result;
}
```


