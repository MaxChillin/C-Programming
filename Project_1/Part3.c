#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define FALSE 0
#define TRUE 1

int main(int argc, char *argv[]) {

//	pid_t pid;
	char buf[1];
	int pipe1[2];
	int pipe2[2];
	int pipe3[2];

	pipe(pipe1);
	pipe(pipe2);
	pipe(pipe3);

	int pipeOneRead = pipe1[0];
	int pipeOneWrite = pipe1[1];

	int pipeTwoRead = pipe2[0];
	int pipeTwoWrite = pipe2[1];

	int pipeThreeRead = pipe3[0];
	int pipeThreeWrite = pipe3[1];

	if (fork() == 0) {
		/* First Child */
		while (TRUE) {
			puts("First child trying to read...");
			read(pipeThreeRead, buf, 1); // this waits for the parent to write
			write(pipeOneWrite, buf, 1); // this writes to the second child
		}
	} else {
		/* Parent */
		if (fork() == 0) {
			/* Second Child */
			while (TRUE) {
				puts("Second child trying to read...");
				read(pipeOneRead, buf, 1); // this waits for the first child to write
				write(pipeTwoWrite, buf, 1); // this writes to the parent
			}
		} else {
			/* Still the Parent */
			while (TRUE) {
				puts("Parent trying to read...");
				read(pipeTwoRead, buf, 1); // this waits for the second child to write
				write(pipeThreeWrite, buf, 1); // this writs to the first child
			} // end while loop
		} // end of second child else
	} // end of first child else

	return EXIT_SUCCESS;
}
