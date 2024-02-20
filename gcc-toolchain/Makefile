# == GCC Cross Compiler toolchain for Mega Drive ==
# Based on kubilus1's, which was in turn based on Chilly Willy's

# == Flag Explanations ==
# - For all targets
# enable-shared=no:      Disables dynamic linking -- static only
# enable-nls:            Compiler Native language support
# disable-werror:        Just who's idea was it for Werror to be the default?
# - For binutils
# enable-install-libbfd: Required for LTO to work
# - For GCC
# without-headers:       Leaves out the stdlib headers
# disable-libssp:        Library that requires stdlib causing failure
# disable-threads/tls:   MD programs are freestanding, so no thread model
# disable-multilib:      Only compiles m68000 support (no other m680x0)
# disable-hosted-libstdxx: (C++) Only build the freestanding parts of libstdc++
# without-libstdcxx:     (C++) The STL is way too big for MD

# == Resuming in case of failure ==
# At the end of each successful step is a "touch" command to creat a marker file
# These files can be deleted to redo the step again, "clean" also deletes them

# == Newlib ==
# To include optional Newlib support, use the 'all-newlib' target
# make all-newlib
# Newlib provides a libc implementation for embedded systems.
# However, we need the cross compiler to build it first.
# Because of this GCC is built in 2 passes:
# The first pass builds C support only for building newlib.
# The second pass builds everything now that newlib's libc is available.

# == Additional Languages ==
# If you want to build support for languages other than C define "LANGS" like this:
# make LANGS=c,c++
# C is required, enabled by default and the most well supported.
# C++ is not extensively tested but all non-STL features are expected to work.

MARSDEV = $(MARS_BUILD_DIR)

MARS_COLOR_RESET = $'\033[0m'
MARS_COLOR_GREEN = $'\033[1;32;49m'

# Tools required
PREREQ_TAR	 	= $(shell which tar)
PREREQ_WGET 	= $(shell which wget)
ifeq ($(PREREQ_TAR),)
  $(error 'tar' not found. Make sure 'tar' is installed.)
endif
ifeq ($(PREREQ_WGET),)
  $(error 'wget' not found. Make sure the 'wget' package is installed.)
endif

GNU_MIRROR    ?= https://ftp.gnu.org/gnu
#GNU_MIRROR   ?= https://mirrors.tripadvisor.com/gnu
NEWLIB_MIRROR ?= ftp://sourceware.org/pub

BINUTILS_VER ?= 2.42
GCC_VER      ?= 13.2.0
NEWLIB_VER   ?= 4.4.0.20231231
GDB_VER      ?= 13.2

BINUTILS_DIR  = binutils-$(BINUTILS_VER)
GCC_DIR       = gcc-$(GCC_VER)
NEWLIB_DIR    = newlib-$(NEWLIB_VER)
GDB_DIR       = gdb-$(GDB_VER)

BINUTILS_PKG  = $(BINUTILS_DIR).tar.xz
GCC_PKG       = $(GCC_DIR).tar.xz
NEWLIB_PKG    = $(NEWLIB_DIR).tar.gz
GDB_PKG       = $(GDB_DIR).tar.xz

TARGET  = m68k-elf
PREFIX  = $(MARSDEV)/$(TARGET)
PATH   := $(PREFIX)/bin:$(PATH)
LOGDIR := $(shell pwd)

LANGS ?= c

# Detect the number of processors for a parallel make
NPROC := $(shell nproc --all)

.PHONY: all with-newlib

all: BUILD_LANGS = $(LANGS)
all: mk-binutils mk-gcc mk-gdb

with-newlib: BUILD_LANGS = c
with-newlib: mk-binutils mk-gcc mk-newlib mk-gcc-newlib mk-gdb

mk-binutils: BUILD_DIR=$(BINUTILS_DIR)/build
mk-binutils: $(BINUTILS_DIR)
	@echo "$(MARS_COLOR_GREEN)>> Building binutils for m68k...$(MARS_COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		../configure --target=$(TARGET) --prefix=$(PREFIX) --with-cpu=m68000 \
		--libdir=$(MARSDEV)/$(TARGET)/lib \
		--libexecdir=$(MARSDEV)/$(TARGET)/libexec \
		--enable-install-libbfd --enable-shared=no --disable-werror \
		> $(LOGDIR)/binutils.log 2>&1
	make -C $(BUILD_DIR) all -j$(NPROC) >> $(LOGDIR)/binutils.log 2>&1
	make -C $(BUILD_DIR) install-strip  >> $(LOGDIR)/binutils.log 2>&1
	@rm -rf $(BUILD_DIR)
	@touch mk-binutils

mk-gcc: BUILD_DIR=$(GCC_DIR)/build
mk-gcc: $(GCC_DIR)
	@echo "$(MARS_COLOR_GREEN)>> Building gcc for m68k...$(MARS_COLOR_RESET)"
	cd $(GCC_DIR) && \
		./contrib/download_prerequisites > $(LOGDIR)/gcc.log 2>&1
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		../configure --target=$(TARGET) --prefix=$(PREFIX) --with-cpu=m68000 \
		--libdir=$(MARSDEV)/$(TARGET)/lib \
		--libexecdir=$(MARSDEV)/$(TARGET)/libexec \
		--enable-languages=$(BUILD_LANGS) \
		--without-headers --disable-libssp --disable-threads --disable-tls \
		--disable-multilib --enable-shared=no --disable-werror \
		>> $(LOGDIR)/gcc.log 2>&1
	make -C $(BUILD_DIR) all -j$(NPROC) >> $(LOGDIR)/gcc.log 2>&1
	make -C $(BUILD_DIR) install-strip  >> $(LOGDIR)/gcc.log 2>&1
	@rm -rf $(BUILD_DIR)
	@touch mk-gcc

