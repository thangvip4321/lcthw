#ifndef _lcthw_server_h_
#define _lcthw_server_h_
#include <lcthw/ringbuffer.h>
#include <sys/socket.h>
int echo_server(char* host,char* port); //make a server binding to a specific port
void read_some(int sockfd,RingBuffer *buffer);
void write_some(int sockfd,RingBuffer *buffer);
typedef struct sock_pack {
	int sockfd;
	struct sockaddr_storage addrs;
	socklen_t storage_size;
	void* data;
} sock_pack;
inline sock_pack* sock_pack_init(int sockfd, struct sockaddr_storage addrs, socklen_t storage_size,void* data){
	sock_pack* packet = malloc(sizeof(sock_pack));
	packet->sockfd = sockfd;
	packet->addrs = addrs;
	packet->storage_size = storage_size;
	packet->data = data;
	return packet;
}
#endif