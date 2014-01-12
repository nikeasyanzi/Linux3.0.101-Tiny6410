#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
//#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>

#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-gpio.h>

#include <plat/gpio-cfg.h>
#include <mach/gpio-bank-e.h>
#include <mach/gpio-bank-k.h>

#define DEVICE_NAME "tiny6410_leds"

static long tiny6410_leds_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    unsigned int tmp = 0;

    if(arg > 4)
    {
	return -EINVAL;
	printk("Invalid Paramter, arg musy < 4 ! \n");
    }

    printk("\ncmd = %d\n", cmd);
    printk("\narg = %ld\n", arg);

    switch(cmd)
	{
		case 0:  // 灭
			tmp = readl(S3C64XX_GPKDAT);
			printk("tmp = %x\n", tmp);
			tmp |= (0xf << 4);  // 全灭
			printk("tmp = %x\n", tmp);
			tmp &= (~(0x1 << (4 + arg))); // 第arg个灯亮
			printk("tmp = %x\n", tmp);
			writel(tmp, S3C64XX_GPKDAT);
			break;
		case 1: // 全亮
			tmp = readl(S3C64XX_GPKDAT);
			printk("tmp = %x\n", tmp);
			tmp &= ~(0xf << 4);
			printk("tmp = %x\n", tmp);
			writel(tmp, S3C64XX_GPKDAT);
			break;
		case 3: // 全灭
			tmp = readl(S3C64XX_GPKDAT);
			printk("tmp = %x\n", tmp);
			tmp |= (0xf << 4);
			printk("tmp = %x\n", tmp);
			writel(tmp, S3C64XX_GPKDAT);
			break;
		case 4:// 流水
			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 0)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 1)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 3)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 2)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			writel(tmp, S3C64XX_GPKDAT);
			break;
		case 5:// 流水
			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 2)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 3)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 1)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			tmp &= (~(0x1 << (4 + 0)));
			writel(tmp, S3C64XX_GPKDAT);
			msleep(500);

			tmp |= (0xf << 4);  // 全灭
			writel(tmp, S3C64XX_GPKDAT);
			break;
		default:
			printk("default call \n");
			return -EINVAL;
			break;
	}
    return 0;
}

static struct file_operations dev_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= tiny6410_leds_ioctl,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &dev_fops,
};

static int __init dev_init(void)
{
	int ret;
	unsigned int tmp;
	
	// 设置GPIOK4~7为输出
	tmp = readl(S3C64XX_GPKCON);
	tmp = (tmp & ~(0xffffU<<16))|(0x1111U<<16);
	writel(tmp, S3C64XX_GPKCON);
	
	// 设置GPIOK4~7为高电平，LED灯灭
	tmp = readl(S3C64XX_GPKDAT);
	tmp |= (0xF << 4);
	writel(tmp, S3C64XX_GPKDAT);

	// 注册LED混杂设备
	ret = misc_register(&misc);
	printk(DEVICE_NAME"\tinitialized\n");

	return ret;
}

static void __exit dev_exit(void)
{
	unsigned int tmp;

	tmp = readl(S3C64XX_GPKDAT);
	tmp |= (0xf << 4);
	writel(tmp, S3C64XX_GPKDAT);
	misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);

/* module info */
MODULE_DESCRIPTION("Driver for basic memory-mapped GPIO controller");
MODULE_AUTHOR("Chan Fai <291222536@qq.com>");
MODULE_LICENSE("Dual BSD/GPL");
