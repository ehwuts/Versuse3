NAME=Versuse

SOURCE=./src/
OUTPUT=./bin/
CC=g++
CFLAGS=-O2 -Wall
LFLAGS=
RC=windres
RFLAGS=-O coff

EXE=$(OUTPUT)$(NAME)
SRC=$(wildcard $(SOURCE)*.cpp)
RES=$(wildcard $(SOURCE)*.rc)
OBJ=$(SRC:.cpp=.o) $(RES:.rc=.res)

.PHONY: sbuild build debug clean

sbuild: 
	$(RC) $(SOURCE)versuse.rc $(RFLAGS) -o $(SOURCE)versuse.res
	$(CC) $(CFLAGS) $(SOURCE)versuse.cpp -o $(SOURCE)versuse.o	
	$(CC) $(OBJ) -o $@ $(LFLAGS)

build: $(SRC) $(RES) $(EXE)
	
debug: CFLAGS:=$(CFLAGS) -g
debug: build		
	
$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LFLAGS)
	
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@	
	
.rc.res:
	$(RC) $< $(RFLAGS) -o $@
	
clean:
	-rm $(SOURCE)*.o $(SOURCE)*.res $(OUTPUT)*

print-%:
	@echo $* = $($*)
	echo $*\'s origin is $(origin $*)