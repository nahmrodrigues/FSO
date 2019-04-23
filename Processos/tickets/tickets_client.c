#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>

#define QUEUE_SERV "/tickets"
#define QUEUE_CLIENTE "/tickets_received"

typedef struct msg {
    int control,
    mqd_t queue
} Msg;

int get_ticket() {
    Msg msg;
    int ticket;    
    
    mqd_t queue_serv = mq_open(QUEUE_SERV, O_RDWR|O_CREAT);
    mqd_t queue_client = mq_open(QUEUE_CLIENTE, O_RDWR|O_CREAT);

    if(queue_serv < 0 || queue_client < 0) {
        perror("mq_open");
        exit(1);
    }

    msg.control = 0;
    msg.queue = queue_client;
    
    int send = mq_send(queue_serv, (void *) msg, sizeof(msg), 0);
    
    ssize_t receive = mq_receive (queue_client, (void*) &ticket, sizeof(ticket), 0);

    if(receive < 0) {
        perror("mq_receive");
        exit(1);
    }

    printf("\nTicket recebido: %d", ticket);

    mq_close(queue_serv);
    mq_close(queue_client);
}

void return_ticket(int t) {
    

}

int main (int argc, char *argv[]) {
    // mqd_t queue; // descritor da fila de mensagens
    // struct mq_attr attr; // atributos da fila de mensagens
    // int msg ; // as mensagens são números inteiros

    // define os atributos da fila de mensagens
    // attr.mq_maxmsg = 10 ; // capacidade para 10 mensagens
    // attr.mq_msgsize = sizeof(msg) ; // tamanho de cada mensagem
    // attr.mq_flags = 0 ;

    // abre ou cria a fila com permissoes 0666
    // if ((queue = mq_open (QUEUE, O_RDWR|O_CREAT, 0666, &attr)) < 0) {
    //     perror("mq_open");
    //     exit(1);
    // }

    // // recebe cada mensagem e imprime seu conteudo
    // for (;;) {
    //     if ((mq_receive (queue, (void*) &msg, sizeof(msg), 0)) < 0) {
    //         perror("mq_receive:");
    //         exit(1);
    //     }
    //     printf ("Received msg value %d\n", msg);
    // }
}