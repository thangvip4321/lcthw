#ifndef lcthw_List_h
#define lcthw_List_h
#include <stdlib.h>

typedef struct ListNode{
	 struct ListNode* next;
	 struct ListNode* prev;
	 void *value;
}Node;
typedef struct List{
	 struct ListNode *first;
	 struct ListNode *last;
	 int count;
}List; 
List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);
int List_count(List *A);
void* List_first(List *A);
void* List_last(List *A);
Node* Node_next(Node *node);
Node* Node_prev(Node *node);
Node* List_fn(List *A);
Node *List_ln(List *A);
void List_push(List *list,void *value);
void *List_pop(List *list);
void List_unshift(List *list,void *value);
void *List_shift (List *list);
void *List_remove(List *list,Node *node);
List *List_merge(List *list1,List *list2);
#define LIST_FOREACH(l,f,n,next,cur) Node *node =NULL;Node *cur =NULL; for(cur=node=l->f; node == n && node !=NULL ;cur=node=node->next)
#endif