#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_LIVES 3

void handle_error(char* msg){
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

/*
    1) calcola il vincitore
    2) modifica eventualmente le vite
    3) scrive nei buffer l'esito 

    EX: c1_buff = "c1_lives:x c2_lives=y";
*/
void compute_winner(char* c1_buff, char* c2_buff, int* c1_lives, int* c2_lives) {

    if (!strcmp(c1_buff, c2_buff)) // pareggio
    {
        sprintf(c1_buff, "Pareggio --> c1_lives:%d c2_lives=%d", *c1_lives, *c2_lives);
        sprintf(c2_buff, "Pareggio --> c1_lives:%d c2_lives=%d", *c1_lives, *c2_lives);
        return;
    }
        
    // c1 wins
    if ((!strcmp(c1_buff, "carta") && !strcmp(c2_buff, "sasso")) || 
        (!strcmp(c1_buff, "sasso") && !strcmp(c2_buff, "forbice")) || 
        (!strcmp(c1_buff, "forbice") && !strcmp(c2_buff, "carta"))) 
    {
        *c2_lives--;
        sprintf(c1_buff, "C1 wins --> c1_lives:%d c2_lives=%d", *c1_lives, *c2_lives);
        sprintf(c2_buff, "C1 wins --> c1_lives:%d c2_lives=%d", *c1_lives, *c2_lives);
        return;
    }

    *c1_lives--;
    sprintf(c1_buff, "C2 wins --> c1_lives:%d c2_lives=%d", *c1_lives, *c2_lives);
    sprintf(c2_buff, "C2 wins --> c1_lives:%d c2_lives=%d", *c1_lives, *c2_lives);
}

int main(int argc, char*argv[]) {

    int sockfd, newsock, c1, c2, c1_lives, c2_lives, num_reg=0;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    char buffer[BUFSIZ], c1_buff[BUFSIZ], c2_buff[BUFSIZ];

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) handle_error("socket");

    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr);
    addr.sin_port = htons(atoi(argv[1]));

    if((bind(sockfd, (struct sockaddr*)&addr, len)) < 0) handle_error("bind");
    listen(sockfd, 5);
    printf("[+]Server listening...\n\n");

    while(num_reg < 2) {

        newsock=accept(sockfd, (struct sockaddr*)&addr, &len);
        printf("New client connected!\n\n");

        if(num_reg == 0) {
            c1 = newsock;
            num_reg ++;
        }
        if(num_reg == 1) {
            c2 = newsock;
            num_reg ++;
        }
    }
    strcpy(buffer, "Start Match!!!");

    close(sockfd);
    close(newsock);

    printf("START GAME\n\n");
    if((send(c1, buffer, strlen(buffer), 0)) < 0)handle_error("send");
    if((send(c2, buffer, strlen(buffer), 0)) < 0)handle_error("send");

    c1_lives = c2_lives = MAX_LIVES;
    while(c1_lives > 0 && c2_lives > 0) {

        if((recv(c1, c1_buff, BUFSIZ, 0)) < 0)handle_error("recv");
        if((recv(c2, c2_buff, BUFSIZ, 0)) < 0)handle_error("recv");

        compute_winner(c1_buff, c2_buff, &c1_lives, &c2_lives);

        if((send(c1, c1_buff, strlen(c1_buff), 0)) < 0)handle_error("send");
        if((send(c2, c2_buff, strlen(c2_buff), 0)) < 0)handle_error("send");
    }

    if((send(c1, c1_buff, strlen(c1_buff), 0)) < 0)handle_error("send");
    if((send(c2, c2_buff, strlen(c2_buff), 0)) < 0)handle_error("send");

    close(c1);
    close(c2);
}
