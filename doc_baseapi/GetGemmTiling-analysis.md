# 案例分析：GetGemmTiling 接口跨版本适配

## 接口概述

`GetGemmTiling` 用于根据 m/k/n 矩阵维度和 dtype 计算配套 `Gemm` 接口所需的 `GemmTiling` 切分参数。仅有 1 个变体：

| 变体 | 参数签名 | 声明位置 | arch 守卫 | 说明 |
|------|---------|---------|-----------|------|
| 唯一变体 | `template <typename T> GemmTiling GetGemmTiling(uint32_t m, uint32_t k, uint32_t n)` | `intf.h:39-40` | 无 | T 为左矩阵 dtype，仅用于推导 c0Size/dtypeSize |

**关键属性：**
- 声明（`intf.h:39-40`）**无 arch 守卫**；定义处（`intf_impl.h:31-32`）带 `[[deprecated]]` 标记（"GetGemmTiling has been deprecated..."），声明处无此标记
- 返回值 `GemmTiling` 结构体定义于 `kernel_struct_conv2d.h:134-170`（无 arch 守卫，全版本同一份；`blockSize` 恒为 16，`LoopMode` 枚举 L37）
- `__aicore__ inline` 纯计算函数，无 pipe 标注（非 `__inout_pipe__`）
- 参数顺序为 `(m, k, n)`，与配套 `Gemm(dst, src0, src1, m, k, n, tiling)` 一致

类型推导逻辑（`intf_impl.h:37-43`）：
- `T` 为 `uint8_t/int8_t` → `c0Size=32, dtypeSize=1`
- 其他（half/bfloat16/float/int32 等）→ `c0Size=16, dtypeSize=2`

## 调用链

```
kernel_operator.h:29
→ include/basic_api/kernel_operator_intf.h:38（无条件 include gemm_intf.h，所有版本可见）
→ include/basic_api/kernel_operator_gemm_intf.h:39-40（声明，无 arch 守卫）
→ [intf.h:74-76: #if defined(__NPU_ARCH__) 才 include intf_impl.h —— 唯一一层守卫]
→ impl/basic_api/kernel_operator_gemm_intf_impl.h:30-71
   GetGemmTiling 定义（无 arch 守卫，带 [[deprecated]]）
   ├─ Std::is_same<T, ...>            （类型判断，运行时 if 风格）
   ├─ DivCeil(a, b)                   impl/basic_api/utils/kernel_utils_ceil_oom_que.h:25（无 arch 守卫，constexpr 纯算术）
   ├─ TOTAL_L0A_SIZE / TOTAL_L0B_SIZE impl/basic_api/utils/kernel_utils_constants.h:56-57（无 arch 守卫，全版本 64KB）
   └─ CalculateGemmTiling(tiling)     impl/basic_api/kernel_operator_gemm_base_impl.h:141-164（无 arch 守卫，纯赋值计算）
      ★ 调用链止步于共享层，无 dav_XXXX 版本分发 ★
```

### 共享层分发逻辑

**不存在版本分发。** 与 `LoadDataWithTranspose`/`Mmad` 的"intf.h → intf_impl.h → base_impl.h → dav_XXXX/xxx_impl.h"四层模式不同：

1. 各 dav 目录（l300/l310/l311/5161/3510）中**均无** `kernel_operator_gemm_*.h` 文件（见 `各版本接口一览.md` §2 第 10 项：GEMM 仅有顶层公共文件）
2. `GetGemmTiling` 与 `CalculateGemmTiling` 均为纯软件 tiling 计算，不调用任何 Cal 层函数、不调用任何编译器内建/硬件指令
3. `kernel_operator_gemm_base_impl.h:27` 虽 include `kernel_operator_mm_base_impl.h`（后者 L27-49 按 `__NPU_ARCH__` 分派 include `dav_XXXX/kernel_operator_mm_impl.h`），但该包含链只为同文件中的 `Gemm` 执行函数（LoadL0A/LoadL0B/MmadFunc）服务，`CalculateGemmTiling` 不经过它

**版本差异的唯一潜在来源**是宏常量，而 `GetGemmTiling` 引用的两个常量恰好全版本统一：

| 常量 | 定义位置 | 守卫 | 值 |
|------|---------|------|-----|
| `TOTAL_L0A_SIZE` | kernel_utils_constants.h:56 | 无 | 64KB（全版本） |
| `TOTAL_L0B_SIZE` | kernel_utils_constants.h:57 | 无 | 64KB（全版本） |

