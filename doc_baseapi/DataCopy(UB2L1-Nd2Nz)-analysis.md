# 案例分析：DataCopy(UB→L1 Nd2Nz) 接口跨版本适配

## 接口概述

`DataCopy(LocalTensor<T>& dst, LocalTensor<T>& src, Nd2NzParams& intriParams)` 用于 UB→L1 的 ND→NZ 格式转换数据搬运（仅 TSCM 场景）。

| 变体 | 行号 | 参数类型 | 模板参数 | Arch 守卫 | pipe 标注 |
|------|------|---------|---------|-----------|-----------|
| UB→L1 Nd2Nz | L98 | `Nd2NzParams` | `<T>` | 无 | 无 |

**关键发现**：
- 声明**无 `__NPU_ARCH__` 守卫**，所有版本均编译
- 版本差异完全下沉到 Impl 层

## 调用链

```
kernel_operator_data_copy_intf.h:98-99（声明，无 arch 守卫）
  → kernel_operator_data_copy_intf_impl.h:254-266（公共包装，无 arch 守卫）
    → DataCopyUB2L1ND2NZImpl(dst, src, intriParams)  [版本特定]
```

### 公共包装层（intf_impl.h:254-266）

```cpp
template <typename T>
__aicore__ inline void DataCopy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const Nd2NzParams& intriParams)
{
    using PrimType = PrimT<T>;
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    CheckNd2NzTypeSupport<T>("DataCopy from LocalTensor to LocalTensor with Nd2NzParams");
    CheckDataCopyTensor(dst, src, intriParams, "DataCopy from LocalTensor to LocalTensor with Nd2NzParams");
#endif
#endif
    ASCENDC_REPORT_OVERFLOW_MEM(CheckDataCopyTensorSizeOverflow(dst, src, intriParams));
    DataCopyUB2L1ND2NZImpl((__cbuf__ PrimType*)dst.GetPhyAddr(), (__ubuf__ PrimType*)src.GetPhyAddr(), intriParams);
}
```

**特点**：
- 无 arch 守卫（仅 2201 的调试检查有守卫）
- 直接调用 `DataCopyUB2L1ND2NZImpl`，无共享层分发

### 版本隔离机制

`kernel_operator_data_copy_base_impl.h:32-55` 按 arch 条件 include 各版本目录。

## 支持矩阵

| 版本 | Arch | DataCopyUB2L1ND2NZImpl | 状态 | 硬件指令 |
|------|------|----------------------|------|---------|
| dav_3510 | 3510 | **完整**（L760-842） | ✅ 完整（参照） | `TransND2NZ` + `CopyUbufToCbuf` / `ScmDataCopyND2NZMsg` |
| dav_l300 | 3003 | **完整**（L302-339） | ✅ 完整 | `copy_ubuf_to_cbuf` |
| dav_l310 | 3103 | **完整**（L327-356） | ✅ 完整 | `copy_ubuf_to_cbuf` |
| dav_l311 | 3113 | **完整**（L378-407） | ✅ 完整 | `copy_ubuf_to_cbuf` |
| dav_5161 | 5101 | **未定义** | ❌ 未定义 | — |

### 支持的数据类型矩阵

| 版本 | 支持类型 | 类型限制 |
|------|---------|---------|
| dav_3510 | 所有基本类型 | `ndNum == 1`（仅 1 个 ND 矩阵） |
| dav_l300 | 所有基本类型 | `dValue/srcNdMatrixStride/srcDValue/dstNzMatrixStride` 需 32B 对齐 |
| dav_l310 | 所有基本类型 | 同 l300 |
| dav_l311 | 所有基本类型 | 同 l300 |
| dav_5161 | — | 未定义 |

## 各版本原始代码

---

### dav_3510（参照版本）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L760-842）

