#include <linux/linkage.h>
#include <asm/macro.h>
#include <config.h>

ENTRY(__sunxi_smc_call)
    smc #0
    ret
ENDPROC(__sunxi_smc_call)
