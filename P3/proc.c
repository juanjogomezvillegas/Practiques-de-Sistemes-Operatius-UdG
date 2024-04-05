// Juan José Gómez Villegas
// u1987338
// Pràctica 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int pid1, pid2, estat1, estat2, fd1[2], fd2[2];

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
		if ((pid1 = fork()) == 0) {
			estat2 = pipe(fd2);
			if ((pid2 = fork()) == 0) { // codi del sort
				printf("UNIX is not LINUX");
				close(0);
				dup(fd2[0]);
				close(fd2[0]);
				close(fd2[1]);
				execl("/bin/sort", "sort", "-nrk2", (char *)0);
				exit(0);
			} else { // codi del grep
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
			}
		} else { // codi del cat
			close(1);
			dup(fd1[1]);
			close(fd1[0]);
			close(fd1[1]);
			execl("/bin/cat", "cat", ruta, (char *)0);
			exit(0);
		}
				
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
