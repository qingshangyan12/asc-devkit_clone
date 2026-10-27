# Gemm 接口适配报告

接口：`Gemm`（`asc-devkit\include\basic_api\kernel_operator_gemm_intf.h` L68，已标记 deprecated）

适配方案（已确认）：
1. 3103 各 buffer 大小常量已由用户补充，按最新实际硬件情况适配
2. 3103/3113 适配为 V2 的 LoadData2D 接口
3. 5101 版本暂时维持现状
4. 非 1001/2002 增加防护，避免静默写非法地址；只加宏控，不缩紧参数

---

## 一、适配后支持矩阵

| 版本 | LoadL0A/LoadL0B（LoadData2D） | Mmad V1 | dst=L0C 路径 | dst=UB 路径 | CPU debug 常量 |
|------|------------------------------|---------|--------------|-------------|----------------|
| 1001/2002 | 维持（V1 完整实现） | 维持（完整） | 维持（可用） | 维持（可用，UB→L0C→UB 中转闭环） | 维持 |
| 3003 (dav_l300) | 维持（V1 完整实现） | 维持（完整） | 维持（可用） | 静默未初始化 → **assert 显式报错（防护）** | 维持（L1=1MB, L0C=128KB） |
| 3103 (dav_l310) | **V1 桩(运行时报错) → V2 完整实现** | 维持（完整） | **不可用 → 可用** | 静默未初始化 → **assert 显式报错（防护）** | **用户补充：L1=512KB, L0C=32KB, UB=128KB（本次扫描确认）** |
| 3113 (dav_l311) | **V1 桩(运行时报错) → V2 完整实现** | 维持（完整） | **不可用 → 可用** | 静默未初始化 → **assert 显式报错（防护）** | 维持（L1=512KB, L0C=64KB） |
| 5101 (dav_5161) | 维持（V1 桩，运行时报错） | 维持（桩，运行时报错） | 维持（不可用：无 L0A/L0C 硬件，TOTAL_L0C_SIZE=0） | 静默未初始化 → **assert 显式报错（防护）** | 维持（L1=1536KB, L0C=0） |
| 3510 (dav_3510，参照) | 维持（V1 完整实现） | 维持（完整） | 维持（可用） | 静默未初始化 → **assert 显式报错（防护）** | 维持（L1=512KB, L0C=256KB） |

说明：
- 3103/3113 的 V2 路径：`LoadL0A`/`LoadL0B` 在 `__NPU_ARCH__ == 3103 || 3113` 下改用 `LoadData2DParamsV2`，经 `LoadDataImpl(LocalTensor, LocalTensor, LoadData2DParamsV2)` → `LoadData2DL12L0ACal/L0BCal(V2)`（dav_l310/dav_l311 均为完整实现）→ `load_cbuf_to_ca/cb` 指令
- Mmad V1 在 l310/l311 为完整实现（此前分析确认），无需改动
- 防护对所有非 1001/2002 架构生效（含 5101），仅加 `#if/#else` 宏控的 `ASCENDC_ASSERT`，`CheckParams`（CPU debug 参数检查）与函数签名均未改动
- 5101 未做任何可用性适配（LoadData2D V1 桩 + Mmad 桩 + 无 L0C 硬件均维持）

## 二、修改的文件清单

| 文件 | 修改内容 | 修改人 |
|------|---------|--------|
| `asc-devkit\impl\basic_api\kernel_operator_gemm_base_impl.h` | `LoadL0A`/`LoadL0B` 增加 `#if 3103/3113` V2 分支（4 个调用点），`#else` 保留原 V1 代码 | 本次适配 |
| `asc-devkit\impl\basic_api\kernel_operator_gemm_intf_impl.h` | `Gemm` 的 dst 非 L0C 分支增加 `#else` + `ASCENDC_ASSERT(false, ...)` 防护 | 本次适配 |
| `asc-devkit\impl\basic_api\utils\kernel_utils_constants.h` | 3103 分支补充 `TOTAL_UB_SIZE=128KB`、`TMP_UB_OFFSET=128KB`、`TOTAL_L1_SIZE=512KB`、`TOTAL_L0C_SIZE=32KB`、`ONE_BLOCK_SIZE=32` | **用户自行补充（本次仅重新扫描确认，未改动）** |

V1→V2 参数映射依据（dav_3510 的 V1 实现中的指令参数换算）：

