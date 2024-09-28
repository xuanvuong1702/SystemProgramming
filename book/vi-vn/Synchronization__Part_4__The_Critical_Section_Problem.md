# Các giải pháp ứng viên

## Vấn đề đoạn mã găng là gì?

Như đã thảo luận trong [[Đồng bộ hóa, Phần 3: Làm việc với Mutex và Semaphore]], có những phần quan trọng trong mã của chúng ta chỉ có thể được thực thi bởi một luồng tại một thời điểm. Chúng tôi mô tả yêu cầu này là 'loại trừ lẫn nhau'; chỉ một luồng (hoặc tiến trình) có thể truy cập vào tài nguyên được chia sẻ.

Trong các chương trình đa luồng, chúng ta có thể bao bọc một đoạn mã găng bằng các lệnh gọi khóa và mở khóa mutex:

```C
pthread_mutex_lock() - chỉ cho phép một luồng tại một thời điểm! (những luồng khác sẽ phải đợi ở đây)
... Thực hiện việc của Đoạn mã găng tại đây!
pthread_mutex_unlock() - cho phép các luồng đang chờ khác tiếp tục
```
Làm cách nào để chúng ta triển khai các lệnh gọi khóa và mở khóa này? Chúng ta có thể tạo một thuật toán đảm bảo loại trừ lẫn nhau không? Một triển khai không chính xác được hiển thị bên dưới, 
```C
pthread_mutex_lock(p_mutex_t *m)     { while(m->lock) {}; m->lock = 1;}
pthread_mutex_unlock(p_mutex_t *m)   { m->lock = 0; }
```


Thoạt nhìn, mã có vẻ hoạt động; nếu một luồng cố gắng khóa mutex, một luồng sau đó phải đợi cho đến khi khóa được giải phóng. Tuy nhiên, triển khai này _không thỏa mãn Loại trừ lẫn nhau_. Hãy xem xét kỹ hơn 'triển khai' này từ quan điểm của hai luồng chạy cùng một lúc. Trong bảng dưới đây, thời gian chạy từ trên xuống dưới-

Thời gian | Luồng 1 | Luồng 2
-----|----------|---------
1 | `while(lock) {}`
2 | | `while(lock) {} ` | 
3 | lock = 1 | lock = 1 |
Rất tiếc! Có một điều kiện tranh đua. Trong trường hợp không may, cả hai luồng đã kiểm tra khóa và đọc giá trị sai và do đó chúng ta không thể tiếp tục. 

## Các giải pháp ứng viên cho bài toán đoạn mã găng.
Để đơn giản hóa cuộc thảo luận, chúng tôi chỉ xem xét hai luồng. Lưu ý rằng các lập luận này hoạt động cho cả luồng và tiến trình và tài liệu CS cổ điển thảo luận về các vấn đề này dưới dạng hai tiến trình cần truy cập độc quyền (tức là loại trừ lẫn nhau) vào một đoạn mã găng hoặc tài nguyên được chia sẻ.

Nâng cờ biểu thị ý định của một luồng/tiến trình muốn vào đoạn mã găng.

Hãy nhớ rằng mã giả được nêu ra dưới đây là một phần của một chương trình lớn hơn; luồng hoặc tiến trình thường sẽ cần phải vào đoạn mã găng nhiều lần trong vòng đời của tiến trình. Vì vậy, hãy tưởng tượng mỗi ví dụ được bao bọc bên trong một vòng lặp, trong đó luồng hoặc tiến trình đang làm việc trên một thứ gì đó khác trong một khoảng thời gian ngẫu nhiên.

Có gì sai với giải pháp ứng viên được mô tả bên dưới không?
```
// Ứng viên #1
đợi cho đến khi cờ của bạn được hạ xuống
nâng cờ của tôi lên
// Thực hiện công việc của Đoạn mã găng
hạ cờ của tôi xuống
```

Trả lời: Giải pháp ứng viên #1 cũng gặp phải điều kiện tranh đua, tức là nó không thỏa mãn Loại trừ lẫn nhau vì cả hai luồng/tiến trình đều có thể đọc giá trị cờ của nhau (=được hạ xuống) và tiếp tục. 

