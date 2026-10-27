# 案例分析：Mmad 接口跨版本适配

## 接口概述

`Mmad` 用于矩阵乘加运算（L0A × L0B → L0C）。共 4 个重载变体：

| 变体 | 参数签名 | 声明位置 | arch 守卫 | 说明 |
|------|---------|---------|-----------|------|
| V1 | `Mmad(dst, fm, filter, MmadParams)` | `intf.h:287-289` | 无 | 基础矩阵乘加 |
| V2 | `Mmad(dst, fm, filter, bias, MmadParams)` | `intf.h:291-294` | 无 | 带 bias 的矩阵乘加 |
| V3 | `Mmad(dst, fm, filter, MmadBitModeParams)` | `intf.h:297-300` | `#if __NPU_ARCH__ == 3510/3003/3103/3113/...`（适配后已扩展为多arch）(L296-306) | BitMode 参数版 |
| V4 | `Mmad(dst, fm, filter, bias, MmadBitModeParams)` | `intf.h:302-305` | `#if __NPU_ARCH__ == 3510/3003/3103/3113/...`（适配后已扩展为多arch）(L296-306) | BitMode + bias 版 |

> V3/V4 原仅在 `__NPU_ARCH__ == 3510` 时声明，适配后已扩展为多arch（3510||3003||3103||3113||...）。

## 调用链

```
intf.h:287/291（V1/V2 声明，无 arch 守卫）
  → intf_impl.h:396-401 / 403-409（V1/V2 包装，无 arch 守卫）
    → MmadImpl（base_impl.h:473-483 / 485-509，共享实现，无 arch 守卫）
      → MmadCal(c, a, b, mmadParams)                          [V1]
      → MmadCal(c, a, b, bias, mmadParams, cmatrixSource)     [V2]
        → dav_XXXX/kernel_operator_mm_impl.h（版本特定实现）

intf.h:297/302（V3/V4 声明，#if __NPU_ARCH__ == 3510||3003||3103||3113||...，适配后已扩展为多arch）
  → intf_impl.h:412-418 / 420-426（V3/V4 包装，#if __NPU_ARCH__ == 3510）
  → intf_impl.h:428-457（MmadMx 包装，#if __NPU_ARCH__ == 3510）
    → MmadImpl（base_impl.h:511-526 / 528-556，#if __NPU_ARCH__ == 3510||3003||3103||3113||...，适配后已扩展为多arch）
      → MmadCal(c, a, b, MmadBitModeParams)                    [V3]
      → MmadCal(c, a, b, bias, MmadBitModeParams)              [V4]
        → dav_3510/kernel_operator_mm_impl.h:540-610（仅 3510 定义）
```

### 共享层分发逻辑（base_impl.h）

**版本隔离机制：** `kernel_operator_mm_base_impl.h:27-49` 通过 `#elif __NPU_ARCH__` 分派 `#include`：

```cpp
#elif (__NPU_ARCH__ == 3003)
  #include "dav_l300/kernel_operator_mm_impl.h"
#elif (__NPU_ARCH__ == 3113)
  #include "dav_l311/kernel_operator_mm_impl.h"
#elif (__NPU_ARCH__ == 3103)
  #include "dav_l310/kernel_operator_mm_impl.h"
#elif (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || ...)
  #include "dav_5161/kernel_operator_mm_impl.h"
```

**MmadImpl 共享层（base_impl.h:473-556）按 dst 位置/bias 位置分发：**

| 变体 | base_impl.h 行号 | arch 守卫 | 调用的 Cal 函数 |
|------|-----------------|-----------|----------------|
| V1 | L473-483 | 无 | `MmadCal(c, a, b, mmadParams)` |
| V2 | L485-509 | 无 | `MmadCal(c, a, b, bias, mmadParams, cmatrixSource)` |
| V3 | L511-526 | `#if __NPU_ARCH__ == 3510/3003/3103/3113/...`（适配后已扩展为多arch）| `MmadCal(c, a, b, MmadBitModeParams)` |
| V4 | L528-556 | `#if __NPU_ARCH__ == 3510/3003/3103/3113/...`（适配后已扩展为多arch）| `MmadCal(c, a, b, bias, MmadBitModeParams)` |

