#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>


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

struct file_operations led_fops ={
	.owner 	= THIS_MODULE,
	.open 	= led_open,
	.release 	= led_close,
};


int __init led_init()
{
	printk("led init \n");

	int ret;
	ret = register_chrdev(500,"led-drv", &led_fops);
	if(ret < 0)
	{
		printk("register chrdev fail \n");
		return -1;
	}

	return 0;
}



void __exit led_exit()
{
	printk("led exit \n");
	unregister_chrdev(248,"led-drv");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MYM");
MODULE_DESCRIPTION("just a driver test,131xxxxx;qq:3455..");


