/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#include "mymutex.h"

int mythread_mutex_init(mythread_mutex_t *mutex, const mythread_mutexattr_t *attr)
{
	mutex->lock = 0;
	return 0;
}

int mythread_mutex_destroy(mythread_mutex_t *mutex)
{
	mutex->block_queue = NULL;
	mutex->lock = 0;
	return 0;
}

int mythread_mutex_lock(mythread_mutex_t *mutex)
{
	int status = __test_test_and_set(mutex);
	if(status == -1){
		mythread_enter_kernel();
		mythread_block(&mutex->block_queue,0);
	}
	else if(status == 0)
	{
		return 0;
	}
}

int mythread_mutex_unlock(mythread_mutex_t *mutex)
{
	mythread_enter_kernel();
	mutex->lock = MUTEX_UNLOCKED;
	mythread_leave_kernel();
	return 0;
}

//Just for shyam
int __test_and_set(mythread_mutex_t *mutex)
{
	return compare_and_swap(&mutex->lock,MUTEX_LOCKED,MUTEX_UNLOCKED);
}

int __test_test_and_set(mythread_mutex_t *mutex)
{
	int counter = 100;
	while (counter > 0)
	{
		while ( mutex->lock == MUTEX_LOCKED);
		if (__test_and_set(mutex)  == MUTEX_UNLOCKED)
		{
			return 0;
		}
		counter--;
	}
	return -1;
}
