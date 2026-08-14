# Logical Compute Tensor API 样例

本样例使用 `reg_tensor<int32_t>` 演示逻辑非、按位与/或/异或，以及寄存器和标量两种移位方式。所有整数结果均与 NumPy 逐元素精确比较。

```bash
source /usr/local/Ascend/cann-9.2.0/bin/setenv.bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j8
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
