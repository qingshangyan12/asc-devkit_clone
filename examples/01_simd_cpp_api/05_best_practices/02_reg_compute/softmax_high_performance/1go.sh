rm -rf build
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCMAKE_VF_MODE=true
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin