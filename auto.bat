@echo off
setlocal

set /p userInput=Enter executable name without extension: 

echo 1 - Building resources
windres resources.rc -O coff -o resources.res

echo 2 - Compiling libs
cd libs
cd minizip
gcc -c *.c
del /f /q miniunz.o minizip.o
cd..
cd zlib
gcc -c *.c
cd..
cd..

echo 3 - Compiling main
g++ -DUNICODE -DNAME="\"%userInput%\"" -Ilibs/zlib -Ilibs/minizip -c main.cpp

echo 4 - Building executable
set /p buildProd=Build release? (Y=release/n=debug): 
if "%buildProd%"=="" set buildProd=Y
if /i "%buildProd%"=="Y" (
  g++ -o %userInput%.exe -mwindows -static-libgcc -static-libstdc++ -static -lwinpthread resources.res main.o libs/zlib/*.o libs/minizip/*.o
) else (
  g++ -o %userInput%.exe resources.res main.o libs/zlib/*.o libs/minizip/*.o
)

pause
