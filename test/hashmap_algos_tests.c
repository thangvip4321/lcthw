#include <lcthw/bstrlib.h>
#include <lcthw/darray.h>
#include <lcthw/hashmap.h>
#include <lcthw/hashmap_algos.h>
#include "minunit.h"
struct tagbstring test1 = bsStatic("test data1");
struct tagbstring test2 = bsStatic("test data2");
struct tagbstring test3 = bsStatic("test data3");
char *test_fnv1a(){
	uint32_t hash = Hashmap_fnv1a_hash(&test1);
	mu_assert(hash !=0,"bad hash");
	hash = Hashmap_fnv1a_hash(&test2);
	mu_assert(hash !=0,"bad hash");
	hash = Hashmap_fnv1a_hash(&test3);
	mu_assert(hash !=0,"bad hash");
	return NULL;
}
char* test_adler32(){
	uint32_t hash =Hashmap_adler32_hash(&test1);
	mu_assert(hash !=0,"bad hash");
	hash = Hashmap_adler32_hash(&test2);
	mu_assert(hash !=0,"bad hash");
	hash = Hashmap_adler32_hash(&test3);
	mu_assert(hash !=0,"bad hash");
	return NULL;
}
char* test_djb(){
	uint32_t hash =Hashmap_djb_hash(&test1);
	mu_assert(hash !=0,"bad hash");
	hash =Hashmap_djb_hash(&test2);
	mu_assert(hash !=0,"bad hash");
	hash =Hashmap_djb_hash(&test3);
	mu_assert(hash !=0,"bad hash");
	return NULL;
}
#define BUCKET 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKET*1000
#define ALGOS_NUM 5
enum {ALGO_FNV1A,ALGO_ADLER32,ALGO_DJB,JENKIN,MINE};
int gen_keys(DArray *keys,int num_key){
	FILE* urand = fopen("/dev/urandom","r");
	check(urand !=NULL,"failed to open /dev/urandom");
	struct bStream *stream = bsopen((bNread) fread,urand);
	check(stream !=NULL,"failed to open /dev/urandom");
	bstring key = bfromcstr("");
	int rc =0;
	for(int i =0;i<num_key;i++){
		rc = bsread(key,stream,BUFFER_LEN);
		check((rc >=0),"failed to read from rand");
		DArray_push(keys,bstrcpy(key));
	}
	bsclose(stream);
	fclose(urand);
	return 0;
	error:
	return -1;
}
void destroy_keys(DArray *keys){
	int i =0;
	for(i=0;i<NUM_KEYS;i++){
		bdestroy(DArray_get(keys,i));
	}
	DArray_destroy(keys);
}
void fill_distribution(int array[BUCKET],DArray *keys,Hashmap_hash al){
	int bucket_n =0;
	int i =0;
	for(i =0;i<keys->end;i++){
		uint32_t hash = al(DArray_get(keys,i));
		bucket_n = hash % BUCKET;
		array[bucket_n] += 1;
	}
}
char* test_distribution(){
	int i =0;
	FILE *hash_data = fopen("hash.txt","w");
	int stats[ALGOS_NUM][BUCKET] = {{0}};
	DArray *keys = DArray_create(0,NUM_KEYS);
	int rc = gen_keys(keys,NUM_KEYS);
	mu_assert(rc ==0,"error generating an array");
	fill_distribution(stats[ALGO_FNV1A],keys,Hashmap_fnv1a_hash);
	fill_distribution(stats[ALGO_ADLER32],keys,Hashmap_adler32_hash);
	fill_distribution(stats[ALGO_DJB],keys,Hashmap_djb_hash);
	fill_distribution(stats[JENKIN],keys,default_hash);
	fill_distribution(stats[MINE],keys,my_hash);
	fprintf(hash_data, "FNV1A\tADL32\tDJB\tJENKIN\tMINE\n");
	for(i =0;i<BUCKET;i++){
		fprintf(hash_data, "%d\t %d\t %d\t %d\t %d\t\n",stats[ALGO_FNV1A][i],stats[ALGO_ADLER32][i],stats[ALGO_DJB][i],stats[JENKIN][i],stats[MINE][i]);
	}
	destroy_keys(keys);
	fclose(hash_data);
	return NULL;
}

char *all_tests(){
	mu_suite_start();
	mu_run_test(test_fnv1a);
	mu_run_test(test_adler32);
	mu_run_test(test_djb);
	mu_run_test(test_distribution);
	return NULL;
}
RUN_TESTS(all_tests);
