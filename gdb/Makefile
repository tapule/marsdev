MARSDEV = $(MARS_BUILD_DIR)

GNU_MIRROR   ?= https://ftp.gnu.org/gnu/gdb
#GNU_MIRROR   ?= https://mirrors.tripadvisor.com/gnu/gdb

GDB_VER = 13.2
GDB_DIR = gdb-$(GDB_VER)
GDB_PKG = $(GDB_DIR).tar.xz

TARGET  = m68k-elf
PREFIX  = $(MARSDEV)/$(TARGET)
PATH   := $(PREFIX)/bin:$(PATH)
LOGDIR := $(shell pwd)

LANGS ?= c

# Detect the number of processors for a parallel make
NPROC := $(shell nproc --all)

.PHONY: all gdb clean

all: gdb

gdb: BUILD_DIR=$(GDB_DIR)/build
gdb: $(GDB_DIR)
	@echo "+++ Building $(GDB_DIR) for m68k..."
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		../configure --target=$(TARGET) --prefix=$(PREFIX) --with-cpu=m68000 \
		--libdir=$(MARSDEV)/$(TARGET)/lib \
		--libexecdir=$(MARSDEV)/$(TARGET)/libexec \
		--enable-languages=$(LANGS) \
		--disable-multilib --disable-tls --disable-werror \
		> $(LOGDIR)/gdb-m68k.log 2>&1
	make -C $(BUILD_DIR) all -j$(NPROC) >> $(LOGDIR)/gdb-m68k.log 2>&1
	make -C $(BUILD_DIR) install >> $(LOGDIR)/gdb-m68k.log 2>&1
	rm -rf $(GDB_DIR)

$(GDB_DIR): $(GDB_PKG)
	tar xf $(GDB_PKG)

$(GDB_PKG):
	@wget $(GNU_MIRROR)/$(GDB_PKG)

clean:
	rm -rf $(GDB_DIR)
