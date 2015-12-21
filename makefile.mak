NAME=Versuse

SOURCE=./src/
OUTPUT=./bin/

CC=g++
CFLAGS=-c -Wall
LFLAGS=

EXE=$(OUTPUT)$(NAME)
SOURCES=$(wildcard $(SOURCE)*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
RESOURCES=$(SOURCES:.rc=.res)

#build: 
#	windres versuse.rc -O coff -o versuse.res
#	g++ -c -Wall versuse.cpp	
#	g++ -o versuse.exe versuse.o versuse.res

build: $(SOURCES) $(RESOURCES) $(EXE)
	
debug: CFLAGS:=$(CFLAGS) -g
debug: $(SOURCES) $(EXE)	
	
$(EXE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)
	
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@	
	
.rc.res:
	windres -O coff $< -o $@
	
.PHONY: clean
clean:
	rm $(SOURCE)*.o $(OUTPUT)*