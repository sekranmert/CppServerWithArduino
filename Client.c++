#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
using namespace std;

#define PORT 9999
int cont = 1;

void * getMessage(void * Args){
	
	int vRead;
	int socket =*((int *)Args);
	char rBuffer[1024]={0};
	
	while(1){
		
		vRead = read(socket,rBuffer,1024);
		if(strcmp(rBuffer,"close")==0){
           exit(0);	
	    }
		cout<<"new message :" << rBuffer <<"\n";
	    cout.flush();
	    memset(rBuffer,0,1024);
	}
	cont=0;
	return NULL;
}

void * sendMessage(void * Args){
	
	int vRead;
	int socket =*((int *)Args);
	char sBuffer[1024]={0};
	string input;
	
	while(1){
      
      getline(cin,input);
      strcpy(sBuffer,input.c_str());     
		  
      send(socket , sBuffer , strlen(sBuffer) , 0 );
      cout<<"Message sent\n";
      cout.flush();
      memset(sBuffer,0,1024);
    }
	
	return NULL;
}
   
int main(int argc, char const *argv[])
{
	
    
    int sock = 0;
    int valread;
    pthread_t t0,t1;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout<<"\n Socket creation error \n";
        cout.flush();
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "10.0.2.15", &serv_addr.sin_addr)<=0)//"3.134.84.99" for cloud computer 
    {
        cout<<"\nInvalid address/ Address not supported \n";
        cout.flush();
        return -1;
        
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"\nConnection Failed \n";
        cout.flush();
        return -1;
    }
    
   
	cout << "Enter your Name :\n";
	cout.flush();
	cin >> buffer;
    send(sock , buffer , strlen(buffer) , 0 );
    memset(buffer,0,1024);
    
    if(pthread_create(&t0,NULL, getMessage,&sock)!=0){
		  cout<<"failed to create thread";
		  cout.flush();
		  } 
	if(pthread_create(&t1,NULL, sendMessage,&sock)!=0){
		  cout<<"failed to create thread";
		  cout.flush();
		  } 
		  	  
		  
    while(cont){
		
	}
}
