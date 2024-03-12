#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

/*
    Scrivere un server in C che consenta la comunicazione con più client e riceva 
    un messaggio "lst_msg" che viene aggiornato ad ogni richiesta ricevuta.

    Quando un client interrompe la comunicazione (es. tramite l'invio di un opportuno messaggio)
    il server deve comunicare l'ultimo messaggio ricevuto "lst_msg" a tutti gli altri client.

    La comunciazione deve rimanere aperta per consentire l'ingresso di nuovi client. 
    Utilizzare il protocollo TCP con indirizzi IPV4.
*/

void handle_error(char*message){
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

    int sockfd, newsock, n;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[1]));

    bind(sockfd, (struct sockaddr*)&addr, len);
    listen(sockfd, 5);

    printf("[+]Server listening...\n\n");

    newsock = accept(sockfd, (struct sockaddr*)&addr, &len);
    close(sockfd);

    n = recv(newsock, buffer, BUFSIZ, 0);
    buffer[n] = 0;
    printf("Client: %s\n", buffer);

    strcpy(buffer, "Okay");
    send(newsock, buffer, strlen(buffer), 0);
    
    close(newsock);
}