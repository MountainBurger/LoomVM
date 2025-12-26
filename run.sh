#!/bin/bash

mkdir -p build

echo "Compiling source files..."
g++ -std=c++17 -g -Iinclude src/* -o build/loom

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful."
    echo "Running program..."
    echo
    ./build/loom
else
    echo "Build failed!"
fi