mk-newlib: BUILD_DIR=$(NEWLIB_DIR)/build
mk-newlib: $(NEWLIB_DIR)
	@echo "$(MARS_COLOR_GREEN)>> Building newlib for m68k...$(MARS_COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		../configure --target=$(TARGET) --prefix=$(PREFIX) --with-cpu=m68000 \
		--libdir=$(MARSDEV)/$(TARGET)/lib \
		--libexecdir=$(MARSDEV)/$(TARGET)/libexec \
		--disable-multilib --disable-werror \
		> $(LOGDIR)/newlib.log 2>&1
	make -C $(BUILD_DIR) all -j$(NPROC) >> $(LOGDIR)/newlib.log 2>&1
	make -C $(BUILD_DIR) install >> $(LOGDIR)/newlib.log 2>&1
	@rm -rf $(BUILD_DIR)
	@touch mk-newlib

mk-gcc-newlib: BUILD_DIR=$(GCC_DIR)/build
mk-gcc-newlib: $(GCC_DIR)
	@echo "$(MARS_COLOR_GREEN)>> Building gcc with newlib for m68k...$(MARS_COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		../configure --target=$(TARGET) --prefix=$(PREFIX) --with-cpu=m68000 \
		--libdir=$(MARSDEV)/$(TARGET)/lib \
		--libexecdir=$(MARSDEV)/$(TARGET)/libexec \
		--enable-languages=$(LANGS) \
		--without-headers --with-newlib --disable-hosted-libstdxx \
		--disable-libssp --disable-threads --disable-tls --disable-multilib \
		--enable-shared=no --disable-werror \
		> $(LOGDIR)/gcc-newlib.log 2>&1
	make -C $(BUILD_DIR) all -j$(NPROC) >> $(LOGDIR)/gcc-newlib.log 2>&1
	make -C $(BUILD_DIR) install-strip  >> $(LOGDIR)/gcc-newlib.log 2>&1
	@rm -rf $(BUILD_DIR)
	@touch mk-gcc-newlib

mk-gdb: BUILD_DIR=$(GDB_DIR)/build
mk-gdb: $(GDB_DIR)
	@echo "$(MARS_COLOR_GREEN)>> Building gdb for m68k...$(MARS_COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		../configure --target=$(TARGET) --prefix=$(PREFIX) --with-cpu=m68000 \
		--libdir=$(MARSDEV)/$(TARGET)/lib \
		--libexecdir=$(MARSDEV)/$(TARGET)/libexec \
		--enable-languages=$(LANGS) \
		--disable-multilib --disable-tls --disable-werror \
		> $(LOGDIR)/gdb.log 2>&1
	make -C $(BUILD_DIR) all -j$(NPROC) >> $(LOGDIR)/gdb.log 2>&1
	make -C $(BUILD_DIR) install >> $(LOGDIR)/gdb.log 2>&1
	@rm -rf $(BUILD_DIR)
	@touch mk-gdb

# Download packages from mirror
$(BINUTILS_PKG):
	@echo "$(MARS_COLOR_GREEN)>> Downloading binutils...$(MARS_COLOR_RESET)"
	@wget $(GNU_MIRROR)/binutils/$(BINUTILS_PKG)

$(GCC_PKG):
	@echo "$(MARS_COLOR_GREEN)>> Downloading gcc...$(MARS_COLOR_RESET)"
	@wget $(GNU_MIRROR)/gcc/gcc-$(GCC_VER)/$(GCC_PKG)

$(NEWLIB_PKG):
	@echo "$(MARS_COLOR_GREEN)>> Downloading newlib...$(MARS_COLOR_RESET)"
	@wget $(NEWLIB_MIRROR)/newlib/$(NEWLIB_PKG)

$(GDB_PKG):
	@echo "$(MARS_COLOR_GREEN)>> Downloading gdb...$(MARS_COLOR_RESET)"
	@wget $(GNU_MIRROR)/gdb/$(GDB_PKG)

# Extract source packages with tar
$(BINUTILS_DIR): $(BINUTILS_PKG)
	tar xf $(BINUTILS_PKG)

$(GCC_DIR): $(GCC_PKG)
	tar xf $(GCC_PKG)

$(NEWLIB_DIR): $(NEWLIB_PKG)
	tar xf $(NEWLIB_PKG)

$(GDB_DIR): $(GDB_PKG)
	tar xf $(GDB_PKG)


.PHONY: clean
clean:
	rm -rf $(BINUTILS_DIR)
	rm -rf $(GCC_DIR)
	rm -rf $(NEWLIB_DIR)
	rm -rf $(GDB_DIR)
	rm -f mk-binutils mk-gcc mk-newlib mk-gcc-newlib mk-gdb
	rm -f binutils.log gcc.log newlib.log gcc-newlib.log gdb.log
