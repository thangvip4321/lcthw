#include "minunit.h"
#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/bstrlib.h>
#include <time.h>
#define TEST_TIME 1
RingBuffer *buffer = NULL;
char* me = "0123456789";

char* test_create(){
	buffer = RingBuffer_create(10);
	mu_assert((buffer !=NULL),"cannot create buffer");
	return NULL;
}
char* test_destroy(){
	RingBuffer_destroy(buffer);
	return NULL;
}
char* test_read_write(){
	debug("start end %d %d",buffer->start,buffer->end);
	int length = RingBuffer_write(buffer,me,10);
	debug("start1 end %d %d",buffer->start,buffer->end);
	mu_assert(length ==10,"hmm have not written full data");
	char* arr = malloc(10); 
	length = RingBuffer_read(buffer,arr,5);
	debug("start2 end %d %d",buffer->start,buffer->end);
	mu_assert((length ==5),"have not read enough num of data requested");
	mu_assert(strcmp(arr,"01234")==0,"hmm havve not read the right data");
	length = RingBuffer_read(buffer,arr,3);
	debug("start3 end %d %d",buffer->start,buffer->end);
	mu_assert(length ==3,"have not read enough num of data requested");
	length = RingBuffer_write(buffer,me,5);
	mu_assert(length ==5,"have not read enough num of data requested");
	length = RingBuffer_read(buffer,arr,4);
	debug("%s",arr);
	mu_assert((length ==4),"have not read enough num of data requested");


	return NULL;
}
char* test_create1(){
	buffer = RB_init(4096);
	mu_assert((buffer !=NULL),"cannot create buffer");
	return NULL;
}
char* test_destroy1(){
	RingBuffer_destroy1(buffer);
	return NULL;
}
char* test_read_write1(){
	debug("start end %d %d",buffer->start,buffer->end);
	int length = RingBuffer_write1(buffer,me,10);
	debug("start1 end %d %d",buffer->start,buffer->end);
	mu_assert(length ==10,"hmm have not written full data");
	char* arr = malloc(10); 
	length = RingBuffer_read1(buffer,arr,5);
	debug("start2 end %d %d",buffer->start,buffer->end);
	mu_assert((length ==5),"have not read enough num of data requested");
	mu_assert(strcmp(arr,"01234")==0,"hmm havve not read the right data");
	length = RingBuffer_read1(buffer,arr,3);
	debug("start3 end %d %d",buffer->start,buffer->end);
	mu_assert(length ==3,"have not read enough num of data requested");
	length = RingBuffer_write1(buffer,me,5);
	mu_assert(length ==5,"have not read enough num of data requested");
	length = RingBuffer_read1(buffer,arr,4);
	debug("%s",arr);
	mu_assert((length ==4),"have not read enough num of data requested");


	return NULL;
}
char* test_get_all(){
	//bstring xxx = RingBuffer_get_all(buffer); // ok somehow bdata is not compatible to RingBuffer_get_all
	//mu_assert((strcmp(xxx,"234")==0),"just a wait signal");
	return NULL;
}
char* speed_test(){
	RingBuffer *b0 = RingBuffer_create(4096*2);
	RingBuffer *b1 = RB_init(4096*2);
	FILE *fp; 
	fp = fopen("/dev/urandom","r");
	char *data = calloc(2000,1);
	fgets(data,2000,fp);
	char *arr = calloc(2000,1);
	time_t elapsed = 0;
	int count = 0;
	time_t fast_start = time(NULL);
	do{for(int i = 0 ;i <1000;i++){
		RingBuffer_write1(b1,data,2000);
		RingBuffer_read1(b1,arr,1997);
		++count;
	}
		b1->start = b1->end = 0;
		elapsed =time(NULL)-fast_start;
	}while(elapsed < TEST_TIME);
	printf(" fast :%lu ,time %d ,OPS: %f \n ",count,elapsed,(double)count/elapsed);
	count = 0;
	elapsed = 0;
	char* arr1 = calloc(2000,1);
	time_t slow_start = time(NULL);
	do{
		for(int i = 0 ;i <1000;i++){
		RingBuffer_write(b0,data,2000);
		RingBuffer_read(b0,arr1,1997); // wtf how is this one faster????????????????????????????????????????????????????
		++count;
		}
		b0->start = b0->end = 0;
		b0->looped = 0;
		elapsed = time(NULL) - slow_start;
	}while(elapsed < TEST_TIME);
	printf("slow: %lu ,time %d ,OPS: %f \n",count,elapsed,(double)count/elapsed);
	fclose(fp);
	return NULL;
}
char* all_tests(){
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_read_write);
	mu_run_test(test_get_all);
	mu_run_test(test_destroy);
	mu_run_test(test_create1);
	mu_run_test(test_read_write1);
	mu_run_test(test_destroy1);
	#if 0
	mu_run_test(speed_test);
	#endif
	return NULL;
}
RUN_TESTS(all_tests);
