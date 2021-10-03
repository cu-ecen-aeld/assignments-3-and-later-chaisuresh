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

struct addrinfo hints;
struct addrinfo *servinfo;
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
  	 
  	if(close(fd) == -1) 
  	{
  		syslog(LOG_ERR,"close file unsuccessful\n");
  		
  	}
  	
  	
  	exit(1);
  	
  	
}


int main(int argc, char *argv[])
{
	
	char daem[5]= "-d";
	
	
	struct sockaddr sockaddr1;
	
	
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET ;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags= AI_PASSIVE;
	
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	
	 	 
  	 
  	 int ret=0, ret2=0, ret3=0, ret5=0;
  	 
  	 int addr2=1;
  	
  	 ret= socket( PF_INET  , SOCK_STREAM  , 0);  	 
  	 if(ret == -1)
  	 {
  	 	perror("socket connection failed");
  	 	return -1;
  	 	//exit(1);
  	 }
  	 
  	 if(setsockopt(ret, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &addr2, sizeof(addr2)) == -1)
  	 {
  	 	perror("socket reuse addr failed");
  	 	return -1;
  	 	
  	 } 
  	 
  	 
  	 ret2=getaddrinfo(NULL,"9000",&hints, &servinfo);
  	 if(ret2 != 0)
  	 {
  	 	perror("getaddrinfo failed");
  	 	return false;
  	 }
                       
                       
  	 
  	 
  	ret2= bind(ret,servinfo->ai_addr,servinfo->ai_addrlen);
  	if(ret2 == -1)
  	 {
  	 	perror("socket bind failed");
  	 	return false;
  	 	//exit(1);
  	 }
  	 
  	 freeaddrinfo(servinfo);
  	 
  	 if(strcmp(argv[0], daem) == 0)
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
    		if (setsid() < 0)
       	 exit(EXIT_FAILURE);

    		/* Catch, ignore and handle signals */
    		//TODO: Implement a working signal handler */
    		signal(SIGCHLD, SIG_IGN);
    		signal(SIGHUP, SIG_IGN);

    		  

    		/* Change the working directory to the root directory */
    		/* or another appropriated directory */
   		 if(chdir("/")==-1)
   		 	perror("chdir unsuccesful");
  		 int fd1;
  	 
  	 	 fd1 = open ("/dev/null", O_RDWR);
  			 if (fd1 != STDIN_FILENO)         /* 'fd' should be 0 */
            		 return -1;
        	if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
           		 return -1;
        	if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
          		  return -1;
            
            	close(STDIN_FILENO);
            	close(STDOUT_FILENO);
            	close(STDERR_FILENO);
            
  	 }
	
  	
  	
  	 ret3= listen(ret, 20);
  	  if(ret3 == -1)
  	 {
  	 	perror("socket listen failed");
  	 	return false;
  	 	//exit(1);
  	 }
  	
  	syslog(LOG_USER, "stuck here 5");
  	 socklen_t size= sizeof(sockaddr1);
  	 
  while(signal_bool==0)
 	{
 	
  	 
  	 ret4= accept(ret, &sockaddr1, &size );
  	 if(ret4 == -1)
  	 {
  	 	perror("socket accept failed");
  	 	return false;
  	 	//exit(1);
  	 }
  	 else syslog(LOG_USER, "Accepted connection from ");
  	 
  	 
  	 syslog(LOG_USER, "stuck here 6");
  	 
  	 
  	 buf = (char *)malloc(sizeof(char) * 200);
  	 
  	 
  	 int len=1, len2, len3;
  	 
  	  int nr,nr1;  
  	


  	 fd= open("/var/tmp/aesdsocketdata.txt", O_CREAT | O_RDWR | O_APPEND, 644);
  	   if(fd == -1)
  	  	perror("File create and open unsuccessful\n");
  	 
  	
  		 len= recv(ret4, buf, strlen(buf), MSG_DONTWAIT | MSG_ERRQUEUE);
  	 	 if(len == -1)
  	 	{
  	 		perror("receive failed");
  	 		return false;
  	 		//exit(1);
  	 	} 
  	 	
  	
  	 	
  	 	len2=1;
  	 	len3=0;
  	 	
  		while(len2!=0)
  		{
  	  		for(int i=len3; i<len; i++)
  	  		{
  	  			if(buf[i] == '\n')
  	  			{
  	  				len2=len-i;
  	  				len3=i;
  	  			}
  	  			else len2=0;
  	  		
  	  		}
  	  	
  			nr=write(fd, buf, len3);
			if(nr == -1) 
			{
				perror("File write unsuccessful\n");
				return false;
			}
			
		
		
		}			
			
	
	
  	 
  	 buf2 = (char *)malloc(sizeof(char) * 200);	
  	 
  	 len3= read(fd, buf2, 200);
  	  if(len3 == -1)
  	 {
  	  	perror("Read unsuccessful\n");
  	  	return false;  	  	
  	  }
	
  	
  	ret5= send(ret4, buf2, strlen(buf2), MSG_DONTWAIT | MSG_ERRQUEUE);
  	 if(ret5 == -1)
  	 {
  	  	perror("Send unsuccessful\n");
  	  	return false;  	  	
  	  }
  	
  	
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
  	
  	} 
  	 
  
   
	

}
