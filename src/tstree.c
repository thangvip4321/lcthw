#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <lcthw/dbg.h>
#include <lcthw/tstree.h>
#include <lcthw/list.h>

char delim ='/';
static inline TSTree *TSTree_insert_base(TSTree *root,TSTree *node,const char *key,size_t len,void *value){
	check((key !=NULL),"Fucked up real hard here , no key to insert");
	if(node ==NULL){
		node = (TSTree*) calloc(1,sizeof(TSTree));
		if(root ==NULL){
			root = node;
		}
		node->splitchar = *key;
	}
	if(*key <node->splitchar){
		node->low = TSTree_insert_base(root,node->low,key,len,value);
	}else if (*key == node->splitchar){
		if(len >1){
			node->equal = TSTree_insert_base(root,node->equal,key +1,len -1,value);
		}else{
			check((node->value == NULL),"duplicate insert into tst");
			node->value = value;
		}
	}
	else{
		node->high = TSTree_insert_base(root,node->high,key,len,value);
	}
	return node;
	error:
	return node;
	}
TSTree *TSTree_insert(TSTree *node,const char *key,size_t len,void *value){ 
	return TSTree_insert_base(node,node,key,len,value);
}
TSTree* TSTree_delim_insert_base (TSTree *node,List* list,const char *key,size_t len,void *value){
	TSTree* xxx = NULL;
	check((key != NULL),"no key to insert");
	if(node==NULL){
		node = calloc(1,sizeof(TSTree));
		node->splitchar = *key;
		node->value = NULL;
		node->stat = NULL;
		if (node->splitchar == delim){
			node->stat = Stats_create(); //ok no problem
			
			xxx = List_last(list);
			if (xxx != NULL){
				xxx->stat->n +=1;
			}
		}
	}
	if(node->splitchar == delim){
		List_push(list, node);
	}
	if(*key > node->splitchar){
		node->high = TSTree_delim_insert_base(node->high,list,key,len,value);
	}else if(*key == node->splitchar){
		if(len >1){
			node->equal = TSTree_delim_insert_base(node->equal,list,key+1,len-1,value); 
		}
		else{
			
			check((node->value == NULL),"duplicate insert into tst");
			node->value = value;
			node->stat = Stats_create();
			xxx = List_last(list);
			if(xxx !=NULL){
				xxx->stat->n +=1;
			}
		}

	}
	else{
		node->low = TSTree_delim_insert_base(node->low,list,key,len,value);
	}
	return node;
	error:
	return node;
}
TSTree* TSTree_delim_insert_update_base (TSTree *node,List* list,const char *key,size_t len,void *value,int* state){
	TSTree* xxx = NULL;
	check((key != NULL),"no key to insert");
	if(node==NULL){
		node = calloc(1,sizeof(TSTree));
		node->splitchar = *key;
		node->value = NULL;
		node->stat = NULL;
		if (node->splitchar == delim){
			node->stat = Stats_create(); //ok no problem
			
			xxx = List_last(list);
			if (xxx != NULL){
				xxx->stat->n +=1;
			}
		}
	}
	if(node->splitchar == delim){
		List_push(list, node);
	}
	if(*key > node->splitchar){
		node->high = TSTree_delim_insert_update_base(node->high,list,key,len,value,state);
	}else if(*key == node->splitchar){
		if(len >1){
			node->equal = TSTree_delim_insert_update_base(node->equal,list,key+1,len-1,value,state); 
		}
		else{
			
			if(node->value !=NULL){
				fprintf(stderr,"duplicate insert into tst");
				*state = -1;
			}
			node->value = value;
			node->stat = Stats_create();
			xxx = List_last(list);
			if(xxx !=NULL){
				xxx->stat->n +=1;
			}
			List_push(list, node);
		}

	}
	else{
		node->low = TSTree_delim_insert_update_base(node->low,list,key,len,value,state);
	}
	return node;
	error:
	return node;
}
TSTree* TSTree_delim_insert(TSTree* root,const char*key,size_t len,void* value){
	List* list = List_create();
	root = TSTree_delim_insert_base(root,list,key,len,value);
	List_destroy(list);
	return root;

	/*for(int i=0; i <delim_count-1;i++){
		node = List_pop(list);
		TSTree *last = List_last(list);
		last->node->stats;
		//for update part then 
		
	}
*/
}

