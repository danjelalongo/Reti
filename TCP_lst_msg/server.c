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

int main(int argc, char* argv[]){

    int sockfd, n;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ];




}