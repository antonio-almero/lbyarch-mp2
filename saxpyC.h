/*
	Almero, Antonio Alvaro
 	Marquez, John Thomas
 	LBYARCH, S30G
*/

#ifndef SAXPYC_H
#define	SAXPYC_H

// Z[n] = A(X[n]) + Y[n]
void saxpyC(int n, float A, float X[], float Y[], float Z[]){
	for (int i = 0; i < n; i++){
		float Ax = A * X[i];
		float Axpy = Ax + Y[i];
		Z[i] = Axpy;
	}
}

#endif