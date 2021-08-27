#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
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


int sendClient(string message,client newClient){
  stringstream ss(message);
  
  string reciever;
  getline(ss,reciever,' ');
  int a = reciever.length()-1;
  string newMessage = newClient.name +" says :"+message.substr(a);
  char* buffer = &newMessage[0];
  for (int i=0; i<20; i++){
	if(clients[i].name==reciever){
	  send(clients[i].socket , buffer , strlen(buffer) , 0 );
	  memset(buffer,0,strlen(buffer));
      return 1;
    }
  }
  return 0;

}


int sendArdu(string message,client newClient){
  stringstream ss(message);
  
  string token;
  getline(ss,token,' ');
  int a = token.length();
  int vRead;
  string newMessage = "message: "+ message.substr(a);
  char* buffer;
  memset(buffer,0,1024);
  buffer = &newMessage[0];
  char rBuffer[1024];
  
  for (int i=0; i<20; i++){
	if(clients[i].name=="arduino"){
	  memset(rBuffer,0,1024);
	  
	  if(message == "led 1 on"){
		send(clients[i].socket , "a" , strlen("a") , 0 );	  
	    return 1;
      }
      else if(message == "led 1 off"){
		send(clients[i].socket , "b" , strlen("b") , 0 );		  	  
	    return 1;
      }
      else if(message == "led status"){
		send(clients[i].socket , "c" , strlen("c") , 0 );
		vRead = read(clients[i].socket,rBuffer,1024);
		send(newClient.socket , rBuffer , strlen(rBuffer) , 0 );		  
	    return 1;
	  }

      else if(message == "buzzer on"){
		send(clients[i].socket , "d" , strlen("d") , 0 );
        return 1;
      }
      
      else if(message == "temp"){
		send(clients[i].socket , "e" , strlen("e") , 0 );
		vRead = read(clients[i].socket,rBuffer,1024);
		send(newClient.socket , rBuffer , strlen(rBuffer) , 0 );	
        return 1;
      }
      
      else if(message == "humd"){
		send(clients[i].socket , "f" , strlen("f") , 0 );
		vRead = read(clients[i].socket,rBuffer,1024);
		send(newClient.socket , rBuffer , strlen(rBuffer) , 0 );	
        return 1;
      }
      else if(message == "servo 0"){
		send(clients[i].socket , "g" , strlen("g") , 0 );
        return 1;
      }
      else if(message == "servo 90"){
		send(clients[i].socket , "h" , strlen("h") , 0 );
        return 1;
      }
      else if(message == "servo 180"){
		send(clients[i].socket , "i" , strlen("i") , 0 );
        return 1;
      }

      else if(token == "lcd"){
		send(clients[i].socket , buffer , 100 , 0 );  	  
        return 1;
      }  
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
    else if(input=="closeall"){
      closeAll();	
	}
	else if(input.length()>6){ 
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
    }
  }  
  return NULL;
}


string listStr(){
  string list="";
  for(int i=0;i<20;i++){
  list+=i+"."+clients[i].name+"\n";
  }
  return list;
}


void * handleClient(void * Args){
	
  int vRead;
  client newClient =*((client *)Args);
  char rBuffer[1024]={0};
  string input;
  string command;
  string message;
	
  while(1){
    memset(rBuffer,0,1024);
    vRead = read(newClient.socket,rBuffer,1024);
	cout<<"new message from "<<newClient.name<<" :" << rBuffer <<"\n";
    cout.flush();
	    
    input = rBuffer;
    int check=0;
	cout.flush();
	if(input.length()>5){     
      command = input.substr(0,4);
      message = input.substr(5);
	    
      if(command=="ardu"){
        check = sendArdu(message,newClient);
        send(newClient.socket , "sending message to ardu" , strlen("sending message to ardu") , 0 );
      }
	  else if(command=="clnt"){
	    check = sendClient(message,newClient);
        send(newClient.socket , "sending message to client" , strlen("sending message to client") , 0 );
	  }
	  else if(command=="list"){
	    string listString ; 
	    listString = listStr();
	    char* charList = &listString[0];
	    send(newClient.socket , charList , strlen(charList) , 0 );			
	    memset(charList,0,strlen(charList));
	    check = 1;
	  }
	  else if(command=="exit"){
		closeClient(newClient.name);  
	  }
	  if (check==0){
	    send(newClient.socket , "error" , strlen("error") , 0 );	
	  }
	
    }
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
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
       
  // connect socket to 9999
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))){
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("172.31.45.192");//127.0.0.1 local host 172.31.45.192
  address.sin_port = htons( PORT );
       
  // bind to 9999
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
    
  int a=0;
    
  if(pthread_create(&t0,NULL, handleInput,&clients)!=0){
	cout<<"failed to create thread";
	cout.flush();  
  }  
      
    
  while(1){
		
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0){
      perror("accept");
      exit(EXIT_FAILURE);
    }
    
    if(a==20){
	  cout<<"client overflow\n";
	  for(int i=0; i<20;i++){
		if(clients[i].name == ""){
		  a = i;
		  break;
		}
	    send(new_socket , "Sorry, Server Full" , strlen("Sorry, Server Full") , 0 );
	    send(new_socket , "close" , strlen("close") , 0 );
	    close(new_socket);
	  }
	}
	
    memset(buffer,0,1024);      
    valread = read(new_socket,buffer,1024);
    string name = buffer;
      
      
    cout<<"a new client: "<<buffer<<"\n";
    cout.flush();
      
    clients[a].name = name;
    clients[a].socket=new_socket;
      
    if(clients[a].name !="arduino"){  
      if(pthread_create(&clients[a].newT,NULL, handleClient,&clients[a])!=0){
	  cout<<"failed to create thread";
	  cout.flush();
      } 
      a++;
    } 
  } 
}
