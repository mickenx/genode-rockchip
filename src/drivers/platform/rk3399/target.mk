TARGET   = rk3588-platform_drv
REQUIRES = arm_v8
include $(call select_from_repositories,src/drivers/platform/target.inc)
PLATFORM_OS_DIR = $(GENODE_DIR)/repos/os/recipes/src/platform



vpath main.cc $(PRG_DIR)
vpath %.cc    $(PLATFORM_OS_DIR)