```cpp
template <typename T>
__aicore__ inline void DataCopyUB2L1ND2NZImpl(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
#if KFC_C310_SSBUF == 1 || __MIX_CORE_AIC_RATION__ != 1
    ASCENDC_ASSERT((GetTPipePtr() != nullptr), { KERNEL_LOG(KERNEL_ERROR, "tpipe ptr can not be nullptr"); });
    ASCENDC_ASSERT((dst != nullptr), { KERNEL_LOG(KERNEL_ERROR, "dst ptr can not be nullptr"); });
    ASCENDC_ASSERT((src != nullptr), { KERNEL_LOG(KERNEL_ERROR, "src ptr can not be nullptr"); });
    ASCENDC_ASSERT((intriParams.dValue * sizeof(T) % ONE_BLK_SIZE == 0), {
        KERNEL_LOG(KERNEL_ERROR, "dValue must be 32B aligned");
    });
    ASCENDC_ASSERT((intriParams.srcDValue * sizeof(T) % ONE_BLK_SIZE == 0), {
        KERNEL_LOG(KERNEL_ERROR, "srcDValue must be 32B aligned");
    });
    ASCENDC_ASSERT((intriParams.nValue > 0 && intriParams.dValue > 0 && intriParams.srcDValue > 0), {
        KERNEL_LOG(KERNEL_ERROR, "nvalue, dValue and srcDValue must be greater than 0");
    });
    ASCENDC_ASSERT((intriParams.ndNum == 1), {
        KERNEL_LOG(KERNEL_ERROR, "intriParams.ndNum is %hu, which can only be 1", intriParams.ndNum);
    });
    if ASCEND_IS_AIV {
        LocalTensor<T> popBuffer;
        const bool ret = PopStackBuffer<T, TPosition::LCM>(popBuffer);
        ASCENDC_ASSERT((ret), { KERNEL_LOG(KERNEL_ERROR, "src ptr can not be nullptr"); });
        const uint32_t bufferSize = popBuffer.GetSize() * sizeof(T);
        const uint32_t actualSize = ((intriParams.dValue * sizeof(T) / ONE_BLK_SIZE - 1) * intriParams.dstNzC0Stride +
                                     (intriParams.nValue - 1) * intriParams.dstNzNStride + 1) *
                                    ONE_BLK_SIZE;
        ASCENDC_ASSERT((bufferSize >= actualSize), { KERNEL_LOG(KERNEL_ERROR, "src ptr can not be nullptr"); });

        const uint32_t rows = intriParams.nValue;
        const uint32_t cols = AlignUp(intriParams.dValue * sizeof(T), ONE_BLK_SIZE) / sizeof(T);
        uint16_t alignedDValueBlockNum = (intriParams.dValue * sizeof(T) - 1) / ONE_BLK_SIZE + 1;
        uint16_t alignedDValue = cols * ONE_BLK_SIZE / sizeof(T);
        TransND2NZ((__ubuf__ T*)popBuffer.GetPhyAddr(), src, rows, cols, static_cast<T>(0));
        auto eventID = GetTPipePtr()->FetchEventID(HardEvent::V_MTE3);
        SetFlag<HardEvent::V_MTE3>(eventID);
        WaitFlag<HardEvent::V_MTE3>(eventID);
        CopyUbufToCbuf(dst, (__ubuf__ T*)popBuffer.GetPhyAddr(), 1, actualSize / ONE_BLK_SIZE, 0, 0);
    }
#else
    // Add for TSCM AIV: Copy buffer into workspace, and send message to AIC
    ASCENDC_ASSERT((GetTPipePtr() != nullptr), { KERNEL_LOG(KERNEL_ERROR, "tpipe ptr can not be nullptr"); });
    ASCENDC_ASSERT((dst != nullptr), { KERNEL_LOG(KERNEL_ERROR, "dst ptr can not be nullptr"); });
    ASCENDC_ASSERT((src != nullptr), { KERNEL_LOG(KERNEL_ERROR, "src ptr can not be nullptr"); });
    if ASCEND_IS_AIV {
        ASSERT(GetKfcClient() != nullptr);
        ASCENDC_DEBUG_ASSERT(
            (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(src)) % ONE_BLK_SIZE == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "src address should be 32B aligned \n"));
        uint32_t tensorSize = intriParams.nValue * intriParams.dValue;
        int32_t ubAddr = -1;
        // 1. get GM addr, copy corresponding data to GM, ND->ND
#if ASCENDC_CPU_DEBUG
        uint64_t absUbAddr = (uint8_t*)src - (uint8_t*)(GetTPipePtr()->GetBaseAddr((int8_t)TPosition::VECIN));
        uint64_t absL1Addr = (uint8_t*)dst - (uint8_t*)(GetTPipePtr()->GetBaseAddr((int8_t)TPosition::TSCM));
        ASCENDC_ASSERT((absL1Addr % ONE_BLK_SIZE == 0), {
            KERNEL_LOG(
                KERNEL_ERROR, "Failed to check dst tensor address "
                              "alignment in DataCopy with Nd2NzParams from UB(VECIN/VECOUT) to L1 Buffer(TSCM), it "
                              "should be 32B aligned");
        });
        ASCENDC_ASSERT((intriParams.ndNum == 1), {
            KERNEL_LOG(KERNEL_ERROR, "intriParams.ndNum is %hu, which can only be 1", intriParams.ndNum);
        });
        GM_ADDR gmAddr = (GetKfcClient()->AllocUB(tensorSize, ubAddr));
#else
        GM_ADDR gmAddr = (GetKfcClient()->AllocUB(tensorSize, ubAddr));
#endif
        event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
        SetFlag<HardEvent::V_MTE3>(eventID);
        WaitFlag<HardEvent::V_MTE3>(eventID);
        // 2. copy ub->GM
        CopyUbufToGmAlignV2((__gm__ T*)gmAddr, src, 0, 1, tensorSize * sizeof(T) / 32, 0, false, 0);

        // 3. send message, copy GM to L1
#if ASCENDC_CPU_DEBUG
        ScmDataCopyND2NZMsg((__cbuf__ void*)absL1Addr, (__gm__ void*)gmAddr, sizeof(T), intriParams, ubAddr);
#else
        ScmDataCopyND2NZMsg((__cbuf__ void*)dst, (__gm__ void*)gmAddr, sizeof(T), intriParams, ubAddr);
#endif
    }
#endif
}
```

