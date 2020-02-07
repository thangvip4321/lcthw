#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>
#include <sys/mman.h>
#include <sys/syscall.h>
int memfd_create(const char* name,unsigned int flags){
	return syscall(__NR_memfd_create,name,flags);
}

RingBuffer *RingBuffer_create(int length){
	RingBuffer *buffer = calloc(1,sizeof(RingBuffer));
	buffer->length = length ;
	buffer->start = 0;
	buffer->end = 0;
	buffer->buffer = calloc(buffer->length,1);
	buffer->looped = 0;
	return buffer;
}
RingBuffer* RB_init(int size){
	RingBuffer *buffer = calloc(1,sizeof(RingBuffer));
	check((size % getpagesize() ==0),"size has to be a multiple of pagesize ");
	buffer->fd = memfd_create("queue buffer",0); // return a fd for a temporary file in memory
	ftruncate(buffer->fd,size);
	buffer->buffer = mmap(NULL,size*2,PROT_NONE,MAP_PRIVATE | MAP_ANONYMOUS,-1,0);
	mmap(buffer->buffer,size,PROT_READ | PROT_WRITE ,MAP_SHARED | MAP_FIXED,buffer->fd,0);
	mmap(buffer->buffer+size,size,PROT_READ | PROT_WRITE ,MAP_SHARED | MAP_FIXED,buffer->fd,0);
	buffer->start = 0;
	buffer->end = 0;
	buffer->looped = 0;
	buffer->length = size;
	return buffer;
	error:
	return NULL;
}
void RingBuffer_destroy(RingBuffer *buffer){
	if(buffer){
		free(buffer->buffer);
		free(buffer);
	}
}
void RingBuffer_destroy1(RingBuffer *buffer){
	if(buffer){
		munmap(buffer->buffer,2*buffer->length);
		free(buffer);
	}
}
int RingBuffer_write(RingBuffer *buffer,char *data,int length){
	if(RingBuffer_available_data(buffer)==0){
		buffer->start = buffer->end = 0;
	}
	void* result = NULL;
	check((length <= RingBuffer_available_space(buffer)),"Not enough space: %d request, %d available",length,RingBuffer_available_space(buffer));
	if(buffer->looped == 0 && (buffer->length-buffer->end <length)){
		result =memcpy(RingBuffer_ends_at(buffer),data,buffer->length-buffer->end); // 2 6 8
		result = memcpy(buffer->buffer,data + buffer->length - buffer->end,length -(buffer->length - buffer->end));
	}
	else{
		result = memcpy(RingBuffer_ends_at(buffer),data,length);
	}
	check(result != NULL,"Failed to write data into buffer");
	RingBuffer_commit_write(buffer,length); // 0 7
	return length;
	error:
	return -1;
}
int RingBuffer_read(RingBuffer *buffer,char* target,int amount){
	check_debug((amount <= RingBuffer_available_data(buffer)),"Not enough in the buffer : has %d, needs %d",RingBuffer_available_data(buffer),amount);
	void* result =NULL;
	if(buffer->looped == 1 && (buffer->length-buffer->start <amount)){
		result =memcpy(target,RingBuffer_starts_at(buffer),amount);
		result = memcpy(target+buffer->length - buffer->start,buffer->buffer,amount-buffer->length+buffer->start);
	}
	else{
		result = memcpy(target,RingBuffer_starts_at(buffer),amount);
	}
	check(result !=NULL,"Failed to write buffer into data");
	RingBuffer_commit_read(buffer,amount); // 5 7 
	if(buffer->end == buffer->start){ 
		buffer->start =buffer->end = 0;
	}
	return amount;
	error:
	return -1;
}
int RingBuffer_write1(RingBuffer *buffer,char* data,int length){
	check_debug((length <= buffer->length -buffer->end + buffer->start),"Not enough in the buffer: has %d, needs %d",buffer->length -buffer->end + buffer->start,length);
	void* result = memcpy(RingBuffer_ends_at(buffer),data,length);
	check((result !=NULL),"failed to write data into the buffer");
	buffer->end = buffer->end+length;
	return length;
	error:
	return -1;
}
int RingBuffer_read1(RingBuffer *buffer,char* target,int amount){
	check_debug((amount <= buffer->end - buffer->start),"Not enough data to read, has %d need %d",buffer->end - buffer->start,amount);
	void* result = memcpy(target,RingBuffer_starts_at(buffer),amount);
	check((result !=NULL),"failed to read the data");
	buffer->start = buffer->start + amount;
	if(buffer->start >= buffer->length){
		buffer->start = buffer->start- buffer->length;
		buffer->end = buffer->end -  buffer->length;
	}
	return amount;
	error:
	return -1;
}
bstring RingBuffer_gets(RingBuffer *buffer,int amount){
	check((amount >0),"Need more than 0 for gets,you gave %d",amount);
	check_debug((amount <= RingBuffer_available_data(buffer)),"Not enough in the buffer");
	bstring result = NULL;
	bstring result1 = NULL;
	if((buffer->looped == 1) && (amount < (buffer->length-buffer->start))){
		result = blk2bstr(RingBuffer_starts_at(buffer),buffer->length-buffer->start);
		result1 = blk2bstr(buffer->buffer,amount-(buffer->length-buffer->start));
		bconcat(result,result1);
	}else{
		result =blk2bstr(RingBuffer_starts_at(buffer),amount);
	}
	check(result !=NULL,"Failed to create gets result");
	check(blength(result) == amount,"Wrong result length");
	RingBuffer_commit_read(buffer,amount);
	#if 0
	bstring result = NULL;
	char* wow = malloc(amount);
	int me = RingBuffer_read1(buffer,wow,amount);
	result = bfromcstr(wow);
	#endif
	assert(RingBuffer_available_data(buffer) >= 0 && "Error in read commits");
	return result;
	error:
	return NULL;
}