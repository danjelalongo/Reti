#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

void handle_error(char *message){
    fprintf(stderr, "Error:  %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){ //argc mi indica il numero di parametri e argv la stringa (e pontatore a caratteri)
    if(argc!=3) handle_error("argc");
    char* IP = argv[1];
    int PORT= atoi (argv[2]); // converto la porta da stringa a intero con atoi
     //      0           1       2
    // [./client] [127.0.0.1] [5533]
    int sockfd, n;
    struct sockaddr_in address; //address variabile di tipo struct sokaddr_in che mi permette di modellare l ip 
    socklen_t len = sizeof(address);
    char buffer[BUFSIZ]; 

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0 ) handle_error("socket");

    memset(&address, 0, len);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &address.sin_addr);


while(1){
    printf("Inserisci la richiesta: ");
    fgets(buffer, BUFSIZ, stdin);
    buffer[strlen(buffer)-1] = 0;

    if((sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&address, len))<0) handle_error("sento");
    if (!(strcmp(buffer, "exit"))) //ritorna zero se le due stringhe sono uguali
        break;

    if ((n=recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&address, &len)) <0) handle_error("receivefrom");
    buffer[n]=0;
    
    printf("Server: %s\n\n", buffer);
}
puts("La comunicazione è stata interrotta");
close(sockfd);
   

}