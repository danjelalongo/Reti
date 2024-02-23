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

    int sockfd, n;
    struct sockaddr_in address; //address variabile di tipo struct sokaddr_in che mi permette di modellare l ip 
    socklen_t len = sizeof(address);
    char buffer[BUFSIZ], row[BUFSIZ]; 

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0 ) handle_error("socket");

    memset(&address, 0, len);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = inet_addr(IP);

    FILE *fd;
    if(!(fd = fopen("file.txt", "r"))) handle_error("fopen");

    while(fgets(row, BUFSIZ, fd)){
        char* token = strtok(row, ";"); // estraggo il nome
        strcpy(buffer, token); // copio in buffer

        if((sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&address, len))<0) handle_error("sento"); // invio buffer

        if ((n=recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&address, &len)) <0) handle_error("receivefrom"); // ricevo feedback
        buffer[n]=0;
        
        printf("Server: %s\n\n", buffer);
    }

    close(sockfd);
}
