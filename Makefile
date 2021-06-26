CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = app

$(EXE): main.o list.o particle.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

list.o: list.h
particle.o: particle.h

clean:
	rm *.o $(EXE)

.PHONY: clean
