#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void handle_error(char *message){
    fprintf(stderr, "Error %s\n", message);
    exit(EXIT_FAILURE);
}

void doSomething(char* buffer){
    sprintf(buffer, "TCP");
}

int main(int argc, char* argv[]){

    int sockfd, newsock, n;
    struct sockaddr_in addr;
    socklen_t len =sizeof(addr);
    char buffer[BUFSIZ];

    if(argc < 3) handle_error("argc");

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) handle_error("socket");//SOCK_STREAM perchè usiamo TCP

    memset(&addr, 0, len);
    addr.sin_family= AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port= htons(atoi(argv[2]));

    if((connect(sockfd, (struct sockaddr*)&addr, len)) < 0) handle_error("connect");
    printf("Connected to server\n\n");

    while(1){
        printf("Enter a request: ");
        fgets(buffer, BUFSIZ, stdin);
        buffer[strlen(buffer)-1] = 0;

        if((send(sockfd, buffer, strlen(buffer), 0)) < 0) handle_error("send");

        if(!strcasecmp("EXIT", buffer)) break;

        if((n = recv(sockfd, buffer, BUFSIZ, 0)) < 0) handle_error("recv");
        buffer[n] = 0;
        printf("Server: %s\n\n", buffer);
    }

    close(sockfd);

}