**MmadImpl V2 中的 bias 位置处理（base_impl.h:493-505）：**
```cpp
const Hardware biasScope = GetPhyType((TPosition)bias.GetPosition());
bool cmatrixSource = false;
if (biasScope == Hardware::BIAS) {
    cmatrixSource = true;
} else if (biasScope == Hardware::L0C) {
    cmatrixSource = false;
} else {
    ASCENDC_ASSERT((false), { ... });
}
```

> `cmatrixSource` 由 bias 张量的硬件位置决定，然后传入 MmadCal。

## 支持矩阵

| 版本 | Arch | V1 (MmadParams) | V2 (MmadParams+bias) | V3 (BitModeParams) | V4 (BitModeParams+bias) |
|------|------|-----------------|----------------------|--------------------|-----------------------|
| dav_l300 | 3003 | **完整** (L451-463) | **完整** (L472-488) | N/A (仅 3510 声明) | N/A |
| dav_l310 | 3103 | **完整** (L426-440) | **完整** (L449-467) | N/A | N/A |
| dav_l311 | 3113 | **完整** (L444-458) | **完整** (L467-485) | N/A | N/A |
| dav_5161 | 5101 | **桩** (L346-350) | **桩** (L352-357) | N/A | N/A |
| dav_3510 | 3510 | **完整** (L476-498) | **完整** (L500-538) | **完整** (L540-572) | **完整** (L574-610) |

> V3/V4 原仅在 `#if __NPU_ARCH__ == 3510` 下声明（intf.h:296），适配后已扩展为多arch（3510||3003||3103||3113||...）。
>
> **dav_5161 (5101) 桩函数说明：** 5101 架构使用宏指令，不支持 `mad` 硬件指令，因此 V1/V2 为桩函数（硬件不支持，非待补全）。V3/V4 亦不支持。
>
> **dav_5161 (5101) 桩函数说明：** 5101 架构使用宏指令，不支持 `mad` 硬件指令，因此 V1/V2 为桩函数（硬件不支持，非待补全）。V3/V4 亦不支持。

### float 特化处理

| 版本 | V1 float 特化 | V2 float 特化 | 说明 |
|------|-------------|-------------|------|
| dav_l300 | **桩** (L445-449) | **桩** (L465-470) | `__cc__ float*` 特化为桩，非 float 走通用模板 |
| dav_l310 | **桩** (L420-424) | **桩** (L442-447) | 同上 |
| dav_l311 | **桩** (L438-442) | **桩** (L460-465) | 同上 |
| dav_5161 | 无特化 | 无特化 | V1/V2 均为桩，无需特化 |
| dav_3510 | 无特化 | 无特化 | 通过 `SupportType` + `isMx` 分派，float 支持由 `mad()` 内部处理 |

## 各版本原始代码

### dav_3510（参照版本）

> 所有 MmadCal 重载均在 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)` 守卫内（L475-611）。

#### V1 — 完整实现（L476-498）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    if ASCEND_IS_AIC {
        bool cmatrixInitVal = mmadParams.cmatrixInitVal && (!mmadParams.isBias);
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, mx_fp8_e4m3_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e4m3_t, mx_fp8_e5m2_t>, Tuple<float, mx_fp8_e5m2_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e5m2_t, mx_fp8_e5m2_t>>();
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx(
                c, (__ca__ AType*)a, (__cb__ BType*)b, mmadParams.m, mmadParams.k, mmadParams.n, mmadParams.unitFlag,
                mmadParams.disableGemv, mmadParams.cmatrixSource, cmatrixInitVal);
        } else {
            mad(c, a, b, mmadParams.m, mmadParams.k, mmadParams.n, mmadParams.unitFlag, mmadParams.disableGemv,
                mmadParams.cmatrixSource, cmatrixInitVal);
        }
    }
}
```

#### V2 — 完整实现（L500-538）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    if ASCEND_IS_AIC {
        bool cmatrixInitVal = mmadParams.cmatrixInitVal && (!mmadParams.isBias);
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, mx_fp8_e4m3_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e4m3_t, mx_fp8_e5m2_t>, Tuple<float, mx_fp8_e5m2_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e5m2_t, mx_fp8_e5m2_t>>();
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx(
                (__cc__ DstT*)c, (__ca__ AType*)a, (__cb__ BType*)b, bias, mmadParams.m, mmadParams.k, mmadParams.n,
                mmadParams.unitFlag, mmadParams.disableGemv, cmatrixSource, cmatrixInitVal);
        } else {
            mad(c, a, b, bias, mmadParams.m, mmadParams.k, mmadParams.n, mmadParams.unitFlag, mmadParams.disableGemv,
                cmatrixSource, cmatrixInitVal);
        }
