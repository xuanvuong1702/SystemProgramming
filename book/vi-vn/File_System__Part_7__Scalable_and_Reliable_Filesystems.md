# Hệ thống tệp đĩa đơn đáng tin cậy

## Kernel lưu trữ hệ thống tệp như thế nào và tại sao?

Hầu hết các hệ thống tệp đều lưu trữ (cache) một lượng đáng kể dữ liệu đĩa trong bộ nhớ vật lý.
Về khía cạnh này, Linux đặc biệt cực đoan: Tất cả bộ nhớ không sử dụng đều được sử dụng như một bộ nhớ đệm đĩa khổng lồ.

Bộ nhớ đệm đĩa có thể có tác động đáng kể đến hiệu suất tổng thể của hệ thống vì I/O đĩa chậm. Điều này đặc biệt đúng đối với các yêu cầu truy cập ngẫu nhiên trên các đĩa quay, trong đó độ trễ đọc-ghi đĩa bị chi phối bởi thời gian tìm kiếm cần thiết để di chuyển đầu đọc-ghi đĩa đến vị trí chính xác.

Để hiệu quả, kernel lưu trữ các khối đĩa được sử dụng gần đây. 
Để ghi, chúng ta phải lựa chọn giữa hiệu suất và độ tin cậy: Ghi đĩa cũng có thể được lưu trữ ("Bộ nhớ đệm ghi lại" - Write-back cache) trong đó các khối đĩa đã sửa đổi được lưu trữ trong bộ nhớ cho đến khi bị loại bỏ. Ngoài ra, một chính sách 'bộ nhớ đệm ghi trực tiếp' (Write-through cache) có thể được sử dụng, trong đó các thao tác ghi đĩa được gửi ngay lập tức đến đĩa. Cách sau an toàn hơn (vì các sửa đổi hệ thống tệp được lưu trữ nhanh chóng vào phương tiện lưu trữ liên tục) nhưng chậm hơn so với bộ nhớ đệm ghi lại; Nếu ghi được lưu trữ, chúng có thể bị trì hoãn và được lên lịch hiệu quả dựa trên vị trí vật lý của mỗi khối đĩa.

Lưu ý đây là mô tả đơn giản hóa vì ổ đĩa thể rắn (SSD) có thể được sử dụng làm bộ nhớ đệm ghi lại thứ cấp.

Cả ổ đĩa thể rắn (SSD) và đĩa quay đều có hiệu suất được cải thiện khi đọc hoặc ghi dữ liệu tuần tự. Do đó, hệ điều hành thường có thể sử dụng chiến lược đọc trước để phân bổ chi phí yêu cầu đọc (ví dụ: chi phí thời gian cho đĩa quay) và yêu cầu một số khối đĩa liền kề trên mỗi yêu cầu. Bằng cách đưa ra yêu cầu I/O cho khối đĩa tiếp theo *trước khi* ứng dụng của người dùng yêu cầu khối đĩa tiếp theo, độ trễ I/O đĩa biểu kiến ​​có thể được giảm bớt.

## Dữ liệu của tôi rất quan trọng! Tôi có thể buộc các thao tác ghi đĩa được lưu vào phương tiện vật lý và đợi nó hoàn thành không?

Có (gần như). Gọi `sync` để yêu cầu ghi (xả) các thay đổi hệ thống tệp vào đĩa.
Tuy nhiên, không phải tất cả các hệ điều hành đều đáp ứng yêu cầu này và ngay cả khi dữ liệu bị xóa khỏi bộ đệm của kernel, phần sụn đĩa có thể sử dụng bộ nhớ đệm trên đĩa nội bộ hoặc có thể vẫn chưa hoàn thành việc thay đổi phương tiện vật lý. 

Lưu ý, bạn cũng có thể yêu cầu tất cả các thay đổi liên quan đến một bộ mô tả tệp cụ thể được ghi vào đĩa bằng cách sử dụng `fsync(int fd)`.

## Điều gì sẽ xảy ra nếu đĩa của tôi bị lỗi trong khi đang thực hiện một thao tác quan trọng?

