PWD := $(shell pwd)
KERNEL := /home/ntai/Linux_Emb/kernelbuildscripts/KERNEL
CROSS := /home/ntai/Linux_Emb/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
obj-m += l3g42000_spi.o

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL) M=$(PWD) modules
	/home/ntai/Linux_Emb/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc -o app_l3g4200d app_l3g4200d.c
clean:
	make -C $(KERNEL) M=$(PWD) clean
	rm app_l3g4200d

copy:
	scp l3g42000_spi.ko debian@192.168.7.2:/home/debian
	scp app_l3g4200d debian@192.168.7.2:/home/debian
