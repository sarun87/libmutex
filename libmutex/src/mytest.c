#include <stdio.h>
#include <unistd.h>
//#include <mythread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "mythread.h"
#include "mymutex.h"
#include "mycond.h"

#define MAX_COUNT 50
int shared;
mythread_mutex_t global_lock;


void *helloClone(void *test)
{
	int myPid;

	myPid = (int)(mythread_self())->tid;

	printf("\n%d:Hello, %s\n",myPid,(char *) test);
	sleep(2);
	printf("\n%d:Yielding..\n",myPid);
	mythread_mutex_lock(&global_lock);
	printf("\nIn critical section.");
	shared = myPid;
	mythread_mutex_unlock(&global_lock);
	printf("\n%d:Set shared to %d.\n",myPid,myPid);
	printf("\n%d:Exiting.\n",myPid);
	mythread_exit(NULL);
	return 0;
}

void test_case1()
{
	int status;
	mythread_t testThreads[3];
	int i;
	status = mythread_mutex_init(&global_lock,NULL);
	if(status !=0)
	{
		printf("Test Case 1: Mutex- init, lock, unlock and destroy.Status: FAIL!\n");
	}

	//mythread_set_loglevel(VDEBUG | VINFO);
	for(i = 0; i < 3; i++)
	{
		if (mythread_create(&testThreads[i],NULL,helloClone,"World") != 0)
		{
			printf("Test Case 1: Mutex- init, lock, unlock and destroy.Status: FAIL!\n");
			return;
		}
		else fprintf(stdout,"Successfully created!\n");
	}

	for (i = 0; i< 3; i++)
	{
		printf("\nWaiting on threads %d..",testThreads[i]);
		mythread_join(testThreads[i],NULL);
		printf("\ndone");
	}
	status = mythread_mutex_destroy(&global_lock);
	if(status ==0){
		printf("\n==============================================");
		printf("\nTest Case 1: Mutex- init, lock, unlock and destroy.Status: PASS\n");
		printf("\n==============================================\n");
	}
	else
	{	
		printf("\n==============================================");
		printf("Test Case 1: Mutex- init, lock, unlock and destroy.Status: FAIL!\n");
		printf("\n==============================================\n");
	}
	
}

/*
 * Method to check functionality of condtion variable.
 */
typedef struct list{
char *item;
struct list *next;
}Node;

Node *begin = NULL;
Node *end = NULL;

int mutexTestFail = 0;
int condSignalTestFail = 0;

#define MAX_THREADS_PER_MARK 30

int searchThreads = 0;
int insertThreads = 0;
int deleteThreads = 0;

mythread_mutex_t adderLock;
mythread_mutex_t deleteLock;
mythread_cond_t deleteCond;
mythread_mutex_t printLock;

mythread_mutex_t searchThreadVarMutex;

char* ADDITION = "addition";
char* RETREIVAL = "retrival";
char* DELETION = "deletion";

bool addToList(char *item){
	bool success = false;
	
	Node *new = NULL;
	new = malloc(sizeof(Node));
	if(new == NULL){
		fprintf(stderr,"addToList: Malloc failed");
		return false;
	}

	char * buf = malloc(strlen(item) + 1);
	if(buf == NULL){
		fprintf(stderr,"addToList: Malloc failed");
		return false;
	}

	strcpy(buf,item);
	new->item = buf;
	new->next = NULL;

	if(begin == NULL || end == NULL){
		begin = new;
		end = new;
		return true;
	}

	end->next = new;
	end = end->next;
	return true;
}

void printList(){
	if(begin == NULL)
		return;

	Node *temp = begin;

	printf("==============\n");
	while(temp!=NULL){
		printf("%s\n" , temp->item);
		temp = temp->next;
	}
}

bool removeFromList(char *item){
	if(end == NULL){
		return false;
	}

	Node *temp = begin;
	Node *prev = NULL;

	while(temp != NULL){
		if(strcmp(temp->item,item)== 0){
			if(prev == NULL){ 
				begin = begin->next;
				free(temp->item);
				free(temp);
				return true;
				}
			
			if(temp->next == NULL){ 
				end = prev;
			}

			prev->next = temp->next;
			free(temp->item);
			free(temp);
			return true;
		}	
		prev = temp;	
		temp = temp->next;
	}
	return false;
}

bool searchList(char *item){
	Node * temp;
	temp = begin;
	
	if(temp == NULL){
		return false;
	}

	while(temp != NULL){
		if(strcmp(temp->item,item) == 0){
			return true;
		}
		temp = temp->next;
	}
	return false;
}

void printThreadInfo(char* operation, char* value, bool success, mythread_t tid){
	int len = strlen(value);
	value[len-1] = '\0'; //remove the endline char
	if(success)
		printf("[%08x]    Success %s [ %s ] Retrievers : %i Adders : %i Deleters : %i\n" ,tid, operation,value,searchThreads,insertThreads,deleteThreads);
	else	
		printf("[%08x]    Fail %s [ %s ] Retrievers : %i Adders : %i Deleters : %i\n" , tid , operation,value,searchThreads,insertThreads,deleteThreads);

}
/*
 * Retriever method.
 */
