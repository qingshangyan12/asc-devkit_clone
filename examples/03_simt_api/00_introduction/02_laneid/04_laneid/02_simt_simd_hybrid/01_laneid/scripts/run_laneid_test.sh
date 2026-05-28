#!/bin/bash

echo "=== Running LaneID Test ==="

# Create output directory if it doesn't exist
if [ ! -d "output" ]; then
    mkdir -p output
fi

# Run the test
if [ -f "build_laneid/demo_laneid_test" ]; then
    ./build_laneid/demo_laneid_test
else
    echo "Error: demo_laneid_test not found. Please build the project first."
    exit 1
fi