#include <stdio.h>
#include <lcthw/bstrlib.h>
#include <regex.h>
#include <lcthw/stats.h>
#include <lcthw/tstree.h>
#include <stdlib.h>
#include <lcthw/dbg.h>
#include <list.h>
#include <mysql.h>
int position = 0;
struct tagbstring commandl = bsStatic("list");
struct tagbstring commandc = bsStatic("create");
struct tagbstring commandr = bsStatic("read");
struct tagbstring commandu = bsStatic("update");
struct tagbstring commandd = bsStatic("delete");
struct tagbstring commands = bsStatic("save");
struct tagbstring commandlo = bsStatic("load");
struct bstrList* process_string(char* string){
	bstring mid = bfromcstr(string);
	btrimws(mid);
	struct bstrList* list = bsplit(mid,' ');
	//for(int i = 0;list->entry[i] !=NULL;i++){
		//debug("%p",bdata(list->entry[i]));
		//debug("%s ,%d",bdata(list->entry[i]),blength(list->entry[i])); // wtf is wrong with this shit?????????
//}
return list;
}
typedef struct stats_packet{
	bstring url;
	Stats* stats;
	char changed;
}stats_packet;

TSTree* create_request(TSTree *root, bstring url){
	Stats* stat0 = Stats_create();
	stats_packet* pack = malloc(sizeof(stats_packet));
	pack->stats = stat0;
	debug("%p",pack->stats);
	pack->url = bstrcpy(url);
	root = TSTree_delim_insert(root,bdata(url),blength(url),pack); //sound very unsure about this, should have put **root
	return root;
}
char* read_request(TSTree *root,bstring url){
	//use to read the request given root and url
	//arg: TSTree* root, bstring url
	debug("still ok here?");
	stats_packet* pack =TSTree_search(root,bdata(url),blength(url)); //maybe try approximate first
	debug("after this?");
	check((pack !=NULL)," error: statistic doesn't exist");
	char* dump = malloc(150);
	Stats* stat0 =pack->stats;
	debug("this? : %p, %p",pack,pack->stats);
	sprintf(dump,"sum : %f, sumsq %f, n: %ld,min:%f, max: %f,mean: %f, stddev: %f \n",stat0->sum,stat0->sumsq,stat0->n,stat0->min,stat0->max,Stats_mean(stat0),Stats_stddev(stat0));
	return dump;
	error:
	return "no such statistic\n";
}
char* update_request(TSTree* root,bstring url,struct bstrList *list){
	List* delim_list = List_create();
	//stats_packet* pack = TSTree_delim_search(root,delim_list,bdata(url),blength(url)); 
	stats_packet* pack = TSTree_delim_search(root,delim_list,bdata(url),blength(url)); 
	debug("length of list is %d",List_count(delim_list));
	TSTree* node1 = TSTree_pinpoint(root,bdata(url),blength(url));
	if(node1 == NULL ){
		return "no such statistic";
	}
	Stats* temp_stat = malloc(sizeof(Stats));
	*(temp_stat) = *(node1->stat);
	for(int i=2;list->entry[i] !=NULL;i++){
		Stats_sample(temp_stat,atof(bdata(list->entry[i])));
	}
	int me = update_tracer(delim_list,temp_stat);
	debug("a/b/: %f",(TSTree_pinpoint(root,"a/b/",4)->stat->sum));
	List_destroy(list);
	if(me != 0){
		return "error updating\n";
	}
	/*for(int i=0;List_count(delim_list) >1;i++){
		TSTree* node = List_pop(delim_list);
		TSTree* prev = List_last(delim_list);
		Stats_sample(prev->stat,node->stat->sum / node->stat->n);
		prev->stat->n -= 1;
	}*/
	return "ok\n";
}
char* delete_request(TSTree* root,bstring url){
	return (TSTree_delete_all_subnodes(root,bdata(url),blength(url))==0) ? "deleted\n" : "no such statistic\n";

	/*debug("%p, %p",pack,pack->stats);
	free(pack->stats);
	pack->stats = NULL;
	free(pack->url);
	pack->url = NULL;
	free(pack);
	pack = NULL; */  
	// very bad code here, can't just free pointer and leave it dangling there, might be attacked.
}
void* add_string(stats_packet* pack,char** list){
	bstring data = pack->url;
	char* string = bdata( data);
	debug("ok here: %p",*list);
	sprintf(*list,"%s\n",string);
	debug("%s",*list);
	*list += blength(data)+1;
}
char* list_request(TSTree* root){
	debug("ok after list_triggered");
	char* list = malloc(1024*1024);
	debug("list in root is: %p",list);
	char* final_list= list;
	int i = 0;
	TSTree_traverse(root,add_string,&list);
	return final_list;
}
char* save_request(TSTree* root,DArray* list){
	// save only modified nodes to sql database to save execution time
	return NULL;
}
char* load_request(TSTree** root){

}
char* process_request(TSTree **root,char* commands){
	debug("a/b/c address: %p",(TSTree_pinpoint(*root,"a/b/c",5)));
	if(commands ==NULL)
		return "no argument sent";
	bstring name = NULL;
	stats_packet *pac =malloc(sizeof(stats_packet));
	int i = 0;
	debug("command is %s",commands);
	char* data = malloc(100);
	struct bstrList *list =process_string(commands);
	debug("data is : %s and name is %s",bdata(list->entry[0]),bdata(list->entry[1]));
	if(bstrcmp(list->entry[0],&commandc) ==0){
		*root = create_request(*root, list->entry[1]);
		return "create stats successfully\n";
	}
	if(bstrcmp(list->entry[0],&commandr) == 0){
		debug("still ok?");
		char* rs =read_request(*root,list->entry[1]); //problem here?
		if (rs ==NULL)
			return "error with read_request\n";
		else
			return rs;
	}
	if(bstrcmp(list->entry[0],&commandu) ==0){
		//char* wow= update_request(*root,list->entry[1],list);
		//debug("a/b/c: %f",(TSTree_pinpoint(*root,"a/b/c",5)->stat->sum));
		return update_request(*root,list->entry[1],list);
	}
	if(bstrcmp(list->entry[0],&commandd) ==0){
		return delete_request(*root,list->entry[1]);
	}
	if(bstrcmp(list->entry[0],&commandl) ==0){
		debug("before request?");
		return list_request(*root);
	}
	else
		return "wrong format\n";
}
#if 0
char* process_request(TSTree **root,char* commands){// considering bstring commands
	check((commands !=NULL),"no data sent");
	bstring name = NULL;
	Stats *stat0 = NULL;
	stats_packet *pac = malloc(sizeof(stats_packet));
	int i = 0;
	debug("root is %p",*root);
	debug("%s",commands);
	char* data = malloc(100);
	struct bstrList *list = process_string(commands); // process string into commands and url
	debug("data is : %s and name is %s",bdata(list->entry[0]),bdata(list->entry[1]));	
	//create
	if(bstrcmp(list->entry[0],&commandc) == 0){
	name = list->entry[1];
	check((name !=NULL),"USAGE: create <stats_name>");
	stat0 = Stats_create();
	pac->stats = stat0;
	pac->url = bdata(name);
	*root = TSTree_insert(*root,bdata(name),blength(name),pac);
	return "ok";
	}
	//read
	else if(bstrcmp(list->entry[0],&commandr) == 0){
	name = list->entry[1];
	printf("names: %s",bdata(name));
	check((name !=NULL),"USAGE: read <stats_name>");
	pac = TSTree_search(*root,bdata(name),blength(name));
	if(pac == NULL){
		return "no such statistic";
	}
	else{
		char* dump = malloc(150);
		stat0 = pack->stats;
		sprintf(dump,"sum : %f, sumsq %f, n: %ld,min:%f, max: %f,mean: %f, stddev: %f \n",stat0->sum,stat0->sumsq,stat0->n,stat0->min,stat0->max,Stats_mean(stat0),Stats_stddev(stat0));
		return dump;
	}
	}
	//update
	else if(bstrcmp(list->entry[0],&commandu) ==0){
	name = list->entry[1];
	printf("names: %s",bdata(name));
	check((name !=NULL),"USAGE: update <stats_name> <numbers>");
	stat0 = TSTree_search(*root,bdata(name),blength(name));
	if(stat0 ==NULL){
		return "no such stat";
	}else{
	for(int i =2;list->entry[i] !=NULL;i++){
			Stats_sample(stat0,atof(bdata(list->entry[i]))); // ok so string data will get processed as 0.0 which we dont want
		}
		return "ok";
	}
	//delete
	}else if(bstrcmp(list->entry[0],&commandd) ==0){
		name = list->entry[1];
		check((name !=NULL),"USAGE: delete <stats_name>");
		TSTree_delete(*root,bdata(name),blength(name)); 
		return "ok";
	}else if(bstrcmp(list->entry[0],&commandl)==0){
		//list_data(*root,&i,data);
		return data;
	}
	else{
		return "wrong format create read update delete list\n";
	}
	error:
	debug("wtf wrong");
	return "uh oh";
}
#endif 
/*void* client_process(void* arg){
	int byte_send = 0;
	RingBuffer *buffer = RingBuffer_create(BUFFER_SIZE);
	sock_pack * packet = arg;
	struct sockaddr_storage addr_storage =  packet->addrs;
	int sockfd = packet->sockfd;
	size_t* addr_size = packet->storage_size; 
	//TSTree *root_thread = packet->data;
	int new_sock = accept(sockfd,(struct sockaddr*) &addr_storage,&addr_size);
	char* respond = malloc(1024*1024);
	if(new_sock == -1){
		perror("cant find sockfd");
		exit(-1);
	}
	//int error_code;
	//int error_code_size = sizeof(error_code);
	//if (setsockopt(new_sock,SOL_SOCKET,MSG_NOSIGNAL,&optval,sizeof(optval)) !=0 ){ //almost work sadly no SO_NOSIGPIPE in linux (weird) 
	//	perror(" error with setsockopt");
	//	exit(-1);
	//}
	while(errno != EPIPE){ // hmmm what if we create a error not EPIPE in this while shit :v 
				debug("start reading");
				read_some(new_sock,buffer); // ok problem with recv blocking? so we can't check the condition if the file exist.
				bstring data = RingBuffer_get_all(buffer);
				pthread_mutex_lock(&lock);
				respond = process_request((TSTree**) packet->data,bdata(data)); // why rs span a lot?????????  hmm not quite efficient? change process request?
				//add an option quit (q) to exit safely
				pthread_mutex_unlock(&lock);
				//check((rs != NULL),"dit con me loi r em oi"); error: server exit after clients
				debug("ok after read_some, maybe the problem in send");
				byte_send = send(new_sock,respond,strlen(respond),MSG_NOSIGNAL);
				//debug("status: %d",getsockopt(new_sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size));
				//debug("wtf error code :%d",error_code);
			}
		close(new_sock);
		pthread_exit(NULL);
	}
	*/ 