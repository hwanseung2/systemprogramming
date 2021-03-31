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
	int fd[2];
	int status;
	//printf("현재 parent pid question : %d\n", getpid());
	/*if ((outfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IWUSR)) < 0){
		perror("error");
		exit(0);
	}*/


	if((infd = open(argv[1], O_RDONLY)) < 0)
	{
		perror("error");
		exit(0);
	}

	int pid1 = fork();

	if (pid1 == 0)
	{
		while ((n = read(infd, &buf, n)) != 0)
		{
		printf("process id : %d %s\n", getpid(), buf);
		sleep(1);	
		}
	exit(0);
	}
	
	int pid2 = fork();
	
	if(pid2 == 0)
	{
		while((n = read(infd, &buf, n)) != 0)
		{
			printf("process id : %d %s\n", getpid(), buf);
			sleep(1);
		}
	exit(0);
	}

	if(pid1 > 0 && pid2 > 0)
	{
		while((n = read(infd, &buf, n))!=0)
		{
			printf("process id : %d %s\n", getpid(), buf);
			sleep(1);
		}
	
	}

	close(infd);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	//close(outfd);
	return 0;
}
