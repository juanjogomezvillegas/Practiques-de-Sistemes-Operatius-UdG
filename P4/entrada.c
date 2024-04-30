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
int i;
char buff[30];
char* FI = "XX";

/**
* Pre: s1 i s2 no són buits
* Post: retorna si s1 i s2 són iguals o no
*/
bool strIsEqual(char* s1, char* s2) {
	return strcmp(s1, s2) == 0;
}

/**
* Pre: Cert
* Post: Va llegint paraules fins a entrar la cadena XX
*/
int main(int argc, char *argv[]) {
	//i = read(0, buff, 30);
	scanf("%s", buff);
	while (!(strIsEqual(buff, FI))) {
		write(1, buff, 30);

		//strcpy(buff, "");

		//printf("\n%s\n", buff);

		//i = read(0, buff, 30);
		scanf("%s", buff);
	}
}
