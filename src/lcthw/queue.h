#ifndef QUEUE_H
#define QUEUE_H
#include <lcthw/list.h>
#include <lcthw/darray.h>

typedef struct List Queue;
#define Queue_create() List_create()
#define Queue_destroy(queue) List_destroy(queue)
#define Queue_send(queue,value) List_push(queue,value)
#define Queue_peek(queue) (queue->first !=NULL?queue->first->value:NULL)
#define Queue_count(queue) List_count(queue)
#define Queue_recv(queue) List_remove(queue,queue->first)//Node *node = queue->first; if(node !=NULL){if(node->next !=NULL){node->next->prev =NULL;};queue->first=node->next;free(node);}
#define QUEUE_FOREACH(queue,cur) Node *cur =NULL;for(cur = queue->first;cur !=NULL;cur=cur->next) 
#if 0
typedef struct DArray Queue;
#define Queue_create(a,b) DArray_create(a,b)
#define Queue_destroy(queue) DArray_destroy(queue)
#define Queue_send(queue,value) DArray_push(queue,value)
#define Queue_peek(queue) (queue->contents !=NULL ?queue->contents[0]:NULL)
#define Queue_count(queue) DArray_count(queue)
#define Queue_recv(queue) 
#endif
#endif
