#!/bin/bash
cmake --build --preset default && (cd build && ./Threshold)
