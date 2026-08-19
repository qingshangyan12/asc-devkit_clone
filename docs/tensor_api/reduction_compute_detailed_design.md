# Tensor API 寄存器归约接口详细设计

## 1. 文档概述

### 1.1 编写目的

本文档描述 Tensor API 实验性寄存器归约接口的需求、使用场景、接口契约、实现方案和约束条件，作为接口开发、代码评审、单元测试及后续演进的依据。

本文档覆盖以下接口：

- `reduce_sum`
- `reduce_max`
- `reduce_min`
- `reduce_scope`

### 1.2 适用范围

| 项目 | 说明 |
| --- | --- |
| 目标架构 | `NPU_ARCH 3510` |
| 执行单元 | AI Vector Core，要求 `ASC_IS_AIV` |
| 接口命名空间 | `asc::te` |
| 数据载体 | `reg_tensor<T>` |
| 接口状态 | `experimental` |
| 归约粒度 | 单个硬件向量寄存器 |

本文档不覆盖跨寄存器、跨 Tensor、跨核或跨设备归约，也不负责将归约结果写回 Global Memory。

### 1.3 相关文件

| 文件 | 职责 |
| --- | --- |
| `include/tensor_api/utils/utils.h` | 声明公共枚举 `reduce_scope` |
| `include/tensor_api/arch/vector/experimental/reduction_compute.h` | 声明对外归约接口 |
| `impl/tensor_api/arch/vector/experimental/reduction_compute_impl.h` | 实现模板分派及底层 C API 调用 |
| `include/tensor_api/arch/vector/experimental/reg_tensor.h` | 定义寄存器数据及掩码载体 `reg_tensor<T>` |
| `include/c_api/reg_compute/reg_vector.h` | 声明底层寄存器归约 C API |
| `tests/api/tensor_api/npu_arch_3510/aiv/test_tensor_api_reduction_compute.cpp` | 独立归约接口 UT |

## 2. 需求及场景描述

### 2.1 背景

Tensor API 的向量计算链路使用 `reg_tensor<T>` 表示硬件向量寄存器及其有效数据掩码。常见向量算法需要先在寄存器内部完成求和、求最大值或求最小值，再参与后续的归一化、比较、缩放或写回操作。

如果上层算法直接调用不同的底层 C API，会产生以下问题：

- 算法代码与具体指令名称和归约粒度强耦合。
- 不同接口对源类型、目标类型和掩码的处理方式不统一。
- 上层需要重复维护 `reg_tensor` 的拆包、结果封装及掩码传播逻辑。
- 后续扩展架构版本或替换底层实现时，修改范围会扩散到算法层。

因此需要提供一组统一的 C++ 模板接口，用 `reduce_scope` 表达归约范围，并在实现层完成到底层寄存器指令的静态分派。

### 2.2 功能需求

1. 支持寄存器内求和、求最大值和求最小值。
2. 支持整寄存器归约和按 Data Block 归约。
3. 求和额外支持相邻元素成对归约。
4. 支持由模板参数显式指定结果类型，满足 `int16_t -> int32_t`、`uint16_t -> uint32_t` 等扩宽求和需求。
5. 从 `reg_tensor` 自动取得底层寄存器和掩码，并将结果重新封装为 `reg_tensor`。
6. 归约范围在编译期确定，不引入运行时分支。
7. 不支持的归约范围或类型组合应在编译阶段失败。
8. 接口应能够在 `__simd_vf__` 寄存器计算函数内调用。

### 2.3 非功能需求

- **零额外循环**：单次接口调用只映射为对应的单寄存器归约指令，不在封装层引入隐式循环。
- **零动态分派**：使用 `if constexpr` 在编译期选择指令。
- **低封装成本**：接口为 `inline` 和 `__simd_callee__`，避免额外函数调用开销。
- **类型可验证**：返回类型由模板参数确定，可通过 `static_assert` 验证。
- **行为可追溯**：每一种 `reduce_scope` 均能对应到明确的底层 C API 和硬件指令。

### 2.4 典型使用场景

#### 2.4.1 Softmax 与 LogSoftmax

Softmax 通常先通过 `reduce_max` 获取寄存器内最大值以提高指数计算的数值稳定性，再通过 `reduce_sum` 计算指数和。归约结果的广播、减法、除法属于后续独立计算步骤，不由本接口完成。

#### 2.4.2 均值、方差及归一化

Mean、Variance、LayerNorm 和 RMSNorm 等算子需要对一组寄存器元素求和。`reduce_sum` 提供单寄存器局部归约能力；当数据跨越多个寄存器时，上层算法仍需显式合并各寄存器的局部结果。