对比：`TOTAL_L0C_SIZE`/`TOTAL_L1_SIZE` 按版本分支（kernel_utils_constants.h:89-225），且 **3103 无分支**（详见差异分析）。

## 支持矩阵

### 主矩阵：GetGemmTiling 本体

| 版本 | Arch | 声明可见 | 定义可用（NPU 编译） | 实现路径 | 实现状态 |
|------|------|---------|---------------------|---------|---------|
| dav_l300 | 3003 | ✅ | ✅ | 共享层同一份代码 | **完整**（纯计算） |
| dav_l310 | 3103 | ✅ | ✅ | 共享层同一份代码 | **完整**（纯计算） |
| dav_l311 | 3113 | ✅ | ✅ | 共享层同一份代码 | **完整**（纯计算） |
| dav_5161 | 5101/5161/5165/5163 | ✅ | ✅ | 共享层同一份代码 | **完整**（纯计算） |
| dav_3510（参照） | 3510 | ✅ | ✅ | 共享层同一份代码 | **完整**（纯计算） |

> CPU debug：`ASCENDC_CPU_DEBUG=1` 时 `__NPU_ARCH__` 仍定义目标架构值，`intf_impl.h` 照常包含，走同一实现（无独立桩；`intf_impl.h:106` Gemm 内嵌 `#if ASCENDC_CPU_DEBUG` 分支可佐证）。

### 扩展矩阵：GetGemmTiling 返回值的下游可用性（配套 Gemm 接口）

`GetGemmTiling` 的产物仅供 `Gemm` 使用，其版本可用性由 Gemm 的执行链决定（`intf_impl.h:102-151` → `GemmExecNm/Mn` → `LoadL0A/LoadL0B`（V1 `LoadData2DParams`）→ `MmadFunc` → `MmadImpl`）：

| 版本 | Arch | LoadData2D V1 L1→L0A | LoadData2D V1 L1→L0B | Mmad V1 (MmadParams) | Gemm 整体可用性 |
|------|------|----------------------|----------------------|----------------------|----------------|
| dav_l300 | 3003 | ✅ 完整（mm_impl.h L62-80） | ✅ 完整（L112-130） | ✅ 完整 | ✅ **可用** |
| dav_l310 | 3103 | ❌ 桩（L285-289） | ❌ 桩（L291-295） | ✅ 完整 | ❌ 运行时断言（LoadData2D 2dv1 not supported） |
| dav_l311 | 3113 | ❌ 桩（L310-314） | ❌ 桩（L316-320） | ✅ 完整 | ❌ 运行时断言 |
| dav_5161 | 5101 | ❌ 桩（L115-119） | ❌ 桩（L121-125） | ❌ 桩（硬件无 mad 指令） | ❌ 完全不可用 |
| dav_3510 | 3510 | ✅ 完整（L67-87） | ✅ 完整（L89-115） | ✅ 完整 | ✅ **可用** |

> - l310/l311 的 V2 `LoadData2DL12L0A/L0BCal`（`LoadData2DParamsV2`）和 BitMode 版本均为**完整实现**，唯独 V1（`LoadData2DParams`）为桩 —— 而 Gemm 的 LoadL0A/LoadL0B 恰好只走 V1（`gemm_base_impl.h:174-178/187-190` 构造 `LoadData2DParams` 后调 `LoadDataImpl`，经 `load2d_impl.h:41-56` 分发到 V1 Cal）
> - 5161 (5101) 为 vector 类架构，`MmadCal` 桩 + `TOTAL_L0C_SIZE = 0`（kernel_utils_constants.h:196），Gemm 不可用系硬件限制

## 各版本原始代码

> **GetGemmTiling/CalculateGemmTiling 在所有版本编译时展开为同一份共享层代码**，故本节先给出共享层完整代码，再给出各版本差异上下文（版本常量分支 + 下游 V1 LoadData2D/Mmad 实现）。

### 共享层（全版本同一份，实际生效代码）

#### 声明 — intf.h:39-40（无 arch 守卫）

```cpp
namespace AscendC {
// T should be left matrix dtype
template <typename T>
__aicore__ inline GemmTiling GetGemmTiling(uint32_t m, uint32_t k, uint32_t n);
```

#### 守卫 — intf.h:74-76（唯一一层 arch 守卫）

```cpp
#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_gemm_intf_impl.h"
#endif
```

#### 定义 — intf_impl.h:30-71（无 arch 守卫，带 [[deprecated]]）

