#include <stdlib.h> // per allocare memoria dinamica
/* malloc, free per allocare e deallocare memoria dinamica
atoi, atol, atof per convertire le stringhe in interi o float
rand, srand per generare numeri pseudocasuali
*/
#include <stdio.h> // printf, scanf, fgets
#include <string.h>

typedef struct persona { // con typedef creo un alias di persona cioè Persona così che 
// invece di dover dichiarare variabili con struct persona, posso semplicemente usare Persona
    char* name;
    char* lastname;
    int age;
}Persona;

void printPersona(Persona p) {
    printf("Name: %s\n", p.name);
    printf("Lastname: %s\n", p.lastname);
    printf("Age: %d\n", p.age);
}

int main(int argc, char* argv[]){

    /*
        int argc -> numero di argomenti passati al main (comprende il nome dell'eseguibile cioè "./main")
        char* argv[] -> vettore di elementi passati al main (vettore di stringhe)
    */
    
    if(argc > 1) {
        printf("argc: %d\n", argc);
        printf("argv: [ "); 
        for(int i=0; i<argc; i++)
            printf("[%s] ", argv[i]);
        puts("] \n");// se c'è solo testo allora posso stampare con puts
    }

    FILE* fd; //puntatore a file di nome fd (file pointer)

    // mi assicuro che non ci siano errori nell'apertura del file
    if(!(fd = fopen("file.txt", "r"))) { //apre il file, r significa modalità read, w wright, a entrambi, a+ se non esiste lo crea
        fprintf(stderr, "Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    } 

    Persona* vet = malloc(10 * sizeof(Persona));
    char row[BUFSIZ]; // conterrà una generica riga letta
    int i=0;
    while(fgets(row, BUFSIZ, fd)) { // ritorna NULL se termina il file
        vet[i].name = strtok(row, ";"); //in name ho quello che viene prima di ;
        vet[i].lastname = strtok(NULL, ";");
        vet[i].age = atoi(strtok(NULL, "\n")); 
        i++;
    }
    
    for(int i=0; i<10; i++) {
        printf("%d)", i+1);
        printPersona(vet[i]);
        printf("\n");
    }
}

/*
    stdint -->0 standard di input
    stdout -->1 standard di output
    stderr -->2 standard di errore
*/