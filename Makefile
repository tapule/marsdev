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

# Tools required
PREREQ_TAR	 	= $(shell which tar)
PREREQ_WGET 	= $(shell which wget)
PREREQ_GIT 		= $(shell which git)
PREREQ_LIBPNG	= $(shell /sbin/ldconfig -p | grep libpng)
PREREQ_JAVA 	= $(shell which java)

$(info * Initial tests...)
$(info > Checking for tar...)
ifeq ($(PREREQ_TAR),)
  $(error 'tar' not found. Make sure 'tar' is installed.)
else
  $(info Found: $(PREREQ_TAR))
endif
$(info > Checking for wget...)
ifeq ($(PREREQ_WGET),)
  $(error 'wget' not found. Make sure the 'wget' package is installed.)
else
  $(info Found: $(PREREQ_WGET))
endif
$(info > Checking for git...)
ifeq ($(PREREQ_GIT),)
  $(error 'git' not found. Make sure the 'git' package is installed.)
else
  $(info Found: $(PREREQ_GIT))
endif
$(info > Checking for libpng...)
ifeq ($(PREREQ_LIBPNG),)
  $(error 'libpng' not found, needed to build Sik tools. Make sure the 'libpng' package is installed.)
else
  $(info Found: $(PREREQ_LIBPNG))
endif
$(info > Checking for java...)
ifeq ($(PREREQ_JAVA),)
  $(error "'java' not found, needed to build SGDK. Make sure a Java runtime is installed.")
else
  $(info Found: $(PREREQ_JAVA))
endif
$(info ----------------------------------)

.PHONY: all with-newlib initial-tests toolchain toolchain-newlib
.PHONY: gdb z80-tools sik-tools sgdk sgdk-samples blastem

all: toolchain gdb z80-tools sik-tools sgdk blastem
with-newlib: toolchain-newlib gdb z80-tools sik-tools sgdk #blastem

toolchain:
	@echo "$(MARS_COLOR_YELLOW)> Building toolchain for m68k...$(MARS_COLOR_RESET)"
	#make -C toolchain

toolchain-newlib:
	@echo "$(MARS_COLOR_YELLOW)> Building toolchain with newlib for m68k...$(MARS_COLOR_RESET)"
	#make -C toolchain with-newlib

gdb:
	@echo "$(MARS_COLOR_YELLOW)> Building gdb for m68k...$(MARS_COLOR_RESET)"
	#make -C gdb

z80-tools:
	@echo "$(MARS_COLOR_YELLOW)> Building z80 tools...$(MARS_COLOR_RESET)"
	#make -C z80-tools

sik-tools:
	@echo "$(MARS_COLOR_YELLOW)> Building Sik tools...$(MARS_COLOR_RESET)"
	#make -C sik-tools

sgdk: z80-tools
	@echo "$(MARS_COLOR_YELLOW)> Building SGDK...$(MARS_COLOR_RESET)"
	#make -C sgdk

sgdk-samples: sgdk
	@echo "$(MARS_COLOR_YELLOW)> Building SGDK samples...$(MARS_COLOR_RESET)"
	#make -C sgdk samples

blastem:
	@echo "$(MARS_COLOR_YELLOW)> Building Blastem emulator...$(MARS_COLOR_RESET)"
	#make -C blastem


.PHONY: clean
clean:
	@echo "$(MARS_COLOR_MAGENTA)> Cleaning project...$(MARS_COLOR_RESET)"
	#make -C toolchain clean
	#make -C gdb clean
	#make -C z80-tools clean
	#make -C sik-tools clean
	#make -C sgdk clean
	#make -C blastem clean
	#rm -rf $(MARS_BUILD_DIR)


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
