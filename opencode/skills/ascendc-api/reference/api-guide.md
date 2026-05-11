# 3510 RegBase API 参考

本文档提供 3510 架构下 `Reg::*` 寄存器操作 API 的完整参考，用于开发 RegBase 计算类接口实现。

---

## 1. 数据搬运

### LoadAlign — UB → Reg 对齐加载

```cpp
// 基本加载
Reg::LoadAlign<T>(reg, ubuf_ptr);

// 带分布模式的加载（如 half 解包）
Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(reg, ubuf_ptr);
```

### StoreAlign — Reg → UB 对齐存回

```cpp
// 基本存回（带掩码控制）
Reg::StoreAlign<T>(ubuf_ptr, reg, mask);

// 带分布模式的存回（如 half 打包）
Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(ubuf_ptr, reg, mask);
```

### LoadDist / StoreDist 分布模式

| 分布模式 | 方向 | 说明 |
|---------|------|------|
| `DIST_UNPACK_B16` | Load | 将 packed B16(2x half) 解包为 32bit lane |
| `DIST_PACK_B32` | Store | 将 32bit lane 结果打包为 B16 |

---

## 2. 掩码 (Mask)

### 创建与更新

```cpp
// 创建掩码寄存器对象
Reg::MaskReg mask;

// 更新掩码 —— 仅前 count 个 lane 有效（用于尾块处理）
mask = Reg::UpdateMask<T>(calCount);

// 带 Trait 的更新（用于 64 位类型等场景）
mask = Reg::UpdateMask<T, Trait>(calCount);
```

### 掩码合并模式 (MaskMergeMode)

| 模式 | 说明 |
|------|------|
| `ZEROING` | 非掩码 lane 结果清零 |
| (默认) | 非掩码 lane 保持原值 |

---

## 3. 算术运算

### 二元向量运算

| 操作 | 签名 | 说明 |
|------|------|------|
| `Reg::Add` | `Add(dst, src0, src1, mask)` | `dst = src0 + src1` |
| `Reg::Sub` | `Sub(dst, src0, src1, mask)` | `dst = src0 - src1` |
| `Reg::Mul` | `Mul(dst, src0, src1, mask)` | `dst = src0 * src1` |
| `Reg::Div` | `Div(dst, src0, src1, mask)` | `dst = src0 / src1` |
| `Reg::Max` | `Max(dst, src0, src1, mask)` | `dst = max(src0, src1)` |
| `Reg::Min` | `Min(dst, src0, src1, mask)` | `dst = min(src0, src1)` |

### 向量-标量运算

| 操作 | 签名 | 说明 |
|------|------|------|
| `Reg::Adds` | `Adds(dst, src, scalar, mask)` | `dst = src + scalar` |
| `Reg::Muls` | `Muls(dst, src, scalar, mask)` | `dst = src * scalar` |
| `Reg::Maxs` | `Maxs(dst, src, scalar, mask)` | `dst = max(src, scalar)` |
| `Reg::Mins` | `Mins(dst, src, scalar, mask)` | `dst = min(src, scalar)` |

### 融合运算

| 操作 | 签名 | 说明 |
|------|------|------|
| `Reg::Axpy` | `Axpy(dst, src, scalar, mask)` | `dst = dst + scalar * src` |
| `Reg::Fma` | `Fma(dst, a, b, c, mask)` | `dst = a * b + c` |

---

## 4. 位运算

| 操作 | 签名 | 说明 |
|------|------|------|
| `Reg::And` | `And(dst, src0, src1, mask)` | `dst = src0 & src1` |
| `Reg::Or` | `Or(dst, src0, src1, mask)` | `dst = src0 \| src1` |
| `Reg::Xor` | `Xor(dst, src0, src1, mask)` | `dst = src0 ^ src1` |
| `Reg::Not` | `Not(dst, src, mask)` | `dst = ~src` |

---

## 5. 逻辑运算 (Mask)

| 操作 | 签名 | 说明 |
|------|------|------|
| `Reg::MaskAnd` | `MaskAnd(dst, src0, src1, mask)` | 逻辑与 |
| `Reg::MaskOr` | `MaskOr(dst, src0, src1, mask)` | 逻辑或 |
| `Reg::MaskXor` | `MaskXor(dst, src0, src1, mask)` | 逻辑异或 |

逻辑运算操作的是 `bool` 等效值（通常 `int8_t`/`uint8_t` 类型），结果非 0 即 1。

---

## 6. 一元数学运算

| 操作 | 签名 | 说明 |
|------|------|------|
| `Reg::Abs` | `Abs(dst, src, mask)` | `dst = abs(src)` |
| `Reg::Neg` | `Neg(dst, src, mask)` | `dst = -src` |
| `Reg::Exp` | `Exp(dst, src, mask)` | `dst = exp(src)` |
| `Reg::Log` | `Log(dst, src, mask)` | `dst = log(src)` |
| `Reg::Sqrt` | `Sqrt(dst, src, mask)` | `dst = sqrt(src)` |
| `Reg::Rsqrt` | `Rsqrt(dst, src, mask)` | `dst = 1/sqrt(src)` |

### Truncate — 精度截断

