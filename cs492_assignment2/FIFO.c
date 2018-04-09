/*
** Matthew Aquiles, Thomas Falsone
** 4/13/2018
** 
*/

#include "FIFO.h"

int fifp_q_init(fifo_queue* q){
    q->size = 0;
    q->head = NULL;
    return 0;
}

void* fifo_q_pop(fifo_queue* q){
    void *obj;
    q_node *node;

    if(q == NULL){
        return NULL;
    }

    node = q->head->previous;
    object = node->object;

    if(q->size-- == 1){
        q->head = NULL;
    }
    else{
        q->head->previous = node->previous;
        node->previous->next = q->head;
    }
    return (void*)object;
}

int fifo_q_push(fifo_queue* q, void* obj){
    fifo_node* node;

    if(q == NULL || obj == NULL){
        return -1;
    }

    node = (fifo_node*)xmalloc(sizeof(fifo_node));
    node->object = obj;

    if(q->size < 1){
        q->head = node;
        node->previous = node;
        node->next = node;
    }
    else{
        node->next = q->head;
        node->previous = q->head->previous;
        node->previous->next = node;
        node->next->previous = node;
        q->head = node;
    }
    q->size++;
    return 0;
}