CC=g++
CXX=g++

CFLAGS=-g -Wall
CXXFLAGS=-g -Wall

.PHONY: default
default: threading threading_lock

threading: threading.o

threading.o: threading.cpp

threading_lock: threading_lock.o

threading_lock.o: threading_lock.cpp

.PHONY: clean
clean:
	rm -rf *.o threading_lock threading

.PHONY: all
all: clean default