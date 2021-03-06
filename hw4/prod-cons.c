#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "linkedlist.h"

void *producer(void*);
void *consumer(void*);
#define MAX_ITEM 100

int count = 0;
int in =0, out = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

//struct LinkedList *L = (struct LinkedList*)malloc(sizeof(struct LinkedList));
//struct LinkedList *L;
//ListInit(L);

void ListInit(struct LinkedList* plist){
	plist->numOfItems = 0;
	plist->head = NULL;
	plist->tail = NULL;
}

int isEmpty(struct LinkedList* plist){
	if(plist->numOfItems ==0){
		return TRUE;
	}
	else{
		return FALSE;
	}
}
int isFull(struct LinkedList* plist){
	if(plist->numOfItems == MAX_ITEM){
		return TRUE;
	}else{
		return FALSE;
	}
}

void insertItem(struct LinkedList* plist, int data){
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;

	if(plist->head == NULL){
		plist->head = newNode;
		plist->tail = newNode;
	}
	else{
		plist->tail->next = newNode;
		plist->tail = newNode;
	}
	(plist->numOfItems)++;
}

int getItem(struct LinkedList* plist){
	struct Node* tmpNode;
	int ret;

	ret = plist->head->data;
	tmpNode = plist->head;
	plist->head = plist->head->next;

	free(tmpNode);
	(plist->numOfItems)--;

	return ret;
}

void freeAllNode(struct LinkedList* plist){
	struct Node* tmpNode;
	tmpNode = plist->head;

	while(tmpNode != NULL){
		plist->head = plist->head->next;
		free(tmpNode);
		tmpNode = plist->head;

		(plist->numOfItems)--;
	}
}

int main(void)
{
	int i;
	pthread_t threads[2];
	struct LinkedList *plist = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	ListInit(plist);
	pthread_create(&threads[0], NULL, producer, (void*)plist);
	pthread_create(&threads[1], NULL, consumer, (void*)plist);
	for(i=0;i<2;i++)
		pthread_join(threads[i], NULL);
	freeAllNode(plist);
	return 0;
}

void *producer(void *plist)
{
	int i;

	//for (i=0; i<1000;i++)
	for(i =0; i<100;i++)
	{

		pthread_mutex_lock(&mutex);
		if(isFull((struct LinkedList*)plist))
			pthread_cond_wait(&buffer_has_space, &mutex);
		//in = (in + 1) % MAX_ITEM;
		//buffer[in] = i;
		int random_data = rand() % 100;
		insertItem((struct LinkedList*)plist, random_data);
		//count++;
		pthread_cond_signal(&buffer_has_data);
		pthread_mutex_unlock(&mutex);
		
	}
}

void *consumer(void *plist)
{
	int i, data;

	//for (i=0;i<1000;i++){
	for(i = 0; i<100;i++){
		pthread_mutex_lock(&mutex);
		if(isEmpty((struct LinkedList*)plist))
			pthread_cond_wait(&buffer_has_data, &mutex);
		//out = (out+1) % MAX_ITEM;
		//data = buffer[out];
		//count--;
		data = getItem((struct LinkedList*)plist);
		pthread_cond_signal(&buffer_has_space);
		pthread_mutex_unlock(&mutex);
		printf("data = %d\n", data);
	}
}
