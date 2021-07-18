## Python Echo Server

From the `asyncio` [stream documentation](https://docs.python.org/3/library/asyncio-stream.html#asyncio-streams).


```shell

% python echoclient.py
Enter message: hello world
Send: hello world
Received: 'hello world'
Enter message: hello again
Send: hello again
Received: 'hello again'
Enter message: bye
Send: bye
Received: 'bye'
Close the connection

% python echoserver.py                                                                                 (master)toyserver
Serving on ('127.0.0.1', 8888)
Received 'hello world' from ('127.0.0.1', 58074)
Send: 'hello world'
Received 'hello again' from ('127.0.0.1', 58074)
Send: 'hello again'
Received 'bye' from ('127.0.0.1', 58074)
Send: 'bye'
Close the connection
```