Điều này cho thấy chúng ta nên nâng cờ _trước_ khi kiểm tra cờ của luồng khác - đó là giải pháp ứng viên #2 bên dưới.

```
// Ứng viên #2
nâng cờ của tôi lên
đợi cho đến khi cờ của bạn được hạ xuống
// Thực hiện công việc của Đoạn mã găng
hạ cờ của tôi xuống
```

Ứng viên #2 thỏa mãn loại trừ lẫn nhau - không thể có hai luồng ở bên trong đoạn mã găng cùng một lúc. Tuy nhiên, mã này bị bế tắc! Giả sử hai luồng muốn vào đoạn mã găng cùng một lúc:

Thời gian | Luồng 1 | Luồng 2
-----|----------|---------
1 | nâng cờ
2 | | nâng cờ
3 | đợi ... | đợi ...
Rất tiếc, cả hai luồng/tiến trình hiện đang đợi luồng kia hạ cờ của chúng. Không luồng nào sẽ vào đoạn mã găng vì cả hai hiện đang bị kẹt mãi mãi!

Điều này cho thấy chúng ta nên sử dụng một biến dựa trên lượt để cố gắng giải quyết ai nên tiếp tục. 

## Các giải pháp dựa trên lượt
Giải pháp ứng viên #3 sau đây sử dụng một biến dựa trên lượt để lịch sự cho phép một luồng và sau đó là luồng khác tiếp tục

```
// Ứng viên #3
đợi cho đến khi đến lượt của tôi là myid
// Thực hiện công việc của Đoạn mã găng
lượt = yourid
```
Ứng viên #3 thỏa mãn loại trừ lẫn nhau (mỗi luồng hoặc tiến trình được quyền truy cập độc quyền vào Đoạn mã găng), tuy nhiên cả hai luồng/tiến trình phải thực hiện cách tiếp cận dựa trên lượt nghiêm ngặt để sử dụng đoạn mã găng; tức là chúng bị buộc vào một mô hình truy cập đoạn mã găng luân phiên. Ví dụ: nếu luồng 1 muốn đọc bảng băm mỗi mili giây nhưng một luồng khác ghi vào bảng băm mỗi giây, thì luồng đọc sẽ phải đợi thêm 999ms trước khi có thể đọc lại từ bảng băm. 'Giải pháp' này không hiệu quả, vì các luồng của chúng ta sẽ có thể tiến hành và đi vào phần găng nếu không có luồng nào khác hiện đang ở trong phần găng.

## Các thuộc tính mong muốn cho các giải pháp cho bài toán đoạn mã găng?
Có ba thuộc tính mong muốn chính mà chúng ta mong muốn trong một giải pháp cho bài toán đoạn mã găng
* Loại trừ lẫn nhau - luồng/tiến trình được quyền truy cập độc quyền; những luồng/tiến trình khác phải đợi cho đến khi nó thoát khỏi đoạn mã găng.
* Chờ đợi bị chặn - nếu luồng/tiến trình phải đợi, thì nó chỉ nên đợi trong một khoảng thời gian hữu hạn (không cho phép thời gian chờ vô hạn!). Định nghĩa chính xác của chờ đợi bị chặn là có một giới hạn trên (không vô hạn) về số lần bất kỳ tiến trình nào khác có thể vào đoạn mã găng của nó trước khi tiến trình đã cho vào.
* Tiến trình - nếu không có luồng/tiến trình nào ở bên trong đoạn mã găng, thì luồng/tiến trình sẽ có thể tiếp tục (tiến hành) mà không phải đợi.


Với những ý tưởng này, hãy xem xét một giải pháp ứng viên khác sử dụng cờ dựa trên lượt chỉ khi cả hai luồng đều yêu cầu truy cập cùng một lúc.

## Giải pháp kết hợp lượt và cờ

