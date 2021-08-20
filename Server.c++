#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
using namespace std;

#define PORT 9999


class client{
	public:
	string name;
	pthread_t newT;
	int socket;
	};
	
client clients[20];


void listClient(){
	
	for(int i=0;i<20;i++){
	        
		    cout << i << ": " << clients[i].name<<"\n";
		    
		    if(clients[i+1].name == ""){
				break;
			}
	   	}
	}
void closeAll(){
	
	for(int i=0;i<20;i++){
		
	        send(clients[i].socket , "close" , strlen("close") , 0 );
	        pthread_cancel(clients[i].newT);
	        close(clients[i].socket);
		    if(clients[i+1].name == ""){
				break;
				exit(0);
			}
	   	}
	
	}

int closeClient(string Args){
	string name = Args;
	for(int i=0;i<20;i++){
		
		if(clients[i].name==name){
			send(clients[i].socket , "close" , strlen("close") , 0 );
	        pthread_cancel(clients[i].newT);
	        close(clients[i].socket);
			return 1;
			
	   	}
	
	}
	return 0;
}


void * handleInput(void * Args){
	
	string input;
	
	while(1){
      
      getline(cin,input);
      
	  if(input=="list"){
      listClient();	
	  }
	  if(input=="closeall"){
      closeAll();	
	  }
	  string dummy= input;
	  string token= dummy.substr(0,5);
	  string name= dummy.substr(6); 
	  if(token=="close"){
		 
         int a = closeClient(name);
         if(a){
			 cout<<"Client "<< name << " is closed\n"; 
			 }
		 else{
			 cout<<"No client as "<< name << "\n";
			 }	
	  }
	  
      //handle code
    }
	
	return NULL;
}
void * handleClient(void * Args){
	
	int vRead;
	client newClient =*((client *)Args);
	char rBuffer[1024]={0};
	
	while(1){
		memset(rBuffer,0,1024);
		vRead = read(newClient.socket,rBuffer,1024);
		cout<<"new message from "<<newClient.name<<" :" << rBuffer <<"\n";
	    cout.flush();
	    
	}
	
	return NULL;
}

int main(int argc, char const *argv[])
{
	
	pthread_t t0;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt=1;
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
    
    int a=0;
    
    if(pthread_create(&t0,NULL, handleInput,&clients)!=0){
		
		  cout<<"failed to create thread";
		  cout.flush();  
		    
      }  
    
    while(1){
		
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
      {
          perror("accept");
          exit(EXIT_FAILURE);
      }
      memset(buffer,0,1024);      
      valread = read(new_socket,buffer,1024);
      string name = buffer;
      
      
      cout<<"a new client: "<<buffer<<"\n";
      cout.flush();
      
      clients[a].name = name;
      clients[a].socket=new_socket;
      
      
      if(pthread_create(&clients[a].newT,NULL, handleClient,&clients[a])!=0){
		  cout<<"failed to create thread";
		  cout.flush();
		  } 
	  a++;
	  if(a==20){
		  cout<<"client overflow\n";
		  }  
     }  
}
      
    

