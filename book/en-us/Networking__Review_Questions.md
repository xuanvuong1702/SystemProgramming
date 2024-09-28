* [Wiki w/Interactive MC Questions](http://angrave.github.io/SystemProgramming/networkingreviewquestions.html)
* See [Coding questions](#coding-questions)
* See [Short answer questions](#short-answer-questions)
* See [MP Wearables](https://courses.engr.illinois.edu/cs241/mps/mp7/) Food For Thought questions


# Short answer questions
## Q1
What is a socket?

## Q2 

What is special about listening on port 1000 vs port 2000?

- Port 2000 is twice as slow as port 1000
- Port 2000 is twice as fast as port 1000
- Port 1000 requires root privileges
- Nothing


## Q3 
Describe one significant difference between IPv4 and IPv6

## Q4
When and why would you use ntohs?

## Q5
If a host address is 32 bits which IP scheme am I most likely using? 128 bits?

## Q6
Which common network protocol is packet based and may not successfully deliver the data?

## Q7
Which common protocol is stream-based and will resend data if packets are lost?

## Q8
What is the SYN ACK ACK-SYN handshake?

## Q9
Which one of the following is NOT a feature of TCP? 
- Packet re-ordering
- Flow control
- Packet re-tranmission
- Simple error detection
- Encryption

## Q10
What protocol uses sequence numbers? What is their initial value? And why?

## Q11
What are the minimum network calls are required to build a TCP server? What is their correct order?

## Q12
What are the minimum network calls are required to build a TCP client? What is their correct order?

## Q13
When would you call bind on a TCP client?

## Q14 
What is the purpose of
socket
bind
listen
accept
?

## Q15
Which of the above calls can block, waiting for a new client to connect?

## Q16
What is DNS? What does it do for you? Which of the CS241 network calls will use it for you?

## Q17
For getaddrinfo, how do you specify a server socket?

## Q18
Why may getaddrinfo generate network packets?

## Q19
Which network call specifies the size of the allowed backlog?

## Q20
Which network call returns a new file descriptor?

## Q21
When are passive sockets used?

## Q22
When is epoll a better choice than select? When is select a better choice than epoll?

## Q23
Will  `write(fd, data, 5000)`  always send 5000 bytes of data? When can it fail?

## Q24 
How does Network Address Translation (NAT) work? 


## Q25
@MCQ
Assuming a network has a 20ms Transmit Time between Client and Server, how much time would it take to establish a TCP Connection?
20 ms
40 ms 
100 ms
60 ms @ANS
3 Way Handshake @EXP
@END


## Q26
What are some of the differences between HTTP 1.0 and HTTP 1.1? How many ms will it take to transmit 3 files from server to client if the network has a 20ms transmit time? How does the time taken differ between HTTP 1.0 and HTTP 1.1?


# Coding questions
## Q 2.1

Writing to a network socket may not send all of the bytes and may be interrupted due to a signal. Check the return value of `write` to implement `write_all` that will repeatedly call `write` with any remaining data. If `write` returns -1 then immediately return -1 unless the `errno` is `EINTR` - in which case repeat the last `write` attempt. You will need to use pointer arithmetic.
````C
// Returns -1 if write fails (unless EINTR in which case it recalls write
// Repeated calls write until all of the buffer is written.
ssize_t write_all(int fd, const char *buf, size_t nbyte) {
  ssize_t nb = write(fd, buf, nbyte);
  return nb;
}
````

## Q 2.2
Implement a multithreaded TCP server that listens on port 2000. Each thread should read 128 bytes from the client file descriptor and echo it back to the client, before closing the connection and ending the thread.

## Q 2.3
Implement a UDP server that listens on port 2000. Reserve a buffer of 200 bytes. Listen for an arriving packet. Valid packets are 200 bytes or less and start with four bytes 0x65 0x66 0x67 0x68. Ignore invalid packets. For valid packets add the value of the fifth byte as an unsigned value to a running total and print the total so far. If the running total is greater than 255 then exit.
