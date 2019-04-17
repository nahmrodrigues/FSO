/*
* Aluna: Natalia Maria Rodrigues Queiroz
* Matricula: 160015839
*/

/*
* Resolucaoo 5a: Codigo.
*
* Resolucao 5b: A funcao get_ticket() garante que os tickets retornados nao estao em
                utilizacao, isso porque um ticket que esta sendo utilizado tem valor igual a 0
                na posicao do vetor com indice correspondente, e um semaforo e utilizado para 
                garantir que a regiao de memoria do vetor nao seja alterada de forma simultanea. 
                Tickets devolvidos com a utilizacao da funcao return_ticket(t) sao reutilizados.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_MAX 3
#define NUM_THREADS 6

sem_t s;

int tickets[NUM_MAX];

int get_ticket() {

    int i, result = -1;

    sem_wait(&s);
    for(i = 0; i < NUM_MAX; i++) {
        if(tickets[i] != 0) {
            result = tickets[i];
            tickets[i] = 0;
            sem_post(&s);
            return result;
        }
    }

    sem_post(&s);
    return result;
}

void return_ticket(int t) {
    sem_wait(&s);
    tickets[t-1] = t;
    sem_post(&s);
}

void *handle_ticket(void *id) {
    int *iid = (int *) id;
    int t;

    while(1) {
        sleep(1);
        
        t = get_ticket();
        
        if(t != -1)
            fprintf(stdout, "thread %d: ticket %d\n", *iid, t);
        else
            fprintf(stdout, "thread %d: tickets esgotados!\n", *iid);

        return_ticket(t);
    }

    pthread_exit(NULL);
}

int main() {
    int i;
    pthread_t threads[NUM_THREADS];

    sem_init(&s, 0, 1);

    for(i = 0; i < NUM_MAX; i++) {
        tickets[i] = i+1;
    }

    for(i = 0; i < NUM_THREADS; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        pthread_create(&threads[i], NULL, handle_ticket, (void *) id);
    }

    while(1);

    return 0;
}