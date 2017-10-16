#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

// PROTOTYPES
void producer(void);
void consumer(void);

int const SIZE = 10;
int wearhouse[10];
int create = 0;
int use = 0;
int productNumber = 1000;
int total = 0;
FILE *output;
sem_t mutex;

int main(int argc, char **argv){

	pthread_t thread1;
	pthread_t thread2;

	sem_init(&mutex, 0, 1);

	if(pthread_create(&thread1, NULL, (void*)producer, NULL) != 0){
		printf("Thread 1 Failed!");
		exit(EXIT_FAILURE);
	}


	if(pthread_create(&thread2, NULL, (void*)consumer, NULL) != 0){
		printf("Thread 2 Failed!");
		exit(EXIT_FAILURE);
	}

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	sem_destroy(&mutex);

	return EXIT_SUCCESS;
}// end of main


void producer(void){
	int x;
	while(1){
		sem_wait(&mutex);

		output = fopen("proj2_part2.txt", "a");

		if(total < SIZE){
			x = create;
			printf("Produced %d create = %d use = %d total = %d\n", wearhouse[create] = productNumber, x, use, ++total);
			fprintf(output, "Produced %d create = %d use = %d total = %d\n", wearhouse[create++] = productNumber++, x, use, total);
			create %= SIZE;
		}else{
			/* Wait */
		}
		fclose(output);
		sem_post(&mutex);
	}// end of while loop
}// end of producer function


void consumer(void){
	int x;
	while(1){
		sem_wait(&mutex);

		output = fopen("proj2_part2.txt", "a");

		if(total == 0){
			/* Wait */
		}else{
			x = use;
			fprintf(output, "Consumed %d use = %d create = %d total = %d\n", wearhouse[use], x, create, --total);
			printf("Consumed %d use = %d create = %d total = %d\n", wearhouse[use++], x, create, total);
			use %= SIZE;
		}
		fclose(output);
		sem_post(&mutex);
	}//end while loop
}// end of consumer function