```cpp
// T should be left matrix dtype
template <typename T>
[[deprecated("NOTICE: GetGemmTiling has been deprecated and will be removed in the next version. "
             "Please do not use it!")]]
__aicore__ inline GemmTiling GetGemmTiling(uint32_t m, uint32_t k, uint32_t n)
{
    uint32_t c0 = 0;
    uint32_t dSize = 1;
    if (Std::is_same<T, uint8_t>::value || Std::is_same<T, int8_t>::value) {
        c0 = 32;
        dSize = 1;
    } else {
        c0 = 16;
        dSize = 2;
    }
    GemmTiling tiling;
    tiling.c0Size = c0;
    tiling.dtypeSize = dSize;
    tiling.mNum = m;
    tiling.nNum = n;
    tiling.kNum = k;
    tiling.roundM = DivCeil(m, tiling.blockSize) * tiling.blockSize; // blockSize = 16 * 16
    tiling.roundN = DivCeil(n, tiling.blockSize) * tiling.blockSize;
    tiling.roundK = DivCeil(k, tiling.c0Size) * tiling.c0Size; // c0Size = 16 || c0Size = 32
    uint32_t k0a = TOTAL_L0A_SIZE / 2 / (tiling.roundM * dSize);
    uint32_t k0b = TOTAL_L0B_SIZE / 2 / (tiling.roundN * dSize);
    uint32_t k0 = k0a > k0b ? k0b : k0a;
    k0 = k0 > k ? k : k0;

    tiling.kTileBlock = k0 / tiling.c0Size;
    if (tiling.kTileBlock == 0) {
        tiling.kTileBlock = 1;
    }
    tiling.loopMode = LoopMode::MODE_NM;

    tiling.mBlockNum = DivCeil(m, tiling.blockSize);
    tiling.nBlockNum = DivCeil(n, tiling.blockSize);
    tiling.kBlockNum = DivCeil(k, tiling.c0Size);

    CalculateGemmTiling(tiling);

    return tiling;
}
```

#### 依赖 1 — CalculateGemmTiling（gemm_base_impl.h:141-164，无 arch 守卫）

```cpp
__aicore__ inline void CalculateGemmTiling(GemmTiling& tiling)
{
    tiling.mIterNum = 1;
    tiling.nIterNum = 1;
    tiling.kIterNum = DivCeil(tiling.kBlockNum, tiling.kTileBlock);

    tiling.mTileBlock = DivCeil(tiling.mBlockNum, tiling.mIterNum);
    tiling.nTileBlock = DivCeil(tiling.nBlockNum, tiling.nIterNum);

    tiling.kTailBlock = tiling.kBlockNum - (tiling.kIterNum - 1) * tiling.kTileBlock;
    tiling.mTailBlock = tiling.mBlockNum - (tiling.mIterNum - 1) * tiling.mTileBlock; // mTailBlock <= mBlockNum
    tiling.nTailBlock = tiling.nBlockNum - (tiling.nIterNum - 1) * tiling.nTileBlock;

    tiling.kHasTail = tiling.kTailBlock != tiling.kTileBlock;
    tiling.kHasTailEle = tiling.roundK != tiling.kNum;
    tiling.kTailEle = tiling.kNum % (tiling.kTileBlock * tiling.c0Size);

    if (tiling.mNum != tiling.mTileBlock * tiling.blockSize) {
        tiling.mHasTail = true;
    } else {
        tiling.mHasTail = false;
    }
    tiling.nHasTail = tiling.nTileBlock != tiling.nTailBlock;
}
```

#### 依赖 2 — DivCeil（kernel_utils_ceil_oom_que.h:25，无 arch 守卫）

```cpp
__aicore__ constexpr inline uint32_t DivCeil(uint32_t a, uint32_t b) { return (a + b - 1) / b; }
```

#### 依赖 3 — 硬件常量（kernel_utils_constants.h:56-57，无 arch 守卫）

```cpp
const uint32_t TOTAL_L0A_SIZE = 64 * 1024;
const uint32_t TOTAL_L0B_SIZE = 64 * 1024;
```

#### 依赖 4 — GemmTiling 结构体（kernel_struct_conv2d.h:134-170，无 arch 守卫）

