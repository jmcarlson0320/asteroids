CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = asteroids

$(EXE): main.o list.o particle.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

wf: wf_util.o list.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

list.o: list.h
particle.o: particle.h
wf_util.o: list.h

clean:
	rm *.o $(EXE) wf

.PHONY: clean
