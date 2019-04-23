#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE "/my_queue"
#define NUM_MAX 100

int tickets[NUM_MAX];

void gera_tickets() {
    int i;

    for(i = 0; i < NUM_MAX; i++) {
        tickets[i] = i+1;
    }
}

int get_ticket() {
    int i, result = -1;

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


int main() {
    mqd_t queue;
    int msg;

    gera_tickets();

    if((queue = mq_open(QUEUE, O_RDWR)) < 0) {
        perror("mq_open");
        exit(1);
    }

    for(;;) {
        msg = get_ticket();

        if(mq_send(queue, (void *) &msg, sizeof(msg), 0) < 0) {
            perror("mq_send");
            exit(1);
        }
        else {
            printf("Sent message with ticket %d\n", msg);
            sleep(1);
        }
    }

    return 0;
}