| V1（LoadData2DParams） | V2（LoadData2DParamsV2） | 说明 |
|------------------------|--------------------------|------|
| startIndex（均为 0） | kStartPosition = 0 | 3510 V1: `kStartPosition = startIndex` |
| repeatTimes | kStep = repeatTimes | 3510 V1: `kStep = repeatTimes` |
| —（隐含） | mStep = 1 | 3510 V1 硬编码 `mStep=1` |
| —（隐含） | mStartPosition = 0 | 3510 V1 硬编码 `mStartPosition=0` |
| srcStride | srcStride（原值） | 直传 |
| dstGap（默认 0） | dstStride = 1 | 3510 V1: `dstStride = dstGap + 1` |
| ifTranspose（默认 false） | ifTranspose = false（默认） | 直传 |

## 三、适配后各版本原始代码

### 3.1 Gemm（`kernel_operator_gemm_intf_impl.h` L99-155）——【已修改】

```cpp
template <typename T, typename U, typename S>
[[deprecated("NOTICE: Gemm has been deprecated and will be removed in the next version. "
             "Please do not use it!")]]
__aicore__ inline __inout_pipe__(V) void Gemm(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<S>& src1, const uint32_t m,
    const uint32_t k, const uint32_t n, GemmTiling tiling, bool partialsum, int32_t initValue)
{
#if ASCENDC_CPU_DEBUG
    bool flag = CheckParams(dst, src0, src1, m, k, n, tiling);
    if (!flag) {
        return;
    }
#endif

#if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)
    DataCopyParams dataCopyParams;
    dataCopyParams.blockCount = 1;
    DataCopyEnhancedParams enhancedParams;
    enhancedParams.blockMode = BlockMode::BLOCK_MODE_MATRIX;
#endif

    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    LocalTensor<T> l0c;
    if (dstScope == Hardware::L0C) {
        l0c = dst[0];
    } else {
#if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)
        TBuffAddr tbufc;
        tbufc.logicPos = static_cast<uint8_t>(TPosition::C2);
        l0c.SetAddr(tbufc);
        l0c.InitBuffer(0, TOTAL_L0C_SIZE / sizeof(PrimT<T>));

        dataCopyParams.blockLen = dst.GetSize() * sizeof(PrimT<T>) / 1024;
        DataCopy(l0c, dst, dataCopyParams, enhancedParams);
#else
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(KERNEL_ERROR, "Gemm dst only supports L0C Buffer(C2) on current device");
        });
#endif
    }

    if (tiling.loopMode == LoopMode::MODE_NM) {
        GemmExecNm(l0c, src0, src1, tiling, initValue);
    } else if (tiling.loopMode == LoopMode::MODE_MN) {
        GemmExecMn(l0c, src0, src1, tiling, initValue);
    } else {
        // other mode are not supported
    }

#if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)
    if (dstScope == Hardware::UB) {
        pipe_barrier(PIPE_ALL);
        dataCopyParams.blockLen = tiling.roundM * tiling.roundN * sizeof(PrimT<T>) / 1024;
        DataCopy(dst, l0c, dataCopyParams, enhancedParams);
    }
#endif
}
```

修改点：else 分支新增 `#else` + `ASCENDC_ASSERT(false, {...})`（L133-137）。`CheckParams`、函数签名、1001/2002 中转路径均未改动。

### 3.2 LoadL0A（`kernel_operator_gemm_base_impl.h` L217-265）——【已修改】

```cpp
template <typename T>
__aicore__ inline void LoadL0A(
    uint32_t kBlocks, uint32_t mBlocks, GemmTiling tiling, uint32_t i, uint32_t t, const LocalTensor<T>& src0,
    const LocalTensor<T>& l0a)
{
    if (kBlocks == 1) {
        uint32_t l1aSize = i * tiling.kTileBlock * tiling.mBlockNum * tiling.blockSize * tiling.c0Size;
        uint32_t l1aOffset = t * tiling.mTileBlock * tiling.blockSize * tiling.c0Size + l1aSize;
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113))
        LoadData2DParamsV2 params;
        params.mStartPosition = 0;
        params.kStartPosition = 0;
        params.mStep = 1;
        params.kStep = mBlocks;
        params.srcStride = 1;
        params.dstStride = 1;
        LoadDataImpl(l0a, src0[l1aOffset], params);
#else
        LoadData2DParams params;
        params.startIndex = 0;
        params.repeatTimes = mBlocks;
        params.srcStride = 1;
        LoadDataImpl(l0a, src0[l1aOffset], params);
#endif
    } else {
        // load data row by row
        for (size_t index = 0; index < mBlocks; index++) {
            uint32_t l0aOffset = index * kBlocks * tiling.blockSize * tiling.c0Size;
            uint32_t l1aOffset = (t * tiling.mTileBlock + index) * tiling.blockSize * tiling.c0Size +
                                 i * tiling.kTileBlock * tiling.mBlockNum * tiling.blockSize * tiling.c0Size;
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113))
            LoadData2DParamsV2 params;
            params.mStartPosition = 0;
            params.kStartPosition = 0;
            params.mStep = 1;
            params.kStep = kBlocks;
            params.srcStride = tiling.mBlockNum;
            params.dstStride = 1;
            LoadDataImpl(l0a[l0aOffset], src0[l1aOffset], params);
#else
            LoadData2DParams params;
            params.startIndex = 0;
            params.repeatTimes = kBlocks;
            params.srcStride = tiling.mBlockNum;
            LoadDataImpl(l0a[l0aOffset], src0[l1aOffset], params);
#endif
        }
    }
}
```

