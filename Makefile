all: julia  parser build

julia:
	flex tiny.l

parser:
	bison -d tiny.y

build:
	gcc -c *.c -fno-builtin-exp -Wno-implicit-function-declaration
	gcc *.o -ly -o tiny -fno-builtin-exp

clean:
	rm -f tiny
	rm -f lex.yy.c
	rm -f *.o
	rm -f tiny.tab.*