#### 2.4.3 最大值和最小值统计

MaxPool、MinPool、MinMax 统计以及阈值计算可使用 `reduce_max` 或 `reduce_min` 获取寄存器或 Data Block 范围内的极值。

#### 2.4.4 Data Block 独立处理

当算法以 32 Byte Data Block 为基本处理单元时，可使用 `reduce_scope::datablock`，使每个 Data Block 独立完成归约，避免上层拆分寄存器。

#### 2.4.5 相邻元素成对求和

需要相邻 lane 两两合并时，可使用 `reduce_sum<DstType, reduce_scope::pair>`。该模式适用于局部合并或分级归约的第一阶段，目前只支持浮点类型。

## 3. 接口描述

### 3.1 头文件及命名空间

用户通过以下头文件使用接口：

```cpp
#include "tensor_api/arch/vector/experimental/reduction_compute.h"
```

接口位于 `asc::te` 命名空间。通过 Tensor API 聚合头文件使用时，预期包含链路为：

```text
tensor_api/tensor.h
  -> tensor_api/arch/arch.h
     -> tensor_api/arch/vector/experimental/reduction_compute.h
```

### 3.2 归约范围枚举

```cpp
enum class reduce_scope : uint8_t {
    vector,
    datablock,
    pair,
};
```

| 枚举值 | 语义 | 适用接口 |
| --- | --- | --- |
| `vector` | 对单个硬件向量寄存器的有效元素进行整体归约 | `reduce_sum`、`reduce_max`、`reduce_min` |
| `datablock` | 以 32 Byte Data Block 为单位分别归约 | `reduce_sum`、`reduce_max`、`reduce_min` |
| `pair` | 对相邻 lane 成对求和 | 仅 `reduce_sum` |

归约结果仍保存在目标寄存器中。结果在寄存器内的具体排布由底层指令定义，本接口不将结果转换为 C++ 标量。

### 3.3 `reduce_sum`

```cpp
template <typename DstType,
          reduce_scope scope = reduce_scope::vector,
          typename SrcType>
__simd_callee__ inline decltype(auto) reduce_sum(reg_tensor<SrcType> src);
```

功能：按照 `scope` 对 `src` 执行求和归约，返回 `reg_tensor<DstType>`。

参数说明：

| 参数 | 类型 | 说明 |
| --- | --- | --- |
| `DstType` | 模板类型参数 | 目标元素类型，必须显式指定 |
| `scope` | 模板非类型参数 | 归约范围，默认 `reduce_scope::vector` |
| `SrcType` | 模板类型参数 | 源元素类型，由 `src` 自动推导 |
| `src` | `reg_tensor<SrcType>` | 源寄存器及其有效数据掩码 |

返回值：`reg_tensor<DstType>`。当前声明使用 `decltype(auto)`，实际返回对象类型由实现中的 `reg_tensor<DstType>` 推导得到。

调用示例：

```cpp
reg_tensor<float> src = /* load from tensor */;

auto vector_sum = reduce_sum<float>(src);
auto block_sum = reduce_sum<float, reduce_scope::datablock>(src);
auto pair_sum = reduce_sum<float, reduce_scope::pair>(src);
```

整数扩宽求和示例：

```cpp
reg_tensor<int16_t> src = /* load from tensor */;
auto result = reduce_sum<int32_t>(src);
```

### 3.4 `reduce_max`

```cpp
template <typename DstType,
          reduce_scope scope = reduce_scope::vector,
          typename SrcType>
__simd_callee__ inline decltype(auto) reduce_max(reg_tensor<SrcType> src);
```

功能：按照 `scope` 对 `src` 执行最大值归约，返回 `reg_tensor<DstType>`。

支持的 `scope`：`vector`、`datablock`。

调用示例：

```cpp
reg_tensor<float> src = /* load from tensor */;

auto vector_max = reduce_max<float>(src);
auto block_max = reduce_max<float, reduce_scope::datablock>(src);
```

当前底层接口要求 `DstType` 与 `SrcType` 相同，不提供整数扩宽或跨类型转换。

### 3.5 `reduce_min`

```cpp
template <typename DstType,
          reduce_scope scope = reduce_scope::vector,
          typename SrcType>
__simd_callee__ inline decltype(auto) reduce_min(reg_tensor<SrcType> src);
```

功能：按照 `scope` 对 `src` 执行最小值归约，返回 `reg_tensor<DstType>`。

支持的 `scope`：`vector`、`datablock`。

调用示例：

```cpp
reg_tensor<float> src = /* load from tensor */;

auto vector_min = reduce_min<float>(src);
auto block_min = reduce_min<float, reduce_scope::datablock>(src);
```

