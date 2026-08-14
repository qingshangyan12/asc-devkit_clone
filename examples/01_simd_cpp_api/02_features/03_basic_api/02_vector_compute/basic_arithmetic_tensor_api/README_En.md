# Basic Arithmetic Tensor API Sample

This sample loads float vectors into `reg_tensor<float>`, applies the basic arithmetic APIs, and writes 16 result rows back to GM on `dav-3510`.

```bash
source /usr/local/Ascend/cann-9.2.0/bin/setenv.bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j8
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
