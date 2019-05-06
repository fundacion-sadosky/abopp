#!/bin/bash

# Compilamos el cliente, enlazando con libimg.so
clang-8 client.c -g -L. -limg -o client
