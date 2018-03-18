#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "myioctl.h"
int key_val;

int main(int argc,char **argv)
{
	int fd = open("/dev/myled",O_RDWR);
	if(fd < 0)
	{
		printf("open fail\n");
		return -1;
	}

	while(1)
	{
		read(fd,&key_val,4);
		if(key_val == 0)
		{
			do{
				msleep(100);
				read(fd,&key_val,4);
			}while(key_val == 0);
		
			ioctl(fd,LED_EOR);
		}
		msleep(300);
	}

	close(fd);
	return 0;
}




