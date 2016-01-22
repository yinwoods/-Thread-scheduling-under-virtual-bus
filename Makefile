KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)/

PWD := $(shell pwd)

obj-m := driver.o
obj-m += device1.o
obj-m += device2.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	$(RM) Module.markers modules.order
