#!/bin/zsh

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure CMake
cmake ..

# Build all targets
cmake --build .

# Run all tests with detailed output
ctest --output-on-failure --verbose

# Return the exit code from ctest
exit $?
