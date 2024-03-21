#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

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

void add_ip(char *client_ip){
    FILE* fp;
    if((fp = fopen("ip_list.txt", "a+"))==NULL) handle_error("fopen");
    rewind(fp); // mi riporto ad inizio file

    char row[BUFSIZ];
    bool is_present = false;
    if(!fgets(row, BUFSIZ, fp)){ // file vuoto
        fprintf(fp, "%s\n", client_ip);
        fflush(fp); // sincronizza le scritture in sospeso
        return;
    }
    
    while(fgets(row, BUFSIZ, fp)) { // legge al massimo BUFSIZ caratteri da fp e li scrive in row 
                                    // fino a "\n" . --> NB : include lo "\n" come ultimo carattere
        row[strlen(row)-1] = 0;
        if (strcmp(client_ip, row) == 0) 
            is_present = true;
    }

    if (is_present == false) 
        fprintf(fp, "%s\n", client_ip);

    fflush(fp);
}

void send_broadcast(){}

int main(int argc, char* argv[]) {

    int sockfd, newsock, n;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ], ip[INET_ADDRSTRLEN], last_message[BUFSIZ];
    int max_clients = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[1]));

    bind(sockfd, (struct sockaddr*)&addr, len);
    listen(sockfd, 5);

    printf("[+]Server listening...\n\n");

    
    while(1) {
        newsock = accept(sockfd, (struct sockaddr*)&addr, &len);

        if (fork()==0){ //processo figlio
        inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);// estraggo l'ip di chi si è collegato
        printf("Client %s connected\n\n", ip);
       
       
        while(1) {
            n = recv(newsock, buffer, BUFSIZ, 0);
            buffer[n] = 0;

            if(!strcasecmp(buffer, "exit")) {
                printf("\nClient %s disconnected\n", ip);
                send_broadcast();
                close(newsock);
                break;
            }
            
            printf("Client: %s\n", buffer);
            strcpy(last_message, buffer); // Aggiorna l'ultimo messaggio ricevuto

            add_ip(ip);

            strcpy(buffer, "IP saved succesfully");
            send(newsock, buffer, strlen(buffer), 0);
            close(newsock);
        }
    }

    close(sockfd);
    }
    return 0;
}