# Protocols

## How does TCP initiate a communication channel between sockets?

## Which POSIX call causes the first syn packet to be sent to the server?

The first outgoing packet is sent by the client when the client calls `connect`

```C
socket(...) <- A socket is created but no actual connection has been performed yet.
connect(fd,...) <- Initiate connection
```

## Subverting protocols case study: Denial of Service
Syn flood
Distributed Denial of Service
Internet of Things Denial of Service

## TCP Latency

Save the astronaut! The moon is 1.3 light seconds distant. The TCP client is on the Earth and a lunar console runs a TCP server. Assume a new TCP connection is required each time. How many seconds elapse between wanting to send a CLOSE-AIRLOCK message and the server receiving the data?

```C
fd=socket(...)
connect(fd,...,...)
write(fd,"CLOSE-AIRLOCK!",14);
```
Answer: 3.9 seconds.
TODO: Explain why. Create diagram

How many seconds elapse between requesting data from the server and receiving the result?

```C
fd= socket(...)
connect(fd,...,...)
write(fd,"READ-TEMP!",10);
bytes= read(fd,buffer,256);
```
Answer: 5.2 seconds. Todo Explain why.

## TCP and HTTP/1.0 performance

If the client-server round trip time is 10 milliseconds, what is the minimum time required to display a web page with an image? Assume HTTP/1.0 and the image requires a separate HTTP request.


## HTTP/1.1 performance improvements

## HTTP/2.0 performance improvements

## QUIC protocol


