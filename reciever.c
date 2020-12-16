#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
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

//function to read data from socket and store in a file
void recvfile(int sockfd) {
    char ch;
    char filename[_POSIX_PATH_MAX];
    int i = 0;
    //read bytes from socket untill '\0' character is read, which marks the end of the filename
    do {
        if(read(sockfd, (char *) &ch, 1) < 0)
            error();

        filename[i++] = ch; // store the file name 
    }while(ch != '\0');

    printf("\nDo you want to recieve the file %s? (y/n)\n", filename);
    char choice;
    scanf("%c", &choice);

    if(choice != 'y') {
        close(sockfd);
        exit(0);
    }

    //check if the file name recieved is available, if not then prompt user to enter new unique filename
    while(access(filename, F_OK) != -1) {
        printf("\n%s already exists. Please choose a different name: ", filename);
        scanf("%s", filename);
    }

    //creating a new file
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IXUSR);
    if(fd < 0) {
        perror("Open failed\n");
        error();
    }

    //read data byte by byte from socket and write to the new file
    while(read(sockfd, (char *) &ch, 1)) {
        if(write(fd, (char *) &ch, 1) < 0)
            error();
    }

    close(fd);
    printf("%s has been recieved\n", filename);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;

    struct sockaddr_in serv_addr; // to store the server address
    struct hostent *server; // to store information about the server

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }

    portno = atoi(argv[2]); // take portno from the command line

    server = gethostbyname(argv[1]); // returns information about host using the hostname
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    //copy the address of the server from hostent variable to serv_addr 
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno); // store portno in network byte order

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error();

    //connects to the server specified by its address
    if (connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error();
    
    printf("\nConnected Successfully!\n\n");

    recvfile(sockfd); // recieves the file sent by the

    close(sockfd);

    return 0;
}