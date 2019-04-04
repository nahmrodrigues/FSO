/*
* Aluna: Natalia Maria Rodrigues Queiroz
* Matricula: 16/0015839
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void init() {
    FILE *fp = fopen("processes_tree.dot", "w+");
    fprintf(fp, "graph {");
    fclose(fp);
}

void generate_tree() {
    FILE *fp = fopen("processes_tree.dot", "a+");
    fprintf(fp, "\n}");
    fclose(fp);

    // System call to generate png (it can be different depending on the system)
    system("dot -Tpng processes_tree.dot -o processes_tree.png");

    printf("\nProcesses Tree Created!\n");
    printf("\nOpen 'processes_tree.png' to see the processes tree.\n\n");
}

void insert_in_file(pid_t dad, pid_t child) {
    FILE *fp = fopen("processes_tree.dot", "a+");
    fprintf(fp, "\n%d -- %d", (int) dad, (int) child);
    fclose(fp);
}

void dowhatchilddo() {
    insert_in_file(getppid(), getpid());
}

void dowhatc133do() {
    dowhatchilddo();

    pid_t c1331 = fork();
    if(c1331 == 0){
        dowhatchilddo();
    }
    else {
        int return1331;
        waitpid(c1331, &return1331, 0);
    }
}

void dowhatc13do() {
    dowhatchilddo();

    pid_t c131 = fork();
    if(c131 == 0){
        dowhatchilddo();
    }
    else {
        int return131;
        waitpid(c131, &return131, 0);
        pid_t c132 = fork();
        if(c132 == 0) {
            dowhatchilddo();
        }
        else {
            int return132;
            waitpid(c132, &return132, 0);
            pid_t c133 = fork();
            if(c133 == 0) {
                dowhatc133do();
            }
            else {
                int return133;
                waitpid(c133, &return133, 0);
            }
                
        }
    }
}

void dowhatc1do() {
    insert_in_file(getppid(), getpid());

    pid_t c11 = fork();
    if(c11 == 0) {
        dowhatchilddo();
    }
    else {
        int return11;
        waitpid(c11, &return11, 0);
        pid_t c12 = fork();
        if(c12 == 0) {
            dowhatchilddo();
        }
        else {
            int return12;
            waitpid(c12, &return12, 0);
            pid_t c13 = fork();
            if(c13==0) {
                dowhatc13do();
            }
            else {
                int return13;
                waitpid(c13, &return13, 0);
                pid_t c14 = fork();
                if(c14 == 0) {
                    dowhatchilddo();
                }
                else {
                    int return14;
                    waitpid(c14, &return14, 0);
                }
            }
        }
    }
}

void create_processes() {

    pid_t c1 = fork();
    if(c1 == 0) {
        dowhatc1do();
    }
    else {
        int return1;
        waitpid(c1, &return1, 0);
        generate_tree();
    }
}

int main() {

    init();
    create_processes();
    
    return 0;
}