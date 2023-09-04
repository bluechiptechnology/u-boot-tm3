// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2015 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 */

#define LOG_CATEGORY UCLASS_ARISC

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <dm/device-internal.h>
#include <dm/root.h>
#include <dm/uclass-internal.h>

static int arisc_post_bind(struct udevice *dev)
{
	dev_or_flags(dev, DM_FLAG_PROBE_AFTER_BIND);
	return 0;
}

UCLASS_DRIVER(arisc) = {
	.id		= UCLASS_ARISC,
	.name		= "arisc",
	.post_bind	= arisc_post_bind,
};
