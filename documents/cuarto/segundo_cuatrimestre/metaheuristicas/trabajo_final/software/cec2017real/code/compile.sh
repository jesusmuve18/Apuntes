mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make sca -j$(nproc)
cd ..
cp build/sca .