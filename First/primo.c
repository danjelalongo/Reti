#include <stdio.h>
#include <stdlib.h> //per l'allocazione dinamica
#include <string.h>

int main(int argc, char **argv) {
    //come dichiarare una stringa in c-- >una stringa è un array di caratteri che termina con un carattere nullo '/0'
    //1. stringa statica
    char stringa1[20];
    //2. stringa dinamica
    char *stringa2;
    stringa2= (char *) malloc(20 * sizeof(char) );//stringa2 è un puntatore a carattere che un punta ad un area di 
    //memoria allocata dinamicamente per contenere una stringa di al massimo 19 caratteri piu il terminatore nullo
    //3. inizializzare una stringa
    char stringa3[]="Hello";// quando la dicharo cosi, devo anche inizializzarla. la lunghezza della stringa viene dedotta
    return 0;
}
