# Course Server

Implementing client-server interactions in C for a few API calls from `coursedb`, a toy database from [systems programming exercises](https://www.inf.usi.ch/carzaniga/edu/sysprog17s/exercises.html).


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
