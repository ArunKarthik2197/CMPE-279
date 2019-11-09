#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include <fcntl.h>
#define PORT 8080 
int main(int argc, char const *argv[]) 
{
	
	printf("Child created...reading from client\n");
	char cwd[100];
	int chr;
	// getting current working directory
	if(getcwd(cwd,sizeof(cwd))!=NULL)
        {
                printf("working directory before chroot: %s\n",cwd);
		// appending the empty directory name to the path
                strncat(cwd,"/emptyDir",sizeof(cwd)-strlen(cwd)-1);
        }
        else
        {
                printf("getcwd error before chroot\n");
        }
	//changing directory to the fake root
        if(chdir(cwd)== 0){
        chr = chroot(cwd); // making the present empty directory a fake root
        }
        else
        {
        printf("chdir error...\n");
        }
	//setuid(65534); 
	/*user - nobody(uid - 65534), current_user(uid -1000)
	use id command to get the userIds	*/
	int new_socket,file,bytes_read;
	char hello[100] = {0} ;
	// check for command line args
	if(argc > 1){
        new_socket=*argv[1];
	file  = *argv[2];
	}
	else // for command line args check
	{
	printf("Error...\n");
	exit(0);
	}
	char buffer[1024] = {0};
	
	// checking if the chroot succeeded
	if(chr == 0){
	// testting the chroot
	if(getcwd(cwd,sizeof(cwd))!=NULL)
        {
                printf("working directory before chroot: %s\n",cwd);
        }
        else
        {
                printf("getcwd error after chroot\n");
        }
	
	// dropping privilege
	if(setuid(65534)== 0 ){
    	int valread = read( new_socket ,buffer, 1024); 
    	printf("%s\n",buffer );
	//converting file contents to string
	if(file >0)
	{
		bytes_read = read(file,hello,100);
		if(bytes_read>0)
		{
			hello[bytes_read] = '\0';
		}
	}
	else // for file
	{
		printf("Empty File\n");
	}
	
    	send(new_socket , hello , strlen(hello) , 0 ); 
    	printf("Hello message sent\n"); 
	printf("IN CHILD ->current_user_id: %d\n",getuid());
	}
	else // for setuid
	{
		printf("access denied");
	}
	}
	else // for chroot
	{
		printf("chroot error\n");
	}
return 0;
}
