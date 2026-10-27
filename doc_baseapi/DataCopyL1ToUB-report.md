# DataCopyL1ToUB 接口适配报告

## 适配后支持矩阵

| 版本 | Arch | V1 (count) | V2 (repeatParams) | DataCopyL12UBImpl | DataCopyL12UBIntf（通用DataCopy L1→UB 路径） |
|------|------|------------|-------------------|-------------------|---------------------------------------------|
| dav_l300 | 3003 | 空函数体 → **完整** | 空函数体 → **完整** | 完整（维持） | 完整（维持） |
| dav_l310 | 3103 | 空函数体 → **完整** | 空函数体 → **完整** | 完整（维持） | 完整（维持） |
| dav_l311 | 3113 | 空函数体 → **完整** | 空函数体 → **完整** | 完整（维持） | 完整（维持） |
| dav_5161 | 5101 | 空函数体 → **完整** | 空函数体 → **完整** | 完整（维持） | 未定义 → **完整** |
| dav_5161 | 5161 | 空函数体（维持） | 空函数体（维持） | 完整（维持） | 未定义（维持） |
| dav_5161 | 5165 | 空函数体（维持） | 空函数体（维持） | 完整（维持） | 未定义（维持） |
| dav_5161 | 5163 | 空函数体（维持） | 空函数体（维持） | 完整（维持） | 未定义（维持） |
| dav_3510 | 3510（参照） | 完整（维持） | 完整（维持） | 完整（维持） | 桩（维持） |

> **适配要点**：
> 1. V1/V2 的函数体守卫从仅 3510 扩展至 3003/3103/3113/5101，消除空函数体（静默无操作）问题
> 2. 3003/3103/3113/5101 的底层 `DataCopyL12UBImpl` 签名为 `template <typename T>`（不支持 subBlockId），与 3510 的 `template <typename T, uint8_t subBlockId>` 区分：V2 中通过 `#if/#elif` 分支区分调用方式（3510 传 `<PrimType, subBlockId>`，其余传 `<PrimType>`）；V1 的 subBlockId 模板参数仅透传给 V2，由 V2 按 arch 忽略
> 3. 5101 新增 `DataCopyL12UBIntf`（参照 l300 定义），修复通用 `DataCopy(L1→UB)` 路径的编译缺失；5161/5165/5163 不在本次目标范围内，维持不变

## 修改的文件清单

| 文件 | 修改位置 | 修改内容 |
|------|---------|---------|
| `impl/basic_api/kernel_operator_data_copy_intf_impl.h` | L535-555（V2） | 3510 的 `#if` 分支后增加 `#elif 3003\|\|3103\|\|3113\|\|5101` 分支：保持相同的溢出检查与 32B 对齐检查，调用 `DataCopyL12UBImpl<PrimType>`（不带 subBlockId） |
| `impl/basic_api/kernel_operator_data_copy_intf_impl.h` | L930-950（V1） | `#if` 守卫由仅 `3510` 扩展为 `3510\|\|3003\|\|3103\|\|3113\|\|5101`，函数体不变（count→repeatParams 转换后委托 V2） |
| `impl/basic_api/dav_5161/kernel_operator_data_copy_impl.h` | L221-228（新增） | 新增 `DataCopyL12UBIntf`，`#if __NPU_ARCH__ == 5101` 守卫，参照 l300（L871-876）的定义，委托给同文件已有的 `DataCopyL12UBImpl` |

未修改的文件：`include/basic_api/kernel_operator_data_copy_intf.h`（声明无 arch 守卫，无需变更）、`impl/basic_api/kernel_operator_data_copy_base_impl.h`（分发 include 已覆盖所有目标 arch）、dav_l300/l310/l311/dav_3510 的实现文件（维持不变）。

## 适配后各版本原始代码

### intf_impl.h — DataCopyL1ToUB V2 (repeatParams)（L535-555，**已修改**）

```cpp
template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    using PrimType = PrimT<T>;
    ASCENDC_REPORT_OVERFLOW_MEM(CheckDataCopyTensorSizeOverflow(dst, src, repeatParams));
    CheckTensorAlign<T>(dst, ONE_BLK_SIZE, "dst", "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"); // 32B align
    CheckTensorAlign<T>(src, ONE_BLK_SIZE, "src", "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"); // 32B align
    DataCopyL12UBImpl<PrimType, subBlockId>(
        (__ubuf__ PrimType*)dst.GetPhyAddr(), (__cbuf__ PrimType*)src.GetPhyAddr(), repeatParams);
#elif (defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113) || (__NPU_ARCH__ == 5101)))
    using PrimType = PrimT<T>;
    ASCENDC_REPORT_OVERFLOW_MEM(CheckDataCopyTensorSizeOverflow(dst, src, repeatParams));
    CheckTensorAlign<T>(dst, ONE_BLK_SIZE, "dst", "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"); // 32B align
    CheckTensorAlign<T>(src, ONE_BLK_SIZE, "src", "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"); // 32B align
    DataCopyL12UBImpl<PrimType>(
        (__ubuf__ PrimType*)dst.GetPhyAddr(), (__cbuf__ PrimType*)src.GetPhyAddr(), repeatParams);
#endif
}
```

