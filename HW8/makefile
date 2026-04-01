all: calc

calc: libcalc.so
	gcc main.c -L. -lcalc -o calc

libcalc.so:
	gcc -fPIC -c add.c sub.c mul.c div.c
	gcc -shared add.o sub.o mul.o div.o -o libcalc.so

clean:
	rm -f *.o libcalc.so calc

run:
	LD_LIBRARY_PATH=$$PWD:$$LD_LIBRARY_PATH  ./calc