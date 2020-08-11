help:
	@echo "Usage:"
	@echo "make tests      - Builds a debug build."
tests:
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
	@echo "\nCompiling C...\n"
	cd tmp;\
		gcc $(shell find $(shell pwd)/src/ -print|grep "\.c") -c -g;
	@echo "\nLinking everything...\n"
	gcc $(shell find $(shell pwd)/tmp/ -print|grep "\.o") -o build/acjg -lSDL2 -lSDL2_image -lm