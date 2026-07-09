# AscendC 高阶 API Tiling 与 GE 类型解耦方案

## 需求

客户希望 AscendC 对外接口与 GE 解耦，避免高阶 API Tiling 接口在公开函数原型和资料中直接暴露 `ge::Shape`、`ge::DataType`，减少用户对 AscendC 依赖 GE 的误解。

本次重点问题包括：

- `GetCumSumMaxMinTmpSize` 等高阶 API Tiling 接口入参使用 `const ge::Shape&`。
- 多个 tiling 头文件直接 `#include "graph/tensor.h"`，导致用户侧需要感知 GE/metadef 头文件。
- 归约、排序、转置、量化等接口或资料中暴露 `ge::DataType`。
- 资料中需要解释 `DataCopy` 和 `Copy` 的能力边界，避免把二者理解为同一类接口。

## 背景简述

### 当前 devkit 侧问题

以 CumSum 为例，当前公开头文件 `include/adv_api/math/cumsum_tiling.h` 存在如下形式：

```cpp
#include "graph/tensor.h"

namespace AscendC {
void GetCumSumMaxMinTmpSize(
    const ge::Shape& srcShape, const uint32_t typeSize, const bool isLastAxis, const bool isReuseSource,
    uint32_t& maxValue, uint32_t& minValue);
} // namespace AscendC
```

实现文件 `impl/adv_api/tiling/math/cumsum_tiling.cpp` 直接使用 `ge::Shape` 的如下接口：

- `GetDimNum()`
- `GetDim(size_t idx)`
- `GetShapeSize()`

同类问题不仅存在于 CumSum。`include/adv_api` 下大量 `*_tiling.h` 文件直接 include `graph/tensor.h`，并在公开接口中使用 `ge::Shape`；部分接口还使用 `ge::DataType`，例如 reduce、sort、transpose、quantization 等。

### metadef 侧类型对照

metadef 仓中相关定义位置：

- `ge::Shape`：`/root/metadef/inc/external/graph/tensor.h`
- `ge::DataType`：`/root/metadef/inc/external/graph/types.h`
- C dtype 编码：`/root/metadef/inc/external/graph/c_types.h`
- `gert::Shape`：`/root/metadef/inc/external/exe_graph/runtime/shape.h`

`ge::Shape` 是 PIMPL 类，公开行为包括：

```cpp
size_t GetDimNum() const;
int64_t GetDim(size_t idx) const;
std::vector<int64_t> GetDims() const;
int64_t GetShapeSize() const;
```

`ge::Shape::GetShapeSize()` 的关键语义：

- dim 为空时返回 `0`。
- dim 包含 `-1` 或 `-2` 时返回 `-1`，表示 unknown shape。
- dim 包含 `0` 时返回 `0`，表示空 tensor。
- 乘积溢出时返回 `0`。

`gert::Shape` 是标准布局类型，最大 25 维，但其 `GetShapeSize()` 在 dim 数为 0 时返回 `1`。该语义和当前高阶 API Tiling 使用的 `ge::Shape` 不完全一致，因此不建议直接用 `gert::Shape` 替代 `ge::Shape` 作为公开 API 类型。

`DataType` 方面，`include/basic_api/kernel_type.h` 的非 NPU 编译分支中 `DT_*` 宏定义与 metadef `c_types.h` 中 `C_DT_*` 编码一致。本需求不需要重新定义一套不兼容的 dtype 编码，重点是将公开接口和资料从 `ge::DataType` 迁移到 `AscendC::DataType` 命名空间，并保留旧接口兼容。

## 涉及领域

- AscendC 高阶 API Tiling Host 侧接口。
- `include/adv_api` 公开头文件。
- `impl/adv_api/tiling` Host 侧 tiling 实现。
- `docs/api/SIMD-API/高阶API` 资料。
- 示例代码和 UT。
- GE/metadef 兼容适配层。
- `DataCopy`/`Copy` 文档解释。

## 方案详述

### 总体原则

采用“新增 AscendC 原生类型 + 新接口优先 + 旧 GE 接口兼容”的方式迁移。

目标不是立即删除 `ge::Shape` / `ge::DataType` 支持，而是让新用户默认只看到 AscendC 类型；老用户已有代码仍能编译，通过兼容接口平滑迁移。

### 1. 新增 `AscendC::Shape`

新增 AscendC 原生 Host/Tiling shape 类型，例如：

```cpp
namespace AscendC {
class Shape {
public:
    Shape();
    explicit Shape(const std::vector<int64_t>& dims);
    Shape(std::initializer_list<int64_t> dims);

    size_t GetDimNum() const;
    int64_t GetDim(size_t idx) const;
    const std::vector<int64_t>& GetDims() const;
    int64_t GetShapeSize() const;

private:
    std::vector<int64_t> dims_;
};
} // namespace AscendC
```

行为要求对齐当前 tiling API 依赖的 `ge::Shape` 语义：

