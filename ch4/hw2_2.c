#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int infd, outfd;
	int n = 10;
	char buf[20];
	int pid;
	int fd1[2], fd2[2];
	int status;
	
	if((infd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("error");
		exit(0);
	}
	if((outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR, S_IWUSR)) < 0 )
	{
		perror("error");
		exit(0);
	}

	pipe(fd1);
	pipe(fd2);

	int pid1 = fork();
	int pid2 = fork();

	if(pid1 > 0 && pid2 >0)
	{
		close(fd1[0]);

		while((n=read(infd, &buf, n)) != 0)
		{
			write(fd1[1], buf, n);
		}
		close(infd);
		close(fd1[1]);
		//exit(0);
	}
	if(pid1 == 0)
	{
		close(fd1[1]);
		close(fd2[0]);

		while((n=read(fd1[0], &buf, n)) != 0)
		{
			for(int i=0;i<10;i++)
			{
				if(buf[i] >= 'a' && buf[i] <= 'z')
				{
					buf[i] = buf[i] - 32;
				}
			}
			write(fd2[1], buf, n);
		}
		close(fd1[0]);
		close(fd2[1]);
		exit(0);
	}
	if(pid2 == 0)
	{
		close(fd2[1]);

		while((n = read(fd2[0], &buf, n)) != 0)
		{
			write(outfd, &buf, n);
		}
		close(fd2[0]);
		close(outfd);
		exit(0);
	}
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	close(infd);
	close(outfd);
	exit(0);
	return 0;
}
