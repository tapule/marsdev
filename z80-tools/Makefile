MARSDEV = $(MARS_BUILD_DIR)
MARSBIN = $(MARSDEV)/m68k-elf/bin

MARS_COLOR_RESET = $'\033[0m'
MARS_COLOR_GREEN = $'\033[1;32;49m'

# Tools required
PREREQ_GIT 		= $(shell which git)
ifeq ($(PREREQ_GIT),)
  $(error 'git' not found. Make sure the 'git' package is installed.)
endif

SJBRANCH ?= v0.39

SJASM_BIN  = $(MARSBIN)/sjasm

LOGDIR    := $(shell pwd)

.PHONY: all sjasm

all: $(MARSBIN) sjasm $(SJASM_BIN)

$(SJASM_BIN): $(MARSBIN) sjasm
	@echo "$(MARS_COLOR_GREEN)>> Building Sjasm...$(MARS_COLOR_RESET)"
	cd sjasm/Sjasm && make > $(LOGDIR)/sjasm.log 2>&1
	cp -f sjasm/Sjasm/sjasm $(SJASM_BIN)

sjasm:
	rm -rf sjasm
	@echo "$(MARS_COLOR_GREEN)>> Downloading Sjasm...$(MARS_COLOR_RESET)"
	git clone https://github.com/konamiman/sjasm --depth=1 --branch $(SJBRANCH)

$(MARSBIN):
	mkdir -p $(MARSBIN)


.PHONY: clean
clean:
	rm -rf sjasm
	rm -f sjasm.log