#else
        // Xd[31:0]: matrix c addr in L0C, Xd[63:32]: bias addr in bias table buffer;
        uint64_t xd = ((uint64_t)c) & 0xffffffffULL | ((bias & 0xffffffffULL) << 32);
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx(
                (__cc__ DstT*)xd, (__ca__ AType*)a, (__cb__ BType*)b, mmadParams.m, mmadParams.k, mmadParams.n,
                mmadParams.unitFlag, mmadParams.disableGemv, cmatrixSource, cmatrixInitVal);
        } else {
            mad((__cc__ DstT*)xd, a, b, mmadParams.m, mmadParams.k, mmadParams.n, mmadParams.unitFlag,
                mmadParams.disableGemv, cmatrixSource, cmatrixInitVal);
        }
#endif
    }
}
```

#### V3 — 完整实现（L540-572）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, mx_fp8_e4m3_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e4m3_t, mx_fp8_e5m2_t>, Tuple<float, mx_fp8_e5m2_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e5m2_t, mx_fp8_e5m2_t>>();
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx(
                c, (__ca__ AType*)a, (__cb__ BType*)b, mmadParams.GetM(), mmadParams.GetK(), mmadParams.GetN(),
                mmadParams.GetUnitFlag(), mmadParams.GetDisableGemv(), mmadParams.GetCmatrixSource(),
                mmadParams.GetCmatrixInitVal());
        } else {
            mad(c, a, b, mmadParams.GetM(), mmadParams.GetK(), mmadParams.GetN(), mmadParams.GetUnitFlag(),
                mmadParams.GetDisableGemv(), mmadParams.GetCmatrixSource(), mmadParams.GetCmatrixInitVal());
        }
#else
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx(c, (__ca__ AType*)a, (__cb__ BType*)b, mmadParams.GetConfig0());
        } else {
            mad(c, a, b, mmadParams.GetConfig0());
        }
#endif
    }
}
```

#### V4 — 完整实现（L574-610）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, MmadBitModeParams& mmadParams)
{
    if ASCEND_IS_AIC {
        mmadParams.SetCmatrixInitVal(false);
        constexpr bool isMx = SupportType<
            Tuple<DstT, Src0T, Src1T>, Tuple<float, fp4x2_e1m2_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e1m2_t, fp4x2_e2m1_t>, Tuple<float, fp4x2_e2m1_t, fp4x2_e1m2_t>,
            Tuple<float, fp4x2_e2m1_t, fp4x2_e2m1_t>, Tuple<float, mx_fp8_e4m3_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e4m3_t, mx_fp8_e5m2_t>, Tuple<float, mx_fp8_e5m2_t, mx_fp8_e4m3_t>,
            Tuple<float, mx_fp8_e5m2_t, mx_fp8_e5m2_t>>();
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx(
                (__cc__ DstT*)c, (__ca__ AType*)a, (__cb__ BType*)b, bias, mmadParams.GetM(), mmadParams.GetK(),
                mmadParams.GetN(), mmadParams.GetUnitFlag(), mmadParams.GetDisableGemv(), mmadParams.GetCmatrixSource(),
                mmadParams.GetCmatrixInitVal());
        } else {
            mad(c, a, b, bias, mmadParams.GetM(), mmadParams.GetK(), mmadParams.GetN(), mmadParams.GetUnitFlag(),
                mmadParams.GetDisableGemv(), mmadParams.GetCmatrixSource(), mmadParams.GetCmatrixInitVal());
        }
#else
        // Xd[31:0]: matrix c addr in L0C, Xd[63:32]: bias addr in bias table buffer;
        uint64_t xd = ((uint64_t)c) & 0xffffffffULL | ((bias & 0xffffffffULL) << 32);
        if constexpr (isMx) {
            using AType = typename GetDstType<Src0T>::Type;
            using BType = typename GetDstType<Src1T>::Type;
            mad_mx((__cc__ DstT*)xd, (__ca__ AType*)a, (__cb__ BType*)b, mmadParams.GetConfig0());
        } else {
            mad((__cc__ DstT*)xd, a, b, mmadParams.GetConfig0());
        }
#endif
    }
}
```

---

### dav_l300（目标版本，3003）

#### V1 float 特化 — 桩函数（L445-449）

```cpp
template <typename U, typename S>
__aicore__ inline void MmadCal(__cc__ float* c, __ca__ U* a, __cb__ S* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported float mmad"); });
}
```

#### V1 通用 — 完整实现（L451-463）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    uint64_t config = 0;
    config |= ((static_cast<uint64_t>(mmadParams.m) & 0xfff) << 0);
    config |= ((static_cast<uint64_t>(mmadParams.k) & 0xfff) << 12);
    config |= ((static_cast<uint64_t>(mmadParams.n) & 0xfff) << 24);
    config |= ((static_cast<uint64_t>(mmadParams.fmOffset) & 0xff) << 36);
    config |= ((static_cast<uint64_t>(mmadParams.unitFlag) & 0x3) << 55);
    config |= ((static_cast<uint64_t>(mmadParams.cmatrixSource) & 0x1) << 62);
    config |= ((static_cast<uint64_t>(mmadParams.cmatrixInitVal) & 0x1) << 63);
    mad(c, a, b, config);
}
```

