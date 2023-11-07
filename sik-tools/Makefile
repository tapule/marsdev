MARSDEV = $(MARS_BUILD_DIR)
MARSBIN = $(MARSDEV)/m68k-elf/bin

MARS_COLOR_RESET = $'\033[0m'
MARS_COLOR_GREEN = $'\033[1;32;49m'

# Tools required
PREREQ_GIT 		= $(shell which git)
ifeq ($(PREREQ_GIT),)
  $(error 'git' not found. Make sure the 'git' package is installed.)
endif

MDTILER  = $(MARSBIN)/mdtiler
MIDI2ESF = $(MARSBIN)/midi2esf
MML2ESF  = $(MARSBIN)/mml2esf
TFI2EIF  = $(MARSBIN)/tfi2eif
VGI2EIF  = $(MARSBIN)/vgi2eif
EIF2TFI  = $(MARSBIN)/eif2tfi
PCM2EWF  = $(MARSBIN)/pcm2ewf
ECHO2VGM = $(MARSBIN)/echo2vgm
SLZ      = $(MARSBIN)/slz
UFTC     = $(MARSBIN)/uftc
HEADGEN  = $(MARSBIN)/headgen
ROMFIX   = $(MARSBIN)/romfix

LOGDIR    := $(shell pwd)

.PHONY: all

all: $(MDTILER)
all: $(MIDI2ESF) $(MML2ESF) $(TFI2EIF) $(VGI2EIF) $(EIF2TFI) $(PCM2EWF) $(ECHO2VGM)
all: $(SLZ) $(UFTC)
all: $(HEADGEN) $(ROMFIX)

mdtools:
	@echo "$(MARS_COLOR_GREEN)>> Downloading Sik tools...$(MARS_COLOR_RESET)"
	git clone https://github.com/sikthehedgehog/mdtools

$(MDTILER): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building mdtiler...$(MARS_COLOR_RESET)"
	make -C mdtools/mdtiler/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/mdtiler/tool/mdtiler $(MDTILER)

$(MIDI2ESF): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building midi2esf...$(MARS_COLOR_RESET)"
	make -C mdtools/midi2esf >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/midi2esf/midi2esf $(MIDI2ESF)

$(MML2ESF): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building mml2esf...$(MARS_COLOR_RESET)"
	make -C mdtools/mml2esf/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/mml2esf/tool/mml2esf $(MML2ESF)

$(TFI2EIF): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building tfi2eif...$(MARS_COLOR_RESET)"
	make -C mdtools/tfi2eif/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/tfi2eif/tool/tfi2eif $(TFI2EIF)

$(VGI2EIF): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building vgi2eif...$(MARS_COLOR_RESET)"
	make -C mdtools/vgi2eif/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/vgi2eif/tool/vgi2eif $(VGI2EIF)

$(EIF2TFI): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building eif2tfi...$(MARS_COLOR_RESET)"
	make -C mdtools/eif2tfi/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/eif2tfi/tool/eif2tfi $(EIF2TFI)

$(PCM2EWF): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building pcm2ewf...$(MARS_COLOR_RESET)"
	make -C mdtools/pcm2ewf/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/pcm2ewf/tool/pcm2ewf $(PCM2EWF)

$(ECHO2VGM): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building echo2vgm...$(MARS_COLOR_RESET)"
	$(MAKE) -C mdtools/echo2vgm >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/echo2vgm/echo2vgm $(ECHO2VGM)

$(SLZ): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building slz...$(MARS_COLOR_RESET)"
	make -C mdtools/slz/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/slz/tool/slz $(SLZ)

$(UFTC): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building uftc...$(MARS_COLOR_RESET)"
	make -C mdtools/uftc/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/uftc/tool/uftc $(UFTC)

$(HEADGEN): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building headgen...$(MARS_COLOR_RESET)"
	make -C mdtools/headgen/tool >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/headgen/tool/headgen $(HEADGEN)

$(ROMFIX): mdtools $(MARSBIN)
	@echo "$(MARS_COLOR_GREEN)>> Building romfix...$(MARS_COLOR_RESET)"
	make -C mdtools/romfix >> $(LOGDIR)/sik-tools.log 2>&1
	cp -f mdtools/romfix/romfix $(ROMFIX)
 
$(MARSBIN):
	mkdir -p $(MARSBIN)

.PHONY: clean
clean:
	rm -rf mdtools
	rm -f sik-tools.log
