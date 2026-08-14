# Composite Compute Tensor API 样例

本样例集中展示 6 个复合计算接口：`axpy`、`abs_sub`、`exp_sub`、`mul_dst_add`、`mul_add_dst` 和 `muls_cast`。输入使用两组易于核对的 float 数据，输出逐行与 NumPy 结果比较。

```bash
source /usr/local/Ascend/cann-9.2.0/bin/setenv.bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j8
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
