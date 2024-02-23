/*
    Server:

    0) socket -> crea una socket
    1) bind -> espone la socket ai client
    
    2) recvfrom -> riceve dati dal client
       [PROCESSING] -> elabora i dati
    3) sendto -> invia i dati al client

    4) close -> dealloca le risorse
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> // struct sockaddr_in
#include <unistd.h> // close

void handle_error(char* msg){
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void processing(char* buffer, char* outcome){
    sprintf(outcome, "%lu", strlen(buffer));
}

int main(int argc, char* argv[]){

    if(argc != 2) handle_error("argc"); // [./server] [5533]
    int PORT = atoi(argv[1]);

    int sockfd, n; // descrittore di socket
    struct sockaddr_in addr; // struttura che modella l'indirizzo IP; il tipo è struct sokaddr_in
    socklen_t len = sizeof(addr); // socklen_t è un intero positivo
    char buffer[BUFSIZ], outcome[BUFSIZ];

    // AF_INET -> IPV4
    // SOCK_DGRAM -> UDP
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) handle_error("socket");

    memset(&addr, 0, len); // pongo a '0' i primi 'len' bytes di 'addr'
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr); //presentation-to-network (passa al formato di rete) :setto l'indirizzo
    addr.sin_port = htons(PORT); // host-to-network-short (passa al formato di rete) : setto la porta

    if((bind(sockfd, (struct sockaddr*)&addr, len)) < 0) handle_error("bind");
    puts("[+]Server listening...\n");

    while(1){

        // Conservo in 'n' il numero di bytes ricevuti
        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&addr, &len)) < 0) handle_error("recvfrom");
        buffer[n] = 0;

        if(!(strcmp(buffer, "exit"))) break; // se il client invia "exit" la comunicazione si interrompe
        
        printf("Client: %s\n", buffer);
        processing(buffer, outcome);
        printf("Lenght of %s : %s\n\n", buffer, outcome);

        if((sendto(sockfd, outcome, strlen(outcome), 0, (struct sockaddr*)&addr, len)) < 0) handle_error("sendto");
    }

    puts("[+]Communication interrupted");
    close(sockfd);
}