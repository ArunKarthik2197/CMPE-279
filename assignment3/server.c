// Server side C/C++ program to demonstrate Socket programming 
#define _GNU_SOURCE
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#define RED(string) "\x1b[31m" string "\x1b[0m"
#define BLUE(string) "\x1b[34m" string "\x1b[0m"

//#define PORT 8080 
int fileSize(int fd);
int main(int argc, char const *argv[], char const *env[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    char *ext;
    //char file_type[100]= {0}; 
    unsigned int port = 8080;   
    int file_fd;
    struct stat st = {0};
    pid_t parent_pid;   
    pid_t current_pid; 
    current_pid=getpid();
     char *envp = getenv("CHECK");
     if(envp == NULL || *envp == '\0'){
    if(argc>2)
    {
	port = strtol(argv[1],NULL,10);
	if(argv[2]!=NULL)
	{
		ext = strrchr(argv[2], '.');
		if(ext!=NULL)
		ext[strlen(ext)] = '\0';
	
		if (!ext) {
		    //printf("ext:%s\n",ext);
		    printf(RED("INVALID FILE\n"));
		    exit(0);
		}
		else if(strcmp(ext,".txt") == 0 || strcmp(ext,".c")==0 || strcmp(ext,".java") == 0)
		{
		file_fd = open(argv[2],O_RDONLY,S_IRUSR);
		}
		else
		{
			printf(RED("INVALID FILE\n"));
			exit(0);
		}
		if(file_fd<0)
		{
			printf(RED("Invalid File\n"));
			exit(0);
		}
	}
    }
    else
	{
		printf (RED("ERROR: not enough command line arguments\n"));
		exit(0);
	}  
	//file_type = fileType(file_fd);
//	printf("file-type: %s\n",fileType(file_fd));
	printf("port: %d\n",port);
	printf("..................\n");
	
    if(port>1024 && port<65536)
    {
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
	printf(BLUE("******* start **********\n"));
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
		//setenv("CHECK","child",1);
	
	char *env[] ={"CHECK=child",NULL};
	printf("execle - ing to child \n");
	if(execle("server","server",&new_socket,&file_fd,(char *) NULL,env)<0)
	{
		printf("exec error\n");
		exit(0);
	}
	}
	else
	{
		printf(RED("Error...file does not exist\n"));
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
    }
    else
    {
       printf(RED("Invalid Port number\n"));  
    }}
    else
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
	
        if(stat(cwd, &st) == -1){
	if(mkdir(cwd,0700) == -1)
	{
	   printf("could not create directory %s\n", cwd);
	   exit(0);
	}
        }

	//changing directory to the fake root
        if(chdir(cwd)==0){
	printf("chdir success \n");
        chr = chroot(cwd); // making the present empty directory a fake root
        }
        else
        {
        printf("chdir error...\n");
	exit(0);
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
	hello = (char*)calloc(file_size,sizeof(char));
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
    return 0; 
}
/*char* fileType(int fd) {
   struct stat s;
   if (fstat(fd, &s) == -1) {
      return(NULL);
   }
   return(s.st_mode);
} */

int fileSize(int fd) {
   struct stat s;
   if (fstat(fd, &s) == -1) {
      return(-1);
   }
   return(s.st_size);
}
