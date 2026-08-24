# local_tensor<EngineType, LayoutType><a name="ZH-CN_TOPIC_0000002000000003"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"tensor_api/tensor/local_tensor.h"`，命名空间为 `asc::te`。

`local_tensor` 是 Unified Buffer（UB）上的 Tensor 视图。它不分配内存，而是通过 `EngineType` 描述地址、通过 `LayoutType` 描述坐标到线性偏移的映射。在 `__simd_vf__` 函数中，`load` 将一个寄存器数据块从 UB 搬入 `reg_tensor`，计算完成后使用 `store` 写回 UB，`local_tensor` 使用模板枚举选择搬运阶段的 sideband 变换。

## 定义原型<a name="section620mcpsimp"></a>

```cpp
template <typename EngineType, typename LayoutType>
struct local_tensor : public base_tensor<EngineType, LayoutType> {
    using tensor_api_base = base_tensor<EngineType, LayoutType>;
    using element_type = typename tensor_api_base::element_type;
    using data_type = get_attribute_element_type<element_type*>;
    using layoutType = typename tensor_api_base::layout_type;

    __aicore__ inline local_tensor() = default;
    __aicore__ inline local_tensor(const EngineType& engine, const LayoutType& layout);

    template <load_sideband_mode sideband_mode = load_sideband_mode::direct,
              typename Coord>
    __simd_callee__ inline decltype(auto) load(const Coord& coord) const;

    template <load_sideband_mode sideband_mode = load_sideband_mode::deintlv,
              typename Coord, typename RegDataType>
    __simd_callee__ inline void load(const Coord& coord,
                                     reg_tensor<RegDataType>& dst0,
                                     reg_tensor<RegDataType>& dst1) const;

    template <broadcast_mode brc_mode = broadcast_mode::elem, typename Coord>
    __simd_callee__ inline decltype(auto) load_broadcast(const Coord& coord) const;

    template <store_sideband_mode sideband_mode = store_sideband_mode::direct,
              typename Coord, typename RegDataType>
    __simd_callee__ inline void store(const Coord& coord,
                                      const reg_tensor<RegDataType>& src);

    template <store_sideband_mode sideband_mode = store_sideband_mode::intlv,
              typename Coord, typename RegDataType>
    __simd_callee__ inline void store(const Coord& coord,
                                      const reg_tensor<RegDataType>& src0,
                                      const reg_tensor<RegDataType>& src1);
};
```

## 模板参数说明<a name="section622mcpsimp"></a>

| 参数名 | 描述 |
| --- | --- |
| `EngineType` | Tensor 存储引擎。引擎的内存位置必须为 `location::ub`。通常由 `make_mem_ptr<location::ub>` 和 `make_tensor` 推导。 |
| `LayoutType` | Tensor 布局类型。`layout(coord)` 返回坐标对应的元素偏移，地址计算由 Tensor 引擎和该偏移共同完成。 |
| `Coord` | 坐标类型，通常由 `make_coord(...)` 生成。坐标单位为元素，必须满足布局的维度和边界要求。 |
| `RegDataType` | `reg_tensor` 的元素类型。必须与 `local_tensor::data_type` 保持一致。 |

构造 `local_tensor` 示例如下：

```cpp
auto mem = make_mem_ptr<location::ub>(ub_addr);
auto layout = make_layout(make_shape(256));
using EngineType = view_engine<decltype(mem)>;
local_tensor<EngineType, decltype(layout)> tensor(mem, layout);
```

## Sideband 和广播模式<a name="section630mcpsimp"></a>

### load_sideband_mode

| 取值 | 说明 | 底层指令 |
| --- | --- | --- |
| `direct` | 普通对齐搬入。 | `asc_loadalign` |
| `deintlv` | 解交织搬入，使用双目的 `load(coord, dst0, dst1)` 重载。 | `asc_loadalign_deintlv` |
| `downsample` | 下采样搬入。 | `asc_loadalign_downsample` |
| `unpack` | 解包搬入。 | `asc_loadalign_unpack` |
| `unpack4` | 4 倍解包搬入。 | `asc_loadalign_unpack4` |
| `upsample` | 上采样搬入。 | `asc_loadalign_upsample` |

### store_sideband_mode

