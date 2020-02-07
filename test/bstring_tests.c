#include "minunit.h"
#include <lcthw/bstrlib.h>
#include <string.h>
char* test_string(){
	char* me = "XXX";
	bstring a = bfromcstr("wow");
	mu_assert(strcmp(bdata(a),"wow")==0,"bstring didnt make the string right");
	bstring b = blk2bstr(me,4);
	mu_assert(bchar(b,3)=='\0',"last char not right");
	int rc = bassigncstr(b,"qwerty");
	mu_assert((rc ==BSTR_OK),"assignment doesnt work");
	mu_assert(strcmp(bdata(b),"qwerty")==0,"bassigncstr didnt assign the right data");
	mu_assert(blength(b)==6,"size of data is not changed");
	rc = bassignblk(b,"concac",3);
	mu_assert(blength(b)==3,"size of data is not changed");
	mu_assert(strcmp(bdata(b),"con")==0,"bassignblk used on wrong purpose");
	rc = bconcat(b,a);
	mu_assert((rc ==BSTR_OK),"concatenation doesnt work");
	mu_assert(strcmp(bdata(b),"conwow")==0,"concatenation doesnt work as it's expected to ");
	debug("%s",b->data);
	rc = bdestroy(b);
	mu_assert((rc ==BSTR_OK),"something goes wrong with b");
	bstring c = bfromcstr("wow");
	rc = bstricmp(a,c);
	mu_assert(rc==0,"sth go wrong? in stricmp");
	rc = biseq(a,c);
	debug("%d",rc);
	mu_assert(rc ==1,"biseq goes wrong?");
	bstring d = bfromcstr("dcmwowvaicalon");
	rc = binstr(d,0,c);
	debug("%d",rc);
	struct bstrList *list = bsplit(d,'c');
	debug("%s",list->entry[0]->data);
	debug("%s",list->entry[1]->data);
	debug("%s",list->entry[2]->data);
	debug("%d %d",list->qty,list->mlen);
	mu_assert(strcmp(list->entry[0]->data,"d")==0,"wrong split?");
	mu_assert(strcmp(list->entry[1]->data,"mwowvai")==0,"wrong split?");
	char *cc = "SSS";
	bstring e = bformat("%d %s",123,"SSS");
	debug("%s",e->data);
	mu_assert(strcmp(e->data,"123 SSS")==0,"bformat went wrong");
	return NULL;
}
char* all_tests(){
	mu_suite_start();
	mu_run_test(test_string);
	return NULL;
}
RUN_TESTS(all_tests);