#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_error(char *message){
    fprintf(stderr, "Error:  %s\n", message);
    exit(EXIT_FAILURE);
}

void save_addr(char* client_ip){
    FILE* fp;
    if((fp = fopen("addr_list.txt", "a+")) == NULL) handle_error("fopen");

    fprintf(fp, "IP: %s\n", client_ip);
    fclose(fp);
}

int main(int argc, char* argv[]){

    int sockfd, n; 
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    char buffer[BUFSIZ], client_ip[INET_ADDRSTRLEN];

    if (argc < 2) handle_error("argc");

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    // addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) handle_error("socket");

    if((bind(sockfd, (struct sockaddr*)&addr, len)) < 0) handle_error("bind");
    printf("[+]Server listening...\n\n");

    while (1)
    {
        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&addr, &len)) < 0) handle_error("recvfrom");
        buffer[n] = 0;
        printf("Client: %s\n", buffer);

        inet_ntop(AF_INET, &addr.sin_addr, client_ip,  INET_ADDRSTRLEN); // salva l'indirizzo IP della richiesta in "client_ip"
        save_addr(client_ip);

        if((sendto(sockfd, "IP saved succesfully\n", strlen("IP saved succesfully\n"), 0, (struct sockaddr*)&addr, len)) < 0) handle_error("sendto");
    }

    close(sockfd);
}
