obj-m := drv.o
KERN_SOURCE := /lib/modules/$(shell uname -r)/build
CFLAGS_drv.o := -DDEBUG

default:
	$(MAKE) -C $(KERN_SOURCE) M=$(PWD) modules
	/lib/modules/$(shell uname -r)/source/scripts/checkpatch.pl --file --no-tree drv.c
install:
	$(MAKE) -C $(KERN_SOURCE) M=$(PWD) modules_install
clean:
	$(MAKE) -C $(KERN_SOURCE) M=$(PWD) clean
