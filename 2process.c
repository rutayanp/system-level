#include <stdio.h>
#include <sys/uio.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>

int main()
{
	int i, nbytes;	//nbytes to keep a count of the no. of bytes received
	int fd[2];	//file descriptor
	int pid1, ppid;
	char temp;	
	char string[80] = "hello world\n";
	char *readbuffer;
	char read1[80];
	int address;
    struct iovec local[1];
    struct iovec remote[1];
    ssize_t nread;
    pid_t pid = 10; 

	ppid = getpid();
	printf("I am the parent process pid : %d \n", ppid);
	
	pipe(fd);
	pid1 = fork(); //child A
	
	if(pid1 == -1){
		perror("fork");
		return 1 ;
	}
	
	if(pid1 == 0){ //body of the child process
        strcpy(string,"hello world (child)\n");		
        readbuffer = string;
		printf("(child) process A with pid : %d\n",getpid());
        printf("(child)the string : %s \n", string);
		printf("(child) Address of the string : %p\n", string);
		//printf("readbuffer[0] : %c\n", readbuffer[0]);
		close(fd[0]);
		write(fd[1], &readbuffer, sizeof(readbuffer));
        for(i = 0; i < 10000; i++);
        printf("(child)the string : %s \n", string);
        
	} else {
		printf("I am the parent and the child pid is : %d, %d\n", getpid(), pid1);
        strcpy(read1,"hello world (parent)\n");
        printf("(Parent)the string: %s \n", read1);		
        printf("(parent) Address of the string : %p\n", read1);
        close(fd[1]);
		nbytes = read(fd[0], &string, sizeof(string));
		printf("(Parent)The address of the rcvd buffer : %p\n",string);
       
        printf("nbytes : %d\n",nbytes);
        local[0].iov_base   = read1;
        local[0].iov_len    = strlen(read1)+1;
        remote[0].iov_base  = (void *)string;
        remote[0].iov_len   = 22;

        nread = process_vm_readv(pid1, local, 1, remote, 1, 0);
        int err1 = errno;
        printf("nread : %d  ; err no: %d, %s\n", nread, errno, strerror(err1));
        printf("(Parent)the string: %s \n", read1);
    }

return 0;

}
