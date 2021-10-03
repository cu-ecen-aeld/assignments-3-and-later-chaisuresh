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


int ret4=0, fd;
char *rec_buf, *read_buf,*tmp_buf, *tmp_buf2;
int signal_bool=0;
char *ip;

void sig_handler(int signum)
{
	
	signal_bool=1;
	syslog(LOG_USER,"Caught signal exiting...");
	
	
	
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
	
	char daem[5]= "-d";
	
	openlog("aesdsocket", LOG_PID, LOG_USER);
	struct sockaddr sockaddr1;
	socklen_t size1;
	struct addrinfo hints;
	struct addrinfo *res;
	struct stat st;
	int buf_size=1200;
	
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
  	 	close(ret4);
  	 	return -1;
  	 	
  	 } 
  	 
  	 
  	 ret2=getaddrinfo(NULL,"9000",&hints, &res);
  	 if(ret2 != 0)
  	 {
  	 	perror("getaddrinfo failed");
  	 	close(ret4);
  	 	return false;
  	 }
                       
                       
  	 
  	 
  	ret2= bind(ret,res->ai_addr,res->ai_addrlen);
  	if(ret2 == -1)
  	 {
  	 	perror("socket bind failed");
  	 	close(ret4);
  	 	return false;
  	 	
  	 }
  	 
  	 freeaddrinfo(res);
  	 
  	 if(strcmp(argv[1], daem) == 0)
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
	
  	
  	
  	 ret3= listen(ret, 100);
  	  if(ret3 == -1)
  	 {
  	 	perror("socket listen failed");
  	 	close(ret4);
  	 	return false;
  	 	//exit(1);
  	 }
  	
  	
  	size1= sizeof(sockaddr1);
  
	 
   int total=0; 
   
 	 
  while(signal_bool==0)
 	{
 	
  	 
  	 	
  	 
  	 
  	 rec_buf = (char *)malloc(sizeof(char) * buf_size);
  	 
  	 
  	 int len=1, len2, len3;  	 
  	 int nr,nr1;   	
  	  	
  	  	
  	 ret4= accept(ret, &sockaddr1, &size1 );
  	 if(ret4 == -1)
  	 {
  	 	perror("socket accept failed");
  	 	close(ret4);
  	 	free(rec_buf); 
  	 	return false;
  	 	
  	 	
  	 }
  	 else 
  	 {
  	 	struct sockaddr_in *addr_in = (struct sockaddr_in *)&sockaddr1;
  	 	ip = inet_ntoa(addr_in->sin_addr);
  	 	syslog(LOG_USER, "Accepted connection from %s",ip);
  	 	
  	 }
  	 
  	 
  	    fd= open("/var/tmp/aesdsocketdata.txt", O_CREAT | O_RDWR | O_APPEND , 0644);
  	 if(fd == -1)
  	 {
  	  	perror("File create and open unsuccessful\n");
  	  	close(ret4);
  	 	free(rec_buf);  		 	
  		close(fd);
  	  	return false;
  	 }
  	 
  	 
  	//int temp_size=0;
  		do
  		{
  		 len= recv(ret4, rec_buf, strlen(rec_buf)-1, 0);
  	 	 if(len == -1)
  	 	{
  	 		perror("receive failed");
  	 		close(ret4);
  	 		free(rec_buf);  		 	
  			close(fd);
  	 		return false;
  	 		
  	 	} 
  	 	 else 
  	 	{
  	 	total+=(len);
  	 	//temp_size+= (strlen(rec_buf)-1);
  	 	
  	 	nr=write(fd, rec_buf, len);
			if(nr == -1) 
			{
				perror("File write unsuccessful\n");
				close(ret4);
  	 			free(rec_buf);  		 	
  				close(fd);
				return false;
			}
  	 	
  	 	
  	 	}
  	 	
  	 	}while(strchr(rec_buf, '\n') == NULL);	
  	 	
  	 	//tmp_buf2=realloc(rec_buf, sizeof(char)*);
  	 	//rec_buf=tmp_buf2;
  	 	
  	 		
  	 	
		
		rec_buf[total]='\0';
		
		//syslog(LOG_USER, " received rec_buf = %s", rec_buf);
	
	lseek(fd, 0, SEEK_SET);
  	 
  	 read_buf = (char *)malloc(sizeof(char) * buf_size);	
  	 
  	 int sent=0;
  	 while(sent< total)
  	 {
  	 
  	 	lseek(fd, sent, SEEK_SET);
  	 	
  	 	int read_len;
  	 	if((total-sent)<buf_size) read_len=total-sent;
  	 	else read_len=buf_size;
  	 	
  	 	len3= read(fd, read_buf, read_len);
  	  	if(len3 == -1)
  	 	{
  	  		perror("Read unsuccessful\n");
  	  		close(ret4);
  	 		free(rec_buf);
  		 	free(read_buf);
  			close(fd);
  	  		return false;  	  	
  	  	}
  	  	
  	  	
  	  	sent+=len3;	
  	  	
  	  	stat("/var/tmp/aesdsocketdata.txt", &st);
  	  	
  	  	tmp_buf= realloc(read_buf, sizeof(char)*st.st_size);
  	  	
		read_buf=tmp_buf;
  	
  	ret5= send(ret4, read_buf, len3, 0);
  	 if(ret5 == -1)
  	 {
  	  	perror("Send unsuccessful\n");
  	  	close(ret4);
  	 	free(rec_buf);
  		 free(read_buf);
  		close(fd);
  	  	return false;  	  	
  	  }
  	  }
  	
  	//syslog(LOG_USER, " read rec_buf = %s", read_buf);
  	
  	ret5 = close(ret4);
  	if(ret5 == -1)
  	 {
  	  	perror("close socket unsuccessful\n");
  	  	
  	  	return(false);  	  	
  	  }else
  	  syslog(LOG_USER, "Closed connection from %s",ip);
  	
  	
  	 free(rec_buf);
  	 free(read_buf);
  	
  	
  	
  	 
  	 	if(close(fd) == -1) 
  	{
  		syslog(LOG_ERR,"close file unsuccessful\n");
  		return false;
  	}
  	
  	closelog();
  	
  	} 
 
   remove("/var/tmp/aesdsocketdata.txt");
}
