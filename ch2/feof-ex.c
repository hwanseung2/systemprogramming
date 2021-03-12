#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int stat = 0;
	FILE *fp;
	char buf[256];

	fp = fopen("test.dat", "r");
	while(!stat)
		if(fgets(buf, 256, fp))
			printf("%s\n", buf);
		else
			stat = feof(fp);
	printf("feof returned %d\n", stat);
	fclose(fp);
	return 0;

}
