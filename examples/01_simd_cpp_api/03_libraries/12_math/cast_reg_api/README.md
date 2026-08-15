# Cast Reg API 样例

## 概述

本样例使用Tensor API 的 `asc::te::cast` 接口，在寄存器中完成逐元素数据类型转换。
样例输入和输出均为形状 `[16, 16]` 的 ND Tensor，共包含 256 个元素。数据首先从全局内存（GM）
搬运到统一缓冲区（UB），再加载到 `reg_tensor` 中执行类型转换，最后写回 UB 和 GM。

完整计算流水如下：

```text
GM input
  -> CopyGM2UB
  -> tensor.load
  -> reg_tensor<From>
  -> asc::te::cast<To, Layout, Round, Sat>
  -> reg_tensor<To>
  -> tensor.store
  -> CopyUB2GM
  -> GM output
```

`asc::te::cast` 负责寄存器中的类型转换，并将输入 `reg_tensor` 的 mask 原样传递给输出。
样例通过编译选项 `SCENARIO_NUM` 选择一种转换场景，覆盖浮点数转整数、整数位宽扩展与收缩以及BF16/FP32 相互转换。所有场景均使用 `cast_layout::zero`，具体的舍入和饱和策略由场景决定。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
cast_reg_api
├── scripts
│   └── gen_data.py       // 生成输入数据和真值数据
├── CMakeLists.txt        // 样例编译配置
├── cast_reg_api.asc      // Kernel 实现、Kernel 调用及结果校验
├── data_utils.h          // 二进制文件读写工具
└── README.md             // 样例说明
```

## 样例描述

### 转换场景

| `SCENARIO_NUM` | 转换 | `cast_layout` | `cast_round_mode` | `cast_sat_mode` | 转换说明 |
| --- | --- | --- | --- | --- | --- |
| 1 | `half -> int32_t` | `zero` | `floor` | `not_sat` | 浮点数向负无穷方向取整后转换为 32 位有符号整数 |
| 2 | `float -> int16_t` | `zero` | `rint` | `sat` | 舍入到最近整数，中间值取偶数，并将越界结果限制到 INT16 范围 |
| 3 | `int8_t -> int32_t` | `zero` | `rint` | `not_sat` | 8 位有符号整数无损扩展为 32 位有符号整数；`rint` 为该类型对的固定选项 |
| 4 | `int32_t -> uint8_t` | `zero` | `rint` | `sat` | 收缩为 8 位无符号整数，并将结果限制到 `[0, 255]` |
| 5 | `bfloat16_t -> float` | `zero` | `rint` | `not_sat` | BF16 扩展为 FP32，保持原 BF16 数值 |
| 6 | `float -> bfloat16_t` | `zero` | `rint` | `not_sat` | FP32 按最近偶数规则舍入为 BF16 |

> `cast_round_mode` 和 `cast_sat_mode` 是否产生实际舍入或饱和行为取决于具体类型对。例如，场景 3
> 是整数扩宽转换，不需要数值舍入，表中的 `rint` 仅用于选择该接口支持的固定策略组合。

### 输入输出规格

每个场景只有一个输入 Tensor `x` 和一个输出 Tensor `y`。二者 shape 均为 `[16, 16]`，内存格式
均为 ND 连续布局。

| 场景 | 输入 `x` 类型 | 输入 shape | 输出 `y` 类型 | 输出 shape | 输入生成范围或方式 |
| --- | --- | --- | --- | --- | --- |
| 1 | `half` | `[16, 16]` | `int32_t` | `[16, 16]` | 从 `[-100, 100)` 均匀采样并转换为 FP16 |
| 2 | `float` | `[16, 16]` | `int16_t` | `[16, 16]` | 从 `[-100, 100)` 均匀采样为 FP32 |
| 3 | `int8_t` | `[16, 16]` | `int32_t` | `[16, 16]` | 从 `[-128, 128)` 随机生成整数 |
| 4 | `int32_t` | `[16, 16]` | `uint8_t` | `[16, 16]` | 从 `[-128, 384)` 随机生成整数，用于覆盖小于 0 和大于 255 的饱和场景 |
| 5 | `bfloat16_t` | `[16, 16]` | `float` | `[16, 16]` | FP32 均匀采样后保留高 16 bit，构造 BF16 输入 |
| 6 | `float` | `[16, 16]` | `bfloat16_t` | `[16, 16]` | 从 `[-100, 100)` 均匀采样为 FP32 |

### Kernel 实现

Kernel 的主要处理步骤如下：

1. 使用 `asc::te::make_tensor`、`make_mem_ptr` 和 `make_frame_layout<nd_ext_layout_ptn>` 分别构造
   GM 和 UB Tensor。
2. 使用 `make_copy(copy_gm_to_ub{})` 和 `copy` 将 256 个输入元素从 GM 搬运到 UB。
3. 通过 `SetFlag`/`WaitFlag` 完成 MTE2 到 Vector 流水同步。
4. `CastRegLoop` 根据源类型和目的类型中较宽类型的字节数计算单次寄存器处理的元素数量。
5. 同位宽转换使用 `tensor.load(coord)`；2 倍或 4 倍位宽扩展分别使用
   `tensor.load<load_sideband_mode::unpack>` 或 `tensor.load<load_sideband_mode::unpack4>`。
6. 使用 `update_mask<WiderT>(remain)` 生成当前迭代的有效 mask，再调用 `asc::te::cast` 完成转换；返回值保留该 mask。
7. 同位宽或位宽扩展结果使用 `tensor.store(coord, value)` 写回；2 倍或 4 倍位宽收缩结果分别使用
   `tensor.store<store_sideband_mode::pack>` 或 `tensor.store<store_sideband_mode::pack_quarter>` 连续写回 UB。
8. 通过 `SetFlag`/`WaitFlag` 完成 Vector 到 MTE3 流水同步，再使用 `copy_ub_to_gm` 将结果搬回 GM。

### 调用与校验

Host 侧根据 `SCENARIO_NUM` 实例化对应的 `RunScenario<SrcT, DstT>`，申请 Host 和 Device 内存，
读取 `input/input_x.bin`，并使用内核调用符 `<<<>>>` 启动一个 Vector Kernel。Kernel 完成后将结果
复制回 Host，并写入 `output/output.bin`。

除 BF16 输出外，结果使用绝对误差和相对误差 `1e-4` 进行比较；BF16 输出按 16-bit 位模式进行
精确比较。全部元素与 `output/golden.bin` 一致时，程序输出 `test pass!`。

## 核心接口说明

### asc::te::cast

本样例以 C++17 模式编译，`cast` 接口形式如下：

```cpp
template <typename To, cast_layout Layout = cast_layout::zero,
    cast_round_mode Round = cast_round_mode::rint,
    cast_sat_mode Sat = cast_sat_mode::not_sat, typename From>
