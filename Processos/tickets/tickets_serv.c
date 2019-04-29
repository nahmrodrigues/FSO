/*
    Aluna: Natalia Maria Rodrigues Queiroz
    Matricula: 16/0015839

    SERVIDOR
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE "/queue_serv"
#define NUM_MAX 100
#define GET 0
#define RETURN 1

int tickets[NUM_MAX];

// preenche o vetor de tickets
void gera_tickets() {
    int i;

    for(i = 0; i < NUM_MAX; i++) {
        tickets[i] = i+1;
    }
}


// retorna o primeiro valor diferente de 0 no vetor de tickets
int get_ticket() {
    int i;

    int result = -1;

    for(i = 0; i < NUM_MAX; i++) {
        if(tickets[i] != 0) {
            result = tickets[i];
            tickets[i] = 0; // seta o valor para 0, indicando que o ticket esta sendo usado
            return result;
        }
    }

    return result;
}

// coloca o ticket devolvido na posicao correta
void return_ticket(int t) {
    tickets[t-1] = t;
}

void handle_get_ticket(int client_id, char *client_queue) {
    mqd_t queue = mq_open(client_queue, O_RDWR); // abre a fila do client que enviou a solicitacao

    if(queue < 0) {
        perror("\nget: mq_open\n");
        exit(1);
    }

    int ticket = get_ticket();

    // envia uma msg na fila do client, com o ticket solicitado
    if((mq_send(queue, (void *) &ticket, sizeof(ticket), 0)) < 0) {
        perror("\nget_ticket: mq_send\n");
        exit(1);
    }

    printf("\nTicket enviado para o cliente %d: %d\n", client_id, ticket);

    mq_close(queue);
}

// funcao responsavel por retornar o nome do arquivo de informacoes do client especifico
// o nome do arquivo e sempre o id do client
char * get_filename(int client_id) {
    char *line = malloc(sizeof(char) * 80);
    sprintf(line, "%i", client_id);

    return line;
}

// funcao responsavel por retornar o nome da fila do client, que e sempre na forma '/id'
char * get_client_queue(char *line) {
    int length = strlen(line);

    char * client_queue = malloc(sizeof(char) * (length + 1));
    strcpy(client_queue, "/");
    strcat(client_queue, line);

    return client_queue;
}

void handle_msg(int client_id) {
    char *filename = get_filename(client_id);
    char *client_queue = get_client_queue(filename);

    // le arquivo de informacoes do client
    FILE *fp = fopen(filename, "r");
    int control;
    int ticket;

    fscanf(fp, "%d", &control);
    fscanf(fp, "%d", &ticket);
    fclose(fp);

    if(control == GET) { // se requisicao for do tipo GET
        handle_get_ticket(client_id, client_queue);
    }
    else { // se requisicao for do tipo RETURN
        return_ticket(ticket);
        printf("\nTicket recebido do cliente %d: %d\n", client_id, ticket);
    }
}


int main() {
    gera_tickets();

    struct mq_attr attr;
    int client_id;

    attr.mq_maxmsg = 10; // capacidade para 10 mensagens
    attr.mq_msgsize = sizeof(client_id); // tamanho de cada mensagem
    attr.mq_flags = 0;

    mqd_t queue = mq_open(QUEUE, O_RDWR|O_CREAT, 0666, &attr); // abre/cria fila do servidor

    if(queue < 0) {
        perror("mq_open\n");
        exit(1);
    }

    while(1) {
        // espera msg de requisicao dos clients
        if((mq_receive(queue, (void *) &client_id, sizeof(client_id), 0)) < 0) {
            perror("\nmq_receive");
            exit(1);
        }

        handle_msg(client_id);     
    }

    mq_close(queue);

    return 0;
}