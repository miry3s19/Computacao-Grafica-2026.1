#!/bin/bash

echo "Compilando o projeto OpenGL..."

g++ T1.cpp -o corre_coelho -lglut -lGL -lGLU -lm

if [ $? -eq 0 ]; then
    echo ""
    echo "===================================="
    echo "Compilação bem-sucedida! Rodando..."
    echo "===================================="
    echo ""
    ./corre_coelho
else
    echo ""
    echo "===================================="
    echo "Falha na compilação."
    echo "===================================="
    echo ""
fi