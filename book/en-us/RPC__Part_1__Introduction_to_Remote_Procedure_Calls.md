
## What is RPC? 

Remote Procedure Call. RPC is the idea that we can execute a procedure (function) on a different machine. In practice the procedure may execute on the same machine, however it may be in a different context - for example under a different user with different permissions and different lifecycle.

## What is Privilege Separation?

The remote code will execute under a different user and with different privileges from the caller. In practice the remote call may execute with more or fewer privileges than the caller. This in principle can be used to improve the security of a system (by ensuring components operate with least privilege). Unfortunately, security concerns need to be carefully assessed to ensure that RPC mechanisms cannot be subverted to perform unwanted actions. For example, an RPC implementation may implicitly trust any connected client to perform any action, rather than a subset of actions on a subset of the data.

## What is stub code? What is marshalling?

The stub code is the necessary code to hide the complexity of performing a remote procedure call. One of the roles of the stub code is to _marshall_ the necessary data into a format that can be sent as a byte stream to a remote server.

````C
// On the outside 'getHiscore' looks like a normal function call
// On the inside the stub code performs all of the work to send and receive the data to and from the remote machine.

int getHiscore(char* game) {
  // Marshall the request into a sequence of bytes:
  char* buffer;
  asprintf(&buffer,"getHiscore(%s)!", game);

  // Send down the wire (we do not send the zero byte; the '!' signifies the end of the message)
  write(fd, buffer, strlen(buffer) );

  // Wait for the server to send a response
  ssize_t bytesread = read(fd, buffer, 8); // we know buffer will be at least 8 bytes and that's sufficient.

  // No error checking in this example!
  // Example: unmarshal the bytes received back from text into an int
  buffer[bytesread] = 0; // Turn the result into a C string

  int score= atoi(buffer);
  free(buffer);
  return score;
}
````

## What is server stub code? What is unmarshalling?
The server stub code will receive the request, unmarshall the request into a valid in-memory data, call the underlying implementation, and send the result back to the caller.

## How do you send an int? float? a struct?  A linked list? A graph?
To implement RPC you need to decide (and document) which conventions you will use to serialize the data into a byte sequence. Even a simple integer has several common choices:
* Signed or unsigned?
* ASCII
* Fixed number of bytes or variable depending on magnitude
* Little or Big endian binary format?

To marshall a struct, decide which fields need to be serialized. It may not be necessary to send all data items (for example, some items may be irrelevant to the specific RPC or can be re-computed by the server from the other data items present).

To marshall a linked list it is unnecessary to send the link pointers- just stream the values. As part of unmarshalling the server can recreate a linked list structure from the byte sequence.

By starting at the head node/vertex, a simple tree can be recursively visited to create a serialized version of the data. A cyclic graph will usually require additional memory to ensure that each edge and vertex is processed exactly once.

## What is an IDL (Interface Description Language)?

Writing stub code by hand is painful, tedious, error prone, difficult to maintain and difficult to reverse engineer the wire protocol from the implemented code. A better approach is specify the data objects, messages and services and automatically generate the client and server code.

A modern example of an Interface Description Language is Google's Protocol Buffer .proto files.

## Complexity and challenges of RPC vs local calls?

Remote Procedure Calls are significantly slower (10x to 100x) and more complex than local calls. An RPC must marshall data into a wire-compatible format. This may require multiple passes through the data structure, temporary memory allocation and transformation of the data representation.

Robust RPC stub code must intelligently handle network failures and versioning. For example, a server may have to process requests from clients that are still running an early version of the stub code.

A secure RPC will need to implement additional security checks (including authentication and authorization), validate data and encrypt communication between the client and host.

## Transferring large amounts of structured data

Let's examine three methods of transferring data using 3 different formats - JSON, XML and Google Protocol Buffers. JSON and XML are text-based protocols. Examples of JSON and XML messages are below.
````xml
<ticket><price currency='dollar'>10</price><vendor>travelocity</vendor></ticket>
````

````javascript
{ 'currency':'dollar' , 'vendor':'travelocity', 'price':'10' }
````

Google Protocol Buffers is an open-source efficient binary protocol that places a strong emphasis on high throughput with low CPU overhead and minimal memory copying. Implementations exist for multiple languages including Go, Python, C++ and C. This means client and server stub code in multiple languages can be generated from the .proto specification file to marshall data to and from a binary stream.

Google Protocol Buffers reduces the versioning problem by ignoring unknown fields that are present in a message. See the introduction to Protocol Buffers for more information.

[[https://developers.google.com/protocol-buffers/docs/overview]]
