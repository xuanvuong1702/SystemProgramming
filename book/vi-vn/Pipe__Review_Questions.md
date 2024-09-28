> Số câu hỏi có thể thay đổi

## Câu hỏi 1
Điền vào chỗ trống để chương trình sau in ra 123456789. Nếu `cat` không được cung cấp đối số, nó chỉ đơn giản là in ra đầu vào của nó cho đến khi gặp EOF. Phần thưởng: Giải thích tại sao cần gọi hàm `close` bên dưới.
````C
int main() {
    int i = 0;
    while (++i < 10) {
        pid_t pid = fork();
        if (pid == 0) { /* tiến trình con */
            char buffer[16];
            sprintf(buffer, ______, i);
            int fds[______];
            pipe(fds);
            write(fds[1], ______, ______); // Ghi bộ đệm vào pipe
            close(______);
            dup2(fds[0], ______);
            execlp("cat", "cat",  ______);
            perror("exec"); exit(1);
        }
        waitpid(pid, NULL, 0);
    }
    return 0;
}
````

## Câu hỏi 2
Sử dụng các lệnh gọi POSIX `fork` `pipe` `dup2` và `close` để triển khai một chương trình chấm điểm tự động. Chụp đầu ra tiêu chuẩn của một tiến trình con vào một pipe. Tiến trình con sẽ `exec` chương trình `./test` mà không có đối số bổ sung nào (ngoài tên tiến trình). Trong tiến trình cha, đọc từ pipe: Thoát tiến trình cha ngay khi đầu ra được chụp chứa ký tự !. Trước khi thoát tiến trình cha, hãy gửi SIGKILL đến tiến trình con. Thoát 0 nếu đầu ra chứa !. Mặt khác, nếu tiến trình con thoát khiến đầu ghi của pipe bị đóng, thì thoát với giá trị 1. Đảm bảo đóng các đầu không sử dụng của pipe trong tiến trình cha và tiến trình con.

## Câu hỏi 3 (Nâng cao)
Thử thách nâng cao này sử dụng pipe để khiến "người chơi AI" tự chơi cho đến khi trò chơi kết thúc.
Chương trình `tictactoe` chấp nhận một dòng đầu vào - chuỗi các lượt đã thực hiện cho đến nay, in ra cùng một chuỗi theo sau là một lượt khác, và sau đó thoát. Một lượt được chỉ định bằng cách sử dụng hai ký tự. Ví dụ: "A1" và "C3" là hai vị trí góc đối diện. Chuỗi `B2A1A3` là một trò chơi gồm 3 lượt/ván. Một phản hồi hợp lệ là `B2A1A3C1` (phản hồi C1 chặn mối đe dọa đường chéo B2 A3). Dòng đầu ra cũng có thể bao gồm hậu tố `-Tôi thắng` `-Bạn thắng` `-không hợp lệ` hoặc `-hòa`
Sử dụng pipe để kiểm soát đầu vào và đầu ra của mỗi tiến trình con được tạo. Khi đầu ra chứa `-`, hãy in dòng đầu ra cuối cùng (toàn bộ chuỗi trò chơi và kết quả) và thoát.