Đừng lo lắng, hầu hết các hệ thống tệp hiện đại đều thực hiện một thứ gọi là **ghi nhật ký** (journalling) để khắc phục điều này. Những gì hệ thống tệp làm trước khi nó hoàn thành một thao tác có khả năng tốn kém, là nó ghi những gì nó sẽ làm vào một nhật ký. Trong trường hợp bị sập hoặc lỗi, người ta có thể xem qua nhật ký và xem tệp nào bị hỏng và sửa chúng. Đây là một cách để cứu vãn ổ cứng trong trường hợp có dữ liệu quan trọng và không có bản sao lưu rõ ràng.

## Khả năng xảy ra lỗi đĩa là bao nhiêu?

Lỗi đĩa được đo bằng cách sử dụng "Thời gian trung bình đến lỗi" (Mean Time To Failure - MTTF). Đối với các mảng lớn, thời gian lỗi trung bình có thể ngắn đến mức đáng ngạc nhiên. Ví dụ: nếu MTTF (đĩa đơn) = 30.000 giờ, thì MTTF (100 đĩa) = 30000/100 = 300 giờ, tức là khoảng 12 ngày!

# Dư thừa

## Làm cách nào để bảo vệ dữ liệu của tôi khỏi lỗi đĩa?

Dễ dàng! Lưu trữ dữ liệu hai lần! Đây là nguyên tắc chính của mảng đĩa "RAID-1". RAID là viết tắt của mảng đĩa độc lập dư thừa (Redundant Array of Inexpensive Disks). Bằng cách sao chép các ghi vào đĩa với các ghi vào một đĩa khác (đĩa sao lưu), có chính xác hai bản sao của dữ liệu. Nếu một đĩa bị lỗi, đĩa kia sẽ đóng vai trò là bản sao duy nhất cho đến khi nó được nhân bản lại. Việc đọc dữ liệu nhanh hơn (vì dữ liệu có thể được yêu cầu từ một trong hai đĩa) nhưng ghi có khả năng chậm gấp đôi (bây giờ cần phải đưa ra hai lệnh ghi cho mỗi lần ghi khối đĩa) và so với việc sử dụng một đĩa đơn, chi phí lưu trữ cho mỗi byte đã tăng gấp đôi.

Một sơ đồ RAID phổ biến khác là RAID-0, có nghĩa là một tệp có thể được chia nhỏ giữa hai đĩa, nhưng nếu bất kỳ đĩa nào bị lỗi, thì các tệp sẽ không thể phục hồi. Điều này có lợi ích là giảm một nửa thời gian ghi vì một phần của tệp có thể được ghi vào đĩa cứng một và phần khác vào đĩa cứng hai.

Việc kết hợp các hệ thống này cũng rất phổ biến. Nếu bạn có nhiều đĩa cứng, hãy xem xét RAID-10. Đây là nơi bạn có hai hệ thống RAID-1, nhưng các hệ thống được kết nối với nhau theo RAID-0. Điều này có nghĩa là bạn sẽ nhận được tốc độ gần giống nhau từ việc giảm tốc độ nhưng bây giờ bất kỳ đĩa nào có thể bị lỗi và bạn có thể khôi phục đĩa đó. (Nếu hai đĩa từ các phân vùng RAID đối lập bị lỗi, có khả năng phục hồi có thể xảy ra mặc dù chúng ta không tính đến nó hầu hết thời gian). 

## RAID-3 là gì?

RAID-3 sử dụng mã chẵn lẻ thay vì nhân bản dữ liệu. Đối với mỗi N bit được ghi, chúng ta sẽ ghi thêm một bit, 'bit chẵn lẻ' (Parity bit) để đảm bảo tổng số bit 1 được ghi là chẵn. Bit chẵn lẻ được ghi vào một đĩa bổ sung. Nếu bất kỳ đĩa nào (bao gồm cả đĩa chẵn lẻ) bị mất, thì nội dung của nó vẫn có thể được tính toán bằng cách sử dụng nội dung của các đĩa khác.

