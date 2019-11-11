#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h>
#include <sys/stat.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include <fcntl.h>
#define RED(string) "\x1b[31m" string "\x1b[0m"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"

int fileSize(int fd); 
int main(int argc, char const *argv[]) 
{
	
	printf("Child created...reading from client\n");
	char cwd[100];
	int chr,new_socket,file,bytes_read,file_size;
	char *hello; 
	// getting current working directory
	if(getcwd(cwd,sizeof(cwd))!=NULL)
        {
                printf("working directory before chroot: %s\n",cwd);
		// appending the empty directory name to the path
                strncat(cwd,"/emptyDir",sizeof(cwd)-strlen(cwd)-1);
		printf("directory after appending: %s\n",cwd);
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

	file_size = fileSize(file);
	printf("size of the file: %d\n",file_size);
	char buffer[1024] = {0};
	if(file_size>-1)
	{
	hello = (char*)calloc(file_size +1,sizeof(char));
	}
	
	// checking if the chroot succeeded
	if(chr == 0){
	// testting the chroot
	if(getcwd(cwd,sizeof(cwd))!=NULL)
        {
                printf(RED("working directory after chroot: %s\n"),cwd);
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
		bytes_read = read(file,hello,file_size);
		if(bytes_read>0)
		{
			hello[bytes_read] = '\0';
		}
		printf("File Contents:\n%s\n",hello);
	}
	else // for file
	{
		printf("Empty File\n");
	}
	// sending the file_size
	file_size= htons(file_size);
 	write(new_socket, &file_size,sizeof(int));
	// sending the contents of actual file	
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
	free(hello);	
	return 0;
}

//function to get file-size
int fileSize(int fd) {
   struct stat s;
   if (fstat(fd, &s) == -1) {
      return(-1);
   }
   return(s.st_size);
}
