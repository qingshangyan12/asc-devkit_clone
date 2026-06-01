# Ternary Transform 链路实现设计

## Context

为 `impl/tensor_api/work/api.md` 中的 14 个 SIMD ternary CAPI 接入 Transform 分发框架。
这些 CAPI 全部来自 Layer A (`include/c_api/reg_compute/reg_vector.h`)，三参数签名统一为
`(src, value, mask)`，加上 dst 共 4 个参数。

命名遵循项目惯例：项目中 Axpy 操作已使用 `ternary_scalar` 命名（如 `kernel_operator_vec_ternary_scalar_intf.h`），
因此新路径使用 **Ternary** 命名，与现有 **Binary** 对称。

## 14 个 API 分两类

| 分类 | `isCompare` | 包含 | dst/src 类型关系 | store 方式 |
|------|:---:|------|-----------------|-----------|
| 非比较(9) | `false` | 算术(5): AddScalar, MulScalar, MaxScalar, MinScalar, Axpy; 移位(2): ShiftLeftScalar, ShiftRightScalar; 混合(2): Muls, MulsV2 | dst/src 由 DstRegType/SrcRegType 自动映射 | `asc_storealign(ptr, reg, mask)` |
| 比较(5) | `true` | EqScalar, GeScalar, LeScalar, LtScalar, NeScalar | dst = **vector_bool** | `asc_storealign(ptr, vector_bool)` (2参数) |

## 文件变更清单

| 操作 | 文件 |
|------|------|
| 新建 | `impl/tensor_api/arch/vector/ternary/instruction.h` |
| 新建 | `impl/tensor_api/arch/vector/ternary/ternary_vf.h` |
| 修改 | `impl/tensor_api/arch/vector/transform/transform_vf.h` |
| 不变 | `impl/tensor_api/algorithm/transform_impl.h` |

## 文件 1: `ternary/instruction.h`

在 `AscendC::Te::Inst` 命名空间定义 14 个指令包装类。
每个类带 `static constexpr bool isCompare` 标记，供 TernaryVF 的 `if constexpr` 使用。

**算术类** (isCompare = false, dst == src)：
```cpp
class AddScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_add_scalar(dst, src, value, mask);
        }
    }
};
```
MulScalar, MaxScalar, MinScalar 同样模式。

**比较类** (isCompare = true, dst = vector_bool)：
```cpp
class EqScalar {
public:
    static constexpr bool isCompare = true;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(vector_bool& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_eq_scalar(dst, src, value, mask);
        }
    }
};
```
GeScalar, LeScalar, LtScalar, NeScalar 同样模式。

**移位类** (isCompare = false, value = int16_t)：
```cpp
class ShiftLeftScalar {
public:
    static constexpr bool isCompare = false;
    template <typename T>
    __simd_callee__ inline static void Run(T& dst, T src, int16_t value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_shiftleft_scalar(dst, src, value, mask);
        }
    }
};
```

**Axpy** (isCompare = false, 需额外 load dst)：
```cpp
class Axpy {
public:
    static constexpr bool isCompare = false;
    template <typename T, typename S>
    __simd_callee__ inline static void Run(T& dst, T src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_axpy(dst, src, value, mask);
        }
    }
};
```

**混合类** (isCompare = false, dst != src)：
```cpp
class Muls {
public:
    static constexpr bool isCompare = false;
    template <typename DstReg, typename SrcReg, typename S>
    __simd_callee__ inline static void Run(DstReg& dst, SrcReg src, S value, vector_bool mask) {
        if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
            asc_muls(dst, src, value, mask);
        }
    }
};
```
MulsV2 同样模式（调用 `asc_muls_v2`）。

## 文件 2: `ternary/ternary_vf.h`

### TernaryVF — 向量化循环体

```cpp
template<typename CalcFunc, typename TraitType>
class TernaryVF {
public:
    template<typename DstType, typename SrcType, typename S>
    __simd_vf__ inline static void Run(
        __ubuf__ DstType* dst, __ubuf__ SrcType* src, S value,
        uint16_t repeat, uint16_t oneRepSize, uint32_t dataSize)
    {
        using VectorTypeTransform = TupleMap<...>;  // 同 BinaryVF

        using SrcRegType = typename VectorTypeTransform::template Get<SrcType>;
        using DstRegType = typename VectorTypeTransform::template Get<DstType>;

        vector_bool vmask;
        SrcRegType reg_src;

        for (uint16_t i = 0; i < repeat; i++) {
            vmask = Inst::UpdateMask::template Run<SrcType>(dataSize);
            asc_loadalign(reg_src, src + i * oneRepSize);

            if constexpr (CalcFunc::isCompare) {
                vector_bool reg_dst;
                CalcFunc::template Run(reg_dst, reg_src, value, vmask);
                asc_storealign(dst + i * oneRepSize, reg_dst);
            } else {
                DstRegType reg_dst;
                if constexpr (Std::is_same_v<CalcFunc, Inst::Axpy>) {
                    asc_loadalign(reg_dst, dst + i * oneRepSize);
                }
                CalcFunc::template Run(reg_dst, reg_src, value, vmask);
                asc_storealign(dst + i * oneRepSize, reg_dst, vmask);
            }
        }
    }
};
```

