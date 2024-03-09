#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_error(char *message){
    fprintf(stderr, "Error:  %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

    if (argc != 3) handle_error("argc ! Usage ./client ip port");

    int sockfd, n;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ];
    
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))<0) handle_error("socket");

    memset(&addr, 0, len);

    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[2]));

    while(1){
        printf("Enter an input: ");
        fgets(buffer, BUFSIZ, stdin);
        if((sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, len))<0) handle_error("sendto");
    
        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr *)&addr, &len))<0) handle_error("recvfrom");
        buffer[n] = 0;
        printf("Server: %s\n\n", buffer);
    }

    close(sockfd);
}