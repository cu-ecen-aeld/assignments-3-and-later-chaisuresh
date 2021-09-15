// Author- Chaithra Suresh
// C code to replace writer.sh
// use tail -f /var/log/syslog | grep writer to see results


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <syslog.h>
#include <syslog.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	
	
	if( argc < 3 ) 
	{
	
     		 syslog(LOG_USER,"Too less arguments\n Expected arguments: 1. path to write 2. string to write \n");
     		 exit(0);
  	 }
  	 else if( argc > 3)
  	 {
  	 	 syslog(LOG_USER,"Too many arguments\n Expected arguments: 1. path to write 2. string to write \n");
  	 	exit(0);
  	 }
  	 
  	 char t[50], new[50];
  	 int keep=49, fd;
  	 ssize_t nr;
  	 
  	 strncpy(t, argv[1], strlen(argv[1]));
  	 
  	 for(int i=0; i< strlen(argv[1]); i++)
  	 {
  	 
  	 	//syslog(LOG_USER, " %c %d \n", t[i], keep);
  	 
  	 	if( t[i]== '/')
  	 	
  	 	keep =i;	 
  	 
  	  }
  	  
  	  
  	  
  	  if(keep==49)   	  
  	  {
  	  	syslog(LOG_ERR, " File Path incorrect\n");
  	  	exit(0);
  	  	
  	  }
  	  
  	 
  	 t[strlen(argv[1])] = '\0';
  	 
  	 //syslog(LOG_USER,"%s\n", t);
  	  	  
  	 fd= open(&t[0], O_CREAT | O_RDWR, 644);
  	   if(fd == -1)
  	  	syslog(LOG_ERR,"File create and open unsuccessful\n");
  	  	
  	
	nr=write(fd, argv[2], strlen(argv[2]));
	if(nr == -1) 
		syslog(LOG_ERR,"File write unsuccessful\n");
	
	syslog(LOG_DEBUG, "\n Writing %s to %s\n", argv[2], argv[1]); 
   
	

}