关键设计点：
- **双寄存器类型**：`SrcRegType` 和 `DstRegType` 分别映射，覆盖所有子类：
  - 算术/移位：DstRegType == SrcRegType
  - 比较：DstRegType 不使用（走 vector_bool 分支）
  - 混合(Muls)：DstRegType = vector_half, SrcRegType = vector_float
- **比较类分支**：`if constexpr (CalcFunc::isCompare)` — dst 用 `vector_bool`，store 用 2 参数 `asc_storealign(ptr, vector_bool)`
- **Axpy 分支**：`if constexpr (is_same_v<CalcFunc, Inst::Axpy>)` — 额外 load dst

### Transform2TernaryVF — 张量层适配

```cpp
template<typename CalcFunc, typename TraitType>
class Transform2TernaryVF {
public:
    template<typename DstTensor, typename SrcTensor, typename S>
    __aicore__ inline static void Run(const DstTensor& dst, const SrcTensor& src, const S& value)
    {
        using dstType = GetAttributeElementType<typename DstTensor::elementType*>;
        using srcType = GetAttributeElementType<typename SrcTensor::elementType*>;

        uint32_t dataSize = src.Size();

        constexpr uint16_t VECTOR_REG_WIDTH = 256;
        constexpr uint16_t oneRepSize = VECTOR_REG_WIDTH / sizeof(srcType);
        uint16_t repeat = Std::ceil_division(dataSize, oneRepSize);

        TernaryVF<CalcFunc, TraitType>::template Run<dstType, srcType>(
            dst.Data().Get(), src.Data().Get(), value,
            repeat, oneRepSize, dataSize);
    }
};
```

关键设计点：
- 从 dst tensor 和 src tensor **分别**提取 element type
- `oneRepSize` 和 `dataSize` 基于 **srcType**（src 决定每次迭代处理多少元素）
- 比较类：dst tensor element type 为 `uint8_t`
- 混合类(Muls)：dst tensor element type 为 `half`，src tensor element type 为 `float`

## 文件 3: `transform/transform_vf.h` 修改

新增 include 和 ternarySet：

```cpp
#include "impl/tensor_api/arch/vector/ternary/ternary_vf.h"

// 在 Transform2VF 类内部新增：
using ternarySet = Std::tuple<
    Inst::MulScalar, Inst::AddScalar, Inst::MaxScalar, Inst::MinScalar,
    Inst::EqScalar, Inst::GeScalar, Inst::LeScalar, Inst::LtScalar,
    Inst::NeScalar, Inst::Axpy, Inst::ShiftLeftScalar, Inst::ShiftRightScalar,
    Inst::Muls, Inst::MulsV2>;

using transformSet = Std::tuple<
    Std::tuple<binarySet,  Transform2BinaryVF<CalcFunc, TraitType>>,
    Std::tuple<castSet,    Transform2CastVF<CalcFunc, TraitType>>,
    Std::tuple<ternarySet, Transform2TernaryVF<CalcFunc, TraitType>>>;
```

## 调用链路

```
Transform<Inst::AddScalar>(dstTensor, srcTensor, scalarValue)
  → Transform2VF<Inst::AddScalar>::Run(...)
    → FindTransform 匹配 ternarySet
    → Transform2TernaryVF::Run(dstTensor, srcTensor, scalarValue)
      → TernaryVF::Run<dstType, srcType>(dst_ptr, src_ptr, value, repeat, ...)
        → for each repeat:
          → UpdateMask<SrcType>(dataSize)
          → asc_loadalign(reg_src, src + offset)
          → [算术/移位/混合] Inst::Run(reg_dst, reg_src, value, vmask)
                             asc_storealign(dst + offset, reg_dst, vmask)
          → [比较]           Inst::Run(bool_dst, reg_src, value, vmask)
                             asc_storealign(dst + offset, bool_dst)
          → [Axpy]           asc_loadalign(reg_dst, dst + offset)
                             Inst::Run(reg_dst, reg_src, value, vmask)
                             asc_storealign(dst + offset, reg_dst, vmask)
```

## 验证

- 编译检查：14 个 Inst 类正确定义，`isCompare` 标记正确
- 类型分发：`Transform<Inst::AddScalar>` 路由到 `Transform2TernaryVF`
- 比较类 store：确认 `asc_storealign(uint8_t*, vector_bool)` 2 参数版本可用
- 混合类(Muls)：确认 float→half 的 mask 语义和 store 正确性
- 如有测试框架，编写单元测试
