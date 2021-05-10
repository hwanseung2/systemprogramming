#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define TRUE 1
#define FALSE 0

#define NUM_RECORDS 100

struct record{
	char name[20];
	int id;
	int balance;
};

int reclock(int fd,int recno,int len,int type);
void display_record(struct record *curr);

int main(int argc, char *argv[])
{
  struct record current;
  int record_no,transfer_rec_no;
  int fd,pos,pos2,i,n;
  char yes;
  char operation;
  int amount;
  char buffer[100];
  int quit= FALSE;

  fd = open(argv[1], O_RDWR | O_CREAT,S_IRUSR | S_IWUSR);
  while(1)
  {
	if (quit == TRUE) break;
	printf("enter account number(0-99): ");
	scanf("%d",&record_no);
	fgets(buffer,100,stdin);

	if(record_no <0 && record_no >= NUM_RECORDS)
		break;
	printf("enter operation name(c/r/d/w/t/q): ");
	scanf("%c",&operation);

	switch(operation){
	case 'c' :
		reclock(fd,record_no,sizeof(struct record),F_WRLCK);
		pos = record_no * sizeof(struct record);
		lseek(fd,pos,SEEK_SET);
		printf("> id ? ");
		scanf("%d",&current.id);
		printf("> name ? ");
		scanf("%s",&current.name);
		current.balance = 0;
		n = write(fd,&current,sizeof(struct record));
		display_record(&current);
		reclock(fd,record_no,sizeof(struct record),F_UNLCK);
		break;
	case 'r':
		reclock(fd,record_no,sizeof(struct record),F_RDLCK);
		pos = record_no * sizeof(struct record);
		lseek(fd,pos,SEEK_SET);
		n = read(fd,&current,sizeof(struct record));
		display_record(&current);
		reclock(fd,record_no,sizeof(struct record),F_UNLCK);
		break;
	case 'd':
		reclock(fd,record_no,sizeof(struct record),F_WRLCK);
		pos = record_no * sizeof(struct record);
		lseek(fd,pos,SEEK_SET);
		n = read(fd,&current,sizeof(struct record));
		display_record(&current);
		printf("enter amount\n");
		scanf("%d",&amount);
		current.balance += amount;
		lseek(fd,pos,SEEK_SET);
		write(fd,&current,sizeof(struct record));
		reclock(fd,record_no,sizeof(struct record),F_UNLCK);
		break;
	case 'w':
		reclock(fd,record_no,sizeof(struct record),F_WRLCK);
		pos = record_no * sizeof(struct record);
		lseek(fd,pos,SEEK_SET);
		n = write(fd,&current,sizeof(struct record));
		display_record(&current);
		printf("enter amount\n");
		scanf("%d",&amount);
		if(amount > current.balance){
			printf("error amount > balance\n");
			reclock(fd,record_no,sizeof(struct record),F_UNLCK);
			break;
			}
		current.balance -= amount;
		display_record(&current);
		lseek(fd,pos,SEEK_SET);
		write(fd,&current,sizeof(struct record));
		reclock(fd,record_no,sizeof(struct record),F_UNLCK);
		break;
	case 't':
		reclock(fd,record_no,sizeof(struct record),F_WRLCK);
		printf("enter account number to transfer (0~99): ");
		scanf("%d",&transfer_rec_no);
		if(current.name[0]== 0){
			printf("invalid account\n");
			break;
		}
		reclock(fd,transfer_rec_no,sizeof(struct record),F_WRLCK);
		pos = record_no * sizeof(struct record);
		lseek(fd,pos,SEEK_SET);

		n = read(fd,&current,sizeof(struct record));
		display_record(&current);
		printf("enter amount: ");
		scanf("%d",&amount);
		if(amount > current.balance){
			printf("error amount > balance\n");
			reclock(fd,record_no,sizeof(struct record),F_UNLCK);
			reclock(fd,transfer_rec_no,sizeof(struct record),F_UNLCK);
			break;
		}
		current.balance -= amount;
		display_record(&current);
		lseek(fd,pos,SEEK_SET);
		write(fd,&current,sizeof(struct record));

		pos2 = transfer_rec_no * sizeof(struct record);
		lseek(fd,pos2,SEEK_SET);
		n = read(fd,&current,sizeof(struct record));
		current.balance += amount;
		lseek(fd,pos2,SEEK_SET);
		write(fd,&current,sizeof(struct record));

		reclock(fd,record_no,sizeof(struct record),F_UNLCK);
		reclock(fd,transfer_rec_no,sizeof(struct record),F_UNLCK);
		break;
	case 'q':
		quit =TRUE;
		break;
	default :
		printf("illegal input\n");
		continue;
	}
  }
	close(fd);
	fflush(NULL);
}
int reclock(int fd,int recno,int len,int type){
	struct flock f1;
	switch(type){
		case F_RDLCK:
		case F_WRLCK:
		case F_UNLCK:
			f1.l_type = type;
			f1.l_whence = SEEK_SET;
			f1.l_start = recno * len;
			f1.l_len = len;
			fcntl(fd,F_SETLKW,&f1); // blocking
			return 1;
		default:
			return -1;
	}
}
void display_record(struct record *curr){
	printf("\n");
	printf("id: %d\n",curr -> id);
	printf("name: %s \n",curr -> name);
	printf("balance: %d\n",curr -> balance);
	printf("\n");
}

