#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "linkedlist.c"

void *prodcuer(void *plist);
void *consumer(void *plist);

pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

void *producer(void *plist)
{
	int i,data;
	for( i =0; i < 250; i++)
	{
		pthread_mutex_lock(&mutex);
		if(isFull((struct LinkedList*)plist)){
			printf("is FULL!!!!!!!!\n");
			pthread_cond_wait(&buffer_has_space,&mutex);
		}
		data = rand() % 100;
		insertItem((struct LinkedList*)plist,data);
		//usleep(10);
		printf("producer = %d data = %d\n",i,data);
		pthread_cond_signal(&buffer_has_data);
		pthread_mutex_unlock(&mutex);
	}
}
void *consumer(void *plist)
{
	int i,data;
	for(i =0; i < 250; i++){
		pthread_mutex_lock(&mutex);
		if(isEmpty((struct LinkedList*)plist)){
			printf("is EMPTY!!!!!\n");
			pthread_cond_wait(&buffer_has_data,&mutex);
		}
		data = getItem((struct LinkedList*)plist);
		//usleep(10);
		pthread_cond_signal(&buffer_has_space);
		pthread_mutex_unlock(&mutex);
		printf("consumer = %d  data = %d\n",i,data);
	}
}

int main()
{
	int i;
	pthread_t threads[2];
	struct LinkedList *plist = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	ListInit(plist);

	pthread_create(&threads[0],NULL,producer,(void*)plist);
	pthread_create(&threads[1],NULL,consumer,(void*)plist);
	for(i =0; i < 2; i++)
		pthread_join(threads[i],NULL);
	
	freeAllNode(plist);
	return 0;
}
