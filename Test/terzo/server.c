#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

/*
    1. Estrae l'IP del client dalla richiesta
    2. Scrive l'IP nel file di testo "ip_list.txt" nel formato 'IP: %s\n'
    3. Quando un client già noto invia una richiesta, non si aggiunge l'indirizzo ip alla lista.

    HELPER

    char client_ip[16]; // INET_ADDRSLEN
    inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));

*/

void handle_error(char *message){
    fprintf(stderr, "Error:  %s\n", message);
    exit(EXIT_FAILURE);
}

void write_ip(char *client_ip){

    FILE* fp;
    if((fp = fopen("ip_list.txt", "a+")) == NULL) handle_error("fopen");

    char row[BUFSIZ], *token;
    bool isPresent = false;

    if(!fgets(row, BUFSIZ, fp))
        fprintf(fp, "IP: %s\n", client_ip);

    while(fgets(row, BUFSIZ, fp)){
        token = strtok(row, " ");
        token = strtok(NULL, "\n");
        if (strcmp(token, client_ip) != 0) {
            isPresent = true;
            break;
        }
    }

    if(!isPresent)
        fprintf(fp, "IP: %s\n", client_ip);
}

int main(int argc, char* argv[]) {

    if (argc != 2) handle_error("argc ! Usage ./server port");

    int sockfd, n;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ], client_ip[16]; // INET_ADDRSLEN;
    
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0))<0) handle_error("socket");

    memset(&addr, 0, len);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));

    if((bind(sockfd, (struct sockaddr *)&addr, len))<0) handle_error("bind");
    printf("Server listening...\n\n");

    while(1){
        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr *)&addr, &len))<0) handle_error("recvfrom");
        buffer[n] = 0;
        printf("Client: %s\n", buffer);
        inet_ntop(AF_INET, &addr.sin_addr, client_ip, sizeof(client_ip));
        write_ip(client_ip);
    
        if((sendto(sockfd, "Received", strlen("Received"), 0, (struct sockaddr*)&addr, len))<0) handle_error("sendto");
    }

    close(sockfd);
}