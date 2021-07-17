CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include -DDEBUG
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = asteroids

$(EXE): main.o list.o particle.o ship.o explosion.o ship_explosion.o utils.o asteroid.o game.o title.o play.o reset.o gameover.o highscore.o test.o start.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

wf: wf_util.o list.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

list.o: list.h
particle.o: particle.h
ship.o: defs.h
explosion.o: defs.h
ship_explosion.o: defs.h
utils.o: defs.h
asteroid.o: defs.h
game.o: defs.h
title.o: defs.h
play.o: defs.h
reset.o: defs.h
gameover.o: defs.h
highscore.o: defs.h
test.o: defs.h
start.o: defs.h
wf_util.o: list.h

clean:
	rm *.o $(EXE) wf

.PHONY: clean
