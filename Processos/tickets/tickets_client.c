/*
    Aluna: Natalia Maria Rodrigues Queiroz
    Matricula: 16/0015839

    CLIENTE
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/stat.h>

#define QUEUE_SERV "/queue_serv1"
#define GET 0
#define RETURN 1

int client_id;
char *filename; // nome do arquivo onde serao colocados 
                // as informacoes da mensagem enviada ao servidor
char *client_queue; // nome da queue do client, onde o servidor
                    // armazenara os tickets enviados aos clients

void set_client_queue() {
    filename = malloc(sizeof(char) * 80);
    sprintf(filename, "%i", client_id); // setando o arquivo de informacoes com o id do client

    int length = strlen(filename);

    // setando o nome da queue do client com '/id'
    client_queue = malloc(sizeof(char) * (length + 1));
    strcpy(client_queue, "/");
    strcat(client_queue, filename);
}

// funcao responsavel por atribuir um id para cada client rodado
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

// funcao responsavel por preencher o arquivo de informacoes das msgs enviadas ao servidor
// control: (GET ou RETURN) indica a solicitacao do client ao servidor
// ticket: se a solicitacao for do tipo GET, ticket = 0
//         se a solicitacao for do tipo RETURN, ticket = ticket a ser devolvido 
void set_msg(int control, int ticket) {
    FILE *fp = fopen(filename, "w+");
    fprintf(fp, "%d\n", control);
    fprintf(fp, "%d\n", ticket);
    fclose(fp);
}

int get_ticket() {
    int ticket;
    struct mq_attr attr;

    attr.mq_maxmsg = 10; // capacidade para 10 mensagens
    attr.mq_msgsize = sizeof(ticket) ; // tamanho de cada mensagem
    attr.mq_flags = 0;

    // abre a queue do client, onde o servidor ira retornar o ticket
    mqd_t queue_client = mq_open(client_queue, O_RDWR|O_CREAT, 0666, &attr);

    if(queue_client < 0) {
        perror("\nget_ticket|client: mq_open\n");
        exit(1);
    }

    // abre a fila do servidor, onde o client ira sinalizar uma solicitacao
    // enviando seu id    
    mqd_t queue_serv = mq_open(QUEUE_SERV, O_RDWR);
    
    if(queue_serv < 0) {
        perror("\nget_ticket|serv: mq_open");
        exit(1);
    }

    set_msg(GET, 0); // preenche o arquivo de informacoes da msg

    // envia uma msg com o id para o servidor
    if((mq_send(queue_serv, (void *) &client_id, sizeof(client_id), 0)) < 0) {
        perror("\nget_ticket: mq_send");
        exit(1);
    }

    printf("\nTicket solicitado.\n");

    // espera o ticket do servidor na fila do client
    if((mq_receive(queue_client, (void*) &ticket, sizeof(ticket), 0)) < 0) {
        perror("\nget_ticket: mq_receive\n");
        exit(1);
    }

    printf("Ticket recebido: %d\n", ticket);
    // remove(filename); // apaga o arquivo de informacoes da msg
    
    mq_close(queue_serv);
    mq_close(queue_client);

    return ticket;
}

void return_ticket(int ticket) {
    mqd_t queue_serv = mq_open(QUEUE_SERV, O_RDWR); // abre a fila do servidor

    if(queue_serv < 0) {
        perror("\nreturn_ticket|serv: mq_open\n");
        exit(1);
    }

    set_msg(RETURN, ticket); // preenche o arquivo de informacoes da msg

    // envia msg na fila do servidor, indicando solicitacao
    if((mq_send(queue_serv, (void *) &client_id, sizeof(client_id), 0)) < 0) {
        perror("\nget_ticket: mq_send\n");
        exit(1);
    }

    mq_close(queue_serv);
}

int main () {
    register_client(); // registra o novo client, atribuindo um id

    int ticket = 0;

    while(1) {
        ticket = get_ticket();
        sleep(1);
        return_ticket(ticket);
    }

    free(filename);
    free(client_queue);

    return 0;
}