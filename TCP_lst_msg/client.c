#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_error(char*message){
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

    int sockfd, n, newsock;
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
    if (fork()==0) {// child
        while(1){
            printf("Enter an input: ");
            fgets(buffer, BUFSIZ, stdin);
            buffer[strlen(buffer)-1] = 0;
            
            send(sockfd, buffer, strlen(buffer), 0);
            if(!strcasecmp(buffer, "exit")) // confronto case-insensitive quindi se è grande o piccolo va bene
                break;

            n = recv(sockfd, buffer, BUFSIZ, 0);
            buffer[n] = 0;
            printf("Server: %s\n\n", buffer);
        }
    
    printf("\nConnection interrupted\n");
    close(sockfd);
    } else {
     if ((bind(newsock, (struct sockaddr*)&addr,len))<0) handle_error("bind");
        recv(newsock, buffer, BUFSIZ, 0);
        close(newsock); }
}

