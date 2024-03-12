#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void handle_error(char *message){
    fprintf(stderr, "Error %s\n", message);
    exit(EXIT_FAILURE);
}

void evaluateLength(char* buffer){
    sprintf(buffer, "Length: %lu", strlen(buffer));
}

int main(int argc, char* argv[]){

    int sockfd, newsock, n;
    struct sockaddr_in6 addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ], ipv6[INET6_ADDRSTRLEN];

    if(argc < 2) handle_error("argc");
    if((sockfd = socket(AF_INET6, SOCK_STREAM, 0))<0) handle_error("socket"); // SOCK_STREAM perchè usiamo TCP

    memset(&addr, 0, len);
    addr.sin6_family= AF_INET6;
    inet_pton(AF_INET6, "::1", &addr.sin6_addr);
    addr.sin6_port= htons(atoi(argv[1]));

    if((bind(sockfd, (struct sockaddr*)&addr,len))<0)handle_error("bind");

    listen(sockfd, 5);
    printf("[+]Server listening on port %d...\n\n", atoi(argv[1]));

    while((newsock = accept(sockfd, (struct sockaddr*)&addr, &len))) {
        
        inet_ntop(AF_INET6, &addr.sin6_addr, ipv6, len);
        printf("Client %s connected\n\n", ipv6);

        if(fork() == 0){ // child process

            close(sockfd);

            while(1){
                if((n = recv(newsock, buffer, BUFSIZ, 0)) < 0) handle_error("recv");
                buffer[n] = 0;

                if(!strcasecmp("EXIT", buffer)) break;
                printf("Client: %s\n", buffer);

                evaluateLength(buffer);

                if((send(newsock, buffer, strlen(buffer), 0)) < 0) handle_error("send");
            }
            printf("\nClient %s disconnected\n\n", ipv6);

            close(newsock);
            exit(EXIT_SUCCESS);

        } else { // father process
            close(newsock);
        }
    }


    

}