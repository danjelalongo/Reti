#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_error(char *message){
    fprintf(stderr, "Error:  %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){

    int sockfd, n; 
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[BUFSIZ], client_ip[INET_ADDRSTRLEN];

    if (argc < 3) handle_error("argc");

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[2]));

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) handle_error("socket");

    while (1)
    {
        printf("Enter an input: ");
        fgets(buffer, BUFSIZ, stdin);
        buffer[strlen(buffer)-1] = 0;
        if((sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, len)) < 0) handle_error("sendto");

        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&addr, &len)) < 0) handle_error("recvfrom");
        buffer[n] = 0;
        printf("Server: %s\n", buffer);
    }

    close(sockfd);
}
