NAME=AlliterativeApples

SOURCE=./src/
OUTPUT=./bin/

CC=g++
CFLAGS=-c -Wall
LFLAGS=-lws2_32

EXE=$(OUTPUT)$(NAME)
SOURCES=$(wildcard $(SOURCE)*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

build: 
	windres versuse.rc -O coff -o versuse.res
	g++ -c -Wall versuse.cpp	
	g++ -o versuse.exe versuse.o versuse.res