## UBUNTU 16.04
#### Server system-call list
- rt_sigreturn
- exit
- read
- write
- execve
- brk
- fstat
- open
- getpid
- access
- socket
- socketcall
- clone
- bind
- listen
- accept
- wait4
-exit_group

**CHILD SPECIFIC CALLS**

- mmap2
- munmap
- fstat64
- close
- mprotect
- set_thread_area
- prctl
- getcwd
- stat64
- mkdir
- chdir
- chroot
- setuid32
- getuid32

#### Client system-calls list
- rt_sigreturn
- exit
- read
- write
- fstat64
- socketcall
- exit_group
