## convertir binario a bytes (metodo antiguo)
xxd -i funnyvoice.zip > archivo_binario_b.h

## generar archivo de recursos
windres resources.rc -O coff -o resources.res

## compilar librerias c
gcc -c zlib/*.c
gcc -c minizip/*.c

## compilar c++
g++ -DUNICODE -Izlib -Iminizip -c main.cpp

## debug consola
g++ -o ejecutable.exe resources.res main.o zlib/*.o minizip/*.o

## release no consola
g++ -o ejecutable.exe -mwindows resources.res main.o zlib/*.o minizip/*.o

## limpiar consola, compilar c++, generar ejecutable debug consola, ejecutar
cls & g++ -DUNICODE -Izlib -Iminizip -c main.cpp & g++ -o ejecutable.exe resources.res main.o zlib/*.o minizip/*.o & ejecutable.exe

## limpiar consola, compilar c++, generar ejecutable release no consola, ejecutar
cls & g++ -DUNICODE -Izlib -Iminizip -c main.cpp & g++ -o ejecutable.exe -mwindows resources.res main.o zlib/*.o minizip/*.o & ejecutable.exe