修改点：两个调用点增加 `#if 3103/3113` V2 分支；V2 参数按上文映射表换算（情形1：kStep=mBlocks, srcStride=1；情形2：kStep=kBlocks, srcStride=tiling.mBlockNum）。其余架构（1001/2002/2201/3002/3003/3102/3510/5101 等）走 `#else` 原 V1 代码，行为不变。

### 3.3 LoadL0B（`kernel_operator_gemm_base_impl.h` L166-215）——【已修改】

```cpp
template <typename T>
__aicore__ inline void LoadL0B(
    uint32_t kBlocks, uint32_t nBlocks, GemmTiling tiling, uint32_t i, uint32_t j, const LocalTensor<T>& src1,
    const LocalTensor<T>& l0b)
{
    if (tiling.nIterNum == 1) {
        uint32_t wSize = tiling.blockSize * tiling.c0Size;
        uint32_t wIdx = (i * tiling.kTileBlock * tiling.nBlockNum + j * tiling.nTileBlock) * wSize;
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113))
        LoadData2DParamsV2 params;
        params.mStartPosition = 0;
        params.kStartPosition = 0;
        params.mStep = 1;
        params.kStep = kBlocks * nBlocks;
        params.srcStride = 1;
        params.dstStride = 1;
        LoadDataImpl(l0b, src1[wIdx], params);
#else
        LoadData2DParams params;
        params.startIndex = 0;
        params.repeatTimes = kBlocks * nBlocks;
        params.srcStride = 1;
        LoadDataImpl(l0b, src1[wIdx], params);
#endif
    } else {
        // load data row by row
        for (size_t index = 0; index < kBlocks; ++index) {
            uint32_t wSize = j * tiling.nTileBlock * tiling.blockSize * tiling.c0Size;
            uint32_t wIdx =
                (i * tiling.kTileBlock + index) * tiling.nBlockNum * tiling.blockSize * tiling.c0Size + wSize;
            uint32_t l0bIdx = index * nBlocks * tiling.blockSize * tiling.c0Size;
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113))
            LoadData2DParamsV2 params;
            params.mStartPosition = 0;
            params.kStartPosition = 0;
            params.mStep = 1;
            params.kStep = nBlocks;
            params.srcStride = 1;
            params.dstStride = 1;
            LoadDataImpl(l0b[l0bIdx], src1[wIdx], params);
#else
            LoadData2DParams params;
            params.startIndex = 0;
            params.repeatTimes = nBlocks;
            params.srcStride = 1;
            LoadDataImpl(l0b[l0bIdx], src1[wIdx], params);
#endif
        }
    }
}
```

修改点：两个调用点增加 `#if 3103/3113` V2 分支（情形1：kStep=kBlocks*nBlocks；情形2：kStep=nBlocks；srcStride 均为 1）。其余架构走原 V1 代码，行为不变。

### 3.4 3103 常量（`kernel_utils_constants.h` L177-182）——【用户补充，本次扫描确认，未改动】

```cpp
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3103)
const uint32_t TOTAL_UB_SIZE = 128 * 1024;
const uint32_t TMP_UB_OFFSET = 128 * 1024;
const uint32_t TOTAL_L1_SIZE = 512 * 1024;
const uint32_t TOTAL_L0C_SIZE = 32 * 1024;
const uint32_t ONE_BLOCK_SIZE = 32;
```

补充后 Gemm 的 CPU debug 路径（`CheckParams`/`CheckOverflow` 引用 `TOTAL_L0C_SIZE`/`TOTAL_L1_SIZE`）在 3103 下可编译。

