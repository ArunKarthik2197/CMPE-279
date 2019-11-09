// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include <fcntl.h>
//#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    unsigned int port = 8080;  
    //char *hello = "Hello from server"; 
    int file_fd;
    pid_t parent_pid;   
    pid_t current_pid; 
    current_pid=getpid();
    if(argc>2)
    {
	printf("arg: %s\n",argv[1]);
	port = strtol(argv[1],NULL,10);
	if(argv[2]!=NULL)
	{
		file_fd = open(argv[2],O_RDONLY,S_IRUSR);
	}
    }  
	printf("port: %d\n",port);
	printf("..................\n");
	
 
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 
    
    
       
    // Forcefully attaching socket to the port 8080 
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
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
	printf("******* start **********\n");
	/*Solution for Assignment-1
	removing the privilege for the read and send functions of the server
	by creating a child process
	run the server as root process..using sudo.
	*/
    current_pid=fork(); // creates a child process
    if(current_pid == 0)
    {
	if(file_fd>0)
	{
	if(execl("reader","reader",&new_socket,&file_fd,NULL)<0)
	{
		printf("exec error\n");
		exit(0);
	}
	}
	else
	{
		printf("Error.....read denied\n");
		return 0;
	}
	
    }
    else if(current_pid > 0)
    {
	wait(NULL);
	printf("control returns to parent...\n");
	
    }
    else
    {
	perror("fork failed\n");
	_exit(2);
    }
    //printf(" IN PARENT -> current_user_id: %d\n",getuid());
    printf("DEBUG /ppid returns:%d\n",parent_pid);
    return 0; 
} 
