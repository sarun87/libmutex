/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#include "mycond.h"

int mythread_cond_init(mythread_cond_t *cond, const mythread_condattr_t *attr)
{
	cond->cond_var = 0;
	return 0;
}
int mythread_cond_destroy(mythread_cond_t *cond)
{
	cond->cond_var = 0;
	return 0;
}
int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t *mutex)
{
	while (cond->cond_var == 0)
	{
		mythread_enter_kernel();
		mythread_block_phase1(&(cond->block_queue), 0);
		mythread_mutex_unlock(mutex);
		mythread_enter_kernel();
		mythread_block_phase2();
		mythread_mutex_lock(mutex);
	}
	(cond->cond_var)--;
	return 0;
}
int mythread_cond_signal(mythread_cond_t *cond)
{
	(cond->cond_var)++;
	mythread_enter_kernel();
	mythread_unblock(&(cond->block_queue),0);
	return 0;
}
int mythread_cond_broadcast(mythread_cond_t *cond)
{
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
