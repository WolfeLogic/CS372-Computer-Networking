# Client-Server Network Application
## OSU CS372-400-W2018 | Programming Assignment 1

### chatserver.py
- Ideally run _chatserver.py_ on a separate host from _chatclient.c_ (i.e. `flip3.engr.oregonstate.edu` & `flip2.engr.oregonstate.edu`)
- USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports)
- Your terminal will provide an error if the port is already in use. Simply try another one if occupied.
- The _chatclient.c_ must connect to the same host as that of _chatserver.py_
- Initiate _chatserver.py_ prior to starting _chatclient.c_
- Terminate the connection between server and client with `\quit`
- Terminate _chatserver.py_ with CTRL + C

*RUN chatserver.py* 
```$ python chatserver.py <port number>```
EXAMPLE:
```$ python chatserver.py 52364```

### chatclient.c
- Ensure _Makefile_ resides within the same directory as _chatclient.c_
- Ideally run _chatclient.c_ on a separate host from _chatserver.py_ (i.e. `flip3.engr.oregonstate.edu` & `flip2.engr.oregonstate.edu`)
- USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports)
- The _chatclient.c_ *must connect to the same host* as that of _chatserver.py_
 - Initiate _chatserver.py_ prior to starting _chatclient.c_
 - Terminate _chatclient.c_ with `\quit`

*RUN chatclient.c*
```$ make all```
```$ ./chatclient <server hostname> <port number>```
EXAMPLE:
```$ ./chatclient flip3.engr.oregonstate.edu 52364```
```$ make clean``` deletes _chatclient_ (executable)

### EXTRAS:
- Neither server nor client _handles_ are hard-coded
