.PHONY: all clean test

LDEXTRAFLAGS ?=

all: main

bar.o: bar_so.cc
	gcc -c -Wall -fpic -o $@ $<

libbar.so: bar.o
	gcc -shared -o $@ $<

main.o: main.cc
	gcc -c -fpermissive -o $@ $<

main_dlopen: main_dlopen.cc
	gcc -fpermissive $(LDEXTRAFLAGS) -o $@ $< -L. -ldl

main: main.o libbar.so main_dlopen
	gcc $(LDEXTRAFLAGS) -o $@ $< -L. -lbar

clean:
	rm -f *.o *.so main main_dlopen

test: main
	./$<
