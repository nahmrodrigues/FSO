#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE "/qq"
#define NUM_MAX 100
#define GET 0
#define RETURN 1

int tickets[NUM_MAX];

void gera_tickets() {
    int i;

    for(i = 0; i < NUM_MAX; i++) {
        tickets[i] = i+1;
    }
}

int get_ticket() {
    int i;

    int result = -1;

    for(i = 0; i < NUM_MAX; i++) {
        if(tickets[i] != 0) {
            result = tickets[i];
            tickets[i] = 0;
            return result;
        }
    }

    return result;
}

void return_ticket(int t) {
    tickets[t-1] = t;
}

void handle_get_ticket(int client_id, char *client_queue) {
    mqd_t queue = mq_open(client_queue, O_RDWR);

    if(queue < 0) {
        perror("\nget: mq_open\n");
        exit(1);
    }

    int ticket = get_ticket();

    if((mq_send(queue, (void *) &ticket, sizeof(ticket), 0)) < 0) {
        perror("\nget_ticket: mq_send\n");
        exit(1);
    }

    printf("\nTicket enviado para o cliente %d: %d\n", client_id, ticket);

    mq_close(queue);
}

char * get_filename(int client_id) {
    char *line = malloc(sizeof(char) * 80);
    sprintf(line, "%i", client_id);

    return line;
}

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

    FILE *fp = fopen(filename, "r");
    int control;
    int ticket;

    fscanf(fp, "%d", &control);
    fscanf(fp, "%d", &ticket);
    fclose(fp);

    if(control == GET) {
        handle_get_ticket(client_id, client_queue);
    }
    else {
        return_ticket(ticket);
        printf("\nTicket recebido do cliente %d: %d\n", client_id, ticket);
    }
}


int main() {
    // mq_unlink(QUEUE);
    gera_tickets();

    struct mq_attr attr;
    int client_id;

    attr.mq_maxmsg = 10; // capacidade para 10 mensagens
    attr.mq_msgsize = sizeof(client_id); // tamanho de cada mensagem
    attr.mq_flags = 0;

    mqd_t queue = mq_open(QUEUE, O_RDWR|O_CREAT, 0666, &attr);

    if(queue < 0) {
        perror("mq_open\n");
        exit(1);
    }

    int ticket;

    while(1) {
        if((mq_receive(queue, (void *) &client_id, sizeof(client_id), 0)) < 0) {
            perror("\nmq_receive");
            exit(1);
        }

        handle_msg(client_id);     
    }

    mq_close(queue);

    return 0;
}