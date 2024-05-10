#!/bin/bash

# Start time
start_time=$(date +%s)

# Step 1: Create build directory
mkdir -p build
echo "Directory 'build' is created."

# Step 2: Run cmake
cmake -S . -B build
echo "CMake configuration completed."

# Step 3: Change to build directory and run make
cd build
make -j
echo "Build process completed."

# End time
end_time=$(date +%s)

# Calculate total time taken
elapsed_time=$((end_time - start_time))
echo "Total time taken: $elapsed_time seconds."
