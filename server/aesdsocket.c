// Author- Chaithra Suresh
//
// References- https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
//      https://github.com/pasce/daemon-skeleton-linux-c/blob/master/daemonize.c
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
#include <sys/time.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>

#include "queue.h"


int ret4=0, fd=0, ret=0;
int signal_bool=0;
char *ip;
int buf_size=500, timer_bufsize = 100;
int total_buffer=0;
int timestamp_len=0;

timer_t timerid;

struct thread_data{

 	pthread_t thread;
 	int accept_fd;
 	sigset_t mask;
 	bool thread_complete_success;
};


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct slist_data_s slist_data_t;
struct slist_data_s {
    struct thread_data threadmain;
    SLIST_ENTRY(slist_data_s) entries;
};

slist_data_t *datap=NULL;
SLIST_HEAD(slisthead, slist_data_s) head;
	

void func_close()
{


		close(ret4);
                close(ret);
                //free(read_buf); 
                close(fd);
                
                pthread_mutex_destroy(&mutex);
               // free(rec_buf); 
                //return false;
                
               while(!SLIST_EMPTY(&head))
    		{
        		datap = SLIST_FIRST(&head);
        		SLIST_REMOVE_HEAD(&head,entries);
        		free(datap);
    		}
    	
                SLIST_FOREACH(datap,&head,entries)
    		{
        		if (datap->threadmain.thread_complete_success != true)
			{
 		       	pthread_cancel(datap->threadmain.thread);
            			
       		}
     		}
	
	
	remove("/var/tmp/aesdsocketdata.txt");
	timer_delete(timerid);

}

