# build-release.ps1
# PowerShell script to configure and build a CMake project in Release mode

# Ensure that you have CMake installed and available in your PATH.
# Also ensure that you have a suitable compiler installed (e.g., GCC, CLANG, MSVC).
# Also ensure you have Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass set.

$buildDir = "cmake-build-release"
if (-Not (Test-Path -Path $buildDir)) {
    Write-Host "Creating build directory: $buildDir"
    mkdir $buildDir
} else {
    Write-Host "Build directory already exists: $buildDir"
}

Write-Host "Configuring project with CMake..."
cmake -S . -B $buildDir -DCMAKE_BUILD_TYPE=Release

Write-Host "Building project..."
cmake --build $buildDir --config Release

Write-Host "`n✅ Build completed!"
