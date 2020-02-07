#include "minunit.h"
#include <lcthw/server.h>
#include <lcthw/ringbuffer.h>
#include <sys/socket.h>
#include <lcthw/bstrlib.h>
int sockfd = 0;
int new_fd = 0;
struct sockaddr_storage their_addr;
socklen_t size = sizeof(their_addr);
char* test_echo(){
	sockfd = echo_server("127.0.0.1","1234");
	mu_assert(( sockfd != -1 ),"failed to establish server");
	listen(sockfd,10);
	new_fd = accept(sockfd,(struct sockaddr*) &their_addr,&size);
	debug("new_fd: %d ",new_fd);
	mu_assert((new_fd != -1),"hmm wrong new_fd man");
	return NULL;
}
char* test_read_write(){
	RingBuffer *buffer = RingBuffer_create(20);
	read_some(new_fd,buffer);
	debug("ok after read");
	debug("available data is :%d",RingBuffer_available_data(buffer));
	bstring bs = (RingBuffer_get_all(buffer));
	debug("%s",bdata(bs));
	mu_assert((strcmp(bdata(bs),"XXX\nqwerty\n") == 0),"not read as expected");
	// ok things to do trim the \n first
	//write_some(new_fd,buffer);
	//read_some(new_fd,buffer);
	//bs = (RingBuffer_get_all(buffer));
	//debug("bs: %x",bdata(bs));
	//mu_assert((strcmp(bdata(bs),"qwerty\n") == 0),"not read as expected");
	debug("available data:%d",RingBuffer_available_data(buffer));
	//debug("buffer data %s",bdata(RingBuffer_get_all(buffer)));
	return NULL;
}
char* test_write(){
	return NULL;
}
char* test_close(){
	close(new_fd);
	close(sockfd);
	return NULL;
}
char* all_test(){
	mu_suite_start();
	mu_run_test(test_echo);
	mu_run_test(test_read_write);
	mu_run_test(test_write);
	mu_run_test(test_close);
	return NULL;
}
RUN_TESTS(all_test);