- `UNKNOWN_DIM = -1`
- `UNKNOWN_DIM_NUM = -2`
- 构造时如果包含 `UNKNOWN_DIM_NUM`，按 `ge::Shape` 行为归一为 `{-2}`。
- `GetDimNum()` 遇到 unknown rank 返回 `0`。
- `GetDim(idx)` 越界返回 `0`。
- `GetShapeSize()`：
  - 空 dims 返回 `0`。
  - 包含 `-1` 或 `-2` 返回 `-1`。
  - 包含 `0` 返回 `0`。
  - 乘积溢出返回 `0`。

该类型建议放在高阶 API Host/Tiling 公共头文件中，例如：

```text
include/adv_api/tiling/shape.h
```

也可以根据现有目录习惯放入：

```text
include/adv_api/utils/shape.h
```

但不建议放入 device 侧核心 tensor 头中，避免 Host tiling 类型污染 device 编程模型。

### 2. 使用现有 `AscendC::DataType`

对外接口统一使用 `AscendC::DataType`，不再在新资料中展示 `ge::DataType`。

处理策略：

- 不新增一套与现有 dtype 编码不一致的枚举。
- 优先复用 `include/basic_api/kernel_type.h` 中已有 `DT_*` 定义。
- 如果某些高阶 tiling 实现需要 dtype size，补充 AscendC 命名空间下的工具函数，例如：

```cpp
namespace AscendC {
int32_t GetSizeByDataType(DataType dataType);
int64_t GetSizeInBytes(int64_t elementCount, DataType dataType);
}
```

实现可参考 metadef `ge::GetSizeByDataType` / `ge::GetSizeInBytes` 的编码规则。对 bit 级 dtype 需要保留特殊返回约定，例如 `1000 + bitSize` 或在接口文档中明确返回单位和含义。

### 3. 新接口与旧接口分层

以 CumSum 为样板，新接口改为：

```cpp
namespace AscendC {
void GetCumSumMaxMinTmpSize(
    const AscendC::Shape& srcShape,
    const uint32_t typeSize,
    const bool isLastAxis,
    const bool isReuseSource,
    uint32_t& maxValue,
    uint32_t& minValue);
} // namespace AscendC
```

旧接口保留：

```cpp
namespace AscendC {
void GetCumSumMaxMinTmpSize(
    const ge::Shape& srcShape,
    const uint32_t typeSize,
    const bool isLastAxis,
    const bool isReuseSource,
    uint32_t& maxValue,
    uint32_t& minValue);
} // namespace AscendC
```

旧接口只作为兼容入口，内部转换：

```cpp
AscendC::Shape ToAscendCShape(const ge::Shape& shape)
{
    return AscendC::Shape(shape.GetDims());
}
```

再调用新接口，确保新旧接口行为一致。

### 4. 实现层抽公共逻辑

不要在新旧接口中复制 tiling 计算逻辑。建议拆成内部公共函数：

```cpp
namespace AscendC::Internal {
void GetCumSumMaxMinTmpSizeImpl(
    const AscendC::Shape& srcShape,
    uint32_t typeSize,
    bool isLastAxis,
    bool isReuseSource,
    uint32_t& maxValue,
    uint32_t& minValue);
}
```

新接口直接调用 `Impl`，旧 GE 接口转换后调用 `Impl`。

这样后续改 bug、补边界检查、优化逻辑时只维护一份实现。

### 5. 头文件作用域调整

目标：新用户 include AscendC tiling 头文件时，不需要额外包含 `graph/tensor.h`。

推荐拆分方式：

```text
include/adv_api/math/cumsum_tiling.h
include/adv_api/math/cumsum_tiling_ge_compat.h
```

`cumsum_tiling.h`：

- 只包含 AscendC 自有头文件。
- 只声明 `AscendC::Shape` 新接口。
- 不 include `graph/tensor.h`。

`cumsum_tiling_ge_compat.h`：

- include `graph/tensor.h`。
- 声明旧 `ge::Shape` overload。
- 标记为兼容接口，资料中不作为主推入口。

如果不希望新增 compat 头，也可以在同一头文件中通过宏控制：

```cpp
#ifdef ASCENDC_ENABLE_GE_COMPAT
#include "graph/tensor.h"
// ge::Shape overload
#endif
```

但默认头文件不应强制 include `graph/tensor.h`。

### 6. 批量迁移范围

第一阶段先做基础能力和 CumSum 样板：

- 新增 `AscendC::Shape`。
- 新增或统一 `AscendC::GetSizeByDataType`。
- 改造 `GetCumSumMaxMinTmpSize`。
- 更新 CumSum API 资料和样例。
- 增加新旧接口结果一致性 UT。

第二阶段批量推广到单 shape + typeSize 的 tiling API：

- activation：Gelu、Sigmoid、Swish、Silu、LogSoftMax、SoftMax 等。
- math：Axpy、Sin、Cos、Exp、Log、Cosh、Asin、Cumsum 等。
- normalization：LayerNorm、RmsNorm、DeepNorm、GroupNorm 等。
- quantization：Quantize、Dequantize、AscendQuant、AscendDequant 等。
- pad/select/transpose 中不涉及 dtype enum 的接口。

第三阶段处理含 dtype enum 的接口：

