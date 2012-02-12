#include "mythread.h"
#include "myqueue.h"
#include "mymutex.h"

#ifndef __MYCOND_H
#define __MYCOND_H

typedef struct mythread_cond
{
	unsigned int cond_var;
	mythread_queue_t block_queue;
} mythread_cond_t;

typedef struct mythread_condattr
{
	unsigned int attr;
} mythread_condattr_t;

int mythread_cond_init(mythread_cond_t *cond, const mythread_condattr_t *attr);
int mythread_cond_destroy(mythread_cond_t *cond);
int mythread_cond_wait(mythread_cond_t *cond, mythread_mutex_t *mutex);
int mythread_cond_signal(mythread_cond_t *cond);
int mythread_cond_broadcast(mythread_cond_t *cond);

#endif