#!/usr/bin/env python

#******************************************************************************
# COURSE:   CS372-400 W2018
# PROJECT:  Programming Assignment #1, client-server network application
# AUTHOR:   DREW WOLFE
# RUN:      python chatserver.py <port number>
# NOTES:
# USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports)
# Your terminal will provide an error if the port is already in use.
# The chatclient.c must connect to the same host as that of chatserver.py
# Initiate chatserver.py prior to starting chatclient.c
# Terminate the connection between server and client with "\quit"
# Terminate chatserver.py with CTRL + C
#
# REFERENCES:
# Socket Programming HOWTO
# https://docs.python.org/3/howto/sockets.html 
#
# Internet Protocols and Support
# https://docs.python.org/release/2.6.5/library/internet.html
#
# List of TCP and UDP port numbers
# https://www.wikiwand.com/en/List_of_TCP_and_UDP_port_numbers#
#******************************************************************************

import socket
import os
import sys

# User validation
def validateArgs():
    if len(sys.argv) < 2:
        print "Expected format => python" + " " + sys.argv[0] + " " + "port"
        sys.exit()

# Starts server | Code based on python.org HOWTO docs
def serverStart():
    portnumber = int(sys.argv[1])
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('',portnumber))
    s.listen(1)
    return s 

# Sends to client
def mSend(c):
    fromUser = raw_input(handle + ": ")
    c.send(handle + ": " + fromUser)
    return fromUser

# Receives from client
def mReceive(c):
    return c.recv(500)


#                      FUNCTION CALLS / PROGRAM START
#******************************************************************************

validateArgs()
chatServer = serverStart()

# SIGINT (signal interupt) to close loop
while 1:
    os.system('clear') #tidy up
    # Server handle
    handle = ""
    handle = raw_input("Enter a handle: ")
    print "Sever connection with client by inputting \"\\quit\""
    print "Terminate chatserver.py via CTRL+C"
    print "LISTENING FOR CLIENT..."
    
    waiting = True
    # Client connection acceptance
    conn, addr = chatServer.accept()
    # \quit to close loop
    while 1:
        received = mReceive(conn)
        if "\quit" in received: break
        if waiting:
            os.system('clear') #tidy up
            print "-Messages-"
        waiting = False
        # Print client message
        print received
        sent = mSend(conn)
        if "\quit" in sent: break
    conn.close()