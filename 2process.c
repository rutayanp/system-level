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
	//printf("I am the parent process pid : %d \n", ppid);
	
	pipe(fd);
	pid1 = fork(); //child A
	
	if(pid1 == -1){
		perror("fork");
		return 1 ;
	}
	
	if(pid1 == 0){ //body of the child process
        strcpy(string,"hello world (child)\n");		
        readbuffer = string;

	//printf("(child) process A with pid : %d\n",getpid()); //to get the pid of the child process
	//printf("(child) Address of the string : %p\n", string);// checks the address of the string in the child process	
	printf("(child)the string : %s \n", string);          //checks the string of the child process

	close(fd[0]);					//pipe descriptor set
	write(fd[1], &readbuffer, sizeof(readbuffer));	//address of the space where data needs to be written is send to the parent
        
        for(i = 0; i < 100000; i++); // this loop is there to make sure the child process runs while the data transfer.
        
	} else {
	//printf("(Parent) Parent Pid : %d \n (parent)the child pid : %d\n", getpid(), pid1); 
        strcpy(read1,"hello world (parent)\n");
        printf("(Parent)the string: %s \n", read1); //print the string before data is received from the remote		
        //printf("(parent) Address of the string : %p\n", read1);
        close(fd[1]);
	nbytes = read(fd[0], &string, sizeof(string));
	//printf("(Parent)The address of the rcvd buffer : %p\n",string); checks the address of the received buffer
       
        //printf("nbytes : %d\n",nbytes); //checks if we have received the address properly
        
	/* TO set up the variables for cross memory attach*/
	local[0].iov_base   = read1;
        local[0].iov_len    = strlen(read1)+1;
        remote[0].iov_base  = (void *)string;
        remote[0].iov_len   = 22;

        nread = process_vm_readv(pid1, local, 1, remote, 1, 0);
        int err1 = errno;
        printf("nread : %d  ; err no: %d, %s\n\n", nread, errno, strerror(err1));    //checks if the cross memory attach was done properly  
        printf("(Parent)the string: %s \n", read1); //Here we check if have the string from the child
    }

return 0;

}
