// Juan José Gómez Villegas
// u1987338
// Pràctica 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int sortida1, pid1, pid2, estat1, estat2, fd1[2], fd2[2];

/**
* comandes:
* ps -ef | grep u1987338
* cat /proc/15735/status | grep Vm | sort -nrk2 >> sortida.txt
**/
int main(int argc, char *argv[]) {
	char codiUdg[] = "u1987338";
		
	if (argc == 2) { // Validació de paràmetres
		// Primer, construeix la ruta on anar a buscar l'estat del procés
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
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
			execl("/bin/grep", "grep", "Vm", (char *)0);
			exit(0);
		} else if (pid1 < 0) {
			exit(1);
		}
		
		pid2 = fork();
		if (pid2 == 0) { // codi del sort
			close(0);
			dup(fd2[0]);
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
			//sortida1 = open("./sortida.txt", O_WRONLY|O_CREAT|O_TRUNC, 00600);
			//close(1);
			//dup(sortida1);
			//close(sortida1);
			execl("/bin/sort", "sort", "-nrk2", (char *)0);
			exit(0);
		} else if (pid2 < 0) {
			exit(1);
		}
		
		// codi del cat
		close(1);
		dup(fd1[1]);
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		execl("/bin/cat", "cat", ruta, (char *)0);
		exit(0);
	} else {
		printf("\033[0;41mError amb els parametres. Ajuda:\n%s <pid del procés> \033[0m \n", argv[0]);
	}
}
