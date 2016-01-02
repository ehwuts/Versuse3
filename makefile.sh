windres ./src/versuse-resource.rc -O coff -o ./src/versuse-resource.res
g++ -c -Wall ./src/versuse-main.cpp -o ./src/versuse-main.o
g++ -o ./bin/Versuse.exe ./src/versuse-main.o ./src/versuse-resource.res -mwindows