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
	@echo "\nCompiling external library(ies)\n"
	gcc lib/Simple-SDL2-Audio/src/audio.c -o tmp/sdl2audio.o -c
	@echo "\nCompiling C...\n"
	cd tmp;\
		gcc $(shell find $(shell pwd)/src/ -print|grep "\.c") -c -g;
	@echo "\nLinking everything...\n"
	gcc $(shell find $(shell pwd)/tmp/ -print|grep "\.o") -o build/acjg -lSDL2 -lSDL2_image -lm