---

### dav_l300（目标版本，3003）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L302-339）

```cpp
template <typename T>
__aicore__ inline void DataCopyUB2L1ND2NZImpl(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    uint16_t ndNum = intriParams.ndNum;
    uint16_t nValue = intriParams.nValue;
    uint16_t dValue = intriParams.dValue;
    uint16_t srcNdMatrixStride = intriParams.srcNdMatrixStride;
    uint16_t srcDValue = intriParams.srcDValue;
    uint16_t dstNzC0Stride = intriParams.dstNzC0Stride;
    uint16_t dstNzNStride = intriParams.dstNzNStride;
    uint16_t dstNzMatrixStride = intriParams.dstNzMatrixStride;

    ASCENDC_DEBUG_ASSERT(
        ((dValue * sizeof(T)) % BYTE_32_ALIGN == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dValue should be 32B aligned \n"));
    ASCENDC_DEBUG_ASSERT(
        ((srcNdMatrixStride * sizeof(T)) % BYTE_32_ALIGN == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "srcNdMatrixStride should be 32B aligned \n"));
    ASCENDC_DEBUG_ASSERT(
        ((srcDValue * sizeof(T)) % BYTE_32_ALIGN == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dValue should be 32B aligned \n"));
    ASCENDC_DEBUG_ASSERT(
        ((dstNzMatrixStride * sizeof(T)) % BYTE_32_ALIGN == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dValue should be 32B aligned \n"));

    uint32_t elementsPerBlock = ONE_BLK_SIZE / sizeof(T);
    uint32_t NBlockSize = dValue / elementsPerBlock;
    for (uint32_t ndIdx = 0; ndIdx < ndNum; ++ndIdx) {
        for (int i = 0; i < NBlockSize; ++i) {
            uint32_t offsetDst = ndIdx * dstNzMatrixStride + i * elementsPerBlock * dstNzC0Stride;
            uint32_t offsetSrc = ndIdx * srcNdMatrixStride + i * elementsPerBlock;
            copy_ubuf_to_cbuf(
                dst + offsetDst, src + offsetSrc, 0, nValue, 1, (srcDValue / elementsPerBlock - 1), (dstNzNStride - 1));
        }
    }
}
```

---

### dav_l310（目标版本，3103）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L327-356）

