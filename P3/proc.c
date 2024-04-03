// Juan José Gómez Villegas
// u1987338
// Pràctica 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pid1, pid2, pid3, estat1, estat2, fd1[2], fd2[2];

/**
* comanda: cat /proc/15735/status | grep Vm | sort -nrk2 >> sortida.txt
**/
int main(int argc, char *argv[]) {
	char codiUdg[] = "u1987338";
		
	if (argc == 2) { // Validació de paràmetres
		// Primer, construeix la ruta on anar a buscar l'estat del procés
		char ruta[100] = "/proc/";
		strcat(ruta, argv[1]);
		strcat(ruta, "/status");
		
		// I segon, executa la comanda
		
		/*estat = pipe(fd);
		pid = fork();
		if (pid == 0) { // es el fill
			close(0);
			dup(fd[0]);
			close(fd[0]);
			close(fd[1]);
			execl("/bin/grep", "grep", "Vm", (char *)0);
			exit(0);
		} else { // es el pare
			close(1);
			dup(fd[1]);
			close(fd[0]);
			close(fd[1]);
			
			execl("/bin/cat", "cat", , (char *)0);
			exit(0);
		}*/
	} else {
		printf("\033[0;41mError amb els parametres. Ajuda:\n%s <pid del procés> \033[0m \n", argv[0]);
	}
}
