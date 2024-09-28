# Tổng quan

Phần tiếp theo đề cập đến những gì xảy ra khi các pthread va chạm, nhưng nếu chúng ta có mỗi luồng làm một việc hoàn toàn khác, không chồng chéo thì sao?

Chúng ta đã tìm thấy tốc độ tối đa cho các bài toán song song?

## Các bài toán song song dễ dàng (Embarrassingly Parallel Problems)

Nghiên cứu về các thuật toán song song đã bùng nổ trong vài năm qua. Một bài toán song song dễ dàng là bất kỳ bài toán nào cần ít nỗ lực để biến thành song song. Rất nhiều trong số chúng có một số khái niệm đồng bộ hóa nhưng không phải lúc nào cũng vậy. Bạn đã biết một thuật toán có thể song song hóa, Merge Sort!

```C
void merge_sort(int *arr, size_t len){
     if(len > 1){
     //Sắp xếp trộn nửa bên trái
     //Sắp xếp trộn nửa bên phải
     //Trộn hai nửa
     }
```

Với hiểu biết mới của bạn về luồng, tất cả những gì bạn cần làm là tạo một luồng cho nửa bên trái và một luồng cho nửa bên phải. Giả sử CPU của bạn có nhiều lõi thực, bạn sẽ thấy tốc độ tăng theo [Định luật Amdahl](https://en.wikipedia.org/wiki/Amdahl's_law). Phân tích độ phức tạp thời gian cũng trở nên thú vị ở đây. Thuật toán song song chạy trong thời gian chạy O(log^3(n)) (vì chúng ta phân tích dựa trên giả định rằng chúng ta có rất nhiều lõi).

Tuy nhiên, trong thực tế, chúng ta thường thực hiện hai thay đổi. Thứ nhất, một khi mảng trở nên đủ nhỏ, chúng ta bỏ thuật toán sắp xếp trộn song song và thực hiện quicksort hoặc thuật toán khác hoạt động nhanh trên các mảng nhỏ (liên quan đến tính kết hợp bộ nhớ đệm). Điều khác mà chúng ta biết là CPU không có lõi vô hạn. Để giải quyết vấn đề đó, chúng ta thường sử dụng một nhóm worker.

## Nhóm Worker (Worker Pool)

Chúng ta biết rằng CPU có một số lượng lõi hữu hạn. Rất nhiều lần chúng ta khởi động một số luồng và giao nhiệm vụ cho chúng khi chúng rảnh rỗi.

## Một bài toán khác, Ánh xạ song song (Parallel Map)

Giả sử chúng ta muốn áp dụng một hàm cho toàn bộ mảng, từng phần tử một.

```C

int *map(int (*func)(int), int *arr, size_t len){
    int *ret = malloc(len*sizeof(*arr));
    for(size_t i = 0; i < len; ++i) 
        ret[i] = func(arr[i]);
    return ret;
}
```

Vì không có phần tử nào phụ thuộc vào bất kỳ phần tử nào khác, bạn sẽ làm thế nào để song song hóa điều này? Bạn nghĩ cách nào là tốt nhất để phân chia công việc giữa các luồng?

## Lập lịch (Scheduling)

Có một số cách để phân chia công việc.
* **Lập lịch tĩnh**: chia các bài toán thành các khối có kích thước cố định (được xác định trước) và để mỗi luồng làm việc trên mỗi khối. Điều này hoạt động tốt khi mỗi bài toán con mất gần cùng một thời gian vì không có thêm chi phí. Tất cả những gì bạn cần làm là viết một vòng lặp và cung cấp hàm ánh xạ cho mỗi mảng con.
* **Lập lịch động**: khi một bài toán mới xuất hiện, hãy để một luồng phục vụ nó. Điều này rất hữu ích khi bạn không biết việc lập lịch sẽ mất bao lâu.
* **Lập lịch hướng dẫn**: Đây là sự kết hợp của hai cách trên với sự kết hợp của các lợi ích và sự đánh đổi. Bạn bắt đầu với một lịch trình tĩnh và chuyển sang động nếu cần.
* **Lập lịch thời gian chạy**: Bạn hoàn toàn không biết các bài toán sẽ mất bao lâu. Thay vì tự quyết định, hãy để chương trình quyết định phải làm gì!

[nguồn](https://software.intel.com/en-us/articles/openmp-loop-scheduling), nhưng không cần ghi nhớ.

## Một vài nhược điểm

Bạn sẽ không thấy tốc độ tăng ngay lập tức vì những thứ như tính kết hợp bộ nhớ đệm và lập lịch thêm luồng.

## Các bài toán khác

Từ [Wikipedia](https://en.wikipedia.org/wiki/Embarrassingly_parallel)
* Phục vụ các tệp tĩnh trên máy chủ web cho nhiều người dùng cùng một lúc.
* Tập hợp Mandelbrot, nhiễu Perlin và các hình ảnh tương tự, trong đó mỗi điểm được tính toán độc lập.
* Kết xuất đồ họa máy tính. Trong hoạt hình máy tính, mỗi khung hình có thể được kết xuất độc lập (xem kết xuất song song).
* Tìm kiếm vét cạn trong mật mã. [8] Các ví dụ thực tế đáng chú ý bao gồm distributed.net và các hệ thống chứng minh công việc được sử dụng trong tiền điện tử.
* Tìm kiếm BLAST trong tin sinh học cho nhiều truy vấn (nhưng không phải cho các truy vấn lớn riêng lẻ) [9]
* Các hệ thống nhận dạng khuôn mặt quy mô lớn so sánh hàng nghìn khuôn mặt thu được tùy ý (ví dụ: video bảo mật hoặc giám sát qua truyền hình mạch kín) với số lượng lớn các khuôn mặt được lưu trữ trước đó (ví dụ: thư viện ảnh hoặc danh sách theo dõi tương tự). [10]
* Mô phỏng máy tính so sánh nhiều kịch bản độc lập, chẳng hạn như mô hình khí hậu.
* Siêu học tính toán tiến hóa chẳng hạn như thuật toán di truyền.
* Tính toán tập hợp của dự đoán thời tiết số.
* Mô phỏng và tái tạo sự kiện trong vật lý hạt.
* Thuật toán marching squares.
* Bước sàng lọc của sàng bậc hai và sàng trường số.
* Bước phát triển cây của kỹ thuật học máy rừng ngẫu nhiên.
* Biến đổi Fourier rời rạc trong đó mỗi sóng hài được tính toán độc lập.


