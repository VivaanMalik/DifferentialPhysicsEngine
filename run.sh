#!/bin/bash

# Ensure we are in the script's directory
cd "$(dirname "$0")"

# Colors for terminal output
CYAN='\033[0;36m'
GREEN='\033[0;32m'
NC='\033[0m'

echo -e "${CYAN}--- Building Engine ---${NC}"

# 1. Check for build folder
if [ ! -d "build" ]; then
    mkdir build
fi

# 2. Compile
# -j4 uses 4 cores for faster compilation
make -j4

# 3. Check if build was successful
if [ $? -eq 0 ]; then
    echo -e "${GREEN}--- Running Simulation ---${NC}"
    ./build/diff_engine
else
    echo -e "\033[0;31mBuild Failed!\033[0m"
    exit 1
fi