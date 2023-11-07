# == MARSDEV ==
# Multiplatform Mega Drive toolchain builder and installer
# This is the 'main' Makefile that calls others in their own subdirectories

MARS_BUILD_DIR   ?= $(shell pwd)/mars
MARS_INSTALL_DIR ?= /opt/mars
export MARS_BUILD_DIR
export MARS_INSTALL_DIR

# Some ANSI terminal color codes
MARS_COLOR_RESET	= $'\033[0m'
MARS_COLOR_RED		= $'\033[1;31;49m'
MARS_COLOR_GREEN	= $'\033[1;32;49m'
MARS_COLOR_YELLOW	= $'\033[1;33;49m'
MARS_COLOR_BLUE		= $'\033[1;34;49m'
MARS_COLOR_MAGENTA	= $'\033[1;35;49m'
MARS_COLOR_CYAN		= $'\033[1;36;49m'
MARS_COLOR_WHITE	= $'\033[1;37;49m'
export MARS_COLOR_RESET
export MARS_COLOR_RED
export MARS_COLOR_GREEN
export MARS_COLOR_YELLOW
export MARS_COLOR_BLUE
export MARS_COLOR_MAGENTA
export MARS_COLOR_CYAN
export MARS_COLOR_WHITE


.PHONY: all with-newlib gcc-toolchain gcc-toolchain-newlib
.PHONY: z80-tools sik-tools sgdk sgdk-samples blastem

all: gcc-toolchain z80-tools sik-tools sgdk blastem
with-newlib: gcc-toolchain-newlib z80-tools sik-tools sgdk blastem

gcc-toolchain:
	@echo "$(MARS_COLOR_YELLOW)> Building gcc toolchain for m68k...$(MARS_COLOR_RESET)"
	make -C gcc-toolchain

gcc-toolchain-newlib:
	@echo "$(MARS_COLOR_YELLOW)> Building gcc toolchain with newlib for m68k...$(MARS_COLOR_RESET)"
	make -C gcc-toolchain with-newlib

z80-tools:
	@echo "$(MARS_COLOR_YELLOW)> Building z80 tools...$(MARS_COLOR_RESET)"
	make -C z80-tools

sik-tools:
	@echo "$(MARS_COLOR_YELLOW)> Building Sik tools...$(MARS_COLOR_RESET)"
	make -C sik-tools

sgdk: gcc-toolchain z80-tools 
	@echo "$(MARS_COLOR_YELLOW)> Building SGDK...$(MARS_COLOR_RESET)"
	make -C sgdk

sgdk-samples: sgdk
	@echo "$(MARS_COLOR_YELLOW)> Building SGDK samples...$(MARS_COLOR_RESET)"
	#make -C sgdk samples

blastem:
	@echo "$(MARS_COLOR_YELLOW)> Building Blastem emulator...$(MARS_COLOR_RESET)"
	make -C blastem

.PHONY: clean
clean:
	@echo "$(MARS_COLOR_MAGENTA)> Cleaning...$(MARS_COLOR_RESET)"
	make -C gcc-toolchain clean
	make -C z80-tools clean
	make -C sik-tools clean
	make -C sgdk clean
	make -C blastem clean
	rm -rf $(MARS_BUILD_DIR)

.PHONY: install
install:
	@mkdir -p $(MARS_INSTALL_DIR)
	cp -rf $(MARS_BUILD_DIR)/* $(MARS_INSTALL_DIR)
	@echo "#!/bin/sh" > $(MARS_INSTALL_DIR)/mars.sh
	@echo "export MARSDEV=$(MARS_INSTALL_DIR)" >> $(MARS_INSTALL_DIR)/mars.sh
	@echo "export GDK=$(MARS_INSTALL_DIR)/m68k-elf" >> $(MARS_INSTALL_DIR)/mars.sh
	@echo "export PATH=\"$$`echo PATH`:$(MARS_INSTALL_DIR)/m68k-elf/bin\"" >> $(MARS_INSTALL_DIR)/mars.sh
	@chmod +x $(MARS_INSTALL_DIR)/mars.sh
	@echo "--------------------------------------------------------------------------------"
	@echo "Marsdev has been installed to $(MARS_INSTALL_DIR)." 
	@echo "Run the following to set the proper environment variables before building your projects:"
	@echo "source $(MARS_INSTALL_DIR)/mars.sh"
	@echo "--------------------------------------------------------------------------------"
