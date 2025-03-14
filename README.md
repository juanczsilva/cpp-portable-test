<https://juanczsilva.github.io/?id=10>

------------------------------------

## automatic
### build resources, compile libs and main, build exe
> auto.bat

## manual
### build resources
> windres resources.rc -O coff -o resources.res

### compile c libs
> gcc -c libs/zlib/\*.c

> gcc -c libs/minizip/\*.c

### compile c++ main
> g++ -DUNICODE -Ilibs/zlib -Ilibs/minizip -c main.cpp

### build exe debug with console
> g++ -o file.exe resources.res main.o libs/zlib/\*.o libs/minizip/\*.o

### build exe release no console
> g++ -o file.exe -mwindows -static-libgcc -static-libstdc++ -static -lwinpthread resources.res main.o libs/zlib/\*.o libs/minizip/\*.o
