PWD := $(shell pwd)
KERNEL := /home/ntai/Linux_Emb/kernelbuildscripts/KERNEL
CROSS := /home/ntai/Linux_Emb/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
obj-m += l3g42000_spi.o

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL) M=$(PWD) modules
clean:
	make -C $(KERNEL) M=$(PWD) clean

copy:
	scp l3g42000_spi.ko debian@192.168.7.2:/home/debian
