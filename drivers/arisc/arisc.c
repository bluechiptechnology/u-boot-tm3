// SPDX-License-Identifier: GPL-2.0+

#include <common.h>
#include <dm.h>
#include <dm-demo.h>
#include <mapmem.h>
#include <asm/io.h>

#include "arisc_i.h"

#define ARM_SVC_ARISC_STARTUP	0x8000ff10
#define SUNXI_RCPUCFG_BASE	(0x07000400L)

struct dts_cfg dts_cfg =
{
	.dram_para = {0x1,0x1}
};
struct dts_cfg_64 dts_cfg_64 =
{
	.dram_para = {0x1,0x1}
};

int sunxi_deassert_arisc(void)
{
	volatile unsigned long value;
	value = readl(SUNXI_RCPUCFG_BASE + 0x0);
	value &= ~1;
	writel(value, SUNXI_RCPUCFG_BASE + 0x0);
	value = readl(SUNXI_RCPUCFG_BASE + 0x0);
	value |= 1;
	writel(value, SUNXI_RCPUCFG_BASE + 0x0);
	return 0;
}

u32 sunxi_smc_call(ulong arg0, ulong arg1, ulong arg2, ulong arg3, ulong pResult)
{
	u32 ret = 0;
	static u32 result[4] = {0};
	ret = __sunxi_smc_call(arg0, arg1, arg2, arg3);
	return ret;
}

int arm_svc_arisc_startup(ulong cfg_base)
{
	return sunxi_smc_call(ARM_SVC_ARISC_STARTUP,cfg_base, 0, 0,0);
}

static int arisc_probe(struct udevice *dev)
{
	sunxi_deassert_arisc();

	dts_cfg_64.space.msgpool_dst = 0x48105000;
	dts_cfg_64.space.msgpool_offset = 0x0;
	dts_cfg_64.space.msgpool_size = 0x1000;
	dts_cfg_64.msgbox.base = 0x3003000;
	dts_cfg_64.msgbox.size = 0x1000;
	dts_cfg_64.msgbox.irq = 0x0;
	dts_cfg_64.msgbox.status = 0x1;
	dts_cfg_64.hwspinlock.base = 0x3004000;
	dts_cfg_64.hwspinlock.size = 0x1000;
	dts_cfg_64.hwspinlock.irq = 0x0;
	dts_cfg_64.hwspinlock.status = 0x1;

	if (arm_svc_arisc_startup((ulong)&dts_cfg_64)) {
		printf("sunxi-arisc driver startup failed\n");
	} else {
		printf("sunxi-arisc driver startup succeeded\n");
	}
	return 0;
}

static int arisc_bind(struct udevice *parent)
{
	ofnode node;

	dev_for_each_subnode(node, parent) {
		struct udevice *dev;
		int ret;
		ret = device_bind_driver_to_node(parent, "arisc",
						 ofnode_get_name(node),
						 node, &dev);
		if (ret)
			return ret;
	}

	return 0;
}

static const struct udevice_id arisc_ids[] = {
	{ .compatible = "arisc" },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(arisc) = {
	.name	= "arisc",
        .id	= UCLASS_ARISC,
	.of_match = arisc_ids,
	.bind	= arisc_bind,
	.probe	= arisc_probe,
};