#### V2 float 特化 — 桩函数（L465-470）

```cpp
template <typename U, typename S>
__aicore__ inline void MmadCal(
    __cc__ float* c, __ca__ U* a, __cb__ S* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported float mmad"); });
}
```

#### V2 通用 — 完整实现（L472-488）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((cmatrixSource == mmadParams.cmatrixSource), {
        KERNEL_LOG(KERNEL_ERROR, "Mmad cmatrixSource param config error");
    });
    uint64_t config = 0;
    config |= ((static_cast<uint64_t>(mmadParams.m) & 0xfff) << 0);
    config |= ((static_cast<uint64_t>(mmadParams.k) & 0xfff) << 12);
    config |= ((static_cast<uint64_t>(mmadParams.n) & 0xfff) << 24);
    config |= ((static_cast<uint64_t>(mmadParams.fmOffset) & 0xff) << 36);
    config |= ((static_cast<uint64_t>(mmadParams.unitFlag) & 0x3) << 55);
    config |= ((static_cast<uint64_t>(mmadParams.cmatrixSource) & 0x1) << 62);
    config |= ((static_cast<uint64_t>(mmadParams.cmatrixInitVal) & 0x1) << 63);
    mad(c, a, b, bias, config);
}
```

---

### dav_l310（目标版本，3103）

#### V1 float 特化 — 桩函数（L420-424）

```cpp
template <typename U, typename S>
__aicore__ inline void MmadCal(__cc__ float* c, __ca__ U* a, __cb__ S* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported float mmad"); });
}
```

#### V1 通用 — 完整实现（L426-440）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void MmadCal(__cc__ T* c, __ca__ U* a, __cb__ S* b, const MmadParams& mmadParams)
{
    uint64_t config = 0;
    uint64_t gemvCtrl = 0;
    config |= (((uint64_t)mmadParams.m & 0xfff) << 0);
    config |= (((uint64_t)mmadParams.k & 0xfff) << 12);
    config |= (((uint64_t)mmadParams.n & 0xfff) << 24);
    config |= (((uint64_t)mmadParams.fmOffset & 0xff) << 36);
    config |= (((uint64_t)mmadParams.unitFlag & 0x3) << 55);
    config |= (((uint64_t)gemvCtrl & 0x1) << 61);
    config |= (((uint64_t)mmadParams.cmatrixSource & 0x1) << 62);
    config |= (((uint64_t)mmadParams.cmatrixInitVal & 0x1) << 63);
    mad(c, a, b, config);
}
```

#### V2 float 特化 — 桩函数（L442-447）

```cpp
template <typename U, typename S>
__aicore__ inline void MmadCal(
    __cc__ float* c, __ca__ U* a, __cb__ S* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported float mmad"); });
}
```

