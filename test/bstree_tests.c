#include <lcthw/bstree.h>
#include "minunit.h"
#include <assert.h>
#include <lcthw/bstrlib.h>
#include <stdlib.h>
#include <time.h>
BSTree *map =NULL;
AVLTree *map1 = NULL;
static int  traverse_called = 0;
struct tagbstring test1 =bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");
struct tagbstring expect1 = bsStatic("value1");
struct tagbstring expect2 = bsStatic("value2");
struct tagbstring expect3 = bsStatic("value3");
static int traverse_good_cb(BSTreeNode *node){
	debug("Key: %s",bdata((bstring)node->key));
	traverse_called++;
	return 0;
}
static int traverse_good_cb1(AVLNode *node){
	debug("Key : %s",bdata((bstring)node->key));
	if(node->parent){
		if(node == node->parent->left)
		debug("left");
		else
			debug("right");
	}

	traverse_called++;
	return 0;
}
static int traverse_fail_cb(BSTreeNode *node){
	debug("Key: %s ",bdata( (bstring) node->key));
	traverse_called++;
	if(traverse_called == 2){
		return 1;
	}else{
		return 0;
	}
}
char* test_create(){
	map = BSTree_create(NULL);
	map1 = AVLTree_create(NULL);
	mu_assert((map !=NULL),"failed to create map");
	mu_assert((map1 !=NULL),"failed to create map1");
	return NULL;
}
char* test_destroy(){
	BSTree_destroy(map);
	return NULL;
}
char *test_get_set(){
	int rc = BSTree_set(map,&test1,&expect1);
	mu_assert((rc == 0),"set failed on key1");
	bstring result = BSTree_get(map,&test1);
	mu_assert((result ==&expect1),"cannot get key");
	rc = BSTree_set(map,&test2,&expect2);
	mu_assert((rc == 0),"failed to set key2");
	result = BSTree_get(map,&test2);
	mu_assert((result == &expect2),"cannot get data2");
	rc = BSTree_set(map,&test3,&expect3);
	mu_assert((rc == 0),"failed to set key3");
	result = BSTree_get(map,&test3);
	mu_assert((result == &expect3),"failed to get data3");
	rc = AVLTree_set(map1,&test1,&expect1);
	mu_assert((rc == 0),"set failed on key1");
	result = AVLTree_get(map1,&test1);
	mu_assert((result ==&expect1),"cannot get key");
	rc = AVLTree_set(map1,&test2,&expect2);
	mu_assert((rc == 0),"failed to set key2");
	result = AVLTree_get(map1,&test2);
	mu_assert((result == &expect2),"cannot get data2 xd");
	rc = AVLTree_set(map1,&test3,&expect3);
	mu_assert((rc == 0),"failed to set key3");
	result = AVLTree_get(map1,&test3);
	mu_assert((result == &expect3),"failed to get data3");
	return NULL;
}
char *test_traverse(){
	int rc = BSTree_traverse(map,traverse_good_cb);
	mu_assert(rc == 0,"failed to traverse");
	mu_assert(traverse_called == 3,"Wrong count traverse");
	traverse_called = 0;
	rc = BSTree_traverse(map,traverse_fail_cb);
	mu_assert(rc ==1,"failed to traverse");
	mu_assert(traverse_called == 2,"wrong count traverse");
	return NULL;
}
char* test_delete(){
	bstring deleted = (bstring) BSTree_delete(map,&test1);
	mu_assert(deleted !=NULL,"got null on delete");
	mu_assert(deleted == &expect1,"should get expect1");
	bstring result = BSTree_get(map,&test1);
	mu_assert(result ==NULL,"should have deleted");
	deleted = (bstring) BSTree_delete(map,&test1);
	mu_assert(deleted == NULL,"cant delete what's deleted");
	deleted = (bstring) BSTree_delete(map,&test2);
	mu_assert(deleted == &expect2,"should get expect2");
	result = BSTree_get(map,&test2);
	mu_assert(result == NULL,"should have deleted");
	deleted = (bstring) BSTree_delete(map,&test3);
	mu_assert(deleted == &expect3,"should get expect3");
	result = BSTree_get(map,&test3);
	mu_assert(result == NULL,"should have deleted");
	deleted = (bstring) AVLTree_delete(map1,&test1);
	mu_assert(deleted !=NULL,"got null on delete wow man");
	mu_assert(deleted == &expect1,"should get expect1");
	result = AVLTree_get(map1,&test1);
	mu_assert(result ==NULL,"should have deleted");
	deleted = (bstring) AVLTree_delete(map1,&test1);
	mu_assert(deleted == NULL,"cant delete what's deleted");
	deleted = (bstring) AVLTree_delete(map1,&test2);
	mu_assert(deleted == &expect2,"should get expect2");
	result = AVLTree_get(map1,&test2);
	mu_assert(result == NULL,"should have deleted");
	deleted = (bstring) AVLTree_delete(map1,&test3);
	mu_assert(deleted == &expect3,"should get expect3");
	result = AVLTree_get(map1,&test3);
	mu_assert(result == NULL,"should have deleted");
}

char* test_fuzzing(){
	traverse_called = 0;
	BSTree *store =BSTree_create(NULL);
	AVLTree *store1 = AVLTree_create(NULL);
	int i =0;
	int j = 0;
	bstring numbers[100] = {NULL};
	bstring data[100] = {NULL};
	srand((unsigned int)time(NULL));
	for(i=0;i<100;i++){
		int num = rand();
		numbers[i] = bformat("%d",num);
		data[i] = bformat("data %d",num);
		BSTree_set(store,numbers[i],data[i]);
		AVLTree_set(store1,numbers[i],data[i]);
	}
	BSTree_traverse(store,traverse_good_cb);
	debug("%d",traverse_called);
	traverse_called = 0;
	AVLTree_traverse(store1, traverse_good_cb1);
	debug("%d",traverse_called);
	for(i=0;i<100;i++){
		bstring wow  =AVLTree_get(store1,numbers[i]);
		mu_assert((wow != NULL),"failed to get the right numbers");
	}
	for(i=0;i<100;i++){
		bstring value =BSTree_delete(store,numbers[i]);
		bstring value1 = AVLTree_delete(store1,numbers[i]);
		mu_assert(value == 	data[i],"failed to delete the right numbers");
		mu_assert(value1 == data[i],"failed to delete the right numbers");
		mu_assert(BSTree_delete(store,numbers[i])==0,"should get nothing");
		mu_assert(AVLTree_delete(store1,numbers[i])==0,"should get nothing");
		for(j=i+1;j<99-i;j++){
			bstring value = BSTree_get(store,numbers[j]);
			bstring value1 = AVLTree_get(store1,numbers[j]);
			mu_assert(value ==data[j],"failed to get the right number");
			mu_assert(value1 == data[j],"failed to get the right number");
		}
		bdestroy(value);
		bdestroy(numbers[i]);
	}
	BSTree_destroy(store);
	return NULL;
}
char* all_tests(){
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_get_set);
	mu_run_test(test_traverse);
	mu_run_test(test_delete);
	mu_run_test(test_destroy);
	mu_run_test(test_fuzzing);
	return NULL;
}
RUN_TESTS(all_tests);