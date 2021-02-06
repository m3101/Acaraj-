#
#   Acarajé - A minimalist swordfighting game
#   Copyright (C) 2020 Amélia O. F. da S.
#   Reachable at a.mellifluous.one@gmail.com
#    or www.github.com/m3101
#
#    Full copyright notice in /src/main.c
#

help:
	@echo "Usage:"
	@echo "make tests      - Builds a debug build."
lib:
	mkdir lib
	cd lib;git clone https://github.com/m3101/Simple-SDL2-Audio.git
GLEW=""
GLEWC=""
TTS=""
TTSC=""
tests:
	make lib
	@if [ -d build ]; then\
		rm -rf build;\
	fi
	@if [ -d tmp ]; then\
		rm -rf tmp;\
	fi
	@mkdir build
	@mkdir tmp
	@echo "\nGenerating packed resources...\n"
	cd src/PackedResources;make
	@echo "\nGenerating packed shaders\n"
	ld -r -b binary src/shaders/vhs.vert -o tmp/vhsV.o
	objdump tmp/vhsV.o -x|grep binary
	ld -r -b binary src/shaders/vhs.frag -o tmp/vhsF.o
	objdump tmp/vhsF.o -x|grep binary

	@echo "\nTesting for required external libraries\n"
	@echo "SDL (SDL,SDL_ttf,SDL_opengl)..."
	@if gcc makefile_test_files/SDL_test.c -lSDL2 -lSDL2_image -lSDL2_ttf -lGL -o makefile_test_files/out.o;then \
		echo "Ok.";\
	else \
		echo "Not found!";\
		error SDL2 dependencies not installed.;\
	fi;

	@echo "\nTesting for optional external libraries\n"
	@echo "gl3w... "
	@if [ -d /usr/local/share/gl3w ];then \
		g++ -c /usr/local/share/gl3w/gl3w.c -o tmp/gl3w.o -g;\
		gcc makefile_test_files/gl3w_test.c -o makefile_test_files/test.o -c;\
		if gcc makefile_test_files/test.o tmp/gl3w.o -lGL -ldl -o makefile_test_files/out.o;then \
			echo $(eval GLEW=-lGL);\
			echo $(eval GLEWC=-DAC_USE_GL3W);\
			echo "Available! Compiling with custom shaders.";\
		fi;\
	fi
	@rm makefile_test_files/*.o
	@export TTS=""
	@export TTSC=""
	@echo "espak-ng TTS engine"
	@if gcc makefile_test_files/espeak_tts_test.c -lespeak-ng -o makefile_test_files/out.o;then \
		echo $(eval TTS=-lespeak-ng);\
		echo $(eval TTSC=-DAC_USE_TTS);\
		echo "Ok.";\
	else \
		echo "espeak-ng not available. Disabling speech synthesis.";\
	fi;

	@echo "\nCompiling included external library(ies)\n"
	gcc lib/Simple-SDL2-Audio/src/audio.c -o tmp/sdl2audio.o -c

	@echo "\nCompiling C...\n"
	cd tmp;\
		gcc $(shell find $(shell pwd)/src/ -print|grep "\.c") -c -g $(GLEWC) $(TTSC);
	make link GLEW=$(GLEW) TTS=$(TTS)
	rm -rf tmp
link:
	@echo "\nLinking everything...\n"
	gcc $(shell find $(shell pwd)/tmp/ -print|grep "\.o") -o build/acjg -lSDL2 -lSDL2_image -lSDL2_ttf -lm -ldl $(GLEW)  $(TTS)