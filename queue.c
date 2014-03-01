/*
 * File: queue.c
 * Author: Chris Wailes <chris.wailes@gmail.com>
 * Author: Wei-Te Chen <weite.chen@colorado.edu>
 * Author: Andy Sayler <andy.sayler@gmail.com>
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2010/02/12
 * Modify Date: 2011/02/04
 * Modify Date: 2012/02/01
 * Description:
 * 	This file contains an implementation of a simple FIFO queue.
 *  
 */

#include <stdlib.h>

#include "queue.h"

int queue_init(queue* q, int size){
    
    int i;

    /* user specified size or default */
    if(size>0) {
	q->maxSize = size;
    }
    else {
	q->maxSize = QUEUEMAXSIZE;
    }

    /* malloc array */
    q->array = malloc(sizeof(queue_node) * (q->maxSize));
    if(!(q->array)){	
	perror("Error on queue Malloc");
	return QUEUE_FAILURE;
    }

    /* Set to NULL */
    for(i=0; i < q->maxSize; ++i){
	q->array[i].payload = NULL;
    }

    /* setup circular buffer values */
    q->front = 0;
    q->rear = 0;

    /* malloc synchronization structures */
    q->mutex = malloc(sizeof(pthread_mutex_t));
    q->content_sem = malloc(sizeof(sem_t));
    q->freespace_sem = malloc(sizeof(sem_t));

    /* initialize synchronization structures */
    pthread_mutex_init(q->mutex, NULL);
    sem_init(q->freespace_sem, 0, q->maxSize);
    sem_init(q->content_sem, 0, q->maxSize);

    return q->maxSize;
}

int queue_is_empty(queue* q){
    if((q->front == q->rear) && (q->array[q->front].payload == NULL)){
	return 1;
    }
    else{
	return 0;
    }
}

int queue_is_full(queue* q){
    if((q->front == q->rear) && (q->array[q->front].payload != NULL)){
	return 1;
    }
    else{
	return 0;
    }
}

void* queue_pop(queue* q){
    void* ret_payload;
	
    /*if(queue_is_empty(q)){
	return NULL;
    }*/
	
    while(sem_wait(q->content_sem));
    pthread_mutex_lock(q->mutex);

    sem_post(q->freespace_sem);

    ret_payload = q->array[q->front].payload;
    q->array[q->front].payload = NULL;
    q->front = ((q->front + 1) % q->maxSize);

    pthread_mutex_unlock(q->mutex);

    return ret_payload;
}

int queue_push(queue* q, void* new_payload){
    
    /*if(queue_is_full(q)){
	return QUEUE_FAILURE;
    }*/

    while(sem_wait(q->freespace_sem));
    pthread_mutex_lock(q->mutex);

    sem_post(q->content_sem);

    q->array[q->rear].payload = new_payload;
    q->rear = ((q->rear+1) % q->maxSize);

    pthread_mutex_unlock(q->mutex);

    return QUEUE_SUCCESS;
}

void queue_cleanup(queue* q)
{
    while(!queue_is_empty(q)){
	queue_pop(q);
    }

    free(q->array);
    sem_destroy(q->content_sem);
    sem_destroy(q->freespace_sem);
    pthread_mutex_destroy(q->mutex);
}