![](http://devnull.typepad.com/.a/6a00e551c39e1c88340133ed18ed66970b-pi)

Một nhược điểm của RAID-3 là bất cứ khi nào một khối đĩa được ghi, khối chẵn lẻ cũng sẽ luôn được ghi. Điều này có nghĩa là về cơ bản có một nút thắt cổ chai trong một đĩa riêng biệt. Trong thực tế, điều này có nhiều khả năng gây ra lỗi hơn vì một đĩa đang được sử dụng 100% thời gian và khi đĩa đó bị lỗi, các đĩa khác sẽ dễ bị lỗi hơn.

## RAID-3 an toàn đến mức nào đối với việc mất dữ liệu?

Lỗi đĩa đơn sẽ không dẫn đến mất dữ liệu (vì có đủ dữ liệu để xây dựng lại mảng từ các đĩa còn lại). Mất dữ liệu sẽ xảy ra khi hai đĩa không sử dụng được vì không còn đủ dữ liệu để xây dựng lại mảng. Chúng ta có thể tính toán xác suất xảy ra lỗi hai đĩa dựa trên thời gian sửa chữa bao gồm không chỉ thời gian để lắp đĩa mới mà còn cả thời gian cần thiết để xây dựng lại toàn bộ nội dung của mảng.
```
MTTF = thời gian trung bình đến lỗi
MTTR = thời gian trung bình để sửa chữa
N = số lượng đĩa ban đầu

p = MTTR / (MTTF-một-đĩa / (N-1))
```
Sử dụng các số điển hình (MTTR = 1 ngày, MTTF = 1000 ngày, N-1 = 9, p = 0,009).

Có 1% khả năng một ổ đĩa khác sẽ bị lỗi trong quá trình xây dựng lại (tại thời điểm đó, tốt hơn hết là bạn nên hy vọng rằng bạn vẫn có bản sao lưu có thể truy cập được dữ liệu ban đầu của mình).


Trong thực tế, xác suất xảy ra lỗi thứ hai trong quá trình sửa chữa có thể cao hơn vì việc xây dựng lại mảng rất tốn I/O (và trên hết là hoạt động yêu cầu I/O thông thường). Tải I/O cao hơn này cũng sẽ gây căng thẳng cho mảng đĩa.



## RAID-5 là gì?
RAID-5 tương tự như RAID-3 ngoại trừ việc khối kiểm tra (thông tin chẵn lẻ) được gán cho các đĩa khác nhau cho các khối khác nhau. Khối kiểm tra được 'xoay' qua mảng đĩa. RAID-5 cung cấp hiệu suất đọc và ghi tốt hơn RAID-3 vì không còn nút thắt cổ chai của đĩa chẵn lẻ đơn. Nhược điểm duy nhất là bạn cần nhiều đĩa hơn để có thiết lập này và cần sử dụng các thuật toán phức tạp hơn.

![](http://www.seagate.com/files/www-content/manuals/business-storage-nas-os-manual/_shared/images/118a_ill_raid_5.png)

## Lưu trữ phân tán

Lỗi là trường hợp phổ biến.
Google báo cáo 2-10% đĩa bị lỗi mỗi năm.
Bây giờ hãy nhân con số đó với hơn 60.000 đĩa trong một trung tâm dữ liệu duy nhất ...
Hệ thống phải tồn tại sau lỗi không chỉ của đĩa mà còn của cả một rack máy chủ hoặc toàn bộ trung tâm dữ liệu.


Giải pháp:

* Dư thừa đơn giản (2 hoặc 3 bản sao của mỗi tệp), ví dụ: Google GFS (2001)
* Dư thừa hiệu quả hơn (tương tự như RAID 3 ++), ví dụ: [Hệ thống tệp Google Colossus](http://goo.gl/LwFIy) (~2010)
* Sao chép có thể tùy chỉnh bao gồm mã Reed-Solomon với độ dư thừa 1,5 lần.

[Chuyển đến Hệ thống tệp: Phần 8](https://github.com/angrave/SystemProgramming/wiki/File-System%2C-Part-8%3A-Removing-preinstalled-malware-from-an-Android-device)
