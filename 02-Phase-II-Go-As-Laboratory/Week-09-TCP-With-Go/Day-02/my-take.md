TCP connection can send packet, together by coalescing multiple small packets into a single larger packet to improve network efficiency. moreover there could be that with fixed buffer size, Errors that the data is incomplete.

So, to tackle these problems, we have, Delimiter, length prefixed, or Size fixed size with spaces as 0 padding.

for Delimiter, we can use a specific character to indicate the end of a message. For example, we can use a newline character \n to indicate the end of a message. and \r\n to indicate the end of a message.

also, as conn read and write uses io.Reader and io.Writer, we can use bufio.Scanner to read the data from the connection. and we can use bufio.Writer to write the data to the connection. Which makes it easy, without having to declare and amnage a manual buffer.
