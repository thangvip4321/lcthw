#ifndef lcthw_List_h
#define lcthw_List_h
#include <stdlib.h>

struct ListNode;
typedef struct ListNode{
	void  *value;
	struct ListNode *prev;
	struct ListNode *next;
}Node;
typedef struct List{
	Node *first;
	Node *last;
	int count;
} List;
List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);
#define List_count(A) ((A)->count)
#define List_first(A)((A)->first !=NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last !=NULL ? (A)->last->value : NULL)
void List_push(List *list,void *value);
void *List_pop(List *list);
void List_unshift(List *list,void *value);
void *List_shift (List *list);
void *List_remove(List *list,Node *node);
#define LIST_FOREACH(list,first,next,cur) Node *node =NULL;Node *cur =NULL; for(cur=node=list->first; node !=NULL;cur=node=node->next)
#endif