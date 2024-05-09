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
int torn = 1;
char buff[30]; // paraula rebuda

/**
* Pre: s1 i s2 no són buits
* Post: retorna si s1 i s2 són iguals
*/
bool strIsEqual(char s1[], char s2[]) {
	return strcmp(s1, s2) == 0;
}

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
* Post: Reorganitza els canals dels processos fills
**/
void reorganitzarCanals() {
	// Impedeix que els processos fills puguin llegir de la pipe amb nom i escriure a la pipe interna
	close(fd[1]);
	close(canal);
}

/**
* Pre: Cert
* Post: Tanca els canals oberts i fa exit
**/
void sortir() {
	// Tanca el canal de lectura de la pipe interna abans d'acabar
	close(fd[0]);
	exit(0);
}

/**
* Pre: Cert
* Post: A partir d'una paraula rebuda per la pipe interna, la mostra per pantalla canviant les vocals per numeros
**/
void canviVocals() {
	int nVocals = 5;
	char lletres[] = {'a','e','i','o','u'};
	char numeros[] = {'4','3','1','0','1'};
	
	char paraula[30];
	int j = read(fd[0], paraula, 30); // Rep la paraula de la pipe interna
	
	if (j > 0) { // I si la rebut correctament, llavors
		char encriptada[j]; // Crea la cadena de caràcters on es guardara la paraula amb les vocals canviades per numeros
		
		int y = 0;
		for (int x = 0; x < j; x++) { // I va iterant la paraula, i per cada caràcter
			while (y < nVocals && paraula[x] != lletres[y]) { // Comprova si es una vocal
				y++;
			}
			
			if (y < nVocals) { // Si es una vocal, fa el canvi
				encriptada[x] = numeros[y];
			} else { // Si no, no farà cap canvi
				encriptada[x] = paraula[x];
			}
			
			y = 0;
		}
	
		write(1, encriptada, j); // I finalment, escriu la paraula canviant les vocals per numeros
	}
}

/**
* Pre: Cert
* Post: A partir d'una paraula rebuda per la pipe interna, la mostra per pantalla del revés
**/
void cadenaDelReves() {
	char paraula[30];
	int j = read(fd[0], paraula, 30); // Rep la paraula de la pipe interna
	
	if (j > 0) { // I si la rebut correctament, llavors
		char reves[j+1]; // Crea la cadena de caràcters on es guardara la paraula del revés
		
		int y = 0; // Declara un comptador per llegir la paraula
		for (int x = j - 1; x >= 0; x--) {
			reves[x] = paraula[y]; // I la va invertint caràcter a caràcter
			y++;
		}
		
		reves[j] = '\n';
		
		write(1, reves, j+1); // I finalment, escriu la paraula del revés per pantalla
	}
}

/**
* Pre: Cert
* Post: A partir d'una paraula rebuda per la pipe interna, la mostra per pantalla amb les lletres duplicades
**/
void duplicarLletres() {
	char paraula[30];
	int j = read(fd[0], paraula, 30); // Rep la paraula de la pipe interna
	
	if (j > 0) { // I si la rebut correctament, llavors
		char duplicada[j * 2]; // Crea una paraula que dupliqui la mida de la paraula llegida
		
		int y = 0; // Crea un comptador que anirà sempre de 2 en 2
		for (int x = 0; x < j; x++) {
			// I va iterant la paraula llegida, assignant cada caràcter a les dues posicions contigues de la paraula duplicada
			duplicada[y] = paraula[x];
			duplicada[y + 1] = paraula[x];
			y += 2;
		}
				
		write(1, duplicada, j * 2); // I finalment, escriu la paraula amb les lletres duplicades
	}
}

/**
* Pre: torn >= 1 i torn <= 3
* Post: Realitza la gestió del torn, i desperta el procés que li toca despertar-se seguint una política Round-Robin
**/
void seleccionarProces() {
	if (torn == 1) { // desperta al procés canvi de vocals
		kill(pid1, SIGUSR1);
		torn = 2;
	} else if (torn == 2) { // desperta al procés cadena del revés
		kill(pid2, SIGUSR1);
		torn = 3;
	} else if (torn == 3) { // desperta al procés duplica lletres
		kill(pid3, SIGUSR1);
		torn = 1;
	}
}

/**
* Pre: Cert
* Post: Va llegint paraules d'una pipe amb nom fins que no en rebi cap, i quan rebi una paraula la mostra per pantalla encriptada
**/
int main(int argc, char* argv[]) {
	canal = crearNamedPipe("./canal", O_RDONLY); // Crea i obre una pipe amb nom en mode lectura
	
	estat = pipe(fd); // Crea una pipe que serà la pipe interna
	
	pid1 = fork(); // Crea el procés de canvi de vocals
	if (pid1 == 0) { // Codi del procés canvi de vocals
		reorganitzarCanals(); // Reorganitza els seus canals per tal que només pugui llegir de la pipe interna
		// Programa els signals
		signal(SIGUSR1, canviVocals);
		signal(SIGUSR2, sortir);
		while(true) { // I s'adorm
			pause();
		}
	} else if (pid1 < 0) { // Si hi ha algun error creant el procés
		perror("Error al crear el procés de canvi de vocals");
		exit(1);
	}
	
	pid2 = fork(); // Crea el procés de cadena del revés
	if (pid2 == 0) { // Codi del procés cadena del revés
		reorganitzarCanals(); // Reorganitza els seus canals per tal que només pugui llegir de la pipe interna
		// Programa els signals
		signal(SIGUSR1, cadenaDelReves);
		signal(SIGUSR2, sortir);
		while(true) { // I s'adorm
			pause();
		}
	} else if (pid2 < 0) { // Si hi ha algun error creant el procés
		perror("Error al crear el procés de cadena del revés");
		exit(1);
	}
	
	pid3 = fork(); // Crea el procés de duplica lletres
	if (pid3 == 0) { // Codi del procés duplica lletres
		reorganitzarCanals(); // Reorganitza els seus canals per tal que només pugui llegir de la pipe interna
		// Programa els signals
		signal(SIGUSR1, duplicarLletres);
		signal(SIGUSR2, sortir);
		while(true) { // I s'adorm
			pause();
		}
	} else if (pid3 < 0) { // Si hi ha algun error creant el procés
		perror("Error al crear el procés de duplica lletres");
		exit(1);
	}
	// El pare (l'encripta) reorganitza els seus canals per tal que només pugui escriure a la pipe interna
	close(fd[0]);
	
	i = read(canal, buff, 30); // Rep la primera paraula
	while (i > 0) {		
		write(fd[1], buff, i); // Envia la paraula a través de la pipe interna
		
		seleccionarProces(); // I desperta a un dels processos fills
				
		i = read(canal, buff, 30); // I rep la següent paraula
	}

	// Un cop ja no llegeixi res de la pipe amb nom, significarà que l'entrada s'ha mort, llavors indica als processos fills que s'ha acabat
	kill(pid1, SIGUSR2);
	kill(pid2, SIGUSR2);
	kill(pid3, SIGUSR2);

	// Tanca la pipe interna, i tanca i esborra la pipe amb nom abans d'acabar
	close(fd[1]);
	close(canal);
	unlink("./canal");
}
