#!/usr/bin/env python
# -*- coding: utf-8 -*-

#******************************************************************************
# COURSE:   CS372-400 W2018
# PROJECT:  Programming Assignment #2, client-server network application
# AUTHOR:   DREW WOLFE
# RUN:      $ python ftclient.py <S-PORT> <S-HOSTNAME> <S-PORT> <COMMAND> <C-PORT>
# NOTES:
# USABLE TCP PORT RANGE: 1024-65535 (49152 - 65535 dynamic, ephemeral ports)
# Your terminal will provide an error if the port is already in use.
# The ftclient.py must connect to the same host as that of ftserver.c
# Initiate ftserver.c prior to starting ftclient.py
# Terminate the connection between server and client with "\quit"
# Terminate ftserver.c with CTRL + C
#
# REFERENCES:
# getClientIP() | Typically "get_ip_address()", but camelcase people
# https://www.quora.com/Which-Python-library-allows-us-to-specify-network-interface-to-connect-to-the-Internet
# http://code.activestate.com/recipes/439094-get-the-ip-address-associated-with-a-network-inter/
#
# "Foundations of Python Network Programming, Third Edition", Brandon Rodes 
# and John Goerzen 
# https://topidesta.files.wordpress.com/2015/10/ptyhon-network-engginering-english-version.pdf
#
# getsizeof()
# https://docs.python.org/3/library/sys.html#sys.getsizeof
#
# socket.connect(address) | Refresher... Right?
# https://docs.python.org/3/library/socket.html#socket.socket.connect
#
# open(name, mode)
# https://docs.python.org/2/library/functions.html#open
#
# Clear screen
# https://stackoverflow.com/questions/2084508/clear-terminal-in-python
#******************************************************************************

import socket
import sys
import time # for "time.sleep"
# import os

# Argument validation
def validateArgs():
    if len(sys.argv) < 4:
        print('\x1b[1;31;40' + 'Not enough arguments:' + '\x1b[0m')
        print len(sys.argv)
        print sys.argv
        print("Expected format => $ python ftclient.py <Server Hostname> <Server Port> <Command -l OR -g> <Client Port>\n")
        print("Example => $ python ftclient.py flip2.engr.oregonstate.edu 52364 -l 52365\n")
        print("argv[1] = Server")
        print("argv[2] = Server port")
        print("argv[3] = Command (-g for file or -l for directory list)")
        print("argv[4] = Filename IFF -g OR Client data port IFF -l")
        print("argv[5] = Client data port when -g")
        print("See README for further help \n \n \n")
        sys.exit()

# Initiates contact w/ server. Server must be running 
def connectServer():
    server = sys.argv[1]
    serverPort = int(sys.argv[2])
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((server, serverPort))
    # print('\x1b[5;31;42m' + 'Success!' + '\x1b[0m')
    print('\x1b[1;36;40m' + 'Client connected to Server'+ '\x1b[0m' + '\n' )
    return s

# Gets IP address of network interface (client in this case)
def getClientIP():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.connect(("8.8.8.8", 80))
    ip = sock.getsockname()[0]
    #print ip
    sock.close()
    return ip

# Sends client credentials and request command to server 
def clientRequest(clientSocket, ip):

    command = sys.argv[3]

    # IPv6 addresses are 128 bits or 8 bytes as opposed to 32 bits of IPv4 addresses
    while(sys.getsizeof(ip) < 17): # Return the size of an object in bytes
        ip += " "
    while(sys.getsizeof(command) < 17):
        command += " "
    print('Sending Client credentials and request' + '\x1b[1;36;40m' + ' (∩｀-´)⊃━'+ '\x1b[5;34;40m' +' ☆ﾟ.*･｡ﾟ' + '\x1b[0m' + '\n')
    
    clientSocket.send(ip + '\n')
    time.sleep(1)
    clientSocket.send(command + '\n')
    time.sleep(1)

    if '-l' in command:
        print("Client requests directory list. \n")
        dataPort = str(sys.argv[4]) # argv[4] = receiving data port in -l case
        while(sys.getsizeof(dataPort) < 3): # Client's data port, 16 bits w/ 8 bit buffer
            dataPort += " "
        clientSocket.send(dataPort)
        return sys.argv[4]
    else:
        print('Client requests file: ' + '\x1b[1;37;40m' + str(sys.argv[4]) + '\x1b[0m' + '\n')
        reqFile = str(sys.argv[4]) # argv[4] = filename requested
        while(sys.getsizeof(reqFile) < 110): # Caps file size of filename
            reqFile += " "
        clientSocket.send(reqFile + '\n')
        time.sleep(1)

        dataPort = str(sys.argv[5]) # argv[5] = receiving port in -g case
        while(sys.getsizeof(dataPort) < 3): # Client's data port, 16 bits w/ 8 bit buffer
            dataPort += " "
        clientSocket.send(dataPort)
        return sys.argv[5]
        

def requestedData(serverDataSock):

    targetFile = open(sys.argv[4], 'a+') # create/open/append a file
    fileBuffer = serverDataSock.recv(20000) # ftserver.c file is 9KB for filesize ref
    targetFile.write(fileBuffer)
    targetFile.close()

def handleServerResponse(s, dataPort):
    
    command = sys.argv[3] # argv[3] = Client user's request for file (-g) or dir list (-l)
   
    dataSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Data socket setup
    dataSocket.bind(('', int(dataPort)))
    dataSocket.listen(1) # Listen for server response
    serverDataSock, addr = dataSocket.accept()

    if command == '-l':
        print("Server directory list: \n")
        listBuffer = serverDataSock.recv(2500) # Approx. size of a page of text 2.5-3KB
        print('\x1b[1;37;40m' + str(listBuffer) + '\x1b[0m')
        time.sleep(2)
    else:
        print('Incomming file from Server: ' + '\x1b[1;37;40m' + str(sys.argv[4]) + '\x1b[1;35;40m' + '（ ^_^）o' + '\x1b[5;33;40m' + '自自' +'\x1b[0m' + '\x1b[1;35;40m' + 'o（^_^ ）' + '\x1b[0m' + '\n')
        requestedData(serverDataSock)

if __name__ == "__main__":
    validateArgs()
    ip = getClientIP()
    s = connectServer()
    dataPort = clientRequest(s, ip)
    handleServerResponse(s, dataPort)
    time.sleep(1)
    # os.system('cls' if os.name == 'nt' else 'clear')
    print('Connection closed'+ '\x1b[1;33;40m' + ' ヽ(´ー｀)ノ' + '\x1b[0m' + '\n\n')
    s.close()