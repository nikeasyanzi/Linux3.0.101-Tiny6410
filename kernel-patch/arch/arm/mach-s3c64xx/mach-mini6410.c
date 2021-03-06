/* linux/arch/arm/mach-s3c64xx/mach-mini6410.c
 *
 * Copyright 2010 Darius Augulis <augulis.darius@gmail.com>
 * Copyright 2008 Openmoko, Inc.
 * Copyright 2008 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/fb.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/dm9000.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/serial_core.h>
#include <linux/types.h>

#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <mach/map.h>
#include <mach/regs-fb.h>
#include <mach/regs-gpio.h>
#include <mach/regs-modem.h>
#include <mach/regs-srom.h>
#include <mach/s3c6410.h>

#include <plat/adc.h>
#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/fb.h>
#include <plat/nand.h>
#include <plat/regs-serial.h>
// #include <plat/ts.h>

#include <video/platform_lcd.h>

// add by chanfai
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/mmc/host.h>

#include <mach/ts.h>
#include <mach/regs-clock.h>

#include <plat/clock.h>
#include <plat/sdhci.h>
#include <plat/iic.h>
#include <plat/audio.h>
#include <plat/regs-usb-hsotg-phy.h>


#define UCON S3C2410_UCON_DEFAULT
#define ULCON (S3C2410_LCON_CS8 | S3C2410_LCON_PNONE | S3C2410_LCON_STOPB)
#define UFCON (S3C2410_UFCON_RXTRIG8 | S3C2410_UFCON_FIFOMODE)

static struct s3c2410_uartcfg mini6410_uartcfgs[] __initdata = {
	[0] = {
		.hwport	= 0,
		.flags	= 0,
		.ucon	= UCON,
		.ulcon	= ULCON,
		.ufcon	= UFCON,
	},
	[1] = {
		.hwport	= 1,
		.flags	= 0,
		.ucon	= UCON,
		.ulcon	= ULCON,
		.ufcon	= UFCON,
	},
	[2] = {
		.hwport	= 2,
		.flags	= 0,
		.ucon	= UCON,
		.ulcon	= ULCON,
		.ufcon	= UFCON,
	},
	[3] = {
		.hwport	= 3,
		.flags	= 0,
		.ucon	= UCON,
		.ulcon	= ULCON,
		.ufcon	= UFCON,
	},
};

/* DM9000AEP 10/100 ethernet controller */

static struct resource mini6410_dm9k_resource[] = {
	[0] = {
		.start	= S3C64XX_PA_XM0CSN1,
		.end	= S3C64XX_PA_XM0CSN1 + 1,
		.flags	= IORESOURCE_MEM
	},
	[1] = {
		.start	= S3C64XX_PA_XM0CSN1 + 4,
		.end	= S3C64XX_PA_XM0CSN1 + 5,
		.flags	= IORESOURCE_MEM
	},
	[2] = {
		.start	= S3C_EINT(7),
		.end	= S3C_EINT(7),
		.flags	= IORESOURCE_IRQ | IORESOURCE_IRQ_HIGHLEVEL
	}
};

static struct dm9000_plat_data mini6410_dm9k_pdata = {
	.flags		= (DM9000_PLATF_16BITONLY | DM9000_PLATF_NO_EEPROM),
};

static struct platform_device mini6410_device_eth = {
	.name		= "dm9000",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(mini6410_dm9k_resource),
	.resource	= mini6410_dm9k_resource,
	.dev		= {
		.platform_data	= &mini6410_dm9k_pdata,
	},
};

/* modify by chanfai, add Nand flash info */
struct mtd_partition mini6410_nand_part[] = {
	{
		.name		= "Bootloader",
		.offset		= 0,
		.size		= (4 * 128 *SZ_1K),
		.mask_flags	= MTD_CAP_NANDFLASH,
	},
	{
		.name		= "Kernel",
		.offset		= (4 * 128 *SZ_1K),
		.size		= (5*SZ_1M) ,
		.mask_flags	= MTD_CAP_NANDFLASH,
	},
	{
		.name		= "File System",
		.offset		= MTDPART_OFS_APPEND,
		.size		= MTDPART_SIZ_FULL,
	}
};

static struct s3c2410_nand_set mini6410_nand_sets[] = {
	[0] = {
		.name		= "nand",
		.nr_chips	= 1,
		.nr_partitions	= ARRAY_SIZE(mini6410_nand_part),
		.partitions	= mini6410_nand_part,
	},
};

static struct s3c2410_platform_nand mini6410_nand_info = {
	.tacls		= 25,
	.twrph0		= 55,
	.twrph1		= 40,
	.nr_sets	= ARRAY_SIZE(mini6410_nand_sets),
	.sets		= mini6410_nand_sets,
};

