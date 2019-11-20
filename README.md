# CMPE-279
## Assignments for CMPE 279 class
### Group
> **Karthik Munipalle (013854867)** _and_
> **Karthika Arumugam (011484460)**

#### Assignment-1
- Privilege seperation using `fork()` call.
- Privilege dropping using `setuid()` call.

#### Assignment-2
- fork() + exec() to add address space randomization.
- using `fork()` for child process.
- Then `setuid()` to drop provileges then using `execl()` for Address Space Randomization.

#### Assignment-3
- modified server and client codes to accept port number as command-line argument
- Server program accepts two command-line arguments port and file-name.
- Sent the socket and text file descriptors using `execl()` to the newly spawned process.
- Checks to prevent buffer from overrunning in client used `sizeof(buffer)` to achieve this.
- `chdir()` to an empty directory and `chroot()` to make this directory a fake root directory.
- File contents were still accessible over the client.

#### Assignment -4
- Used capabilities API to add restrictions to the server file.
- Capabilities API: **SECCOMP-BPF**
- Dependencies: `libseccomp` To install: `sudo apt-get install libseccomp-dev`
- Easier to use over `SELinux` and `AppArmor`
- Enforced various rules using `secomp-bpf` to restrict the functionality to only the required system calls.
- How to compile: `gcc server.c -o server -lseccomp`
- How to trace system-calls used: `sudo strace ./server (command line args)` **OR/AND** `sudo strace -f ./server (command line args)`