### 3.5 各版本 LoadData2D V1 Cal——【未修改，维持】

3003/3510 走 V1 完整实现（本次未改动），5101 维持 V1 桩：

```cpp
// dav_l300 L62-80（L0A，完整实现，维持）
load_cbuf_to_ca(dst, src, startIndex, repeatTimes, srcStride, dstGap, 0, inc);

// dav_3510 L68-87（L0A，完整实现，维持）
uint16_t mStartPosition = 0;
uint16_t kStartPosition = loadDataParam.startIndex;
uint8_t kStep = loadDataParam.repeatTimes;
int16_t srcStride = static_cast<int16_t>(loadDataParam.srcStride);
uint16_t dstStride = loadDataParam.dstGap + 1;
load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, 1, kStep, srcStride, dstStride, 0);

// dav_5161 L115-119（L0A，桩，维持）
ASCENDC_ASSERT(false, {
    KERNEL_LOG(KERNEL_ERROR,
        "LoadData from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
});
```

### 3.6 3103/3113 LoadData2D V2 Cal——【未修改，本次新启用】

```cpp
// dav_l310 L53-67 / dav_l311 L53-67（L0A，完整实现，两版本代码一致）
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    if constexpr (1 == sizeof(T)) {
        load_cbuf_to_ca(
            (__ca__ int8_t*)dst, (__cbuf__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.mStep, loadDataParam.kStep, loadDataParam.srcStride, loadDataParam.dstStride, false);
    } else if constexpr (2 == sizeof(T)) {
        load_cbuf_to_ca(
            (__ca__ int16_t*)dst, (__cbuf__ int16_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.mStep, loadDataParam.kStep, loadDataParam.srcStride, loadDataParam.dstStride, false);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}

// dav_l310 L97-125 / dav_l311 L94-122（L0B，完整实现，非转置分支）
load_cbuf_to_cb(
    (__cb__ int8_t*/int16_t*)dst, (__cbuf__ int8_t*/int16_t*)src, loadDataParam.mStartPosition,
    loadDataParam.kStartPosition, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.srcStride,
    loadDataParam.dstStride, false);
```

说明：Gemm 支持的类型组合（s8s8s32/f16f16f32/f16f16f16）中 src0/src1 为 int8/uint8/half（1 或 2 字节），V2 Cal 的 `sizeof(T)==1/2` 分支完全覆盖。分发链：`LoadDataImpl(LocalTensor, LocalTensor, LoadData2DParamsV2)`（`kernel_operator_mm_load2d_impl.h` L187-202）→ 上述 V2 Cal，经 `kernel_operator_mm_base_impl.h` L51 引入，对 3103/3113 完整可用。

### 3.7 各版本 Mmad V1——【未修改，维持】

维持原状（详见 Mmad-analysis.md）：3003/3103/3113/3510 均为完整实现（C 风格 config 打包 + `mad` 指令），5101 维持桩（`ASCENDC_ASSERT(false, ...)`，硬件不支持 mad 指令）。

### 3.8 同步 API（SetFlag/WaitFlag\<HardEvent::M_MTE1\>、PipeBarrier\<PIPE_M\>）——【未修改，维持】

维持原状：AIC 上全架构完整实现，Gemm 主路径依赖的 M_MTE1 同步无需改动。

---

## 四、验证结论

1. `LoadL0A`/`LoadL0B` 修改后已读回验证：4 个调用点的 V2 参数与 V1 语义逐一对应（映射依据 dav_3510 V1 实现的指令参数换算），`#else` 分支代码与修改前逐字一致
2. `Gemm` 防护修改后已读回验证：仅新增 `#else` + `ASCENDC_ASSERT(false, {...})`，格式与仓库桩函数统一格式一致，`CheckParams` 与函数签名未动
3. 可见性验证：`LoadData2DParamsV2` 经 `kernel_struct_mm.h`（gemm_base_impl.h L29 引入）可见；`LoadDataImpl` V2 重载经 `kernel_operator_mm_load2d_impl.h`（mm_base_impl.h L51 引入）可见；`ASCENDC_ASSERT`/`KERNEL_LOG` 在同一翻译单元的 include 链中先于使用点定义（dav_l310/l311 mm_impl.h 均已在链中使用该宏）
4. 影响面确认：1001/2002/3003/3510/5101 的 V1 路径与修改前编译产物等价（宏控隔离）；5101 未做任何可用性适配，维持现状