- reduce：`GetReduce*MaxMinTmpSize`
- sort：`GetSortMaxMinTmpSize`、`GetTopKMaxMinTmpSize`
- transpose：`GetTransDataMaxMinTmpSize`
- antiquantize/ascend_antiquant 等量化接口

这些接口新增 `AscendC::DataType` overload，旧 `ge::DataType` overload 保留兼容。

### 7. 资料修改方案

资料中主函数原型改为 AscendC 类型，例如：

```cpp
void GetCumSumMaxMinTmpSize(
    const AscendC::Shape& srcShape,
    const uint32_t typeSize,
    const bool isLastAxis,
    const bool isReuseSource,
    uint32_t& maxValue,
    uint32_t& minValue);
```

调用示例改为：

```cpp
AscendC::Shape srcShape({32, 32});
uint32_t maxValue = 0;
uint32_t minValue = 0;
AscendC::GetCumSumMaxMinTmpSize(srcShape, 2, true, false, maxValue, minValue);
```

兼容说明放在“兼容性说明”小节：

```text
历史版本支持使用 ge::Shape / ge::DataType 调用 Tiling 接口。
为减少 AscendC 与 GE 命名空间耦合，新代码建议使用 AscendC::Shape / AscendC::DataType。
ge::Shape / ge::DataType 形式作为兼容接口保留。
```

同时修正 CumSum 样例中 `isLastAxis` 和 `isReuseSource` 注释顺序不一致的问题。

### 8. DataCopy 与 Copy 资料解释

该问题不建议改接口，只补资料说明。

建议在数据搬运导览和 UBToUB 数据搬运页面增加说明：

- `DataCopy`：描述存储空间之间的数据搬运行为，覆盖 GM、UB、L1、L0 等不同存储空间通路，也包括部分 UB 到 UB 搬运形态。
- `Copy`：描述 VECTOR pipeline 上的 UB 到 UB 矢量搬运能力，重点支持 mask、repeat、stride 等控制能力。
- 简单连续搬运优先按数据通路选择 `DataCopy`。
- 需要 mask 掩码或 VECTOR stride 控制时使用 `Copy`。

建议说明文本：

```text
DataCopy 和 Copy 的定位不同。DataCopy 面向不同存储空间之间的数据搬运通路，强调数据在 GM、UB、L1、L0 等存储位置之间移动；Copy 面向 VECTOR 计算流水上的 UB 到 UB 矢量拷贝，支持 mask 掩码、repeat 和 stride 等矢量控制参数。两者名称相近，但不是同一层级接口。
```

### 9. 兼容性策略

兼容原则：

- 不删除旧 `ge::Shape` / `ge::DataType` 接口。
- 新接口作为资料和样例主推入口。
- 旧接口可增加 deprecated 标记，但不建议第一阶段立即强制报 warning，避免影响存量构建。
- 如果引入 deprecated，建议分版本推进：
  - 当前版本：新增 AscendC 类型接口，旧接口保留。
  - 后续版本：资料标记旧接口不推荐。
  - 再后续版本：按版本策略决定是否增加编译期 deprecated 提示。

### 10. 验证方案

基础类型验证：

- `AscendC::Shape({1, 1}).GetShapeSize() == 1`
- `AscendC::Shape({0, 1}).GetShapeSize() == 0`
- `AscendC::Shape({-1, 1}).GetShapeSize() == -1`
- `AscendC::Shape({-2}).GetDimNum() == 0`
- `AscendC::Shape({-2}).GetDims().size() == 1`
- 越界 `GetDim()` 返回 `0`
- int64 乘积溢出返回 `0`

接口一致性验证：

- 同一组 shape/typeSize/config 下，新 `AscendC::Shape` 接口和旧 `ge::Shape` 接口输出的 `maxValue/minValue` 完全一致。
- CumSum 覆盖：
  - `isLastAxis = true/false`
  - `typeSize = sizeof(uint16_t)/sizeof(float)`
  - inner 32B 对齐和非对齐错误路径
  - 维度不是 2 的错误路径

头文件验证：

- 只 include `include/adv_api/math/cumsum_tiling.h`，不 include `graph/tensor.h`，可编译调用新接口。
- include `include/adv_api/math/cumsum_tiling_ge_compat.h` 后，可编译调用旧 `ge::Shape` 接口。

资料验证：

- CumSum 资料原型、参数顺序、示例代码一致。
- `ge::Shape` / `ge::DataType` 只出现在兼容性说明中。
- DataCopy/Copy 的说明覆盖二者差异。

## 推荐落地路径

建议按如下顺序落地：

1. 新增 `AscendC::Shape`，补基础 UT。
2. 新增 dtype size 工具函数，复用现有 `DT_*` 编码，不改变现有 dtype 对外值。
3. 改造 `GetCumSumMaxMinTmpSize` 为样板接口。
4. 调整 CumSum 资料、样例和 UT。
5. 评审样板接口通过后，批量迁移其他 `*_tiling.h`。
6. 最后统一补充 DataCopy/Copy 资料说明。

该方案可以解决客户对 `ge::` 命名空间的误解，同时保留历史接口兼容，不要求存量代码一次性迁移。