```cpp
template <typename T>
__aicore__ inline void DataCopyUB2L1ND2NZImpl(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    uint16_t ndNum = intriParams.ndNum;
    uint16_t nValue = intriParams.nValue;
    uint16_t dValue = intriParams.dValue;
    uint16_t srcNdMatrixStride = intriParams.srcNdMatrixStride;
    uint16_t srcDValue = intriParams.srcDValue;
    uint16_t dstNzC0Stride = intriParams.dstNzC0Stride;
    uint16_t dstNzNStride = intriParams.dstNzNStride;
    uint16_t dstNzMatrixStride = intriParams.dstNzMatrixStride;

    ASCENDC_DEBUG_ASSERT(((dValue * sizeof(T)) % BYTE_32_ALIGN == 0), "dValue should be 32B aligned \n");
    ASCENDC_DEBUG_ASSERT(
        ((srcNdMatrixStride * sizeof(T)) % BYTE_32_ALIGN == 0), "srcNdMatrixStride should be 32B aligned \n");
    ASCENDC_DEBUG_ASSERT(((srcDValue * sizeof(T)) % BYTE_32_ALIGN == 0), "srcDValue should be 32B aligned \n");
    ASCENDC_DEBUG_ASSERT(
        ((dstNzMatrixStride * sizeof(T)) % BYTE_32_ALIGN == 0), "dstNzMatrixStride should be 32B aligned \n");

    uint32_t elementsPerBlock = ONE_BLK_SIZE / sizeof(T);
    uint32_t NBlockSize = dValue / elementsPerBlock;
    for (uint32_t ndIdx = 0; ndIdx < ndNum; ++ndIdx) {
        for (int i = 0; i < NBlockSize; ++i) {
            uint32_t offsetDst = ndIdx * dstNzMatrixStride + i * elementsPerBlock * dstNzC0Stride;
            uint32_t offsetSrc = ndIdx * srcNdMatrixStride + i * elementsPerBlock;
            copy_ubuf_to_cbuf(
                dst + offsetDst, src + offsetSrc, 0, nValue, 1, srcDValue / elementsPerBlock - 1, dstNzNStride - 1);
        }
    }
}
```

---

### dav_l311（目标版本，3113）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L378-407）

```cpp
template <typename T>
__aicore__ inline void DataCopyUB2L1ND2NZImpl(__cbuf__ T* dst, __ubuf__ T* src, const Nd2NzParams& intriParams)
{
    uint16_t ndNum = intriParams.ndNum;
    uint16_t nValue = intriParams.nValue;
    uint16_t dValue = intriParams.dValue;
    uint16_t srcNdMatrixStride = intriParams.srcNdMatrixStride;
    uint16_t srcDValue = intriParams.srcDValue;
    uint16_t dstNzC0Stride = intriParams.dstNzC0Stride;
    uint16_t dstNzNStride = intriParams.dstNzNStride;
    uint16_t dstNzMatrixStride = intriParams.dstNzMatrixStride;

    ASCENDC_DEBUG_ASSERT(((dValue * sizeof(T)) % BYTE_32_ALIGN == 0), "dValue should be 32B aligned \n");
    ASCENDC_DEBUG_ASSERT(
        ((srcNdMatrixStride * sizeof(T)) % BYTE_32_ALIGN == 0), "srcNdMatrixStride should be 32B aligned \n");
    ASCENDC_DEBUG_ASSERT(((srcDValue * sizeof(T)) % BYTE_32_ALIGN == 0), "srcDValue should be 32B aligned \n");
    ASCENDC_DEBUG_ASSERT(
        ((dstNzMatrixStride * sizeof(T)) % BYTE_32_ALIGN == 0), "dstNzMatrixStride should be 32B aligned \n");

    uint32_t elementsPerBlock = ONE_BLK_SIZE / sizeof(T);
    uint32_t NBlockSize = dValue / elementsPerBlock;
    for (uint32_t ndIdx = 0; ndIdx < ndNum; ++ndIdx) {
        for (int i = 0; i < NBlockSize; ++i) {
            uint32_t offsetDst = ndIdx * dstNzMatrixStride + i * elementsPerBlock * dstNzC0Stride;
            uint32_t offsetSrc = ndIdx * srcNdMatrixStride + i * elementsPerBlock;
            copy_ubuf_to_cbuf(
                dst + offsetDst, src + offsetSrc, 0, nValue, 1, srcDValue / elementsPerBlock - 1, dstNzNStride - 1);
        }
    }
}
```

> **dav_l310 与 dav_l311 的代码完全一致。**

---

### dav_5161（目标版本，5101）

#### DataCopyUB2L1ND2NZImpl — 未定义

