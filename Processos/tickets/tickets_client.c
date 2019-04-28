#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>

#define QUEUE_SERV "/qq"
#define GET 0
#define RETURN 1

int client_id;
char *filename;
char *client_queue;

void set_client_queue() {
    filename = malloc(sizeof(char) * 80);
    sprintf(filename, "%i", client_id);

    int length = strlen(filename);

    client_queue = malloc(sizeof(char) * (length + 1));
    strcpy(client_queue, "/");
    strcat(client_queue, filename);
}

void register_client() {
    FILE *fp = fopen("clients.txt", "a+");

    int id = 0;

    while(!feof(fp))
        fscanf(fp, "%d", &id);
    
    client_id = id + 1;
    fprintf(fp, "%d\n", client_id);

    fclose(fp);
    set_client_queue();
}

void set_msg(int control, int ticket) {
    FILE *fp = fopen(filename, "w+");
    fprintf(fp, "%d\n", control);
    fprintf(fp, "%d\n", ticket);
    fclose(fp);
}

int c_get_ticket() {     
    mqd_t queue_serv = mq_open(QUEUE_SERV, O_RDWR);
    
    if(queue_serv < 0) {
        perror("\nget_ticket|serv: mq_open\n");
        exit(1);
    }
    
    set_msg(GET, 0);

    if((mq_send(queue_serv, (void *) &client_id, sizeof(client_id), 0)) < 0) {
        perror("\nget_ticket: mq_send\n");
        exit(1);
    }

    printf("\nTicket solicitado.\n");

    int ticket;
    struct mq_attr attr;

    attr.mq_maxmsg = 100; // capacidade para 100 mensagens
    attr.mq_msgsize = sizeof(ticket) ; // tamanho de cada mensagem
    attr.mq_flags = 0;

    mqd_t queue_client = mq_open(client_queue, O_RDWR|O_CREAT, 0666, &attr);

    if(queue_client < 0) {
        perror("\nget_ticket|client: mq_open\n");
        exit(1);
    }

    if((mq_receive(queue_client, (void*) &ticket, sizeof(ticket), 0)) < 0) {
        perror("\nget_ticket: mq_receive\n");
        exit(1);
    }

    printf("Ticket recebido: %d\n", ticket);
    remove(filename);
    
    mq_close(queue_serv);
    mq_close(queue_client);

    return ticket;
}

void return_ticket(int ticket) {
    mqd_t queue_serv = mq_open(QUEUE_SERV, O_RDWR);

    if(queue_serv < 0) {
        perror("\nreturn_ticket|serv: mq_open\n");
        exit(1);
    }

    set_msg(RETURN, ticket);

    if((mq_send(queue_serv, (void *) &client_id, sizeof(client_id), 0)) < 0) {
        perror("\nget_ticket: mq_send\n");
        exit(1);
    }

    mq_close(queue_serv);
}

int main (int argc, char *argv[]) {
    register_client();

    int ticket = 0;

    while(1) {
        ticket = c_get_ticket();
        sleep(1);
        return_ticket(ticket);
    }

    free(filename);
    free(client_queue);

    return 0;
}