当前底层接口要求 `DstType` 与 `SrcType` 相同，不提供整数扩宽或跨类型转换。

### 3.6 类型支持矩阵

#### 3.6.1 `reduce_sum`

| `SrcType` | `DstType` | `vector` | `datablock` | `pair` |
| --- | --- | --- | --- | --- |
| `int16_t` | `int32_t` | 支持 | 支持 | 不支持 |
| `int32_t` | `int32_t` | 支持 | 支持 | 不支持 |
| `uint16_t` | `uint32_t` | 支持 | 支持 | 不支持 |
| `uint32_t` | `uint32_t` | 支持 | 支持 | 不支持 |
| `half` | `half` | 支持 | 支持 | 支持 |
| `float` | `float` | 支持 | 支持 | 支持 |

#### 3.6.2 `reduce_max` 和 `reduce_min`

| `SrcType` | `DstType` | `vector` | `datablock` | `pair` |
| --- | --- | --- | --- | --- |
| `int16_t` | `int16_t` | 支持 | 支持 | 不支持 |
| `int32_t` | `int32_t` | 支持 | 支持 | 不支持 |
| `uint16_t` | `uint16_t` | 支持 | 支持 | 不支持 |
| `uint32_t` | `uint32_t` | 支持 | 支持 | 不支持 |
| `half` | `half` | 支持 | 支持 | 不支持 |
| `float` | `float` | 支持 | 支持 | 不支持 |

未列出的类型和类型组合不在当前接口能力范围内，包括 `int8_t`、`uint8_t`、`bfloat16_t` 和 FP8 等类型。

## 4. 方案概述

### 4.1 总体分层

```text
算法层 / 用户代码
        |
        v
reduction_compute.h            对外模板接口
        |
        v
reduction_compute_impl.h       scope 编译期分派、结果封装、掩码传播
        |
        v
asc_reduce_* C API             寄存器类型重载和架构适配
        |
        v
3510 向量指令                 vcadd/vcgadd/vcpadd/vcmax/vcgmax/vcmin/vcgmin
```

该分层将算法语义、Tensor API 封装和架构指令解耦。用户只依赖 `reg_tensor`、归约操作名和归约范围，不直接依赖具体指令名称。

### 4.2 文件职责

#### 4.2.1 `utils.h`

声明公共枚举 `reduce_scope`。该枚举独立于单个归约函数，后续同类接口可以复用，但扩展枚举值时必须同时评估所有使用该枚举的接口。

#### 4.2.2 `reduction_compute.h`

- 提供稳定的对外函数模板声明。
- 为 `scope` 设置默认值 `reduce_scope::vector`。
- 包含实现头文件，使模板定义在实例化点可见。
- 不承载底层指令分派细节。

#### 4.2.3 `reduction_compute_impl.h`

- 创建 `reg_tensor<DstType>` 结果对象。
- 使用 `if constexpr` 根据 `scope` 选择底层 C API。
- 通过 `static_assert` 限制接口支持的归约范围。
- 将 `src.mask` 传播到返回对象。
- 返回目标类型的 `reg_tensor`。

### 4.3 静态分派设计

`reduce_sum` 的逻辑等价于：

```cpp
reg_tensor<DstType> dst;

if constexpr (scope == reduce_scope::vector) {
    asc_reduce_sum(dst.reg, src.reg, src.mask);
} else if constexpr (scope == reduce_scope::datablock) {
    asc_reduce_sum_datablock(dst.reg, src.reg, src.mask);
} else {
    asc_pair_reduce_sum(dst.reg, src.reg, src.mask);
}

dst.mask = src.mask;
return dst;
```

`reduce_max` 和 `reduce_min` 采用相同结构，但只允许 `vector` 和 `datablock` 两种范围。

由于 `scope` 是模板参数，未选中的分支不会参与最终代码生成，不存在运行时判断成本。

### 4.4 底层指令映射

| Tensor API | `scope` | C API | 3510 指令 |
| --- | --- | --- | --- |
| `reduce_sum` | `vector` | `asc_reduce_sum` | `vcadd` |
| `reduce_sum` | `datablock` | `asc_reduce_sum_datablock` | `vcgadd` |
| `reduce_sum` | `pair` | `asc_pair_reduce_sum` | `vcpadd` |
| `reduce_max` | `vector` | `asc_reduce_max` | `vcmax` |
| `reduce_max` | `datablock` | `asc_reduce_max_datablock` | `vcgmax` |
| `reduce_min` | `vector` | `asc_reduce_min` | `vcmin` |
| `reduce_min` | `datablock` | `asc_reduce_min_datablock` | `vcgmin` |

