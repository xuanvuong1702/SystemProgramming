# Tổng quan

Phần tiếp theo sẽ đề cập đến những gì xảy ra khi các pthreads xung đột, nhưng nếu chúng ta để mỗi luồng thực hiện một công việc hoàn toàn khác nhau, không có sự chồng chéo?

Chúng ta đã tìm ra tốc độ tăng tối đa cho các vấn đề song song?

## Các vấn đề Song song một cách Xấu hổ

Nghiên cứu về thuật toán song song đã bùng nổ trong vài năm qua. Một vấn đề song song một cách xấu hổ là bất kỳ vấn đề nào cần ít nỗ lực để chuyển sang song song. Rất nhiều vấn đề như vậy có một số khái niệm đồng bộ hóa đi kèm nhưng không phải lúc nào cũng vậy. Bạn đã biết một thuật toán có thể song song hóa, Merge Sort!

```C
void merge_sort(int *arr, size_t len){
    if(len > 1){
    //Sắp xếp nửa bên trái
    //Sắp xếp nửa bên phải
    //Hợp nhất hai nửa
    }
```

Với sự hiểu biết mới về luồng, tất cả những gì bạn cần làm là tạo một luồng cho nửa bên trái và một luồng cho nửa bên phải. Cho rằng CPU của bạn có nhiều lõi thực sự, bạn sẽ thấy tốc độ tăng theo [Luật Amdahl](https://vi.wikipedia.org/wiki/Lu%E1%BA%ADt_Amdahl). Phân tích độ phức tạp thời gian cũng trở nên thú vị ở đây. Thuật toán song song chạy trong thời gian O(log^3(n)) (vì chúng ta giả sử rằng chúng ta có rất nhiều lõi.

Trên thực tế, chúng ta thường thực hiện hai thay đổi. Một, một khi mảng trở nên nhỏ đủ, chúng ta bỏ thuật toán mergesort song song và thực hiện một thuật toán quicksort hoặc thuật toán khác hoạt động nhanh trên các mảng nhỏ (có gì đó về sự nhất quán của bộ nhớ đệm). Điều khác mà chúng ta biết là CPU không có số lõi vô hạn. Để vượt qua điều đó, chúng ta thường giữ một bể công nhân.

## Bể Công Nhân

Chúng ta biết rằng CPU có một số lượng lõi hữu hạn. Rất nhiều lần chúng ta khởi động một số luồng và giao cho chúng các tác vụ khi chúng rảnh rỗi.

## Một vấn đề khác, Bản đồ Song song

Giả sử chúng ta muốn áp dụng một hàm cho toàn bộ mảng, một phần tử mỗi lần.

```C
int *map(int (*func)(int), int *arr, size_t len){
    int *ret = malloc(len*sizeof(*arr));
    for(size_t i = 0; i < len; ++i) 
        ret[i] = func(arr[i]);
    return ret;
}
```

Vì không có phần tử nào phụ thuộc vào phần tử khác, bạn sẽ tiếp cận như thế nào để song song hóa điều này? Theo bạn, cách tốt nhất để chia công việc giữa các luồng là gì?

## Lập lịch

Có một số cách để chia công việc.
* Lập lịch tĩnh: chia các vấn đề thành các khối cố định (được xác định trước) và để mỗi luồng làm việc trên mỗi khối. Điều này hoạt động tốt khi mỗi vấn đề con mất thời gian gần như nhau vì không có chi phí phát sinh thêm. Tất cả những gì bạn cần làm là viết một vòng lặp và cung cấp hàm map cho mỗi mảng con.
* Lập lịch động: khi một vấn đề mới trở nên sẵn sàng, hãy để một luồng phục vụ nó. Điều này hữu ích khi bạn không biết lập lịch sẽ mất bao lâu
* Lập lịch hướng dẫn: Đây là sự kết hợp của hai phương pháp trên với sự kết hợp của các lợi ích và các thỏa hiệp. Bạn bắt đầu với lập lịch tĩnh và chuyển dần sang lập lịch động nếu cần
* Lập lịch thời gian chạy: Bạn hoàn toàn không biết các vấn đề sẽ mất bao lâu. Thay vì tự quyết định, hãy để chương trình quyết định những gì cần làm!

[Nguồn](https://software.intel.com/en-us/articles/openmp-loop-scheduling), nhưng không cần nhớ.

## Một số nhược điểm

Bạn sẽ không thấy tốc độ tăng ngay lập tức vì những thứ như sự nhất quán của bộ nhớ đệm và lập lịch các luồng phụ.

## Các vấn đề khác

Từ [Wikipedia](https://en.wikipedia.org/wiki/Embarrassingly_parallel)
* Phục vụ các tệp tĩnh trên một máy chủ web cho nhiều người dùng cùng một lúc.
* Tập hợp Mandelbrot, nhiễu Perlin và các hình ảnh tương tự, nơi mỗi điểm được tính toán độc lập.
* Hiển thị đồ họa máy tính. Trong hoạt hình máy tính, mỗi khung có thể được hiển thị độc lập (xem hiển thị song song).
* Tìm kiếm bằng cách dùng lực trong mật mã học. Ví dụ thực tế đáng chú ý bao gồm distributed.net và hệ thống chứng minh công việc được sử dụng trong tiền điện tử.
* Tìm kiếm BLAST trong sinh thông tin cho nhiều truy vấn (nhưng không phải cho các truy vấn lớn riêng lẻ)
* Hệ thống nhận dạng khuôn mặt quy mô lớn so sánh hàng nghìn khuôn mặt tùy ý thu được (ví dụ, video an ninh hoặc giám sát qua truyền hình mạch kín) với số lượng lớn khuôn mặt đã lưu trữ trước đó (ví dụ, một danh sách người phạm tội hoặc danh sách giám sát tương tự).
* Mô phỏng máy tính so sánh nhiều kịch bản độc lập, như mô hình khí hậu.
* Thuật toán tiến hóa như thuật toán di truyền.
* Tính toán tập hợp của dự báo thời tiết số.
* Mô phỏng sự kiện và tái tạo trong vật lý hạt.
* Thuật toán marching squares
* Bước lọc của quadratic sieve và number field sieve.
* Bước phát triển cây của kỹ thuật học máy rừng ngẫu nhiên.
* Biến đổi Fourier rời rạc nơi mỗi hài âm được tính toán độc lập.