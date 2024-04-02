#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc == 2) {
		printf("\033[0m Parametre 1 = %s \033[0m \n", argv[1]);
	} else {
		printf("\033[0;41m Error amb els parametres. \033[0m \n");
	}
}