void thread_to_send(void *threadparam)
{

	struct thread_data *threadlocal= (struct thread_data *) threadparam;
	
	 //signal(SIGINT, sig_handler);
       // signal(SIGTERM, sig_handler);
        
        threadlocal->thread_complete_success=0;
        
         int temp_size=buf_size, len3,ret5, len, nr;
    		
    	//sigset_t mask;
    	
    	//if(sigemptyset(&mask) == -1)
    	//perror("creating empty signal set failed");
    	
    	//if((sigaddset(&mask, SIGINT) == -1) |  (sigaddset(&mask, SIGTERM) == -1))
    	//perror("addint signals to set failed");
    	
    
        char *rec_buf = (char *)malloc(sizeof(char) * buf_size);
        char *read_buf = (char *)malloc(sizeof(char) * buf_size);
        
        if((rec_buf==NULL)| (read_buf==NULL))   
        {
                perror("malloc failed for buffers");
               func_close();
                free(read_buf);
      		free(rec_buf);

                                
        
        }
	
	int total=0;
        
                do
                {
                
                 len= recv(threadlocal->accept_fd, rec_buf+total, buf_size, 0);
                 if(len == -1)
                {
                        perror("receive failed");
                        func_close();
                         free(read_buf);
      			free(rec_buf);

                        
                } 
                 
                total+=(len);
                temp_size+= (buf_size);
                
              rec_buf=realloc(rec_buf, sizeof(char)*(temp_size)); 
                
                              
               
                
                
                
                }while(strchr(rec_buf, '\n') == NULL);  
                
                total_buffer+=total;
                
                rec_buf[total]='\0';
                
                
                pthread_mutex_lock(&mutex);
                
               if (sigprocmask(SIG_BLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("block failed");
               }
               
                
               nr=write(fd, rec_buf, total);
                        if(nr == -1) 
                        {
                                perror("File write unsuccessful\n");
                                func_close();
                                free(read_buf);
      				 free(rec_buf);

                        }   
                
                
                 if (sigprocmask(SIG_UNBLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("unblock failed");
               }
                
                
                
                syslog(LOG_USER, " received rec_buf = %s", rec_buf);
        
                
		pthread_mutex_unlock(&mutex);
        
         
         
         read_buf= realloc(read_buf, sizeof(char)*(total_buffer+timestamp_len));
        
        // lseek(fd, 0, SEEK_SET);
         
         int sent=0;
         //while(sent< total_buffer)
        // {
         
                //lseek(fd, sent, SEEK_SET);
                
                //int read_len;
               // if((total_buffer-sent)<buf_size) read_len=total_buffer-sent;
                //else read_len=buf_size;
                
                
                pthread_mutex_lock(&mutex);
                
                 if (sigprocmask(SIG_BLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("block failed");
               }
                
                lseek(fd, 0, SEEK_SET);
         
                len3= read(fd, read_buf, total_buffer+timestamp_len);
                if(len3 == -1)
                {
                        perror("Read unsuccessful\n");
                       func_close();   
                       free(read_buf);
         		free(rec_buf);
        
                }
                
                
                
                sent+=len3;   
                
                //stat("/var/tmp/aesdsocketdata.txt", &st);
                
                
        
        ret5= send(threadlocal->accept_fd, read_buf, len3+timestamp_len, 0);
         if(ret5 == -1)
         {
                perror("Send unsuccessful\n");
                func_close(); 
                free(read_buf);
          	free(rec_buf);
         
          }
          
          
        
       syslog(LOG_USER, " read rec_buf = %s", read_buf);
        
        pthread_mutex_unlock(&mutex);
        
         if (sigprocmask(SIG_UNBLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("unblock failed");
               }
        
       ret5 = close(threadlocal->accept_fd);
        if(ret5 == -1)
         {
                perror("close socket unsuccessful\n");
                free(read_buf);
          	free(rec_buf);

                
                                 
          }else
          syslog(LOG_USER, "Closed connection from %s",ip);
          
          threadlocal->thread_complete_success=1;
          
          free(read_buf);
          free(rec_buf);


}

void thread_for_timer( union sigval sigval)
{
	
	pthread_mutex_lock(&mutex);
	
	char timer_buf[timer_bufsize];
	
	time_t time_cur;
	time(&time_cur);
	
	
	struct tm *time_stamp;
	
	time_stamp= localtime(&time_cur);
	
	int time_buf_siz= strftime(timer_buf, timer_bufsize, "timestamp:%a, %d %b %Y %T %z\n",time_stamp) ;
	if(time_buf_siz < 0 ) perror(" strftime failed");
	
	
	timestamp_len = time_buf_siz;
	
	if( write(fd, timer_buf, time_buf_siz) == -1)
	{
		perror("timetsamp write error");
		func_close();
	}
	
	pthread_mutex_unlock(&mutex);

}


void sig_handler(int signum)
{
        
        signal_bool=1;
        syslog(LOG_USER,"Caught signal exiting...");
        
        
        close(ret);
        
        shutdown(ret4, SHUT_RDWR);
         
         remove("/var/tmp/aesdsocketdata.txt");
         
         func_close();       
     
        
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
        
        //struct thread_data *threadmain;
        
        
              
        
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET ;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags= AI_PASSIVE;
        
        signal(SIGINT, sig_handler);
        signal(SIGTERM, sig_handler);
                 
         sigset_t mask;
    	
    	if(sigemptyset(&mask) == -1)
    	perror("creating empty signal set failed");
    	
    	if((sigaddset(&mask, SIGINT) == -1) |  (sigaddset(&mask, SIGTERM) == -1))
    		perror("addint signals to set failed");
    		
         int ret3=0;
         
         int addr2=1;
        
         ret= socket( PF_INET, SOCK_STREAM, 0);          
         if(ret == -1)
         {
                perror("socket connection failed");
               func_close();
         }
         
         if(setsockopt(ret, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &addr2, sizeof(int)) == -1)
         {
                perror("socket reuse addr failed");
                func_close();
                
         } 
         
         
         
         if(getaddrinfo(NULL,"9000",&hints, &res) != 0)
         {
                perror("getaddrinfo failed");
                func_close();
         }
                       
                       
         
         
        
        if(bind(ret,res->ai_addr,res->ai_addrlen) == -1)
         {
                perror("socket bind failed");
                func_close();
                
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
               func_close();
                //exit(1);
         }
        
        
        size1= sizeof(sockaddr1);
  
         
   
         
     fd= open("/var/tmp/aesdsocketdata.txt", O_CREAT | O_RDWR | O_APPEND , 0644);
         if(fd == -1)
         {
                perror("File create and open unsuccessful\n");
               func_close();
         }
         
         
         
         struct sigevent sev;
         
         memset(&sev,0,sizeof(struct sigevent));
          int clockid = CLOCK_MONOTONIC;
          
           sev.sigev_notify = SIGEV_THREAD;
   	  sev.sigev_notify_function = thread_for_timer;
   	  
   	  struct timespec starttime;
         
         if(timer_create(clockid,&sev,&timerid) != 0)
         {
         	perror("timer create failed");
         	func_close();
         }
         
         if(clock_gettime(clockid,&starttime) == -1)
         {
         
         	perror("timer gettime failed");
         	func_close();
         
         }
         
         struct itimerspec itimer;
    	itimer.it_interval.tv_sec = 10;
    	itimer.it_interval.tv_nsec = 0;
    	
    	itimer.it_value.tv_sec = starttime.tv_sec + itimer.it_interval.tv_sec;
    	itimer.it_value.tv_nsec = starttime.tv_nsec + itimer.it_interval.tv_nsec;
    	
    	if( itimer.it_value.tv_nsec > 1000000000L )
       {
       	itimer.it_value.tv_nsec -= 1000000000L;
        	itimer.it_value.tv_sec ++;
    	}
    	
    	if(timer_settime(timerid, TIMER_ABSTIME, &itimer, NULL) != 0)
    	{
    		perror("timer settime failed");
         	func_close();
    		
    	}
         
         
         SLIST_INIT(&head); 
         
  while(signal_bool==0)
        {
        
         //int len=0, len2, len3=0;        
             
	
                
         ret4= accept(ret, &sockaddr1, &size1 );
         if(ret4 == -1)
         {
                perror("socket accept failed");
                func_close();
                
                
         }
         else 
         {
                struct sockaddr_in *addr_in = (struct sockaddr_in *)&sockaddr1;
                ip = inet_ntoa(addr_in->sin_addr);
                syslog(LOG_USER, "Accepted connection from %s",ip);
                
                
                
                	datap = (slist_data_t*)malloc(sizeof(slist_data_t));
   			SLIST_INSERT_HEAD(&head,datap,entries);
   			datap->threadmain.accept_fd = ret4;
   	             	datap->threadmain.thread_complete_success=0;
        		datap->threadmain.mask=mask;
                	
                	
                	if(pthread_create(&(datap->threadmain.thread), 0, (void *)&thread_to_send ,&(datap->threadmain)) != 0)
                	{
                		perror("pthread failed");
                		func_close();
                	}
                	
                	SLIST_FOREACH(datap,&head,entries)
                	{
                	
                		if(datap->threadmain.thread_complete_success == 1)
                		pthread_join(datap->threadmain.thread, NULL);
                	
                	
                	
                	}
                
                
                       
               
                
         }         
         
          
         
        } 
        
        
        
         
         
                
        //closelog();
        
       // } 
 
   //remove("/var/tmp/aesdsocketdata.txt");
}

