BLASTEM_DIR = blastem
PYTHON = python2

.PHONY: all blastem blastem_pkg sdl glew clean

all: blastem

#blastem_pkg sdl glew
blastem: 
	#cd $(BLASTEM_DIR) && sh build_release
	echo "+++ Copying $(BLASTEM_DIR)..."
	export VERSTR=`./blastem/blastem -v`
	export VER=`echo $(VERSTR) | awk '/blastem/ { gsub(/\r/, "", $2); print $2 }'`
	echo $(VER) 
	#cp -f -r `./$(BLASTEM_DIR)/blastem -v` $(MARSDEV)/blastem

sdl: blastem_pkg
	git clone https://github.com/libsdl-org/SDL $(BLASTEM_DIR)/sdl
	
glew: blastem_pkg
	git clone https://github.com/nigels-com/glew.git $(BLASTEM_DIR)/glew
	cd $(BLASTEM_DIR)/glew/auto &&	make PYTHON=$(PYTHON)

blastem_pkg: clean
	hg clone https://www.retrodev.com/repos/blastem $(BLASTEM_DIR)
	sed -i 's/env python/env $(PYTHON)/g' $(BLASTEM_DIR)/img2tiles.py

clean:
	rm -rf $(BLASTEM_DIR) 