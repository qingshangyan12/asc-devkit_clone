# Tensor API Ternary Transform 示例集

本目录包含 14 个 Tensor API Ternary Transform 操作的完整示例，展示了如何使用 Ascend C 基础 API 实现向量与标量的各种运算。

## 概述

Tensor API 的 `Transform<Inst::XxxScalar>` 是 asc-devkit 仓库中的新功能，通过编译期模板元编程将高层操作路由到对应的向量化循环实现。由于该功能尚未发布到 CANN 包中，本示例集使用已发布的基础 API 来演示相同的概念。

## 操作列表

### 算术操作（5个）

| 目录 | 操作 | 公式 | API | 数据类型 |
|------|------|------|-----|----------|
| [add_scalar](./add_scalar) | 向量加标量 | `out = src + scalar` | `Adds` | half |
| [mul_scalar](./mul_scalar) | 向量乘标量 | `out = src * scalar` | `Muls` | half |
| [max_scalar](./max_scalar) | 向量与标量取最大值 | `out = max(src, scalar)` | `Maxs` | half |
| [min_scalar](./min_scalar) | 向量与标量取最小值 | `out = min(src, scalar)` | `Mins` | half |
| [axpy](./axpy) | 向量乘标量后累加 | `out = src * scalar + out` | `Axpy` | half |

### 比较操作（5个）

| 目录 | 操作 | 公式 | API | 输入类型 | 输出类型 |
|------|------|------|-----|----------|----------|
| [eq_scalar](./eq_scalar) | 等于比较 | `out = (src == scalar) ? 1 : 0` | `Compares(EQ)` | half | uint8 |
| [ge_scalar](./ge_scalar) | 大于等于比较 | `out = (src >= scalar) ? 1 : 0` | `Compares(GE)` | half | uint8 |
| [le_scalar](./le_scalar) | 小于等于比较 | `out = (src <= scalar) ? 1 : 0` | `Compares(LE)` | half | uint8 |
| [lt_scalar](./lt_scalar) | 小于比较 | `out = (src < scalar) ? 1 : 0` | `Compares(LT)` | half | uint8 |
| [ne_scalar](./ne_scalar) | 不等于比较 | `out = (src != scalar) ? 1 : 0` | `Compares(NE)` | half | uint8 |

### 移位操作（2个）

| 目录 | 操作 | 公式 | API | 数据类型 |
|------|------|------|-----|----------|
| [shiftleft_scalar](./shiftleft_scalar) | 向量左移 | `out = src << scalar` | `ShiftLeft` | int16 |
| [shiftright_scalar](./shiftright_scalar) | 向量右移 | `out = src >> scalar` | `ShiftRight` | int16 |

### 混合类型操作（2个）

| 目录 | 操作 | 公式 | API | 输入类型 | 输出类型 |
|------|------|------|-----|----------|----------|
| [muls](./muls) | 向量乘标量并类型转换 | `out(half) = src(float) * scalar` | `MulsCast` | float | half |
| [muls_v2](./muls_v2) | 向量乘标量并类型转换 v2 | `out(half) = src(float) * scalar` | `MulsCast` | float | half |

## 目录结构

每个操作目录包含以下文件：

```
<operation>/
├── <operation>.asc          # Kernel 实现（包含设备代码和主机代码）
├── CMakeLists.txt           # CMake 构建配置
├── data_utils.h             # 数据读写工具函数
├── README.md                # 操作详细文档
├── run.sh                   # 一键运行脚本
└── scripts/
    ├── gen_data.py          # 测试数据生成脚本
    └── verify_result.py     # 结果验证脚本
```

## 快速开始

### 1. 配置环境变量

```bash
source /usr/local/Ascend/cann/set_env.sh
```

### 2. 选择并进入操作目录

```bash
cd add_scalar  # 或其他操作目录
```

### 3. 编译和运行

#### CPU 调试模式（推荐用于开发调试）

```bash
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..
make -j
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

#### NPU 模式（需要 Ascend NPU 硬件）

```bash
mkdir -p build && cd build
cmake ..
make -j
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

### 4. 使用一键运行脚本

```bash
./run.sh
```

## 运行模式对比

