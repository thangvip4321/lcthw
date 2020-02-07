#include "bstrlib.h"
#include <stdio.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <stdlib.h>
char* wtf(){
	char* xd = "wtf";
	return xd;
}
char* data(bstring wow){
	return ((wow == NULL || wow->data == NULL) ? NULL : ((char *)wow->data) );
}
#define sex(a) (a)
char* sex1(char* wow){
	return wow;
}
char* clgt(int a){
	char* xx = malloc(a);
	debug("wow %d",a);
	return xx;
}
#define bdataofse(b, o, e)  (((b) == (void *)0 || (b)->data == (void*)0) ? (char *)(e) : ((char *)(b)->data) + (o))
#define bdataofs(b, o)      (bdataofse ((b), (o), (void *)0))
#define bdatae(b, e)        (bdataofse (b, 0, e))
#define bdata(b)            (bdataofs (b, 0))
int main(int argc,char* argv[]){
	RingBuffer* buffer = RingBuffer_create(20);
	RingBuffer_write(buffer,"12345678",8);
	char* wow = data(RingBuffer_get_all(buffer));
	debug("wow: %s",wow);
	RingBuffer* buffer1 = RingBuffer_create(20);
	RingBuffer_write(buffer1,"12345678",8);
	bstring xxx = RingBuffer_gets(buffer1,RingBuffer_available_data(buffer1));
	char* wow1 = bdataofse(xxx,0,"xxx");
	debug("still ok?");
	debug("wow: %s",wow1);
	char* nice = sex(clgt(4));
	char* ok = sex1(clgt(4));
	debug("nice : %s",nice);
	debug("okay: %s",ok);
}