> **修改**：新增 `#elif` 分支。3510 分支保持原样（`DataCopyL12UBImpl<PrimType, subBlockId>`）；新分支用于 3003/3103/3113/5101（`DataCopyL12UBImpl<PrimType>`，不带 subBlockId）。检查逻辑（溢出检查、32B 对齐检查）从 3510 完整移植。

### intf_impl.h — DataCopyL1ToUB V1 (count)（L930-950，**已修改**）

```cpp
template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
{
#if (defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 3003) || (__NPU_ARCH__ == 3103) || (__NPU_ARCH__ == 3113) || \
        (__NPU_ARCH__ == 5101)))
    using PrimType = PrimT<T>;
    ASCENDC_ASSERT((count % AscendCUtils::GetC0Count(sizeof(PrimType)) == 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to "
            "check count value in DataCopy from LocalTensor to LocalTensor, count * sizeof(T) must be 32B align, "
            "current count value is %u. In NPU mode, no error is reported. The value is rounded down by 32B.",
            count);
    });
    ASCENDC_REPORT_OVERFLOW_MEM((CheckDataCopyTensorSizeOverflow(dst, src, count)));
    struct DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(PrimType));
    DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams);
#endif
}
```

> **修改**：仅扩展 `#if` 守卫（3510 → 3510||3003||3103||3113||5101）。函数体不变：count→blockLen 转换逻辑与 arch 无关，委托 V2（`DataCopyL1ToUB<T, subBlockId>`），subBlockId 的 arch 差异由 V2 内部分支处理。

### dav_3510 — DataCopyL12UBImpl（L876-890，未修改，维持）

```cpp
// SRC: L1
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
#if __MIX_CORE_AIC_RATION__ != 1
    static_assert(subBlockId < 2, "subBlockId must be less than 2 when AIC:AIV ratio is 1:2");
    CopyCbufToUbuf<T, subBlockId>(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
#else
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(
            KERNEL_ERROR, "unsupported data copy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) on current device when "
                          "AIC:AIV ratio is 1:1");
    });
#endif
}
```

> 唯一带 `subBlockId` 模板参数的版本，供 3510 的 V2 `#if` 分支调用。

### dav_3510 — DataCopyL12UBIntf（L1410-1417，未修改，维持桩）

```cpp
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) on current device");
    });
}
```

### dav_5161 — DataCopyL12UBImpl（L200-219，未修改，维持）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    constexpr uint32_t POS_LOOP3_SIZE = 40;
    constexpr uint32_t POS_MODE = 61;
    constexpr uint32_t POS_LOOP4_SRC_STRIDE = 48;

    uint64_t xm = 0;
    xm |= ((uint64_t)intriParams.blockLen * 32 / sizeof(T)) << POS_LOOP3_SIZE;
    xm |= (uint64_t)fixp_trans_mode_t::NORMAL_DMA << POS_MODE;

    uint64_t xt = ((uint64_t)intriParams.blockLen + (uint64_t)intriParams.srcStride);

    uint64_t config = 0;
    config |= (uint64_t)intriParams.blockCount;
    config |= ((uint64_t)intriParams.blockLen + (uint64_t)intriParams.dstStride) << POS_LOOP4_SRC_STRIDE;

    set_fixp_nz_para(config);
    fix_cbuf_to_ubuf((__ubuf__ int8_t*)dst, (__cbuf__ int8_t*)src, xm, xt);
}
```

### dav_5161 — DataCopyL12UBIntf（L221-228，**新增**，仅 5101）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#endif
```

> **新增**：参照 l300 的 Intf 定义（无 pipe 属性），提取 LocalTensor 物理地址后委托给同文件已有的 `DataCopyL12UBImpl`。`#if __NPU_ARCH__ == 5101` 守卫，5161/5165/5163 维持未定义状态不变。修复通用 `DataCopy(L1→UB)` 路径（`intf_impl.h:402`）在 5101 上的编译缺失。

### dav_l300 — DataCopyL12UBImpl（L151-157，未修改，维持）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}
```

### dav_l300 — DataCopyL12UBIntf（L871-876，未修改，维持，本次适配的参照定义）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
```

### dav_l310 — DataCopyL12UBImpl（L182-188，未修改，维持）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}
```

### dav_l310 — DataCopyL12UBIntf（L916-921，未修改，维持）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
```

### dav_l311 — DataCopyL12UBImpl（L182-188，未修改，维持）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}
```

### dav_l311 — DataCopyL12UBIntf（L967-972，未修改，维持）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
```
