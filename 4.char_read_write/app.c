#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char wr_buf[100] = "hello";
char rd_buf[100];

int main()
{
	int fd = open("/dev/myled",O_RDWR);
	if(fd < 0)
	{
		printf("open fail\n");
		return -1;
	}
	
	
	write(fd,wr_buf,99);
	sleep(3);
	read(fd,rd_buf,99);
	printf("rd_buf=%s\n",rd_buf);
	
	close(fd);
	return 0;
}




