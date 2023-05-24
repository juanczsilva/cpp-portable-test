## convertir binario a bytes (metodo antiguo)
xxd -i funnyvoice.zip > archivo_binario_b.h

## generar archivo de recursos
windres resources.rc -O coff -o resources.res

## compilar librerias c
gcc -c zlib/*.c
gcc -c minizip/*.c

## compilar c++
g++ -DUNICODE -Izlib -Iminizip -c main.cpp

## generar ejecutable debug con consola
g++ -o ejecutable.exe resources.res main.o zlib/*.o minizip/*.o

## generar ejecutable release no consola
g++ -o ejecutable.exe -mwindows -static-libgcc -static-libstdc++ -static -lwinpthread resources.res main.o zlib/*.o minizip/*.o

## limpiar terminal, compilar c++, generar ejecutable debug con consola, ejecutar
cls & g++ -DUNICODE -Izlib -Iminizip -c main.cpp & g++ -o ejecutable.exe resources.res main.o zlib/*.o minizip/*.o & ejecutable.exe
