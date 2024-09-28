## `socket`

`int socket(int domain, int socket_type, int protocol);`

Socket creates a socket with domain (e.g. AF_INET for IPv4 or AF_INET6 for IPv6), `socket_type` is whether to use UDP or TCP or other socket type, `protocol` is an optional choice of protocol configuration (for our examples this we can just leave this as 0 for default). This call creates a socket object in the kernel with which one can communicate with the outside world/network. 
You can use the result of `getaddressinfo` to fill in the `socket` parameters, or provide them manually.

The socket call returns an integer - a file descriptor - and, for TCP clients, you can use it like a regular file descriptor i.e. you can use `read` and `write` to receive or send packets.

TCP sockets are similar to `pipes` except that they allow full duplex communication i.e. you can send and receive data in both directions independently.

## `getaddressinfo`

We saw this in the last section! You're experts at this. 

## `connect`

`int connectok = connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`

Pass `connect` the socket file descriptor, the address you want to connect to and the length in bytes of the address structure. To help identify errors and mistakes it is good practice to check the return value of all networking calls, including `connect`

## `read`/`write`

Once we have a successful connection we can read or write like any old file descriptor. Keep in mind if you are connected to a website, you want to conform to the HTTP protocol specification in order to get any sort of meaningful results back. There are libraries to do this, usually you don't connect at the socket level because there are other libraries or packages around it.

The number of bytes read or written may be smaller than expected. Thus it is important to check the return value of read and write. 

## Complete Simple TCP Client Example

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int s;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_INET; /* IPv4 only */
    hints.ai_socktype = SOCK_STREAM; /* TCP */

    s = getaddrinfo("www.illinois.edu", "80", &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(1);
    }

    if (connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1) {
        perror("connect");
        exit(2);
    }

    char *buffer = "GET / HTTP/1.0\r\n\r\n";
    printf("SENDING: %s", buffer);
    printf("===\n");

    // For this trivial demo just assume write() sends all bytes in one go and is not interrupted

    write(sock_fd, buffer, strlen(buffer));

    char resp[1000];
    int len = read(sock_fd, resp, 999);
    resp[len] = '\0';
    printf("%s\n", resp);

    return 0;
}
```

Example output:
```
SENDING: GET / HTTP/1.0

===
HTTP/1.1 200 OK
Date: Mon, 27 Oct 2014 19:19:05 GMT
Server: Apache/2.2.15 (Red Hat) mod_ssl/2.2.15 OpenSSL/1.0.1e-fips mod_jk/1.2.32
Last-Modified: Fri, 03 Feb 2012 16:51:10 GMT
ETag: "401b0-49-4b8121ea69b80"
Accept-Ranges: bytes
Content-Length: 73
Connection: close
Content-Type: text/html

Provided by Web Services at Public Affairs at the University of Illinois
```

## Comment on HTTP request and response
The example above demonstrates a request to the server using Hypertext Transfer Protocol.
A web page (or other resources) are requested using the following request:
```
GET / HTTP/1.0

```
There are four parts (the method e.g. GET,POST,...); the resource (e.g. / /index.html /image.png); the proctocol "HTTP/1.0" and two new lines (\r\n\r\n)


The server's first response line describes the HTTP version used and whether the request is successful using a 3 digit response code:
```
HTTP/1.1 200 OK
```
If the client had requested a non existing file, e.g. `GET /nosuchfile.html HTTP/1.0`
Then the first line includes the response code is the well-known `404` response code:
```
HTTP/1.1 404 Not Found
```


