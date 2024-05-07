// Juan José Gómez Villegas
// u1987338
// Pràctica 4 - Encripta

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

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
* Pre: Cert
* Post: Reorganitza els canals dels processos fills de l'encripta, per tal que només puguin llegir de la pipe interna
**/
void reorganitzarCanals() {
	close(0);
	dup(fd[0]);
	close(fd[0]);
	close(fd[1]);
	close(canal); // I que tampoc puguin llegir de la pipe amb nom
}

/**
* Pre: Cert
* Post:
**/
void canviVocals() {
	char* paraula;
	int j = read(0, paraula, 30);
	
	if (j > 0) {
		write(1, paraula, j);
	}
}

/**
* Pre: Cert
* Post: A partir d'una paraula rebuda per la pipe interna, la mostra per pantalla del revés
**/
void cadenaDelReves() {
	char* paraula;
	int j = read(0, paraula, 30); // Rep la paraula de la pipe interna
	
	if (j > 0) { // I si la rebut correctament, llavors
		int y = 0; // Declara un comptador per llegir la paraula
		char reves[i]; // I on es guardara la paraula del revés
		for (int x = j - 1; x >= 0; x--) {
			reves[x] = paraula[y]; // I la va invertint caràcter a caràcter
			y++;
		}
		
		write(1, reves, j); // I finalment, escriu la paraula del revés per pantalla
	}
}

/**
* Pre: Cert
* Post:
**/
void duplicarLletres() {
	char* paraula;
	int j = read(0, paraula, 30);
	
	if (j > 0) {
		write(1, paraula, j);	
	}
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
		reorganitzarCanals();
		signal(SIGUSR1, canviVocals);
		pause();
	} else if (pid1 < 0) {
		perror("Error al crear el procés de canvi de vocals");
		exit(1);
	}
	
	pid2 = fork(); // Crea el procés de cadena del revés
	if (pid2 == 0) {
		reorganitzarCanals();
		signal(SIGUSR1, cadenaDelReves);
		pause();
	} else if (pid2 < 0) {
		perror("Error al crear el procés de cadena del revés");
		exit(1);
	}
	
	pid3 = fork(); // Crea el procés de duplica lletres
	if (pid3 == 0) {
		reorganitzarCanals();
		signal(SIGUSR1, duplicarLletres);
		pause();
	} else if (pid3 < 0) {
		perror("Error al crear el procés de duplica lletres");
		exit(1);
	}
	// Reorganitzar els canals del pare, per tal que només pugui escriure a la pipe interna
	close(fd[0]);
	dup(fd[1]);
	close(fd[1]);
	
	i = read(canal, buff, 30); // Rep la primera paraula
	while (i > 0) {
		// Tractar paraula
		write(fd[0], buff, i);
		
		// Despertar a un dels fills
		//kill(pid2, SIGUSR1);
		
		i = read(canal, buff, 30); // Rep la següent paraula
	}

	// Indica als processos fills que s'ha acabat
	kill(pid1, SIGTERM);
	kill(pid2, SIGTERM);
	kill(pid3, SIGTERM);
	close(estat); // Tanca la pipe interna
	close(canal); // Tanca el canal de lectura a la pipe amb nom
	unlink("./canal"); // I esborra la pipe amb nom
}

// crear tres processos dormint
// crear una pipe
// el pare desperta un dels processos en llegir una paraula de la namedpipe
