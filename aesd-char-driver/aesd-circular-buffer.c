/**
 * @file aesd-circular-buffer.c
 * @brief Functions and data related to a circular buffer imlementation
 *
 * @author Dan Walkes
 * @date 2020-03-01
 * @copyright Copyright (c) 2020
 *
 */

 

#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/slab.h>
#else
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#endif

#include "aesd-circular-buffer.h"

/**
 * @param buffer the buffer to search for corresponding offset.  Any necessary locking must be performed by caller.
 * @param char_offset the position to search for in the buffer list, describing the zero referenced
 *      character index if all buffer strings were concatenated end to end
 * @param entry_offset_byte_rtn is a pointer specifying a location to store the byte of the returned aesd_buffer_entry
 *      buffptr member corresponding to char_offset.  This value is only set when a matching char_offset is found
 *      in aesd_buffer. 
 * @return the struct aesd_buffer_entry structure representing the position described by char_offset, or
 * NULL if this position is not available in the buffer (not enough data is written).
 */
struct aesd_buffer_entry *aesd_circular_buffer_find_entry_offset_for_fpos(struct aesd_circular_buffer *buffer,
			size_t char_offset, size_t *entry_offset_byte_rtn )
{
    /**
    * TODO: implement per description
    */
    
    
    
    uint8_t i=buffer->out_offs;    
    int count=buffer->entry[i].size, count2=0;
     
     int mark=0; 

     if(buffer == NULL ) return NULL;
     
     
    
    while((count-1) < char_offset)
    {
    		
    		count2+= buffer->entry[i].size;   		
    		
    		i= (i +1) % AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED ; 		
    		count+= buffer->entry[i].size; 
    		if( (i ==  buffer->out_offs)) return NULL;
    		
    }
    
    //i=i-1;
    
    mark=   char_offset- count2;
 	*entry_offset_byte_rtn = mark;  
       	
   
   	
    return (&buffer->entry[i]);
}

/**
* Adds entry @param add_entry to @param buffer in the location specified in buffer->in_offs.
* If the buffer was already full, overwrites the oldest entry and advances buffer->out_offs to the
* new start location.
* Any necessary locking must be handled by the caller
* Any memory referenced in @param add_entry must be allocated by and/or must have a lifetime managed by the caller.
*/
const char *aesd_circular_buffer_add_entry(struct aesd_circular_buffer *buffer, const struct aesd_buffer_entry *add_entry)
{
    /**
    * TODO: implement per description 
    */
    
    const char *modif = NULL;
    
    if(buffer == NULL ) return NULL ;
    if(add_entry == NULL ) return NULL; 
    
   
 
    if(buffer->in_offs == buffer->out_offs)
    {
    if(buffer->len > 0 )
    buffer->full= 1;
    }else buffer->full=0;
   	
   	
    
    if(buffer->full == 1)
    {
    
    	modif = buffer->entry[buffer->out_offs].buffptr;
    	
	

    }
  
    buffer->entry[buffer->in_offs]= *add_entry;
    buffer->in_offs=(buffer->in_offs+ 1)% AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED;
    
     
    if(buffer->full == 1)
    {
    
    	    	buffer->out_offs=  (buffer->out_offs+ 1) % AESDCHAR_MAX_WRITE_OPERATIONS_SUPPORTED;
	

    }
    
     buffer->len++;

     return modif;
    
        
}

/**
* Initializes the circular buffer described by @param buffer to an empty struct
*/
void aesd_circular_buffer_init(struct aesd_circular_buffer *buffer)
{
    memset(buffer,0,sizeof(struct aesd_circular_buffer));
    buffer->len=0;

}

void aesd_circular_buffer_deinit(struct aesd_circular_buffer *buffer)
{
	
	int i;
	struct aesd_buffer_entry *entry;

	AESD_CIRCULAR_BUFFER_FOREACH(entry, buffer, i)
	{

		if(entry->buffptr != NULL)
		{
			
#ifdef _KERNEL_
			kfree(entry->buffptr);
#else 	
		//	free((void *)entry->buffptr);
#endif



		}

	}


}
