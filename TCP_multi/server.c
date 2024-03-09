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

    if(argc < 2) handle_error("argc");

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0) handle_error("socket");//SOCK_STREAM perchè usiamo TCP

    memset(&addr, 0, len);
    addr.sin_family= AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port= htons(atoi(argv[1]));

    if((bind(sockfd, (struct sockaddr*)&addr,len))<0)handle_error("bind");

    listen(sockfd, 5);
    printf("[+]Server listening on port %d...\n\n", atoi(argv[1]));

    while((newsock = accept(sockfd, (struct sockaddr*)&addr, &len))) {
        printf("Client %s connected\n\n", inet_ntoa(addr.sin_addr));

        if(fork() == 0){ // child process

            close(sockfd);

            while(1){
                if((n = recv(newsock, buffer, BUFSIZ, 0)) < 0) handle_error("recv");
                buffer[n] = 0;

                if(!strcasecmp("EXIT", buffer)) break;
                printf("Client: %s\n", buffer);

                doSomething(buffer);

                if((send(newsock, buffer, strlen(buffer), 0)) < 0) handle_error("send");
            }
            printf("\nClient %s disconnected\n\n", inet_ntoa(addr.sin_addr));

            close(newsock);
            exit(EXIT_SUCCESS);

        } else { // father process
            close(newsock);
        }
    }


    

}