#!/bin/sh
# build-release.sh
# POSIX-compliant shell script to run tests first, then build the project in Release mode
# Ensure that you have CMake installed and available in your PATH.

BUILD_DIR="cmake-build-release"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory: $BUILD_DIR"
    mkdir "$BUILD_DIR"
else
    echo "Build directory already exists: $BUILD_DIR"
fi

echo "Configuring project with CMake..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release

echo "Running tests with ctest before build..."
cd "$BUILD_DIR"
ctest --output-on-failure
cd -

echo "Building project..."
cmake --build "$BUILD_DIR" --config Release

echo "✅ Tests and build completed!"
