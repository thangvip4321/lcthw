#include <lcthw/proc_request.h>
#include <lcthw/tstree.h>
#include "minunit.h"
#include <lcthw/bstrlib.h>
TSTree* node = NULL;
char* str = NULL;
struct tagbstring com1 = bsStatic("create");
struct tagbstring com2 = bsStatic("read");
struct tagbstring com3 = bsStatic("update");
struct tagbstring com4 = bsStatic("delete");
char* test_create(){
	str = process_request(&node,"create a/b/c");
	mu_assert((node !=NULL),"create node wrongly?");
	str = process_request(&node,"create a/b/d");
	mu_assert((TSTree_pinpoint(node,"a/b/c",5))->stat !=NULL,"create node without stat");
	debug("root is %p",node);
	return NULL;
}
char* test_update(){
	str = process_request(&node,"update a/b/c 1 2 3 4");
	mu_assert((str !="ok"),"should have updated the existing url");
	debug("a/b/c: %d and %f",(TSTree_pinpoint(node,"a/b/c",5)->stat->n),(TSTree_pinpoint(node,"a/b/c",5)->stat->sum));
	mu_assert((process_request(&node,"update clgt 1 2 3 4") =="no such statistic"),"handler error in update");
	mu_assert((TSTree_pinpoint(node,"a/b/c",5)->stat->sum == 10),"should have the sum of 10");
	mu_assert((TSTree_pinpoint(node,"a/b/c",5)->stat->n == 4),"should have the num of 4");
	mu_assert((process_request(&node,"update a/b/d 1 3 5") =="ok\n"),"handler failed to update existing node");
	debug("a/b/: %d and %f",(TSTree_pinpoint(node,"a/b/",4)->stat->n),(TSTree_pinpoint(node,"a/b/",4)->stat->sum));
	mu_assert((TSTree_pinpoint(node,"a/b/",4))->stat->sum == 5.5,"ok handler update node not as expected");
	mu_assert((TSTree_pinpoint(node,"a/b/",4)->stat->n == 2),"should have the num of 2");
	return NULL;
}
char* test_read(){
	str = process_request(&node,"read a/b/c");
	mu_assert((str !="no such statistic"),"ok why cant it read?");
	str = process_request(&node,"read a/b/q");
	mu_assert((str =="no such statistic\n"),"ok why can it read non-existing node now?");
	debug("root is %p",node);

	return NULL;
}
char* test_delete(){
	str = process_request(&node,"delete a/b/f");
	debug("root is %p",node);
	mu_assert((str =="no such statistic\n"),"ok TSTree_pinpoint hasnt done its job well enough ");
	str = process_request(&node,"delete a/b/d");
	mu_assert(process_request(&node,"read a/b/d") == "no such statistic\n","handler forgot to delete the node");
	debug("%f",(TSTree_pinpoint(node,"a/b/",4))->stat->sum);
	mu_assert((TSTree_pinpoint(node,"a/b/",4))->stat->sum == 2.5,"delete forget to re_calculate means");
	mu_assert((TSTree_pinpoint(node,"a/b/",4))->stat->n == 1,"delete forget to re count subnodes");
	str = process_request(&node,"delete a/b/");
	mu_assert(process_request(&node,"read a/b/c") == "no such statistic\n","handler forgot to delete the node");

	return NULL;
}
char* test_list(){
	return NULL;
}
char* all_test(){
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_update);
	mu_run_test(test_read);
	mu_run_test(test_delete);
	return NULL;
}
RUN_TESTS(all_test);