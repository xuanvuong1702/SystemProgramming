## Biểu đồ phân bổ tài nguyên là gì?

Biểu đồ phân bổ tài nguyên theo dõi tài nguyên nào được giữ bởi tiến trình nào và tiến trình nào đang chờ tài nguyên thuộc một loại cụ thể. Nó là một công cụ rất mạnh mẽ và đơn giản để minh họa cách các tiến trình tương tác có thể rơi vào bế tắc (deadlock). Nếu một tiến trình đang _sử dụng_ một tài nguyên, một mũi tên sẽ được vẽ từ nút tài nguyên đến nút tiến trình. Nếu một tiến trình đang _yêu cầu_ một tài nguyên, một mũi tên sẽ được vẽ từ nút tiến trình đến nút tài nguyên.


Nếu có một chu trình trong Biểu đồ phân bổ tài nguyên và mỗi tài nguyên trong chu trình chỉ cung cấp một phiên bản, thì các tiến trình sẽ bị deadlock. Ví dụ: nếu tiến trình 1 giữ tài nguyên A, tiến trình 2 giữ tài nguyên B và tiến trình 1 đang chờ B và tiến trình 2 đang chờ A, thì tiến trình 1 và tiến trình 2 sẽ bị deadlock.

Đây là một ví dụ khác, cho thấy Tiến trình 1 và 2 giành được tài nguyên 1 và 2 trong khi tiến trình 3 đang chờ để giành được cả hai tài nguyên. Trong ví dụ này, không có deadlock vì không có sự phụ thuộc vòng.

![ResourceAllocationGraph-Ex1.png](https://raw.githubusercontent.com/wiki/angrave/SystemProgramming/ResourceAllocationGraph-Ex1.png)


## Deadlock!

Rất nhiều lần, chúng ta không biết thứ tự cụ thể mà một tài nguyên có thể được giành được, nên chúng ta có thể vẽ đồ thị có hướng.

![](http://i.imgur.com/V16FfnX.png)

Như một ma trận khả năng. Sau đó, chúng ta có thể vẽ các mũi tên và xem liệu có một phiên bản có hướng nào sẽ dẫn chúng ta đến deadlock hay không.

![RAG Deadlock](http://i.imgur.com/6duq0PD.png)

Hãy xem xét biểu đồ phân bổ tài nguyên sau (giả sử rằng các tiến trình yêu cầu quyền truy cập độc quyền vào tệp). Nếu bạn có một loạt các tiến trình đang chạy và chúng yêu cầu tài nguyên và hệ điều hành kết thúc ở trạng thái này, bạn sẽ bị deadlock! Bạn có thể không thấy điều này bởi vì hệ điều hành có thể **cướp đoạt* một số tiến trình phá vỡ chu kỳ nhưng vẫn có khả năng ba tiến trình đơn độc của bạn có thể bị deadlock. Bạn cũng có thể tạo loại đồ thị này bằng `make` và các phụ thuộc quy tắc (ví dụ: với parmake MP của chúng ta).

![](http://cs241.cs.illinois.edu/images/ColorfulDeadlock.svg)
