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

#define USE_AESD_CHAR_DEVICE 1

#ifdef USE_AESD_CHAR_DEVICE
#define FILENAME "/dev/aesdchar"
#else
#define FILENAME "/var/tmp/aesdsocketdata"
#endif

int ret4=0,  ret=0;
int signal_bool=0;
char *ip;
int buf_size=1000, timer_bufsize = 100;
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

                pthread_mutex_destroy(&mutex);
                
             
  		SLIST_FOREACH(datap,&head,entries)
		 {
		    if(datap->threadmain.thread_complete_success == true)
			    {
			      pthread_join(datap->threadmain.thread, NULL);
			    }
  		}
		
              
                while(!SLIST_EMPTY(&head))
  		{
    			datap = SLIST_FIRST(&head);
    			SLIST_REMOVE_HEAD(&head,entries);
    			free(datap);
  		}
 
#ifdef  USE_AESD_CHAR_DEVICE
#else 
	timer_delete(timerid);
#endif
	//remove(FILENAME);
	

}

void thread_to_send(void *threadparam)
{

	struct thread_data *threadlocal= (struct thread_data *) threadparam;
	
	 //signal(SIGINT, sig_handler);
       // signal(SIGTERM, sig_handler);
        
        threadlocal->thread_complete_success=0;
        
        int fd;
        
        
        
         int temp_size=buf_size, len3,ret5, len, nr;
    		
  
    	
    
        char *rec_buf = (char *)malloc(sizeof(char) * buf_size);
        if(rec_buf == NULL)
               		{
               		
               			perror("Malloc unsuccessful\n");
              					
      						free(rec_buf);
               	 			func_close(); 
                				
               		
               		}
        char *read_buf = (char *)malloc(sizeof(char) * buf_size);
        if(read_buf == NULL)
               		{
               		
               			perror("Malloc unsuccessful\n");
              					free(read_buf);
      						free(rec_buf);
               	 			func_close(); 
                				
               		
               		}
        
        if((rec_buf==NULL)| (read_buf==NULL))   
        {
                perror("malloc failed for buffers");
              func_close();
                free(read_buf);
      		free(rec_buf);
		
                                
        
        }
	
	int total=0;
	
	fd= open(FILENAME, O_CREAT | O_RDWR , 0666);
        if(fd == -1)
       {
              perror("File create and open unsuccessful\n");
              close(fd);
              func_close();
        }
        
        
        
                do
                {
                
                 len= recv(threadlocal->accept_fd, rec_buf+total, buf_size, 0);
                 if(len == -1)
                {
                        perror("receive failed");
                        free(read_buf);
      			free(rec_buf);
                        func_close();
                         close(fd);

                        
                } 
                 
                total+=(len);
                temp_size+= (buf_size);
                
                syslog(LOG_USER ," rec len = %d", len);
                
              rec_buf=realloc(rec_buf, sizeof(char)*(temp_size)); 
              if(rec_buf == NULL)
               		{
               		
               			perror("Malloc unsuccessful\n");
              					free(read_buf);
      						free(rec_buf);
               	 			func_close(); 
                				close(fd);
               		
               		}
               		
               		memset(rec_buf+total, 0, (temp_size-total));
                
                              
               //syslog(LOG_USER, " received rec_buf = %s", rec_buf);
                
                
                
                }while(strchr(rec_buf, '\n') == NULL);  
                
                total_buffer+=total;
                
                //rec_buf[total]='\0';
                
                
                pthread_mutex_lock(&mutex);
                
               if (sigprocmask(SIG_BLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("block failed");
               }
               
                //syslog(LOG_USER, " write rec_buf = %s", rec_buf);
                
                
        
               nr=write(fd, rec_buf, total);
                        if(nr == -1) 
                        {
                                perror("File write unsuccessful\n");
                                free(read_buf);
      				 free(rec_buf);
				close(fd);
                                func_close();
                                
                        }   
                
                syslog(LOG_USER ," write nr = %d", nr);
                
                 if (sigprocmask(SIG_UNBLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("unblock failed");
               }
                
                
                
                
        
                
		pthread_mutex_unlock(&mutex);
        
         
          
        
 
         
         
 	int i=0, j=0;
 	int sent_bytes= buf_size;
                
                pthread_mutex_lock(&mutex);
                
                 if (sigprocmask(SIG_BLOCK,&threadlocal->mask,NULL) == -1) {   
               perror("block failed");
               }
                
              
          
        	 /*read_buf= (char *)realloc(read_buf, (total_buffer+1));
        	 if(read_buf == NULL)
               		{
               		
               			perror("Malloc unsuccessful\n");
              					free(read_buf);
      						free(rec_buf);
               	 			func_close(); 
                				
               		
               		}*/
        	 
        	char one_byte;
        	
        	 lseek(fd,0,SEEK_SET);
        	
        	
               while((len3 = read(fd, &one_byte, sizeof(char)))>0)
               {
               
               	read_buf[i]= one_byte;
               	
               	
               	if(read_buf[i] == '\n')
               	{
               	
               		int packets= i- j + 1;
               		
               		syslog(LOG_USER, " here i= %d, j= %d", i, j);
               		
               		ret5= send(threadlocal->accept_fd, read_buf+j, packets, 0);
               		if(ret5 == -1)
         				{
              					perror("Send unsuccessful\n");
              					free(read_buf);
      						free(rec_buf);
               	 			func_close(); 
                				close(fd);
         
        				  }
        				  
        			j=i+1;
               	
               	
               	}
               
               	i++;
               	
               	if(i >=sent_bytes)
               	{
               	
               		sent_bytes+= buf_size;
               		
               		syslog(LOG_USER, "current realloc size = %d, current i %d", sent_bytes, i);
               		
               		read_buf= realloc(read_buf, sizeof(char) * sent_bytes);
               		if(read_buf == NULL)
               		{
               		
               			perror("Malloc unsuccessful\n");
              					free(read_buf);
      						free(rec_buf);
               	 			func_close(); 
               	 			close(fd);
                				
               		
               		}
               	
               	
               	}
               	
               
               
               
               
               }
               
               //syslog(LOG_USER, " read rec_buf = %s", read_buf);
               
      
          
          
        
       
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
          	close(fd);

                
                                 
          }else
          syslog(LOG_USER, "Closed connection from %s",ip);
          
          threadlocal->thread_complete_success=1;
          
          free(read_buf);
          free(rec_buf);
		close(fd);
		close(ret4);


}


#ifdef  USE_AESD_CHAR_DEVICE
#else 
void thread_for_timer( union sigval sigval)
{
	
	
	
	char timer_buf[timer_bufsize];
	
	time_t time_cur;
	time(&time_cur);
	
	
	struct tm *time_stamp;
	
	time_stamp= localtime(&time_cur);
	
	int time_buf_siz= strftime(timer_buf, timer_bufsize, "timestamp:%Y %b %a %H:%M:%S%n",time_stamp) ;
	if(time_buf_siz < 0 ) perror(" strftime failed");
	
	pthread_mutex_lock(&mutex);
	timestamp_len = time_buf_siz;
	
	//if( write(fd, timer_buf, time_buf_siz) == -1)
	//{
	//	perror("timetsamp write error");
	//	func_close();
	//}
	
	pthread_mutex_unlock(&mutex);

}
#endif

void sig_handler(int signum)
{
        
        signal_bool=1;
        syslog(LOG_USER,"Caught signal exiting...");
        
        
     
        
        shutdown(ret, SHUT_RDWR);
         

        
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
        
        //int deamon=0;
        
        //struct thread_data *threadmain;
        
        SLIST_INIT(&head); 
              
        
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
         
          pid_t pid;
          
            
        
         ret3= listen(ret, 10);
          if(ret3 == -1)
         {
                perror("socket listen failed");
               func_close();
                //exit(1);
         }
        
        
        size1= sizeof(sockaddr1);
  
         
  
   
         
         
         if(argc == 2)
         {
         if (!strcmp( "-d", argv[1]))
         {
         	
         	//deamon=1;
                
                 pid = fork();

                
                if (pid < 0)
                {
                func_close();
                return -1;
                }
        
                
                if (pid > 0)
                  exit(EXIT_SUCCESS);

                
                if(pid==0)
                {
                if (setsid() < 0)
                {
               	 close(ret4);
                	close(ret);
                	func_close();
                	return -1;
        	 	//exit(EXIT_FAILURE);
       	 }

                
                signal(SIGCHLD, SIG_IGN);
                signal(SIGHUP, SIG_IGN);

                  

                 if(chdir("/")==-1)
                 {
                        perror("chdir unsuccesful");
                        close(ret4);
                        func_close();
                        return -1;
                        //exit(EXIT_FAILURE);
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
         
         }
         
#ifdef  USE_AESD_CHAR_DEVICE
#else      
       if((deamon==0) || ( pid ==0))      
          {
         struct sigevent sev;
         
         memset(&sev,0,sizeof(struct sigevent));
          int clockid = CLOCK_MONOTONIC;
          
           sev.sigev_notify = SIGEV_THREAD;
   	  sev.sigev_notify_function = thread_for_timer;
   	  
   	  struct timespec starttime;
         
         if(timer_create(clockid,&sev,&timerid) != 0)
         {
         	perror("timer create failed");
         	//func_close();
         }
         
         if(clock_gettime(clockid,&starttime) == -1)
         {
         
         	perror("timer gettime failed");
         	//func_close();
         
         }
         
         struct itimerspec itimer;
    	itimer.it_interval.tv_sec = 10;
    	itimer.it_interval.tv_nsec = 1000000;
    	
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
         	//func_close();    		
    	}
         
         
         
       }
     
        
      
 #endif       
         
         
         
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
                
                	//timestamp_len=0;
                
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
        
       
        
         func_close(); 
         
       
         
                

   
   return 0;
}

