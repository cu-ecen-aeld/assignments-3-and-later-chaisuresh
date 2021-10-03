// Author- Chaithra Suresh


#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <syslog.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
 #include <netdb.h>
 #include <string.h>
 #include <signal.h>
 #include <errno.h>
 #include <sys/stat.h>
 #include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <ctype.h>


int ret4=0, fd;
char *buf, *buf2;
int signal_bool=0;

void sig_handler(int signum)
{
	
	signal_bool=1;
	syslog(LOG_USER,"Caught signal exiting...");
	
	
	
	if(ret4 != -1)
  	
  	if(close(ret4) == -1)
  	 {
  	  	perror("close socket unsuccessful\n");
  	  	  	  	
  	  }
  	
  	 free(buf);
  	 free(buf2);
  	 
  	 remove("/var/tmp/aesdsocketdata.txt");
  	 
  	if(close(fd) == -1) 
  	{
  		syslog(LOG_ERR,"close file unsuccessful\n");
  		
  	}
  	
  	closelog();
  	
  	exit(1);
  	
  	
}


int main(int argc, char *argv[])
{
	
	char daem[5]= "-d";
	
	openlog("aesdsocket", LOG_PID, LOG_USER);
	struct sockaddr sockaddr1;
	socklen_t size1;
	struct addrinfo hints;
	struct addrinfo *res;
	
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET ;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags= AI_PASSIVE;
	
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	 	 
  	 
  	 int ret=0, ret2=0, ret3=0, ret5=0;
  	 
  	 int addr2=1;
  	
  	 ret= socket( PF_INET, SOCK_STREAM, 0);  	 
  	 if(ret == -1)
  	 {
  	 	perror("socket connection failed");
  	 	return -1;
  	 	//exit(1);
  	 }
  	 
  	 if(setsockopt(ret, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &addr2, sizeof(int)) == -1)
  	 {
  	 	perror("socket reuse addr failed");
  	 	return -1;
  	 	
  	 } 
  	 
  	 
  	 ret2=getaddrinfo(NULL,"9000",&hints, &res);
  	 if(ret2 != 0)
  	 {
  	 	perror("getaddrinfo failed");
  	 	return false;
  	 }
                       
                       
  	 
  	 
  	ret2= bind(ret,res->ai_addr,res->ai_addrlen);
  	if(ret2 == -1)
  	 {
  	 	perror("socket bind failed");
  	 	return false;
  	 	//exit(1);
  	 }
  	 
  	 freeaddrinfo(res);
  	 
  	 if(strcmp(argv[1], daem) == 0)
  	 {
  	 
  	 	 pid_t pid;
  	 	 

    		/* Fork off the parent process */
   		 pid = fork();

    		/* An error occurred */
    		if (pid < 0)
        	exit(EXIT_FAILURE);
	
    		/* Success: Let the parent terminate */
    		if (pid > 0)
      		  exit(EXIT_SUCCESS);

    		/* On success: The child process becomes session leader */
    		if(pid==0)
    		{
    		if (setsid() < 0)
       	 exit(EXIT_FAILURE);

    		/* Catch, ignore and handle signals */
    		//TODO: Implement a working signal handler */
    		signal(SIGCHLD, SIG_IGN);
    		signal(SIGHUP, SIG_IGN);

    		  

    		/* Change the working directory to the root directory */
    		/* or another appropriated directory */
   		 if(chdir("/")==-1)
   		 {
   		 	perror("chdir unsuccesful");
   		 	exit(EXIT_FAILURE);
   		 	}
   		 	
  		 int fd1;
  	 
  	 	 fd1 = open ("/dev/null", O_RDWR);
  		dup2(fd1, STDIN_FILENO) ;           		
        	dup2(fd1, STDOUT_FILENO) ;
        	dup2(fd1, STDERR_FILENO) ;
            	close(STDIN_FILENO);
            	close(STDOUT_FILENO);
            	close(STDERR_FILENO);
            }
  	 }
	
  	
  	
  	 ret3= listen(ret, 100);
  	  if(ret3 == -1)
  	 {
  	 	perror("socket listen failed");
  	 	return false;
  	 	//exit(1);
  	 }
  	
  	
  	size1= sizeof(sockaddr1);
  
	 
   int total=0; 
 	 
  while(signal_bool==0)
 	{
 	
  	 
  	 	
  	 
  	 
  	 buf = (char *)malloc(sizeof(char) * 8192);
  	 
  	 
  	 int len=1, len2, len3;  	 
  	 int nr,nr1;   	
  	  	
  	  	
  	 ret4= accept(ret, &sockaddr1, &size1 );
  	 if(ret4 == -1)
  	 {
  	 	perror("socket accept failed");
  	 	return false;
  	 	
  	 }
  	 else syslog(LOG_USER, "Accepted connection from ");
  	 
  	 
  	    fd= open("/var/tmp/aesdsocketdata.txt", O_CREAT | O_RDWR | O_APPEND , 0644);
  	 if(fd == -1)
  	 {
  	  	perror("File create and open unsuccessful\n");
  	  	return false;
  	 }
  	 
  	 
  	
  		do
  		{
  		 len= recv(ret4, buf, strlen(buf)-1, 0);
  	 	 if(len == -1)
  	 	{
  	 		perror("receive failed");
  	 		return false;
  	 		
  	 	} 
  	 	 else 
  	 	{
  	 	total+=(len);
  	 	
  	 	nr=write(fd, buf, len);
			if(nr == -1) 
			{
				perror("File write unsuccessful\n");
				return false;
			}
  	 	}
  	 	}while(strchr(buf, '\n') == NULL);		
		
		buf[total]='\0';
		
		syslog(LOG_USER, " received buf = %s", buf);
	
	lseek(fd, 0, SEEK_SET);
  	 
  	 buf2 = (char *)malloc(sizeof(char) * 8192);	
  	 
  	 int sent=0;
  	 while(sent< total)
  	 {
  	 
  	 	lseek(fd, sent, SEEK_SET);
  	 	
  	 	int read_len;
  	 	if((total-sent)<400) read_len=total-sent;
  	 	else read_len=400;
  	 	
  	 	len3= read(fd, buf2, read_len);
  	  	if(len3 == -1)
  	 	{
  	  		perror("Read unsuccessful\n");
  	  		return false;  	  	
  	  	}
  	  	
  	  	
  	  	sent+=len3;	
  	  
  	  
	
  	
  	ret5= send(ret4, buf2, len3, 0);
  	 if(ret5 == -1)
  	 {
  	  	perror("Send unsuccessful\n");
  	  	return false;  	  	
  	  }
  	  }
  	
  	syslog(LOG_USER, " read buf = %s", buf2);
  	
  	ret5 = close(ret4);
  	if(ret5 == -1)
  	 {
  	  	perror("close socket unsuccessful\n");
  	  	return(false);  	  	
  	  }
  	
  	 free(buf);
  	 free(buf2);
  	
  	 
  	 	if(close(fd) == -1) 
  	{
  		syslog(LOG_ERR,"close file unsuccessful\n");
  		return false;
  	}
  	
  	closelog();
  	
  	} 
 
   remove("/var/tmp/aesdsocketdata.txt");
}
