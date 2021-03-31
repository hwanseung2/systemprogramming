#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int infd, outfd;
	int n;
	char buf;
	int pid;
	int fd[2];
	
	pipe(fd);

	if ((pid = fork()) == 0)
	{
		close(fd[0]);

		if((infd = open(argv[1], O_RDONLY)) < 0)
		{
			perror("error");
			exit(0);
		}

		while((n=read(infd, &buf, 1)) != 0)
		{
			printf("write to pipe[1] %d, %c\n", pid, buf);
			write(fd[1], &buf, n);
		}
		close(infd);
		close(fd[1]);
		exit(0);
	}
	else
	{
		close(fd[1]);

		if((outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR, S_IWUSR)) < 0)
		{
			perror("error");
			exit(0);
		}

		while ((n=read(fd[0], &buf, 1)) != 0)
		{
			printf("read from pipe[0] %d, %c\n", pid, buf);
			sleep(1);
			write(outfd, &buf, n);
		}

		close(outfd);
		close(fd[0]);
		//wait();
	
	}
	return 0;
}
