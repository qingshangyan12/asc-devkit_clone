# 适配报告：DataCopy(UB→L1 Nd2Nz) 接口（L98）

## 适配后支持矩阵

| 版本 | Arch | DataCopyUB2L1ND2NZImpl | 适配前 → 适配后 |
|------|------|----------------------|----------------|
| dav_3510 | 3510 | 完整实现（L760-842） | 维持 |
| dav_l300 | 3003 | 完整实现（L302-339） | 维持 |
| dav_l310 | 3103 | 完整实现（L327-356） | 维持 |
| dav_l311 | 3113 | 完整实现（L378-407） | 维持 |
| dav_5161 | 5101 | **完整实现**（L398-427） | 未定义 → **完整实现**（从 dav_l311 移植） |

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `impl/basic_api/dav_5161/kernel_operator_data_copy_impl.h` | 新增 `DataCopyUB2L1ND2NZImpl` 完整实现（L398-427），从 dav_l311 移植 |

## 适配后各版本原始代码

### dav_3510（参照版本，维持不变）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L760-842，未修改）

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
#if ASCENDC_CPU_DEBUG
        uint64_t absUbAddr = (uint8_t*)src - (uint8_t*)(GetTPipePtr()->GetBaseAddr((int8_t)TPosition::VECIN));
        uint64_t absL1Addr = (uint8_t*)dst - (uint8_t*)(GetTPipePtr()->GetBaseAddr((int8_t)TPosition::TSCM));
        ASCENDC_ASSERT((absL1Addr % ONE_BLK_SIZE == 0), {
            KERNEL_LOG(KERNEL_ERROR, "...");
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
        CopyUbufToGmAlignV2((__gm__ T*)gmAddr, src, 0, 1, tensorSize * sizeof(T) / 32, 0, false, 0);
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

### dav_l300（目标版本，3003，维持不变）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L302-339，未修改）

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

### dav_l310（目标版本，3103，维持不变）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L327-356，未修改）

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

### dav_l311（目标版本，3113，维持不变）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L378-407，未修改）

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

### dav_5161（目标版本，5101，**已修改**）

#### DataCopyUB2L1ND2NZImpl — 完整实现（L398-427，**新增**，从 dav_l311 移植）

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
