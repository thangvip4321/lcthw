#include "minunit.h"
#include <lcthw/hashmap.h>
#include <time.h>
#include <lcthw/bstrlib.h>
#define WORD_LEN 10
int gen_keys(DArray *array,int num){
	int i =0;
	FILE *urand = fopen("/dev/urandom","r");
	check(urand !=NULL,"Failed to open /dev/urandom");
	struct bStream *stream 	=bsopen((bNread) fread,urand);
	check(stream !=NULL,"failed to use bStream");
	bstring key =bfromcstr("");
	int rc = 0;
	for(i=0;i<num;i++){
		rc = bsread(key,stream,WORD_LEN);
		check((rc>=0),"failed to read");
		DArray_push(array,bstrcpy(key));
	}
	bsclose(stream);
	fclose(urand);
	return 0;
	error:
	return -1;
}

int main(int argc,char *argv[]){

	DArray *keys = DArray_create(0,20);
	int rc = gen_keys(keys,keys->max);
	Hashmap *map = Hashmap_create(NULL,NULL);
	clock_t clock1 =clock();
	debug("okkkkkkkks");
	for (int i = 0;i<keys->end;i++){
	Hashmap_set(map,keys->contents[i],5);
}
	clock_t clock2 = clock();
	debug("okkkkkkkk");
	for(int i =0;i<keys->end;i++){
		bstring* data = Hashmap_get(map,keys->contents[i]);
}
	clock_t clock3 = clock();
	debug("time1: %f 2:%f",(float) clock2-clock1,(float)clock3-clock2);
}