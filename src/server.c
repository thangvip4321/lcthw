#include <lcthw/server.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <lcthw/dbg.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#define QUEUE_MAX 10
void* get_inaddr(struct sockaddr_storage *addr){
	if(addr->ss_family == AF_INET)
		return &((struct sockaddr_in *) addr)->sin_addr;
	return &((struct sockaddr_in6 *) addr)->sin6_addr;
}
void read_some(int sockfd,RingBuffer *buffer){
	int byte_recv =0;
	check((RingBuffer_available_space(buffer) > 0),"out of space, release some data");
	if(buffer->looped == 0){
		byte_recv = recv(sockfd,RingBuffer_ends_at(buffer),buffer->length - buffer->end,0);
		if(byte_recv < buffer->length- buffer->end){
			debug("ok then byte_recv is %d",byte_recv);
			RingBuffer_commit_write(buffer,byte_recv);
			return ;
		}else{
			debug("ok then not enough, byte_recv is %d",byte_recv);
			RingBuffer_commit_write(buffer,byte_recv);
			byte_recv = recv(sockfd,RingBuffer_ends_at(buffer),RingBuffer_available_space(buffer),0);
			RingBuffer_commit_write(buffer,byte_recv);
		}
	}else{
		byte_recv = recv(sockfd,RingBuffer_ends_at(buffer),RingBuffer_available_space(buffer),0);
		RingBuffer_commit_write(buffer,byte_recv);
	}
	error:
	errno = EOVFL;
	return;
	
}
void write_some(int sockfd,RingBuffer *buffer){
	int byte_send = 0;
	check((RingBuffer_available_data(buffer)>0),"nothing to retrieve");
	if(buffer->looped == 1){
		byte_send = send(sockfd,RingBuffer_starts_at(buffer),buffer->length- buffer->start,0);
		if(byte_send <buffer->length-buffer->start){

			RingBuffer_commit_read(buffer,byte_send);

			return;
		}
		RingBuffer_commit_read(buffer,byte_send);
		byte_send = send(sockfd,RingBuffer_starts_at(buffer),RingBuffer_available_data(buffer),0);
		RingBuffer_commit_read(buffer,byte_send);
	}else{
		byte_send = send(sockfd,RingBuffer_starts_at(buffer),RingBuffer_available_data(buffer),0);
		RingBuffer_commit_read(buffer,byte_send);
	}
	error:
	return;
}
int echo_server(char* host,char* port){
	//establish connection with a port and an IP address
	struct addrinfo hint, *res, *p;
	int sockfd,new_fd;
	int yes = 1;
	struct sockaddr_storage their_addr;
	memset(&hint,0,sizeof(hint));
	socklen_t addrlen = sizeof(their_addr);
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	char list[INET6_ADDRSTRLEN];
	int rs = getaddrinfo(host,port,&hint,&res);
	for(p = res; p !=NULL;p = p->ai_next){
		sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if(sockfd == -1){
			close(sockfd);
			perror("socket failed");
			continue;
		}if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
			perror("socket opt failed");
			return -1;
		}if(bind(sockfd,p->ai_addr,p->ai_addrlen) == -1){
			close(sockfd);
			perror("bind failed");
			continue;
		}
		break;	
	}
	freeaddrinfo(res);
	if(p ==NULL){// ???
		perror("failed to bind");
		return -1;
	}
	return sockfd;
}
// after having a sockfd, listen on them and create connection
	#if 0
	listen(sockfd,QUEUE_MAX);
	while(1){
		new_fd = accept(sockfd,(struct sockaddr*) &their_addr,&addrlen);
		if(new_fd == -1){
			perror("accept");
			continue;
		}
		printf("internet connection set at %s",inet_ntop(their_addr.ss_family,get_inaddr(their_addr),list,sizeof(list)));
		break;
		//do something here? fork?
		while(fork() >0){ //not parent
			close(sockfd); 
			//do sth here
			close(new_fd);
			exit(0);
		} 
	}
	//check((rs ==0),"hmm wrong shit");
	return new_fd;
	error:
	return -1;
}
#endif
