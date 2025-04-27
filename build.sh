#!/bin/bash
set -eo pipefail  # Exit on error, undefined var, pipeline failure

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Build configuration
BUILD_TYPE=${BUILD_TYPE:-Release}  # Default to Release, can be overridden
BUILD_DIR="build"

# Check required dependencies
check_dependency() {
    if ! command -v "$1" &> /dev/null; then
        echo -e "${RED}Error: $1 could not be found. Please install it first.${NC}"
        exit 1
    fi
}

# Main build function
build() {
    echo -e "${GREEN}Configuring CMake project...${NC}"
    cmake -S . -B "${BUILD_DIR}" \
          -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

    echo -e "${GREEN}Building project...${NC}"
    cmake --build "${BUILD_DIR}" --config "${BUILD_TYPE}" -j "$(nproc)"
}

# Check dependencies
check_dependency cmake

# Clean build directory if requested
if [ "$1" = "clean" ]; then
    echo -e "${GREEN}Cleaning build directory...${NC}"
    rm -rf "${BUILD_DIR}"
fi

# Execute build
build

echo -e "${GREEN}Build completed successfully!${NC}"