/* add by chanfai MMC/SD config */
static struct s3c_sdhci_platdata mini6410_hsmmc0_pdata = {
    .max_width      = 4,
    .cd_type        = S3C_SDHCI_CD_INTERNAL,
};

static struct s3c_sdhci_platdata mini6410_hsmmc1_pdata = {
    .max_width      = 4,
    .cd_type        = S3C_SDHCI_CD_PERMANENT,
};

static struct s3c_fb_pd_win mini6410_fb_win[] = {
	{
		.win_mode	= {	/* 4.3" 480x272 */
			.left_margin	= 3,
			.right_margin	= 2,
			.upper_margin	= 1,
			.lower_margin	= 1,
			.hsync_len	= 40,
			.vsync_len	= 1,
			.xres		= 480,
			.yres		= 272,
		},
		.max_bpp	= 32,
		.default_bpp	= 16,
	}, {
		.win_mode	= {	/* 7.0" 800x480 */
			.left_margin	= 8,
			.right_margin	= 13,
			.upper_margin	= 7,
			.lower_margin	= 5,
			.hsync_len	= 3,
			.vsync_len	= 1,
			.xres		= 800,
			.yres		= 480,
		},
		.max_bpp	= 32,
		.default_bpp	= 16,
	},
};

static struct s3c_fb_platdata mini6410_lcd_pdata __initdata = {
	.setup_gpio	= s3c64xx_fb_gpio_setup_24bpp,
	.win[0]		= &mini6410_fb_win[0],
	.vidcon0	= VIDCON0_VIDOUT_RGB | VIDCON0_PNRMODE_RGB,
	.vidcon1	= VIDCON1_INV_HSYNC | VIDCON1_INV_VSYNC,
};

/* modify by chanfai */
static void mini6410_lcd_power_set(struct plat_lcd_data *pd,
				   unsigned int power)
{
	if (power) {
		gpio_direction_output(S3C64XX_GPF(13), 1);
		gpio_direction_output(S3C64XX_GPF(15), 1);

		/* fire nRESET on power up */
		gpio_direction_output(S3C64XX_GPN(5), 0);
		msleep(10);
		gpio_direction_output(S3C64XX_GPN(5), 1);
		msleep(1);
	} else {
		gpio_direction_output(S3C64XX_GPF(15), 0);
		gpio_direction_output(S3C64XX_GPF(13), 0);
	}
}

static struct plat_lcd_data mini6410_lcd_power_data = {
	.set_power	= mini6410_lcd_power_set,
};

static struct platform_device mini6410_lcd_powerdev = {
	.name			= "platform-lcd",
	.dev.parent		= &s3c_device_fb.dev,
	.dev.platform_data	= &mini6410_lcd_power_data,
};


/* add by chanfai, for support usb */
#ifdef CONFIG_USB_SUPPORT
/* Initializes OTG Phy. to output 48M clock */
void s3c_otg_phy_config(int enable) {
	u32 val;

	if (enable) {
		__raw_writel(0x0, S3C_PHYPWR);	/* Power up */

		val = __raw_readl(S3C_PHYCLK);
		val &= ~S3C_PHYCLK_CLKSEL_MASK;
		__raw_writel(val, S3C_PHYCLK);

		__raw_writel(0x1, S3C_RSTCON);
		udelay(5);
		__raw_writel(0x0, S3C_RSTCON);	/* Finish the reset */
		udelay(5);
	} else {
		__raw_writel(0x19, S3C_PHYPWR);	/* Power down */
	}
}

/* configure usb host clock */
void s3c_usb_clk_config(void)
{
	/* UHOST_RATIO = 1 */
	__raw_writel(__raw_readl(S3C_CLK_DIV1) & 0xff0fffff, S3C_CLK_DIV1);
	/* UHOST_SEL = MOUNTepll, EPLL_SEL=1 */
	__raw_writel((__raw_readl(S3C_CLK_SRC) & 0xffffff9b) | 0x24, S3C_CLK_SRC);

	/* here default disable usb & usb-otg clock gate ctrl, because do it in bootloader */
	/* enable HCLK gate */
	// __raw_writel(__raw_readl(S3C_HCLK_GATE) | S3C_CLKCON_HCLK_UHOST | S3C_CLKCON_HCLK_USB, S3C_HCLK_GATE);
	/* enable SCLK gate */
	// __raw_writel(__raw_readl(S3C_SCLK_GATE) | S3C_CLKCON_SCLK_UHOST, S3C_SCLK_GATE);
}

