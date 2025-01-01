#!/bin/bash
mkdir -p build
cmake --build --preset default && (cd build && ./Threshold)
