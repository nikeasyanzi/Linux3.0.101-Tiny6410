# Cross compile tools
ARCH=arm
COMPILE=arm-linux-

# To build modules outside of the kernel tree, we run "make"
# in the kernel source tree; the Makefile these then includes this
# Makefile once again.
# This conditional selects whether we are being included from the
# kernel Makefile or not.
ifeq ($(KERNELRELEASE),)

    # Assume the source tree is where the running kernel was built
    # You should set KERNELDIR in the environment if it's elsewhere
    KERNELDIR ?= /home/chanfai/workspace/linux-3.0.101
    # The current directory is passed to sub-makes as argument
    PWD := $(shell pwd)

modules:
	make -C $(KERNELDIR) M=$(PWD) modules ARCH=$(ARCH) CROSS_COMPILE=$(COMPILE)

install:
	cp tiny6410_leds.ko /home/chanfai/workspace/rootfs/

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers

.PHONY: modules modules_install clean

else
    # called from kernel build system: just declare what our modules are
    obj-m := tiny6410_adc.o tiny6410_buttons.o tiny6410_leds.o tiny6410_pwm.o
endif
