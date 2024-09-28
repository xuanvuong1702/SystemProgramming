# Mô phỏng máy là gì?

Bằng cách mô phỏng bộ xử lý, bộ nhớ và các thành phần máy khác (ví dụ: card mạng, ổ lưu trữ, card đồ họa), hành vi của một máy thực có thể được mô phỏng. Ví dụ: Jor1k mô phỏng một máy tính hoàn chỉnh, bao gồm một bộ xử lý đơn giản ("OR1K") bằng JavaScript và do đó có thể chạy bên trong trình duyệt web.

# Mô phỏng CPU có nghĩa là mô phỏng mọi mã op có thể

Để thực thi một lệnh CPU duy nhất, trình mô phỏng phải trước tiên đọc một từ từ bộ nhớ (dịch địa chỉ ảo thành địa chỉ vật lý), sau đó giải mã từ đó thành mã op. Việc thực thi mỗi mã op CPU có thể có được thực hiện bằng cách sử dụng một tập hợp rất lớn các câu lệnh case hoặc tra cứu bằng bảng hàm. Lệnh mã hóa lệnh mã op (cộng, hoặc, nhảy đến địa chỉ mới, v.v.) trong các bit cao (bit 26 đến 31). Các bit thấp mã hóa các thanh ghi CPU nào sẽ được sử dụng cho các phép toán AND và OR.

```C
     ins = this.GetInstruction(this.pc<<2)
     // Mã op CPU được lưu trữ trong
     switch ((ins >> 26)&0x3F) {
        ...
        case 0x29:
            // andi
            r[(ins >> 21) & 0x1F] = r[(ins >> 16) & 0x1F] & (ins & 0xFFFF);
            break;

        case 0x2A:
            // ori
            r[(ins >> 21) & 0x1F] = r[(ins >> 16) & 0x1F] | (ins & 0xFFFF);
            break;
         ...
       }
       this.pc = this.nextpc++;

Nguồn: https://github.com/s-macke/jor1k/blob/master/js/worker/or1k/safecpu.js
```

Một trình mô phỏng của bộ xử lý RISC-V được hiển thị bên dưới.
```C
// Mô phỏng bộ xử lý RISC-V
                // addi, slti, sltiu, xori, ori, andi, slli, srli, srai
                rindex = (ins >> 7) & 0x1F;
                rs1 = r[(ins >> 15) & 0x1F];
                imm = (ins >> 20);
                switch((ins >> 12)&0x7) {
                    
                    case 0x00:
                        // addi
                        r[rindex] = rs1 + imm;
                        break;

                    case 0x06:
                        // ori
                        r[rindex] = rs1 | imm;
                        break;
                     ....
              }
Nguồn: https://github.com/s-macke/jor1k/blob/master/js/worker/riscv/safecpu.js
```

Trong thực tế, ít nhất 10 đến 100 lệnh CPU thực cần được thực thi để mô phỏng hành vi của một lệnh duy nhất của máy ảo. Do đó, phần mềm chạy trong máy được mô phỏng thường thực thi chậm hơn 10 đến 100 lần so với hiệu suất gốc.

# Ưu điểm của mô phỏng.

Mô phỏng đầy đủ cung cấp hai tính năng quan trọng - bảo mật và khả năng thực thi các ảnh nhị phân đã được biên dịch cho một kiến trúc khác. Ví dụ: hệ điều hành Android Linux được biên dịch cho bộ xử lý ARM có thể được kiểm tra và gỡ lỗi trên máy chủ x86-64bit (Intel), mặc dù CPU Intel có bộ lệnh hoàn toàn khác.

# Máy được mô phỏng có an toàn không?

Trình mô phỏng hệ thống đầy đủ như JOR1K cung cấp một môi trường tương đối an toàn để chạy mã không đáng tin cậy. Ví dụ: sẽ rất khó để viết mã độc để thoát khỏi môi trường JOR1K và chạy mã tùy ý trên máy chủ.

Tuy nhiên, trình mô phỏng rất phức tạp và cung cấp các thiết bị ảo để hỗ trợ I/O. Phần mềm độc hại vẫn có thể sử dụng các thiết bị ảo để thực hiện các cuộc tấn công thực tế như từ chối dịch vụ hoặc đoán mật khẩu trên các dịch vụ thực. Nếu có lỗi bảo mật trong việc triển khai các thiết bị ảo, thì những lỗ hổng này có thể bị khai thác để truy cập các tài nguyên (chẳng hạn như tệp máy chủ hoặc Internet) mà lẽ ra phải được ngăn chặn.

# Độ chính xác

Cần làm: thảo luận về sự đánh đổi hiệu suất so với độ chính xác.

# QEMU là một trình mô phỏng hệ thống đầy đủ.

QEMU là một trình mô phỏng và ảo hóa máy tính đa năng và mã nguồn mở, có thể chạy trong máy chủ Windows, Linux hoặc OSX. Tương tự như trình mô phỏng Jor1k ở trên, QEMU có thể mô phỏng một hệ thống hoàn chỉnh bao gồm CPU, bộ nhớ, CD-ROM, âm thanh, kết nối USB, mạng, bộ nhớ, đồ họa và I/O bàn phím.
