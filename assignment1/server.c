// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#define RED(string) "\x1b[31m" string "\x1b[0m"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"
#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);
    int val; 
    char buffer[1024] = {0}; 
    
    char *hello = "Hello from server"; 
    pid_t parent_pid;   
    pid_t current_pid; 
    current_pid=getpid();  
 
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror(RED("socket failed")); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror(RED("setsockopt")); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
    
    
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror(RED("bind failed")); 
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
        perror(RED("accept")); 
        exit(EXIT_FAILURE); 
    } 
	printf(BLUE("******* start **********\n"));
	/*Solution for Assignment-1
	removing the privilege for the read and send functions of the server
	by creating a child process
	run the server as root process..using sudo.
	*/
    current_pid=fork(); // creates a child process
    if(current_pid == 0)
    {
	printf("Child created...reading from client\n");
	printf("child id: %d\n",current_pid);
	val = setuid(65534); /*user - nobody(uid - 65534), current_user(uid -1000)
	use id command to get the userIds
	*/
	printf("val: %d\tcurrent_id: %d\n",val,getuid());
	if(val ==-1){
	printf(RED("Error dropping privilege\n"));
	return 0;
	}
    	valread = read( new_socket , buffer, 1024); 
    	printf("%s\n",buffer ); 
    	send(new_socket , hello , strlen(hello) , 0 ); 
    	printf("Hello message sent\n"); 
	printf(RED("IN CHILD ->")BLUE("current_user_id: %d\n"),getuid());
    }
    else if(current_pid > 0)
    {
	wait(NULL);
	printf("control returns to parent...\n");
	
    }
    else
    {
	perror(RED("fork failed\n"));
	_exit(2);
    }
    //printf(" IN PARENT -> current_user_id: %d\n",getuid());
    printf(RED("DEBUG /ppid returns:"BLUE("%d\n")),parent_pid);
    printf(BLUE("*********END***********\n"));
    return 0; 
} 
