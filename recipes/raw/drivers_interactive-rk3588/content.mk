content: drivers.config event_filter.config en_us.chargen special.chargen fb_drv.config

drivers.config fb_drv.config:
	cp $(REP_DIR)/recipes/raw/drivers_interactive-rk3588/$@ $@

drivers.config event_filter.config:
	cp $(REP_DIR)/recipes/raw/drivers_interactive-rk3588/$@ $@

en_us.chargen special.chargen:
	cp $(GENODE_DIR)/repos/os/src/server/event_filter/$@ $@
