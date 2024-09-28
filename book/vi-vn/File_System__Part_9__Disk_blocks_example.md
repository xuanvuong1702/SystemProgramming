## Đang xây dựng

## Bạn có thể giải thích một mô hình đơn giản về cách nội dung của tệp được lưu trữ trong một hệ thống tệp dựa trên i-node đơn giản không?

Chắc chắn rồi! Để trả lời câu hỏi này, chúng ta sẽ xây dựng một đĩa ảo và sau đó viết một số mã C để truy cập nội dung của nó. Hệ thống tệp của chúng ta sẽ chia các byte khả dụng thành không gian cho inode và một không gian lớn hơn nhiều cho các khối đĩa. Mỗi khối đĩa sẽ là 4096 byte: 

```C
// Kích thước đĩa:
#define MAX_INODE (1024)
#define MAX_BLOCK (1024*1024)

// Mỗi khối là 4096 byte:
typedef char[4096] block_t;

// Một đĩa là một mảng inode và một mảng các khối đĩa:
struct inode[MAX_INODE] inodes;
block_t[MAX_BLOCK] blocks;
```

Lưu ý để rõ ràng, chúng tôi sẽ không sử dụng kiểu dữ liệu `unsigned` trong ví dụ mã này. Các inode có kích thước cố định của chúng tôi sẽ chứa kích thước của tệp tính bằng byte, quyền, người dùng, thông tin nhóm, siêu dữ liệu thời gian. Điều liên quan nhất đến vấn đề hiện tại là nó cũng sẽ bao gồm mười con trỏ đến các khối đĩa mà chúng ta sẽ sử dụng để tham chiếu đến nội dung thực tế của tệp!

```C
struct inode {
    int[10] directblocks; // chỉ mục cho mảng khối, tức là nơi tìm nội dung của tệp
    long size;
    // ... siêu dữ liệu inode tiêu chuẩn, ví dụ:
    int mode, userid, groupid;
    time_t ctime, atime, mtime;
};
```
Bây giờ chúng ta có thể tìm ra cách đọc một byte ở vị trí `position` của tệp của chúng ta:
```C
char readbyte(struct inode *inode, long position) {
    if (position < 0 || position >= inode->size) return -1; // vị trí bù không hợp lệ

    int block_count = position / 4096, offset = position % 4096;
  
    // block_count tốt hơn nên là 0..9!
    int physical_idx = lookup_physical_block_index(inode, block_count );

    // kiểm tra tỉnh táo rằng chỉ mục khối đĩa là hợp lý ...
    assert (physical_idx >= 0 && physical_idx < MAX_BLOCK);


    // đọc khối đĩa từ đĩa ảo 'blocks' của chúng ta và trả về byte cụ thể
    return blocks[physical_idx][offset];
}
```
Phiên bản ban đầu của `lookup_physical_block` của chúng tôi rất đơn giản - chúng tôi có thể sử dụng bảng 10 khối trực tiếp của mình!

```C
int lookup_physical_block_index(struct inode *inode, int block_count) {
    assert (block_count >= 0 && block_count < 10);

    return inode->directblocks[block_count]; // trả về một giá trị chỉ mục trong khoảng [0,MAX_BLOCK)
}
```


Biểu diễn đơn giản này là hợp lý với điều kiện là chúng ta có thể biểu diễn tất cả các tệp có thể chỉ với mười khối, tức là tối đa 40KB. Còn những tệp lớn hơn thì sao? Chúng ta cần struct `inode` luôn có cùng kích thước, do đó, việc chỉ tăng mảng khối trực tiếp hiện có lên 20 sẽ làm tăng gấp đôi kích thước inode của chúng ta. Nếu hầu hết các tệp của chúng ta yêu cầu ít hơn 10 khối, thì dung lượng lưu trữ inode của chúng ta bây giờ sẽ lãng phí. Để giải quyết vấn đề này, chúng ta sẽ sử dụng một khối đĩa được gọi là *khối gián tiếp* để mở rộng mảng con trỏ theo ý của chúng ta. Chúng ta sẽ chỉ cần điều này cho các tệp > 40KB.


