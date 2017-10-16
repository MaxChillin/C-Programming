#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ 0
#define WRITE 1
#define BUFFERSIZE 256

int main(int argc, char *argv[]){

	int *status = 0;

	printf("My PID is %d, I am the parent, I have no children\n", getpid());
	if(fork() == 0){
		/* first Child */
		printf("My PID is %d, I am the first child of %d\n", getpid(), getppid());
		system("ls > P2file.txt");

	}else{
		/* Parent */
		printf("My PID is %d, I am the parent, I have 1 child\n", getpid());
		if(fork() == 0){
			/* Second Child */
			printf("My PID is %d, I am the second child of %d\n", getpid(), getppid());
			system("who > P3file.txt");
		}else{
			/* Still the Parent */
			printf("My PID is %d, I am the parent, I have 2 Children\n", getpid());
		}// end of second child else
	}// end of first child else
	wait(status);
	return EXIT_SUCCESS;

}// end of main
