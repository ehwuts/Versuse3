NAME=Versuse

SOURCE=./src/
OUTPUT=./bin/

CC=g++
CFLAGS=-c -Wall
LFLAGS=

EXE=$(OUTPUT)$(NAME).exe
SOURCES=$(wildcard $(SOURCE)*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
RESOURCES=$(SOURCES:.rc=.res)

build: 
	windres $(SOURCE)versuse.rc -O coff -o $(SOURCE)versuse.res
	g++ -c -Wall $(SOURCE)versuse.cpp	
	g++ -o $(EXE) $(SOURCE)versuse.o $(SOURCE)versuse.res
	
.PHONY: clean
clean:
	-rm $(SOURCE)*.o $(SOURCE)*.res $(OUTPUT)*