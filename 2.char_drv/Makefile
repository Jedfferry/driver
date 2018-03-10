

obj-m += led.o

KERNEL_DIR := /usr/mkdrv/src/android-kernel-samsung-dev

all:
	make modules -C $(KERNEL_DIR) M=`pwd`
	arm-linux-gcc app.c -o app
	cp led.ko app /nfs  
clean:
	make modules clean -C $(KERNEL_DIR) M=`pwd` 