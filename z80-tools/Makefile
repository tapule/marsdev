MARSDEV = $(MARS_BUILD_DIR)
MARSBIN = $(MARSDEV)/m68k-elf/bin

SJASM     = $(MARSBIN)/sjasm
SJBRANCH ?= v0.39

.PHONY: all clean

all: $(SJASM)

$(SJASM): $(MARSBIN)
	rm -rf sjasm
	@echo "> Downloading Sjasm..."
	git clone https://github.com/konamiman/sjasm --depth=1 --branch $(SJBRANCH)
	@echo "> Building Sjasm..."
	cd sjasm/Sjasm && make
	cp -f sjasm/Sjasm/sjasm $(SJASM)

$(MARSBIN):
	mkdir -p $(MARSBIN)

clean:
	rm -rf sjasm
