#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#define PORT 8080 
int main(int argc, char const *argv[]) 
{
	printf("Child created...reading from client\n");
	//setuid(65534); 
	/*user - nobody(uid - 65534), current_user(uid -1000)
	use id command to get the userIds	*/
	int new_socket;
	const char* hello;
	if(argc > 0){
        new_socket=*argv[1];
	hello=argv[2];
	}
	char buffer[1024] = {0};
	printf("debug: %s\n\n",argv[3]);
    	int valread = read( new_socket ,buffer, 1024); 
    	printf("%s\n",buffer ); 
    	send(new_socket , hello , strlen(hello) , 0 ); 
    	printf("Hello message sent\n"); 
	printf("IN CHILD ->current_user_id: %d\n",getuid());
return 0;
}