#### V2 通用 — 完整实现（L449-467）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void MmadCal(
    __cc__ T* c, __ca__ U* a, __cb__ S* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((cmatrixSource == mmadParams.cmatrixSource), {
        KERNEL_LOG(KERNEL_ERROR, "Mmad cmatrixSource param config error");
    });
    uint64_t config = 0;
    uint64_t gemvCtrl = 0;
    config |= (((uint64_t)mmadParams.m & 0xfff) << 0);
    config |= (((uint64_t)mmadParams.k & 0xfff) << 12);
    config |= (((uint64_t)mmadParams.n & 0xfff) << 24);
    config |= (((uint64_t)mmadParams.fmOffset & 0xff) << 36);
    config |= (((uint64_t)mmadParams.unitFlag & 0x3) << 55);
    config |= (((uint64_t)gemvCtrl & 0x1) << 61);
    config |= (((uint64_t)mmadParams.cmatrixSource & 0x1) << 62);
    config |= (((uint64_t)mmadParams.cmatrixInitVal & 0x1) << 63);
    mad(c, a, b, bias, config);
}
```

---

### dav_l311（目标版本，3113）

#### V1 float 特化 — 桩函数（L438-442）

```cpp
template <typename U, typename S>
__aicore__ inline void MmadCal(__cc__ float* c, __ca__ U* a, __cb__ S* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported float mmad"); });
}
```

#### V1 通用 — 完整实现（L444-458）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void MmadCal(__cc__ T* c, __ca__ U* a, __cb__ S* b, const MmadParams& mmadParams)
{
    uint64_t config = 0;
    uint64_t gemvCtrl = 0;
    config |= (((uint64_t)mmadParams.m & 0xfff) << 0);
    config |= (((uint64_t)mmadParams.k & 0xfff) << 12);
    config |= (((uint64_t)mmadParams.n & 0xfff) << 24);
    config |= (((uint64_t)mmadParams.fmOffset & 0xff) << 36);
    config |= (((uint64_t)mmadParams.unitFlag & 0x3) << 55);
    config |= (((uint64_t)gemvCtrl & 0x1) << 61);
    config |= (((uint64_t)mmadParams.cmatrixSource & 0x1) << 62);
    config |= (((uint64_t)mmadParams.cmatrixInitVal & 0x1) << 63);
    mad(c, a, b, config);
}
```

#### V2 float 特化 — 桩函数（L460-465）

```cpp
template <typename U, typename S>
__aicore__ inline void MmadCal(
    __cc__ float* c, __ca__ U* a, __cb__ S* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported float mmad"); });
}
```

#### V2 通用 — 完整实现（L467-485）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void MmadCal(
    __cc__ T* c, __ca__ U* a, __cb__ S* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT((cmatrixSource == mmadParams.cmatrixSource), {
        KERNEL_LOG(KERNEL_ERROR, "Mmad cmatrixSource param config error");
    });
    uint64_t config = 0;
    uint64_t gemvCtrl = 0;
    config |= (((uint64_t)mmadParams.m & 0xfff) << 0);
    config |= (((uint64_t)mmadParams.k & 0xfff) << 12);
    config |= (((uint64_t)mmadParams.n & 0xfff) << 24);
    config |= (((uint64_t)mmadParams.fmOffset & 0xff) << 36);
    config |= (((uint64_t)mmadParams.unitFlag & 0x3) << 55);
    config |= (((uint64_t)gemvCtrl & 0x1) << 61);
    config |= (((uint64_t)mmadParams.cmatrixSource & 0x1) << 62);
    config |= (((uint64_t)mmadParams.cmatrixInitVal & 0x1) << 63);
    mad(c, a, b, bias, config);
}
```

> **dav_l310 与 dav_l311 的 MmadCal 函数定义完全相同**（仅行号不同）。

---

### dav_5161（目标版本，5101）

#### V1 — 桩函数（L346-350）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

#### V2 — 桩函数（L352-357）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(
    __cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, uint64_t bias, const MmadParams& mmadParams, bool cmatrixSource)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

> dav_5161 无 float 特化（V1/V2 均为桩），无 V3/V4（BitModeParams 仅 3510 声明）。

---

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| AIC 守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无（桩） |
| 参数传递方式 | 参数逐个传 `mad()`/`mad_mx()` | 手动打包 config 寄存器 | 手动打包 config 寄存器 | 手动打包 config 寄存器 | 无（桩） |
| 硬件指令 | `mad` / `mad_mx` | `mad` | `mad` | `mad` | 无（桩） |
| isMx 分派 | `SupportType<Tuple<...>>` 编译期分派 | 无 | 无 | 无 | 无 |
| 支持 MX 类型 | fp4x2_e1m2_t, fp4x2_e2m1_t, mx_fp8_e4m3_t, mx_fp8_e5m2_t | 无 | 无 | 无 | 无 |
| gemvCtrl (bit 61) | 通过 `mmadParams.disableGemv` 传入 | **未设置** (bit 61 留空) | 硬编码 `gemvCtrl=0` (bit 61=0) | 硬编码 `gemvCtrl=0` (bit 61=0) | 无（桩） |
| cmatrixInitVal 处理 | `mmadParams.cmatrixInitVal && (!mmadParams.isBias)` | 直接用 `mmadParams.cmatrixInitVal` | 直接用 `mmadParams.cmatrixInitVal` | 直接用 `mmadParams.cmatrixInitVal` | 无（桩） |
| V2 cmatrixSource 断言 | 无 | 有 (`cmatrixSource == mmadParams.cmatrixSource`) | 有 | 有 | 无（桩） |
| V2 bias 传递 | 打包进 xd 寄存器 (`c\|bias<<32`) | 作为独立参数传 `mad(c,a,b,bias,config)` | 作为独立参数传 `mad(c,a,b,bias,config)` | 作为独立参数传 `mad(c,a,b,bias,config)` | 无（桩） |
| CPU debug 分支 | 有 `#if ASCENDC_CPU_DEBUG` | 无 | 无 | 无 | 无 |
| float dst 特化 | 无（SupportType 统一处理） | 桩 (`__cc__ float*`) | 桩 (`__cc__ float*`) | 桩 (`__cc__ float*`) | 无（全桩） |
| 类型转换风格 | `static_cast` | `static_cast<uint64_t>()` | C 风格 `(uint64_t)` | C 风格 `(uint64_t)` | — |
| 模板参数命名 | `<DstT, Src0T, Src1T>` | `<DstT, Src0T, Src1T>` (通用) / `<U, S>` (float) | `<T, U, S>` (通用) / `<U, S>` (float) | `<T, U, S>` (通用) / `<U, S>` (float) | `<DstT, Src0T, Src1T>` |

