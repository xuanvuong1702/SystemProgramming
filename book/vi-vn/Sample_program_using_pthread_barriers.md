```C
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_COUNT 4

pthread_barrier_t mybarrier;

void* threadFn(void *id_ptr) {
  int thread_id = *(int*)id_ptr;
  int wait_sec = 1 + rand() % 5;
  printf("luồng %d: Chờ trong %d giây.\n", thread_id, wait_sec);
  sleep(wait_sec);
  printf("luồng %d: Tôi đã sẵn sàng...\n", thread_id);

  pthread_barrier_wait(&mybarrier);

  printf("luồng %d: bắt đầu!\n", thread_id);
  return NULL;
}


int main() {
  int i;
  pthread_t ids[THREAD_COUNT];
  int short_ids[THREAD_COUNT];

  srand(time(NULL));
  pthread_barrier_init(&mybarrier, NULL, THREAD_COUNT + 1);

  for (i=0; i < THREAD_COUNT; i++) {
    short_ids[i] = i;
    pthread_create(&ids[i], NULL, threadFn, &short_ids[i]);
  }

  printf("main() đã sẵn sàng.\n");

  pthread_barrier_wait(&mybarrier);

  printf("main() bắt đầu!\n");

  for (i=0; i < THREAD_COUNT; i++) {
    pthread_join(ids[i], NULL);
  }

  pthread_barrier_destroy(&mybarrier);

  return 0;
}
```

**Giải thích mã:**

Mã này minh họa cách sử dụng `pthread_barrier` trong lập trình đa luồng C. 

**pthread_barrier là gì?**

`pthread_barrier` là một cơ chế đồng bộ hóa cho phép nhiều luồng chờ đợi lẫn nhau tại một điểm nhất định trước khi tiếp tục thực thi. Nó giống như một rào cản mà tất cả các luồng phải đến trước khi bất kỳ luồng nào có thể vượt qua.

**Chi tiết mã:**

1. **Khai báo biến `pthread_barrier_t mybarrier`:** Biến này đại diện cho rào cản.

2. **Hàm `threadFn`:**
   - Nhận ID luồng làm đối số.
   - Tạo một thời gian chờ ngẫu nhiên từ 1 đến 5 giây.
   - In thông báo cho biết luồng đang chờ.
   - Gọi `pthread_barrier_wait(&mybarrier)`: Luồng sẽ dừng tại đây cho đến khi tất cả các luồng khác (bao gồm cả luồng chính) cũng gọi `pthread_barrier_wait`.
   - Sau khi tất cả các luồng đã đến rào cản, chúng sẽ được giải phóng đồng thời và tiếp tục thực thi.
   - In thông báo cho biết luồng đang bắt đầu.

3. **Hàm `main`:**
   - Khởi tạo bộ tạo số ngẫu nhiên.
   - Khởi tạo rào cản `mybarrier` với số lượng luồng cần chờ là `THREAD_COUNT + 1` (bao gồm cả luồng chính).
   - Tạo `THREAD_COUNT` luồng, mỗi luồng chạy hàm `threadFn`.
   - In thông báo cho biết luồng chính đã sẵn sàng.
   - Gọi `pthread_barrier_wait(&mybarrier)`: Luồng chính sẽ dừng tại đây cho đến khi tất cả các luồng khác đã đến rào cản.
   - Sau khi tất cả các luồng đã đến rào cản, chúng sẽ được giải phóng đồng thời và tiếp tục thực thi.
   - In thông báo cho biết luồng chính đang bắt đầu.
   - Chờ tất cả các luồng con kết thúc bằng cách sử dụng `pthread_join`.
   - Hủy rào cản `mybarrier`.

**Kết quả:**

Chương trình sẽ tạo ra 4 luồng, mỗi luồng sẽ chờ một khoảng thời gian ngẫu nhiên trước khi đến rào cản. Khi tất cả các luồng (bao gồm cả luồng chính) đã đến rào cản, chúng sẽ được giải phóng đồng thời và in thông báo "bắt đầu!". Cuối cùng, luồng chính sẽ chờ tất cả các luồng con kết thúc trước khi kết thúc chương trình. 

**Lưu ý:**

`_GNU_SOURCE` được định nghĩa để đảm bảo rằng các hàm pthread được khai báo chính xác. `rand()` và `srand()` được sử dụng để tạo số ngẫu nhiên. `sleep()` được sử dụng để tạm dừng luồng trong một khoảng thời gian nhất định. 
