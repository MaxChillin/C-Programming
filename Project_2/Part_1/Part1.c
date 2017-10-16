#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

int **matrixA;
int **matrixB;
int **matrixC;
unsigned int matrixARow;
unsigned int matrixBRow;
unsigned int matrixAColumn;
unsigned int matrixBColumn;
double thread_1_time;
double thread_2_time;
clock_t start, end, thread_start = 0, thread_end = 0;


/****************************
 *  	PROTOTYPES  	    *
 ****************************
 */
void thread_1(void);
void thread_2(void);

int main(int argc, char *argv[]){

	double cpu_time_used;
	FILE *matrixAfd;
	FILE *matrixBfd;
	size_t BUFFER_SIZE = 131072;
	char buffer1[BUFFER_SIZE];
	int sum;
	char *token;

	pthread_t thread1;
	pthread_t thread2;

	matrixAfd = fopen("matrixA.txt", "rb+");
	matrixARow = 0;
	matrixAColumn = 0;
	while(fgets(buffer1, BUFFER_SIZE, matrixAfd)){
		token = strtok(buffer1, ",");
		while(token != NULL){
			if(matrixARow){

			}else{
				matrixAColumn++;
			}
			token = strtok(NULL, ",");
		}
		matrixARow++;
	}

	printf("Matrix A is %d X %d\n", matrixARow, matrixAColumn);

	/**************************************************
	 * 		MEMORY ALLOCATION		  *
         **************************************************
	 *This is where we allocate the memory for Matrix A
	 */
	matrixA = (int **)malloc(matrixARow * sizeof(*matrixA));
	for (int i=0; i<matrixARow; i++)
		matrixA[i] = (int *)malloc(matrixAColumn * sizeof(*matrixA));

	/*
	 * This points back to the beginning of the file to read it again
	 * in order to fill the matrix with the data
	 */
	rewind(matrixAfd);

	matrixARow = 0;
	while(fgets(buffer1, BUFFER_SIZE, matrixAfd)){
		matrixAColumn = 0;
		token = strtok(buffer1, ",");
		while(token != NULL){
			matrixA[matrixARow][matrixAColumn++] = atoi(token);
			token = strtok(NULL, ",");
		}
		matrixARow++;
	}


	matrixBfd = fopen("matrixB.txt", "rb+");
	matrixBRow = 0;
	matrixBColumn = 0;
	while(fgets(buffer1, BUFFER_SIZE, matrixBfd)){
		token = strtok(buffer1, ",");
		while(token != NULL){
			if(matrixBRow){

			}else{
				matrixBColumn++;
			}
			token = strtok(NULL, ",");
		}
		matrixBRow++;
	}


	printf("Matrix B is %d X %d\n", matrixBRow, matrixBColumn);

	/**************************************************
	 * 		MEMORY ALLOCATION		  *
	 **************************************************
	 *This is where we allocate the memory for Matrix B
	 */
	matrixB = (int **)malloc(matrixBRow * sizeof(*matrixB));
	for (int i=0; i<matrixBRow; i++)
		matrixB[i] = (int *)malloc(matrixBColumn * sizeof(*matrixB));

	/*
	 * This points back to the beginning of the file to read it again
	 * in order to fill the matrix with the data
	 */
	rewind(matrixBfd);


	matrixBRow = 0;
	while(fgets(buffer1, BUFFER_SIZE, matrixBfd)){
		matrixBColumn = 0;
		token = strtok(buffer1, ",");
		while(token != NULL){
			matrixB[matrixBRow][matrixBColumn++] = atoi(token);
			token = strtok(NULL, ",");
		}
		matrixBRow++;
	}


	if(matrixAColumn != matrixBRow){
		puts("Dimension Mismatch Error!");
		exit(EXIT_SUCCESS);
	}


	/**************************************************
	 * 		MEMORY ALLOCATION		  *
	 **************************************************
	 *This is where we allocate the memory for Matrix C
	 */
	matrixC = (int **)malloc(matrixARow * sizeof(*matrixC));
	for (int i=0; i<matrixARow; i++){
		matrixC[i] = (int *)malloc(matrixBColumn * sizeof(*matrixC));
	}

	/*
	 * This starts the clock counting so we can measure
	 * how long it takes the process to calculate
	 */
	start = clock();

	for(int i = 0; i < matrixARow; i++){
		for(int j = 0; j < matrixBColumn; j++){
			sum = 0;
			for(int k = 0; k < matrixAColumn; k++){
				sum+= (matrixA[i][k] * matrixB[k][j]);
			}
			matrixC[i][j] = sum;
		}
	}
	/*
	 * This stops the clock counting so we can measure
	 * how long it takes the process to calculate
	 */
	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("This process with 1 thread took %f seconds to calculate the matrix\n", cpu_time_used);
	printf("Matrix C [0][0] = %d\n", matrixC[0][0]);
	printf("Matrix C [99][4999] = %d\n", matrixC[99][4999]);

	if(pthread_create(&thread1, NULL, (void*)thread_1, NULL) != 0){
		printf("Thread 1 Failed!");
		exit(EXIT_FAILURE);
	}


	if(pthread_create(&thread2, NULL, (void*)thread_2, NULL) != 0){
		printf("Thread 2 Failed!");
		exit(EXIT_FAILURE);
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	cpu_time_used = (thread_end - thread_start) / (double)CLOCKS_PER_SEC;
	printf("This process with 2 thread took %f seconds to calculate the matrix\n", cpu_time_used);
	printf("Matrix C [0][0] = %d\n", matrixC[0][0]);
	printf("Matrix C [99][4999] = %d\n", matrixC[99][4999]);

	fclose(matrixAfd);
	fclose(matrixBfd);
	free(matrixA);
	free(matrixB);
	free(matrixC);

	return EXIT_SUCCESS;
}


void thread_1(void){
	/*
	 * This starts the clock counting so we can measure
	 * how long it takes the process to calculate
	 */
	if(thread_start){

	}else{
		thread_start = clock();
	}
	int sum;
	for(int i = 0; i < matrixARow/2; i++){
		for(int j = 0; j < matrixBColumn; j++){
			sum = 0;
			for(int k = 0; k < matrixAColumn; k++){
				sum+= (matrixA[i][k] * matrixB[k][j]);
			}
			matrixC[i][j] = sum;
		}
	}
	/*
	 * This stops the clock counting so we can measure
	 * how long it takes the process to calculate
	 */
	thread_end = clock();

}


void thread_2(void){
	/*
	 * This starts the clock counting so we can measure
	 * how long it takes the process to calculate
	 */
	if(thread_start){

	}else{
		thread_start = clock();
	}

	int sum;
	for(int i = matrixARow/2; i < matrixARow; i++){
		for(int j = 0; j < matrixBColumn; j++){
			sum = 0;
			for(int k = 0; k < matrixAColumn; k++){
				sum+= (matrixA[i][k] * matrixB[k][j]);
			}
			matrixC[i][j] = sum;
		}
	}
	/*
	 * This stops the clock counting so we can measure
	 * how long it takes the process to calculate
	 */
	thread_end = clock();

}