void retriever(void * arg){
	bool retStatus;
	// Lock for the searchThreads variable
	mythread_mutex_lock(&searchThreadVarMutex);
	 searchThreads = searchThreads+1;
	mythread_mutex_unlock(&searchThreadVarMutex);
	
	// Search from list.
	retStatus = searchList((char*)arg);
	//sleep(2);
	// Print info. Lock for the same.
	mythread_mutex_lock(&printLock);
	 printThreadInfo(RETREIVAL, (char*) arg, retStatus, mythread_self());
	mythread_mutex_unlock(&printLock);
	
	// Change searchThreads variable. Using lock.
	mythread_mutex_lock(&searchThreadVarMutex);
	 searchThreads = searchThreads-1;
	mythread_mutex_unlock(&searchThreadVarMutex);
	
	// Signal delete 
	mythread_mutex_lock(&deleteLock);
	if(searchThreads == 0)
	{
		mythread_cond_signal(&deleteCond);
	}
	mythread_mutex_unlock(&deleteLock);
}

/*
 * Deleter method
 */
void deleter(void * arg){
	bool status;
	// Apply deleteLock. This lock is associated with global variable.
	mythread_mutex_lock(&deleteLock);
	while(insertThreads != 0 || searchThreads!=0)
	{
		mythread_cond_wait(&deleteCond, &deleteLock);
	}
	// Lock readers and adders
	mythread_mutex_lock(&searchThreadVarMutex);
	mythread_mutex_lock(&adderLock);
	deleteThreads +=1;
	if(searchThreads !=0 || insertThreads !=0)
	{
		condSignalTestFail = 1;
	}
	status = removeFromList((char *) arg);	
	mythread_mutex_lock(&printLock);
	printThreadInfo(DELETION,(char*)arg, status,mythread_self());
	mythread_mutex_unlock(&printLock);
	deleteThreads-=1;
	// Unlock all the locks
	mythread_mutex_unlock(&adderLock);
	mythread_mutex_unlock(&searchThreadVarMutex);
	mythread_mutex_unlock(&deleteLock);
}
/*
 * Adder method
 */
void adder(void *arg){
	bool status;
	// Adder Lock
	mythread_mutex_lock(&adderLock);
	insertThreads = insertThreads + 1;
	if(insertThreads > 1)
	{
		mutexTestFail = 1;
	}
	status = addToList((char*)arg);
	mythread_mutex_lock(&printLock);
	printThreadInfo(ADDITION,(char*)arg, status,mythread_self());
	mythread_mutex_unlock(&printLock);
	insertThreads = insertThreads - 1;	
	mythread_mutex_unlock(&adderLock);
	// Done with add. Unlock adderLock complete. Now signal deleters.
	mythread_mutex_lock(&deleteLock);
	if(insertThreads ==0)
		mythread_cond_signal(&deleteCond);
	mythread_mutex_unlock(&deleteLock);
}

void test_case2(){
	FILE *file;
	char operationType, word[30][20];
	mythread_t myThreads[MAX_THREADS_PER_MARK];
	char space;
	int threadCount = 0, instructionCount = 0;
	int retValue, threadIter;

	// Check input
	// Read the text file and create threads as per requirements.
	file = fopen("p5TestInput.txt", "r");
	if(file == NULL){
		printf("\nError opening file!");
		return;
	}
	// initialize mutex and condition variables.
	mythread_mutex_init(&printLock, NULL);
	mythread_mutex_init(&adderLock, NULL);
	mythread_cond_init(&deleteCond, NULL);
	mythread_mutex_init(&deleteLock, NULL);
	mythread_mutex_init(&searchThreadVarMutex, NULL);
	// Read each line from file and perform the appropriate task.
	while(!feof(file)){
		fscanf(file,"%c",&operationType);
		instructionCount++;
		if(operationType == 'M'){
					// Need to mark all threads finished the opearations.
					// that is wait for the threads to complete.
					for(threadIter = 0; threadIter < threadCount; ++threadIter){
						mythread_join(myThreads[threadIter],NULL);
					}
					// Extra \n to be removed from end of line.
					fscanf(file,"%c",&operationType);
					threadCount = 0;
					//printf("\nMark\n");
					continue;
		}
		fscanf(file,"%c",&space);
		fgets(word[threadCount],20,file);
		if(operationType =='A'){
			retValue = mythread_create(&myThreads[threadCount],NULL, (void* (*)(void*))adder, (void*) word[threadCount]);
		}
		else if(operationType == 'D'){
			retValue = mythread_create(&myThreads[threadCount], NULL, (void* (*)(void*))deleter,(void*) word[threadCount]);
		}
		else if(operationType == 'R'){
			retValue = mythread_create(&myThreads[threadCount], NULL, (void* (*)(void*))retriever,(void*) word[threadCount]);
		}
		++threadCount;
	}
	fclose(file);
	printList();
	if(condSignalTestFail == 1)
	{
		printf("\n==============================================");
		printf("\nTest-case2 Condition Signal (init, wait and signal). Status - FAIL!");
		printf("\n==============================================");
	}
	else
	{
		printf("\n==============================================");
		printf("\nTest-case2 Condition Signal (init, wait and signal). Status - PASS!");
		printf("\n==============================================");
	}
	if(mutexTestFail == 1)
	{
		printf("\n==============================================");
		printf("\nTest-case2 Mutex (init, lock, unlock). Status - FAIL");
		printf("\n==============================================\n");
	}
	else
	{
		printf("\n==============================================");
		printf("\nTest-case2 Mutex (init, lock, unlock). Status - PASS");
		printf("\n==============================================\n");
	}
}

int main()
{
	// Run test case for mutex
	test_case1();

	// Run test case for condition variable
	test_case2();
}
