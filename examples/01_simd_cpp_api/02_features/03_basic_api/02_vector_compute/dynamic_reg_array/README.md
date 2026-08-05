# 四路 Reg Value 测试

## 概述

本样例用于验证同时包含 `vector_float` 和 `vector_bool` 的结构体按值返回能力。样例不使用寄存器数组，
而是显式定义 `max0`、`max1`、`max2` 和 `max3`。外层循环执行 4 次，每次展开四组 `add + max`，总计执行
16 次加法。

输入标量由 `input/input.bin` 提供。Kernel 输出 64 个 `float32` 数据到 `output/output.bin`，独立 Python
脚本将其与 `output/golden.bin` 比较。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```text
dynamic_reg_array/
|-- scripts/
|   |-- gen_data.py
|   `-- verify_result.py
|-- CMakeLists.txt
|-- README.md
|-- README_En.md
|-- data_utils.h
|-- dynamic_reg_array.asc
`-- run.sh
```

## 计算逻辑

初始值为 `3.25`。Kernel 连续执行 16 次加 1 操作，其中 `max0` 累积第 1、5、9、13 次结果，`max1` 累积
第 2、6、10、14 次结果。最终比较 `max0` 和 `max1`，因此每个输出元素的真值为 `17.25`。

程序先预热 20 次，再执行 1000 次测量并输出 Host 侧观测到的平均 Kernel 时间。设备侧周期应使用性能分析
工具测量。

## 编译运行

配置 CANN 环境后执行：

```bash
bash run.sh
```

也可以手动执行：

```bash
cmake -S . -B build
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

校验成功后输出：

```text
test pass!
```
