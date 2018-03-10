#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
	int fd = open("/dev/myled",O_RDWR);
	if(fd < 0)
	{
		printf("open fail\n");
		return -1;
	}
	sleep(3);
	close(fd);
	return 0;
}




