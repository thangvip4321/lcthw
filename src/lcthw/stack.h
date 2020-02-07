#ifndef STACK_H
#define STACK_H
#include <lcthw/list.h>
typedef struct List Stack;
#define Stack_create() List_create()
#define Stack_destroy(stack) List_destroy(stack)
#define Stack_push(stack,data) List_push(stack,data)
#define Stack_pop(stack) List_pop(stack)
#define Stack_peek(stack) (List_last(stack) != NULL ?stack->last->value : NULL)
#define Stack_count(stack) List_count(stack)
#define STACK_FOREACH(stack,x) Node *x=NULL;for(x = stack->first;x !=NULL;x = x->next)
#endif