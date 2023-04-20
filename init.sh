#!/bin/bash

VCPKG_REPO_URL="https://github.com/microsoft/vcpkg.git"
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
VCPKG_DIR="${SCRIPT_DIR}/vcpkg"

if ! command -v git &> /dev/null; then
    echo "git command not found. Please install git and try again."
    exit 1
fi

if [ ! -d "${VCPKG_DIR}" ]; then
    echo "vcpkg not found. Cloning vcpkg repository..."
    git clone "${VCPKG_REPO_URL}" "${VCPKG_DIR}"
fi

echo "Building vcpkg..."
cd "${VCPKG_DIR}"
ls -a
./bootstrap-vcpkg.sh 

echo "Done!"
