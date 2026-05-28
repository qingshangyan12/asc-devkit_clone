#!/bin/bash

echo "=== Building LaneID Test ==="

# Create build directory
if [ ! -d "build_laneid" ]; then
    mkdir build_laneid
fi

# Copy CMakeLists file
cp CMakeLists_laneid_test.txt build_laneid/CMakeLists.txt

cd build_laneid

# Configure
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_BUILD_TYPE=Release ..

if [ $? -eq 0 ]; then
    # Build
    make -j
    if [ $? -eq 0 ]; then
        echo "=== Build Successful ==="
        echo "To run the test, execute: ./build_laneid/demo_laneid_test"
    else
        echo "=== Build Failed ==="
        exit 1
    fi
else
    echo "=== CMake Failed ==="
    exit 1
fi