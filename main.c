/*
	Almero, Antonio Alvaro
 	Marquez, John Thomas
 	LBYARCH, S30G
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "saxpyC.h"

extern void saxpyASM(int n, float A, float X[], float Y[], float Z[]);

double benchmarkKernel(void (*kernel)(int, float, float *, float *, float *),
		int n, float A, float X[], float Y[], float Z[], double msPerTick,
		double runTimesMs[30]){
	double totalMs = 0.0;

	// Warm up the kernel and touch all output pages before timing.
	kernel(n, A, X, Y, Z);
	for (int run = 0; run < 30; run++){
		LARGE_INTEGER start, end;
		QueryPerformanceCounter(&start);
		kernel(n, A, X, Y, Z);
		QueryPerformanceCounter(&end);
		runTimesMs[run] = (end.QuadPart - start.QuadPart) * msPerTick;
		totalMs += runTimesMs[run];
	}

	return totalMs / 30;
}

void printArr(float arr[], int n){
	for (int i = 0; i < n; i++){
		printf("%9f ", arr[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]){
	char *sizeArg = argc > 1 ? argv[1] : "0";
	int exponent;
	if (argc > 2){
		fprintf(stderr, "Usage: %s [20|24|28|30]\n", argv[0]);
		return 1;
	}
	if (strcmp(sizeArg, "20") == 0){
		exponent = 20;
	} else if (strcmp(sizeArg, "24") == 0){
		exponent = 24;
	} else if (strcmp(sizeArg, "28") == 0){
		exponent = 28;
	} else if (strcmp(sizeArg, "30") == 0){
		exponent = 30;
	} else {
		fprintf(stderr, "Usage: %s [20|24|28|30]\n", argv[0]);
		return 1;
	}

	LARGE_INTEGER frequency;
	if (!QueryPerformanceFrequency(&frequency)){
		fprintf(stderr, "Could not initialize the benchmark timer.\n");
		return 1;
	}
	double msPerTick = 1000.0 / frequency.QuadPart;

	srand(time(NULL));
	int n = 1 << exponent;
	float A = 20.0 * ((float)rand() / (float)RAND_MAX) - 10.0;
	float *X = malloc(n * sizeof(*X));
	float *Y = malloc(n * sizeof(*Y));
	float *Z = malloc(n * sizeof(*Z));

	// Check if all mallocs were successful. If not, n is likely too large for machine
	if (X == NULL || Y == NULL || Z == NULL){
		fprintf(stderr, "Could not allocate vectors for n = %d.\n", n);
		free(X);
		free(Y);
		free(Z);
		return 1;
	}

	// Generate values of X and Y which are randomized every time program (not kernel) is launched
	for (int i = 0; i < n; i++){
		X[i] = 20.0 * ((float)rand() / (float)RAND_MAX) - 10.0;
		Y[i] = 20.0 * ((float)rand() / (float)RAND_MAX) - 10.0;
	}

	double cRunTimesMs[30];
	double asmRunTimesMs[30];
	float cPreview[10];
	int status = 1;
	double cAverageMs = benchmarkKernel(saxpyC, n, A, X, Y, Z, msPerTick, cRunTimesMs);
	memcpy(cPreview, Z, 10 * sizeof(*cPreview));

	double asmAverageMs = benchmarkKernel(saxpyASM, n, A, X, Y, Z, msPerTick, asmRunTimesMs);

	// Display values of n, A, X, Y
	printf("n: %d (2^%d)\n", n, exponent);
	printf("A: %f\n", A);
	printf("X: ");
	printArr(X, 10);
	printf("Y: ");
	printArr(Y, 10);

	// Display first ten rows of Z, C and Assembly results side by side
	printf("\n%5s %14s %14s\n", "Index", "C Z[i]", "Assembly Z[i]");
	for (int i = 0; i < 10; i++){
		printf("%5d %14.6f %14.6f\n", i, cPreview[i], Z[i]);
	}

	// Display execution time across 30 runs
	printf("\n%3s %14s %14s\n", "Run", "C (ms)", "Assembly (ms)");
	for (int run = 0; run < 30; run++){
		printf("%3d %14.6f %14.6f\n", run + 1, cRunTimesMs[run], asmRunTimesMs[run]);
	}

	// Display average
	printf("\nAverage kernel time over %d runs (ms):\n", 30);
	printf("       C: %.6f\n", cAverageMs);
	printf("Assembly: %.6f\n", asmAverageMs);
	status = 0;

	free(X);
	free(Y);
	free(Z);

	return status;
}
