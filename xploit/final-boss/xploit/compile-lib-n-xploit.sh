#!/bin/bash

# Compilamos nuestra versión de libimg.so
clang-8 libimg-custom.c -shared -fPIC -o libimg.so

# Compilamos el cliente, enlazando con libimg.so
clang-8 xploit.c -g -L. -limg -o xploit
