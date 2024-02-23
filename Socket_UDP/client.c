/*
    Client:

    0) socket -> crea una socket
    
    1) recvfrom -> riceve dati dal client
    2) sendto -> invia i dati al client

    3) close -> dealloca le risorse
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> // sendto, recvfrom, socket ecc...
#include <arpa/inet.h> // struct sockaddr_in
#include <unistd.h> // close

void handle_error(char* msg){ //prende in input il puntatore al messaggio
    fprintf(stderr, "Error: %s\n", msg); // fprintf è come printf(), ma anziché scrivere sullo standard output (generalmente lo schermo), scrive su un file specificato.
    exit(EXIT_FAILURE); //il programma termina restituendo exit failure
}

int main(int argc, char* argv[]){

    if(argc != 3) handle_error("argc"); // [./client] [127.0.0.1] [5533]
    char* IP = argv[1];
    int PORT = atoi(argv[2]);

    int sockfd, n; // descrittore di socket
    struct sockaddr_in addr; // struttura che modella l'indirizzo IP; il tipo è struct sokaddr_in
    socklen_t len = sizeof(addr); // socklen_t è un intero positivo
    char buffer[BUFSIZ];

    // AF_INET -> IPV4
    // SOCK_DGRAM -> UDP
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) handle_error("socket"); // se restituisce -1 ha fallito

    memset(&addr, 0, len); // pongo a '0' i primi 'len' bytes di 'addr'
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &addr.sin_addr); //presentation-to-network (passa al formato di rete) :setto l'indirizzo
    addr.sin_port = htons(PORT); // host-to-network-short (passa al formato di rete) : setto la porta

    while(1){
        printf("Inserisci richiesta: ");
        fgets(buffer, BUFSIZ, stdin);
        buffer[strlen(buffer)-1] = 0; // tolgo il carattere '\n' dal buffer

        if((sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, len)) < 0) handle_error("sendto");
        if(!(strcmp(buffer, "exit"))) break; // se il client invia "exit" la comunicazione si interrompe
        
        // Conservo in 'n' il numero di bytes ricevuti
        if((n = recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&addr, &len)) < 0) handle_error("recvfrom");
        buffer[n] = 0;
        printf("\nServer: %s\n\n", buffer);
    }
    
    puts("[+]Communication interrupted");
    close(sockfd);
}