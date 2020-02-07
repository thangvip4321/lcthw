#include<lcthw/list_algos.h>
#include <string.h>
#include <stdio.h>
#include <lcthw/list.h>
#include <lcthw/dbg.h>
int swap_nodes(Node* A,Node *B){
	check(A && B,"both nodes must not be null");
	check((A->next==B && B->prev ==A) ||(A->prev ==B && B->next ==A),"nodes are not next to each other");
	void* temp =A->value;
	A->value =B->value;
	B->value =temp;
	return 0;
	error:
	return 1;
}
int List_bubble_sort(List *list,List_compare cmp){
	if(List_count(list)<=1){
		return 0;
	}
	else{
			LIST_FOREACH(list,last,list->first,prev,cur){
		int has_sorted = 0;
		LIST_FOREACH(list,first,cur,next,cur1){
			debug("qwe %lx \n",cur1);
			if(cur1->next){
				if(cmp(cur1->value,cur1->next->value)>0){
				swap_nodes(cur1,cur1->next);
				has_sorted =1;
			}
			}
		}
		if(has_sorted ==0)
			break;
		}
	}
	return 0;
	}
static List *merge_and_sort(List *l1,List *l2,List_compare cmp){
	check(l1 && l2,"no NULL lists are allowed");
	List * list_temp = List_create();
	while(l1->first && l2->first){
		if(cmp(l1->first->value,l2->first->value)>0){
		char *val = List_shift(l1);
		List_push(list_temp,val);
		}
		else{
		char *val = List_shift(l2);
		List_push(list_temp,val);
	 		}
	}
	if(l1->first){
		list_temp =List_merge(list_temp,l1);
	}
	if(l2->first){
		list_temp = List_merge(list_temp,l2);
	}
	return list_temp;
	error:
	return NULL;
}
List* Merge_sort(List* array[],int num,List_compare cmp){
	if (num ==1)
		return array[0];
	int num1 = (num+1)/2;
	List *ar1[num1];
	for(int i =0;i<num1;i++){
		ar1[i]=merge_and_sort(array[2*i],array[2*i+1],cmp);
	}
	return Merge_sort(ar1,num1,cmp);
}
int List_insert_sorted(List *list,List_compare cmp, void *value){
	check(list,"list must not be null");
	check(value,"value mustn't be null");
	if(!list->first)
		List_push(list,value);
	else{
		LIST_FOREACH(list,last,list->first,prev,cur){
			if(cmp(value,cur->value)>0){
				Node* next = cur->next;
				if(!next){
					List_push(list,value);
				}
				else{
				Node* add_on = calloc(1,sizeof(Node));
				add_on->next = next;
				add_on->prev = cur;
				cur->next = add_on;
				next->prev = add_on;
				}
				break;
			}
		}
	}
	return 0;
	error:
	return -1;
}
List* List_merge_sort(List *list,List_compare cmp){
	check(list,"list must not be null");
	if(list->count <=1)
		return list;
	else{
		List *list_array[list->count];
		int i =0;
		list_array[0] =List_create();
		List_push(list_array[0],list->first->value);
		LIST_FOREACH(list,first,list->last,next,cur){
		if(!cur->next){
			break;
		}
		if(cmp(cur->value,cur->next->value)>0){
			List_push(list_array[i],cur->next->value);
		}
		else{
			i++;
			list_array[i] =List_create();
			List_push(list_array[i],cur->next->value);
		}
	}
	// list_array = {list1,list2,list3,...,NULL,NULL...};
	return Merge_sort(list_array,i+1,cmp);
	}
	error:
	return NULL;
}
