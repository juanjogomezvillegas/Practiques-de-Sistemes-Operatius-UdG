// Juan José Gómez Villegas
// u1987338
// Pràctica 4 - Encripta

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
* Pre:
* Post:
**/
int main(int argc, char *argv[]) {
	if (mkfifo("./canal", 00600) != 0) { // Primer, crea la pipe amb nom
		perror("Error al crear la pipe amb nom");
		exit(1);
	}
	// I obre la pipe amb nom amb mode lectura
	canal = open("./canal", O_RDONLY);
	if (canal < 0) {
		perror("Error al obrir la pipe amb nom");
		exit(1);
	}
		
	do {
		i = read(canal, buff, 30);
		write(1, buff, i);
	} while (!(strIsEqual(buff, FI)));

	close(canal);
}

// crear tres processos dormint
// crear una pipe
// el pare desperta un dels processos en llegir una paraula de la namedpipe
