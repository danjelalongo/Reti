#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define handle_error(msg)                       \
do{                                             \
    fprintf(stderr, "[%d] error ", __LINE__);   \
    perror(msg);                                \
    exit(EXIT_FAILURE);                         \
}while(1);

void processing(char* names[]){
    FILE* fd;
    if(!(fd=fopen("output.txt", "w"))) handle_error("fopen");

    for(int i=0; i<10; i++)
        fprintf(fd, "%d° nome: %s\n", i+1, names[i]);

    fclose(fd);
    
}

int main(int argc, char* argv[]){

    if (argc!=2) handle_error("argc");
    int PORT = atoi(argv[1]);

    int sockfd, n;
    struct sockaddr_in address;
    socklen_t len = sizeof(address); 
    char buffer[BUFSIZ];
    char* names[10];

    if ((sockfd=socket(AF_INET, SOCK_DGRAM, 0))<0) handle_error("socket");

    memset(&address, 0, len);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("0.0.0.0");
    address.sin_port = htons(PORT);

    if((bind(sockfd, (struct sockaddr*)&address, len)) < 0) handle_error("bind");
    printf("[+]Server listening...\n\n");

    for (int i = 0; i < 10; i++) 
        names[i] = malloc(BUFSIZ); // alloco 10 stringhe lunghe BUFSIZ

    int idx=0;
    while(1){
        if((n=recvfrom(sockfd, buffer, BUFSIZ, 0, (struct sockaddr*)&address, &len)) < 0) handle_error("recvfrom");
        buffer[n] = 0;

        printf("Client: %s\n", buffer);
        strcpy(names[idx], buffer);
        sprintf(buffer, "Ricevuto nome '%s'", names[idx]);
        idx++;

        if(idx == 10) break;
        if((sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&address,len)) < 0) handle_error("sendto");
    }

    close(sockfd);

    processing(names);
    puts("\nIl file è stato riempito correttamente:\n");
    system("cat output.txt");
}