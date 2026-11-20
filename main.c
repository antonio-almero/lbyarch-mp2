#include <stdio.h>
#include <stdlib.h>
#include "saxpyC.h"

void printArr(float arr[], int n){
	for (int i = 0; i < n; i++){
		printf("%f ", arr[i]);
	}
	printf("\n");
}

void printResults(int n, float A, float X[], float Y[], float Z[]){
	printf("n: %d\n", n);
	printf("A: %f\n", A);
	printf("X: ");
	printArr(X, n);
	printf("Y: ");
	printArr(Y, n);
	printf("Z: ");
	printArr(Z, n);
}

int main(){
	int n, i;
	float A, *X, *Y, *Z, buffer;
	
	scanf("%d", &n);
	X = malloc(n * sizeof(*X));
	Y = malloc(n * sizeof(*Y));
	Z = malloc(n * sizeof(*Z));
	
	scanf("%f", &A);
	
	for (i = 0; i < n; i++){
		scanf("%f", &buffer);
		X[i] = buffer;
	}
	
	for (i = 0; i < n; i++){
		scanf("%f", &buffer);
		Y[i] = buffer;
	}
	
	for (i = 0; i < n; i++){
		Z[i] = 0.0;
	}
	
	saxpyC(n, A, X, Y, Z);
	printResults(n, A, X, Y, Z);
	
	return 0;
}