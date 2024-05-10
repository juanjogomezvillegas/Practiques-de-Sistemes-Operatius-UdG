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

// Constants
const int MIDA_BUFF = 30;

// Variables
int canal, i; // canal, és la pipe amb nom; i, és el nombre de caràcters llegits
int pid1, pid2, pid3; // pids dels processos fills
int fd[2], estat; // pipe interna
int torn = 1; // comptador que indica a quin procés es cal despertar
char buff[30]; // paraula rebuda

/**
* Pre: s1 i s2 no són buits i són cadenes de caràcter
* Post: retorna si s1 i s2 són iguals
*/
bool strIsEqual(char s1[], char s2[]) {
	return strcmp(s1, s2) == 0;
}

/**
* Pre: nom i mode no són buits, nom és una cadena de caràcters, i mode és el mode amb el qual obrirà la pipe amb nom
* Post: crea una pipe amb el nom indicat i l'obre també amb el mode indicat (lectura o escriptura)
**/
int crearNamedPipe(char* nom, mode_t mode) {
	int aux = mknod(nom, S_IFIFO|0600, 0); // crea una pipe amb nom
	
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
* Pre: cert
* Post: reorganitza els canals dels processos fills
**/
void reorganitzarCanals() {
	// tanca els canals oberts tant el de lectura de la pipe amb nom, com el d'escriptura a la pipe interna
	close(fd[1]);
	close(canal);
}

/**
* Pre: cert
* Post: tanca els canals oberts i fa exit
**/
void sortir() {
	// tanca el canal de lectura de la pipe interna abans d'acabar
	close(fd[0]);
	exit(0);
}

/**
* Pre: cert
* Post: a partir d'una paraula rebuda per la pipe interna, la mostra per pantalla canviant les vocals per números
**/
void canviVocals() {
	int nVocals = 5;
	char lletres[] = {'a','e','i','o','u'};
	char numeros[] = {'4','3','1','0','1'};
	
	char paraula[MIDA_BUFF];
	int j = read(fd[0], paraula, MIDA_BUFF); // rep la paraula de la pipe interna
	
	if (j > 0) { // i si la rebut correctament, llavors
		char encriptada[j]; // crea la cadena de caràcters on es guardara la paraula amb les vocals canviades
		
		int y = 0; // crea un comptador per iterar les vocals i els números
		for (int x = 0; x < j; x++) { // i va iterant per cada caràcter
			while (y < nVocals && paraula[x] != lletres[y]) { // comprovant si es una vocal
				y++;
			}
			
			if (y < nVocals) { // si es una vocal, fa el canvi
				encriptada[x] = numeros[y];
			} else { // si no, no farà cap canvi
				encriptada[x] = paraula[x];
			}
			
			y = 0; // reinicia el comptador de vocals pel següent caràcter
		}
	
		write(1, encriptada, j); // i finalment, escriu la paraula amb les vocals canviades per números
	}
}

/**
* Pre: cert
* Post: a partir d'una paraula rebuda per la pipe interna, la mostra per pantalla del revés
**/
void cadenaDelReves() {
	char paraula[MIDA_BUFF];
	int j = read(fd[0], paraula, MIDA_BUFF); // rep la paraula de la pipe interna
	
	if (j > 0) { // i si la rebut correctament, llavors
		char reves[j+1]; // crea la cadena de caràcters on es guardara la paraula del revés
		
		int y = 0; // crea un comptador per llegir la paraula
		for (int x = j - 1; x >= 0; x--) {
			reves[x] = paraula[y]; // i va llegint paraula de 0 fins a n-1, i reves de n-1 fins a 0, i va assignant cada caràcter de paraula a reves
			y++;
		}
		
		reves[j] = '\n'; // afegeix un salt de línia a la paraula del revés només per a que es mostri correctament a la pantalla
		
		write(1, reves, j+1); // i finalment, escriu la paraula del revés per pantalla
	}
}

/**
* Pre: cert
* Post: a partir d'una paraula rebuda per la pipe interna, la mostra per pantalla amb les lletres duplicades
**/
void duplicarLletres() {
	char paraula[MIDA_BUFF];
	int j = read(fd[0], paraula, MIDA_BUFF); // rep la paraula de la pipe interna
	
	if (j > 0) { // i si la rebut correctament, llavors
		char duplicada[j * 2]; // crea una paraula que dupliqui la mida de la paraula llegida
		
		int y = 0; // crea un comptador que anirà de 2 en 2
		for (int x = 0; x < j; x++) {
			// i va iterant la paraula llegida, assignant cada caràcter a les dues posicions (y i y+1) de la paraula duplicada
			duplicada[y] = paraula[x];
			duplicada[y + 1] = paraula[x];
			y += 2;
		}
				
		write(1, duplicada, j * 2); // i finalment, escriu la paraula amb les lletres duplicades
	}
}

/**
* Pre: torn >= 1 i torn <= 3
* Post: realitza la gestió del torn, i desperta el procés que li toca despertar-se seguint una política Round-Robin
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
* Pre: cert
* Post: va llegint paraules d'una pipe amb nom fins que no en rebi cap, i quan rep una paraula la mostra per pantalla encriptada
**/
int main(int argc, char* argv[]) {
	canal = crearNamedPipe("./canal", O_RDONLY); // crea i obre una pipe amb nom en mode lectura
	
	estat = pipe(fd); // crea una pipe normal, que serà la pipe interna
	
	pid1 = fork(); // crea el procés de canvi de vocals
	if (pid1 == 0) { // codi del procés fill
		reorganitzarCanals(); // reorganitza els seus canals per tal que només pugui llegir de la pipe interna
		// programa els signals
		signal(SIGUSR1, canviVocals);
		signal(SIGUSR2, sortir);
		while(true) { // i s'adorm
			pause();
		}
	} else if (pid1 < 0) { // si hi ha algun error creant el procés
		perror("Error al crear el procés de canvi de vocals");
		exit(1);
	}
	
	pid2 = fork(); // crea el procés de cadena del revés
	if (pid2 == 0) { // codi del procés fill
		reorganitzarCanals(); // reorganitza els seus canals per tal que només pugui llegir de la pipe interna
		// programa els signals
		signal(SIGUSR1, cadenaDelReves);
		signal(SIGUSR2, sortir);
		while(true) { // i s'adorm
			pause();
		}
	} else if (pid2 < 0) { // si hi ha algun error creant el procés
		perror("Error al crear el procés de cadena del revés");
		exit(1);
	}
	
	pid3 = fork(); // crea el procés de duplica lletres
	if (pid3 == 0) { // codi del procés fill
		reorganitzarCanals(); // reorganitza els seus canals per tal que només pugui llegir de la pipe interna
		// programa els signals
		signal(SIGUSR1, duplicarLletres);
		signal(SIGUSR2, sortir);
		while(true) { // i s'adorm
			pause();
		}
	} else if (pid3 < 0) { // si hi ha algun error creant el procés
		perror("Error al crear el procés de duplica lletres");
		exit(1);
	}
	// el pare (l'encripta) reorganitza els seus canals per tal que només pugui escriure a la pipe interna
	close(fd[0]);
	
	i = read(canal, buff, MIDA_BUFF); // rep la primera paraula
	while (i > 0) {		
		write(fd[1], buff, i); // envia la paraula a través de la pipe interna
		
		seleccionarProces(); // desperta a un dels processos fills
				
		i = read(canal, buff, MIDA_BUFF); // rep la següent paraula
	}

	// un cop ja no llegeixi res de la pipe amb nom, significarà que l'entrada s'ha mort, llavors indica als processos fills que s'ha acabat
	kill(pid1, SIGUSR2);
	kill(pid2, SIGUSR2);
	kill(pid3, SIGUSR2);

	// finalment, tanca la pipe interna, i tanca i esborra la pipe amb nom abans d'acabar
	close(fd[1]);
	close(canal);
	unlink("./canal");
}
