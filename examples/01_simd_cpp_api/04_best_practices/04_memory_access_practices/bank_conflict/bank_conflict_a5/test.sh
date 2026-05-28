rm -rf build
mkdir -p build && cd build
cmake ..  -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 && make -j
python3 ../scripts/gen_data.py
./demo