CC=g++
# CXXFLAGS=-O2 -std=c++11 -Wall -pedantic
CXXFLAGS=-O0 -std=c++11 -Wall -pedantic
LDFLAGS= -lpthread

.PHONY: clean all

all: test md5sum_raw

md5sum_raw: md5sum_raw.o md5.o
	$(CC) $(CXXFLAGS) -o md5sum_raw md5sum_raw.o md5.o

test: test.o md5.o Barrier.o
	$(CC) $(CXXFLAGS) -o test test.o md5.o Barrier.o $(LDFLAGS)

md5.o: md5.h
Barrier.o: Barrier.hpp

clean:
	rm test md5sum_raw *.o
    
