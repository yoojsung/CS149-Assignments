#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 25

int main(int argc, char *argv[])
{
	pid_t pid;
	int first_pipe[2];
	
	struct timeval start, end;
	//long int startS, startM;
	//long int endS, endM;

	if (pipe(first_pipe) == -1) {
		return 1;
	}

	pid = fork();

	if (pid < 0) {
		return 1;
	}

	else if (pid > 0) {
		close(first_pipe[WRITE_END]);
		double timetaken;
		long int startS, startM;
		dup2(first_pipe[0], 0);
		scanf("%ld", &startS);
		scanf("%ld", &startM);
		gettimeofday(&end, NULL);
		long int endS = end.tv_sec;
		long int endM = end.tv_usec;
		double duration = (endS - startS) + ((endM - startM) * 1e-6);
		printf("start time: %ld.%ld\n", startS, startM);
		printf("end time: %ld.%ld\n", end.tv_sec, end.tv_usec);
		printf("duration: %f seconds\n", duration);
		close(first_pipe[READ_END]);
		wait(NULL);
	}

	else {
		close(first_pipe[READ_END]);
		if (gettimeofday(&start, NULL) != 0)
			return -1;
		long int startS = start.tv_sec;
		long int startM = start.tv_usec;
		char buffer[25];
		sprintf(buffer, "%ld %ld", start.tv_sec, start.tv_usec);
	        write(first_pipe[WRITE_END], buffer, strlen(buffer)+1);
		execvp(argv[1], argv+1);
		close(first_pipe[WRITE_END]);	
	}
}
