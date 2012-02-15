/* Group Info - 
asriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#ifndef __MYMUTEX_H
#define __MYMUTEX_H

#include "mythread.h"

/*
 * Mutex values
 */ 
#define MUTEX_LOCKED 1
#define MUTEX_UNLOCKED 0

/*
 * Structure - mythread_mutex_t
 */
typedef struct mythread_mutex_t
{
	unsigned int lock;	// Lock variable that holds the mutex values.
	mythread_queue_t block_queue; // Queue that holds the threads that are waiting on the mutex.
} mythread_mutex_t;

/*
 * Mutex attribute structure
 */
typedef struct mythread_mutexattr_t
{
	int attr;
} mythread_mutexattr_t;

/*
 * Initialize the mutex variable to UNLOCKED
 */
int mythread_mutex_init(mythread_mutex_t *mutex, const mythread_mutexattr_t *attr);

/*
 * Re-initialize the mutex variable to UNLOCKED
 */
int mythread_mutex_destroy(mythread_mutex_t *mutex);

/*
 * The mutex value is changed to LOCKED. This uses the test-test-and-set lock. The
 * thread that calls this method polls till the mutex value becomes UNLOCKED
 */
int mythread_mutex_lock(mythread_mutex_t *mutex);

/*
 * Change the value of mutex to UNLOCKED
 */
int mythread_mutex_unlock(mythread_mutex_t *mutex);

/*
 * test-test-and-set method. used to check mutex variable. Tiy count = 100
 */
int __test_test_and_set(mythread_mutex_t *mutex);

/*
 * test-and-set method used to check mutex variable. Called by __test_test_and_set
 */
int __test_and_set(mythread_mutex_t *mutex);

#endif
