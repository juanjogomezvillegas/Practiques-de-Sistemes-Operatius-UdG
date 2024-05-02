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
int canal, i; // canal = pipe amb nom; i = caràcters llegits
int pid1, pid2, pid3; // pids dels processos fills
int fd[2], estat; // pipe interna
char buff[30]; // paraula rebuda

/**
* Pre: nom i mode no són buits
* Post: Crea una pipe amb el nom indicat i l'obre també amb el mode indicat (lectura o escriptura)
**/
int crearNamedPipe(char* nom, mode_t mode) {
	int aux = mknod(nom, S_IFIFO|0600, 0); // Crea la pipe amb nom
	
	if (aux != 0) { // I mostra si hi ha algun error
		perror("Error al crear la pipe amb nom");
		exit(1);
	}
	
	aux = open(nom, mode, 0); // I si tot ha anat bé, obre la pipe amb nom amb el mode indicat
	
	if (aux < 0) { // I mostra si hi ha algun error
		perror("Error al obrir la pipe amb nom");
		exit(1);
	}
	
	return aux;
}

/**
* Pre:
* Post:
**/
int main(int argc, char* argv[]) {
	canal = crearNamedPipe("./canal", O_RDONLY); // Crea i obre la pipe amb nom en mode lectura
	
	estat = pipe(fd); // Crea la pipe interna
	
	pid1 = fork(); // Crea el procés de canvi de vocals
	if (pid1 == 0) {
		wait(2);
		printf("Soc el 1");
		wait(2);
	} else if (pid1 < 0) {
		perror("Error al crear el procés de canvi de vocals");
		exit(1);
	}
	
	pid2 = fork(); // Crea el procés de cadena del revés
	if (pid2 == 0) {
		wait(2);
		printf("Soc el 2");
		wait(2);
	} else if (pid1 < 0) {
		perror("Error al crear el procés de cadena del revés");
		exit(1);
	}
	
	pid3 = fork(); // Crea el procés de duplica lletres
	if (pid3 == 0) {
		wait(2);
		printf("Soc el 3");
		wait(2);
	} else if (pid1 < 0) {
		perror("Error al crear el procés de duplica lletres");
		exit(1);
	}
	
	i = read(canal, buff, 30); // Rep la primera paraula
	while (i > 0) {
		// Tractar paraula
		write(1, buff, i);
		i = read(canal, buff, 30); // Rep la següent paraula
	}

	// Mata als processos fills
	kill(pid1, 9);
	kill(pid2, 9);
	kill(pid3, 9);
	close(estat); // Tanca la pipe interna
	close(canal); // Tanca el canal de lectura a la pipe amb nom
}

// crear tres processos dormint
// crear una pipe
// el pare desperta un dels processos en llegir una paraula de la namedpipe
