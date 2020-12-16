# file-sharing
File sharing application through WLAN/LAN using Unix api's for C

## Project Description:
  A simple application with command line interface to share files across systems belonging to the same network (LAN/ Wireless LAN). The program is written in C language using the unix api’s, so the application can be used only in a Unix platform.
The file sharing mechanism is implemented using the socket api’s for Unix, data is passed across the network using the Transmission Control Protocol.
We require two programs to run the application, one program to run on the machine that acts as a sender and the other one for the receiver.
### Sender​ : 
The sender program acts as the server for the communication, this program must be run if you want to share files from your system. The sender.c file implements the mechanism for sending the file byte by byte across the network.
The sender program does the following:
1. Sets up a server in your machine using the port number given by the user.
2. Waits for a connection request from the receiver/client side.
3. Connects to the receiver process.
4. Shares the name of the file.
5. Shares the data from the file byte by byte.
6. Closes the socket used for communication and terminates.
### Receiver​: 
The receiver program acts as the client for the communication, this program is executed on the receiver system. The receiver.c file implements the mechanism for receiving data from the network and storing it in a new file.
The receiver program does the following:
1. Gets the server address by using the hostname for the server.
2. Sets up a socket and connects it to the server address.
3. Reads the file name from the socket.
4. Checks for the availability of the filename and confirms with the user.
5. Starts reading the data from the socket byte by byte and writes it simultaneously to a
new file.
6. Closes the socket and terminates.

## Usage
1. Compile the file using any C compiler in unix. $ gcc -o sender sender.c
```$ gcc -o receiver receiver.c```
2. Run the executables in your respective sender and receiver machine.
```$ ./sender port```
port - port number (safe to use port numbers greater than 2000).
```$ ./receiver hostname port```
hostname - the name or ip address of the sender/server.
port - port number (same port number used while executing sender).