| 特性 | CPU 模式 | NPU 模式 |
|------|---------|---------|
| 硬件要求 | 无 | 需要 Ascend NPU |
| 编译器 | GCC | ASC (bisheng) |
| 内核启动 | `ICPU_RUN_KF` | `<<<>>>` |
| 调试支持 | GDB 完整支持 | 有限支持 |
| 性能 | 模拟执行，较慢 | 真实硬件性能 |
| 适用场景 | 开发调试 | 生产环境 |

## GDB 调试示例（CPU 模式）

```bash
# 编译 CPU 模式
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..
make -j

# 启动 GDB
gdb --args ./demo

# 在 GDB 中设置断点
(gdb) break Compute
(gdb) run
(gdb) print srcLocal
(gdb) next
(gdb) continue
```

## 批量测试

可以使用以下脚本批量测试所有操作：

```bash
#!/bin/bash
for dir in add_scalar mul_scalar max_scalar min_scalar axpy \
           eq_scalar ge_scalar le_scalar lt_scalar ne_scalar \
           shiftleft_scalar shiftright_scalar muls muls_v2; do
    echo "========== Testing $dir =========="
    cd $dir
    ./run.sh
    cd ..
done
```

## Tensor API 与基础 API 的对应关系

| 基础 API | Tensor API | 说明 |
|---------|-----------|------|
| `AscendC::Adds(dst, src, scalar, count)` | `Transform<Inst::AddScalar>(dst, src, scalar)` | 向量加标量 |
| `AscendC::Muls(dst, src, scalar, count)` | `Transform<Inst::MulScalar>(dst, src, scalar)` | 向量乘标量 |
| `AscendC::Maxs(dst, src, scalar, count)` | `Transform<Inst::MaxScalar>(dst, src, scalar)` | 向量与标量取最大值 |
| `AscendC::Mins(dst, src, scalar, count)` | `Transform<Inst::MinScalar>(dst, src, scalar)` | 向量与标量取最小值 |
| `AscendC::Axpy(dst, src, scalar, count)` | `Transform<Inst::Axpy>(dst, src, scalar)` | dst = src * scalar + dst |
| `AscendC::Compares(dst, src, scalar, CMPMODE::EQ, count)` | `Transform<Inst::EqScalar>(dst, src, scalar)` | 等于比较 |
| `AscendC::Compares(dst, src, scalar, CMPMODE::GE, count)` | `Transform<Inst::GeScalar>(dst, src, scalar)` | 大于等于比较 |
| `AscendC::Compares(dst, src, scalar, CMPMODE::LE, count)` | `Transform<Inst::LeScalar>(dst, src, scalar)` | 小于等于比较 |
| `AscendC::Compares(dst, src, scalar, CMPMODE::LT, count)` | `Transform<Inst::LtScalar>(dst, src, scalar)` | 小于比较 |
| `AscendC::Compares(dst, src, scalar, CMPMODE::NE, count)` | `Transform<Inst::NeScalar>(dst, src, scalar)` | 不等于比较 |
| `AscendC::ShiftLeft(dst, src, scalar, count)` | `Transform<Inst::ShiftLeftScalar>(dst, src, scalar)` | 向量左移 |
| `AscendC::ShiftRight(dst, src, scalar, count)` | `Transform<Inst::ShiftRightScalar>(dst, src, scalar)` | 向量右移 |
| `AscendC::MulsCast(dst, src, scalar, count)` | `Transform<Inst::Muls>(dst, src, scalar)` | 向量乘标量并类型转换 |

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 注意事项

1. **CANN 版本要求**：需要安装支持 Ascend 950 的 CANN 开发套件
2. **CPU 调试模式**：无需 NPU 硬件，适合开发和调试
3. **NPU 模式**：需要实际的 Ascend NPU 硬件
4. **数据验证**：每个操作都包含自动验证脚本，确保计算结果正确
5. **性能测试**：CPU 模式为模拟执行，性能数据不代表真实硬件性能

## 相关文档

- [Ascend C 编程指南](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2alpha001/infaicdev/aicdevrefascendc)
- [Tensor API 架构说明](../../../../docs/api/context/tensor_api/README.md)
- [CPU 调试指南](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2alpha001/infaicdev/aicdevrefascendc/02/0001)

## 许可证

本示例集遵循 CANN Open Software License Agreement Version 2.0。
