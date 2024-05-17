#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

// Pre: valor >= 0, si valor == 0, sincronització; si valor == 1, exclusió mútua
// Post: s = valor
void sem_init(semaphore s, unsigned int valor) {
    s = valor;
}

// Pre: cert
// Post: s = s - 1, o si s <= 0, m'adormo
void sem_wait(semaphore s) {
    while (s <= 0) { // si hi ha processos fent servir la secció crítica, m'adormo
        pause();
    }

    s = s - 1; // quan em desperti, accedeixo a la secció crítica
}

// Pre: cert
// Post: s = s + 1
void sem_signal(semaphore s) {
    s = s + 1; // allibero la secció crítica
}
