#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pid, estat, fd[2];

/**
	comanda ls -lisa | sort
	gcc -o pipe pipe.c
**/
main() {
	estat = pipe(fd);
	pid = fork();
	if (pid == 0) { // es el fill
		close(0);
		dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		execl("/bin/grep", "grep", "a", (char *)0);
		exit(1); // el fill no va bé
	} else { // es el pare
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		execl("/bin/ls", "ls", "-lisa", (char *)0);
		exit(0);
	}
}
