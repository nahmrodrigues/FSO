#include <stdio.h>

int l1, l2, c1, c2;

void le_matriz(int l, int c, int M[l][c]) {
    int i, j;

    for(i = 0; i < l; i++) {
        for(j = 0; j < c; j++) {
            scanf("%d", &M[i][j]);
        }
    }
}

void imprime_matriz(int l, int c, int M[l][c]) {
    int i, j;

    printf("\n");
    for(i = 0; i < l; i++) {
        for(j = 0; j < c; j++) {
            printf("%d ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int soma(int M1[l1][c1], int M2[l2][c2], int l, int c) {
    int i, result = 0;

    for(i = 0; i < c1; i++) {
        result += M1[l][i] * M2[i][c];
    }

    return result;
}

void multiplica_matrizes(int M1[l1][c1], int M2[l2][c2]) {
    int MR[l1][c2];
    int i, j;

    for(i = 0; i < l1; i++) {
        for(j = 0; j < c2; j++) {
            MR[i][j] = soma(M1, M2, i, j);
        }
    }

    printf("\nResultado: \n");
    imprime_matriz(l1, c2, MR);
}

int main() {

    do {
        printf("Nº Linhas e Colunas M1: ");
        scanf("%d %d", &l1, &c1);

        printf("Nº Linhas e Colunas M2: ");
        scanf("%d %d", &l2, &c2);
    } while(c1 != l2);

    int M1[l1][c1];
    int M2[l2][c2];
    int MR[c1][l2];

    printf("\nEntre M1: \n");
    le_matriz(l1, c1, M1);

    printf("\nEntre M2: \n");
    le_matriz(l2, c2, M2);

    multiplica_matrizes(M1, M2);

    return 0;
}