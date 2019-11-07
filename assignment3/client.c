// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <stdlib.h> 
//#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client";
    char c; 
    //char buffer[1024] = {0};
    FILE *fp;
    int port = 8080;
    if(argc > 1)
    {
	port = strtol(argv[1],NULL,10);
	 // strtol(char *number,char *string, int base)
    } 
    printf("port: %d\n",port);
    printf("..................\n");
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
    valread = read( sock , fp, 1024);
    if(fp == NULL)
        {
                printf("File Read Error\n");
        } 
        else
        {
                do
                {
                        c = getc(fp);
                        if(c!=EOF){
                        printf("%c",c);
                        }
                }while(c!=EOF);
        } 
    //printf("%s\n",buffer ); 
    return 0; 
} 
