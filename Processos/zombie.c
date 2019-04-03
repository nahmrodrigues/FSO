#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>



int main() {

    if(fork() > 0)
        scanf("%*d");
    else
        printf("jnsdxa\n");    
        
    return 0;
}