EXPORT_SYMBOL(s3c_otg_phy_config);
#endif

/* modify by chanfai */
#ifdef CONFIG_SAMSUNG_DEV_TS
static struct s3c2410_ts_mach_info s3c_ts_platform __initdata = {
	.delay			= 10000,
	.presc			= 49,
	.oversampling_shift	= 2,
};
#endif

/* add by chanfai */
#ifdef CONFIG_TOUCHSCREEN_MINI6410
static struct s3c_ts_mach_info s3c_ts_platform __initdata = {
	.delay			= 0xFFFF,
	.presc			= 0xFF,
	.oversampling_shift	= 2,
	.resol_bit		= 12,
	.s3c_adc_con	= ADC_TYPE_2,
};
#endif

/* add by chanfai */
static struct map_desc mini6410_iodesc[] = {
	{
		/* LCD support */
		.virtual    = (unsigned long)S3C_VA_LCD,
		.pfn        = __phys_to_pfn(S3C_PA_FB),
		.length     = SZ_16K,
		.type       = MT_DEVICE,
	},
};

/* modify by chanfai */
static struct platform_device *mini6410_devices[] __initdata = {
	&mini6410_device_eth,

#ifdef CONFIG_MINI6410_SD_CH0
	&s3c_device_hsmmc0,
#endif

#ifdef CONFIG_MINI6410_SD_CH1
	&s3c_device_hsmmc1,
#endif

	&s3c_device_i2c0,
#ifdef CONFIG_S3C_DEV_I2C1
	&s3c_device_i2c1,
#endif

	&s3c_device_ohci,
	&s3c_device_usb_hsotg,
	&s3c_device_nand,
	&s3c_device_fb,
	&mini6410_lcd_powerdev,

#ifdef CONFIG_S3C_ADC
	&s3c_device_adc,
#endif

#if defined(CONFIG_TOUCHSCREEN_MINI6410) || defined(CONFIG_SAMSUNG_DEV_TS)
	&s3c_device_ts,
#endif

#ifdef CONFIG_S3C_DEV_RTC
	&s3c_device_rtc,
#endif

#ifdef CONFIG_SND_SAMSUNG_AC97
	&s3c64xx_device_ac97,
#else
	&s3c64xx_device_iisv4,
#endif
	&samsung_asoc_dma,

};

/* add by chanfai */
static struct i2c_board_info i2c_devs0[] __initdata = {
	{ I2C_BOARD_INFO("ov965x", 0x30), },
};

static struct i2c_board_info i2c_devs1[] __initdata = {
	/* Add your i2c device here */
};

/* modify by chanfai */
static void __init mini6410_map_io(void)
{
	u32 tmp;

	// s3c64xx_init_io(NULL, 0);
	s3c64xx_init_io(mini6410_iodesc, ARRAY_SIZE(mini6410_iodesc));
	s3c24xx_init_clocks(12000000);
	s3c24xx_init_uarts(mini6410_uartcfgs, ARRAY_SIZE(mini6410_uartcfgs));

	/* set the LCD type */
	tmp = __raw_readl(S3C64XX_SPCON);
	tmp &= ~S3C64XX_SPCON_LCD_SEL_MASK;
	tmp |= S3C64XX_SPCON_LCD_SEL_RGB;
	__raw_writel(tmp, S3C64XX_SPCON);

	/* remove the LCD bypass */
	tmp = __raw_readl(S3C64XX_MODEM_MIFPCON);
	tmp &= ~MIFPCON_LCD_BYPASS;
	__raw_writel(tmp, S3C64XX_MODEM_MIFPCON);
}

/*
 * mini6410_features string
 *
 * 0-9 LCD configuration
 *
 */
static char mini6410_features_str[12] __initdata = "0";

static int __init mini6410_features_setup(char *str)
{
	if (str)
		strlcpy(mini6410_features_str, str,
			sizeof(mini6410_features_str));
	return 1;
}

__setup("mini6410=", mini6410_features_setup);

#define FEATURE_SCREEN (1 << 0)

struct mini6410_features_t {
	int done;
	int lcd_index;
};

static void mini6410_parse_features(
		struct mini6410_features_t *features,
		const char *features_str)
{
	const char *fp = features_str;

	features->done = 0;
	features->lcd_index = 0;

	while (*fp) {
		char f = *fp++;

		switch (f) {
		case '0'...'9':	/* tft screen */
			if (features->done & FEATURE_SCREEN) {
				printk(KERN_INFO "MINI6410: '%c' ignored, "
					"screen type already set\n", f);
			} else {
				int li = f - '0';
				if (li >= ARRAY_SIZE(mini6410_fb_win))
					printk(KERN_INFO "MINI6410: '%c' out "
						"of range LCD mode\n", f);
				else {
					features->lcd_index = li;
				}
			}
			features->done |= FEATURE_SCREEN;
			break;
		}
	}
}

