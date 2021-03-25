#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int infd, outfd;
	int n;
	char buf;
	int pid;

	if((infd = open(argv[1], O_RDONLY)) < 0)
	{
	perror("error");
	exit(0);
	}
	

	if ((outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR, S_IWUSR)) <0)
	{
		perror("error");
		exit(0);
	}
	pid = fork();



	while((n = read(infd, &buf, 1)) != 0 )
	{
		printf("%d, %c\n", pid, buf);
		sleep(1);
		write(outfd, &buf, n);
	}

	close(infd);
	close(outfd);

	return 0;
}
