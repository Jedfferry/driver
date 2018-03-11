#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "myioctl.h"

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
		ioctl(fd,LED_ON);
		sleep(1);
		ioctl(fd,LED_OFF);
		sleep(1);
	}

	close(fd);
	return 0;
}




