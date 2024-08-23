REQUIRES = hw

REP_INC_DIR += src/core/board/rk3399

# add C++ sources
SRC_CC += kernel/vm_thread_off.cc
SRC_CC += spec/arm/gicv3.cc
SRC_CC += platform_services.cc


# include less specific configuration
include $(call select_from_repositories,lib/mk/spec/arm_v8/core-hw.inc)