### config 寄存器位域对比

| 位域 | dav_l300 | dav_l310 / dav_l311 | dav_3510 | 说明 |
|------|----------|---------------------|----------|------|
| [0:11] | m | m | — (参数直传) | 左矩阵行数 |
| [12:23] | k | k | — | 左矩阵列数 |
| [24:35] | n | n | — | 右矩阵列数 |
| [36:43] | fmOffset | fmOffset | — | fm 偏移 |
| [55:56] | unitFlag | unitFlag | — | unit flag |
| [61] | **未设置** | gemvCtrl=0 | — (disableGemv 直传) | GeMv 控制位 |
| [62] | cmatrixSource | cmatrixSource | — | C 矩阵来源 |
| [63] | cmatrixInitVal | cmatrixInitVal | — | C 矩阵初始值 |

> **关键差异：** dav_l300 完全不设置 bit 61（gemvCtrl），dav_l310/l311 硬编码 bit 61=0，dav_3510 通过参数直传 `disableGemv`。

### 两种实现模式

**模式1：dav_3510 — 参数直传模式**
- 所有 mmadParams 字段逐个传给 `mad()` / `mad_mx()` 硬件指令
- 通过 `SupportType<Tuple<DstT, Src0T, Src1T>, ...>` 编译期判断是否为 MX 类型
- 支持 fp4x2 和 mx_fp8 类型，调用 `mad_mx()` 指令
- 有 `if ASCEND_IS_AIC` 守卫，仅在 AIC 核执行
- V2 bias 路径：将 c 地址和 bias 地址打包进 64 位 xd 寄存器
- 有 CPU debug / 硬件双路径
- cmatrixInitVal 受 isBias 修饰：`mmadParams.cmatrixInitVal && (!mmadParams.isBias)`

**模式2：dav_l300/l310/l311 — config 寄存器打包模式**
- 手动构建 64 位 config 寄存器，通过位域拼接 m/k/n/fmOffset/unitFlag/cmatrixSource/cmatrixInitVal
- 调用 `mad(c, a, b, config)` 或 `mad(c, a, b, bias, config)`
- 无 AIC 守卫
- 无 MX 类型支持
- float dst 类型通过模板特化为桩函数（不支持 float 矩阵乘加）
- V2 有 `cmatrixSource == mmadParams.cmatrixSource` 一致性断言
- cmatrixInitVal 直接使用，不受 isBias 修饰

