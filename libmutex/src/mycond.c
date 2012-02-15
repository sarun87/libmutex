/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#include "mycond.h"


/*
 * See header file for description
 */
int mythread_cond_init(mythread_cond_t *cond, const mythread_condattr_t *attr)
{
	// Initialize the condition variable with 0.
	cond->cond_var = 0;
	cond->block_queue = NULL;
	return 0;
}

/*
 * See header file for description
 */
int mythread_cond_destroy(mythread_cond_t *cond)
{
	// Re-initialize the condition variable with 0
	cond->cond_var = 0;
	cond->block_queue = NULL;
	return 0;
}

/*
 * See header file for description
 */
int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t *mutex)
{
	// Wait on cond_var to become 1
	while (cond->cond_var == 0)
	{
		// Invoke 1st phase blocking
		mythread_enter_kernel();
		mythread_block_phase1(&(cond->block_queue), 0);
		// Unlock the locked mutex and run block phase 2 to block the thread.
		mythread_mutex_unlock(mutex);
		mythread_enter_kernel();
		mythread_block_phase2();
		// Once out of blcoking state, acquire the mutex again before returning
		// to the function that called wait.
		mythread_mutex_lock(mutex);
	}
	// Condition Variable is reset back to 0 (If multiple threads are on wait (
	// blocking state), this will reduce the count of the number of threads.
	(cond->cond_var)--;
	// Return success
	return 0;
}

/*
 * See header file for description
 */
int mythread_cond_signal(mythread_cond_t *cond)
{
	// Make condition variable increment. (>0)
	(cond->cond_var)++;
	// Unblock the head of queue.
	mythread_enter_kernel();
	mythread_unblock(&(cond->block_queue),0);
	return 0;
}

/*
 * See header file for description
 */
int mythread_cond_broadcast(mythread_cond_t *cond)
{
	// FOr each thread blocked on the condition variable, call unblock.
	mythread_queue_t curr;
	curr = cond->block_queue;
	do
	{
		cond->cond_var++;
		mythread_enter_kernel();
		mythread_unblock(&(cond->block_queue),0);
		curr = curr->next;
	}while (curr != cond->block_queue);
	return 0;
}
