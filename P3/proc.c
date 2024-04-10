// Juan José Gómez Villegas
// u1987338
// Pràctica 3

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int sortida1, pid1, pid2, estat1, estat2, fd1[2], fd2[2];

void redirect(char* fitxerOut, int canal, int mask) {
	sortida1 = open(fitxerOut, O_WRONLY|O_CREAT|O_APPEND, mask);
        close(canal);
        dup(sortida1);
        close(sortida1);
}

void netejaCanals() {
	close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
}

char* rutaProces(char* spid) {
	char* ruta = "/proc/";
        strcat(ruta, spid);
        strcat(ruta, "/status");

	return ruta;
}

/**
* comandes:
* ps -ef | grep u1987338
* cat /proc/15735/status | grep Vm | sort -nrk2 >> sortida.txt
**/
int main(int argc, char *argv[]) {
	char codiUdg[] = "u1987338";
		
	if (argc == 2) { // Validació de paràmetres
		//char* ruta = rutaProces(argv[1]);
		char ruta[100] = "/proc/";
        	strcat(ruta, argv[1]);
        	strcat(ruta, "/status");

		// I segon, executa la comanda
		estat1 = pipe(fd1);
		estat2 = pipe(fd2);
		pid1 = fork();
		if (pid1 == 0) { // codi del grep
			close(0);
			close(1);
			dup(fd1[0]);
			dup(fd2[1]);
			netejaCanals();
			execl("/bin/grep", "grep", "Vm", (char *)0);
			exit(0);
		} else if (pid1 < 0) {
			printf("\033[0;41mError en executar el fork \033[0m \n");
			exit(1);
		}
		
		pid2 = fork();
		if (pid2 == 0) { // codi del sort
			close(0);
			dup(fd2[0]);
			netejaCanals();
			redirect("./sortida.txt", 1, 0600);
			execl("/bin/sort", "sort", "-nrk2", (char *)0);
			exit(0);
		} else if (pid2 < 0) {
			printf("\033[0;41mError en executar el fork \033[0m \n");
			exit(1);
		}
		
		// codi del cat
		close(1);
		dup(fd1[1]);
		netejaCanals();
		execl("/bin/cat", "cat", ruta, (char *)0);
		exit(0);
	} else {
		printf("\033[0;41mError amb els parametres. Ajuda:\n%s <pid del procés> \033[0m \n", argv[0]);
		exit(1);
	}
}
