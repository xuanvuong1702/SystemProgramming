## Bài toán Người đọc - Người viết là gì?

Hãy tưởng tượng bạn có một cấu trúc dữ liệu ánh xạ khóa-giá trị được sử dụng bởi nhiều luồng. Nhiều luồng sẽ có thể tra cứu (đọc) các giá trị cùng một lúc miễn là cấu trúc dữ liệu không bị ghi vào. Người viết không hòa đồng lắm - để tránh hỏng dữ liệu, chỉ có một luồng tại một thời điểm có thể sửa đổi (`viết`) cấu trúc dữ liệu (và không có người đọc nào có thể đọc vào thời điểm đó).

Đây là một ví dụ về _Bài toán Người đọc - Người viết_. Cụ thể là làm thế nào chúng ta có thể đồng bộ hóa hiệu quả nhiều người đọc và người viết sao cho nhiều người đọc có thể đọc cùng nhau nhưng người viết được quyền truy cập độc quyền?

Một nỗ lực không chính xác được hiển thị bên dưới ("khóa" là viết tắt của `pthread_mutex_lock`):

## Nỗ lực #1

<table><tr><td>
<pre>
read() {
  khóa(&m)
  // thực hiện công việc đọc
  mở khóa(&m)
}
</pre>
</td><td>
<pre>
write() {
  khóa(&m)
  // thực hiện công việc viết
  mở khóa(&m)
}
</pre></td></tr></table>

Ít nhất nỗ lực đầu tiên của chúng ta không bị hỏng dữ liệu (người đọc phải đợi trong khi người viết đang viết và ngược lại)! Tuy nhiên, người đọc cũng phải đợi những người đọc khác. Vì vậy, hãy thử một triển khai khác..

## Nỗ lực #2:
<table><tr><td>
<pre>read() {
  while(writing) {/*quay vòng*/}
  reading = 1
  // thực hiện công việc đọc
  reading = 0
}
</pre>
</td><td>
<pre>
write() {
  while(reading || writing) {/*quay vòng*/}
  writing = 1
  // thực hiện công việc viết
  writing = 0
}
</pre></td></tr></table>

Nỗ lực thứ hai của chúng ta bị điều kiện tranh đua - hãy tưởng tượng nếu hai luồng đều gọi `read` và `write` (hoặc cả hai đều gọi write) cùng một lúc. Cả hai luồng sẽ có thể tiếp tục! Thứ hai, chúng ta có thể có nhiều người đọc và nhiều người viết, vì vậy hãy theo dõi tổng số người đọc hoặc người viết. Điều đó đưa chúng ta đến nỗ lực #3,


## Nỗ lực #3

Hãy nhớ rằng `pthread_cond_wait` thực hiện *Ba* hành động. Đầu tiên, nó mở khóa mutex một cách nguyên tử và sau đó ngủ (cho đến khi nó được đánh thức bởi `pthread_cond_signal` hoặc `pthread_cond_broadcast`). Thứ ba, luồng được đánh thức phải lấy lại khóa mutex trước khi trả về. Do đó, chỉ có một luồng có thể thực sự chạy bên trong đoạn mã găng được xác định bởi các phương thức khóa và mở khóa().

Triển khai #3 bên dưới đảm bảo rằng người đọc sẽ vào cond_wait nếu có bất kỳ người viết nào đang viết.
```C
read() {
    khóa(&m)
    while (writing)
        cond_wait(&cv, &m)
    reading++;

/* Đọc ở đây! */

    reading--;
    cond_signal(&cv);
    mở khóa(&m);
}
```
Tuy nhiên, chỉ có một người đọc tại một thời điểm có thể đọc vì ứng viên #3 không mở khóa mutex. Một phiên bản tốt hơn sẽ mở khóa trước khi đọc:
```C
read() {
    khóa(&m);
    while (writing)
        cond_wait(&cv, &m)
    reading++;
    mở khóa(&m)
/* Đọc ở đây! */
    khóa(&m)
    reading--;
    cond_signal(&cv);
    mở khóa(&m);
}
```
Điều này có nghĩa là người viết và người đọc có thể đọc và viết cùng một lúc không? Không! Trước hết, hãy nhớ rằng cond_wait yêu cầu luồng lấy lại khóa mutex trước khi trả về. Do đó, chỉ có một luồng có thể thực thi mã bên trong đoạn mã găng (được đánh dấu bằng **) tại một thời điểm!
```C
read() {
    khóa(&m);
**  while (writing)
**      cond_wait(&cv, &m)
**  reading++;
    mở khóa(&m)
/* Đọc ở đây! */
    khóa(&m)
**  reading--;
**  cond_signal(&cv);
    mở khóa(&m);
}
```


