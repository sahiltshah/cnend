#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#define BUF_LEN 100

int main()
{
	int state=1;
	key_t h=1;	// value of state should on every program where this share memory is used
 	int shmid=shmget(h,sizeof(int),IPC_CREAT|0666);
 	int *share_memory=(char*)shmat(shmid,(const void*)0,0);
 	gets(share_memory);
 	printf("%s",share_memory);
 	
	int rsfd=socket(AF_INET,SOCK_RAW,100);
	if(rsfd==-1)
	perror("socket");
	
	char buf[BUF_LEN];
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	
	recvfrom(rsfd,buf,BUF_LEN,0,(struct sockaddr*)&client,&clilen);
	perror("recv");
	
	struct iphdr *ip;
  	ip=(struct iphdr*)buf;
	printf("%s\n",(buf+(ip->ihl)*4));

}