```cpp
struct GemmTiling {
    __aicore__ GemmTiling()
    {
        mIterNum = 1;
        nIterNum = 1;
        kIterNum = 1;
        loopMode = LoopMode::MODE_NM;
    }

    const uint32_t blockSize = 16;
    LoopMode loopMode = LoopMode::MODE_NM;
    uint32_t mNum = 0;
    uint32_t nNum = 0;
    uint32_t kNum = 0;
    uint32_t roundM = 0;
    uint32_t roundN = 0;
    uint32_t roundK = 0;
    uint32_t c0Size = 32;
    uint32_t dtypeSize = 1;
    uint32_t mBlockNum = 0;
    uint32_t nBlockNum = 0;
    uint32_t kBlockNum = 0;
    uint32_t mIterNum = 0;
    uint32_t nIterNum = 0;
    uint32_t kIterNum = 0;
    uint32_t mTileBlock = 0;
    uint32_t nTileBlock = 0;
    uint32_t kTileBlock = 0;
    uint32_t kTailBlock = 0;
    uint32_t mTailBlock = 0;
    uint32_t nTailBlock = 0;
    bool kHasTail = false;
    bool mHasTail = false;
    bool nHasTail = false;
    bool kHasTailEle = false;
    uint32_t kTailEle = 0;
};
```

---

### 各版本上下文（编译 GetGemmTiling 时实际生效的差异部分）

#### dav_l300（目标版本，3003）

kernel_utils_constants.h:155-162 生效分支：

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3003)
const int32_t TOTAL_VEC_LOCAL_SIZE = 118 * 1024;
const uint32_t TOTAL_UB_SIZE = 118 * 1024;
const uint32_t TMP_UB_OFFSET = 118 * 1024;
const uint32_t TOTAL_L1_SIZE = 1024 * 1024;
const uint32_t VECTOR_REG_WIDTH = 256;
const uint32_t ONE_BLOCK_SIZE = 32;
const uint32_t TOTAL_L0C_SIZE = 128 * 1024;
```

下游 Gemm 依赖（V1 L1→L0A/L0B，mm_impl.h L62-80 / L112-130）：

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_ca(
            (__ca__ uint8_t*)dst, (__cbuf__ uint8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, 0, inc);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_ca(
            (__ca__ half*)dst, (__cbuf__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, 0, inc);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_ca(
            (__ca__ uint32_t*)dst, (__cbuf__ uint32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, 0, inc);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_cb(
            (__cb__ uint8_t*)dst, (__cbuf__ uint8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, 0, inc);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_cb(
            (__cb__ half*)dst, (__cbuf__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, 0, inc);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_cb(
            (__cb__ uint32_t*)dst, (__cbuf__ uint32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, 0, inc);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

> MmadCal V1/V2 完整（mm_impl.h L451-463 / L472-488，详见 Mmad-analysis.md）。

#### dav_l310（目标版本，3103）

kernel_utils_constants.h **无 3103 分支**（落入 L223-225 的 `#else`，仅定义 `VECTOR_REG_WIDTH = 256`）：

```cpp
#else
const uint32_t VECTOR_REG_WIDTH = 256;
#endif
```

> 即 `TOTAL_L1_SIZE`/`TOTAL_L0C_SIZE`/`TOTAL_UB_SIZE` 等在 3103 下**未定义**。GetGemmTiling 不引用它们故可编译；但同文件 `Gemm` 的 CPU debug 检查 `CheckParams`（gemm_base_impl.h:76/85）引用 `TOTAL_L0C_SIZE`/`TOTAL_L1_SIZE`，3103 + CPU debug 时编译会失败。

下游 Gemm 依赖（V1 L1→L0A/L0B，mm_impl.h L285-295，**桩**）：

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

> 对比：同文件 V2 版本 `LoadData2DL12L0ACal/L0BCal(LoadData2DParamsV2)`（L53-67 / L97-125）为**完整实现**（调用 `load_cbuf_to_ca`/`load_cbuf_to_cb`），BitMode 版本（L198-280）也完整。MmadCal V1/V2 完整（L426-440 / L449-467）。

#### dav_l311（目标版本，3113）

