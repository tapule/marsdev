MARSDEV = $(MARS_BUILD_DIR)

MARS_COLOR_RESET = $'\033[0m'
MARS_COLOR_GREEN = $'\033[1;32;49m'

# Tools required
PREREQ_GIT	 		= $(shell which git)
PREREQ_HG	 		= $(shell which hg)
PREREQ_LDCONFIG		= $(wildcard /sbin/ldconfig)
PREREQ_LIBASOUND 	= $(shell /sbin/ldconfig -p | grep libasound.so)
PREREQ_LIBPULSE 	= $(shell /sbin/ldconfig -p | grep libpulse.so)

ifeq ($(PREREQ_GIT),)
  $(error 'git' not found. Make sure the 'git' package is installed.)
endif
ifeq ($(PREREQ_HG),)
  $(error 'hg' not found. Make sure the 'mercurial' package is installed.)
endif
ifeq ($(PREREQ_LDCONFIG),)
  ifeq ($(PREREQ_LIBASOUND),)
    $(info ** 'libasound' not found. MAke sure the 'libasound2-dev' package is installed.)
  endif
  ifeq ($(PREREQ_LIBPULSE),)
    $(info ** 'libpulse' not found. MAke sure the 'libpulse-dev' package is installed.)
  endif
else
  $(info ** Ensure you have 'libasound2-dev' and 'libpulse-dev' packages installed. **)
endif

BLASTEM_DIR 	:= blastem
BLASTEM_BIN 	:= $(BLASTEM_DIR)/blastem
BLASTEM_VER 	 = $(shell echo $(shell ./$(BLASTEM_BIN) -v) | awk '/blastem/ { gsub(/\r/, "", $$2); print $$2 }')
BLASTEM_SUFIX 	 = $(shell file $(BLASTEM_BIN) | sed -E 's/^[^:]*: [^ ]* ([0-9]*)-bit .*/\1/')

.PHONY: all blastem mk-blastem blastem_pkg sdl glew clean

all: blastem

blastem: mk-blastem 
	cp -f -r $(BLASTEM_DIR)/blastem$(BLASTEM_SUFIX)-$(BLASTEM_VER) $(MARSDEV)/blastem

mk-blastem: clean blastem_pkg sdl glew
	@echo "$(MARS_COLOR_GREEN)>> Building Blastem...$(MARS_COLOR_RESET)"
	cd $(BLASTEM_DIR) && sh build_release > ../blastem.log 2>&1

sdl: blastem_pkg
	@echo "$(MARS_COLOR_GREEN)>> Downloading SDL2...$(MARS_COLOR_RESET)"
	git clone --branch SDL2 --recurse-submodules https://github.com/libsdl-org/SDL $(BLASTEM_DIR)/sdl
	
glew: blastem_pkg
	@echo "$(MARS_COLOR_GREEN)>> Downloading Glew...$(MARS_COLOR_RESET)"
	git clone https://github.com/nigels-com/glew.git $(BLASTEM_DIR)/glew
	cd $(BLASTEM_DIR)/glew/auto && make > ../../../glew.log 2>&1

blastem_pkg:
	@echo "$(MARS_COLOR_GREEN)>> Downloading Blastem...$(MARS_COLOR_RESET)"
	hg clone https://www.retrodev.com/repos/blastem -r tip $(BLASTEM_DIR)

clean:
	rm -rf $(BLASTEM_DIR)
	rm -f blastem.log glew.log
