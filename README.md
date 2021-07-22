# Asteroids

## Installation

Asteroids uses the Tiny Framework. See tiny-fw README for installation.

Download the source.
```bash
git clone https://github.com/jmcarlson0320/asteroids.git
```

In the Makefile, makesure the -L and -I directories point to your system's tiny-fw installation. Default location is ~/lib and ~/include.
```make
CFLAGS = -g -Wall -I/home/jmc/include
LIBS = -L/home/jmc/lib -ltinyfw -lm -lSDL2
```

Compile and run.
```bash
make
./asteroids
```

## Usage

Controls (can be changed in game.c)
```
Z - rotate left
C - rotate right
N - thrust
M - fire
Q - quit

B - toggle software bloom effect
UP/DOWN - adjust bloom spread
LEFT/RIGHT - adjust bloom intensity
```
