# Basic Arithmetic Tensor API 样例

本样例在 `dav-3510` 上演示寄存器基础算术接口。输入向量从 GM 搬入 UB，加载为 `reg_tensor<float>` 后依次执行一元数学、激活和二元算术运算，最后将 16 行结果写回 GM。

核心调用集中在 `compute` 中，覆盖 `abs`、`exp`、`sqrt`、`log`、`log2`、`log10`、一元负号、`relu`、`prelu`、`leaky_relu`、加减乘除以及 `min/max`。

```bash
source /usr/local/Ascend/cann-9.2.0/bin/setenv.bash
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j8
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```
