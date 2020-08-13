#
#   Acarajé - A minimalist swordfighting game
#   Copyright (C) 2020 Amélia O. F. da S.
#   Reachable at a.mellifluous.one@gmail.com
#    or www.github.com/m3101
#
#    Full copyright notice in /src/main.c
#

test:
	for res in $(shell find . -not -name "*.h"|grep "/"|grep -o '(?<=/).*\.*' -P);do\
		ld -r -b binary $$res -o "../../tmp/$$res.o";\
		objdump -x "../../tmp/$$res.o"|grep binary;\
	done