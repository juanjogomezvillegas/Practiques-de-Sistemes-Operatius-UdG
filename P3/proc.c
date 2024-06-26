// Juan José Gómez Villegas
// u1987338
// Pràctica 3

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

// Creació de variables globals
int pid1, pid2, estat1, estat2, fd1[2], fd2[2];
char codiUdg[] = "u1987338";

/**
* Pre: nomFitxer no és buit, mask >= 0000, i canal >= 0
* Post: Creat el fitxer nomFitxer amb els permisos indicats al paràmetre mask, al que s'accedirà pel canal (0,1,2...) indicat per paràmetre
**/
void redirect(char* nomFitxer, int mask, int canal) {
	int sortida1 = open(nomFitxer, O_WRONLY|O_CREAT|O_APPEND, mask);
	close(canal);
	dup(sortida1);
	close(sortida1);
}

/**
* Pre: Cert
* Post: Els canals que no es facin servir queden tancats
**/
void netejaCanals() {
	close(fd1[0]);
	close(fd1[1]);
	close(fd2[0]);
	close(fd2[1]);
}

/**
* Programa que imita el funcionament de la comanda: cat /proc/<pid d'un procés>/status | grep Vm | sort -nrk2 >> sortida.txt
*
* Pre: argc == 2 i argv[1] == <pid d'un procés>, per obtenir el pid es pot executar al shell: ps -ef | grep u1987338
* Post: 0, si el programa ha acabat correctament; > 0, si s'ha produït algun error
**/
int main(int argc, char *argv[]) {
	if (argc == 2) { // Si han entrat exactament un paràmetre
		// Primer, forma la ruta on anar a buscar l'estat del procés
		char ruta[100] = "/proc/";
		strcat(ruta, argv[1]);
		strcat(ruta, "/status");
		
		// I Comprova que la ruta del fitxer que conté l'estat del procés existeix
		if (!fopen(ruta, "r")) {
			printf("\033[0;41mError el PID %s no existeix o no hi tens permisos. Per buscar un altre executa: ps -ef | grep %s \033[0m \n", argv[1], codiUdg);
			exit(1);
		}
		
		// I si la ruta és correcte, a partir d'aquí el codi serà el procés cat
		
		// Primer, el procés cat crea dues pipes
		estat1 = pipe(fd1);
		estat2 = pipe(fd2);
		
		// I després, ja pot crear els processos fills
		pid1 = fork(); // El procés cat crea el procés grep
		if (pid1 == 0) { // Codi del grep
			// Es preparen els canals que farà servir
			close(0);
			close(1);
			dup(fd1[0]);
			dup(fd2[1]);
			netejaCanals(); // Es tanquen els canals que no farà servir
			// Executa el grep fent un execl, que esborra la memòria i canvia el codi pel codi de /bin/grep
			execl("/bin/grep", "grep", "Vm", (char *)0);
			exit(0);
		} else if (pid1 < 0) { // I en cas d'error en crear el grep
			printf("\033[0;41mError en executar el fork \033[0m \n");
			exit(1);
		}
		
		pid2 = fork(); // El procés cat crea el procés sort
		if (pid2 == 0) { // codi del sort
			// Es preparen els canals que farà servir
			close(0);
			dup(fd2[0]);
			netejaCanals(); // Es tanquen els canals que no farà servir
			redirect("./sortida.txt", 0600, 1); // I prepara el fitxer on s'escriurà la sortida
			// Executa el sort fent un execl, que esborra la memòria i canvia el codi pel codi de /bin/sort
			execl("/bin/sort", "sort", "-nrk2", (char *)0);
			exit(0);
		} else if (pid2 < 0) { // I en cas d'error en crear el sort
			printf("\033[0;41mError en executar el fork \033[0m \n");
			exit(1);
		}
		
		// codi del cat, es preparen els canals que farà servir
		close(1);
		dup(fd1[1]);
		netejaCanals(); // Es tanquen els canals que no farà servir
		// Executa el cat fent un execl, que esborra la memòria i canvia el codi pel codi de /bin/cat
		execl("/bin/cat", "cat", ruta, (char *)0);
		exit(0);
		
	} else { // En cas d'error amb els paràmetres, mostra una ajuda de com executar el programa
		printf("\033[0;41mError amb els parametres.\nAjuda:   %s <pid del procés> \033[0m \n", argv[0]);
		exit(1);
	}
}
