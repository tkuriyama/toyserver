
# Toy Server 

Toy server exercises in C.


## Echo Server 

An echo server that works for [this simple client](http://www.cs.cmu.edu/afs/cs/academic/class/15213-f00/www/class24code/echoclient.c).

```
√ echoserver % ./echoserver_comp
Client IP address: 127.0.0.1
Client port      : 57890
Echoed message: hello server
Bye.

----

√ echoserver % ./echoclient_comp 127.0.0.1 3490
Please enter msg: hello server
Echo from server: hello server

```


<hr>

## Course Server

Implementing a few API calls from `coursedb`, a toy database from [systems programming exercises](https://www.inf.usi.ch/carzaniga/edu/sysprog17s/exercises.html), for client-server interaction.

```
√ courseserver % ./course_server_comp                                                  
> Started server on 127.0.0.1:3490
> Client connected from: 127.0.0.1:57519
Received: hi
Sent: Command not recognized.

Received: load
Sent: Success

Received: stats
Sent: (stats retrieved and sent).

> Client asked to close connection.
> Success: Closing DB
> Bye.

----

√ courseserver % ./course_client_comp 127.0.0.1 3490
> Connected...
Enter command: hi
Server response: Command not recognized.

Enter command: load
Server response: Success

Enter command: stats 
Server response: 
--- Database Stats
Course ct / active / max:     19 / 19 / 22
Student ct / active / max:    29 / 29 / 33
Enrollment ct / active / max: 81 / 81 / 90
-------------------

Enter command: bye
> Closing connection.
```

<hr>

## References 

* https://beej.us/guide/bgnet/html/
