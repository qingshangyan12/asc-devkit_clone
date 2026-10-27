# 案例分析：Gemm 接口跨版本适配

## 接口概述

`Gemm` 是一个**组合式**矩阵乘法接口（在框架内部组合 LoadData2D + Mmad + 硬件同步事件，自行完成分块搬运与乘加循环），**不同于 Mmad/MmadFix 等"声明→包装→Impl→Cal"四层直链接口，Gemm 没有版本特定的 GemmCal 函数**，其版本差异完全由底层依赖（LoadData2D V1、Mmad V1、同步 API、硬件容量常量）决定。

| 接口 | 签名 | 声明位置 | arch 守卫 | 状态 |
|------|------|---------|-----------|------|
| Gemm | `Gemm(dst, src0, src1, m, k, n, tiling, partialsum=true, initValue=0)` | `gemm_intf.h:68-71` | 无 | **已标记 deprecated**（intf_impl.h:100-101） |

### 参数说明

- `dst`：输出 LocalTensor，支持 L0C（C2）；UB 仅 1001/2002 有中转实现
- `src0/src1`：输入 LocalTensor，必须位于 L1（A1/B1）
- `m/k/n`：矩阵维度
- `tiling`：`GemmTiling` 结构体（`GetGemmTiling` 计算或用户填充），`loopMode` 决定执行路径（MODE_NM→GemmExecNm，MODE_MN→GemmExecMn）
- `partialsum`：**声明但实现中完全未使用**
- `initValue`：C 矩阵初始化控制（0=保留已有值，1=清零），传入 MmadFunc 控制 `cmatrixInitVal`/首块 `isBias`

### 关联接口

`GetGemmTiling`（`gemm_intf.h:39-40`，deprecated）：基于 `TOTAL_L0A_SIZE`/`TOTAL_L0B_SIZE`（全 arch 统一 64KB）计算 tiling，无 arch 守卫，共享实现。

## 调用链

### 完整链路

```
gemm_intf.h:68-71（声明，无 arch 守卫，无文件级守卫）
  → gemm_intf_impl.h:99-151（Gemm 实现，无文件级 arch 守卫；
      #if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002) 专用于 UB 中转/DataCopy 分支 L113-118/125-133/144-150）
    → CheckParams（仅 ASCENDC_CPU_DEBUG 编译，gemm_base_impl.h:92-138）
    → dst 为 L0C：l0c = dst[0]；dst 为 UB：仅 1001/2002 走 L0C 中转，其余 arch l0c 保持未初始化（隐患）
    → GemmExecNm（gemm_base_impl.h:419-430）/ GemmExecMn（L532-543），无 arch 守卫
      → GemmExecNmNopingpong（L316-347）/ GemmExecNmPingPong（L350-416）
        / GemmExecMnNopingpong（L433-464）/ GemmExecMnPingPong（L467-529）
        依赖① GetSingleThreadBuffer（L298-313）/ GetPingPongBuffer（L270-296）→ TBuffAddr/InitBuffer（TOTAL_L0A/B_SIZE 对半分）
        依赖② LoadL0A（L196-221）→ LoadDataImpl（mm_load2d_impl.h:41-56，无 arch 守卫）→ LoadData2DL12L0ACal [dav 版本特定]
        依赖③ LoadL0B（L167-193）→ LoadDataImpl（同上）→ LoadData2DL12L0BCal [dav 版本特定]
        依赖④ MmadFunc（L224-268）→ MmadImpl（mm_base_impl.h:473-483，无 arch 守卫）→ MmadCal V1 [dav 版本特定]
        依赖⑤ SetFlag/WaitFlag<HardEvent::M_MTE1>（公共实现，所有 arch 的 AIC 上完整、AIV 上设计性 no-op）
        依赖⑥ PipeBarrier<PIPE_M>（公共实现，所有 arch 完整）
        依赖⑦ GetTPipePtr()->FetchEventID/AllocEventID/ReleaseEventID（公共实现，所有 arch 完整）
```

### 共享层分发逻辑