void* TSTree_delim_insert_load(TSTree *root,const char *key,size_t len,void *value,Stats* src,int* state){
	check(src !=NULL,"cannot copy a NULL TSTree");
	List* list = List_create();
	root = TSTree_delim_insert_update_base(root,list,key,len,value,state);
	if(*state != -1){
	Stats* new_stat = src;
	update_tracer(list,new_stat);
	}
	return root;
	error:
	return root;
}
TSTree* TSTree_pinpoint(TSTree *root,const char *key,size_t len){
	TSTree *node = root;
	check(root !=NULL,"cant find an empty tree");
	size_t i = 0;
	while ((i <len) && (node !=NULL)){
		if (key[i] <node->splitchar){
			node = node->low;
		}else if (key[i] == node->splitchar){
			i++;
			if (i < len){
				node = node->equal;
			}
		}else{
			node = node->high;
		}
	}
		return node;
	error:
		return NULL;
	}
void* TSTree_search(TSTree *root,const char *key,size_t len){
	TSTree *node = root;
	size_t i = 0;
	while ((i <len) && (node !=NULL)){
		if (key[i] <node->splitchar){
			node = node->low;
		}else if (key[i] == node->splitchar){
			i++;
			if (i < len){
				node = node->equal;
			}
		}else{
			node = node->high;
		}
	}
		if (node)
		{
			return node->value;
		}
		else{
			return NULL;
		}
	}
void* TSTree_delim_search(TSTree* root,List* list,const char *key,size_t len){
	TSTree *node = root;
	size_t i = 0;
	while ((i <len) && (node !=NULL)){
		if (key[i] < node->splitchar){
			node = node->low;
		}else if (key[i] == node->splitchar){
			if( node->splitchar == delim){
				List_push(list, node);
			}
			i++;
			if (i < len){
				node = node->equal;
			}

		}else{
			node = node->high;
		}
	}
		if (node)
		{	
			
			if(node->splitchar != delim){
				List_push(list,node);
			}
			
			return node->value;
		}
		else{
			return NULL;
		}
}

