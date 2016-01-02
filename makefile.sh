windres ./src/versuse.rc -O coff -o ./src/versuse.res
g++ -c -Wall ./src/versuse.cpp -o ./src/versuse.o
g++ -o ./bin/Versuse.exe ./src/versuse.o ./src/versuse.res -mwindows