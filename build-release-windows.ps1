# build-release.ps1
# PowerShell script to run tests first, then build the project in Release mode

$buildDir = "cmake-build-release"
if (-Not (Test-Path -Path $buildDir)) {
    Write-Host "Creating build directory: $buildDir"
    mkdir $buildDir
} else {
    Write-Host "Build directory already exists: $buildDir"
}

Write-Host "Configuring project with CMake..."
cmake -S . -B $buildDir -DCMAKE_BUILD_TYPE=Release

Write-Host "Running tests with ctest before build..."
Push-Location $buildDir
ctest --output-on-failure
Pop-Location

Write-Host "Building project..."
cmake --build $buildDir --config Release

Write-Host "✅ Tests and build completed!"