void* TSTree_search_prefix(TSTree *root,const char *key,size_t len){
	if(len == 0){
		return NULL;
	}
	TSTree *node = root;
	TSTree *last = NULL;
	size_t i = 0;
	while (i<len && node){
		if(key[i] <node->splitchar){
			node = node->low;
		}else if(key[i] == node->splitchar){
			i++;
			if(i <len){
				if(node->value)
					last = node;
				node = node->equal;
			}
		}else{
				node = node->high;
			}
	}
		node = (node !=NULL) ? node : last;
		while ((node !=NULL) && (!node->value)){
			node = node->equal;
		}
		return node ? node->value : NULL;
}
void add_all_psb(TSTree *node,DArray *list){
	if(!node)
		return;
	if(node->value){
		DArray_push(list,node->value);
	add_all_psb(node->equal,list);
	add_all_psb(node->low,list);
	add_all_psb(node->high,list);
}
}
DArray *TSTree_possible_match_by_prefix(TSTree *root,const char* key,size_t len){
	check((key != NULL && len != 0),"error in setting up keys and len");
	int i = 0;
	TSTree* node = root;
	TSTree *last = NULL;
	DArray* list = DArray_create(sizeof(char*),3);
	while(i <len && node !=NULL){
		if(key[i] < node->splitchar)
			node = node->low;
		else if(key[i] == node->splitchar){
			i++;
			if( i < len){
				if(node->value)
					last = node;
				node = node->equal;
			}
		}else{
			node = node->high;
		}
	}
	node = (node != NULL) ? node : last;
	add_all_psb(node,list);
	return list;
	error:
	return NULL;
}
int TSTree_delete(TSTree *root,const char* key,size_t len){
	size_t i = 0;
	TSTree* node =root;
	while(i <len && node !=NULL){
		if(key[i]<node->splitchar){
			node = node->low;
		}else if(key[i]==node->splitchar){
			i++;
			node = (i ==len ) ? node: node->equal;
			}
		else{
			node = node->high;
		}
	}
	
	if(!node){
		return -1;
	}
	else{
		free(node->value);
		node->value = NULL;
		return 0;
	}
}
int update_tracer(List* list, Stats* new_stat){
	// this include the whole list of traversed delim nodes;
	//assume all delim nodes have stat parameters in them.
	//also assume that the first modified node is not rlly modified yet, for simplicity
	Node* list_last = list->last;
	Node* list_first = list->first;
	check(list_last !=NULL,"list has no node");
	Node* cur = NULL;
	Node* node = NULL;
	for(cur = node = list->first; cur != NULL && cur != list_last; cur = node = node->next){
		Stats* value1 = ((TSTree*)node->value)->stat;
		Stats* value2 = (node->next != NULL) ? ((TSTree*)node->next->value)->stat : NULL;
		if( value1 ==NULL || value2 ==NULL){
			
			break;
		}
		else{
			
			double old_mean = (value2->n != 0) ? value2->sum /value2->n : 0;
			value1->sum -= old_mean;
			value1->sumsq -= old_mean * old_mean;
		}
	
	} // should change the last node here
	free(((TSTree*)list_last->value)->stat);
	((TSTree*) list_last->value)->stat = new_stat;
	for(cur=node=list->last ; cur != NULL && cur != list_first ; cur= node= node->prev){
		Stats* value1 =((TSTree*)node->value)->stat;
		Stats* value2 =(node->prev != NULL) ? ((TSTree*)node->prev->value)->stat : NULL;
		if( value1 == NULL || value2 ==NULL ){
			
			break;
		}
		else{
			double new_mean = (value1->n != 0) ? value1->sum / value1->n : 0;
			
			value2->sum += new_mean;
			value2->sumsq += new_mean*new_mean;
		}
	}
	
	return 0;
	error:
	return -1;
}
int TSTree_delete_all_subnodes(TSTree* root,const char* key,size_t len){
	size_t i = 0;
	TSTree* node = root;
	List* list = List_create();
	while((i < len) && ( node !=NULL)){
		if(key[i] < node->splitchar){
			node = node->low;
		}else if (key[i] == node->splitchar){
			if(node->splitchar == delim){
			List_push(list,node);
			}
			i++;
			node = (i == len) ? node : node->equal;
		}else{
			node = node->high;
		}
	}
	if( !node){
		return -1;
	}
	else{
		if( node->splitchar == delim){
			Stats *empty = Stats_create();
			update_tracer(list,empty);
			TSTree* cur_last = (list->last->prev !=NULL) ? list->last->prev->value : NULL;
			if(cur_last != NULL)
				cur_last->stat->n -=1;
			if(node->equal){
				TSTree_destroy(node->equal);
				}
			
				free(node->value);
				free(node->stat);
				free(node);
				node = NULL;
		}
		else {
			List_push(list,node);
			Stats *temp = Stats_create();
			//temp->sum -= (node->stat->sum/node->stat->n);
			//temp->sumsq -=(node->stat->sum/node->stat->n)*(node->stat->sum/node->stat->n);
			//temp->n -=1;
			update_tracer(list,temp);
			TSTree* cur_last = (list->last->prev !=NULL) ? list->last->prev->value : NULL;
			if(cur_last != NULL)
				cur_last->stat->n -=1;
			free(node->value);
			node->value = NULL;
		}
		
		return 0;
	}
}
void TSTree_traverse(TSTree *node,TSTree_traverse_cb cb, void*data){
	if(!node)
		return;
	if (node->low)
		TSTree_traverse(node->low,cb,data);
	if(node->high)
		TSTree_traverse(node->high,cb,data);
	if (node->equal){
		TSTree_traverse(node->equal,cb,data);
	}
	if (node->value)
		cb(node->value,data);
}
void TSTree_destroy( TSTree *node){
	if(node == NULL)
		return;
	if(node->low)
		TSTree_destroy(node->low);
	if(node->equal){
		TSTree_destroy(node->equal);
	}
	if(node->high)
		TSTree_destroy(node->high);
	free(node);
	node = NULL;
}
