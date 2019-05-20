#!/bin/bash

# Compilamos el servidor con SafeStack y CFI.
clang++-8 -g imgserver.cpp imgserver-handler.cpp imgserver-types.cpp -pie -fstack-protector -fsanitize=safe-stack -fsanitize=cfi -flto -fvisibility=hidden -Wl,-z,relro,-z,now -o imgserver

# Compilamos la biblioteca.
clang-8 -g libimg.c -shared -fPIC -o libimg.so
