#include <stdio.h>
#include <unistd.h>
int global;
typedef int pid_t;
int main()
{

	pid_t pid;
	global = 10;
	pid = fork();

	if (pid == 0)
	{
	global--;
	printf("Child's global : %d\n", global);
	}
	else
	{
		global++;
		printf("parent's global : %d\n", global);
	}

	return 0;
}
