// Juan José Gómez Villegas
// u1987338
// Pràctica 4 - Entrada

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

// Variables
int canal, i;
char buff[30];
char FI[] = "XX";

/**
* Pre: s1 i s2 no són buits
* Post: retorna si s1 i s2 són iguals
*/
bool strIsEqual(char s1[], char s2[]) {
	return strcmp(s1, s2) == 0;
}

/**
* Pre: Cert
* Post: Va llegint paraules del teclat fins a entrar la cadena XX
*/
int main(int argc, char *argv[]) {
	canal = open("./canal", O_WRONLY);
	if (canal < 0) {
		perror("Error al obrir la pipe amb nom");
		exit(1);
	}
	
	do {
		i = read(0, buff, 30);
		write(canal, buff, i);
	} while (!(strIsEqual(buff, FI)));
	
	close(canal);
}
