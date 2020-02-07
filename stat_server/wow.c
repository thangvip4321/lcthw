#include <stdio.h>
#include <lcthw/bstrlib.h>
#include <regex.h>
#include <lcthw/stats.h>
#include <lcthw/tstree.h>
#include <stdlib.h>
#include <lcthw/dbg.h>
char* format = "([a-zA-Z0-9]+)\\s*)";
int position = 0;
struct tagbstring commandl = bsStatic("list");
struct tagbstring commandc = bsStatic("create");
struct tagbstring commandr = bsStatic("read");
struct tagbstring commandu = bsStatic("update");
struct tagbstring commandd = bsStatic("delete");
struct bstrList* process_string(char* string){
	bstring mid = bfromcstr(string);
	struct bstrList* list = bsplit(mid,' ');
	for(int i = 0;list->entry[i] !=NULL;i++){
		btrimws(list->entry[i]);
		debug("%s",bdata(list->entry[i]));
}
return list;
}
void* list_data(TSTree *node,int *i,char *list){
	if(node == NULL)
		return NULL;
	else{
		debug("i: %d",*i);
		list[*i] = node->splitchar;
		if(node->value) {//?????
			debug("%s\n",list);
		}
		if(node->equal){
			(*i)++;
		}else{
			memset(list,0,*i);
			*i = 0;
		}
		debug("equal, i %d",*i);
		list_data(node->equal,i,list);
		debug("high ,i %d",*i);
		list_data(node->high,i,list);
		debug("low,i %d",*i);
		list_data(node->low,i,list);
	}		
}
int process_request(int argc,TSTree *root,char* commands){ // considering bstring commands
	bstring name = NULL;
	Stats *stat0 = NULL;
	int i = 0;
	debug("%s",commands);
	struct bstrList *list = process_string(commands);
	char* data = malloc(100); 
	if(bstrcmp(list->entry[0],&commandc) == 0){
	name = list->entry[1];
	check((name !=NULL),"USAGE: create <stats_name>");
	stat0 = Stats_create();
	root =TSTree_insert(root,bdata(name),blength(name),stat0);
	}
	else if(bstrcmp(list->entry[0],&commandr) == 0){
	name = list->entry[1];
	printf("names: %s",bdata(name));
	check((name !=NULL),"USAGE: read <stats_name>");
	stat0 = TSTree_search(root,bdata(name),blength(name));
	if(stat0 == NULL){
		printf("no such statistic");
	}
	else{
		Stats_dump(stat0);
	}
	}
	else if(bstrcmp(list->entry[0],&commandu) ==0){
	name = list->entry[1];
	printf("names: %s",bdata(name));
	check((name !=NULL),"USAGE: update <stats_name> <numbers>");
	stat0 = TSTree_search(root,bdata(name),blength(name));
	if(stat0 ==NULL){
		printf("no such stat");
	}else{
	for(int i =2;list->entry[i] !=NULL;i++){
			Stats_sample(stat0,atof(bdata(list->entry[i]))); // ok so string data will get processed as 0.0 which we dont want
		}
	}
	}else if(bstrcmp(list->entry[0],&commandd) ==0){
		name = list->entry[1];
		check((name !=NULL),"USAGE: delete <stats_name>");
		stat0 = TSTree_search(root,bdata(name),blength(name));
		if(stat0 == NULL)
			printf("no such stat");
		else{
			TSTree_delete(root,bdata(name),blength(name)); 
		}
	}else if(bstrcmp(list->entry[0],&commandl)==0){
		list_data(root,&i,data);
	}
	else{
		printf("wrong format create read update delete list\n");
	}
	return 0;
	memset(commands,0,100) //well 100 is not right,just a guess
	error:
	return -1;
	memset(commands,0,100);// strlen is bullshit
}
