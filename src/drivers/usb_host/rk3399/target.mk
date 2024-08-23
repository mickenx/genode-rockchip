
include $(REP_DIR)/src/drivers/usb_host/rk3399/target.inc

TARGET    = rk3399_usb_host_drv
REQUIRES  = arm_v8

SRC_C   += usb/dwc3/core.c
SRC_C   += usb/dwc3/host.c
SRC_C   += usb/host/xhci-plat.c

INC_DIR += $(REP_DIR)/src/drivers/usb_host/spec/arm
INC_DIR += $(REP_DIR)/src/include/spec/arm_64

SRC_CC  += spec/arm/platform.cc
SRC_CC  += spec/rk3339/platform.cc
# lx_kit
SRC_CC   += lx_kit/printf.cc
SRC_CC   += lx_kit/bug.cc
SRC_CC   += lx_kit/work.cc
SRC_CC   += lx_kit/timer.cc
SRC_CC   += lx_kit/scheduler.cc
SRC_CC   += lx_kit/irq.cc
SRC_CC   += lx_kit/malloc.cc
SRC_CC   += lx_kit/env.cc


CC_OPT  += -DCONFIG_ARM64
CC_OPT  += -DCONFIG_USB_DWC3_HOST=1
