#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>

int main(int argc, char *argv[])
{
	int ffd[2];
	int pipefd[2];

	char c;
	pid_t pid;
	char one[2];

	ffd[0] = open(argv[1], O_RDONLY);
	ffd[1] = open(argv[2], O_CREAT | O_RDWR, 066:6);

	pipe(pipefd);

	pid = fork();

	if (pid > 0) {
		close(pipefd[0]);
		close(ffd[1]);
		while ((c = read(ffd[0], one, 1)) > 0) {
			write(pipefd[1], one, c);
		}
		close(pipefd[1]);
		close(ffd[0]);
	}
	else { //child
		close(pipefd[1]);
		close(ffd[0]);
		while ((c = read(pipefd[0], one, 1)) > 0) {
			write(ffd[1], one, c);
		}
		close(pipefd[0]);
		close(ffd[1]);
	}
	return 0;
}

	
