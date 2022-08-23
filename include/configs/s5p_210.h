/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef __CONFIG_H
#define __CONFIG_H

/* #define DEBUG */

/* High Level Configuration Options */
#define CONFIG_SAMSUNG		1	/* in a SAMSUNG core */
#define CONFIG_S5P		1	/* which is in a S5P Family */
#define CONFIG_S5PC110		1	/* which is in a S5PC110 */

#include <asm/arch/cpu.h>		/* get chip and board defs */

/* DRAM Base */
#define CONFIG_SYS_SDRAM_BASE		0x20000000

/* PWM */
#define CONFIG_PWM			1

#define KERNEL_NAME 		"zImage"
#define SATE210_DTB_NAME	"210-sate210.dtb"
#define TQ210_DTB_NAME		"210-tq210v4.dtb"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"bootk=" \
		"tftpboot 0x20008000 " KERNEL_NAME ";" \
		"tftpboot 0x21008000 ${dtbname};" \
		"bootz 0x20008000 - 0x21008000\0" \
	"mmcboot=" \
		"fatload mmc 0:2 0x20008000 " KERNEL_NAME ";" \
		"fatload mmc 0:2 0x21008000 ${dtbname};" \
		"bootz 0x20008000 - 0x21008000\0" \
	"dtbname=" SATE210_DTB_NAME "\0" \
	"ethaddr=00:26:22:D9:FA:25\0"

#define CONFIG_SYS_PBSIZE	384	/* Print Buffer Size */

#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE
#define PHYS_SDRAM_2		0x40000000

/*
0x20000000 --> 0x23000000	(48 MB)
*/
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - 0x1000000)

#define CONFIG_MISC_COMMON

/* Ethernet */
#define CONFIG_DM9000_BASE	0x88000000
#define DM9000_IO			CONFIG_DM9000_BASE
#define DM9000_DATA			(CONFIG_DM9000_BASE + 0x4)
#define CONFIG_DM9000_NO_SROM
#define CONFIG_NETMASK		255.255.255.0
#define CONFIG_IPADDR		192.168.1.240
#define CONFIG_SERVERIP		192.168.1.3
#define CONFIG_GATEWAYIP	192.168.1.1

/*
 * bootloader(sd)
 * |512|spl(16k)|env(8k)|uboot(512k)|kernel(16M)|p1_rootfs(64M)|p2_data(~)
 */

#define CONFIG_SKIP_LOWLEVEL_INIT

#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG

/* Size of malloc() pool before and after relocation */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (16 << 20))

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x4000000)

#define CONFIG_ENV_OVERWRITE
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_SYS_MMC_ENV_PART		0

#define CONFIG_DRIVER_DM9000

#endif	/* __CONFIG_H */