- **mm 层版本隔离**：`kernel_operator_mm_base_impl.h:27-49` 通过 `#elif __NPU_ARCH__` 分派 `#include` 各 dav 的 `kernel_operator_mm_impl.h`（3003→dav_l300、3113→dav_l311、3103→dav_l310、5101/5161/5165/5163→dav_5161、3510→dav_3510）
- **LoadDataImpl 分发**（mm_load2d_impl.h:41-56）：按 dst 位置运行时分发——L0A→`LoadData2DL12L0ACal`，L0B→`LoadData2DL12L0BCal`，**无 arch 守卫**
- **MmadImpl V1**（mm_base_impl.h:473-483）：直接调 `MmadCal(c, a, b, mmadParams)`，**无 arch 守卫**
- **Gemm 自身**：gemm_base_impl.h 全部函数（GemmExecNm/Mn×2、LoadL0A/B、MmadFunc、GetPingPongBuffer 等）**均无 arch 守卫**，仅 `GemmExecNmPingPong` 尾部事件释放有 `#if __NPU_ARCH__ == 2201` 特例（L407-415）

## 支持矩阵

### Gemm 主路径支持（dst=L0C，tiling.loopMode=MODE_NM 或 MODE_MN）

| 版本 | Arch | Gemm 整体 | LoadData2D V1 L0A 路径 | LoadData2D V1 L0B 路径 | Mmad V1 | 同步 API (⑤⑥⑦) | 硬件容量常量 |
|------|------|-----------|------------------------|------------------------|---------|----------------|-------------|
| dav_l300 | 3003 | **完整可用** | **完整** (L62-80) | **完整** (L112-130) | **完整** (L451-463) | 完整 | L1=1MB, L0C=128KB |
| dav_l310 | 3103 | **运行时报错**（依赖不可用） | **桩** (L285-289) | **桩** (L291-295) | **完整** (L426-440) | 完整 | **TOTAL_L1/L0C_SIZE 未定义**（仅 CPU debug 路径受影响） |
| dav_l311 | 3113 | **运行时报错**（依赖不可用） | **桩** (L310-314) | **桩** (L316-320) | **完整** (L444-458) | 完整 | L1=512KB, L0C=64KB |
| dav_5161 | 5101 | **完全不可用**（多重依赖不可用） | **桩** (L115-119) | **桩** (L121-125) | **桩** (L346-350) | 完整 | **无 L0C 硬件**：TOTAL_L0C_SIZE=0，ASC_L0A_SIZE=0 |
| dav_3510 | 3510（参照） | **完整可用** | **完整** (L67-87) | **完整** (L89-115) | **完整** (L476-498) | 完整 | L1=512KB, L0C=256KB |

### dst=UB 路径支持

| 版本 | dst=UB | 说明 |
|------|--------|------|
| 1001/2002 | 完整 | DataCopy UB→L0C 中转（intf_impl.h L125-133）+ 结果回搬（L144-150） |
| 3003/3103/3113/5101/3510 | **不可用（静默隐患）** | `l0c` 未初始化即传入 GemmExec*，无任何报错 |

> 注：CPU debug 的 `CheckParams`（gemm_base_impl.h:108-110）允许 dst=UB，与 NPU 实际行为（非 1001/2002 不支持）不一致。

### GetGemmTiling

全部 arch 共享实现（intf_impl.h:30-71），仅依赖全 arch 统一定义的 TOTAL_L0A/B_SIZE（64KB），各版本均可编译运行。

## 各版本底层编译器 API 列表

| 版本 | Gemm 链路实际触发的底层编译器 API |
|------|--------------------------------|
| dav_3510 | `load_cbuf_to_ca`、`load_cbuf_to_cb`、`mad`（MmadCal 直传参数模式）、`set_flag`/`wait_flag`(M_MTE1)、`pipe_barrier`(PIPE_M) |
| dav_l300 | `load_cbuf_to_ca`、`load_cbuf_to_cb`、`mad`（config 打包模式）、`set_flag`/`wait_flag`(M_MTE1)、`pipe_barrier`(PIPE_M) |
| dav_l310 | `mad`、`set_flag`/`wait_flag`(M_MTE1)、`pipe_barrier`(PIPE_M)（LoadData2D V1 为桩，无 load 指令；运行时在 LoadL0A/B 处 assert 失败） |
| dav_l311 | 同 dav_l310 |
| dav_5161 | 仅 `set_flag`/`wait_flag`(M_MTE1)、`pipe_barrier`(PIPE_M)（LoadData2D V1 与 Mmad 均为桩；运行时最先在 LoadL0A/B 处 assert 失败） |
| （对照）1001/2002 | 额外含 `DataCopy`（UB↔L0C 中转） |

