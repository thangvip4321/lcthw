#include <lcthw/suffix_tree.h>
#include "minunit.h"
#include <lcthw/bstrlib.h>
Suffix_Node *node = NULL;
struct tagbstring string = bsStatic("abcabxabcd");
char* create_tree_test(){
	node = Suffix_tree_build(&string);
	mu_assert((node != NULL),"hmm ok wrong shit");
	return NULL;
}
char* all_test(){
	mu_suite_start();
	mu_run_test(create_tree_test);
	return NULL;
}
RUN_TESTS(all_test);