Người viết phải đợi mọi người. Loại trừ lẫn nhau được đảm bảo bởi khóa.
```C
write() {
    khóa(&m);
**  while (reading || writing)
**      cond_wait(&cv, &m);
**  writing++;
**
** /* Viết ở đây! */
**  writing--;
**  cond_signal(&cv);
    mở khóa(&m);
}
```

Ứng viên #3 ở trên cũng sử dụng `pthread_cond_signal`; điều này sẽ chỉ đánh thức một luồng. Ví dụ: nếu nhiều người đọc đang đợi người viết hoàn thành thì chỉ có một người đọc đang ngủ sẽ được đánh thức khỏi giấc ngủ của họ. Người đọc và người viết nên sử dụng `cond_broadcast` để tất cả các luồng nên thức dậy và kiểm tra điều kiện vòng lặp while của chúng.


## Người viết bị bỏ đói

Ứng viên #3 ở trên bị bỏ đói. Nếu người đọc liên tục đến thì người viết sẽ không bao giờ có thể tiếp tục (số lượng 'đọc' không bao giờ giảm xuống 0). Điều này được gọi là *bỏ đói* và sẽ được phát hiện khi tải nặng. Cách khắc phục của chúng ta là triển khai một giới hạn chờ cho người viết. Nếu người viết đến, họ vẫn sẽ cần đợi những người đọc hiện tại, tuy nhiên, những người đọc trong tương lai phải được đặt trong "khu vực chờ" và đợi người viết kết thúc. "Khu vực chờ" có thể được triển khai bằng cách sử dụng một biến và một biến điều kiện (để chúng ta có thể đánh thức các luồng sau khi người viết kết thúc).

Kế hoạch của chúng ta là khi người viết đến, và trước khi chờ những người đọc hiện tại kết thúc, hãy đăng ký ý định viết của chúng ta (bằng cách tăng bộ đếm 'writer'). Phác thảo bên dưới -

```C
write() {
    khóa()
    writer++

    while (reading || writing)
        cond_wait
    mở khóa()
  ...
}
```

Và những người đọc đến sẽ không được phép tiếp tục trong khi `writer` khác 0. Lưu ý 'writer' cho biết người viết đã đến, trong khi bộ đếm 'reading' và 'writing' cho biết có người đọc hoặc người viết _đang hoạt động_.
```C
read() {
    khóa()
    // những người đọc đến *sau* khi người viết đến sẽ phải đợi ở đây!
    while(writer)
        cond_wait(&cv,&m)

    // những người đọc đến trong khi có người viết đang hoạt động
    // cũng sẽ đợi.
    while (writing) 
        cond_wait(&cv,&m)
    reading++
    mở khóa
  ...
}
```

## Nỗ lực #4
Dưới đây là giải pháp hoạt động đầu tiên của chúng tôi cho bài toán Người đọc - Người viết.
Lưu ý rằng nếu bạn tiếp tục đọc về "Bài toán Người đọc - Người viết" thì bạn sẽ phát hiện ra rằng chúng tôi đã giải quyết "Bài toán Người đọc - Người viết thứ hai" bằng cách ưu tiên người viết truy cập vào khóa. Giải pháp này không phải là tối ưu. Tuy nhiên, nó thỏa mãn vấn đề ban đầu của chúng ta (N người đọc đang hoạt động, một người viết đang hoạt động, tránh bỏ đói người viết nếu có một luồng người đọc liên tục).

Bạn có thể xác định bất kỳ cải tiến nào không? Ví dụ: bạn sẽ cải thiện mã như thế nào để chúng ta chỉ đánh thức người đọc hoặc một người viết? 
```C

int writers; // Số lượng luồng người viết muốn vào đoạn mã găng (một số hoặc tất cả những luồng này có thể bị chặn)
int writing; // Số lượng luồng đang thực sự viết bên trong Đoạn mã găng (chỉ có thể là 0 hoặc 1)
int reading; // Số lượng luồng đang thực sự đọc bên trong Đoạn mã găng
// nếu writing !=0 thì reading phải bằng 0 (và ngược lại)

reader() {
    khóa(&m)
    while (writers)
        cond_wait(&turn, &m)
    // Không cần phải đợi trong khi (viết ở đây) vì chúng ta chỉ có thể thoát khỏi vòng lặp ở trên
    // khi writing bằng 0
    reading++
    mở khóa(&m)

  // thực hiện đọc ở đây

    khóa(&m)
    reading--;
    cond_broadcast(&turn);
    mở khóa(&m);
}

writer() {
    khóa(&m)  
    writers++  
    while (reading || writing)   
        cond_wait(&turn, &m)  
    writing++  
    mở khóa(&m)  
    // thực hiện viết ở đây  
    khóa(&m)  
    writing--  
    writers--  
    cond_broadcast(&turn)  
    mở khóa(&m)  
}
```