## 各版本原始代码

### 共享层：gemm_intf_impl.h — Gemm（L99-151，所有版本共用，无 arch 守卫）

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

### 共享层：gemm_base_impl.h — 关键函数（所有版本共用，无 arch 守卫）

#### GemmExecNm — 分发入口（L419-430）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void GemmExecNm(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    uint32_t needL0Asize = tiling.roundM * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    uint32_t needL0Bsize = tiling.roundN * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    if (needL0Asize > TOTAL_L0A_SIZE || needL0Bsize > TOTAL_L0B_SIZE) {
        GemmExecNmNopingpong(l0c, src0, src1, tiling, initValue);
        return;
    }
    GemmExecNmPingPong(l0c, src0, src1, tiling, initValue);
}
```

> 按 tiling 需求选择乒乓（PingPong，L0A/L0B 对半分双缓冲）或单缓冲（Nopingpong）路径。MODE_MN 的 `GemmExecMn`（L532-543）结构相同。

#### GemmExecNmNopingpong — 单缓冲执行循环（L316-347）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void GemmExecNmNopingpong(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    LocalTensor<U> l0a;
    LocalTensor<S> l0b;
    GetSingleThreadBuffer(l0a, l0b);
    event_t eventIdMToMte1 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::M_MTE1));
    SetFlag<HardEvent::M_MTE1>(eventIdMToMte1);
    for (size_t indexK = 0; indexK < tiling.kIterNum; indexK++) {
        uint32_t kBlocks = tiling.kTileBlock;
        if (indexK == tiling.kIterNum - 1) {
            kBlocks = tiling.kTailBlock;
        }
        WaitFlag<HardEvent::M_MTE1>(eventIdMToMte1);
        for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
            // load data from l1 to l0b
            LoadL0B(kBlocks, tiling.nTileBlock, tiling, indexK, indexN, src1, l0b);
            for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
                // load data from l1 to l0a
                LoadL0A(kBlocks, tiling.mTileBlock, tiling, indexK, indexM, src0, l0a);
                event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                PipeBarrier<PIPE_M>();
                MmadFunc(l0a, l0b, l0c, initValue, tiling, indexK);
            }
        }
        SetFlag<HardEvent::M_MTE1>(eventIdMToMte1);
    }
    WaitFlag<HardEvent::M_MTE1>(eventIdMToMte1);
}
```

> GemmExecNmPingPong（L350-416）循环结构相同，使用 l0aPing/l0aPong/l0bPing/l0bPong 双缓冲交替，事件 id0/id1 交替 Wait/Set；尾部事件释放在 `#if __NPU_ARCH__ == 2201` 下带 ReleaseEventID，其余 arch 仅 WaitFlag（L407-415）。GemmExecMnNopingpong/MnPingPong（L433-529）为 M/N 循环顺序对调的镜像实现。

#### LoadL0A — L1→L0A 搬运（L196-221）

```cpp
template <typename T>
__aicore__ inline void LoadL0A(
    uint32_t kBlocks, uint32_t mBlocks, GemmTiling tiling, uint32_t i, uint32_t t, const LocalTensor<T>& src0,
    const LocalTensor<T>& l0a)
{
    if (kBlocks == 1) {
        uint32_t l1aSize = i * tiling.kTileBlock * tiling.mBlockNum * tiling.blockSize * tiling.c0Size;
        uint32_t l1aOffset = t * tiling.mTileBlock * tiling.blockSize * tiling.c0Size + l1aSize;
        LoadData2DParams params;
        params.startIndex = 0;
        params.repeatTimes = mBlocks;
        params.srcStride = 1;
        LoadDataImpl(l0a, src0[l1aOffset], params);
    } else {
        // load data row by row
        for (size_t index = 0; index < mBlocks; index++) {
            uint32_t l0aOffset = index * kBlocks * tiling.blockSize * tiling.c0Size;
            uint32_t l1aOffset = (t * tiling.mTileBlock + index) * tiling.blockSize * tiling.c0Size +
                                 i * tiling.kTileBlock * tiling.mBlockNum * tiling.blockSize * tiling.c0Size;
            LoadData2DParams params;
            params.startIndex = 0;
            params.repeatTimes = kBlocks;
            params.srcStride = tiling.mBlockNum;
            LoadDataImpl(l0a[l0aOffset], src0[l1aOffset], params);
        }
    }
}
```

