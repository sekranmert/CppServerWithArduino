#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>

#define PORT 8080


class client{
	public:
	std::string name;
	pthread_t newT;
	int socket;
	};


void * handleClient(void * new_socket){
	int vRead;
	int socket =*((int *)new_socket);
	char rBuffer[1024]={0};
	
	while(1){
		vRead = read(socket,rBuffer,1024);
		printf("new message: %s\n",rBuffer);
		}
	
	return NULL;
	}

int main(int argc, char const *argv[])
{
	client clients[20];
	pthread_t t[20];
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
       
    //socket object
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // connect socket to 9999
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("10.0.2.15");//127.0.0.1 local host
    address.sin_port = htons( PORT );
       
    // bind to 9999
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    int i=0;
    
    while(1){
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
      {
          perror("accept");
          exit(EXIT_FAILURE);
      }
            
      send(new_socket, "name" , strlen("name") , 0 );
      valread = read(new_socket,buffer,1024);
      std::string name = buffer;
      
      
      printf("a new client: %s",buffer);
      
      clients[i].name = name;
      clients[i].socket=new_socket;
      clients[i].newT=t[i];
      
      if(pthread_create(&t[i],NULL, handleClient,&new_socket)!=0){
		  printf("failed to create thread");
		  }
      
      
     
    }  
      
    
}
