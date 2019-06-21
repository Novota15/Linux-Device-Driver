#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<asm/uaccess.h>
// #include<linux/uaccess.h> 

#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */
#define DEVICE_NAME "simplechardev"
static char device_buffer[BUFFER_SIZE];
// open/close counts
int open_count = 0;
int close_count = 0;


ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */

	int dev_buff_size = strlen(device_buffer);
	// if nothing in the device_buffer, return
	if (dev_buff_size == 0) {
		printk(KERN_ALERT "simplechardev buffer is empty.\n");
		return 0;
	}
	// copy data from offset to end of buffer
	printk(KERN_ALERT "reading from simplechardev.\n");
	copy_to_user(buffer, device_buffer, dev_buff_size);
	// print # of bytes in device buffer
	printk(KERN_ALERT "number of bytes read by simplechardev: %d \n", dev_buff_size);

	return 0;
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	int dev_buff_size = strlen(device_buffer);
	// if user space buffer empty, return 0
	if (length == 0) {
		return 0;
	}
	// set offset to current size of the dev buffer so data isn't overwritten
	*offset = dev_buff_size;
	// write from user space buffer to device buffer
	printk(KERN_ALERT "writing to simplechardev.\n");
	copy_from_user(device_buffer + *offset, buffer, length);
	// print # of bytes written from user
	printk(KERN_ALERT "simplechardev wrote %zu bytes.\n", strlen(buffer));
	
	return length;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	open_count++;
	printk(KERN_ALERT "simplechardev is open.");

	printk(KERN_ALERT "It has been opened %d times.", open_count);

	return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	close_count++;
	printk(KERN_ALERT "simplechardev is closed.");
	
	printk(KERN_ALERT "It has been closed %d times.", close_count);
	
	return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	/* Update open file position according to the values of offset and whence */
	return 0;
}

struct file_operations simple_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
	.read = simple_char_driver_read,
    .write = simple_char_driver_write,
    .open = simple_char_driver_open,
    .release = simple_char_driver_close,
    .llseek = simple_char_driver_seek,
};

static int simple_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	printk(KERN_ALERT "%s function called\n",__FUNCTION__);
	/* register the device */
	printk(KERN_ALERT "Within %s function\n",__FUNCTION__);
	register_chrdev(240, "simple_character_device", &simple_char_driver_file_operations);
	return 0;
}

static void simple_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
	printk(KERN_ALERT "%s function called\n",__FUNCTION__);
	/* unregister  the device using the register_chrdev() function. */
	unregister_chrdev(240, "simple_character_device");
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);