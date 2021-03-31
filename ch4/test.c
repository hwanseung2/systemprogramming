#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int fd1, fd2;
	int n = 1;

	char buf;
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)
	{
		perror("error");
		exit(0);
	}
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IWUSR)) < 0)
	{
		perror("error");
		exit(0);
	}
	int pid = fork();

	while((n = read(fd1, &buf, n)) != 0) 
	{
		printf("pid %d\n", pid);
		write(fd2, &buf, 1);
	}

	close(fd1);
	close(fd2);

	return 0;
}
