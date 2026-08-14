# Composite Compute Tensor API Sample

This sample demonstrates `axpy`, `abs_sub`, `exp_sub`, `mul_dst_add`, `mul_add_dst`, and `muls_cast` with float input vectors.

```bash
source /usr/local/Ascend/cann-9.2.0/bin/setenv.bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j8
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
