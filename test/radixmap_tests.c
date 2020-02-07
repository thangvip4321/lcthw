#include "minunit.h"
#include <lcthw/radixmap.h>
#include <time.h>
static int make_random(RadixMap *map){
	size_t i = 0;
	for(i=0;i<map->max;i++){
		uint32_t key = (uint32_t) (rand()|(rand()<<16));
		check(RadixMap_add(map,key,i)==0,"Failed to add key %u.",key);
	}
	return i;
	error:
	return 0;
}
static int check_order(RadixMap *map){
	RMElement d1,d2;
	unsigned int i = 0;

	for(i=0;map->end>0 && i <map->end-1;i++){
		d1 = map->contents[i];
		d2 = map->contents[i+1];
		if(d1.data.key >d2.data.key){
			debug("failed: i = %u ,key :%u,value:%u,equal max? %d",i,d1.data.key,d1.data.value,d2.data.key ==UINT32_MAX);
			return 0;
		}
	}
	return 1;
}
static int test_search(RadixMap *map){
	unsigned i = 0;
	RMElement *d = NULL;
	RMElement *found = NULL;
	for(i=map->end/2;i<map->end;i++){
		d = &map->contents[i];
		found = RadixMap_find(map,d->data.key);
		check(found != NULL,"Didnt find %u at %u",d->data.key,i);
		check(found->data.key == d->data.key,"got the wrong result :%p :%u looking for %u at %u",found,found->data.key,d->data.key,i);
	} 
	return 1;
	error:
	return 0;
}
static char *test_operations(){
	size_t N = 10;
	RadixMap *map =RadixMap_create(N);
	mu_assert(map !=NULL,"Failed to make the map");
	debug("nothing goes wrong");
	clock_t time1 =clock();
	mu_assert(make_random(map),"Didnt make a random fake radix map.");
	clock_t time2 = clock();
	double cpu_time_used;
	cpu_time_used =((double)(time2-time1));
	debug("%d ccc",cpu_time_used);
	mu_assert(check_order(map),"failed to sort radixmap");
	RadixMap_sort(map,0);
	mu_assert(check_order(map),"failed to sort radixmap");
	mu_assert(test_search(map),"failed the search test");
	mu_assert(check_order(map),"radixmap didnt stay sorted after search");
	while(map->end >0){
		RMElement *el =RadixMap_find(map,map->contents[map->end/2].data.key);
		mu_assert(el !=NULL,"should get a result");
		size_t old_end = map->end;
		mu_assert(RadixMap_delete(map,el)==0,"Didnt delete it");
		mu_assert(old_end-1 == map->end,"Wrong size after delete");
		mu_assert(check_order(map),"Radixsort should stay sorted after delete");
	}
		RadixMap_destroy(map);
		return NULL;
}
char *all_tests(){
	mu_suite_start();
	srand(time(NULL));
	mu_run_test(test_operations);
	return NULL;
}
RUN_TESTS(all_tests);