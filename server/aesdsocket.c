// Author- Chaithra Suresh
//
// References- https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
//	https://github.com/pasce/daemon-skeleton-linux-c/blob/master/daemonize.c
//


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


int ret4=0, fd=0, ret=0;
char *rec_buf, *read_buf;
int signal_bool=0;
char *ip;

void sig_handler(int signum)
{
	
	signal_bool=1;
	syslog(LOG_USER,"Caught signal exiting...");
	
	
	close(ret);
	if(ret4 != -1)
  	
  	if(close(ret4) == -1)
  	 {
  	  	perror("close socket unsuccessful\n");
  	  	  	  	
  	  } else syslog(LOG_USER, "Closed connection from %s",ip);
  	
  	 free(rec_buf);
  	 free(read_buf);
  	 
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
	
	//char daem[]= "-d";
	
	//openlog("aesdsocket", LOG_PID, LOG_USER);
	struct sockaddr sockaddr1;
	socklen_t size1;
	struct addrinfo hints;
	struct addrinfo *res;
	//struct stat st;
	int buf_size=500;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET ;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags= AI_PASSIVE;
	
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	 	 
  	 
  	 int ret3=0, ret5=0;
  	 
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
  	 	close(ret4);
  	 	close(ret);
  	 	return -1;
  	 	
  	 } 
  	 
  	 
  	 
  	 if(getaddrinfo(NULL,"9000",&hints, &res) != 0)
  	 {
  	 	perror("getaddrinfo failed");
  	 	close(ret4);
  	 	close(ret);
  	 	return false;
  	 }
                       
                       
  	 
  	 
  	
  	if(bind(ret,res->ai_addr,res->ai_addrlen) == -1)
  	 {
  	 	perror("socket bind failed");
  	 	close(ret4);
  	 	close(ret);
  	 	return false;
  	 	
  	 }
  	 
  	 freeaddrinfo(res);
  	 
  	 if(argc == 2)
  	 if(!strcmp( "-d", argv[1]))
  	 {
  	 
  	 	 pid_t pid;
  	 	 pid = fork();

    		
    		if (pid < 0)
        	exit(EXIT_FAILURE);
	
    		
    		if (pid > 0)
      		  exit(EXIT_SUCCESS);

    		
    		if(pid==0)
    		{
    		if (setsid() < 0)
    		{
    		close(ret4);
    		close(ret);
       	 exit(EXIT_FAILURE);
       	 }

    		
    		signal(SIGCHLD, SIG_IGN);
    		signal(SIGHUP, SIG_IGN);

    		  

   		 if(chdir("/")==-1)
   		 {
   		 	perror("chdir unsuccesful");
   		 	close(ret4);
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
	
  	
  	
  	 ret3= listen(ret, 10);
  	  if(ret3 == -1)
  	 {
  	 	perror("socket listen failed");
  	 	close(ret4);
  	 	close(ret);
  	 	return false;
  	 	//exit(1);
  	 }
  	
  	
  	size1= sizeof(sockaddr1);
  
	 
    int temp_size=buf_size;
    int total_buffer=0;
    
   	rec_buf = (char *)malloc(sizeof(char) * buf_size);
  	read_buf = (char *)malloc(sizeof(char) * buf_size);
  	if((rec_buf==NULL)| (read_buf==NULL))	
  	{
  		perror("malloc failed for buffers");
  		close(ret4);
  	 	close(ret);
  	 			
  	
  	}
  	 
     fd= open("/var/tmp/aesdsocketdata.txt", O_CREAT | O_RDWR | O_APPEND , 0644);
  	 if(fd == -1)
  	 {
  	  	perror("File create and open unsuccessful\n");
  	  	close(ret4);
  	  	close(ret);
  	 	free(rec_buf); 
  	 	free(read_buf); 		 	
  		close(fd);
  	  	return false;
  	 }
 	 
  while(signal_bool==0)
 	{
 	
  	 
  	 	
  	 
  	 
  	 
  	 
  	 int len=0, len2, len3=0;  	 
  	 int nr,nr1;   	
  	  	
  	  	
  	 ret4= accept(ret, &sockaddr1, &size1 );
  	 if(ret4 == -1)
  	 {
  	 	perror("socket accept failed");
  	 	close(ret4);
  	 	close(ret);
  	 	free(read_buf); 
  	 	close(fd);
  	 	free(rec_buf); 
  	 	return false;
  	 	
  	 	
  	 }
  	 else 
  	 {
  	 	struct sockaddr_in *addr_in = (struct sockaddr_in *)&sockaddr1;
  	 	ip = inet_ntoa(addr_in->sin_addr);
  	 	syslog(LOG_USER, "Accepted connection from %s",ip);
  	 	
  	 }
  	 
  	 
  	  
  	 
  	 
  	
  	int total=0;
  	
  		do
  		{
  		
  		 len= recv(ret4, rec_buf+total, buf_size, 0);
  	 	 if(len == -1)
  	 	{
  	 		perror("receive failed");
  	 		close(ret4);
  	 		close(ret);
  	 		free(rec_buf);  
  	 		free(read_buf); 		 	
  			close(fd);
  	 		return false;
  	 		
  	 	} 
  	 	 
  	 	total+=(len);
  	 	temp_size+= (buf_size);
  	 	
  	 	rec_buf=realloc(rec_buf, sizeof(char)*temp_size);
  	 	
  	 	
  	 	
  	 	}while(strchr(rec_buf, '\n') == NULL);	
  	 	
  	 	total_buffer+=total;
  	 	
  	 	rec_buf[total]='\0';
  	 	
  	 		nr=write(fd, rec_buf, total);
			if(nr == -1) 
			{
				perror("File write unsuccessful\n");
				close(ret4);
				close(ret);
  	 			free(rec_buf); 
  	 			free(read_buf);  		 	
  				close(fd);
				return false;
			}
  	 	
  	 	
  	 	
		
		
		
		syslog(LOG_USER, " received rec_buf = %s", rec_buf);
	
		

	
  	 
  	 
  	 read_buf= realloc(read_buf, sizeof(char)*total_buffer);
  	 	
  	 	lseek(fd, 0, SEEK_SET);
  	 
  	 	len3= read(fd, read_buf, total_buffer);
  	  	if(len3 == -1)
  	 	{
  	  		perror("Read unsuccessful\n");
  	  		close(ret4);
  	  		close(ret);
  	 		free(rec_buf);
  		 	free(read_buf);
  			close(fd);
  	  		return false;  	  	
  	  	}
  	  	
  	  	
  	  	
  	  	
  	
  	ret5= send(ret4, read_buf, len3, 0);
  	 if(ret5 == -1)
  	 {
  	  	perror("Send unsuccessful\n");
  	  	close(ret4);
  	  	close(ret);
  	 	free(rec_buf);
  		 free(read_buf);
  		close(fd);
  	  	return false;  	  	
  	  }
  	
  	
  	syslog(LOG_USER, " read rec_buf = %s", read_buf);
  	
  	ret5 = close(ret4);
  	if(ret5 == -1)
  	 {
  	  	perror("close socket unsuccessful\n");
  	  	
  	  	return(false);  	  	
  	  }else
  	  syslog(LOG_USER, "Closed connection from %s",ip); 
  	 
  
  	
  	} 
 
   //remove("/var/tmp/aesdsocketdata.txt");
}
