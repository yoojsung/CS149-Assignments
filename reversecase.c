#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

int main() 
{
	char write_msg[100] = "Hey How is it going";
	char read_msg[100];

pid_t pid;
	int pfd1[2];
	int pfd2[2];

	if (pipe(pfd1) == -1) 
		return 1;
	if (pipe(pfd2) == -1)
		return 1;

	pid = fork();

	if (pid < 0) 
		return 1;

	else if (pid > 0) {
		close(pfd1[0]);
		close(pfd2[1]);
		write(pfd1[1], write_msg, strlen(write_msg)+1);
		read(pfd2[0], read_msg, 100);
		printf("%s\n", read_msg);
		close(pfd1[1]);
		close(pfd2[0]);
	}
	
	else {
		close(pfd1[1]);
		close(pfd2[0]);
		read(pfd1[0], read_msg, 100);
		printf("%s\n", read_msg);

		int i = 0; 
		while (read_msg[i] != '\0') {
			if (islower(read_msg[i]))
				write_msg[i] = read_msg[i] - 32;
			else if (isupper(read_msg[i]))
				write_msg[i] = read_msg[i] + 32;
			i++;
		}
		write(pfd2[1], write_msg, strlen(write_msg)+1);
		close(pfd1[0]);
		close(pfd2[1]);
	}
	return 0;
}

