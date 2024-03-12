#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    int sockfd, n;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[2]));

    connect(sockfd, (struct sockaddr*)&addr, len);
    printf("Connected...\n\n");

    printf("Enter an input: ");
    fgets(buffer, BUFSIZ, stdin);
    buffer[strlen(buffer)-1] = 0;
    
    send(sockfd, buffer, strlen(buffer), 0);

    n = recv(sockfd, buffer, BUFSIZ, 0);
    buffer[n] = 0;
    printf("Server: %s\n", buffer);
    
    close(sockfd);
}