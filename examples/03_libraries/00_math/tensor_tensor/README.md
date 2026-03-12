# Tensor-Tensor 算子直调样例

## 概述

本样例演示了如何直接调用 **Tensor‑Tensor** 类型的 Ascend C API。算子接受一个 Tensor 输入，输出一个 Tensor。样例支持以下功能：

- **多种计算模式**：count 连续计算模式、mask 连续模式、mask 逐比特模式。
- **特殊使用场景**：操作数重叠。
- **灵活配置**：可通过模板参数切换 API、数据类型、计算模式等。

## 支持产品

- Atlas A3 训练系列 / Atlas A3 推理系列
- Atlas A2 训练系列 / Atlas A2 推理系列

## 目录结构

```
tensor_tensor/
├── CMakeLists.txt              # 编译配置文件
├── tensor_tensor.asc           # Ascend C 算子实现与调用样例
├── data_utils.h                # 数据读写工具函数
└── scripts/
    └── gen_data.py             # 生成输入数据与参考输出（真值）的脚本
```

## 算子描述

### 功能特性
- 支持不同 Tensor‑Tensor API 的切换（默认为 `CastDequant`）。
- 支持三种计算模式：
  - **COUNT 模式**：使用 `count` 参数控制计算长度。
  - **MASK 连续模式**：使用连续 mask 进行高位切分。
  - **MASK 逐比特模式**：使用逐比特 mask 进行精细控制。
- 支持特殊场景：
  - **操作数重叠**：源 Tensor 与目的 Tensor 地址重叠。

### 核心函数
- `Kernel()`：核函数入口，负责 Global/Local Tensor 的分配与调度。
- `TestFunctionality()`：功能测试主函数，完成数据搬运、计算、结果回写。
- `TestPerformance()`：性能测试主函数，循环执行算子以测量耗时。
- `ComputeCountMode()`：启用 count 模式的计算函数。
- `ComputeContinuousMaskMode()`：启用 mask 连续模式的计算函数。
- `ComputeBitwiseMaskMode()`：启用 mask 逐比特模式的计算函数。

## 快速开始

### 1. 环境准备

根据 CANN 包的安装方式，选择对应的命令设置环境变量：

```bash
# 默认路径，root 用户安装
source /usr/local/Ascend/cann/set_env.sh

# 默认路径，非 root 用户安装
source $HOME/Ascend/cann/set_env.sh

# 自定义安装路径
source ${install_path}/cann/set_env.sh
```

### 2. 编译与运行

在样例根目录执行以下命令：

```bash
# 设置计算大小（可调整）
SIZE=256

# 创建构建目录并编译
mkdir -p build && cd build
cmake .. -DSIZE=$SIZE
make -j

# 生成测试数据
python3 ../scripts/gen_data.py

# 运行样例
./demo
```

若输出 `test pass!` 表示精度验证通过。

## 测试场景配置

### 测试单个 API 的不同场景

以默认的 `CastDequant` 为例，修改 `tensor_tensor.asc` 中的配置段：

```c++
/************************ 用户可自定义配置 ********************** */
constexpr uint32_t size = SIZE;                       // 计算量
constexpr uint32_t REPEAT_TIMES_FOR_PERF_TEST = 1000; // 性能测试循环次数
using srcType = int16_t;                              // 源 Tensor 数据类型
using dstType = uint8_t;                              // 目的 Tensor 数据类型
constexpr bool halfBlock = false;                      // halfBlock参数
constexpr bool isVecDeq = false;                      // 是否使用向量量化模式
using CurrentConfig = KernelConfig<ComputeMode::USE_COUNT_MODE, CaseType::NORMAL, false>;
#define TEMPLATE_API CastDequant
/************************************************************** */
```

**参数说明**：
- `ComputeMode`：`USE_COUNT_MODE`（count 模式）、`USE_CONTINUOUS_MODE_OF_MASK`（mask 连续模式）、`USE_BITWISE_MASK_MODE`（mask 逐比特模式）。
- `CaseType`：`NORMAL`（常规调用）、`OVERLAP`（操作数重叠）。
- `PerfTest`：`true` 启用性能测试，`false` 启用功能测试。
- `halfBlock`：CastDequant 特有参数，控制输出元素存放在上半还是下半 Block。
- `isVecDeq`：CastDequant 特有参数，控制是否选择向量量化模式。

### 测试其他 API

若需更换为其他 Tensor‑Tensor API，需修改以下三处：

1. **算子文件**（`tensor_tensor.asc`）：
    - 将 `#define TEMPLATE_API CastDequant` 改为目标 API 名称。
    - 根据 API 语义调整 `srcType`、`dstType`。

2. **数据生成脚本**（`scripts/gen_data.py`）：
    - 更新 `src_dtype`、`dst_dtype` 为对应的 NumPy 数据类型。
    - 在 `compute()` 函数中实现与目标 API 一致的数学运算。

3. **验证逻辑**（`tensor_tensor.asc` 中的 `CompareResult`）：
    - 确保 `CompareResult` 模板参数与 `dstType` 一致。
    - 若精度要求不同，可调整容差值。

## 注意事项

1. **环境变量**：务必在编译前正确设置 CANN 环境变量，否则可能导致编译失败或运行时错误。
2. **数据类型**：若切换 `srcType`/`dstType`，需同步修改 `gen_data.py` 中的数据类型，并确保 `compute()` 函数正确实现对应计算。
3. **计算模式**：mask 模式对数据长度有对齐要求，请确保 `SIZE` 是 `256 / sizeof(dstType)` 的整数倍。
4. **错误排查**：若运行失败，可检查 `build/` 目录下的日志，或尝试减小 `SIZE` 以排除内存不足等问题。
5. **CastDequant 特性**：
   - 输入类型支持：int16_t/int32_t
   - 输出类型支持：int8_t/uint8_t/half
   - 输出大小是输入大小的 2 倍（因为 int16_t 转换为 uint8_t 时每个元素占用 2 字节）
