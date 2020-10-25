## Go Echo Server

Based on Unix domain sockets examples from [Eli Bendersky's post](https://eli.thegreenplace.net/2019/unix-domain-sockets-in-go/).

The server runs in an infinite loop, accepting an arbitrary number of clients. 

```
√ echoserver_go % go run echo_server.go  
2020/10/25 13:23:32 Client Connected [unix]
2020/10/25 13:23:40 Client connection closed [unix]
^Csignal: interrupt

---- 

√ echoserver_go % go run echo_client.go
Enter text to send to server; "bye" to quit...

> Hi            
Echo from server: Hi

> Hello World
Echo from server: Hello World

> bye
√ echoserver_go %

```
