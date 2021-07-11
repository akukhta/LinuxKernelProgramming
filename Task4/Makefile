obj-m := solution.o
KBUILD_EXTRA_SYMBOLS := Module.symvers

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