Liệu sau đây có phải là giải pháp chính xác cho CSP không?
```
\\ Ứng viên #4
nâng cờ của tôi lên
nếu cờ của bạn được nâng lên, hãy đợi cho đến khi đến lượt của tôi
// Thực hiện công việc của Đoạn mã găng
lượt = yourid
hạ cờ của tôi xuống
```
Một người hướng dẫn và một giảng viên CS khác ban đầu đã nghĩ như vậy! Tuy nhiên, việc phân tích các giải pháp này rất phức tạp. Ngay cả các bài báo được đánh giá ngang hàng về chủ đề cụ thể này cũng chứa các giải pháp không chính xác! Thoạt nhìn, nó có vẻ thỏa mãn Loại trừ lẫn nhau, Chờ đợi bị chặn và Tiến trình: Cờ dựa trên lượt chỉ được sử dụng trong trường hợp hòa (vì vậy Tiến trình và Chờ đợi bị chặn được cho phép) và loại trừ lẫn nhau dường như được thỏa mãn. Tuy nhiên.... Có lẽ bạn có thể tìm thấy một ví dụ phản chứng?

Ứng viên #4 thất bại vì một luồng không đợi cho đến khi luồng kia hạ cờ của chúng. Sau khi suy nghĩ (hoặc lấy cảm hứng), kịch bản sau đây có thể được tạo ra để chứng minh cách Loại trừ lẫn nhau không được thỏa mãn.

Hãy tưởng tượng luồng đầu tiên chạy mã này hai lần (vì vậy cờ lượt bây giờ trỏ đến luồng thứ hai). Trong khi luồng đầu tiên vẫn ở bên trong Đoạn mã găng, luồng thứ hai đến. Luồng thứ hai có thể ngay lập tức tiếp tục vào Đoạn mã găng!

Thời gian | Lượt | Luồng #1 | Luồng #2
-----|------|-----------|----------
1| 2 | nâng cờ của tôi lên | 
2| 2 | nếu cờ của bạn được nâng lên, hãy đợi cho đến khi đến lượt của tôi | nâng cờ của tôi lên 
3| 2 | // Thực hiện công việc của Đoạn mã găng | nếu cờ của bạn được nâng lên, hãy đợi cho đến khi đến lượt của tôi (ĐÚNG!)
4| 2 | // Thực hiện công việc của Đoạn mã găng | // Thực hiện công việc của Đoạn mã găng - Ôi!

# Các giải pháp hoạt động

## Giải pháp của Peterson là gì?

Peterson đã công bố giải pháp mới lạ và đáng ngạc nhiên đơn giản của mình trong một bài báo dài 2 trang vào năm 1981. 
Một phiên bản của thuật toán của ông được hiển thị bên dưới, sử dụng biến chia sẻ `turn`:

```
\\ Ứng viên #5
nâng cờ của tôi lên
lượt = your_id
đợi trong khi cờ của bạn được nâng lên và lượt là your_id
// Thực hiện công việc của Đoạn mã găng
hạ cờ của tôi xuống
```

Phiên bản giả C tương đương ở bên dưới.
````C
\\ Ứng viên #5
bool flag1, flag2  //cả hai ban đầu đều sai
int turn = 1

luồng1:                          luồng2:
  flag1 = true                      flag2 = true
  turn = 2                          turn = 1
  while(flag2 && turn == 2) {}      while(flag1 && turn == 1) {}
  Đoạn mã găng                  Đoạn mã găng
  flag1 = false                     flag2 = false
````
Giải pháp này thỏa mãn Loại trừ lẫn nhau, Chờ đợi bị chặn và Tiến trình. Nếu luồng #2 đã đặt lượt thành 1 và hiện đang ở bên trong đoạn mã găng. Luồng #1 đến, _đặt lượt trở lại 2_ và bây giờ đợi cho đến khi luồng 2 hạ cờ. 

