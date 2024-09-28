# Chủ đề
* Thao tác nguyên tử
* Đoạn mã găng 
* Bài toán Nhà sản xuất - Người tiêu dùng
* Sử dụng Biến điều kiện
* Sử dụng Semaphore đếm
* Triển khai rào cản
* Triển khai bộ đệm vòng
* Sử dụng pthread_mutex
* Triển khai nhà sản xuất - người tiêu dùng
* Phân tích mã đa luồng

# Câu hỏi
* Thao tác nguyên tử là gì?
* Tại sao đoạn mã sau đây không hoạt động trong mã song song
```C
//Trong phần toàn cục
size_t a;
//Trong hàm pthread
for(int i = 0; i < 100000000; i++) a++;
```
Và đoạn mã này thì hoạt động?
```C
//Trong phần toàn cục
atomic_size_t a;
//Trong hàm pthread
for(int i = 0; i < 100000000; i++) atomic_fetch_add(&a, 1);
```
* Một số nhược điểm của thao tác nguyên tử là gì? Điều gì sẽ nhanh hơn: giữ một biến cục bộ hay nhiều thao tác nguyên tử?
* Đoạn mã găng là gì?
* Khi bạn đã xác định được một đoạn mã găng, có cách nào để đảm bảo rằng chỉ có một luồng sẽ ở trong đoạn mã đó tại một thời điểm?
* Xác định đoạn mã găng ở đây

```C
struct linked_list;
struct node;
void add_linked_list(linked_list *ll, void* elem){
    node* packaged = new_node(elem);
    if(ll->head){
         ll->head = 
    }else{
         packaged->next = ll->head;
         ll->head = packaged;
         ll->size++;
    }
    
}

void* pop_elem(linked_list *ll, size_t index){
    if(index >= ll->size) return NULL;
    
    node *i, *prev;
    for(i = ll->head; i && index; i = i->next, index--){
        prev = i;
    }

    //i trỏ đến phần tử chúng ta cần lấy ra, prev trước đó
    if(prev->next) prev->next = prev->next->next;
    ll->size--;
    void* elem = i->elem;
    destroy_node(i);
    return elem;
}
```

Bạn có thể làm cho đoạn mã găng chặt chẽ đến mức nào?
* Bài toán nhà sản xuất - người tiêu dùng là gì? Làm thế nào bài toán nhà sản xuất - người tiêu dùng có thể được sử dụng trong phần trên? Bài toán nhà sản xuất - người tiêu dùng có liên quan như thế nào đến bài toán người đọc - người viết?
* Biến điều kiện là gì? Tại sao việc sử dụng biến điều kiện lại có lợi thế hơn so với vòng lặp `while`?
* Tại sao đoạn mã này nguy hiểm?
```C
if(not_ready){
     pthread_cond_wait(&cv, &mtx);
}
```
* Semaphore đếm là gì? Hãy cho tôi một phép so sánh với hộp bánh quy/hộp pizza/mặt hàng thực phẩm có hạn.
* Rào cản luồng là gì?
* Sử dụng semaphore đếm để triển khai rào cản.

* Viết một hàng đợi Nhà sản xuất/Người tiêu dùng, Còn một ngăn xếp Nhà sản xuất/Người tiêu dùng thì sao?
* Hãy cho tôi một triển khai khóa đọc-ghi với các biến điều kiện, tạo một cấu trúc với bất cứ thứ gì bạn cần, nó chỉ cần có thể hỗ trợ các hàm sau
```C
void reader_lock(rw_lock_t* lck);
void writer_lock(rw_lock_t* lck);
void reader_unlock(rw_lock_t* lck);
void writer_unlock(rw_lock_t* lck);
```
Đặc tả duy nhất là trong khoảng thời gian giữa `reader_lock` và `reader_unlock`, không có trình ghi nào có thể ghi. Trong khoảng thời gian giữa các khóa trình ghi, chỉ có một trình ghi có thể ghi tại một thời điểm.

* Viết mã để triển khai nhà sản xuất - người tiêu dùng CHỈ sử dụng ba semaphore đếm. Giả sử có thể có nhiều hơn một luồng gọi enqueue và dequeue.
Xác định giá trị ban đầu của mỗi semaphore.
* Viết mã để triển khai nhà sản xuất - người tiêu dùng sử dụng các biến điều kiện và mutex. Giả sử có thể có nhiều hơn một luồng gọi enqueue và dequeue.
* Sử dụng biến điều kiện để triển khai các hàm chặn add(unsigned int) và subtract(unsigned int) không bao giờ cho phép giá trị toàn cục lớn hơn 100.
* Sử dụng biến điều kiện để triển khai rào cản cho 15 luồng.
* Có bao nhiêu câu sau đây đúng?
     * Có thể có nhiều trình đọc đang hoạt động
     * Có thể có nhiều trình ghi đang hoạt động
     * Khi có một trình ghi đang hoạt động, số lượng trình đọc đang hoạt động phải bằng không
     * Nếu có một trình đọc đang hoạt động, số lượng trình ghi đang hoạt động phải bằng không
     * Trình ghi phải đợi cho đến khi các trình đọc đang hoạt động hiện tại kết thúc
* Việc cần làm: Phân tích các đoạn mã đa luồng
