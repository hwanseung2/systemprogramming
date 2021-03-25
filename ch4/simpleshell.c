#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
	char buf[300];
	char *arg[10]; //argument를 받는다.
	char *str;
	char *save; // 스트링 분리
	int argn; //arg number

	static const char tok[] = " \t\n"; //분리하기 위한 토큰
	char name[256];
	int pid; //fork를 통해 반환받을 pid

	system("clear"); //command를 그대로 실행할 수 있는 성격
	while(1)
	{
		printf("[MyShell #] ");
		gets(buf);
		argn =0;

		str = strtok_r(buf, tok, &save);
		while(str != NULL)
		{
			arg[argn++] = str;
			str = strtok_r(NULL, tok, &save);
		}

		arg[argn] = (char *)0;

		if (strcmp(arg[0], "quit") ==0 ){
			break;
		}
		if((pid = fork()) == 0){
			//child일 경우,
			execvp(arg[0], arg);
		}

		else if(pid > 0){
		//parent일 경우ㅡ,
		pid = wait(NULL);
		}

		else{
		puts("Error\n");

		}

	}



	return 0;

}
