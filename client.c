//client file

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>


//receive message function
void *recvmg(void *sock)
{
	int their_sock = *((int *)sock);
	char msg[500];
	int len,i;
	while(1){
		while((len = recv(their_sock,msg,500,0)) > 0) {
			printf("Sender %c: ",msg[0]);
			for(i=1;msg[i]!='\0';++i)
			printf("%c",msg[i]);
			printf("\n");
			memset(msg,'\0',sizeof(msg));
		}
	}
	
}

// to send message
void *sendmg(void *sock)
{
	int their_sock = *((int *)sock);
	char msg[500];
	int len;
	while(1){
		scanf("%s", msg);
		if(strlen(msg)>0){
			send(their_sock,msg,500,0);
			bzero(msg,sizeof(msg));
		}
	}
}


int main(){
	int clientSocket;
  	char message[256];
  	struct sockaddr_in serverAddr;
  	socklen_t addr_size;
  	//declaring the thread
  	pthread_t send_thread,receive_thread;
   	// setting up the client socket
  	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  	serverAddr.sin_family = AF_INET;
  	serverAddr.sin_port = htons(7891);
  	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  	addr_size = sizeof serverAddr;
  	//connecting to the server
  	if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0)
  	{
  		perror("Connection not esatablished");
		exit(1);
  	}
  	printf("Connected to the server! \n");
  	
  	// send thread
  	pthread_create(&receive_thread,NULL,recvmg,&clientSocket);
  	// receive thread
  	pthread_create(&send_thread,NULL,sendmg,&clientSocket);
  	
  	//joining the threads
  	pthread_join(receive_thread, NULL);
  	pthread_join(send_thread, NULL);
  	
	return 0;
}
