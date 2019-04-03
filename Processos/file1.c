#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void gera_aleatorio() {
    FILE *arq = fopen("/tmp/numeros.txt", "w+");

    int aleatorio = rand();
    fprintf(arq, "%d\n", aleatorio);

    fclose(arq);
}

void verifica() {
    FILE *arq = fopen("/tmp/numeros.txt", "r");

    int n;

    fscanf(arq, "%d", &n);

    fclose(arq);

    if(n%2 == 0) {
        printf("%d e par\n", n);
    }
    else {
        printf("%d e impar\n", n);
    }
}

void fazoqueopaifaz(pid_t filho) {
    
    while(1) {
        kill(filho, SIGUSR1);
        sleep(1);
        verifica();
    }
}

void fazoqueofilhofaz() {
    signal(SIGUSR1, gera_aleatorio);

    while(1)
        sleep(1);
}

int main() {

    gera_aleatorio();
    verifica();
    
    pid_t filho = fork();

    if(filho > 0) {
        sleep(1);
        fazoqueopaifaz(filho);
    }
    else {
        fazoqueofilhofaz();
    }

    return 0;
}