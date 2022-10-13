CC=g++
CXX=g++

CFLAGS=-g -Wall
CXXFLAGS=-g -Wall

.PHONY: default
default: threading threading_lock threading_cond

threading: threading.o

threading.o: threading.cpp

threading_lock: threading_lock.o

threading_lock.o: threading_lock.cpp

threading_cond: threading_cond.o

threading_cond.0: threading_cond.cpp

.PHONY: clean
clean:
	rm -rf *.o threading_lock threading threading_cond

.PHONY: all
all: clean default