#include <lcthw/darray_algos.h>
#include "minunit.h"
int testcmp(char *a,char *b){
	return strcmp(a,b);
}
DArray *create_words(){
	DArray *result = DArray_create(0,5);
	char *words[] ={"qwe","xasd","123","Qerf","XXX","Casd"};
	int i =0;
	for(i=0; i<5;i++){
		DArray_push(result,words[i]);
	}
	return result;
}
int is_sorted(DArray *array){
	int i =0;
	for(i=0;i<DArray_count(array)-1;i++){
		
		if(strcmp(DArray_get(array,i),DArray_get(array,i+1))>0){
			debug("unsorted numbers: %s %s: %d ",DArray_get(array,i),DArray_get(array,i+1),i);
			return 0;
		}
	}
	return 1;
}
char* run_sort_test(int (*func)(DArray *,compare_DA ),const char* name){
	DArray *words = create_words();
	mu_assert(!is_sorted(words),"words should start not sorted ");
	debug("------------------testing sorting algo: %s",name);
	int rc = func(words,(compare_DA) strcmp);
	mu_assert(rc !=0,"sorting failed");
	mu_assert(is_sorted(words),"words are not sorted");
	DArray_destroy(words);
	return NULL;
}

char *test_qsort(){
	return run_sort_test(DArray_qsort,"qsort");
}
char *test_heapsort(){
	return run_sort_test(DArray_heapsort,"heapsort");
}
char *test_mergesort(){
	return run_sort_test(DArray_mergesort,"mergesort");
}
char *test_define(){
	DArray *words =create_words();
	mu_assert(!is_sorted(words),"words should start not sorted");
	debug("--------------------------test all");
	int rc = DArray_sort(words,strcmp,DArray_qsort);
	mu_assert(rc != 0,"sorting failed");
	mu_assert(is_sorted(words),"words are not sorted");
	DArray_destroy(words);
	return NULL;
}

char *all_tests(){
	mu_suite_start();
	mu_run_test(test_qsort);
	mu_run_test(test_heapsort);
	mu_run_test(test_mergesort);
	mu_run_test(test_define);
	return NULL;
}
RUN_TESTS(all_tests);