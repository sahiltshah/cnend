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
#define ADDRESS "mysocket"

int send_fd(int socket, int fd_to_send)
 {
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
 }

int  usfd;
struct sockaddr_un userv_addr,ucli_addr;
int userv_len,ucli_len;
int nusfd;

int main()
{

	
	int rsfd=socket(AF_INET,SOCK_RAW,100);
	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	//cout<<"opt"<<endl;
	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	//client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	
	//char buff[] = "HELLO";
  	//sendto(rsfd,buff,5,0,(struct sockaddr*)&client,sizeof(client));
  	//perror("send");
	
	struct sockaddr_un userv_addr,ucli_addr;


	usfd = socket(AF_UNIX , SOCK_STREAM , 0);
	perror("socket");

  	bzero(&userv_addr,sizeof(userv_addr));

  	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	userv_len = sizeof(userv_addr);

	if(bind(usfd, (struct sockaddr *)&userv_addr, userv_len)==-1)
	perror("server: bind");

	listen(usfd, 5);

	ucli_len=sizeof(ucli_addr);
	

	
	
	FILE* f1 = popen("./r1","r");
	FILE* f2 = popen("./r2","r");
	FILE* f3 = popen("./r3","r");
	
	int fd[3];
	fd[0] = fileno(f1);
	fd[1] = fileno(f2);
	fd[2] = fileno(f3);
	
	fd_set readset;
	FD_ZERO(&readset);
	
	int max=-1;
	
	for(int i=0;i<3;i++)
	{
		FD_SET(fd[i], &readset);
		if(fd[i]>max)
		max=fd[i];		
	}
	
	struct timeval t;
	t.tv_sec=1000;
	t.tv_usec=0;
	int rv = select(max+1, &readset, NULL, NULL, &t);
	printf("%d\n",rv);
	printf("%d\n",max);
	
	nusfd=accept(usfd, (struct sockaddr *)&ucli_addr, &ucli_len);
	printf("%d\n",nusfd);
	//while(1)
	{	
	
	
		if (rv == -1) 
		{
			perror("select");
		}
	
		else if (rv == 0) 
		{
	    		printf("Timeout occurred!\n");
		} 
	
		else 
		{
			int i;
			// check for events 
			for(int i=0;i<3;i++)
	    		if (FD_ISSET(fd[i], &readset)) 
			{
	                      char* buff;
	                      read(fd[i],buff,100);
	                      printf("\n%s",buff);
	                      printf("\n%c",buff[0]);
	                     if(buff[0] =='d')
	                     { 
				printf("\nEntered");			
			        send_fd(nusfd,fd[i]);
			        printf("\nSent");
	                     }
	                     
	                     else
	                     {
	                     	// sendto(rsfd,buff,5,0,(struct sockaddr*)&client,sizeof(client));
 				 //perror("send");
	                     }
	    		}
		}
	}
 	
		
	/*char *buff;
        read(fileno(f),buff,20);
        
        printf("%s\n",buff);*/
        
}
	
