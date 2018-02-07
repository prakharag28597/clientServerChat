//server file

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<pthread.h>

int client_sock[5];
void *recvmg(void *sock)
{
	int their_sock = *((int *)sock);
	char msg[500];
	int i;
	int len;
	while(1){
		while((len = recv(their_sock,msg,500,0)) > 0) {
		// here message is received from the client 1
			msg[len] = '\0';
			// msg now should be written back to client 2
			char send_to=msg[0];
			for(i=0;i<5;++i)
			{
				if(client_sock[i]==send_to-'0'){
					msg[0]=their_sock+'0';
					send(client_sock[i],msg,strlen(msg),0);
					bzero(msg,sizeof(msg));
				}
			}
		}
	}
	
}
/*
void *recvmg2(void *sock)
{
	int their_sock = *((int *)sock);
	char msg[500];
	int len;
	while(1){
		while((len = recv(their_sock,msg,500,0)) > 0) {
		// here message is received from the client 1
			msg[len] = '\0';
			// msg now should be written back to client 2
			send(client_sock[0],msg,500,0);
			bzero(msg,sizeof(msg));
		}
	}
	
}*/

int main(){
  pthread_t client_thread[5];
  
  int serverSocket, newSocket;
  char message[256];
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
 
  //socket created
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  //IP type of connection
  serverAddr.sin_family = AF_INET;
  //specifing port number
  serverAddr.sin_port = htons(7891);
  //specifing server address
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
  //binding the socket with information
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  //listening 
  printf("Maximum 5 clients can connect at a time\n");
  if(listen(serverSocket,5)==0)
    printf("Listening\n");
  else
    printf("Error\n");
  addr_size = sizeof serverStorage;
	
	int i=0;
	while(1){
		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
		//if a new socket or conection is established
		if(newSocket>0)
  		{
  			client_sock[i]=newSocket;
  			printf("Client - %d connected to the server\n",client_sock[i]);
  			pthread_create(&client_thread[i],NULL,recvmg,&client_sock[i]);
  			++i;
  		}
  		if(i==5)
  		break;
	}
	//closing the thread
  	for(i=0;i<5;++i)
  	pthread_join(client_thread[i], NULL);

  	
  return 0;
}
