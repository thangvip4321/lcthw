#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <lcthw/dbg.h>
#include <lcthw/ringbuffer.h>
#include <lcthw/proc_request.h>
#include <lcthw/server.h>
#include <sys/wait.h>
#include <pthread.h>
#define QUEUE_MAX 10
#define BUFFER_SIZE 1024
#define ptr_to_u64(ptr) ((__u64)((uintptr_t)(ptr)))
struct addrinfo hints;
struct addrinfo *res;
struct sockaddr_storage their_addr;
socklen_t addr_size;
int new_sock = 0;

int p = 0 ;
int clients_num=0;
pthread_mutex_t lock;
void child_handler(int signal){
	fprintf(stderr,"found a child killed");
	int saved_errno = errno;
	while(waitpid(-1,NULL,WNOHANG)>0);
	errno = saved_errno;
}
void* client_process(void* arg){
	int byte_send = 0;
	RingBuffer *buffer = RingBuffer_create(BUFFER_SIZE);
	sock_pack * packet = arg;
	struct sockaddr_storage addr_storage =  packet->addrs;
	int sockfd = packet->sockfd;
	size_t* addr_size = packet->storage_size; 
	//TSTree *root_thread = packet->data;
	int new_sock = accept(sockfd,(struct sockaddr*) &addr_storage,&addr_size);
	char* respond = malloc(1024*1024);
	if(new_sock == -1){
		perror("cant find sockfd");
		exit(-1);
	}
	//int error_code;
	//int error_code_size = sizeof(error_code);
	//if (setsockopt(new_sock,SOL_SOCKET,MSG_NOSIGNAL,&optval,sizeof(optval)) !=0 ){ //almost work sadly no SO_NOSIGPIPE in linux (weird) 
	//	perror(" error with setsockopt");
	//	exit(-1);
	//}
	while(errno != EPIPE){ // hmmm what if we create a error not EPIPE in this while shit :v 
				debug("start reading");
				read_some(new_sock,buffer); // ok problem with recv blocking? so we can't check the condition if the file exist.
				bstring data = RingBuffer_get_all(buffer);
				pthread_mutex_lock(&lock);
				respond = process_request((TSTree**) packet->data,bdata(data)); // why rs span a lot?????????  hmm not quite efficient? change process request?
				//add an option quit (q) to exit safely
				pthread_mutex_unlock(&lock);
				//check((rs != NULL),"dit con me loi r em oi"); error: server exit after clients
				debug("ok after read_some, maybe the problem in send");
				byte_send = send(new_sock,respond,strlen(respond),MSG_NOSIGNAL);
				//debug("status: %d",getsockopt(new_sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size));
				//debug("wtf error code :%d",error_code);
			}
		close(new_sock);
		pthread_exit(NULL);
	}
int main(int argc,char* argv[]){
	pthread_t threads[QUEUE_MAX];
	TSTree *root = NULL;
	debug("the pointer to root has address: %p",&root);
	addr_size = sizeof(their_addr);
	int sockfd = echo_server("127.0.0.1","1234");
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_handler = child_handler;
	act.sa_flags = SA_RESTART;

	if(sigaction(SIGCHLD,&act,NULL) == -1){
		perror("cant change action of the signal");
		exit(1);
	}

	
	debug("still ok here");
	listen(sockfd,QUEUE_MAX);
	
	while(clients_num<QUEUE_MAX){
		debug("client number: %d",clients_num);
		sock_pack * packet = sock_pack_init(sockfd,their_addr,addr_size,&root);
		pthread_create(&threads[clients_num],NULL,client_process,packet); // hey what if accept in every threads fight for the connection hmmm
		++clients_num;
	}
	for(int i=0;i <QUEUE_MAX;i++){
		pthread_join(threads[i],NULL);
	}
	exit(1);
}

	/*while(1){
		addr_size = sizeof(their_addr);
		debug("still working here my friend?");
		new_sock = accept(sockfd,(struct sockaddr*)&their_addr,&addr_size);
		if(new_sock == -1){
			debug("cant find sockfd");
			continue;
		}
		debug("pid: %d",getpid());
		//p = fork();
		// the idea here is to let the children do the fucking task and the parent only listen for connection
		debug("p1 is %d",p);
		if(clone() == 0){ //not parent ?
			debug("p is %d",p);
			debug("dont need sockfd in kids");
			close(sockfd);
			while(1){
				debug("start reading");
				read_some(new_sock,buffer);
				bstring data = RingBuffer_get_all(buffer);
				rs = process_request(&root,bdata(data)); // why rs span a lot?????????  hmm not quite efficient? change process request?
				//check((rs != NULL),"dit con me loi r em oi"); error: server exit after clients
				byte_send = send(new_sock,rs,strlen(rs),0);
			}
			debug("ok");
			debug(" closing this process: %d",getpid());
			close(new_sock);
			exit(0);	
			//error:
			//close(sockfd);	
		}
		debug("hmmm qewsd00");
		close(new_sock);
	}
	close(sockfd);
	*/


