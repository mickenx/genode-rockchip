DRIVER := usb_host/rk3399
MIRRORED_FROM_REP_DIR := src/drivers/usb_host/rk3399
 
content: $(MIRRORED_FROM_REP_DIR)
$(MIRRORED_FROM_REP_DIR):
	$(mirror_from_rep_dir)
MIRROR_FROM_OS_DIR := src/lib/genode_c_api/usb.cc

content: $(MIRROR_FROM_OS_DIR)

$(MIRROR_FROM_OS_DIR):
	mkdir -p $(dir $@)
	cp -r $(GENODE_DIR)/repos/os/$@ $@

include $(REP_DIR)/src/drivers/usb_host/rk3399/extra.inc
