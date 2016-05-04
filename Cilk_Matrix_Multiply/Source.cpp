#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api_windows.h>
#include <cilk/hyperobject_base.h>
#include <cilk/common.h>

using namespace std;

const int N = 1000;

int matrixMultiplierSequential() {
	
	// Create some 2D dynamic arrays
	int **A = new int*[N];
	for (int i = 0; i < N; ++i) {
		A[i] = new int[N];
	}
	int **B = new int*[N];
	for (int i = 0; i < N; ++i) {
		B[i] = new int[N];
	}
	int **C = new int*[N];
	for (int i = 0; i < N; ++i) {
		C[i] = new int[N];
	}

for (int i = 0; i < N; i++) {
	for (int j = 0; j < N; j++) {
		C[i][j] = 0;
		for (int k = 0; k < N; k++) {
			C[i][j] += A[i][k] * B[k][j];
		}
	}
}

return 0;
}

int matrixMultiplierCilkFor() {

	// Create some 2D dynamic arrays
	int **A = new int*[N];
	for (int i = 0; i < N; ++i) {
		A[i] = new int[N];
	}
	int **B = new int*[N];
	for (int i = 0; i < N; ++i) {
		B[i] = new int[N];
	}
	int **C = new int*[N];
	for (int i = 0; i < N; ++i) {
		C[i] = new int[N];
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			C[i][j] = 0;
			cilk_for (int k = 0; k < N; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {

	int start, finish, count;
	double totalSequential, totalCilkFor;

	// If we've got a parameter, assume it's the number of workers to be used
	if (argc > 1)
	{
		// Values less than 1, or parameters that aren't numbers aren't allowed
		if (atoi(argv[1]) < 1)
		{
			printf("Usage: matrixMultiplierCilkFor [workers]\n");
			return 1;
		}

		// Set the number of workers to be used
		__cilkrts_set_param("nworkers", argv[1]);
	}

	// Mesure cilk_for time
	start = clock();
	count = matrixMultiplierCilkFor();
	finish = clock();

	totalCilkFor = finish - start;

	// Measure sequential time
	start = clock();
	matrixMultiplierSequential();
	finish = clock();

	totalSequential = finish - start;

	std::cout << "Sequential function took: " << totalSequential / CLOCKS_PER_SEC << " seconds.\n"
		<< "cilk_for took: " << totalCilkFor / CLOCKS_PER_SEC << " seconds.\n";

	return 0;
}