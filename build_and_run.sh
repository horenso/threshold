#!/bin/bash
mkdir -p build
cmake --preset default
cmake --build --preset default && (cd build && ./Threshold)
