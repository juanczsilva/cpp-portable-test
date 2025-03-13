## build resources
windres resources.rc -O coff -o resources.res

## compile c libs
gcc -c libs/zlib/*.c
gcc -c libs/minizip/*.c

## compile c++ main
g++ -DUNICODE -Izlib -Iminizip -c main.cpp

## build exe debug with console
g++ -o file.exe resources.res main.o libs/zlib/*.o libs/minizip/*.o

## build exe release no console
g++ -o file.exe -mwindows -static-libgcc -static-libstdc++ -static -lwinpthread resources.res main.o libs/zlib/*.o libs/minizip/*.o

## clear, compile main, build exe debug with console, run
cls & g++ -DUNICODE -Izlib -Iminizip -c main.cpp & g++ -o file.exe resources.res main.o libs/zlib/*.o libs/minizip/*.o & file.exe
