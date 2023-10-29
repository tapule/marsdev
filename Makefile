# == MARSDEV ==
# Multiplatform Mega Drive toolchain builder and installer
# This is the 'main' Makefile that calls others in their own subdirectories

MARSDEV ?= /opt/mars
export MARSDEV

# .PHONY: m68k-toolchain m68k-gdb z80-tools sik-tools flamewing-tools sgdk blastem $(MAKECMDGOALS)
.PHONY: all m68k-toolchain m68k-toolchain-newlib sh-toolchain sh-toolchain-newlib
.PHONY: m68k-gdb sh-gdb z80-tools sik-tools flamewing-tools sgdk blastem

all: m68k-toolchain m68k-gdb z80-tools sik-tools flamewing-tools sgdk #blastem

m68k-toolchain:
	make -C toolchain ARCH=m68k

m68k-toolchain-newlib:
	make -C toolchain all-newlib ARCH=m68k

m68k-gdb:
	make -C gdb ARCH=m68k

sh-toolchain:
	make -C toolchain ARCH=sh

sh-toolchain-newlib:
	make -C toolchain all-newlib ARCH=sh

sh-gdb:
	make -C gdb ARCH=sh

z80-tools:
	make -C z80-tools

sik-tools:
	make -C sik-tools

flamewing-tools:
	make -C flamewing-tools

sgdk:
	make -C sgdk

blastem:
	make -C blastem

.PHONY: clean toolchain-clean gdb-clean tools-clean sgdk-clean blastem-clean

clean: toolchain-clean gdb-clean tools-clean sgdk-clean blastem-clean

toolchain-clean:
	make -C toolchain clean

gdb-clean:
	make -C gdb clean

tools-clean:
	make -C z80-tools clean
	make -C sik-tools clean
	make -C flamewing-tools clean

sgdk-clean:
	make -C sgdk clean

blastem-clean:
	make -C blastem clean