> **使用 LoadData2D V1 API（`LoadData2DParams`）** — 这是 Gemm 在 l310/l311/5161 上不可用的直接原因。LoadL0B（L167-193）结构类似（L1→L0B）。

#### MmadFunc — 乘加参数组装（L224-268）

```cpp
template <typename T, typename U, typename S>
__aicore__ inline void MmadFunc(
    const LocalTensor<U>& l0a, const LocalTensor<S>& l0b, const LocalTensor<T>& l0c, int32_t initValue,
    GemmTiling tiling, size_t i)
{
    MmadParams mmadParams;
    mmadParams.m = tiling.mTileBlock * tiling.blockSize;
    mmadParams.n = tiling.nTileBlock * tiling.blockSize;
    mmadParams.isBias = 1;

    if (tiling.kIterNum == 1) {
        mmadParams.k = tiling.kNum;
        mmadParams.isBias = initValue;
    } else if (initValue == 1 && tiling.kHasTailEle) {
        if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailEle;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    } else if (initValue != 1 && tiling.kHasTailEle) {
        if (i == 0) {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
            mmadParams.isBias = 0;
        } else if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailEle;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    } else if (initValue == 1 && !tiling.kHasTailEle) {
        if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailBlock * tiling.c0Size;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    } else {
        if (i == 0) {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
            mmadParams.isBias = 0;
        } else if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailBlock * tiling.c0Size;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    }
    MmadImpl(l0c, l0a, l0b, mmadParams);
}
```

> **使用 Mmad V1 API（`MmadParams`，无 bias）** — MmadImpl 位于 mm_base_impl.h:473-483（无 arch 守卫），直接调 `MmadCal(c, a, b, mmadParams)`。

#### GetPingPongBuffer（L270-296）与 GetSingleThreadBuffer（L298-313）

```cpp
template <typename T, typename U>
__aicore__ inline void GetPingPongBuffer(
    LocalTensor<T>& l0aPing, LocalTensor<T>& l0aPong, LocalTensor<U>& l0bPing, LocalTensor<U>& l0bPong)
{
    // L0Abuffer
    TBuffAddr tbufaPing;
    tbufaPing.logicPos = static_cast<uint8_t>(TPosition::A2);
    l0aPing.SetAddr(tbufaPing);
    l0aPing.InitBuffer(0, TOTAL_L0A_SIZE / 2 / sizeof(PrimT<T>));
    // ...（l0aPong 偏移 TOTAL_L0A_SIZE/2；l0bPing/l0bPong 对 L0B 同样对半分）
}
```

> 直接以 `TOTAL_L0A_SIZE`（全 arch 统一 64KB）对半初始化乒乓缓冲，绕过 TQue/TBuf 分配机制。

### 版本特定层：LoadData2DL12L0ACal / LoadData2DL12L0BCal（LoadData2DParams 版本）

#### dav_3510（参照）— 完整实现

```cpp
// L67-87
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
```

```cpp
// L89-115（L0B，transpose 情形按 512B fractal 循环逐块搬运）
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

#### dav_l300（3003）— 完整实现

```cpp
// L62-80
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
```

```cpp
// L112-130（L0B 同构，指令为 load_cbuf_to_cb）
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

#### dav_l310（3103）— 桩函数

