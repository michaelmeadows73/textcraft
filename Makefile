textcraft: textcraft.o map.o link.o point.o
	gcc -o textcraft textcraft.o map.o link.o point.o -lncurses

textcraft.o: textcraft.c
	gcc -c textcraft.c -o textcraft.o

map.o: map.h map.c
	gcc -c map.c -o map.o

link.o: link.h link.c
	gcc -c link.c -o link.o

point.o: point.h point.c
	gcc -c point.c -o point.o
