#!/bin/bash

echo "Compilando o projeto OpenGL..."

g++ T1.cpp -o corre_coelho.exe -lfreeglut -lopengl32 -lglu32 -lm

if [ $? -eq 0 ]; then
    echo ""
    echo "===================================="
    echo "Compilação bem-sucedida! Rodando..."
    echo "===================================="
    echo ""
    ./corre_coelho.exe
else
    echo ""
    echo "===================================="
    echo "Falha na compilação."
    echo "===================================="
    echo ""
fi