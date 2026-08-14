# Histogram Compute Tensor API 样例

本样例输入 256 个 `uint8_t` 数据，并分别计算 `bin0/bin1` 的频次直方图和累积直方图。输出为 4 行、每行 128 个 `uint16_t` 结果。

```bash
source /usr/local/Ascend/cann-9.2.0/bin/setenv.bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j8
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