### 关键差异点

1. **dav_5161 的 V1/V2 全为桩函数** — **硬件不支持。** 5101 架构使用宏指令，不支持 `mad` 指令，V3/V4 亦不支持
2. **dav_l300 不设置 gemvCtrl (bit 61)** — 与 l310/l311 不同，l300 的 config 完全不设 bit 61，可能导致 GeMv 行为差异
3. **dav_l300 使用 `static_cast`，l310/l311 使用 C 风格转换** — 功能等价但风格不统一
4. **dav_3510 独有的 isMx 分派** — 通过 `SupportType<Tuple<...>>` 支持 fp4/fp8 MX 类型，调用 `mad_mx()`
5. **dav_3510 的 cmatrixInitVal 受 isBias 修饰** — `cmatrixInitVal && (!isBias)`，其余版本直接使用原始值
6. **dav_3510 V2 无 cmatrixSource 一致性断言** — l300/l310/l311 断言 `cmatrixSource == mmadParams.cmatrixSource`，3510 无此断言
7. **dav_3510 V2 将 c+bias 打包进 xd 寄存器** — `xd = c[31:0] | bias[31:0]<<32`，而 l300/l310/l311 将 bias 作为独立参数传递
8. **dav_l300/l310/l311 的 float dst 特化为桩** — 不支持 float 类型作为 Mmad 的 dst，3510 通过 SupportType 统一处理
9. **dav_l310 与 dav_l311 代码完全相同** — 4 个 MmadCal 函数体逐字符一致
10. **dav_3510 有 CPU debug / 硬件双路径** — V2/V3/V4 使用 `#if ASCENDC_CPU_DEBUG` 分支，其余版本无此分支

## 待确认问题

1. **~~dav_5161 的 V1/V2 桩函数~~** — **已确认：硬件不支持。** 5101 架构使用宏指令，不支持 `mad` 硬件指令，V1/V2 桩函数是硬件限制，非待补全。V3/V4 亦不支持。
2. **dav_l300 缺少 gemvCtrl (bit 61)** — l300 完全不设置 bit 61，而 l310/l311 硬编码为 0。这是硬件差异（l300 无 GeMv 控制位）还是遗漏？是否需要从 l310 补上 `gemvCtrl` 设置？
3. **dav_l300/l310/l311 的 float dst 桩函数** — 硬件是否支持 float 类型的矩阵乘加？如不支持，是否应在 intf.h 层面通过 `enable_if` 约束？还是硬件支持但软件未实现？
4. **dav_3510 的 cmatrixInitVal 修饰逻辑** — `mmadParams.cmatrixInitVal && (!mmadParams.isBias)` 在 l300/l310/l311 中不存在。这是否意味着 l300/l310/l311 在带 bias 时仍使用原始 cmatrixInitVal 值，可能导致结果错误？
5. **dav_3510 V2 无 cmatrixSource 一致性断言** — l300/l310/l311 断言 `cmatrixSource == mmadParams.cmatrixSource`，3510 无此断言。是否需要统一加上？
6. **dav_3510 V2 的 xd 寄存器打包** — 将 c 和 bias 地址打包进 64 位 xd，而 l300/l310/l311 分别传递。这是硬件指令接口差异（3510 的 `mad()` 接受合并地址，l300 的 `mad()` 接受独立 bias 参数）还是实现选择？
7. **~~dav_5161 是否需要 V3/V4 (BitModeParams)~~** — **已确认：不支持。** 5101 使用宏指令，V3/V4 亦不支持。
8. **模板参数命名不统一** — l300 用 `<DstT, Src0T, Src1T>`，l310/l311 用 `<T, U, S>`。是否需要统一命名以提升可维护性？
9. **dav_l300 的 `static_cast` vs l310/l311 的 C 风格转换** — 功能等价，但是否应统一为 `static_cast` 以符合 C++ 最佳实践？
10. **disableGemv 字段的使用差异** — l300 不使用 `mmadParams.disableGemv`（不设 bit 61），l310/l311 硬编码 bit 61=0（也不使用 `mmadParams.disableGemv`），仅 3510 使用该字段。`MmadParams::disableGemv` 在 l300/l310/l311 上是否实际无效？
