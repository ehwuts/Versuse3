NAME=Versuse

SOURCE=./src/
OUTPUT=./bin/

CC=g++
CFLAGS=-Wall
LFLAGS=-mwindows

EXE=$(OUTPUT)$(NAME).exe
SOURCES=$(wildcard $(SOURCE)*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
RESOURCES=$(SOURCES:.rc=.res)

build: 
	windres $(SOURCE)versuse.rc -O coff -o $(SOURCE)versuse.res
	g++ -c $(CFLAGS) $(SOURCE)versuse.cpp -o $(SOURCE)versuse.o
	g++ -o $(EXE) $(SOURCE)versuse.o $(SOURCE)versuse.res $(LFLAGS)
	
.PHONY: clean
clean:
	-rm $(SOURCE)*.o $(SOURCE)*.res $(OUTPUT)*