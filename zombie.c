#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) 
{
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "Unable to create child process\n");
		return -1;
	}
	else if (pid == 0) {
		printf("This is the child process with pid of %d\n", getpid());
		return 0;
	}
	else {
		printf("This is the parent process with pid of %d and child pid of %d\n", getpid(), pid);
		sleep(10);
		printf("Parent exiting\n");
		return 0;
	}
}