底层 C API 使用 `MODE_ZEROING` 处理非激活 lane。Tensor API 封装层不修改底层指令的计算语义。

### 4.5 掩码处理

`reg_tensor<SrcType>` 同时保存底层向量寄存器 `reg` 和有效数据掩码 `mask`。归约时：

1. `src.mask` 作为指令掩码传给底层 C API。
2. 非激活 lane 按底层 `MODE_ZEROING` 规则参与结果生成。
3. 返回对象的元数据掩码设置为 `src.mask`，保证后续 Tensor API 继续沿用相同的有效范围。

掩码用于控制计算有效性，但不能修复越界加载。源 `reg_tensor` 必须由合法的 load 或等价构造产生，尾块场景需要在加载阶段建立正确掩码。

### 4.6 类型策略

接口将结果类型 `DstType` 放在第一个模板参数位置，并要求调用方显式指定，原因如下：

- 求和存在合法的扩宽组合，无法一律从 `SrcType` 推导。
- 显式目标类型便于调用点审查数值范围和精度。
- 三个归约接口保持统一的调用形式。

底层 C API 通过寄存器类型重载约束合法组合。设计上建议在 Tensor API 层增加针对 `DstType`、`SrcType` 和 `scope` 的显式类型 `static_assert`，从而提供比“无匹配重载”更清晰的编译错误。

### 4.7 函数放置

归约接口本身使用 `__simd_callee__ inline`，完整的寄存器数据流应放置在 `__simd_vf__` 函数中，例如：

```cpp
__simd_vf__ inline void compute_sum(/* tensor arguments */)
{
    reg_tensor<float> src = /* load */;
    auto sum = reduce_sum<float>(src);
    // Continue register computation or store the result.
}
```

全局 `__aicore__` kernel 负责参数准备和调用组织，不应直接承载复杂寄存器表达式链，以免触发编译器指令提升或切分限制。

## 5. 约束条件

### 5.1 架构约束

- 当前实现只面向 `NPU_ARCH 3510` 的向量核。
- 底层实现受 `ASC_IS_AIV` 约束，不适用于 AI Cube Core 路径。
- 公共头文件应使用与同目录实验性接口一致的架构保护，避免其他架构错误实例化 3510 专属实现。

### 5.2 归约范围约束

- `reduce_sum` 支持 `vector`、`datablock` 和 `pair`。
- `reduce_max`、`reduce_min` 不支持 `pair`。
- `reduce_scope` 为编译期参数，不支持运行时动态选择。
- `pair` 只支持 `half` 和 `float` 同类型输入输出。

### 5.3 数据范围约束

- 单次调用只处理一个 `reg_tensor` 对应的硬件向量寄存器。
- `vector` 不等价于对任意长度 Tensor 的全局归约。
- 数据跨寄存器时，上层必须显式执行多阶段归约。
- `datablock` 的基本单位为 32 Byte，不是固定元素个数；不同元素类型对应的 lane 数不同。

### 5.4 结果表示约束

- 返回值始终是 `reg_tensor<DstType>`，不是标量。
- 有效归约结果在寄存器中的 lane 位置和重复方式由对应底层指令定义。
- 本接口不负责广播、标量提取、跨寄存器合并或内存写回。

### 5.5 数值约束

- 计算顺序遵循硬件归约指令，不保证与 C++ 从左到右顺序累加完全一致。
- 浮点结果可能因归约树顺序产生舍入差异，测试应使用合理误差阈值。
- `int16_t` 和 `uint16_t` 求和使用 32 位目标类型降低溢出风险。
- 32 位整数求和仍可能溢出，溢出行为由底层指令规定，接口不做饱和或异常检查。
- 最大值和最小值不执行隐式类型转换。

### 5.6 掩码与尾块约束

- 调用方必须保证 `src.mask` 与寄存器中的有效数据一致。
- 掩码只控制计算 lane，不代替合法的内存访问边界处理。
- 返回对象沿用源掩码；如果后续算法只应消费归约结果 lane，上层需要根据底层结果布局重新设置合适掩码。

### 5.7 接口稳定性约束

接口位于 `experimental` 路径，当前不承诺稳定 ABI。以下内容在正式稳定前仍可能调整：

- 结果类型是否继续要求显式指定。
- 返回类型是否从 `decltype(auto)` 收敛为明确的 `reg_tensor<DstType>`。
- 结果掩码是否继续简单继承源掩码。
- 是否增加更多归约操作、数据类型或架构版本。

## 6. 异常与编译期诊断

归约接口不包含运行时异常。非法使用应在编译阶段暴露：

