#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>		// copy_to_user/copy_form_user
//arch/arm/include/asm/uaccess.h 
#include <linux/errno.h>
#include "myioctl.h"

#include <linux/ioport.h>
#include <linux/io.h>

struct resource *keytr = NULL;
unsigned long *vaddr = NULL;

struct resource *ledtr = NULL;
unsigned long *keyaddr = NULL;

static dev_t dev_num;
static struct cdev *cdev_p;

int led_open(struct inode *inode, struct file *file)
{
	printk("led init ....\n");
	/*GPJ2CON[3:0] = 0001*/
	//*vaddr &=~(0xF<<0);
	//*vaddr |=(0x1<<0);

	int val;
	val = ioread32(vaddr);
	val &=~(0xF<<0);
	val |=(0x1<<0);
	iowrite32(val,vaddr);

	/* GPJ2DAT[0] = 1 */
	//*(vaddr + 1) |=(0x1<<0);
	//iowrite32((ioread32(vaddr + 1) | (0x1<<0)),vaddr + 1)
	val = ioread32(vaddr+1);
	val |=(0x1<<0);
	iowrite32(val,vaddr+1);
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


char keybuf[100];
ssize_t key_write(struct file *file, char __user *buf, size_t count, loff_t *offset)
{

	//memcpy(kbuf,buf,count);
	if(copy_from_user(kbuf,buf,count))
	{
		printk("copy data form user fail\n");
		return -EFAULT;
	}
	
	return 0;
}
ssize_t key_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	if(copy_to_user(buf,kbuf,count))
	{
		printk("copy data to user fail\n");
		return -EFAULT;
	}
	return 0;
}


int led_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	printk("cmd = %d; arg = %d\n",cmd,arg);
	int val;
	if(_IOC_NR(cmd) >= MAGIC_MAX)
		return -1;
	if(_IOC_TYPE(cmd) != TYPE)
		return -1;
	
	switch(_IOC_NR(cmd)){
	case 0:
		printk("led on \n");
		//*(vaddr + 1) &=~(0x1<<0);
		val = ioread32(vaddr+1);
		val &=~(0x1<<0);
		iowrite32(val,vaddr+1);
		break;
	case 1:
		printk("led off\n");
		//*(vaddr + 1) |=(0x1<<0);
		val = ioread32(vaddr+1);
		val |=(0x1<<0);
		iowrite32(val,vaddr+1);
		break;		
	}	
	return 0;
}

struct file_operations led_fops ={
	.owner 	= THIS_MODULE,
	.open 	= led_open,
	.release 	= led_close,
	.write 	= led_write,
	.read 	= led_read,
	.kwrite 	= key_write,
	.kread 	= led_read,
	.ioctl	= led_ioctl,
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
	
	ptr = request_mem_region(0xe0200280,0x1000,"led-drv");
	if(ptr == NULL)
	{
		printk("request_mem_region\n");
		return -1;
	}
	
	vaddr = (unsigned long*)ioremap(0xe0200280,0x1000);
	if(vaddr == NULL)
	{
		printk("ioremap fail\n");
		return -1;
	}

	keytr = request_mem_region(0xe0200C40,0x1000,"key-drv");
	if(keytr == NULL)
	{
		printk("request_mem_region\n");
		return -1;
	}
	
	keyaddr = (unsigned long*)ioremap(0xe0200C40,0x1000);
	if(keyaddr == NULL)
	{
		printk("ioremap fail\n");
		return -1;
	}
	return 0;
}

void __exit led_exit()
{
	printk("led exit \n");

	
	iounmap(vaddr);
	release_mem_region(0xe0200280,0x1000);
	
	cdev_del(cdev_p);
	unregister_chrdev_region(dev_num,1);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MYM");
MODULE_DESCRIPTION("just a driver test,134xxxxx;qq:3455..");


