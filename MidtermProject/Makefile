# Intermediate Programming 601.220 Midterm Project 10/06/2017
# Randy Kuang rkuang2 rkuang2@jhu.edu
# Justin Huang jhuan104 jhuan104@jhu.edu

# Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g -O2

all: main 

main: imageManip.o menuUtil.o ppmIO.o project.o
	$(CC) -O2 -o main imageManip.o menuUtil.o ppmIO.o project.o -lm

project.o: project.c imageManip.h menuUtil.h ppmIO.h
	$(CC) $(CFLAGS) -c project.c

ppmIO.o: ppmIO.c imageManip.h menuUtil.h ppmIO.h
	$(CC) $(CFLAGS) -c ppmIO.c

menuUtil.o: menuUtil.c imageManip.h menuUtil.h ppmIO.h
	$(CC) $(CFLAGS) -c menuUtil.c

imageManip.o: imageManip.c imageManip.h menuUtil.h ppmIO.h
	$(CC) $(CFLAGS) -c imageManip.c

# not sure why clean isn't working right now
clean:
	rm -f *.o
