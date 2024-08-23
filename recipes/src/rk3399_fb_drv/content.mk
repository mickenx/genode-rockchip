MIRRORED_FROM_REP_DIR := src/drivers/framebuffer/rk3399

content: $(MIRRORED_FROM_REP_DIR)
$(MIRRORED_FROM_REP_DIR):
	$(mirror_from_rep_dir)

