test:
	for res in $(shell find . -not -name "*.h"|grep "/"|grep -o '(?<=/).*\.*' -P);do\
		ld -r -b binary $$res -o "../../tmp/$$res.o";\
	done