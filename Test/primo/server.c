#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_error(char* message){
    fprintf(stderr, "Error: %s", message);
    exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]){
    if(argc!=2) handle_error("argc");
    int PORT = atoi(argv[1]);

    int sockfd, n;
    struct sockaddr_in address;
    socklen_t len= sizeof(address);
    char buffer[BUFSIZ], outcome[BUFSIZ];


    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0) handle_error("socket");

    memset(&address, 0, len);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    address.sin_port =htons(PORT);

    if ((bind(sockfd, (struct sockaddr*)&address, len))<0) handle_error("bind");

    while(1){
        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&address, &len))<0) handle_error("recvfrom");
        buffer[n] = 0;

        printf("%s", buffer);
        if(( sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&address, len))<0) handle_error("sendto");
    }
    close(sockfd);

}