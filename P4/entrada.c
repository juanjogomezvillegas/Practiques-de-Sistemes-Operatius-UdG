// Juan José Gómez Villegas
// u1987338
// Pràctica 4 - Entrada

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

// Constants
const int MIDA_BUFF = 30;

// Variables
int canal, i; // canal, és la pipe amb nom; i, és el nombre de caràcters llegits
char buff[30]; // buffer on es guardarà la paraula llegida
char FI[] = "XX\n"; // marca de fi

/**
* Pre: s1 i s2 no són buits i són cadenes de caràcter
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
	canal = open("./canal", O_WRONLY, 0); // Obre una pipe amb nom amb mode escriptura
	if (canal < 0) { // I si no ha pogut obrir la pipe, mostra un missatge d'error
		perror("Error al obrir la pipe amb nom");
		exit(1);
	}
	
	// I si tot va bé, es posa a llegir paraules del teclat mentre la cadena entrada no sigui XX (la cadena XX no s'enviarà)
	
	printf("Entra paraules a encriptar (Per acabar entra \"XX\"):\n");
	
	i = read(0, buff, MIDA_BUFF); // Llegeix la primera paraula
	while (!(strIsEqual(buff, FI))) { // Mentre sigui diferent de XX
		write(canal, buff, i); // Envia la paraula a la pipe amb nom
		
		i = read(0, buff, MIDA_BUFF); // I llegeix la següent paraula
		
		buff[i] = '\0'; // I abans de continuar, insereix una marca de fi de cadena a la paraula llegida per poder-la comparar
	}
	
	close(canal); // I abans d'acabar, tanca el canal d'escriptura a la pipe amb nom
}
