#include <stdio.h>

int main() {

    int x = 6;

    {
        int x = 5;
        printf("%d\n", x);
    }

    printf("%d\n", x);

    return 0;
}