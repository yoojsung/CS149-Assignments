#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
	//time using shared memory
	const char *name = "time_shm";
	const int SIZE = 24;

	struct timeval start, end;
	pid_t pid;
	int shm_fd;
	char *ptr;

	if (argc == 1)
		return 0;
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	if (shm_fd == -1) {
		printf("failed to create shared memory\n");
		return -1;
	}
	ftruncate(shm_fd, SIZE);

	ptr = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("memory map failed\n");
		return -1;
	}

	pid = fork();

	if (pid == 0) {
		if (gettimeofday(&start, NULL) != 0)
			return 0;
		long int startS = start.tv_sec;
		long int startM = start.tv_usec;
		printf("child: %ld seconds \n", startS);
		printf("child: %06ld microseconds\n", startM);
		ptr += sprintf(ptr, "%ld.%06ld\n", startS, startM);

		execvp(argv[1], &argv[1]);
   		
		//should not print this if execvp worked
		fprintf(stderr, "error");
	}
	else { 
		//parent
		wait(NULL);
		if (gettimeofday(&end, NULL) != 0) 
			return -1;
		long int endS = end.tv_sec;
		long int endM = end.tv_usec;
		printf("parent: %ld seconds \n", endS);
		printf("parent: %06ld microseconds \n", endM);
		double temp = (double)endS + ((double)endM * 0.000001);
		printf("duration: %f seconds\n", temp - atof((char*)ptr));
	}
	return 0;
}

