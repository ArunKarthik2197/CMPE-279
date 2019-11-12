>**1. What happens if the file being requested is not inside the chroot? Does it matter?**
     
>- File requested is only used by the child process to read the content and transform that to string to send it to client. 
>- So, it is not required to have file in the chroot directory, passing a file descriptor of a file after opening is enough. 
>- If we are keeping the file chroot directory, as per the requirement we should take care of read-write-execute permissions to the child process.
 
**2. Explain the mechanism you are using to pass file descriptors between parent and child processes?**

- We are opening the file passed in the command line arguments using the open() function to get the file descriptor. 
- We are sharing the file descriptors between parent and execed child process by passing a reference to the file descriptors as 
  command line arguments to the newly execed process.

**3. What happens if the file size of the disk file exceeds the size of the client’s hardcoded buffer?** 
Does the client have sufficient checks to ensure no buffer overruns occur?

- As per the file size, we are reading the file in chunks of client's hardcoded buffer size. 
- So irrespective of how big the file is, at any time client buffer will have bytes equal to or less than the size of buffer.
- We added checks to make sure that data read from server never exceeds the client's buffer size.