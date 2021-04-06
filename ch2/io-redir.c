#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int backup_des, stdout_des, of_des;

	stdout_des = fileno(stdout);
	printf("stdout des : %d\n", stdout_des);
	backup_des = dup(stdout_des);
	printf("backup des : %d\n", backup_des);

	printf("Hello world! (1)\n");

	of_des = open("test.txt", O_RDONLY | O_CREAT | O_TRUNC, S_IRUSR, S_IWUSR);
	printf("ofdes : %d\n", of_des);
	
	dup2(of_des, stdout_des);
	printf("Hello World (2)\n");

	dup2(backup_des, stdout_des);
	printf("Hello World! (3)\n");

	close(of_des);

	return 0;
}
