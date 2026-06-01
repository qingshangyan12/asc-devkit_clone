# muls 算子直调样例

## 概述

本样例演示使用 Ascend C 基础 API 的 `MulsCast` 实现向量乘标量并类型转换（float->half）操作。

数学公式：
```
out(half) = src(float) * scalar(float)
```

**注意**：tensor_api 的 `Transform<Inst::Muls>` 是 asc-devkit 仓库中的新功能，尚未发布到 CANN 包中。本示例使用已发布的基础 API 来演示相同的概念。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── muls/
│   ├── scripts/
│   │   ├── gen_data.py              // 输入数据和真值数据生成脚本
│   │   └── verify_result.py         // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── data_utils.h                 // 数据读入写出函数
│   ├── run.sh                       // 一键运行脚本
│   └── muls.asc                // Ascend C算子实现 & 调用样例
```

## 算子描述
- 算子功能：
  使用 Ascend C 的 `MulsCast` API 实现向量乘标量并类型转换（float->half）。

  对应的数学表达式为：
  ```
  out(half) = src(float) * scalar(float)
  ```

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="5" align="center"> MulsCast </td></tr>
  <tr><td rowspan="2" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">default</td></tr>
  <tr><td align="center">x</td><td align="center">512</td><td align="center">float32</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">out</td><td align="center">512</td><td align="center">float16</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td rowspan="1" align="center">标量</td><td align="center">scalar</td><td align="center">-</td><td align="center">float</td><td align="center">-</td><td align="center">2.0</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">kernel_muls</td></tr>
  </table>

## 编译运行

### CPU 调试模式

```bash
# 配置环境变量
source /usr/local/Ascend/cann/set_env.sh

# 编译（CPU 模式）
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..
make -j

# 运行
./demo
```

### NPU 模式（默认）

```bash
# 配置环境变量
source /usr/local/Ascend/cann/set_env.sh

# 编译（NPU 模式）
mkdir -p build && cd build
cmake ..
make -j

# 运行
./demo
```

### 完整运行流程

```bash
# 1. 生成测试数据
python3 scripts/gen_data.py

# 2. 编译（选择 CPU 或 NPU 模式）
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..  # 或 cmake .. 用于 NPU 模式
make -j

# 3. 运行
./demo

# 4. 验证结果
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

预期输出：
```
test pass!
```
