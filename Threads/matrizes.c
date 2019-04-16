#include <stdio.h>

int n;

void le_matriz(int m[n][n]) {
	int i, j;
	
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			scanf("%d", &m[i][j]);
		}
	}
}

void imprime_matriz(int m[n][n]) {
	int i, j;
	
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			printf("%d ", m[i][j]);
		}
		printf("\n");
	}
}

int soma(int a[n][n], int b[n][n], int l, int c) {
	int i, soma = 0;
	
	for(i = 0; i < n; i++) {
		soma += a[l][i] * b[i][c];		
	}
	
	return soma;
}

void multiplica_matrizes(int a[n][n], int b[n][n], int r[n][n]) {
	int i, j;
	
	for(i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			r[i][j] = soma(a, b, i, j);
		}
	}
}

int main() {
	scanf("%d", &n);
	
	int a[n][n], b[n][n], r[n][n];
	
	le_matriz(a);
	le_matriz(b);
	
	multiplica_matrizes(a, b, r);
	
	printf("\nResultado: \n\n");
	imprime_matriz(r);
	
	return 0;
}
