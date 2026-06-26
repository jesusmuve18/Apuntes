#!/usr/bin/env bash
set -euo pipefail

if [ $# -ne 1 ]; then
    echo "Uso: $0 <target>  (ej: $0 sca  o  $0 sca_ls)"
    exit 1
fi

TARGET="$1"

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make "$TARGET" -j"$(nproc)"
cd ..
cp "build/$TARGET" .