```cpp
// L285-289
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

```cpp
// L291-295
template <typename T>
__aicore__ inline void LoadData2DL12L0BCal(__cb__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

> 注意：l310 的 `LoadData2DParamsV2` 版本（L54-67 L0A / L98-125 L0B）是完整实现（调用 `load_cbuf_to_ca`/`load_cbuf_to_cb`），仅 V1 是桩。

#### dav_l311（3113）— 桩函数

```cpp
// L310-314（L0A）/ L316-320（L0B），与 dav_l310 逐字符相同
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

> V2 版本（L54-67 L0A / L95-122 L0B）完整实现。

#### dav_5161（5101）— 桩函数

```cpp
// L115-119（L0A）/ L121-125（L0B）
template <typename T>
__aicore__ inline void LoadData2DL12L0ACal(__ca__ T* dst, __cbuf__ T* src, const LoadData2DParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

> dav_5161 中 LoadData2D V2 的 L0A 路径（L149-153）**同样是桩**（"LoadData A1 To A2 is not supported!"，5101 无 L0A），仅 L0B V2（L155-175）完整。

### 版本特定层：MmadCal V1（MmadParams 版本）

详见 `Mmad-analysis.md`，此处仅列结论与代码：

#### dav_l300（3003）— 完整（L451-463，config 位域打包模式）

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

> ⚠️ float dst 特化（L445-449）为桩（"unsupported float mmad"）——Gemm 的 dst=T 若为 float（f16f16f32 组合）在 3003 上运行时报错。

#### dav_l310（3103）— 完整（L426-440）/ dav_l311（3113）— 完整（L444-458）

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

> float dst 特化同样为桩。

#### dav_5161（5101）— 桩（L346-350）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Mmad is not supported!"); });
}
```

> **已确认（前次 Mmad 分析）：5101 硬件不支持 `mad` 指令（使用宏指令），桩为硬件限制，非待补全。**

#### dav_3510（参照）— 完整（L476-498，参数直传模式）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadCal(__cc__ DstT* c, __ca__ Src0T* a, __cb__ Src1T* b, const MmadParams& mmadParams)
{
    if ASCEND_IS_AIC {
        bool cmatrixInitVal = mmadParams.cmatrixInitVal && (!mmadParams.isBias);
        constexpr bool isMx = SupportType<...>();  // fp4x2/mx_fp8 类型分派
        if constexpr (isMx) {
            mad_mx(c, (__ca__ AType*)a, (__cb__ BType*)b, mmadParams.m, mmadParams.k, mmadParams.n,
                mmadParams.unitFlag, mmadParams.disableGemv, mmadParams.cmatrixSource, cmatrixInitVal);
        } else {
            mad(c, a, b, mmadParams.m, mmadParams.k, mmadParams.n, mmadParams.unitFlag, mmadParams.disableGemv,
                mmadParams.cmatrixSource, cmatrixInitVal);
        }
    }
}
```

## 代码级实现差异分析

### 风格对比表（依赖函数层）

| 维度 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| LoadData2D V1 | 完整（参数重排 + AIC 守卫 + transpose 分支） | 完整（sizeof(T) 三分支直传 + `inc` 参数） | 桩 | 桩 | 桩 |
| LoadData2D V1 类型检查 | `static_assert(SupportType<...>)` 9 种类型 | `if constexpr` 按字节宽 B8/B16/B32 | — | — | — |
| LoadData2D transpose | `ifTranspose` 分支（L0B 逐 fractal 循环） | 不支持（无分支，第 7 参固定 0） | — | — | — |
| Mmad V1 | 参数直传 `mad()` + `mad_mx()` MX 分派 + AIC 守卫 | config 位域打包（static_cast） | config 位域打包（C 风格 + gemvCtrl bit61=0） | 同 l310 | 桩 |
| Mmad float dst | SupportType 统一处理（支持） | 桩（不支持 float 输出） | 桩 | 桩 | 全桩 |
| AIC 守卫 | LoadData2D/Mmad 均有 `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无 |

### Gemm 组合层实现模式总结

1. **组合式设计**：Gemm 自身（gemm_base_impl.h）是纯共享代码，不做任何版本分发；版本能力完全下放到 LoadData2D/Mmad 两个依赖的 Cal 层
2. **同步策略**：M_MTE1（M→MTE1 许可）+ MTE1_M（MTE1→MTE1 完成通知）事件对 + `PipeBarrier<PIPE_M>` 保证搬运与乘加的流水顺序；乒乓模式用两套事件 id 交替
3. **缓冲管理**：直接以 `TBuffAddr` + `InitBuffer` 硬编码方式切分 L0A/L0B（对半分乒乓），不经过 TPipe/TQue 队列
4. **tiling 驱动**：`GetGemmTiling`/`CalculateGemmTiling` 计算 kIterNum/kTileBlock/m/nBlockNum 等分块参数，GemmExec* 按其循环

### 关键差异点

1. **Gemm 实际可用版本只有 3003 与 3510**：3103/3113 因 LoadData2D V1 桩在运行时首条 LoadL0A/B 处 assert 失败；5101 因 LoadData2D V1 桩 + Mmad 桩 + 无 L0C 硬件三重不可用
2. **Gemm 锁死 LoadData2D V1 API**：`LoadL0A/LoadL0B` 使用 `LoadData2DParams`（V1）。l310/l311 的 V1 桩是"弃用 V1、主推 V2（`LoadData2DParamsV2`，完整实现）"的策略性选择，而非硬件无 load 指令——若要让 Gemm 在 l310/l311 可用，需把 LoadL0A/B 改写为 V2 语义
3. **float 输出（f16f16f32）仅 3510 支持**：3003/3103/3113 的 Mmad float dst 特化均为桩
4. **3103 常量缺口**：`kernel_utils_constants.h` 无 3103 分支，TOTAL_L1_SIZE/TOTAL_L0C_SIZE 未定义——Gemm 的 NPU 编译路径不受影响（仅用 TOTAL_L0A/B_SIZE），但 CPU debug 路径（CheckParams/CheckOverflow）在 3103 下无法编译
5. **5101 无 L0A/L0C**：sys_constants.h 中 ASC_L0A_SIZE=0、ASC_L0C_SIZE=0，TOTAL_L0C_SIZE=0——Gemm 的硬件前提（L0A/L0B/L0C 三级 Cube 缓冲）在 5101 上不存在
6. **dst=UB 静默隐患**：非 1001/2002 架构上 dst=UB 时 `l0c` 未初始化即被使用，无编译期/运行期防护

## 待确认问题

1. **3103/3113 的 LoadData2D V1 桩是弃用还是待补全？** 两者的 V2 完整实现使用相同的 `load_cbuf_to_ca`/`load_cbuf_to_cb` 指令，说明硬件指令本身可用。是有意淘汰 V1 API（则 Gemm 作为 V1 的唯一共享层用户应同步改写或明确不支持），还是 V1 语义（startIndex/repeatTimes/srcStride/dstGap）在这两代硬件上无法映射？
2. **是否通过改写 LoadL0A/LoadL0B 为 V2 语义来恢复 3103/3113 的 Gemm？** 若确认问题 1 为"弃用 V1"，这是唯一路径；需评估 V1→V2 参数映射（startIndex→mStartPosition/kStartPosition 等）的正确性。
3. **5101 的 Gemm 是否应显式声明不支持？** 5101 无 L0A/L0C、Mmad 为桩，Gemm 硬件层面不可能工作，但当前代码仅靠依赖桩在运行时报错。是否应参照 MmadMxFix 的处理（arch 守卫隐藏声明）或在 Gemm 入口加桩断言？
4. **3103 的 TOTAL_L1_SIZE/TOTAL_L0C_SIZE 未定义是否为遗漏？** 若 3103 需要支持 CPU debug（CheckParams 用到），需补 kernel_utils_constants.h 的 3103 分支；若确认编译系统以 -D 注入则无需修改（需验证）。
5. **dst=UB 路径的静默未初始化是否要加防护？** 非 1001/2002 架构上建议加 `ASCENDC_ASSERT(dstScope == Hardware::L0C)`，避免静默写非法地址。
6. **partialsum 参数声明但未使用** — 历史遗留还是未实现的功能？deprecated 状态下是否直接删除？
7. **Gemm/GetGemmTiling 均已 deprecated** — 若官方计划下个版本移除，3103/3113/5101 的适配是否还有必要？建议先确认deprecated 时间线再决定投入。
8. **3113 的 L0A/L0B 容量不一致**：TOTAL_L0A_SIZE=64KB（Gemm 乒乓切分依据）vs sys_constants.h ASC_L0A_SIZE=32KB。若实际硬件 L0A 为 32KB，GetPingPongBuffer 按 64KB 切分会越界，需硬件确认。
9. **CPU debug 的 CheckParams 允许 dst=UB** 与 NPU 实际支持（仅 L0C）不一致，且 MATMUL_SUPPORT_TYPE（s8s8s32/f16f16f32/f16f16f16）中 f16f16f32 在 3003/3103/3113 上实际不可用（float dst 桩）——CPU debug 检查规则需同步收紧。
