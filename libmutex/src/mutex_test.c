#include "mythread.h"
#include "mymutex.h"

int shared;
mythread_mutex_t global_lock;

void *helloClone(void *test)
{
	int myPid;

	myPid = mythread_self();

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


int main()
{
	mythread_t testThreads[3];
	int i;
	mythread_mutex_init(&global_lock,NULL);

	//mythread_set_loglevel(VDEBUG | VINFO);
	for(i = 0; i < 3; i++)
	{
		if (mythread_create(&testThreads[i],NULL,helloClone,"World") != 0)
		{
			fprintf(stderr,"Error in creation!\n");
		}
		else fprintf(stdout,"Successfully created!\n");
	}

	for (i = 0; i< 3; i++)
	{
		printf("\nWaiting on threads %d..",testThreads[i]);
		mythread_join(testThreads[i],NULL);
		printf("\ndone");
	}
	printf("\nDoneALL");
	mythread_mutex_destroy(&global_lock);
	return 0;
}