kernel_utils_constants.h:183-190 生效分支：

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3113)
const int32_t TOTAL_VEC_LOCAL_SIZE = 118 * 1024;
const uint32_t TOTAL_UB_SIZE = 118 * 1024;
const uint32_t TMP_UB_OFFSET = 118 * 1024;
const uint32_t TOTAL_L1_SIZE = 512 * 1024;
const uint32_t TOTAL_L0C_SIZE = 64 * 1024;
const uint32_t VECTOR_REG_WIDTH = 256;
const uint32_t ONE_BLOCK_SIZE = 32;
```

下游 Gemm 依赖（V1 L1→L0A/L0B，mm_impl.h L310-320，**桩**）：

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

> MmadCal V1/V2 完整（L444-458 / L467-485，与 dav_l310 逐字符相同）。

#### dav_5161（目标版本，5101/5161/5165/5163）

kernel_utils_constants.h:191-206 生效分支（5101 与 5161 仅 L1 尺寸不同）：

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
const int32_t TOTAL_VEC_LOCAL_SIZE = 120 * 1024;
const uint32_t TOTAL_UB_SIZE = 120 * 1024;
const uint32_t TMP_UB_OFFSET = 120 * 1024;
const uint32_t TOTAL_L1_SIZE = 1536 * 1024;
const uint32_t TOTAL_L0C_SIZE = 0;          // ← 无 L0C 硬件
const uint32_t VECTOR_REG_WIDTH = 64;
const uint32_t ONE_BLOCK_SIZE = 32;
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161)
const int32_t TOTAL_VEC_LOCAL_SIZE = 120 * 1024;
const uint32_t TOTAL_UB_SIZE = 120 * 1024;
const uint32_t TMP_UB_OFFSET = 120 * 1024;
const uint32_t TOTAL_L1_SIZE = 1280 * 1024;
const uint32_t TOTAL_L0C_SIZE = 0;          // ← 无 L0C 硬件
const uint32_t VECTOR_REG_WIDTH = 64;
const uint32_t ONE_BLOCK_SIZE = 32;
```

下游 Gemm 依赖（V1 L1→L0A/L0B，mm_impl.h L115-125，**桩**；MmadCal V1/V2 亦为桩，L346-357）：

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

> 5101 架构使用宏指令、无 `mad` 硬件指令（见 Mmad-analysis.md 待确认问题 1 的结论），Gemm 链路完全不可用。

#### dav_3510（参照版本）

kernel_utils_constants.h:172-182 生效分支：

```cpp
#elif (__NPU_ARCH__ == 3510)
const int32_t TOTAL_VEC_LOCAL_SIZE = 248 * 1024;
const uint32_t TOTAL_UB_SIZE = 248 * 1024;
const uint32_t TMP_UB_OFFSET = 248 * 1024;
const uint32_t TOTAL_L1_SIZE = 512 * 1024;
const uint32_t SINGLE_MSG_SIZE = 64;
const uint32_t CACHE_LINE_SIZE = 64;
const uint32_t TOTAL_L0C_SIZE = 256 * 1024;
const uint32_t VECTOR_REG_WIDTH = 256;
const uint32_t VECTOR_REG_WIDTH_2XVL = 512;
const uint32_t ONE_BLOCK_SIZE = 32;
```

下游 Gemm 依赖（V1 L1→L0A/L0B，mm_impl.h L67-115，**完整实现**，将 V1 参数适配为 V2 语义后调用硬件指令）：

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    static_assert(
        SupportType<T, uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, float, int32_t, uint32_t>(),
        "LoadData 2dv2 only support uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, float, int32_t, uint32_t on "
        "current device!");
    if ASCEND_IS_AIC {
        uint16_t mStartPosition = 0;
        uint16_t kStartPosition = loadDataParam.startIndex;
        uint8_t kStep = loadDataParam.repeatTimes;
        int16_t srcStride = static_cast<int16_t>(loadDataParam.srcStride);
        uint16_t dstStride = loadDataParam.dstGap + 1;

        if (loadDataParam.ifTranspose) {
            load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, 1, kStep, srcStride, dstStride, 1);
        } else {
            load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, 1, kStep, srcStride, dstStride, 0);
        }
    }
}