| 取值 | 说明 | 底层指令 |
| --- | --- | --- |
| `direct` | 普通对齐搬出，使用源 `reg_tensor` 中的 mask。 | `asc_storealign` |
| `store_1st` | 仅搬出源寄存器的第一个元素，尝在归约场景中将结果写回。该模式不使用 mask。 | `asc_storealign_1st` |
| `intlv` | 两个 `reg_tensor` 交织搬出，必须使用双源 `store(coord, src0, src1)` 重载。 | `asc_storealign_intlv` |
| `pack` | 按硬件规则压缩后搬出低半位数据，使用源寄存器 mask。 | `asc_storealign_pack` |
| `pack_quarter` | 按硬件规则压缩后搬出低四分之一数据，使用源寄存器 mask。 | `asc_storealign_pack_quarter` |

### broadcast_mode

| 取值 | 说明 | 底层指令 |
| --- | --- | --- |
| `elem` | 读取一个元素并广播到整拍。 | `asc_loadalign_brc_elem` |
| `datablock` | 读取一个 32B DataBlock 并广播。 | `asc_loadalign_brc_datablock` |
| `elem2datablock` | 将元素扩展到 DataBlock 后广播。 | `asc_loadalign_brc_elem2datablock` |

## 函数说明<a name="section640mcpsimp"></a>

### load<a name="section641mcpsimp"></a>

#### 函数原型

```cpp
template <load_sideband_mode sideband_mode = load_sideband_mode::direct,
          typename Coord>
__simd_callee__ inline decltype(auto) load(const Coord& coord) const;

template <load_sideband_mode sideband_mode = load_sideband_mode::deintlv,
          typename Coord, typename RegDataType>
__simd_callee__ inline void load(const Coord& coord,
                                reg_tensor<RegDataType>& dst0,
                                reg_tensor<RegDataType>& dst1) const;
```

#### 功能说明

从 UB 按 `coord` 计算地址并对齐搬入一个寄存器数据块。单目的重载返回 `reg_tensor<data_type>`，返回值不自动绑定 mask；尾块处理时应使用 `with_mask` 显式绑定谓词。`deintlv` 只允许双目的重载，两个目的寄存器分别接收解交织后的数据。

#### 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `coord` | 输入 | 逻辑坐标，通常为 `make_coord(offset)`。`offset` 按元素计，实际地址由 `engine() + layout()(coord)` 计算。 |
| `dst0`、`dst1` | 输出 | 解交织模式的两个目标寄存器，类型为 `reg_tensor<RegDataType>`。两个寄存器的元素类型必须与 Tensor 元素类型一致。 |

#### 返回值说明

单目的 `load` 返回 `reg_tensor<data_type>`；双目的 `load` 无返回值。

#### 约束说明

- 仅支持位于 UB 的 `local_tensor`。

### load_broadcast<a name="section642mcpsimp"></a>

#### 函数原型

```cpp
template <broadcast_mode brc_mode = broadcast_mode::elem, typename Coord>
__simd_callee__ inline decltype(auto) load_broadcast(const Coord& coord) const;
```

#### 功能说明

从 UB 读取一个元素或一个 DataBlock，并按照 `broadcast_mode` 广播为整拍 `reg_tensor<data_type>`。该接口适合 bias、scale 等在多个 lane 中复用的输入。

#### 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `coord` | 输入 | 广播源坐标。地址由 Tensor 的引擎和布局计算。 |

#### 返回值说明

返回 `reg_tensor<data_type>`。返回值需要参与尾块计算时，可通过 `with_mask` 绑定对应谓词。

#### 约束说明

- 仅支持位于 UB 的 `local_tensor`。
- 地址对齐约束与对应 `asc_loadalign_brc_*` 指令一致。

### store（单源）<a name="section643mcpsimp"></a>

#### 函数原型

```cpp
template <store_sideband_mode sideband_mode = store_sideband_mode::direct,
          typename Coord, typename RegDataType>
__simd_callee__ inline void store(const Coord& coord,
                                  const reg_tensor<RegDataType>& src);
```

#### 功能说明

将一个 `reg_tensor` 按坐标对齐搬出到 UB。`direct`、`pack` 和 `pack_quarter` 从 `src.mask` 读取有效 lane；`store_1st` 仅写回第一个元素并忽略 mask。`store` 没有独立的 mask 参数，调用方应在计算结果上使用 `with_mask` 绑定谓词。

#### 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `coord` | 输入 | 目标逻辑坐标，通常为 `make_coord(offset)`。 |
| `src` | 输入 | 源寄存器，类型为 `reg_tensor<RegDataType>`。`RegDataType` 必须与 Tensor 元素类型一致；有效谓词保存在 `src.mask` 中。 |

