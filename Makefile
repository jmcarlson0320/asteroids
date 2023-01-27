CC = gcc
CFLAGS = -g -Wall -I/Users/jmc/devel/include -I/opt/homebrew/include
LIBS = -L/Users/jmc/devel/lib -L/opt/homebrew/lib -ltinyfw -lm -lSDL2
EXE = asteroids

$(EXE): main.o list.o particle.o ship.o explosion.o ship_explosion.o utils.o asteroid.o game.o title.o play.o reset.o gameover.o highscore.o test.o start.o bloom.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

wf: wf_util.o list.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

run: $(EXE)
	./$(EXE)

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
bloom.o: bloom.h defs.h

clean:
	rm *.o $(EXE) wf

.PHONY: clean