template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    static_assert(
        SupportType<T, uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, float, int32_t, uint32_t>(),
        "LoadData 2dv2 only support uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, float, int32_t, uint32_t on "
        "current device!");
    if ASCEND_IS_AIC {
        uint16_t mStartPosition = 0;
        uint16_t kStartPosition = loadDataParam.startIndex;
        uint8_t kStep = loadDataParam.repeatTimes;
        int16_t srcStride = static_cast<int16_t>(loadDataParam.srcStride);
        uint16_t dstStride = loadDataParam.dstGap + 1;

        if (loadDataParam.ifTranspose) {
            for (uint8_t i = 0; i < kStep; i++) {
                const uint64_t srcFractalIndex =
                    static_cast<uint64_t>(kStartPosition) + static_cast<uint64_t>(i) * loadDataParam.srcStride;
                load_cbuf_to_cb(
                    dst + static_cast<uint64_t>(i) * VALUE_512 / sizeof(T),
                    src + srcFractalIndex * VALUE_512 / sizeof(T), mStartPosition, 0, 1, 1, srcStride, dstStride, 1);
            }
        } else {
            load_cbuf_to_cb(dst, src, mStartPosition, kStartPosition, 1, kStep, srcStride, dstStride, 0);
        }
    }
}
```

> MmadCal V1/V2/V3/V4 完整（L476-498 / L500-538 / L540-572 / L574-610，详见 Mmad-analysis.md）。

## 代码级实现差异分析

### 最底层编译器 API 列表

**GetGemmTiling 本体（全版本统一）：无任何编译器内建 API / 硬件指令** —— 仅 `DivCeil`（constexpr 整数算术）与两个全版本统一的 `const uint32_t` 常量。这是它与 LoadDataWithTranspose/Mmad 等接口的本质区别。

下游 Gemm 执行链各版本调用的最底层编译器 API（决定 GetGemmTiling 产物是否有意义）：

| 版本 | LoadData2D V1（L1→L0A/L0B） | Mmad V1 | 底层编译器 API |
|------|------------------------------|---------|---------------|
| dav_l300 | 完整 | 完整 | `load_cbuf_to_ca` / `load_cbuf_to_cb` / `mad` |
| dav_l310 | 桩 | 完整 | `mad`（LoadData2D 断言） |
| dav_l311 | 桩 | 完整 | `mad`（LoadData2D 断言） |
| dav_5161 | 桩 | 桩 | 无 |
| dav_3510 | 完整 | 完整 | `load_cbuf_to_ca` / `load_cbuf_to_cb` / `mad` / `mad_mx` |

### 实现风格对比

| 特征 | GetGemmTiling（全版本共享） | 说明 |
|------|---------------------------|------|
| 类型检查方式 | `Std::is_same<T, ...>::value` + 运行时 `if` | 非 `if constexpr`/`static_assert`/`SupportType`，与 3510 的 Cal 层风格不一致 |
| 硬件指令调用 | 无 | 纯 tiling 计算 |
| arch 守卫 | 仅 include 层（intf.h:74-76） | 函数体无任何 `__NPU_ARCH__` 分支 |
| 版本分发 | 无 | 调用链止步共享层 |
| deprecated | 仅定义处（intf_impl.h:31-32） | 声明处（intf.h:39）无标记 |

### 实现模式总结

**模式：全版本共享的纯计算函数**
- `GetGemmTiling` + `CalculateGemmTiling` + `DivCeil` + `TOTAL_L0A/B_SIZE` 构成闭环，任何 `__NPU_ARCH__` 值下编译产物完全相同
- 版本隔离机制对其"透明"：mm_base_impl.h 的 dav 分派 include 虽在包含链上，但未被本函数触达

### 关键差异点

1. **GetGemmTiling 是"伪版本无关"接口** —— 编译/运行全版本一致，但其返回值仅供 `Gemm` 使用，而 Gemm 在 l310/l311（V1 LoadData2D 桩）和 5161（Mmad + LoadData2D 全桩，无 L0C 硬件）上不可用，即 **tiling 计算结果在这 3 个目标版本上是"死值"**
2. **3103 在 kernel_utils_constants.h 中无常量分支** —— `TOTAL_L1_SIZE`/`TOTAL_L0C_SIZE`/`TOTAL_UB_SIZE` 未定义（对比 3003 L155、3113 L183、3510 L172 均有分支）。GetGemmTiling 不受影响，但 Gemm 的 CPU debug 检查（gemm_base_impl.h:76/85 引用 `TOTAL_L0C_SIZE`/`TOTAL_L1_SIZE`）在 3103 + CPU debug 下无法编译
3. **k0 计算只看 L0A/L0B（64KB 全版本统一），不看 L0C/L1** —— 与版本相关的 L1/L0C 尺寸（1MB/512KB/1536KB；128KB/64KB/256KB/0）不进入 tiling 计算，tiling 在不同版本的行为一致，但也意味着没有按版本调优
4. **deprecated 属性声明/定义不对称** —— intf.h:39 声明无 `[[deprecated]]`，intf_impl.h:31 定义有；用户仅 include intf.h 时告警行为依赖编译器对属性的合并规则
5. **类型分支粒度粗** —— 仅区分 8bit 与非 8bit：`bfloat16_t`/`float`/`int32_t` 均落入 `c0=16, dSize=2` 分支（32bit 类型的 dtypeSize 按 2 计算是不精确的，Gemm 官方支持组合仅 s8s8s32/f16f16f32/f16f16f16，见 gemm_base_impl.h:36）

## 待确认问题

1. **l310/l311 的 V1 LoadData2D 桩是硬件不支持还是待补全？** 同文件中 V2（`LoadData2DParamsV2`）和 BitMode 版本的 L1→L0A/L0B 均为完整实现（调用同一 `load_cbuf_to_ca/cb` 指令），说明硬件具备 L1→L0A/L0B 搬运能力，V1 桩大概率只是**未实现**（参数形式旧）。若按 dav_3510 的 V1→V2 参数适配模式（mm_impl.h L67-115）补全，则 Gemm + GetGemmTiling 在 3103/3113 即可整体可用，是否需要补全？
2. **5161 (5101) 上 GetGemmTiling 是否应显式禁用？** 该函数可正常编译运行并返回 tiling，但下游全断言（无 `mad` 指令、`TOTAL_L0C_SIZE=0`）。是否应参照 `ASCENDC_REPORT_NOT_SUPPORT` 风格给出明确报错，或在 intf.h 声明层面排除 5101/5161/5165/5163（类似 kernel_operator_intf.h:27-32 对 vec 接口的排除）？
3. **3103 缺失 kernel_utils_constants.h 分支是否为遗漏？** `TOTAL_L1_SIZE`/`TOTAL_L0C_SIZE`/`TOTAL_UB_SIZE` 在 3103 下未定义。需要确认 l310 硬件的 L1/L0C 实际尺寸（类比：l300 为 1MB/128KB，l311 为 512KB/64KB），并补充分支；否则 Gemm 的 CPU debug 校验在 3103 无法编译。
4. **Gemm dst=UB 路径在 3003/3103/3113/5101/5161/3510 上使用未初始化的 l0c**

   现象与证据链：
   - `intf_impl.h:120-134`：`LocalTensor<T> l0c;` 默认构造后，仅当 dst 位置为 L0C 时通过 `l0c = dst[0]`（拷贝构造）继承地址；dst 为 **UB** 时，只有 `(__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)` 下才执行"UB 借 L0C 计算"的前置搬运：`TBuffAddr{logicPos=C2} → l0c.SetAddr(...) → l0c.InitBuffer(0, TOTAL_L0C_SIZE/sizeof(T)) → DataCopy(l0c, dst, ..., BLOCK_MODE_MATRIX)`；3003/3103/3113/5101/5161/3510 下 `#if` 块为空，**l0c 原样保持默认构造状态**
   - 默认构造即垃圾值：`LocalTensor` 默认构造为空实现（kernel_tensor.h:150 `__aicore__ inline LocalTensor<T>(){};`），唯一数据成员 `TBuffAddr address_`（kernel_tensor_base.h:228）为无默认初始化器的 POD 聚合（L107-115：`dataLen`/`bufferAddr`/`bufferHandle`/`logicPos`）→ 各字段为栈上未定义值
   - 垃圾值如何被消费：`GemmExecNm(l0c, ...)` → `MmadFunc`（gemm_base_impl.h:267）→ `MmadImpl`（mm_base_impl.h:481）执行 `(__cc__ PrimT<T>*)dst.GetPhyAddr()`；NPU 路径 `GetPhyAddr()` 直接返回 `address_.bufferAddr`（kernel_tensor_impl.h:741-749）→ **mad 指令的 C 矩阵目的地址为垃圾值**，累加结果写入非法 L0C 偏移（结果错误或硬件异常；CPU debug 路径用 `address_.absAddr` 垃圾指针解引用，直接崩溃）
   - 尾部回写同样缺失：`intf_impl.h:144-150` 的 `pipe_barrier(PIPE_ALL) + DataCopy(dst, l0c, ..., BLOCK_MODE_MATRIX)`（结果 L0C→UB）同样仅 1001/2002 编译 —— 即使 mad 碰巧未崩溃，UB 上的 dst 也永远收不到计算结果
   - 与 CPU debug 校验矛盾：`CheckParams`（gemm_base_impl.h:108-110）明确允许 `dstScope == UB || dstScope == L0C`，说明设计意图上 UB dst 是合法用法（1001/2002 有完整实现：前置 UB→L0C、后置 L0C→UB），但 3003 之后的版本从未补上该分支
   - 补全的技术可行性：`TBuffAddr`/`SetAddr`/`InitBuffer`（C2 位置）本身全版本可用（kernel_tensor_base.h:107-115/206-223 无 arch 守卫；NPU 路径 `InitBuffer` 用 `get_imm(0)` 取硬件 C2 基址），卡点仅在矩阵模式 `DataCopy(UB↔L0C)` 于各版本的支持情况（需另行核实 DataCopy 的 BLOCK_MODE_MATRIX 路径）

   两种解释待裁定：
   - **解释 A（实现缺陷）**：3003+ 应支持 UB dst，需在 `#else` 分支按 1001/2002 写法补 SetAddr(C2) + InitBuffer + DataCopy 前后置搬运
   - **解释 B（有意裁剪，校验未同步）**：3003+ 的正确用法是 dst=L0C、结果经 Fixpipe 搬出（matrix DataCopy 直通 UB 属于 1001/2002 时代路径），此时应把 CheckParams 收紧为仅 L0C，让 CPU debug 尽早拦截 UB 用法
   - 验证方法：CPU debug（`__NPU_ARCH__=3003`）下构造 dst=UB 的 Gemm 用例观察崩溃；或查 1001/2002 → 3002/3003 迁移期提交历史确认是否有意裁剪

   影响范围：仅 `Gemm` 的 UB dst 用法；`GetGemmTiling` 本身纯计算不受影响，但其产物默认配套 Gemm 使用，评估 GetGemmTiling 在各版本的实际意义时需一并考虑（见问题 1/2）。

