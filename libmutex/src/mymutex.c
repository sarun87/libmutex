/* Group Info - 
ksriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#include "mymutex.h"

int mythread_mutex_init(mythread_mutex_t *mutex, const mythread_mutexattr_t *attr)
{
	//mutex->block_queue = (mythread_queue_t) malloc (sizeof(struct mythread_queue));
	mutex->lock = 0;
	//mutex->owner = mythread_self();
}


int mythread_mutex_destroy(mythread_mutex_t *mutex)
{
	//free(mutex->block_queue);
	mutex->block_queue = NULL;
	mutex->lock = 0;
	mutex->owner = 0;
}

int mythread_mutex_lock(mythread_mutex_t *mutex)
{
	mythread_enter_kernel();
	if (mutex->lock == MUTEX_UNLOCKED)
	{
		mutex->lock = MUTEX_LOCKED;
		mythread_leave_kernel();
	}
	else
	{
		mythread_block(&mutex->block_queue,0);
	}
}


int mythread_mutex_unlock(mythread_mutex_t *mutex)
{
	mythread_enter_kernel();
	if (mutex->lock == MUTEX_LOCKED)
	{
		mythread_unblock(&mutex->block_queue,0);
		mutex->lock = MUTEX_UNLOCKED;
	}
	else
	{
		mythread_leave_kernel();
	}
}

