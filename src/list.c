#include <lcthw/list.h> 
#include  <lcthw/dbg.h> 
#include <string.h>
int List_count(List *A){
	return (A)->count;
}
void* List_last(List *A){
	return (A)->last !=NULL ? (A)->last->value : NULL;
}
void* List_first(List *A){
	return (A)->first !=NULL ? (A)->first->value :NULL;
}
Node* List_fn(List *A){
	return A->first;
}
Node* List_ln(List *A){
	return A->last;
}
Node *Node_next(Node* node){
	return node->next;
}
Node* Node_prev(Node* node){
	return node->prev;
}
List* List_merge(List *list1, List *list2){
	check(list1 || list2, "can merge sth that doesnt exist");
	if(list1->first && !list2->first)
		return list1;
	if(!list1->first && list2->first)
		return list2;
	if(!list1->first && !list2->first)
		return list1;

	// list1 is the main list, merge then destroy list2
	list2->first->prev = list1->last;
	list1->last->next = list2->first;
	list1->last = list2->last;
	free(list2); //  MAY CAUSE BUGS HERE BEWARE!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return list1;
	error:
	return NULL;
}
void List_destroy(List *list){
LIST_FOREACH(list,first,list->last,next,cur)
{ if(cur->prev)
	 free(cur->prev); 
}
	free(list->last); 
	free(list); 
}
List *List_create(){ 
	return calloc(1,sizeof(List)); 
}
void List_clear(List *list){
	LIST_FOREACH(list,first,list->last,next,cur){ 
		free(cur->value); } 
	} 
void List_clear_destroy(List *list){
 // clear everything on a list. Precondition :value !=NULL, list must have 1 element

check(list !=NULL,"no such list exist"); 
LIST_FOREACH(list,first,list->last,next,cur){ 
	if(cur->value)
		free(cur->value);
	if(cur->prev)
		{ free(cur->prev);} 
	if (list->last)
		free(list->last);
	free(list); 
	}
	error:
	return; 
} 
void List_push(List *list,void *value){ 
	//precondition List != NULL
	check((list !=NULL),"list must exist");
	Node *node =calloc(1,sizeof(Node)); 
	check_mem(node); 
	 if(list->last==NULL){ list->last =node; list->first =node; } 
	 else
	 	{ list->last->next =node;
		node->prev = list->last; 
		list->last =node; } 
	list->count++; 
	node->value = value;
	error:
	debug("gay shit"); 	
	return;  }
void *List_pop(List *list){ Node *node =list->last; return node !=NULL ? List_remove(list,node) :NULL; }

void* List_remove(List *list , Node *node){
	void *result =NULL;
	check(list,"list must not be null");
	check(list->first && list->last,"list is empty");
	check(node,"node cant be null");
	
	Node *next = node->next;
	Node *prev = node->prev;
	if(next ==NULL){
		if(prev !=NULL){
			list->last =prev;
			list->last->next =NULL;
		}
		else{
			list->first =NULL;
			list->last =NULL;
		}
	}
	else{
		if(prev ==NULL){
			list->first =next;
			list->first->prev=NULL;
		}
		else{
			next->prev = prev;
			prev->next = next;
		}
	}
	result =node->value;
	free(node);
	list->count--;
	return result;
	error:
	return result;
}
/*
void *List_remove(List * list, Node * node)
  {
    void *result = NULL;
 
    check(list->first && list->last, "List is empty.");
 	check(node, "node can't be NULL");
 	if (node == list->first && node == list->last) {
       list->first = NULL;
        list->last = NULL;
        debug("nothing goes wrong");
       } else if (node == list->first) {
           list->first = node->next;
           debug("nothing goes wrong1");
           check(list->first != NULL,
                   "Invalid list, somehow got a first that is NULL.");
           list->first->prev = NULL;
       } else if (node == list->last) {
           list->last = node->prev;
           check(list->last != NULL,
                   "Invalid list, somehow got a next that is NULL.");
           debug("nothing goes wrong2");
           list->last->next = NULL;
       } else {
           Node *after = node->next;
           Node *before = node->prev;
           after->prev = before;
           before->next = after;
           debug("nothing goes wrong3");
       }
       list->count--;
       result = node->value;
       free(node);
   error:
       return result;
   }
  */
void List_unshift(List *list,void *value){
	Node *node = calloc(1,sizeof(Node));
	check_mem(node);
	node->value =value;
	if(list->first ==NULL){
		list->last =node;
		list->first =node;
	}
	else{
		node->next =list->first;
		list->first->prev =node;
		list->first =node;
	}
	list->count++;
	error:
	return;
}
void* List_shift(List *list){
	Node *node =list->first;
	return node !=NULL ? List_remove(list,node) : NULL;
}