__simd_callee__ inline reg_tensor<To> cast(reg_tensor<From> src);
```

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| `To` | 模板类型参数 | 目的元素类型 |
| `Layout` | `cast_layout` | 选择位宽变化时参与转换的物理 lane 分区，不表示 Tensor 的 shape/stride layout |
| `Round` | `cast_round_mode` | 指定浮点转换的舍入方式 |
| `Sat` | `cast_sat_mode` | 指定是否将越界结果限制到目的类型范围 |
| `From` | 从函数实参推导 | 源元素类型 |
| `src` | `reg_tensor<From>` | 源寄存器数据和对应的 predicate mask |
| 返回值 | `reg_tensor<To>` | 转换后的寄存器数据，mask 与 `src.mask` 一致 |

C++17 调用示例：

```cpp
auto dst = asc::te::cast<int32_t, asc::te::cast_layout::zero,
    asc::te::cast_round_mode::floor, asc::te::cast_sat_mode::not_sat>(src);
```

本接口当前属于 experimental 接口，业务代码应包含公开头文件：

```cpp
#include "tensor_api/arch/vector/experimental/type_conversion.h"
```

不应直接包含 `impl/` 目录下的内部实现头文件。

## 编译运行

在本样例根目录下执行以下步骤。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的安装方式配置环境变量，例如：

```bash
source ${install_path}/cann/set_env.sh
```

### 样例执行

```bash
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./demo
```

依次将 `SCENARIO_NUM` 设置为 `1` 至 `6`，可以验证全部转换场景。生成数据时传入的
`-scenarioNum` 必须与编译时的 `-DSCENARIO_NUM` 保持一致，否则输入文件类型和 Kernel 模板实例
不匹配。

CPU 调试模式和 NPU 仿真模式示例如下：

```bash
# CPU 调试模式
cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j

# NPU 仿真模式
cmake -DSCENARIO_NUM=4 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

> 切换 `SCENARIO_NUM` 或编译运行模式前，应重新执行 CMake。切换运行模式时若缓存仍保留旧值，
> 需要清理 `build/CMakeCache.txt` 后重新配置。

### 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `SCENARIO_NUM` | `1`、`2`、`3`、`4`、`5`、`6` | 必选，指定编译和执行的类型转换场景 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 分别表示 NPU 运行、CPU 调试和 NPU 仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | 指定目标 NPU 架构 |

本样例使用枚举类型模板策略参数，ASC 源文件使用 C++17 标准编译。

### 执行结果

执行成功时输出：

```text
test pass!
```
