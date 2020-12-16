#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h> 
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>

int errno;

void error() {
    fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
    exit(1);
}

//function to share a file byte by byte
void sharefile(int sockfd, const char* filepath) {
    int fd = open(filepath, O_RDONLY);
    if(fd < 0) {
        perror("Open failed!\n");
        error();
    }
    
    int plen = strlen(filepath);
    int i = plen - 1;
    //iterating the filepath from the end until '/' character is reached, to find the file name
    while(filepath[i] != '/') { i--; }

    //writing the name of the file to the socket
    if(write(sockfd, (char *) &filepath[i+1], plen - i) < 0) {
        perror("write failed!\n");
        error();
    }

    printf("\nSending %s...\n", &filepath[i+1]);

    char ch;
    int rr;

    //read a byte from the file and write it to socket till the end of file
    while((rr = read(fd, (char *) &ch, 1))) {
        
        if(rr < 0) {
            perror("read failed!\n");
            error();
        }
        
        //write the byte read from the file to the socket
        if(write(sockfd, (char *) &ch, 1) < 0) {
            perror("write failed!\n");
            error();
        }
    }

    printf("\n%s has been sent successfully\n", &filepath[i+1]);

    close(fd);
    close(sockfd);
}

int main(int argc, char* argv[]) {
    int sockfd, newsockfd, portno;

    struct sockaddr_in ser_addr, cli_addr; //to store the address of server and client

    if(argc < 2) {
        printf("Usage: %s portno\n", argv[0]);
        exit(1);
    }

    portno = atoi(argv[1]); //store the port number from the command line

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) 
        error();

    bzero((char *) &ser_addr, sizeof(ser_addr)); //initialize server address to zero

    ser_addr.sin_family = AF_INET; //ipv4 addressing
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //get any address for the host
    ser_addr.sin_port = htons(portno); //convert portno to network byte order

    //bind the socket to the server address
    if(bind(sockfd, (struct sockaddr *) &ser_addr, sizeof(ser_addr)) < 0) 
        error();

    listen(sockfd, 5); //listen to a maximum of 5 clients

    socklen_t clilen = sizeof(cli_addr);

    printf("\nWaiting for connection request...\n");

    //accept a connection from the client
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd < 0)
        error();
    
    printf("\nConnected Successfully!\n\n");
    
    //input the file path from the user
    char filepath[_POSIX_PATH_MAX];
    printf("Enter the path of the file to be shared: ");
    scanf("%[^\n]%*c", filepath);

    sharefile(newsockfd, filepath); //shares file to client

    close(newsockfd);
    close(sockfd);

    return 0;
}