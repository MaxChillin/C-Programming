#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define FALSE 0
#define TRUE 1

/*
 
 */

int main(int argc, char *argv[]){

	system("echo 0 > myFile.txt");

	FILE *fd;
	size_t BUFFER_SIZE = 10;
	pid_t pid;
	char buffer1[BUFFER_SIZE];
	char buffer2[BUFFER_SIZE];
	char buf[1];
	int pipe1[2];
	int pipe2[2];
	int counter = 0;

	pipe(pipe1);
	pipe(pipe2);

	int childW = pipe1[1];
	int childR = pipe2[0];

	int parentW	= pipe2[1];
	int parentR	= pipe1[0];

	write(childW, buf, 1);
	if((pid = fork()) == 0){
		/* Child */
		while(TRUE){
			read(childR, buf, 1);// this waits for the parent

			fd = fopen("myFile.txt", "r+");
			fgets(buffer1, BUFFER_SIZE, fd);
			fclose(fd);
			fd = fopen("myFile.txt", "r+");
			printf("Child id is %d, and reads the number %s\n", getpid(), buffer1);
			counter = atoi(buffer1);
			if(counter >= 50){
				close(pipe1[0]);
				close(pipe1[1]);
				close(pipe2[0]);
				close(pipe2[1]);
				sprintf(buffer2, "%d", counter);
				fputs(buffer2, fd);
				fclose(fd);
				exit(EXIT_SUCCESS);
			}
			counter++;
			sprintf(buffer2, "%d", counter);
			fputs(buffer2, fd);
			fclose(fd);

			write(childW, buf, 1);// this tells the parent to continue
		}// end of the child while loop
	}else{
		/* Parent */
		while(TRUE){
			read(parentR, buf, 1);// this waits for the child

			fd = fopen("myFile.txt", "r+");
			fgets(buffer1, BUFFER_SIZE, fd);
			fclose(fd);
			fd = fopen("myFile.txt", "r+");
			printf("Parent id is %d, and reads the number %s\n", getpid(), buffer1);
			counter = atoi(buffer1);
			if(counter >= 50){
				close(pipe1[0]);
				close(pipe1[1]);
				close(pipe2[0]);
				close(pipe2[1]);
				sprintf(buffer2, "%d", counter);
				fputs(buffer2, fd);
				fclose(fd);
				kill(pid, SIGTERM);
				exit(EXIT_SUCCESS);
			}
			counter++;
			sprintf(buffer2, "%d", counter);
			fputs(buffer2, fd);
			fclose(fd);

			write(parentW, buf, 1);// this tells the child to continue
		}// end of the parent while loop
	}// end else

	return EXIT_SUCCESS;
}// end of main
