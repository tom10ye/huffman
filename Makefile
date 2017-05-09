# A simple makefile for CSE 100 PA3

CC=g++
CXXFLAGS=-std=c++11 -g
LDFLAGS=-g

all: compress uncompress

compress:  HCNode.o HCTree.o #BitInputStream.o BitOutputStream.o

uncompress:  HCNode.o HCTree.o #BitInputStream.o BitOutputStream.o

HCTree.o:  HCNode.h HCTree.h #BitInputStream.h BitOutputStream.h

HCNode.o: HCNode.h

BitOutputStream.o: BitOutputStream.h

BitInputStream.o: BitInputStream.h

clean:
	rm -f compress uncompress *.o core*

