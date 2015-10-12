#include <pthread.h>

struct msg {
    struct msg *m_next;
    /* ... more stuff here ... */
};

struct msg *workq;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

/* 
When using condition variables there is always a Boolean predicate involving shared variables 
associated with each condition wait that is true if the thread should proceed. 
Spurious wakeups from the pthread_cond_timedwait() or pthread_cond_wait() 
functions may occur. Since the return from pthread_cond_timedwait() or pthread_cond_wait() 
does not imply anything about the value of this predicate, the predicate should be re-evaluated 
upon such return.

If a signal is delivered to a thread waiting for a condition
variable,  upon  return  from  the signal handler the thread
resumes waiting for the condition variable as if it was  not
interrupted, or it returns 0 due to spurious wakeup.
 */

void
process_msg(void)
{
    struct msg *mp;

    for (;;) {
        pthread_mutex_lock(&qlock);
        //!! 为什么必须用循环while spurious wakeup  
        while (workq == NULL)
            pthread_cond_wait(&qready, &qlock);
        mp = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
        /* now process the message mp */
    }
}

void
enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}
