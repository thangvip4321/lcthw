#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <lcthw/list.h>
#include <time.h>
char *values[] = {"12ewds","XXX","CON","CAC","LOZ","im","gay","nope","shit","never","q","we","e"};
#define NUM_VALUES 5
List *create_words(){
	int i =0;
	List *words =List_create();
	for(i=0;i<NUM_VALUES;i++){
		List_push(words,values[i]);
	}
	return words;
}
int is_sorted(List *words){
	LIST_FOREACH(words,first,words->last,next,cur){
		if(cur->next){
			if(strcmp(cur->value,cur->next->value)>0){
			debug("%s %s \n",(char*) cur->value,(char*) cur->next->value);
			return 0;
		}
		}
	}
	return 1;
}
char* test_bubble_sort(){
	List *words = create_words();
	int rc = List_bubble_sort(words,(List_compare) strcmp);
	mu_assert(rc==0,"bubble sort failed");
	mu_assert(is_sorted(words)==1,"Words are not sorted after bubble sort");
	rc =List_bubble_sort(words,(List_compare) strcmp);
	mu_assert(rc==0,"Sorted list sorting failed");
	mu_assert(is_sorted(words),"word should be sorted if  already bubble sorted");
	List_destroy(words);
	words = List_create();
	words->count =0;
	rc = List_bubble_sort(words,(List_compare) strcmp);
	mu_assert(rc==0,"Bubble sort failed on empty list");
	mu_assert(is_sorted(words),"word should be sorted if empty");
	List_destroy(words);
	return NULL;
}
char* test_merge_sort(){
	List* words = create_words();
	List* res = List_merge_sort(words,(List_compare) strcmp);
	mu_assert(is_sorted(res)==1,"words should be sorted after merger_sort");
	List *res2 = List_merge_sort(res,(List_compare) strcmp);
	mu_assert(is_sorted(res)==1,"words should still be sorted");
	List_destroy(words);
	words = List_create();
	List* res3 = List_merge_sort(words,(List_compare) strcmp);
	mu_assert(is_sorted(res)==1,"list should be sorted if empty");
	List_destroy(words);
	return NULL;
}
char* test_speed(){
	clock_t time1 = clock();
		for(int i =0;i<1000;i++){
		List* words =create_words();
		List* res =List_merge_sort(words,(List_compare) strcmp);
		List_destroy(words);
		List_destroy(res);
	}
	clock_t time2=clock();
	for(int i=0;i<1000;i++){
		List* words =create_words();
		int rc = List_bubble_sort(words,(List_compare) strcmp);
		List_destroy(words);
	}
	clock_t time4= clock();
	debug("time1: %d vs time2: %d",(time2-time1),(time4-time2));
	return NULL;
}
char* test_insert(){
	List *words = List_create();
	for(int i =0; values[i] !=NULL;i++){
		int rc = List_insert_sorted(words,(List_compare) strcmp,values[i]);
		mu_assert(rc == 0,"insertion went wrong");
	}
	mu_assert(is_sorted(words)==1,"list should be sorted");
	List_destroy(words);
	return NULL;
}
char* all_tests(){
	mu_suite_start();
	mu_run_test(test_bubble_sort);
	mu_run_test(test_merge_sort);
	mu_run_test(test_speed);
	mu_run_test(test_insert);
	return NULL;
}
RUN_TESTS(all_tests);