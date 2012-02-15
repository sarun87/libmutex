/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/
#ifndef __MYCOND_H
#define __MYCOND_H

// User includes
#include "mythread.h"
#include "mymutex.h"

// Condition variable mythread_cond_t structure
typedef struct mythread_cond
{
	unsigned int cond_var;	// Value - o to +ve number
	mythread_queue_t block_queue; // Queue of threads that are blocked this cond variable.
} mythread_cond_t;

// Condition attribute - Currently not being used.
typedef struct mythread_condattr
{
	unsigned int attr;
} mythread_condattr_t;

/*
 * Initializes the condition variable with 0. Cond_attr not being used.
 * @return 0 on success
 */
int mythread_cond_init(mythread_cond_t *cond, const mythread_condattr_t *attr);

/*
 * Sets the blocking queue to NULL and the condition variable value to 0.
 */
int mythread_cond_destroy(mythread_cond_t *cond);

/*
 * Wait on condition variable "cond" and mutex "mutex". The thread that calls this
 * method is blocked till the value of condition variable cond becomes positive.
 * Once the thread is blocked, the mutex is released and later on when the thread again
 * starts execution, the mutex is re-acquired.
 */
int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t *mutex);

/*
 * Signals the condition variable. Increment the condition variable value.
 * Unblock the head of the block queue. 
 */
int mythread_cond_signal(mythread_cond_t *cond);

/*
 * Signals all the threads that are in the block queue of the condition variable.
 */
int mythread_cond_broadcast(mythread_cond_t *cond);

#endif
