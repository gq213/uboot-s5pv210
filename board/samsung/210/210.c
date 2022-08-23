// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <asm/gpio.h>
#include <asm/arch/mmc.h>
#include <asm/arch/sromc.h>
#include <netdev.h>
#include <samsung/misc.h>

DECLARE_GLOBAL_DATA_PTR;

#define BOARD_TYPE_ADDR				0x20000000	// offset=0
static unsigned int *p_board_type = (unsigned int *)(BOARD_TYPE_ADDR);

#ifdef CONFIG_DISPLAY_BOARDINFO
int checkboard(void)
{
	if (*p_board_type == 1) {
		puts("Board:\tSate210\n");
	} else if (*p_board_type == 2) {
		puts("Board:\tTq210\n");
	} else {
		puts("Board:\tUnknown\n");
		return -1;
	}
	return 0;
}
#endif

int dram_init(void)
{
	if (*p_board_type == 1)
		gd->ram_size = (256 << 20) + (256 << 20);
	else if (*p_board_type == 2)
		gd->ram_size = (512 << 20) + (512 << 20);

	return 0;
}

int dram_init_banksize(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM_1;
	gd->bd->bi_dram[0].size = gd->ram_size / 2;
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size = gd->ram_size / 2;

	return 0;
}

// initr_caches

static void dm9000_pre_init(void)
{
	#define CONFIG_ENV_SROM_BANK	1
	u32 smc_bw_conf, smc_bc_conf;

	smc_bw_conf = SMC_DATA16_WIDTH(CONFIG_ENV_SROM_BANK)
			| SMC_BYTE_ADDR_MODE(CONFIG_ENV_SROM_BANK);
	smc_bc_conf = SMC_BC_TACS(0) | SMC_BC_TCOS(5) | SMC_BC_TACC(15)
			| SMC_BC_TCOH(5) | SMC_BC_TAH(0)
			| SMC_BC_TACP(0) | SMC_BC_PMC(0);

	s5p_config_sromc(CONFIG_ENV_SROM_BANK, smc_bw_conf, smc_bc_conf);
}

int board_init(void)
{
	dm9000_pre_init();

	gd->bd->bi_arch_number = 2457;
	gd->bd->bi_boot_params = PHYS_SDRAM_1 + 0x100;

	return 0;
}

#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bis)
{
	int i, ret;
	int i_start, i_end, i_skip, index;

	if (gd->ram_size == (512 << 20)) {
		i_start = S5PC110_GPIO_G20;
		i_end = S5PC110_GPIO_G27;
		i_skip = S5PC110_GPIO_G22;
		index = 2;
	} else /* if (gd->ram_size == (1024 << 20)) */ {
		i_start = S5PC110_GPIO_G00;
		i_end = S5PC110_GPIO_G07;
		i_skip = S5PC110_GPIO_G02;
		index = 0;
	}

	for (i = i_start; i < i_end; i++) {
		if (i == i_skip)
			continue;
		gpio_cfg_pin(i, 0x2);
		gpio_set_pull(i, S5P_GPIO_PULL_NONE);
		gpio_set_drv(i, S5P_GPIO_DRV_4X);
	}

	ret = s5p_mmc_init(index, 4);
	if (ret)
		pr_err("MMC: Failed to init MMC:%d.\n", index);

	return ret;
}
#endif

#ifdef CONFIG_MISC_INIT_R
int misc_init_r(void)
{
#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	set_board_info();
#endif
	return 0;
}
#endif

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	if (gd->ram_size == (512 << 20)) {
		env_set("dtbname", SATE210_DTB_NAME);
	} else if (gd->ram_size == (1024 << 20)) {
		env_set("dtbname", TQ210_DTB_NAME);
	}
	return 0;
}
#endif

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	int rc = 0;

	rc = dm9000_initialize(bis);

	return rc;
}
#endif

u32 get_board_rev(void)
{
	return 0;
}
