/**
 * @file aesdchar.c
 * @brief Functions and data related to the AESD char driver implementation
 *
 * Based on the implementation of the "scull" device driver, found in
 * Linux Device Drivers example code.
 *
 * @author Dan Walkes
 * @date 2019-10-22
 * @copyright Copyright (c) 2019
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h> // file_operations
#include <linux/uaccess.h>  
#include <linux/string.h>
#include <linux/slab.h> 

#include "aesdchar.h"
int aesd_major =   0; // use dynamic major
int aesd_minor =   0;

#include "aesd-circular-buffer.h"

MODULE_AUTHOR("Chaisuresh"); /** TODO: fill in your name **/
MODULE_LICENSE("Dual BSD/GPL");

struct aesd_dev aesd_device;

int aesd_open(struct inode *inode, struct file *filp)
{
	struct aesd_dev *dev;

	PDEBUG("open");
	/**
	 * TODO: handle open
	 */

	
	dev = container_of(inode->i_cdev, struct aesd_dev, cdev);
	filp->private_data = dev;
	return 0;
}

int aesd_release(struct inode *inode, struct file *filp)
{
	PDEBUG("release");
	/**
	 * TODO: handle release
	 */
	return 0;
}

ssize_t aesd_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
	ssize_t retval = 0;
	struct aesd_dev *dev = (struct aesd_dev *)filp->private_data;
	size_t read_bytes;
	size_t offset = 0;
	size_t temp_count = count;
	size_t cur_pos = 0;
	//int ret;

	struct aesd_buffer_entry *temp_buffer = NULL;



	PDEBUG("read %zu bytes with offset %lld",count,*f_pos);
	/**
	 * TODO: handle read
	 */

	retval = mutex_lock_interruptible(&dev->mutex);
	if(retval != 0)
	{
		return retval;
	}

	do
	{


		temp_buffer = aesd_circular_buffer_find_entry_offset_for_fpos(&dev->buff1, *f_pos, &offset);
		if(temp_buffer == NULL) 
		{
			mutex_unlock(&dev->mutex);
		}
		else 
		{
			read_bytes= temp_buffer->size - offset;
			if(read_bytes > count)
				read_bytes=count;

		}

		retval= copy_to_user( &buf[cur_pos], (temp_buffer->buffptr + offset), read_bytes );

		if(retval != 0 )
			return -EFAULT;

	
		cur_pos += (read_bytes - retval);
	      temp_count-= (read_bytes - retval);	

	}while(temp_count);

	*f_pos+= cur_pos;
	mutex_unlock(&dev->mutex);

	return cur_pos;
}

ssize_t aesd_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	ssize_t retval;
	size_t cur_pos = 0;

	struct aesd_dev *dev = (struct aesd_dev *)filp->private_data;

	unsigned long bytes_remaining;
	const char *bytes_temp;

	 PDEBUG("write %zu bytes with offset %lld",count,*f_pos);
        /**
         * TODO: handle write
         */

	retval = mutex_lock_interruptible(&dev->mutex);
        if(retval != 0)
        {
                return retval;
        }

	if(dev->buff_entry1.size == 0)
	{
		dev->buff_entry1.buffptr = kzalloc( count*sizeof(char), GFP_KERNEL);
		if(dev->buff_entry1.buffptr == NULL)
		{
			mutex_unlock(&dev->mutex);
			return -ENOMEM;
		}
	}else 
	{

		dev->buff_entry1.buffptr= krealloc(dev->buff_entry1.buffptr, (dev->buff_entry1.size + count), GFP_KERNEL);
		if(dev->buff_entry1.buffptr == NULL)
                {
                        mutex_unlock(&dev->mutex);
                        return -ENOMEM;
                }

	}

	cur_pos= dev->buff_entry1.size;
	dev->buff_entry1.size += count;

	bytes_remaining =  copy_from_user((void*)( dev->buff_entry1.buffptr + cur_pos) , buf, count);

	if(bytes_remaining != 0)
	{

		return -EFAULT;

	}

	cur_pos = count - bytes_remaining;
	dev->buff_entry1.size -= bytes_remaining;


	if( strchr(dev->buff_entry1.buffptr, '\n') != NULL)
	{
		
		bytes_temp = aesd_circular_buffer_add_entry( &(dev->buff1), &(dev->buff_entry1)); 
		if(bytes_temp != 0)
		{

			kfree(bytes_temp);

		}

		dev->buff_entry1.size=0;
		dev->buff_entry1.buffptr = NULL;

	}
	
	*f_pos=0;

	mutex_unlock(&dev->mutex);


	return count;
}
struct file_operations aesd_fops = {
	.owner =    THIS_MODULE,
	.read =     aesd_read,
	.write =    aesd_write,
	.open =     aesd_open,
	.release =  aesd_release,
};

static int aesd_setup_cdev(struct aesd_dev *dev)
{
	int err, devno = MKDEV(aesd_major, aesd_minor);

	cdev_init(&dev->cdev, &aesd_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &aesd_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	if (err) {
		printk(KERN_ERR "Error %d adding aesd cdev", err);
	}
	return err;
}



int aesd_init_module(void)
{
	dev_t dev = 0;
	int result;
	result = alloc_chrdev_region(&dev, aesd_minor, 1,
			"aesdchar");
	aesd_major = MAJOR(dev);
	if (result < 0) {
		printk(KERN_WARNING "Can't get major %d\n", aesd_major);
		return result;
	}
	memset(&aesd_device,0,sizeof(struct aesd_dev));

	/**
	 * TODO: initialize the AESD specific portion of the device
	 */

	mutex_init(&aesd_device.mutex);

	aesd_circular_buffer_init(&aesd_device.buff1);

	result = aesd_setup_cdev(&aesd_device);

	if( result ) {
		unregister_chrdev_region(dev, 1);
	}
	return result;

}

void aesd_cleanup_module(void)
{
	dev_t devno = MKDEV(aesd_major, aesd_minor);

	cdev_del(&aesd_device.cdev);

	/**
	 * TODO: cleanup AESD specific poritions here as necessary
	 */
	
	aesd_circular_buffer_deinit(&aesd_device.buff1);
	unregister_chrdev_region(devno, 1);
}



module_init(aesd_init_module);
module_exit(aesd_cleanup_module);
