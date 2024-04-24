// Juan José Gómez Villegas
// u1987338
// Pràctica 4 - Entrada

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int i;
char buff[30];

/**
* Pre:
* Post:
**/
int main(int argc, char *argv[]) {
	i = read(0, buff, 30);
	while (i > 0) {
		write(1, buff, 30);
		i = read(0, buff, 30);
	}
}
