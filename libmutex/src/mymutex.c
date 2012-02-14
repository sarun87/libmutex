/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#include "mymutex.h"
#include "mythread.h"

/*
 * See header file for description
 */
int mythread_mutex_init(mythread_mutex_t *mutex, const mythread_mutexattr_t *attr)
{
	mutex->lock = 0;
	return 0;
}

/*
 * See header file for description
 */
int mythread_mutex_destroy(mythread_mutex_t *mutex)
{
	mutex->block_queue = NULL;
	mutex->lock = 0;
	return 0;
}

/*
 * See header file for description
 */
int mythread_mutex_lock(mythread_mutex_t *mutex)
{
	// Check the test_test_and_set value.
	// returns -1 if mutex value is LOCKED
	int status = __test_test_and_set(mutex);
	if(status == -1){
		// If locked, wait on mutex to become unlocked.
		// this condition will happen only if 100 tries in the 
		// test-test and set fails.
		mythread_enter_kernel();
		mythread_block(&mutex->block_queue,0);
	}
	else if(status == 0)
	{
		// The test_test_and_set returns 0 when the lcok was previously
		// not set and now it is set. i.e. lock has been acquired.
		return 0;
	}
}

/*
 * See header file for description
 */
int mythread_mutex_unlock(mythread_mutex_t *mutex)
{
	// Change mutex value to UNLOCKED
	mythread_enter_kernel();
	mutex->lock = MUTEX_UNLOCKED;
	mythread_leave_kernel();
	return 0;
}

/*
 * See header file for description
 */
int __test_and_set(mythread_mutex_t *mutex)
{
	return compare_and_swap(&mutex->lock,MUTEX_LOCKED,MUTEX_UNLOCKED);
}

/*
 * See header file for description
 */
int __test_test_and_set(mythread_mutex_t *mutex)
{
	// Test and set call. 100 tries.
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
