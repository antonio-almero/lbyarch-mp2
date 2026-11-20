@echo off

nasm -f win64 saxpyASM.asm -o saxpyASM.obj
gcc main.c saxpyASM.obj -o main.exe