| 非法情况 | 预期结果 |
| --- | --- |
| `reduce_max<DstType, reduce_scope::pair>` | `static_assert` 失败 |
| `reduce_min<DstType, reduce_scope::pair>` | `static_assert` 失败 |
| `reduce_sum<int32_t>` 输入为 `float` | 类型组合检查或 C API 重载匹配失败 |
| `reduce_sum<int32_t, reduce_scope::pair>` 输入为 `int16_t` | 类型组合检查或 C API 重载匹配失败 |
| 在非 3510 架构实例化接口 | 架构保护或能力检查失败 |

为提高可维护性，建议显式诊断同时包含操作名、`scope` 及受支持的类型组合。

## 7. 测试与验收设计

### 7.1 编译期接口测试

- 使用 `static_assert(std::is_same_v<...>)` 验证三个接口的返回类型。
- 覆盖 `reduce_sum` 的 `vector`、`datablock` 和 `pair` 三条分支。
- 覆盖 `reduce_max`、`reduce_min` 的 `vector` 和 `datablock` 分支。
- 覆盖所有合法类型组合，尤其是 16 位到 32 位扩宽求和。
- 增加负向编译测试，验证非法 `scope` 和非法类型组合产生明确诊断。

### 7.2 指令映射测试

通过 C API 桩或编译产物检查验证每个模板分支调用正确的底层接口：

| 用例 | 预期调用 |
| --- | --- |
| `reduce_sum<float, vector>` | `asc_reduce_sum` |
| `reduce_sum<float, datablock>` | `asc_reduce_sum_datablock` |
| `reduce_sum<float, pair>` | `asc_pair_reduce_sum` |
| `reduce_max<float, vector>` | `asc_reduce_max` |
| `reduce_max<float, datablock>` | `asc_reduce_max_datablock` |
| `reduce_min<float, vector>` | `asc_reduce_min` |
| `reduce_min<float, datablock>` | `asc_reduce_min_datablock` |

### 7.3 功能正确性测试

设备侧测试应至少覆盖：

- 全掩码条件下的正数、负数和混合数据。
- 尾块掩码条件下非激活 lane 不影响归约结果。
- `vector`、`datablock` 和 `pair` 的结果布局。
- 浮点特殊值，包括正负零、无穷和 NaN；期望行为应以底层指令规范为准。
- 整数边界值及可能溢出的输入。
- 归约结果参与后续寄存器计算和 store 的完整链路。

### 7.4 UT 文件组织

归约接口测试使用独立文件：

```text
tests/api/tensor_api/npu_arch_3510/aiv/test_tensor_api_reduction_compute.cpp
```

不将用例并入 `test_tensor_api_reg_tensor.cpp`，以保证 `reg_tensor` 数据结构测试与归约计算测试的职责清晰。当前测试目录由 Tensor API CMake 配置统一收集 `npu_arch_3510/aiv/*.cpp`。

### 7.5 验收标准

1. 三个接口在支持的模板参数组合下可以通过 3510 AIV 编译。
2. 返回类型与显式指定的 `DstType` 一致。
3. 每个 `scope` 映射到预期 C API 和硬件指令。
4. 非法 `scope` 和类型组合在编译期失败。
5. 源掩码正确传递给底层接口，返回对象的掩码行为符合设计。
6. 设备侧结果与参考计算在相应整数规则或浮点误差范围内一致。
7. 归约 UT 保持为独立测试文件。

## 8. 当前实现状态与待闭环项

以下内容属于当前代码状态，不改变前述目标设计：

1. `reduction_compute.h` 和 `reduction_compute_impl.h` 当前仍包含旧路径 `impl/tensor_api/experimental/reg/reg_tensor_impl.h`，而 `reg_tensor` 已迁移到 `include/tensor_api/arch/vector/experimental/reg_tensor.h`。需要更新包含路径后再验证聚合头文件编译。
2. 当前实现已对 `scope` 做 `static_assert`，但合法类型组合主要依赖底层 C API 重载失败。建议增加显式类型检查和可读诊断。
3. 当前独立 UT 主要验证模板实例化和返回类型，尚不足以证明设备侧数值结果、掩码语义及实际指令映射。
4. 当前 UT 的归约调用辅助函数使用 `__aicore__`。为匹配寄存器接口的推荐放置方式，建议将完整寄存器计算链调整到 `__simd_vf__` 函数中，并由 kernel 入口负责调用。
5. 返回掩码直接继承源掩码。对于整寄存器归约后只有部分结果 lane 有意义的场景，应结合底层指令结果布局确认该策略是否满足所有后续操作。

上述待闭环项应在接口进入非实验状态前完成，并纳入回归测试。
