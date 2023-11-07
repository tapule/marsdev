MARSDEV = $(MARS_BUILD_DIR)
MARSBIN = $(MARSDEV)/m68k-elf/bin

MARS_COLOR_RESET = $'\033[0m'
MARS_COLOR_GREEN = $'\033[1;32;49m'

# Tools required
PREREQ_GIT 		= $(shell which git)
PREREQ_JAVA 	= $(shell which java)
ifeq ($(PREREQ_GIT),)
  $(error 'git' not found. Make sure the 'git' package is installed.)
endif
ifeq ($(PREREQ_JAVA),)
  $(error "'java' not found, needed to build SGDK. Make sure a Java runtime is installed.")
endif


GDK := $(MARSDEV)/m68k-elf
INC := $(GDK)/inc
BIN := $(GDK)/bin
LIB := $(GDK)/lib

BINTOS         = $(BIN)/bintos
RESCOMP        = $(BIN)/rescomp.jar
XGMTOOL        = $(BIN)/xgmtool
XGMROMBUILDER  = $(BIN)/xgmRomBuilder.jar
SIZEBND        = $(BIN)/sizebnd.jar
LZ4W           = $(BIN)/lz4w.jar
APJ            = $(BIN)/apj.jar

SGDK_VER ?= master
SGDK_OPT ?= default

LOGDIR    := $(shell pwd)

.PHONY: all tools libmd samples

all: SGDK tools libmd

tools: $(BINTOS) $(RESCOMP) $(XGMTOOL) $(XGMROOMBUILDER) $(SIZEBND) $(LZ4W) $(APJ)

libmd: $(MARSBIN) SGDK
	@echo "$(MARS_COLOR_GREEN)>> Building SGDK...$(MARS_COLOR_RESET)"
	# Copy SGDK files to toolchain
	cp -rf SGDK/inc $(GDK)/
	cp -rf SGDK/res $(GDK)/
	cp -rf SGDK/src $(GDK)/
	cp -f SGDK/common.mk $(GDK)/
	cp -f SGDK/makelib.gen $(GDK)/
	cp -f SGDK/makefile.gen $(GDK)/
	cp -f SGDK/md.ld $(GDK)/
	# release & debug
	#cp -f config-$(SGDK_OPT).h $(INC)/config.h
	make -C $(GDK) -f makelib.gen cleanrelease > $(LOGDIR)/sgdk.log 2>&1
	make -C $(GDK) -f makelib.gen release >> $(LOGDIR)/sgdk.log 2>&1
	make -C $(GDK) -f makelib.gen cleandebug >> $(LOGDIR)/sgdk.log 2>&1
	make -C $(GDK) -f makelib.gen debug >> $(LOGDIR)/sgdk.log 2>&1
	# Clean intermediate files only
	make -C $(GDK) -f makelib.gen cleanobj cleandep cleanlst >> $(LOGDIR)/sgdk.log 2>&1
	
SGDK:
	@echo "$(MARS_COLOR_GREEN)>> Downloading SGDK...$(MARS_COLOR_RESET)"
	git clone --depth=1 --branch $(SGDK_VER) https://github.com/Stephane-D/SGDK
	# Linux native branch in common.mk breaks LTO plugin, this is a workaround
	mv -f SGDK/common.mk SGDK/common.mk.bak
	cp -f common.mk SGDK/common.mk

# SGDK Samples
# If a sample is commented, it fails to build (out of date or special hardware required)

$(GDK)/lib/libmd.a: libmd
	@true

samples: $(MARSBIN) SGDK $(GDK)/lib/libmd.a
	@echo "$(MARS_COLOR_GREEN)>> Building samples...$(MARS_COLOR_RESET)"
	cd SGDK/sample/basics/hello-world && make -f $(GDK)/makefile.gen
	cd SGDK/sample/basics/image && make -f $(GDK)/makefile.gen
	cd SGDK/sample/bench && make -f $(GDK)/makefile.gen
	cd 'SGDK/sample/bitmap/cube 3D' && make -f $(GDK)/makefile.gen
	cd SGDK/sample/bitmap/partic && make -f $(GDK)/makefile.gen
	#cd SGDK/sample/console && make -f $(GDK)/makefile.gen
	cd SGDK/sample/demo/bad-apple && make -f $(GDK)/makefile.gen
	cd SGDK/sample/demo/starfield-donut && make -f $(GDK)/makefile.gen
	#cd SGDK/sample/flash-save && make -f $(GDK)/makefile.gen
	cd SGDK/sample/fx/h-int/scaling && make -f $(GDK)/makefile.gen
	cd SGDK/sample/fx/h-int/wobble && make -f $(GDK)/makefile.gen
	cd SGDK/sample/fx/hilight-shadow && make -f $(GDK)/makefile.gen
	cd SGDK/sample/fx/scroll/linescroll && make -f $(GDK)/makefile.gen
	cd SGDK/sample/joy-test && make -f $(GDK)/makefile.gen
	#cd SGDK/sample/megawifi && make -f $(GDK)/makefile.gen
	cd SGDK/sample/multitasking && make -f $(GDK)/makefile.gen
	#cd SGDK/sample/platformer && make -f $(GDK)/makefile.gen
	cd SGDK/sample/sonic && make -f $(GDK)/makefile.gen
	cd SGDK/sample/sound && make -f $(GDK)/makefile.gen
	#cd SGDK/sample/xgm-player && make -f $(GDK)/makefile.gen

# Tools

$(BINTOS): $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building bintos...$(MARS_COLOR_RESET)"
	gcc SGDK/tools/bintos/src/bintos.c -o $@
	
$(RESCOMP): $(MARSBIN)
	cp -f SGDK/bin/rescomp.jar $@
	
$(XGMTOOL): $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building xgmtool...$(MARS_COLOR_RESET)"
	cd SGDK/tools/xgmtool/src && gcc *.c -I../inc -lm -o $@

$(XGMROMBUILDER): $(MARSBIN)
	cp -f SGDK/bin/xgmRomBuilder.jar $@
	
$(SIZEBND): $(MARSBIN)
	cp -f SGDK/bin/sizebnd.jar $@

$(LZ4W): $(MARSBIN)
	cp -f SGDK/bin/lz4w.jar $@

$(APJ):
	cp -f SGDK/bin/apj.jar $@

$(MARSBIN):
	mkdir -p $(MARSBIN)


.PHONY: clean
clean:
	rm -rf SGDK
	rm -f sgdk.log
