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

struct cdev *mcdev; 
int major_number;
int ret;
dev_t dev_num;

#define DEVICE_NAME "simple_drv"






int simple_open(struct inode *pinode, struct file *pfile)
{
        printk(KERN_INFO"simple open");
	if(down_interruptible(&virt_dev.sem) !=0)
	{
		printk(KERN_ALERT"simple_drv could not lock device");
		return 1;
	}
        return 0;
}

ssize_t simple_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_INFO"simple_read ");
	ret = copy_to_user(buffer,virt_dev.data,length);
	return 0;
}

ssize_t simple_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_INFO"simple_write %d ",(int)length);
	ret = copy_from_user(virt_dev.data,buffer,length);
	return ret;

}

int simple_close(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO"simple close release");
	up(&virt_dev.sem);
        return 0;
}

struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = simple_open,
	.read = simple_read,
	.write = simple_write,
	.release = simple_close,	
};


static int driver_entry(void)
{
        ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
        if(ret<0){
                        printk(KERN_ALERT"simmple_drv failed to alloc major number");
                        return ret;
        }
        major_number=MAJOR(dev_num);
        printk(KERN_INFO"simple_drv major %d",major_number);
        printk(KERN_INFO"simple_drv use mknod /dev/%s c %d 0",DEVICE_NAME,major_number);

        mcdev = cdev_alloc();
        mcdev->ops=&fops;
        mcdev->owner=THIS_MODULE;
        ret=cdev_add(mcdev,dev_num,1);
        if(ret<0)
        {
                 printk(KERN_ALERT"simple_drv unable to add cdev to kernel");
                 return ret;
        }

        sema_init(&virt_dev.sem,1);

        return 0;
}

static void driver_exit(void)
{
        cdev_del(mcdev);
        unregister_chrdev_region(dev_num,1);
        printk(KERN_ALERT"simple_drv unloaded module");
}



int __init hw_init(void) {
        printk(KERN_INFO"Hello World\n");

	// register with the kernel
	//              Major number
	register_chrdev(240,"Simple Chr drv",&fops);
	printk(KERN_INFO"Registered simple char driver");
        return 0;
}

void __exit hw_exit(void) {
        printk(KERN_INFO"Bye World, unregster \n");
	unregister_chrdev(240,"Simple Chr drv");
}

MODULE_LICENSE("GPL");

module_init(driver_entry);
module_exit(driver_exit);

//#module_init(hw_init);
//module_exit(hw_exit);