```cpp
Reg::Truncate<T, RoundMode>(dst, src, mask);

// 带 MaskMergeMode 的版本
Reg::Truncate<T, RoundMode, MaskMergeMode::ZEROING>(dst, src, mask);
```

RoundMode 取值：

| RoundMode | C++ 等效 | 说明 |
|-----------|---------|------|
| `CAST_RINT` | `std::rint(x)` | 向偶数舍入 |
| `CAST_CEIL` | `std::ceil(x)` | 向上取整 |
| `CAST_FLOOR` | `std::floor(x)` | 向下取整 |
| `CAST_TRUNC` | `std::trunc(x)` | 向零取整 |
| `CAST_ROUND` | `std::round(x)` | 四舍五入 |
| `UNKNOWN` | — | 不指定舍入模式（用于 Cast） |

---

## 7. 比较运算

### Compare — 向量比较

```cpp
Reg::Compare<T, CMPMODE>(dstMask, src0, src1, inputMask);
```

### CompareScalar — 向量-标量比较

```cpp
Reg::CompareScalar<T, CMPMODE>(dstMask, src, scalar, inputMask);
```

CMPMODE 取值：

| CMPMODE | C++ 等效 | 说明 |
|---------|---------|------|
| `EQ` | `==` | 等于 |
| `NE` | `!=` | 不等于 |
| `LT` | `<` | 小于 |
| `GT` | `>` | 大于 |
| `LE` | `<=` | 小于等于 |
| `GE` | `>=` | 大于等于 |

---

## 8. 选择运算

### Select — 条件选择

```cpp
Reg::Select(dst, trueVal, falseVal, cmpMask);
// dst[i] = cmpMask[i] ? trueVal[i] : falseVal[i]
```

常用于实现 `a > 0 ? 1 : (a < 0 ? -1 : 0)` 这样的符号提取逻辑。

---

## 9. 数据构造

### Duplicate — 标量广播

```cpp
Reg::Duplicate(reg, scalarValue);
// 将标量值广播到寄存器所有 lane
```

常用场景：创建全零、全一的常数寄存器。

---

## 10. 类型转换

### Cast — 寄存器类型转换

```cpp
Reg::Cast<DstT, SrcT, CastTrait>(dstReg, srcReg, mask);
```

CastTrait 结构：

```cpp
struct Reg::CastTrait {
    Reg::RegLayout layout;        // RegLayout::ZERO
    Reg::SatMode satMode;         // SAT / UNKNOWN
    Reg::MaskMergeMode mergeMode; // ZEROING
    RoundMode roundMode;          // 用于溢出处理的舍入模式
};
```

常用 CastTrait 示例：

```cpp
// half → float (解包后转换)
constexpr Reg::CastTrait F16_2_F32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN,
    Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};

// float → half (含饱和 + 舍入)
constexpr Reg::CastTrait F32_2_F16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::SAT,
    Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};
```

---

## 11. 寄存器类型与 Trait

### RegTensor 声明

```cpp
// 标准寄存器张量（T 为 half/float/int8_t/... 等）
Reg::RegTensor<T> reg;

// 64 位数据双寄存器槽位
Reg::RegTensor<T, Reg::RegTraitNumTwo> regB64;
```

### RegTrait 常量

| Trait | 用途 |
|-------|------|
| `Reg::RegTraitNumOne` (默认) | 标准数据类型 |
| `Reg::RegTraitNumTwo` | 64 位类型（int64_t/uint64_t）需要双倍寄存器空间 |

### 支持的数据类型

```cpp
SupportType<T, half, float>()           // 浮点类型
SupportType<T, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t>()  // 整数类型
```

---

## 12. 快捷参考卡片

### 最常见的代码模板

```cpp
// 一元运算
template <typename T>
__simd_vf__ inline void UnaryCompute(
    __ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount, uint16_t repeatTimes)
{
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T);
    Reg::RegTensor<T> srcReg, dstReg;
    Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<T>(calCount);
        Reg::LoadAlign<T>(srcReg, src + i * vlSize);
        Reg::XXX(dstReg, srcReg, mask);  // 替换 XXX 为具体运算
        Reg::StoreAlign<T>(dst + i * vlSize, dstReg, mask);
    }
}
```

### 64 位类型分支

```cpp
if constexpr (sizeof(T) == 8) {
    using RegT = Reg::RegTensor<T, Reg::RegTraitNumTwo>;
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T) * 2;
    uint16_t repeatTime = CeilDivision(count, vlSize);
    XxxCompute<T, RegT, Reg::RegTraitNumTwo>(dstUb, srcUb, count, repeatTime, vlSize);
} else {
    using RegT = Reg::RegTensor<T>;
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T);
    uint16_t repeatTime = CeilDivision(count, vlSize);
    XxxCompute<T, RegT>(dstUb, srcUb, count, repeatTime, vlSize);
}
```

### 常用辅助宏与函数

```cpp
GetVecLen()                     // 单次向量运算字节数
CeilDivision(a, b)              // 向上取整除法
CheckTensorPosition(t, name, positions)  // 检查 tensor 位置合法
CheckCalCount(count, name, tensor, tname, api) // 检查元素数有效
SupportType<T, types...>()      // 类型是否受支持
```
