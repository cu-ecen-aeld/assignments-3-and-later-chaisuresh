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
	
	
	
	memset(&hints,0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags= AI_PASSIVE;
	
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	
	 	 
  	 
  	 int ret=0, ret2=0, ret3=0, ret5=0;
  	 
  	 
  	 ret= socket( PF_INET | PF_INET6, SOCK_STREAM  , 0);  	 
  	 if(ret == -1)
  	 {
  	 	perror("socket connection failed");
  	 	return -1;
  	 	//exit(1);
  	 }
  	 
  	 
  	 ret2=getaddrinfo(NULL,"9000",&hints, &servinfo);
  	 if(ret2 == -1)
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
	if(daemon(0,0)== -1)
	perror("Daemon unsuccesful");
	syslog(LOG_USER, " Daemon successful");
  	
  	
  	 ret3= listen(ret, 50);
  	  if(ret3 == -1)
  	 {
  	 	perror("socket listen failed");
  	 	return false;
  	 	//exit(1);
  	 }
  	
  	
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