Một phiên bản giống C của triển khai Peterson được thảo luận tại Wikipedia [tại đây](https://en.wikipedia.org/wiki/Peterson%27s_algorithm#The_algorithm).

Liên kết đến tệp pdf bài báo gốc của Peterson:
[G. L. Peterson: "Myths About the Mutual Exclusion Problem", Information Processing Letters 12(3) 1981, 115–116](http://dl.acm.org/citation.cfm?id=945527)

## Giải pháp của Peterson có phải là giải pháp đầu tiên không?

Không, Thuật toán của Dekker (1962) là giải pháp chính xác có thể chứng minh đầu tiên. Một phiên bản của thuật toán ở bên dưới.
```
nâng cờ của tôi lên
while(cờ của bạn được nâng lên) :
   nếu đến lượt bạn thắng :
     hạ cờ của tôi xuống
     đợi trong khi đến lượt của bạn
     nâng cờ của tôi lên
// Thực hiện công việc của Đoạn mã găng
đặt lượt của bạn để thắng
hạ cờ của tôi xuống
```

Lưu ý cách cờ của tiến trình luôn được nâng lên trong suốt đoạn mã găng, bất kể vòng lặp được lặp lại 0, một lần hay nhiều lần. Hơn nữa, cờ có thể được hiểu là một ý định ngay lập tức để vào đoạn mã găng. Chỉ khi tiến trình kia cũng nâng cờ, một tiến trình sẽ trì hoãn, hạ cờ ý định của chúng xuống và đợi.

## Tôi có thể chỉ cần triển khai thuật toán của Peterson (hoặc Dekker) trong C hoặc hợp ngữ không?
Có - và với một chút tìm kiếm, thậm chí ngày nay có thể tìm thấy nó trong sản xuất cho các bộ xử lý di động đơn giản cụ thể: Thuật toán của Peterson được sử dụng để triển khai các khóa Kernel Linux cấp thấp cho bộ xử lý di động Tegra (một tiến trình ARM hệ thống trên chip và lõi GPU của Nvidia)
https://android.googlesource.com/kernel/tegra.git/+/android-tegra-3.10/arch/arm/mach-tegra/sleep.S#58

Tuy nhiên, nhìn chung, CPU và trình biên dịch C có thể sắp xếp lại các lệnh CPU hoặc sử dụng các giá trị bộ nhớ đệm cục bộ cụ thể của lõi CPU đã cũ nếu một lõi khác cập nhật các biến được chia sẻ. Do đó, một mã giả đơn giản để triển khai C là quá ngây thơ đối với hầu hết các nền tảng. Bạn có thể ngừng đọc ngay bây giờ.

Ồ... bạn quyết định tiếp tục đọc. Chà, đây là những con rồng! Đừng nói rằng chúng tôi đã không cảnh báo bạn. Hãy coi đây là chủ đề nâng cao và khó nhằn nhưng (cảnh báo spoiler) là một kết thúc có hậu.

Xem xét mã sau,

```C
while(flag2 ) { } /* vòng lặp bận - lặp lại */
```
Một trình biên dịch hiệu quả sẽ suy ra rằng biến `flag2` không bao giờ bị thay đổi bên trong vòng lặp, vì vậy phép thử đó có thể được tối ưu hóa thành `while(true)` 
Sử dụng `volatile` phần nào ngăn chặn tối ưu hóa trình biên dịch thuộc loại này.

Các lệnh độc lập có thể được sắp xếp lại bởi trình biên dịch tối ưu hóa hoặc trong thời gian chạy bởi tối ưu hóa thực thi ngoài trình tự bởi CPU. Những tối ưu hóa phức tạp này nếu mã yêu cầu các biến được sửa đổi và kiểm tra và theo thứ tự chính xác.

Một thách thức liên quan là các lõi CPU bao gồm bộ nhớ đệm dữ liệu để lưu trữ các giá trị bộ nhớ chính được đọc hoặc sửa đổi gần đây. Các giá trị đã sửa đổi có thể không được ghi lại vào bộ nhớ chính hoặc đọc lại từ bộ nhớ ngay lập tức. Do đó, các thay đổi dữ liệu, chẳng hạn như trạng thái của cờ và biến lượt trong các ví dụ trên, có thể không được chia sẻ giữa hai mã CPU. 

Nhưng có một kết thúc có hậu. May mắn thay, phần cứng hiện đại giải quyết các vấn đề này bằng cách sử dụng các lệnh CPU 'hàng rào bộ nhớ' (còn được gọi là rào cản bộ nhớ) để đảm bảo rằng bộ nhớ chính và bộ nhớ đệm của CPU ở trạng thái hợp lý và nhất quán. Các nguyên thủy đồng bộ hóa cấp cao hơn, chẳng hạn như `pthread_mutex_lock` sẽ gọi các lệnh CPU này như một phần của việc triển khai chúng. Do đó, trong thực tế, việc bao quanh đoạn mã găng bằng các lệnh gọi khóa và mở khóa mutex là đủ để bỏ qua các vấn đề cấp thấp hơn này.

Đọc thêm nâng cao: chúng tôi đề xuất bài đăng trên web sau đây thảo luận về việc triển khai thuật toán của Peterson trên một quy trình x86 và tài liệu linux về rào cản bộ nhớ.

http://bartoszmilewski.com/2008/11/05/who-ordered-memory-fences-on-an-x86/

https://elixir.bootlin.com/linux/latest/source/Documentation/memory-barriers.txt

# Các giải pháp phần cứng

## Bạn sẽ triển khai Bài toán đoạn mã găng trong phần cứng như thế nào?

Trên một máy CPU đơn giản, một tiến trình có quyền truy cập vào tất cả các lệnh CPU có thể tạm thời tắt ngắt.
```C
tắt_ngắt
// Mã đoạn mã găng
bật_ngắt
```
Nếu ngắt bị tắt thì luồng hiện tại không thể bị ngắt! tức là các lệnh CPU của đoạn mã găng sẽ hoàn thành.

Tuy nhiên, hầu hết các hệ thống ngày nay đều có nhiều hơn một lõi CPU và việc tắt ngắt là một lệnh đặc quyền - vì vậy kỹ thuật trên hiếm khi phù hợp.

Thay vào đó, giả sử CPU cung cấp cho chúng ta một lệnh nguyên tử đặc biệt `__exch` hoán đổi các giá trị tại hai vị trí bộ nhớ. Chúng ta có thể hỗ trợ các Đoạn mã găng bằng cách triển khai các khóa mutex bằng cách sử dụng mã giả sau.
````C
my_mutex_init(int* m)  { *m= 0; }

my_mutex_lock(int* m) {
  for(int q = 1; q ; ) {  __exch(&m , &q); }
} // khi điều này trả về, bạn có thể an toàn vào đoạn mã găng của mình

// Sau khi phần găng của bạn kết thúc, hãy gọi unlock...
my_mutex_unlock(int* m)  { *m= 0; }
````

Lệnh trao đổi phải là nguyên tử, tức là nó hoạt động như một lệnh __không thể bị gián đoạn__ và không thể chia cắt duy nhất. Ví dụ: nếu hai luồng đều gọi `my_mutex_lock` (và sau đó là __exch) cùng một lúc, thì một luồng _sẽ_ nhận được giá trị 0 và luồng kia sẽ thua và nhận giá trị mới hơn là 1 (vì vậy sẽ tiếp tục thăm dò).


## Làm cách nào để chúng ta thực sự triển khai Bài toán đoạn mã găng trên phần cứng thực? (Chủ đề nâng cao)

Một giải pháp hoàn chỉnh sử dụng nguyên tử C11 được trình bày chi tiết ở đây (đây là mutex spinlock, các triển khai [futex](https://locklessinc.com/articles/mutex_cv_futex/) có thể được tìm thấy trực tuyến).

```C
typedef struct mutex_{
    atomic_int_least8_t lock;
    pthread_t owner;
} mutex;

#define UNLOCKED 0
#define LOCKED 1
#define UNASSIGNED_OWNER 0

int mutex_init(mutex* mtx){
    if(!mtx){
        return 0;
    }
    atomic_init(&mtx->lock, UNLOCKED); // Không an toàn cho luồng, người dùng phải tự lo điều này
    mtx->owner = UNASSIGNED_OWNER;
    return 1;
}
```

Đây là mã khởi tạo, không có gì lạ mắt ở đây. Chúng tôi đặt trạng thái của mutex thành đã mở khóa và đặt chủ sở hữu thành đã khóa.

```C
int mutex_lock(mutex* mtx){
    int_least8_t zero = UNLOCKED;
    while(!atomic_compare_exchange_weak_explicit
            (&mtx->lock, 
             &zero, 
             LOCKED,
             memory_order_acq_rel,
             memory_order_relaxed)){
        zero = UNLOCKED;
        sched_yield(); //Sử dụng lệnh gọi hệ thống để lên lịch tốc độ
    }
    //Chúng ta có khóa ngay bây giờ!!!!
    mtx->owner = pthread_self();
    return 1;
}
```
Ôi trời! Đoạn mã này làm gì? Chà, để bắt đầu, nó khởi tạo một biến mà chúng ta sẽ giữ nguyên trạng thái đã mở khóa. [Atomic Compare and Exchange](https://en.wikipedia.org/wiki/Compare-and-swap) là một lệnh được hỗ trợ bởi hầu hết các kiến ​​trúc hiện đại (trên x86, đó là `lock cmpxchg`). Mã giả cho thao tác này trông như thế này

```C
int atomic_compare_exchange_pseudo(int* addr1, int* addr2, int val){
    if(*addr1 == *addr2){
        *addr1 = val;
        return 1;
    }else{
        *addr2 = *addr1;
        return 0;
    }
}
```
Ngoại trừ tất cả được thực hiện _nguyên tử_, có nghĩa là trong một thao tác không thể bị gián đoạn. Phần _yếu_ có nghĩa là gì? Chà, các lệnh nguyên tử cũng dễ bị **lỗi giả mạo**, có nghĩa là có hai phiên bản cho các hàm nguyên tử này, một phần _mạnh_ và một phần _yếu_, phần mạnh đảm bảo thành công hoặc thất bại trong khi phần yếu có thể thất bại. Chúng tôi đang sử dụng yếu vì yếu nhanh hơn và chúng tôi đang ở trong một vòng lặp! Điều đó có nghĩa là chúng ta ổn nếu nó thất bại thường xuyên hơn một chút vì dù sao chúng ta cũng sẽ tiếp tục quay vòng.

Thứ tự bộ nhớ này là gì? Chúng ta đã nói về hàng rào bộ nhớ trước đó, nó đây rồi! Chúng tôi sẽ không đi sâu vào chi tiết vì nó nằm ngoài phạm vi của khóa học này nhưng không phải là phạm vi của [bài viết này](https://gcc.gnu.org/wiki/Atomic/GCCMM/AtomicSync).

Bên trong vòng lặp while, chúng ta đã không lấy được khóa! Chúng tôi đặt lại 0 thành đã mở khóa và ngủ một lúc. Khi chúng ta thức dậy, chúng ta lại cố gắng lấy khóa. Khi chúng ta trao đổi thành công, chúng ta đang ở trong phần găng! Chúng tôi đặt chủ sở hữu của mutex thành luồng hiện tại cho phương thức mở khóa và trả về thành công.

Làm thế nào điều này đảm bảo loại trừ lẫn nhau, khi làm việc với nguyên tử, chúng tôi không hoàn toàn chắc chắn! Nhưng trong ví dụ đơn giản này, chúng ta có thể bởi vì luồng có thể mong đợi thành công khóa là UNLOCKED (0) và hoán đổi nó sang trạng thái LOCKED (1) được coi là người chiến thắng. Làm cách nào để chúng ta triển khai unlock?

```C
int mutex_unlock(mutex* mtx){
    if(unlikely(pthread_self() != mtx->owner)){
        return 0; //Bạn không thể mở khóa mutex nếu bạn không phải là chủ sở hữu
    }
    int_least8_t one = 1;
    mtx->owner = UNASSIGNED_OWNER;
    //Đoạn mã găng kết thúc sau nguyên tử này
    //Ngoài ra, điều này có thể thất bại, nhưng điều đó không sao
    if(!atomic_compare_exchange_strong_explicit(
                &mtx->lock, 
                &one, 
                UNLOCKED,
                memory_order_acq_rel,
                memory_order_relaxed)){
        //Mutex chưa bao giờ bị khóa ngay từ đầu
        return 0;
    }
    return 1;
}
```

Để đáp ứng api, bạn không thể mở khóa mutex trừ khi bạn là người sở hữu nó. Sau đó, chúng tôi hủy gán chủ sở hữu mutex, bởi vì phần găng đã kết thúc sau nguyên tử. Chúng tôi muốn có một cuộc trao đổi mạnh mẽ vì chúng tôi không muốn chặn (pthread_mutex_unlock không chặn). Chúng tôi mong đợi mutex bị khóa và chúng tôi hoán đổi nó để mở khóa. Nếu việc hoán đổi thành công, chúng tôi đã mở khóa mutex. Nếu việc hoán đổi không thành công, điều đó có nghĩa là mutex đã được MỞ KHÓA và chúng tôi đã cố gắng chuyển nó từ MỞ KHÓA sang MỞ KHÓA, giữ nguyên trạng thái không chặn của mở khóa.