```C
struct inode {
    int[10] directblocks; // nếu size <4KB thì chỉ cái đầu tiên là hợp lệ
    int indirectblock; // giá trị hợp lệ khi size >= 40KB
    int size;
    ...
};
```

Khối gián tiếp chỉ là một khối đĩa thông thường có 4096 byte, nhưng chúng tôi sẽ sử dụng nó để chứa các con trỏ đến các khối đĩa. Các con trỏ của chúng ta trong trường hợp này chỉ là các số nguyên, vì vậy chúng tôi cần ép kiểu con trỏ thành con trỏ số nguyên:


```C
int lookup_physical_block_index(struct inode *inode, int block_count) {
    assert (sizeof (int) == 4); // Cảnh báo mã này giả định một chỉ mục là 4 byte!
    assert (block_count >= 0 && block_count < 1024 + 10); // 0 <= block_count <1034

    if (block_count < 10)
        return inode->directblocks[block_count];
  
    // đọc khối gián tiếp từ đĩa:
    block_t* oneblock = &blocks[inode->indirectblock];

    // Coi 4KB là một mảng gồm 1024 con trỏ đến các khối đĩa khác
    int *table = (int *) oneblock;
  
    // Tra cứu mục nhập chính xác trong bảng
    // Bù 10 vì 10 khối dữ liệu đầu tiên đã được 
    // tính đến
    return table[block_count - 10];
}
```


Đối với một hệ thống tệp điển hình, các giá trị chỉ mục của chúng tôi là 32 bit, tức là 4 byte. Do đó, trong 4096 byte, chúng tôi có thể lưu trữ 4096 / 4 = 1024 mục nhập.
Điều này có nghĩa là khối gián tiếp của chúng ta có thể tham chiếu đến 1024 * 4KB = 4MB dữ liệu. Với mười khối trực tiếp đầu tiên, do đó chúng tôi có thể chứa các tệp có dung lượng lên đến 40KB + 1024 * 4KB = 4136KB. Một số mục nhập bảng sau này có thể không hợp lệ đối với các tệp nhỏ hơn mức này. 

Đối với các tệp thậm chí còn lớn hơn, chúng ta có thể sử dụng hai khối gián tiếp. Tuy nhiên, có một giải pháp thay thế tốt hơn, cho phép chúng tôi mở rộng quy mô hiệu quả lên đến các tệp khổng lồ. Chúng tôi sẽ bao gồm một con trỏ gián tiếp kép, và nếu điều đó là không đủ, một con trỏ gián tiếp ba. Con trỏ gián tiếp kép có nghĩa là chúng ta có một bảng gồm 1024 mục nhập đến các khối đĩa được sử dụng làm 1024 mục nhập. Điều này có nghĩa là chúng ta có thể tham chiếu đến 1024 * 1024 khối dữ liệu đĩa.

![inode disk blocks for data](http://uw714doc.sco.com/en/FS_admin/graphics/s5chain.gif)

(nguồn: http://uw714doc.sco.com/en/FS_admin/graphics/s5chain.gif)

```C
int lookup_physical_block_index(struct inode *inode, int block_count) {
    if (block_count < 10)
        return inode->directblocks[block_count];

    // Sử dụng khối gián tiếp cho 1024 khối tiếp theo:
    // Giả định 1024 số nguyên có thể nằm gọn bên trong mỗi khối!
    if (block_count < 1024 + 10) {   
        int *table = (int *) &blocks[inode->indirectblock];
        return table[block_count - 10];
    }
    // Đối với các tệp lớn, chúng tôi sẽ sử dụng một bảng các bảng
    int i = (block_count - 1034) / 1024, j = (block_count - 1034) % 1024;
    assert (i < 1024); // gián tiếp ba không được triển khai ở đây!

    int *table1 = (int *) &blocks[inode->doubleindirectblock];
    // Bảng đầu tiên cho chúng ta biết nên đọc bảng thứ hai ở đâu ...
    int *table2 = (int *) &blocks[table1[i]];
    return table2[j];
 
    // Đối với các tệp khổng lồ, chúng tôi sẽ cần triển khai gián tiếp ba (bảng của bảng của bảng)
}
```
Lưu ý rằng việc đọc một byte bằng gián tiếp kép yêu cầu đọc 3 khối đĩa (hai bảng và khối dữ liệu thực tế).


