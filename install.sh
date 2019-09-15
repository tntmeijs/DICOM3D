#!/bin/bash
echo "== DICOM3D installer ==="

# Generate GL3W files
python dependencies/gl3w/gl3w_gen.py --root=dependencies/gl3w

# Make sure there is a build directory in the project
mkdir -p ./build

# Run CMake for Visual Studio 16 2019 64-bit
cd ./build
cmake .. -G "Visual Studio 16 2019" -A x64

# Wait until the user exits the script
read -n 1 -s -r -p "Press any key to continue"