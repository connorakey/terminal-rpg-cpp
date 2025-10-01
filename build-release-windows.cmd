@echo off
REM build-release.cmd
REM Batch script to configure and build a CMake project in Release mode
REM Ensure that you have CMake installed and available in your PATH.
REM Also ensure that you have a suitable compiler installed (e.g., GCC, CLANG, MSVC).

set BUILD_DIR=cmake-build-release

if not exist "%BUILD_DIR%" (
    echo Creating build directory: %BUILD_DIR%
    mkdir %BUILD_DIR%
) else (
    echo Build directory already exists: %BUILD_DIR%
)

echo Configuring project with CMake...
cmake -S . -B %BUILD_DIR% -DCMAKE_BUILD_TYPE=Release

echo Running tests with ctest before build...
pushd %BUILD_DIR%
ctest --output-on-failure
popd

REM Step 3: Build the project
echo Building project...
cmake --build %BUILD_DIR% --config Release

echo.
echo ✅ Tests and build completed!
