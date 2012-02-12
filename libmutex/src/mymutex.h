/* Group Info - 
ksriram Arun Sriraman
shyamp Shyam Prasad
vineet Vineet Krishnan
*/

#include "mythread.h"

#ifndef __MYMUTEX_H
#define __MYMUTEX_H


#define MUTEX_LOCKED 1
#define MUTEX_UNLOCKED 0

typedef struct mythread_mutex_t
{
	unsigned int lock;
	mythread_queue_t block_queue;
} mythread_mutex_t;

typedef struct mythread_mutexattr_t
{
	int attr;
} mythread_mutexattr_t;

int mythread_mutex_init(mythread_mutex_t *mutex, const mythread_mutexattr_t *attr);

int mythread_mutex_destroy(mythread_mutex_t *mutex);

int mythread_mutex_lock(mythread_mutex_t *mutex);

int mythread_mutex_unlock(mythread_mutex_t *mutex);

int __test_test_and_set(mythread_mutex_t *mutex);
int __test_and_set(mythread_mutex_t *mutex);

#endif