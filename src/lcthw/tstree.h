#ifndef _lcthw_TSTree_h
#define _lcthw_TSTree_h
#include <stdlib.h>
#include<lcthw/darray.h>
#include <lcthw/stats.h>
#include <lcthw/list.h>
typedef struct TSTree{
	char splitchar;
	struct TSTree *low;
	struct TSTree *high;
	struct TSTree *equal;
	Stats* stat;
	void* value;
}TSTree;
void *TSTree_search(TSTree *root,const char* key,size_t len);
void* TSTree_delim_search(TSTree *root,List* list,const char* key,size_t len);
TSTree* TSTree_pinpoint(TSTree* root,const char* key,size_t len);
int TSTree_delete_all_subnodes(TSTree* node,const char* key,size_t len);
TSTree* TSTree_insert(TSTree *root,const char* key,size_t len,void* value);
void *TSTree_search_prefix(TSTree *root,const char *key,size_t len);
void* TSTree_delim_search_prefix(TSTree *root,const char *key,size_t len);
typedef void (*TSTree_traverse_cb) (void *value,void *data);
typedef void (*TSTree_delim_traverse_cb) (void *value,void *data);
TSTree *TSTree_delim_insert(TSTree *node,const char* key,size_t len,void *value);
TSTree* TSTree_delim_insert_update(TSTree *node,const char* key,size_t len,void *value);
DArray *TSTree_possible_match_by_prefix(TSTree *root,const char* key,size_t len);
void TSTree_traverse(TSTree *node,TSTree_traverse_cb cb,void *data);
void TSTree_destroy(TSTree *root);
int TSTree_delete(TSTree* root,const char* key,size_t len);
int update_tracer(List* list, Stats* new_stat);
#endif