5. **[[deprecated]] 标记声明/定义不对称**

   现状：声明 `intf.h:39-40` 无属性；定义 `intf_impl.h:31-33` 带 `[[deprecated("NOTICE: GetGemmTiling has been deprecated and will be removed in the next version. Please do not use it!")]]`。同文件 `Gemm` 同样不对称（intf.h:68-71 无属性 vs intf_impl.h:100-101 有属性）。

   告警当前实际是否生效：
   - C++14 [dcl.attr.deprecated]：属性出现在任一声明上，该名字即被视为 deprecated；但 GCC/Clang（含基于 Clang 的毕昇/CCE）仅在"调用点可见的声明集合中包含带属性的声明"时才发 -Wdeprecated-declarations 告警
   - 本仓布局下 `intf.h:74-76` 在 `__NPU_ARCH__` 定义时紧随声明包含 intf_impl.h，属性声明必然先于用户代码可见 → **正常 NPU / CPU-debug 编译路径下告警实际是生效的**
   - 例外：`__NPU_ARCH__` 未定义的翻译单元（host 侧代码等）只看到无属性的声明 —— 但此时也无定义，调用会直接链接失败，告警缺失没有实际后果
   - 结论：功能上"侥幸"成立，其正确性依赖 intf.h:74-76 的包含顺序这一**副作用**，而非显式设计

   仍建议同步到声明处的理由：
   - **可见性**：intf_impl.h 是 internal header（文件头 L15-20 有 pragma message 禁止直接使用）；用户、IDE 悬浮提示、Doxygen 等只消费 intf.h，完全看不到废弃状态
   - **偏离仓库惯例**：同仓 `GetLength`/`SetAddrWithOffset`（kernel_tensor.h:197-199/184-187）、`InitBuffer`（kernel_tensor_base.h:123/207）、`Print`/`ToFile`（kernel_tensor_impl.h 多处）均把 `[[deprecated]]` 标在**声明处**，gemm 的"仅定义处标记"是少数派写法
   - **健壮性**：消除对 include 顺序的隐式依赖；避免未来重构移除定义处属性时告警静默消失；规避部分静态分析工具对"属性位于非首次声明"的解析差异

   修改建议与风险：
   - 最小改动：将属性复制到 intf.h:39-40 声明（定义处可保留，重复标记合法；或移除保留一处）。`Gemm`（intf.h:68-71）同理
   - 风险：属性上移到公共声明后，所有存量调用点立即触发告警，`-Werror` 的 CI 会大面积失败 —— 需先盘点存量算子依赖（与问题 7 联动）再操作
6. **GetGemmTiling 类型分支是否需要细化？** 32bit 类型（float/int32）落入 `dtypeSize=2` 分支，与其真实 4 字节不符；鉴于 Gemm 支持组合仅 s8s8s32/f16f16f32/f16f16f16（dst 均非 L0A/B 输入侧），输入侧只可能是 8bit/16bit，现状或许够用，但 T=float 时 tiling 计算的 k0 偏大一倍（不安全方向）。是否需要加 `static_assert` 约束 T ∈ {int8/uint8/half}？
7. **接口已废弃，是否仍有版本适配必要？** `[[deprecated]]` 提示将在下一版本移除，且官方指引使用 Matmul 高层接口。对 GetGemmTiling/Gemm 做跨版本补全（问题 1/3）投入前，是否先确认目标版本上是否有存量算子依赖该接口？