> 文件中不存在此函数。若包装层（L265）被调用，将导致链接错误。

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| 实现状态 | ✅完整 | ✅完整 | ✅完整 | ✅完整 | ❌未定义 |
| 实现方式 | AIV 路径 + TSCM 消息 | 循环 `copy_ubuf_to_cbuf` | 循环 `copy_ubuf_to_cbuf` | 同 l310 | — |
| 硬件指令 | `TransND2NZ` + `CopyUbufToCbuf` / `ScmDataCopyND2NZMsg` | `copy_ubuf_to_cbuf` | `copy_ubuf_to_cbuf` | 同 l310 | — |
| AIV/AIC 守卫 | `if ASCEND_IS_AIV` | 无 | 无 | 无 | — |
| ndNum 限制 | `ndNum == 1`（ASSERT） | 无限制（循环 ndNum） | 无限制（循环 ndNum） | 同 l310 | — |
| 对齐检查 | dValue/srcDValue 32B | dValue/srcNdMatrixStride/srcDValue/dstNzMatrixStride 32B | 同 l300 | 同 l300 | — |
| KFC/TSCM 消息 | 有（`ScmDataCopyND2NZMsg`） | 无 | 无 | 无 | — |
| PopStackBuffer | 有 | 无 | 无 | 无 | — |
| 条件编译 | `#if KFC_C310_SSBUF` / `#else` | 无 | 无 | 无 | — |
| 代码行数 | ~80 行 | ~35 行 | ~30 行 | ~30 行 | 0 |

### 实现模式总结

**模式1：dav_3510 — TSCM AIV 消息模式**
- 复杂的两路条件编译（`KFC_C310_SSBUF` / `__MIX_CORE_AIC_RATION__`）
- SSBUF 路径：AIV 中 `TransND2NZ`（软件转置）+ `CopyUbufToCbuf`
- TSCM 路径：AIV 中 UB→GM + `ScmDataCopyND2NZMsg` 消息通知 AIC
- 限制 `ndNum == 1`
- 代码量大，逻辑复杂

**模式2：dav_l300/l310/l311 — 循环 copy_ubuf_to_cbuf 模式**
- 简单双重循环（ndIdx × NBlockSize）
- 每次循环调用 `copy_ubuf_to_cbuf` 搬运一个 block
- 无 AIV/AIC 守卫，无条件编译
- 支持 `ndNum > 1`
- l300 用 `KERNEL_LOG_INTERNAL` 格式，l310/l311 用简化字符串格式
- l300/l310/l311 代码几乎一致（仅 `ASCENDC_DEBUG_ASSERT` 格式和括号差异）

### 关键差异点

1. **dav_5161 未定义** — 5101 不支持 UB→L1 Nd2Nz，若调用将链接错误。是硬件不支持还是待补全？

2. **dav_3510 独有 TSCM 消息机制** — 3510 通过 `ScmDataCopyND2NZMsg` 实现跨核 ND2NZ，其他版本直接循环 `copy_ubuf_to_cbuf`

3. **dav_3510 限制 ndNum == 1** — 3510 强制 `ndNum == 1`，其他版本支持 `ndNum > 1`

4. **dav_3510 两路条件编译** — `KFC_C310_SSBUF` 和 `__MIX_CORE_AIC_RATION__` 控制不同路径

5. **l300/l310/l311 代码几乎一致** — 仅 `ASCENDC_DEBUG_ASSERT` 调用格式不同（l300 带 `KERNEL_LOG_INTERNAL`，l310/l311 用字符串）

## 待确认问题

1. **dav_5161 未定义** — `DataCopyUB2L1ND2NZImpl` 在 5101 上完全未定义。是硬件不支持 UB→L1 Nd2Nz 还是待补全？若需要补全，应参照 l310/l311 模式（循环 `copy_ubuf_to_cbuf`）还是其他？

2. **dav_3510 ndNum 限制** — 3510 强制 `ndNum == 1`，其他版本支持 `ndNum > 1`。是硬件限制还是软件保守？

3. **dav_3510 TSCM 消息机制** — 3510 的 `ScmDataCopyND2NZMsg` 是否为 3510 独有？其他版本是否需要类似机制？

4. **l300/l310/l311 代码一致性** — 三个版本代码几乎一致，仅 `ASCENDC_DEBUG_ASSERT` 格式不同。是否应该统一？

5. **dav_5161 `copy_ubuf_to_cbuf` 支持** — 5101 是否支持 `copy_ubuf_to_cbuf` 指令？若支持，可参照 l310/l311 模式补全。
