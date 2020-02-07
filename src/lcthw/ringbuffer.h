#ifndef _lcthw_ringbuffer_h_
#define _lcthw_ringbuffer_h_
#include <lcthw/bstrlib.h>
typedef struct Ringbuffer {
	char* buffer;
	int length;
	int start;
	int end;
	int fd;
	int looped;
}RingBuffer;
RingBuffer *RingBuffer_create(int length);
RingBuffer *RB_init(int size);
void RingBuffer_destroy(RingBuffer *buffer);
void RingBuffer_destroy1(RingBuffer *buffer);
int RingBuffer_read(RingBuffer *buffer,char * target,int amount);
int RingBuffer_write(RingBuffer *buffer,char* data,int length);
int RingBuffer_read1(RingBuffer *buffer,char* target,int amount);
int Ringbuffer_write1(RingBuffer *buffer,char* data,int length);
int RingBuffer_empty(RingBuffer *buffer);
int RingBuffer_full(RingBuffer *buffer);
static inline int RingBuffer_available_data(RingBuffer *buffer){
	return (buffer->looped ==0) ?(buffer->end - buffer->start) : (buffer->length + buffer->end -buffer->start);
}
static inline int RingBuffer_available_space(RingBuffer *buffer){
	return buffer->length- RingBuffer_available_data(buffer);
}
bstring RingBuffer_gets(RingBuffer *buffer,int amount);
//#define RingBuffer_available_data(B) (((B)->end +1) % (B)->length - (B)->start - 1)  start 5 end 3: (3+1) %8 - 5 -1 = -2
//#define RingBuffer_available_space(B) ((B)->length- (B)->end-1)
#define RingBuffer_full(B) (RingBuffer_available_data(B)-(B)->length == 0)
#define RingBuffer_empty(B) (RingBuffer_available_data(B) == 0 )
#define RingBuffer_puts(B,D) RingBuffer_write((B),bdata((D)),blength((D)))
#define RingBuffer_get_all(B) RingBuffer_gets(B,RingBuffer_available_data(B))
#define RingBuffer_starts_at(B) ((B)->buffer +(B)->start)
#define RingBuffer_ends_at(B) ((B)->buffer + (B)->end)
#define RingBuffer_commit_read(B,A) (B)->start = (B)->start+(A); if((B)->start >=(B)->length){(B)->looped = 0;(B)->start %=(B)->length;}
#define RingBuffer_commit_write(B, A) (B)->end += A;if((B)->end >=(B)->length){(B)->looped =1;(B)->end %= (B)->length;}
#endif