static void __init mini6410_machine_init(void)
{
	u32 cs1;
	struct mini6410_features_t features = { 0 };

	printk(KERN_INFO "MINI6410: Option string mini6410=%s\n",
			mini6410_features_str);

	/* Parse the feature string */
	mini6410_parse_features(&features, mini6410_features_str);

	mini6410_lcd_pdata.win[0] = &mini6410_fb_win[features.lcd_index];

	printk(KERN_INFO "MINI6410: selected LCD display is %dx%d\n",
		mini6410_lcd_pdata.win[0]->win_mode.xres,
		mini6410_lcd_pdata.win[0]->win_mode.yres);

	s3c_nand_set_platdata(&mini6410_nand_info);
	s3c_fb_set_platdata(&mini6410_lcd_pdata);
	// s3c24xx_ts_set_platdata(&s3c_ts_platform);

	/* add i2c */
	s3c_i2c0_set_platdata(NULL);
#ifdef CONFIG_S3C_DEV_I2C1
	s3c_i2c1_set_platdata(NULL);
#endif

	/* add for mini6410 touchscreen */
#ifdef CONFIG_TOUCHSCREEN_MINI6410
	s3c_ts_set_platdata(&s3c_ts_platform);
#endif

	/* add s3c nand name */
#ifdef CONFIG_MTD_NAND_S3C
	s3c_device_nand.name = "s3c6410-nand";
#endif

	/* add for mmc platform data */
	s3c_sdhci0_set_platdata(&mini6410_hsmmc0_pdata);
	s3c_sdhci1_set_platdata(&mini6410_hsmmc1_pdata);

	/* add for soundcard */
	s3c64xx_ac97_setup_gpio(0);

	/* add for usb host */
#ifdef CONFIG_USB_SUPPORT
	s3c_usb_clk_config();
	s3c_otg_phy_config(1);
#endif	

	/* configure nCS1 width to 16 bits */

	cs1 = __raw_readl(S3C64XX_SROM_BW) &
		~(S3C64XX_SROM_BW__CS_MASK << S3C64XX_SROM_BW__NCS1__SHIFT);
	cs1 |= ((1 << S3C64XX_SROM_BW__DATAWIDTH__SHIFT) |
		(1 << S3C64XX_SROM_BW__WAITENABLE__SHIFT) |
		(1 << S3C64XX_SROM_BW__BYTEENABLE__SHIFT)) <<
			S3C64XX_SROM_BW__NCS1__SHIFT;
	__raw_writel(cs1, S3C64XX_SROM_BW);

	/* set timing for nCS1 suitable for ethernet chip */

	__raw_writel((0 << S3C64XX_SROM_BCX__PMC__SHIFT) |
		(6 << S3C64XX_SROM_BCX__TACP__SHIFT) |
		(4 << S3C64XX_SROM_BCX__TCAH__SHIFT) |
		(1 << S3C64XX_SROM_BCX__TCOH__SHIFT) |
		(13 << S3C64XX_SROM_BCX__TACC__SHIFT) |
		(4 << S3C64XX_SROM_BCX__TCOS__SHIFT) |
		(0 << S3C64XX_SROM_BCX__TACS__SHIFT), S3C64XX_SROM_BC1);

	/* add & modify for mini6410 lcd power */
	gpio_request(S3C64XX_GPF(15), "LCD power");
	gpio_request(S3C64XX_GPN(5), "LCD power");
	gpio_request(S3C64XX_GPF(13), "LCD power");

	/* add i2c */
	if (ARRAY_SIZE(i2c_devs0)) {
		i2c_register_board_info(0, i2c_devs0, ARRAY_SIZE(i2c_devs0));
	}
	if (ARRAY_SIZE(i2c_devs1)) {
		i2c_register_board_info(1, i2c_devs1, ARRAY_SIZE(i2c_devs1));
	}

	platform_add_devices(mini6410_devices, ARRAY_SIZE(mini6410_devices));
}

MACHINE_START(MINI6410, "MINI6410")
	/* Maintainer: Darius Augulis <augulis.darius@gmail.com> */
	.boot_params	= S3C64XX_PA_SDRAM + 0x100,
	.init_irq	= s3c6410_init_irq,
	.map_io		= mini6410_map_io,
	.init_machine	= mini6410_machine_init,
	.timer		= &s3c24xx_timer,
MACHINE_END
