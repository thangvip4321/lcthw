#undef NDEBUG
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/dbg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
struct tagbstring NL = bsStatic("\n");
struct tagbstring CRLF = bsStatic("\r\n");
int nonblock(int fd){
	int flags = fcntl(fd,F_GETFL,0); // do sth with the file descriptor in this case get the access mode
	check((flags >= 0),"Invalid flags on nonblock");
	int rc = fcntl(fd,F_SETFL,flags | O_NONBLOCK); // this case set the accessmode to nonblock
	check(rc ==0,"Cant set nonblocking");
	return 0;
	error:
	return -1;
}
int client_connect(char* host,char* port){
	int rc = 0;
	struct addrinfo  *addr = NULL;
	rc = getaddrinfo(host,port,NULL,&addr);
	check(rc == 0,"Failed to lookup %s: %s",host,port);
	int sock = socket(AF_INET,SOCK_STREAM,0); // create a socket that stream data
	rc = connect(sock,addr->ai_addr,addr->ai_addrlen); // connect that socket to the fucking host???????????????????????
	check(rc == 0 ,"connect failed");
	rc = nonblock(sock);
	check(rc == 0 ,"cant set nonblocking");
	freeaddrinfo(addr);
	return sock;
	error:
	freeaddrinfo(addr);
	return -1;
}
int read_some(RingBuffer *buffer,int fd,int is_socket){
	int rc = 0;
	if( RingBuffer_available_data(buffer)==0){
		buffer->start = buffer->end = 0;
	}
	char* data = malloc(RingBuffer_available_space(buffer));

	if(is_socket){
		rc = recv(fd,data,RingBuffer_available_space(buffer),0);
	}else{
		rc = read(fd,data,RingBuffer_available_space(buffer)); // basically it's recv without flags
	}
	int rs = RingBuffer_write(buffer,data,RingBuffer_available_space(buffer));
	check((rc >=0),"failed to read from fd: %d",fd);
	check((rs !=-1),"failed to transmit from data into buffer");
	free(data);
	return rc;
	error:
	return -1;
}
int write_some(RingBuffer *buffer,int fd,int is_socket){
	int rc = 0;
	bstring data = RingBuffer_get_all(buffer);
	check((data !=NULL),"failed to get from the buffer");
	check(bfindreplace(data,&NL,&CRLF,0) == BSTR_OK,"Failed to replace NL");
	if (is_socket){
		rc = send(fd,bdata(data),blength(data),0);
	}else{
		rc = write(fd,bdata(data),blength(data));
	}
	check((rc == blength(data)),"Failed to write every thing to fd: %d.",fd);
	bdestroy(data);
	debug("ok write_some");
	return rc;
	error:
	return -1;
}
int main(int argc,char* argv[]){
	fd_set allreads;
	fd_set readmask;
	int socket = 0;
	int rc = 0;
	RingBuffer *in_rb = RingBuffer_create(1024 *10);
	RingBuffer *sock_rb = RingBuffer_create(1024 *10);
	check((argc ==3),"USAGE: netclient host port");
	socket = client_connect(argv[1],argv[2]);
	check((socket >=0),"Connect to %s: %s failed.",argv[1],argv[2]);
	FD_ZERO(&allreads);
	FD_SET(socket,&allreads);
	FD_SET(0,&allreads);
	while(1){
		readmask = allreads;
		rc =select(socket+1,&readmask,NULL,NULL,NULL); // ok why socket +1, you ask? because select only check file indicator, UP TO THAT LIMIT
		// also select(lim,read mask,write mask, except mask,time_out); readmask check ability of a fd to read without blocking??? same to write mask
		check((rc >=0),"select failed");
		if(FD_ISSET(0,&readmask)){
			rc = read_some(in_rb,0,0);
			check_debug((rc != -1),"Failed to read from stdin");
			debug("step 1");

		}
		if(FD_ISSET(socket,&readmask)){
			rc = read_some(sock_rb,socket,0);
			check_debug((rc !=-1),"Failed to read from socket");
			debug("step 2");
			sleep(2);

		}
		while(!RingBuffer_empty(sock_rb)){
			rc = write_some(sock_rb,1,0);
			check_debug((rc != -1),"Failed to write to stdout");
			debug("step 3");
		}
		while(!RingBuffer_empty(in_rb)){
			rc = write_some(in_rb,socket,1);
			check_debug((rc !=-1),"Failed to write to socket");
			debug("step 4");
		}
	}
	return 0;
	error:
	return -1;
}