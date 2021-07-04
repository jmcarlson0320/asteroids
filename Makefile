CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = asteroids

$(EXE): main.o list.o particle.o ship.o utils.o asteroid.o game.o title.o play.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

wf: wf_util.o list.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

list.o: list.h
particle.o: particle.h
ship.o: defs.h
utils.o: defs.h
asteroid.o: defs.h
game.o: defs.h
title.o: defs.h
play.o: defs.h
wf_util.o: list.h

clean:
	rm *.o $(EXE) wf

.PHONY: clean
