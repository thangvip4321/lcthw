#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>
static List *list =NULL;
char *test1 = "data1";
char *test2 ="data2";
char *test3 = "data3";
char *test_create(){
	list =List_create();
	mu_assert(list !=NULL,"failed to make a list");
	return NULL;
}
char *test_destroy(){
	List_clear_destroy(list);
	return NULL;
}

char *test_push_pop(){
	List_push(list,test1);
	mu_assert(List_last(list)==test1,"Wrong last_value1");
	List_push(list,test2);
	mu_assert(List_last(list)==test2,"Wrong last_value2");
	List_push(list,test3);
	mu_assert(List_last(list)==test3,"Wrong last_value3");
	mu_assert(List_count(list)==3,"Wrong count on push");
	char *val =List_pop(list);
	mu_assert(val == test3,"Wrong pop value1");
	val = List_pop(list);
	mu_assert(val == test2,"wrong pop value2");
	val =List_pop(list);
	mu_assert(val == test1,"Wrong pop value3");
	mu_assert(List_count(list)==0,"wrong count after pop");
	return NULL;
}
char *test_unshift()
{
	List_unshift(list,test1);
	mu_assert(List_first(list)==test1,"wrong first value");
	List_unshift(list,test2);
	mu_assert(List_first(list)==test2,"wrong first value");
	List_unshift(list,test3);
	mu_assert(List_first(list)==test3,"wrong last value");
	mu_assert(List_count(list)==3,"wrong count on unshift");
	return NULL;
}
char *test_remove(){
	debug("good af");
	Node* node_test = Node_next(List_fn(list));
	char *val =List_remove(list,node_test);
	debug("true_test_remove");
	mu_assert(val ==test2,"Wrong removed element");
	mu_assert(List_first(list)==test3,"wrong first element after rm");
	mu_assert(List_last(list)==test1,"Wrong last element after rm");
	return NULL;
}
char *test_shift()
{
	mu_assert(List_count(list) != 0,"wrong count before shift");
	debug("true test_shift");
	char *val =List_shift(list);
	mu_assert(val ==test3,"Wrong value on shift");
	val =List_shift(list);
	mu_assert(val==test1,"wrong value on shift");
	mu_assert(List_count(list)==0,"wrong count after shift");
	return NULL;
}


char *all_tests(){
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_destroy);
	mu_run_test(test_push_pop);
	mu_run_test(test_unshift);
	mu_run_test(test_remove);
	mu_run_test(test_shift);
	return NULL;
}
RUN_TESTS(all_tests);