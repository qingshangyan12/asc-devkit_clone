cd /data/hxj/code/2/asc-devkit_0616/examples/01_simd_cpp_api/05_best_practices/04_memory_access/cache_consistency_verify_c_api
source /usr/local/Ascend/cann/set_env.sh
mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
make -j
./demo_c_api
python3 ../scripts/verify_result.py output/output.bin
