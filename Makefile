CC = gcc
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
EXE = app

$(EXE): main.o list.o
	$(CC) $(INCLUDES) -o $@ $^ $(LIBS)

list.o: list.h

clean:
	rm *.o $(EXE)

.PHONY: clean
