#include<linux/module.h>    //needed for module_init and module_exit
#include<linux/kernel.h>    //needed for KERN_INFO
#include<linux/init.h>      //needed for the macros

#include <linux/fs.h> // char drv  
#include <linux/cdev.h>
#include <asm/uaccess.h>

struct fake_device
{
	char data[100];
	struct semaphore sem;
}virt_dev;

int simple_open(struct inode *pinode, struct file *pfile)
{
        printk(KERN_INFO"simple open");
        return 0;
}

ssize_t simple_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_INFO"simple_read ");
	return 0;
}

ssize_t simple_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_INFO"simple_write %d ",(int)length);
	return length;

}

int simple_close(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO"simple close release");
        return 0;
}

struct file_operations file_operat = 
{
	.owner = THIS_MODULE,
	.open = simple_open,
	.read = simple_read,
	.write = simple_write,
	.release = simple_close,	
};




int __init hw_init(void) {
        printk(KERN_INFO"Hello World\n");

	// register with the kernel
	//              Major number
	register_chrdev(240,"Simple Chr drv",&file_operat);
	printk(KERN_INFO"Registered simple char driver");
        return 0;
}

void __exit hw_exit(void) {
        printk(KERN_INFO"Bye World, unregster \n");
	unregister_chrdev(240,"Simple Chr drv");
}

MODULE_LICENSE("GPL");

module_init(hw_init);
module_exit(hw_exit);
