// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h> 
//#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024]={0};
    char try[1024]={0};
    int total_bytes_read=0,file_size;
    int port = 8080;
    if(argc > 1)
    {
	port = strtol(argv[1],NULL,10);
	 // strtol(char *number,char *string, int base)
    } 
    printf("port: %d\n",port);
    printf("..................\n");
    if(port<0 || port>65535)
    {
	printf("INVALID PORT NUMBER\n");
   	exit(0);
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Hello message sent\n");
    // reading file-size
    valread = read(sock,&file_size,sizeof(file_size)); 
    file_size = ntohs(file_size);
    printf("file-size: %d\ttry-size: %d\n",file_size,sizeof(try));
  
   /* if(file_size<sizeof(buffer))
    {
	read(sock,buffer,sizeof(buffer));
	printf("%s\n",buffer);
    }
    else{*/
    while(total_bytes_read<file_size)
    {
	if(file_size-total_bytes_read >= sizeof(buffer))
        {
    	total_bytes_read+=read(sock,buffer,sizeof(buffer));
        printf("%s",buffer);
 	}
	else if(file_size-total_bytes_read!=0)
	{
		total_bytes_read+= read(sock,try,sizeof(try));
		printf("%s\n",try);
		break;
	}
    }
    
   // valread = read(sock,buffer,sizeof(buffer));
    printf("valread: %d\n",valread);
    //free(buffer);
    return 0; 
} 
