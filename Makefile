textcraft: textcraft.o map.o list.o point.o
	gcc -ggdb -o textcraft textcraft.o map.o list.o point.o -lncurses

textcraft.o: textcraft.c
	gcc -ggdb -c textcraft.c -o textcraft.o

map.o: map.h map.c
	gcc -ggdb -c map.c -o map.o

list.o: list.h list.c
	gcc -ggdb -c list.c -o list.o

point.o: point.h point.c
	gcc -ggdb -c point.c -o point.o
