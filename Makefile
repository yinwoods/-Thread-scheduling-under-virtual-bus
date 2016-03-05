KERNEL_DIR = /usr/src/linux-headers-$(shell uname -r)/

PWD := $(shell pwd)

obj-m := drivers.o
obj-m += dev.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
	$(RM) Module.markers modules.order