#### 返回值说明

无。

#### 约束说明

- 仅支持位于 UB 的 `local_tensor`。
- `sideband_mode::intlv` 必须使用双源重载。
- 未调用 `with_mask` 的 `reg_tensor` 可能包含未初始化 mask；尾块写回前必须显式绑定 `update_mask` 或 `all_mask`。
- 地址对齐和压缩规则与对应 `asc_storealign*` 指令一致。

### store（双源 intlv）<a name="section644mcpsimp"></a>

#### 函数原型

```cpp
template <store_sideband_mode sideband_mode = store_sideband_mode::intlv,
          typename Coord, typename RegDataType>
__simd_callee__ inline void store(const Coord& coord,
                                  const reg_tensor<RegDataType>& src0,
                                  const reg_tensor<RegDataType>& src1);
```

#### 功能说明

仅当 `sideband_mode == store_sideband_mode::intlv` 时生效。两个源寄存器中的元素按 lane 交错写回 UB。双源重载不接受显式 mask，当前实现调用的 `asc_storealign_intlv` 也不传递谓词；是否需要尾块保护应按硬件指令约束保证输入范围。

#### 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `coord` | 输入 | 目标逻辑坐标。 |
| `src0`、`src1` | 输入 | 两个源寄存器，类型必须相同，且元素类型必须与 Tensor 元素类型一致。 |

#### 返回值说明

无。

#### 约束说明

- 只有 `store_sideband_mode::intlv` 支持双源重载；其他模式会触发编译期错误。
- 双源搬出会写入两路寄存器对应的交织数据，目标 UB 空间和地址对齐需满足 `asc_storealign_intlv` 要求。

## 数据类型<a name="section650mcpsimp"></a>

Tensor 元素类型与 `reg_tensor` 元素类型必须一致。当前寄存器类型映射包括 `bool`、`int8_t`/`uint8_t`、`int16_t`/`uint16_t`、`int32_t`/`uint32_t`、`int64_t`/`uint64_t`、`half`、`bfloat16_t`、`float`、hifloat8、FP8、int4x2 和 fp4x2 类型；具体指令对元素位宽和地址对齐的限制以对应 C API 为准。`reg_tensor` 的数据结构和生命周期说明请参考[RegTensor](register_data_types/RegTensor.md)。

## 调用示例<a name="section660mcpsimp"></a>

### 单源搬入、计算和搬出

下面示例使用 `update_mask` 处理尾块。示例中的 `update_mask` 当前支持 b8、b16 和 b32 元素类型，且 `count` 不应超过 Tensor 布局容量。
其中，`load` 返回的 `reg_tensor` 不带有效 mask。推荐使用 `load(...).with_mask(update_mask<T>(remain))` 的链式写法。

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

template <typename T>
__simd_vf__ inline void LocalTensorCopy(__ubuf__ T* dst_addr,
                                        __ubuf__ T* src_addr,
                                        uint32_t count)
{
    auto layout = make_layout(make_shape(256));
    auto dst = make_tensor(make_mem_ptr<location::ub>(dst_addr), layout);
    auto src = make_tensor(make_mem_ptr<location::ub>(src_addr), layout);

    constexpr uint32_t vl = 256 / sizeof(T);
    uint32_t remain = count;
    for (uint32_t offset = 0; offset < count; offset += vl) {
        auto mask = update_mask<T>(remain);
        auto value = src.load(make_coord(offset)).with_mask(mask);
        dst.store(make_coord(offset), value);
    }
}
```

### 广播、解交织和交织搬运

```cpp
__simd_vf__ inline void LocalTensorSideband(__ubuf__ uint8_t* dst_addr,
                                            __ubuf__ uint8_t* src_addr)
{
    auto layout = make_layout(make_shape(256));
    auto dst = make_tensor(make_mem_ptr<location::ub>(dst_addr), layout);
    auto src = make_tensor(make_mem_ptr<location::ub>(src_addr), layout);

    auto bias = src.load_broadcast<broadcast_mode::elem>(make_coord(0));

    reg_tensor<uint8_t> even;
    reg_tensor<uint8_t> odd;
    src.load<load_sideband_mode::deintlv>(make_coord(0), even, odd);
    // intlv 写回不接收独立 mask；调用方需确保目标空间和输入范围满足硬件要求。
    dst.store<store_sideband_mode::intlv>(make_coord(0), even, odd);
    (void)bias;
}
```
