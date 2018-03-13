# 2-Connection Client-Server File Transfer Application  
## CS372-400-W2018 | Programming Assignment 2

### Files:
- ftserver.c
- Makefile
- ftclient.py

### How to Run
- Ensure the _server_ and _client_ files are within separate directories
- Ensure the _Makefile_ is within the _server_ directory
- The _-l_ command allows the _client_ to request a list of the files within the directory where the _server_ file resides
- The _-g_ command allows the _client_ to request the transfer of a file from within the _server’s_ directory

#### ftserver.c
1. `$ make all`
2. `$ ./ftserver <PORT>`  
**EXAMPLE**  
`$ ./ftserver 52364`
3. `$ make clean` Optional

#### ftclient.py
1. `$ python ftclient.py <SERVER HOSTNAME><SERVER PORT><COMMAND><DATA PORT>`  
**EXAMPLES**  
`$ python ftclient.py flip2.engr.oregonstate.edu 52364 -l 52365`  
`$ python ftclient.py flip2.engr.oregonstate.edu 52364 -g small_test.txt 52365`  

### Extra Credit  
- Colored + Blinking text & ASCII emojis (required explicit declaration in `.py` file and escape sequences in both files)

### References
 **REFERENCES: ftserver.c**
`struct addrinfo{};`  
- http://beej.us/guide/bgnet/html/single/bgnet.html#structs

`getaddrinfo()`  
- https://www.wikiwand.com/en/Getaddrinfo (seriously)  
- http://beej.us/guide/bgnet/html/single/bgnet.html#getaddrinfo  

`struct dirent`  
- https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
 
`open()`  
- http://codewiki.wikidot.com/c:system-calls:open

`killSpaces()` | It's C++, but the principle of the new temp string, helped  
- https://stackoverflow.com/questions/35301432/remove-extra-white-spaces-in-c

Clear screen  
- https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c

**REFERENCES: ftclient.py**  
`getClientIP()` | Typically “`get_ip_address()`”, but camelcase people  
- https://www.quora.com/Which-Python-library-allows-us-to-specify-network-interface-to-connect-to-the-Internet  
- http://code.activestate.com/recipes/439094-get-the-ip-address-associated-with-a-network-inter/

*_Foundations of Python Network Programming, Third Edition_*, Brandon Rodes and John Goerzen  
- https://topidesta.files.wordpress.com/2015/10/ptyhon-network-engginering-english-version.pdf

`getsizeof()`  
 - https://docs.python.org/3/library/sys.html#sys.getsizeof

`socket.connect(address)` | Refresher... Right?  
- https://docs.python.org/3/library/socket.html#socket.socket.connect

`open(name, mode)`  
 - https://docs.python.org/2/library/functions.html#open

 Clear screen  
 - https://stackoverflow.com/questions/2084508/clear-terminal-in-python