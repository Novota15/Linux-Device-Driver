#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
// #include<asm/uaccess.h>

MODULE_LICENSE("GPL");

#define BUFFER_SIZE 1024

static int loc_in_buffer = 0;
static int end_of_buffer = 1024;
static int MAJOR_NUMBER = 240;
char* DEVICE_NAME = "simple_char_driver";
typedef struct{
    char* buf;
}buffer;

char *device_buffer;
static int open_counter=0;
static int close_counter=0;

ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    printk("read called");
     if (*offset > BUFFER_SIZE)
     {
        printk("offset is greater than the buffer size");
        return 0;
     }
     if (*offset + length > BUFFER_SIZE)
     {
        length = BUFFER_SIZE - *offset;
     }
     if (copy_to_user(buffer, device_buffer + *offset, length) != 0)
    {
        return -EFAULT;
    }
    *offset += length;
    return length;
}

ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
    printk("write called")
    int bytes_to_copy;
    if (BUFFER_SIZE - 1 -*offset <= length)
    {
        bytes_to_copy= BUFFER_SIZE - 1 -*offset;
        printk("BUFFER_SIZE - 1 -*offset <= length");
    }
    else if (BUFFER_SIZE - 1 - *offset > length)
    {
        bytes_to_copy = length;
        printk("BUFFER_SIZE - 1 -*offset > length");
    }
    printk(KERN_INFO "writing to device\n");
    if (*offset + length > BUFFER_SIZE)
    {
        printk("can't write to device");
        return -1;
    }
    printk("copying from device");
    copy_from_user(device_buffer + *offset, buffer, bytes_to_copy);
    device_buffer[*offset + bytes_to_copy] = '\0';
    *offset += bytes_to_copy;
    return bytes_to_copy;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
    printk(KERN_ALERT"this device is now open");    
    open_counter++;
    printk(KERN_ALERT "this device has been opened %d times\n", open_counter);    
    return 0;
}

int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
    printk(KERN_ALERT"this device is now closed");  
    close_counter++;
    printk(KERN_ALERT "this device has been closed %d times\n", close_counter);   
    return 0;
}

loff_t simple_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
    printk(KERN_ALERT"now seeking");
    switch(whence){
        //SEEK_SET
        case 0:{ 
            if(offset<= end_of_buffer && offset >0){
                loc_in_buffer = offset;
                printk(KERN_ALERT"this is our location in the buffer: %d\n", loc_in_buffer);
                }
            else{
                printk(KERN_ALERT"you are attempting to go ouside the buffer");
                }       
            break;
        }
        //SEEK_CUR
        case 1:{
            if(((loc_in_buffer+offset)<= end_of_buffer)&&((loc_in_buffer+offset)>0)){
                loc_in_buffer = loc_in_buffer+offset;
                printk(KERN_ALERT" this is our location in the buffer: %d\n", loc_in_buffer);
                }
                else{
                    printk(KERN_ALERT"you are attempting to go ouside the buffer");
                    }   
                break;          
        }
        //SEEK_END
        case 2:{
            if((end_of_buffer-offset)>=0&& offset>0){
                loc_in_buffer = end_of_buffer-offset;
                printk(KERN_ALERT" this is our location in the buffer: %d\n", loc_in_buffer);
                }
                else{
                    printk(KERN_ALERT"you are attempting to go ouside the buffer");
                    }   
                break;
        }
        default:{

        }
    }
    printk(KERN_ALERT"I sought %d\n", whence);
    return loc_in_buffer;
}

struct file_operations simple_char_driver_file_operations = {
        .owner   = THIS_MODULE,
        .read = simple_char_driver_read,
        .write = simple_char_driver_write,
        .open = simple_char_driver_open,
        .llseek = &simple_char_driver_seek,
        .release = simple_char_driver_close,
};

static int simple_char_driver_init(void)
{   
    printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
    register_chrdev(MAJOR_NUMBER,DEVICE_NAME, &simple_char_driver_file_operations);
    device_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    return 0;
}

static void simple_char_driver_exit(void)
{
    printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
    unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME);
    kfree(device_buffer);
}

module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);