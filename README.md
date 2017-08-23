## Marsdev - GNU toolchain for MegaDrive + 32X 

It's sort-of working now.


### Install this stuff first

 * Native GNU toolchain: `build-essential` on Debian/Ubuntu
 * GNU Autotools: `libtool` `autoconf` `automake` `autopoint` `gettext`
 * Everything else: `wget` `texinfo` `libpng16-dev`
 * And if you want SGDK, Java


### Compiling

Clone the repo and run `make`. Make sure you have 3-5GB free disk space.

Possible targets to install are:
 * `m68k-toolchain`: GNU toolchain for Motorola 68000 (MegaDrive)
 * `m68k-gdb`: GNU debugger for M68000
 * `sh-toolchain`: GNU toolchain for SH-2 (32X / Saturn)
 * `tools`: A couple Z80 assemblers and various tools I find useful
 * `sgdk`: SGDK and its tools (rescomp, xgmtool, etc)
 * `umdk`: UMDK tools

By default, only the `m68k-toolchain` and `tools` targets are built.

You can change the install path like `make MARSDEV=/where/to/install`. 
But really you can just copy the "mars" directory anywhere and change 
your project's Makefile to point to it.


### How is this different from Gendev?

Added:
 * plugin support for nm, so you can still generate a symbol table with LTO.
 * Most of Sik's tools, and z80asm so Echo can be built.
 * Optional `sh-elf` toolchain for 32X dev.
 * Installs to `${HOME}/mars` by default. No need for superuser.

Not Included:
 * Aplib/Sixpack
 * Newlib
 * Massive archive files


### Examples

The `examples` directory contains various skeleton/example projects to base others on.
Naming convention:
 * `md-`: Bare metal MegaDrive project, requires `m68k-toolchain` and `tools`
 * `sgdk-`: SGDK MegaDrive project, requires `m68k-toolchain`, `tools`, and `sgdk`
 * `mars-`: 32X project, requires `m68k-toolchain`, `sh-toolchain`, and `tools`

Should be as easy as a `make` for any of them.


### TODO

 * 32X Example
 * GDB maybe?
 * SDCC?
