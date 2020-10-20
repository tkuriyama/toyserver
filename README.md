
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


<hr>

## References 

* https://beej.us/guide/bgnet/html/
