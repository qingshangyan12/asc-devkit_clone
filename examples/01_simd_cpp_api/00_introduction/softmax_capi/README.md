# Softmax C API 样例

## 概述

本样例基于 SIMD C API 实现按行 Softmax 计算。输入为 `float32` 矩阵，形状为 `[81920, 128]`，输出与输入形状一致。

## 目录结构

```text
softmax_capi
|-- CMakeLists.txt
|-- README.md
|-- data_utils.h
|-- softmax.asc
|-- softmax.h
`-- scripts
    |-- gen_data.py
    `-- verify_result.py
```

## 编译运行

在本样例目录下执行：

```bash
mkdir -p build && cd build
cmake ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

执行成功后会看到：

```bash
test pass!
```
