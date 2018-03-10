#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>		// copy_to_user/copy_form_user
//arch/arm/include/asm/uaccess.h 
#include <linux/errno.h>


static dev_t dev_num;
static struct cdev *cdev_p;

int led_open(struct inode *inode, struct file *file)
{
	printk("led init ....\n");
	
	return 0;
}

int led_close(struct inode *inode, struct file *file)
{
	printk("led close ....\n");
	
	return 0;
}

char kbuf[100];
ssize_t led_write(struct file *file, char __user *buf, size_t count, loff_t *offset)
{

	//memcpy(kbuf,buf,count);
	if(copy_from_user(kbuf,buf,count))
	{
		printk("copy data form user fail\n");
		return -EFAULT;
	}
	
	return 0;
}
ssize_t led_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	if(copy_to_user(buf,kbuf,count))
	{
		printk("copy data to user fail\n");
		return -EFAULT;
	}
	return 0;
}
struct file_operations led_fops ={
	.owner 	= THIS_MODULE,
	.open 	= led_open,
	.release 	= led_close,
	.write 	= led_write,
	.read 	= led_read,
};

int __init led_init()
{
	printk("led init \n");

	int ret;

	ret = alloc_chrdev_region(&dev_num,0,1,"led-drv");
	if(ret < 0)
	{
		printk("all chrdev num fail\n");
		return -1;
	}
	printk("dev number =  %d ; major = %d ; minor = %d\n",dev_num,MAJOR(dev_num),MINOR(dev_num));
	
	cdev_p = cdev_alloc();
	cdev_init(cdev_p,&led_fops);
	
	ret = cdev_add(cdev_p,dev_num,1);
	if(ret)
	{
		printk("cdev add fail\n");
		return -1;
	}
	return 0;
}

void __exit led_exit()
{
	printk("led exit \n");

	cdev_del(cdev_p);
	unregister_chrdev_region(dev_num,1);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GEC210");
MODULE_DESCRIPTION("just a driver test,134xxxxx;qq:3455..");


