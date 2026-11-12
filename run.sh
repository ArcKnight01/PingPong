#!/usr/bin/env bash
# Build (if needed) and run PingPong.
#   ./run.sh          -> build and play
#   ./run.sh --clean  -> wipe build/ first, then build and play
set -euo pipefail

cd "$(dirname "$0")"

if [[ "${1:-}" == "--clean" ]]; then
  rm -rf ./build
fi

if ! command -v cmake >/dev/null; then
  echo "cmake not found. Install with: sudo apt install build-essential cmake libsfml-dev" >&2
  exit 1
fi

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug >/dev/null
cmake --build build -j"$(nproc)"

cd build   # the game loads Font/Candal.ttf relative to here
./PingPong
