textcraft: textcraft.o attack.o flee.o peasant.o getgold.o getwood.o move.o command.o entity.o map.o team.o list.o point.o
	gcc -ggdb -o textcraft textcraft.o attack.o flee.o peasant.o getgold.o getwood.o move.o command.o entity.o map.o team.o list.o point.o -lncurses

textcraft.o: textcraft.c
	gcc -ggdb -c textcraft.c -o textcraft.o

attack.o: attack.h attack.c
	gcc -ggdb -c attack.c -o attack.o

flee.o: flee.h flee.c
	gcc -ggdb -c flee.c -o flee.o

peasant.o: peasant.h peasant.c
	gcc -ggdb -c peasant.c -o peasant.o

getgold.o: getgold.h getgold.c
	gcc -ggdb -c getgold.c -o getgold.o

getwood.o: getwood.h getwood.c
	gcc -ggdb -c getwood.c -o getwood.o

move.o: move.h move.c
	gcc -ggdb -c move.c -o move.o

command.o: command.h command.c
	gcc -ggdb -c command.c -o command.o

entity.o: entity.h entity.c
	gcc -ggdb -c entity.c -o entity.o

map.o: map.h map.c
	gcc -ggdb -c map.c -o map.o

team.o: team.h team.c
	gcc -ggdb -c team.c -o team.o

list.o: list.h list.c
	gcc -ggdb -c list.c -o list.o

point.o: point.h point.c
	gcc -ggdb -c point.c -o point.o
