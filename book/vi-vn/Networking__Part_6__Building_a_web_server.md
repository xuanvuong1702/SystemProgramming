## Làm cách nào để phục vụ một trang web tĩnh đơn giản?

Một máy chủ web cần triển khai **HTTP** (Giao thức truyền tải siêu văn bản), là một đặc tả về cách máy khách có thể yêu cầu tài nguyên từ máy chủ và cách máy chủ có thể phản hồi tin nhắn của máy khách.

Chúng ta sẽ không xây dựng một máy chủ web hoàn toàn tuân thủ. Thay vào đó, chúng ta sẽ xây dựng máy chủ web đơn giản nhất có thể, sử dụng các phần của **máy chủ TCP** đã được giới thiệu.

Đầu tiên, chúng ta sẽ thiết lập một **socket máy chủ thụ động**
```C
 hints.ai_family = AF_INET;
 hints.ai_socktype = SOCK_STREAM;
 hints.ai_flags = AI_PASSIVE;
 getaddrinfo(NULL, "0", &hints, &result); // "0"  = sử dụng bất kỳ cổng nào còn trống
```
Chúng ta không cần **hết thời gian chờ** sau khi chúng ta sử dụng xong cổng - nó có thể được sử dụng lại ngay lập tức,
```C
 int optval = 1;
 setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
```
**Liên kết** socket với một cổng trên máy cục bộ và bắt đầu **lắng nghe** các kết nối
```C
 bind(sock_fd, result->ai_addr, result->ai_addrlen)
 listen(sock_fd, 10)
```
Bây giờ chúng ta đã liên kết với một cổng, chúng ta có thể tìm ra số cổng thực tế được sử dụng. Lưu ý rằng chúng ta cần chuyển đổi nó từ **thứ tự byte mạng** sang **máy chủ**, (`ntohs`)
```C
  struct sockaddr_in sin;
  socklen_t socklen = sizeof(sin);
  getsockname(sock_fd, (struct sockaddr *)&sin, &socklen)
  printf("Đang lắng nghe trên cổng %d\n", ntohs(sin.sin_port)) 
````
  
Bây giờ gọi `accept` **chặn** cho đến khi chúng ta có yêu cầu của máy khách để phục vụ. Đối với mỗi máy khách, hãy đợi yêu cầu của trình duyệt web sau đó luôn gửi lại một thông báo thân thiện. Điều này bắt đầu với **tiêu đề phản hồi** bao gồm **MIMETYPE** - loại dữ liệu được biểu diễn bởi các byte theo sau. Tiêu đề phản hồi, giống như tiêu đề yêu cầu được kết thúc bằng hai dòng trống cùng nhau `\r\n\r\n`/ Lưu ý mã này cũng trình bày việc sử dụng `dprintf` của Linux cho phép định dạng giống như `printf` trực tiếp đến một **bộ mô tả tệp**.
```C
  while(1) {
    int client_fd = accept(sock_fd, (struct sockaddr*) &client_info, &size);

    char *connected_ip= inet_ntoa(client_info.sin_addr);
  // ^^^^ Điều này có vẻ an toàn cho luồng đối với bạn không?
    int port = ntohs(client_info.sin_port);


    char buffer[1000];
    read(client_fd, buffer, sizeof(buffer)-1);
    dprintf(client_fd, "HTTP/1.0 200 OK\r\n");
    dprintf(client_fd, "Content-Type: text/html\r\n\r\n");
    dprintf(client_fd, "<html><body><h1>Xin chào</h1></body></html>");
    shutdown(client_fd , SHUT_RDWR);
    close(client_fd);
  }
```

## Làm cách nào để phục vụ một tệp hình ảnh tĩnh đơn giản?
```C
  FILE*file = fopen("apicture.jpg","r");
  if(file) {
    fseek(file,0, SEEK_END);
    long size = ftell(file);
    fseek(file,0,SEEK_SET);
    printf("Đang gửi %ld byte\n", size);

    char*buf = malloc(size);
    fread(buf,1,size,file);

    char response[2048];
    
    sprintf( response, "HTTP/1.0 200 OK\r\n"
             "Content-Type: image/jpeg\r\n"
             "Content-Length: %ld\r\n\r\n" , size);

    write(client_fd, response, strlen(response));
    
    write(client_fd, buf, size);
    fclose(file);
    free(buf);
  }		
  shutdown(client_fd , SHUT_RDWR);
  close(client_fd);
```



