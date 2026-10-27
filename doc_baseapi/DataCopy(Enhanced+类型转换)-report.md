# DataCopy（Enhanced Local→Local + 类型转换变体）适配报告

> 适配对象：`asc-devkit\include\basic_api\kernel_operator_data_copy_intf.h` L308/L323/L333/L342/L351/L360/L369/L378 的 8 个 DataCopy 变体
> 适配范围：dav_5161（5101/5161/5165/5163 共用实现文件）
> 其余 10 个版本（dav_c100/dav_m200/dav_c220/dav_m300/dav_l300/dav_m310/dav_l310/dav_l311/dav_m510/dav_3510）及声明层/包装层/共享层均未改动

---

## 0. 适配方案与改动映射

| 方案条目 | 落地改动（dav_5161\kernel_operator_data_copy_impl.h） |
|---|---|
| 1. V1 5101 补充 DataCopyUB2UBIntf 实现 | 新增 `DataCopyUB2UBIntf`（L239-247），自参照版本 dav_3510 L1429-1437 **原样移植**（含 `#pragma begin_pipe(V)`/`#pragma end_pipe`、`PrimT<T>` 转型），转调本文件已有完整 `DataCopyUB2UBImpl`（L183-190，手动拼 1017 config + `copy_ubuf_to_ubuf`）。文件为四架构共用，5101/5161/5165/5163 同时生效 |
| 2. V1 5101 的 DataCopyUB2L0CIntf 打桩 | 新增 `DataCopyUB2L0CIntf` 桩（L231-237），按统一格式 `ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from UB(CO2) to L0C Buffer(CO1)")` |
| 3. V1 解决 5101/5161/5165/5163 的编译失败 | ① 上述新增 UB2UBIntf 解决四架构 UB→UB 路径编译失败；② 新增 UB2L0CIntf 桩解决四架构 UB→L0C 路径编译失败；③ `DataCopyL12UBIntf` 原宏控仅 5101，**扩展至 5101/5161/5165/5163**（L221-222），复用文件内已有完整 `DataCopyL12UBImpl`（FIX 指令族，L200-219），解决 5161/5165/5163 的 L1→UB 路径编译失败（方案未要求对该路径打桩） |
| 4. 5101 没有 L0C，UB 场景继续维持打桩 | UB→L0C 经新增 Intf 桩维持不可用；既有 L0C 相关桩（`DataCopyL12L0CImpl`/`DataCopyL0C2UBImpl`/`DataCopyUB2L0CImpl`，ASSERT 风格）按"已存在代码不做修改"要求原样保留（现 L743-755、L770-775） |

修改摘要：单文件 3 处改动（1 处宏控扩展 + 2 处新增函数，共 +19 行），既有函数体零改动。

---

## 1. 适配后支持矩阵

图例：**✓** = 完整支持；**桩** = 编译通过但运行时静默桩（CPU 调试构建报错终止，设备构建无操作）；**AIC✓** = 仅 AIC 核完整（VEC 核为桩）；**声明不可见** = 该架构上该重载不存在（调用即编译失败）；**编译失败** = 路径经 Intf 符号缺失，实例化即编译失败。**【修改】** = 本次适配改动；未标注均为"维持"。

| 版本 | V1 UB→L1 | V1 UB→UB | V1 UB→L0C | V1 L1→UB | V1 L1→L0C | V1 L0C→UB | V2 bf16←f32 | V3 f16←f32 | V4~V7 s16/s8/u8/f16←s32 | V8 f32←f16 |
|---|---|---|---|---|---|---|---|---|---|---|
| 1001（dav_c100） | ✓ | ✓ | ✓ | ✓ | 桩 | ✓ | 声明不可见 | L1→L0C 桩；L0C→UB ✓ | ✓ | ✓ |
| 2002（dav_m200） | AIC✓ | AIC✓ | AIC✓ | AIC✓ | 桩 | AIC✓（int8 同类型桩） | 声明不可见 | L1→L0C 桩；L0C→UB AIC✓ | AIC✓ | AIC✓ |
| 2201（dav_c220） | ✓（软件模拟） | ✓ | 桩 | 桩（设备静默） | **✓** | 桩 | **✓** | L1→L0C ✓；L0C→UB 桩 | 桩 | 桩 |
| 3002（dav_m300） | ✓ | ✓ | 桩 | ✓ | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 3003（dav_l300） | ✓ | ✓ | 桩 | ✓ | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 3102（dav_m310） | ✓ | ✓ | 桩 | ✓ | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 3103（dav_l310） | ✓ | ✓ | 桩 | ✓ | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 3113（dav_l311） | ✓ | ✓ | 桩 | ✓ | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 5102（dav_m510） | ✓ | ✓ | 桩 | ✓ | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 5101（dav_5161） | ✓ | 编译失败 → ✓【修改：新增 UB2UBIntf】 | 编译失败 → 桩【修改：新增 UB2L0CIntf 桩】 | ✓（维持，Intf 宏控扩展） | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 5161/5165/5163（dav_5161） | ✓ | 编译失败 → ✓【修改：新增 UB2UBIntf】 | 编译失败 → 桩【修改：新增 UB2L0CIntf 桩】 | 编译失败 → ✓【修改：L12UBIntf 宏控扩展】 | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |
| 3510（dav_3510，参照） | ✓ | ✓ | 桩 | 桩（Intf 桩） | 桩 | 桩 | 声明不可见 | 桩 | 桩 | 桩 |

适配效果：
- 四个架构（5101/5161/5165/5163）V1 全部 6 条路径均编译通过，其中 4 条为完整支持（UB→L1、UB→UB、L1→UB、含 5101 原有的 FIX 指令族路径），2 条因无 L0C 硬件维持桩（UB→L0C、L1→L0C、L0C→UB 中后两者为既有桩）。
- V2~V8 类型转换变体在四个架构上维持桩/声明不可见，未做改动（方案未涉及）。

---

## 2. 修改的文件清单

| 文件 | 改动类型 | 改动内容 |
|---|---|---|
| `asc-devkit\impl\basic_api\dav_5161\kernel_operator_data_copy_impl.h` | 修改（+19 行，1030 → 1049 行） | ① L221-222：`DataCopyL12UBIntf` 宏控由 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)` 扩展为 `#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))`（函数体不变）② L231-237：新增 `DataCopyUB2L0CIntf` 桩（`ASCENDC_REPORT_NOT_SUPPORT` 格式）③ L239-247：新增 `DataCopyUB2UBIntf` 实现（自 dav_3510 L1429-1437 原样移植，含 `#pragma begin_pipe(V)`） |

未修改的文件（维持）：
- 声明层 `include\basic_api\kernel_operator_data_copy_intf.h`（8 个声明均无版本宏控变化需求）
- 包装层 `impl\basic_api\kernel_operator_data_copy_intf_impl.h`（V1 分发逻辑不依赖被调函数的版本差异）
- 共享层 `impl\basic_api\kernel_operator_data_copy_base_impl.h`（版本分发宏控不变）
- 其余 10 个版本目录 dav_c100 / dav_m200 / dav_c220 / dav_m300 / dav_l300 / dav_m310 / dav_l310 / dav_l311 / dav_m510 / dav_3510 的全部文件

改动正确性验证（已执行）：
1. 新增两函数签名与包装层调用点完全匹配（intf_impl.h L1164 `DataCopyUB2L0CIntf(dst, src, intriParams, enhancedParams)` 4 参；L1167 `DataCopyUB2UBIntf(dst, src, intriParams)` 3 参）；
2. `DataCopyUB2UBImpl` 定义于 L183-190，先于 `DataCopyUB2UBIntf`（L239-247），调用可解析；
3. 全文件扫描（L1-1030 原文件）确认无 `DataCopyUB2UBIntf`/`DataCopyUB2L0CIntf` 重复定义；
4. `ASCENDC_REPORT_NOT_SUPPORT`（kernel_log.h）与 `PrimT<T>`（kernel_tensor.h，经 base_impl.h L24 先于 dav 包含）在本翻译单元可用（文件内已使用 ASCENDC_ASSERT/KERNEL_LOG，兄弟版本同位置使用 PrimT<T>）；
5. 宏控扩展写法与文件内既有 4 架构/3 架构宏控风格一致（如原 L473/L533/L602/L662，现为 L492/L552/L621/L681）；既有 L0C 桩区域（L743-755、L770-775）内容零改动，仅行号整体 +19；
6. 大括号配对、`#pragma begin_pipe(V)`/`#pragma end_pipe` 配对、4 空格缩进均已回读核对。

---

## 3. 适配后各版本原始代码

以下为 8 个变体调用链上的 9 个底层函数（DataCopyUB2UBImpl / DataCopyUB2L1Impl / DataCopyL12UBImpl / DataCopyL12UBIntf / DataCopyUB2L0CImpl / DataCopyUB2L0CIntf / DataCopyUB2UBIntf / DataCopyL12L0CImpl / DataCopyL0C2UBImpl）在各版本的当前代码，行号为适配后实际行号。每函数标注是否修改。

### 3.1 dav_l300（3003）——全部未修改

```cpp
// DataCopyUB2UBImpl（L135-141）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_ubuf(
        (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyUB2L1Impl（L143-149）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L151-157）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBIntf（L871-876）【未修改·维持】
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L609-614）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to l0c"); });
}

// DataCopyUB2L0CIntf（L853-860）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams, enhancedParams);
}

// DataCopyUB2UBIntf（L862-869）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L590-595）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to l0c"); });
}

// DataCopyL0C2UBImpl（L602-607）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf"); });
}
```

### 3.2 dav_l310（3103）——全部未修改

```cpp
// DataCopyUB2UBImpl（L165-172）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // 当前#1018实现有问题，需要手动拼1017的config值
    uint64_t config = ((uint64_t)intriParams.dstStride << 48) + (((uint64_t)intriParams.srcStride) << 32) +
                      (((uint64_t)intriParams.blockLen) << 16) + (((uint64_t)intriParams.blockCount) << 4);
    copy_ubuf_to_ubuf((__ubuf__ void*)dst, (__ubuf__ void*)src, config);
}

// DataCopyUB2L1Impl（L174-180）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L182-188）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBIntf（L916-921）【未修改·维持】
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L540-545）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to l0c on this version"); });
}

// DataCopyUB2L0CIntf（L898-905）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams, enhancedParams);
}

// DataCopyUB2UBIntf（L907-914）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L521-526）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to l0c on this version"); });
}

// DataCopyL0C2UBImpl（L533-538）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf on this version"); });
}
```

### 3.3 dav_l311（3113）——全部未修改

```cpp
// DataCopyUB2UBImpl（L165-172）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // 当前#1018实现有问题，需要手动拼1017的config值
    uint64_t config = ((uint64_t)intriParams.dstStride << 48) + (((uint64_t)intriParams.srcStride) << 32) +
                      (((uint64_t)intriParams.blockLen) << 16) + (((uint64_t)intriParams.blockCount) << 4);
    copy_ubuf_to_ubuf((__ubuf__ void*)dst, (__ubuf__ void*)src, config);
}

// DataCopyUB2L1Impl（L174-180）、DataCopyL12UBImpl（L182-188）【未修改·维持·完整】
// 与 dav_l310 L174-188 逐字相同（copy_ubuf_to_cbuf / copy_cbuf_to_ubuf 直接指令）

// DataCopyL12UBIntf（L967-972）【未修改·维持】
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L591-596）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to l0c on this version"); });
}

// DataCopyUB2L0CIntf（L949-956）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams, enhancedParams);
}

// DataCopyUB2UBIntf（L958-965）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L572-577）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to l0c on this version"); });
}

// DataCopyL0C2UBImpl（L584-589）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf on this version"); });
}
```

### 3.4 dav_5161（5101/5161/5165/5163）——本次适配唯一修改的版本

```cpp
// DataCopyUB2UBImpl（L183-190）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // 当前#1018实现有问题，需要手动拼1017的config值
    uint64_t config = ((uint64_t)intriParams.dstStride << 48) + (((uint64_t)intriParams.srcStride) << 32) +
                      (((uint64_t)intriParams.blockLen) << 16) + ((uint64_t)intriParams.blockCount);
    copy_ubuf_to_ubuf((__ubuf__ void*)dst, (__ubuf__ void*)src, config);
}

// DataCopyUB2L1Impl（L192-198）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L200-219）【未修改·维持·完整·FIX 指令族】
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

// DataCopyL12UBIntf（L221-229）【本次修改：宏控由仅 5101 扩展为 5101/5161/5165/5163，函数体不变】
#if defined(__NPU_ARCH__) && \
    ((__NPU_ARCH__ == 5101) || (__NPU_ARCH__ == 5161) || (__NPU_ARCH__ == 5165) || (__NPU_ARCH__ == 5163))
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#endif

// DataCopyUB2L0CIntf（L231-237）【本次新增：桩（方案条目 2；统一 ASCENDC_REPORT_NOT_SUPPORT 格式）】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from UB(CO2) to L0C Buffer(CO1)");
}

// DataCopyUB2UBIntf（L239-247）【本次新增：实现（方案条目 1；自 dav_3510 L1429-1437 原样移植）】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl(
        (__ubuf__ PrimT<T>*)dstLocal.GetPhyAddr(), (__ubuf__ PrimT<T>*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyUB2L0CImpl（L770-775）【未修改·维持·桩（方案条目 4：无 L0C，UB 场景维持打桩）】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy UB to L0C on current device");
}

// DataCopyL12L0CImpl（L743-748）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy L1 to L0C on current device");
}

// DataCopyL0C2UBImpl（L750-755）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy L0C to UB on current device");
}
```

适配前对照（dav_5161 原状）：
- `DataCopyL12UBIntf` 宏控为 `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)`（仅 5101 可见，5161/5165/5163 的 V1 L1→UB 路径编译失败）；
- `DataCopyUB2L0CIntf`、`DataCopyUB2UBIntf` 全文件未定义（四架构 V1 UB→L0C / UB→UB 路径编译失败）。

### 3.5 dav_c220（2201）——全部未修改

```cpp
// DataCopyUB2UBImpl（L119-133）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(src)) % ONE_BLK_SIZE == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "src address should be 32B aligned \n"));
        ASCENDC_DEBUG_ASSERT(
            (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(dst)) % ONE_BLK_SIZE == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dst address should be 32B aligned \n"));
        copy_ubuf_to_ubuf(
            (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
            intriParams.srcStride, intriParams.dstStride);
    }
}

// DataCopyUB2L1Impl（L135-189）【未修改·维持·完整（TSCM 软件模拟）】
template <typename T>
__aicore__ inline void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // Add for TSCM AIV: Copy buffer into workspace, and send message to AIC
    ASCENDC_ASSERT((GetTPipePtr() != nullptr), { KERNEL_LOG(KERNEL_ERROR, "tpipe ptr can not be nullptr"); });
    ASCENDC_ASSERT((dst != nullptr), { KERNEL_LOG(KERNEL_ERROR, "dst ptr can not be nullptr"); });
    ASCENDC_ASSERT((src != nullptr), { KERNEL_LOG(KERNEL_ERROR, "src ptr can not be nullptr"); });
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (GetKfcClient() != nullptr),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "DataCopy from UB to L1 is software-emulated on this device and works with Matmul API. Use "
                "REGISTER_MATMUL_OBJ to enable Matmul API first.\n"));
        ASCENDC_DEBUG_ASSERT(
            (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(src)) % ONE_BLK_SIZE == 0),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR, "src address should be 32B aligned \n"));
        // 1.获取GM的地址
        uint32_t tensorSize = intriParams.blockCount * intriParams.blockLen * 32;
        int32_t ubAddr = -1;
#if ASCENDC_CPU_DEBUG
        uint64_t absSrc = (uint8_t*)src - (uint8_t*)(GetTPipePtr()->GetBaseAddr((int8_t)TPosition::VECIN));
        ASCENDC_ASSERT((static_cast<uint64_t>(absSrc) >= 0 && static_cast<uint64_t>(absSrc) < TOTAL_UB_SIZE), {
            KERNEL_LOG(KERNEL_ERROR, "abs src is 0x%lx, which should be in range of [0, %u)", absSrc, TOTAL_UB_SIZE);
        });
        GM_ADDR gmAddr = (GetKfcClient()->ubStart) + static_cast<uint64_t>(absSrc);
#else
        GM_ADDR gmAddr = (GetKfcClient()->AllocUB(tensorSize, ubAddr));
#endif
        event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
        SetFlag<HardEvent::V_MTE3>(eventID);
        WaitFlag<HardEvent::V_MTE3>(eventID);
        // 2.进行对等拷贝ub->GM
        copy_ubuf_to_gm(
            (__gm__ void*)gmAddr, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
            intriParams.srcStride, intriParams.srcStride);

        // 3.发送消息,从GM拷贝至L1
#if ASCENDC_CPU_DEBUG
        uint8_t* aicBaseTSCMAddr = GetTPipePtr()->GetBaseAddr(int8_t(TPosition::TSCM));
        ASCENDC_ASSERT(
            (aicBaseTSCMAddr != nullptr), { KERNEL_LOG(KERNEL_ERROR, "aicBaseTSCMAddr can not be nullptr"); });
        uint64_t l1AddrDst = (uint8_t*)dst - (uint8_t*)aicBaseTSCMAddr;
        ASCENDC_ASSERT((l1AddrDst < TOTAL_L1_SIZE), {
            KERNEL_LOG(
                KERNEL_ERROR, "l1AddrDst is 0x%lx, which should be in range of [0, %u)", l1AddrDst, TOTAL_L1_SIZE);
        });
        ScmDataCopyMsg((__cbuf__ void*)l1AddrDst, (__gm__ void*)gmAddr, intriParams, ubAddr);
#else
#ifndef __NO_KFC_SERVER__
        ScmDataCopyMsg((__cbuf__ void*)dst, (__gm__ void*)gmAddr, intriParams, ubAddr);
#endif
#endif
    }
}

// DataCopyL12UBImpl（L245-251）【未修改·维持·桩（设备构建下空函数体，静默）】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    ReportNotSupport(false, "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)");
#endif
}

// DataCopyL12UBIntf（L1170-1175）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L432-437）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ReportNotSupport(false, "DataCopy from GM(CO2) to L0C Buffer(CO1)");
}

// DataCopyUB2L0CIntf（L1177-1184）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

// DataCopyUB2UBIntf（L1186-1193）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L400-420）【未修改·维持·完整（全版本唯一 L1→L0C 实路径）】
template <typename T, typename U>
__aicore__ inline __inout_pipe__(MTE1) void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<half, half>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, float>,
                Tuple<bfloat16_t, bfloat16_t>, Tuple<int32_t, int32_t>, Tuple<uint32_t, uint32_t>>()),
            "Failed to check dtype in DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1), current api support "
            "dtype combination is src: half, dst: half; src: float, dst: half / bfloat16_t / float; src: bfloat16_t, "
            "dst: bfloat16_t; src: int32_t, dst: int32_t; src: uint32_t, dst: uint32_t.");
        ASCENDC_CHECK_TENSOR_PTR_ALIGN(
            src, TPosition::A1, ONE_BLK_SIZE, "srcLocal", "DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1)");
        ASCENDC_CHECK_TENSOR_PTR_ALIGN(
            dst, TPosition::CO1, 1024, "dstLocal", "DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1)");
        copy_matrix_cbuf_to_cc(
            (__cc__ T*)dst, (__cbuf__ U*)src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
            intriParams.dstStride);
    }
}

// DataCopyL0C2UBImpl（L425-430）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ReportNotSupport(false, "DataCopy from L0C Buffer(CO1) to GM(CO2)");
}
```

### 3.6 dav_c100（1001）——全部未修改

```cpp
// DataCopyUB2UBImpl（L61-67）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_ubuf(
        (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyUB2L1Impl（L69-76）【未修改·维持·完整】
template <typename T>
__aicore__ inline __in_pipe__(MTE3)
    __out_pipe__(MTE3) void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L78-84）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBIntf（L1076-1081）【未修改·维持】
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L909-924）【未修改·维持·完整（blockMode 分发）】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_MATRIX) {
        DataCopyMatrixUB2L0CImpl(dst, src, intriParams, enhancedParams);   // L749-796: copy_matrix_ubuf_to_cc
    } else if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_VECTOR) {
        DataCopyVectorUB2L0CImpl(dst, src, intriParams, enhancedParams);   // L800-847: copy_vector_ubuf_to_cc
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "In DataCopy from UB(CO2) to L0C Buffer(CO1), blockMode should be "
                              "BLOCK_MODE_MATRIX / BLOCK_MODE_VECTOR");
        });
    }
}

// DataCopyUB2L0CIntf（L1058-1066）与 DataCopyUB2UBIntf（L1068-1073）【未修改·维持】（同处一个 #pragma begin_pipe(V) 块）
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L849-854）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1)");
}

// DataCopyL0C2UBImpl（L861-907）【未修改·维持·完整（deqScale SPR 预处理 + blockMode 分发）】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    if ((enhancedParams.deqScale == DeqScale::DEQ8) || (enhancedParams.deqScale == DeqScale::DEQ16)) {
        uint64_t deqScaleSpr = enhancedParams.deqValue;
        if (enhancedParams.isRelu) {
            deqScaleSpr |= 1ULL << 47;
        }
        if (IsSameType<T, int8_t>::value) {
            deqScaleSpr |= 1ULL << 46;
        }
        set_deqscale(deqScaleSpr);
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    } else if ((enhancedParams.deqScale == DeqScale::VDEQ8) || (enhancedParams.deqScale == DeqScale::VDEQ16)) {
        uint64_t deqScaleSpr = enhancedParams.deqTensorAddr >> 5;
        if (enhancedParams.isRelu) {
            deqScaleSpr |= 1ULL << 15;
        }
        set_deqscale(deqScaleSpr);
#if ASCENDC_CPU_DEBUG
        uint64_t deqScaleSprAddr = enhancedParams.deqTensorAddr;
        SetModelDeqTensor((void*)deqScaleSprAddr);
        deqScaleSprAddr = 0;
        if (enhancedParams.isRelu) {
            deqScaleSprAddr = static_cast<uint64_t>(1) << static_cast<uint64_t>(15);
        }
        set_deqscale(deqScaleSprAddr);
#endif
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    }
    if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_MATRIX) {
        DataCopyMatrixL0C2UBImpl(dst, src, intriParams, enhancedParams);   // L370-555: copy_matrix_cc_to_ubuf + CRMODE 分发
    } else if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_VECTOR) {
        DataCopyVectorL0C2UBImpl(dst, src, intriParams, enhancedParams);   // L559-744: copy_vector_cc_to_ubuf + CRMODE 分发
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "In DataCopy from L0C Buffer(CO1) to UB(CO2), blockMode should be "
                              "BLOCK_MODE_MATRIX / BLOCK_MODE_VECTOR");
        });
    }
}
```

### 3.7 dav_m200（2002）——全部未修改

```cpp
// DataCopyUB2UBImpl（L94-111）【未修改·维持·VEC 桩 / AIC 完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
#if defined(__DAV_M200_VEC__)
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from UB(VECIN) to UB(VECCALC), UB(VECCALC) to UB(VECOUT)");
#else
    CheckDataCopyParams(intriParams.blockCount, intriParams.blockLen);
    ASCENDC_DEBUG_ASSERT(
        (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(src)) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "src address should be 32B aligned \n"));
    ASCENDC_DEBUG_ASSERT(
        (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(dst)) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dst address should be 32B aligned \n"));
    copy_ubuf_to_ubuf(
        (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
#endif
}

// DataCopyUB2L1Impl（L113-128）【未修改·维持·VEC 桩 / AIC 完整】
template <typename T>
__aicore__ inline __inout_pipe__(MTE3) void DataCopyUB2L1Impl(
    __cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
#if defined(__DAV_M200_VEC__)
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from UB(VECIN/VECCALC/VECOUT) to L1 Buffer(A1/B1)");
#else
    CheckDataCopyParams(intriParams.blockCount, intriParams.blockLen);
    ASCENDC_DEBUG_ASSERT(
        (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(src)) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "src address should be 32B aligned \n"));
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
#endif
}

// DataCopyL12UBImpl（L130-144）【未修改·维持·VEC 桩 / AIC 完整】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
#if defined(__DAV_M200_VEC__)
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECCALC/VECOUT)");
#else
    CheckDataCopyParams(intriParams.blockCount, intriParams.blockLen);
    ASCENDC_DEBUG_ASSERT(
        (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(dst)) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dst address should be 32B aligned \n"));
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
#endif
}

// DataCopyL12UBIntf（L1414-1419）【未修改·维持】
template <typename T>
__aicore__ inline __inout_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl int8_t 同类型特化（L1234-1239）【未修改·维持·桩】
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ int8_t* dst, __ubuf__ int8_t* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from UB(CO2) to L0C Buffer(CO1) with type int8_t");
}

// DataCopyUB2L0CImpl 模板版（L1241-1259）【未修改·维持·VEC 桩 / AIC 完整】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_DEBUG_ASSERT(
        (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(src)) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "src address should be 32B aligned \n"));
    if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_MATRIX) {
        DataCopyMatrixUB2L0CImpl(dst, src, intriParams, enhancedParams);   // copy_matrix_ubuf_to_cc（VEC 桩 / AIC 完整）
    } else if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_VECTOR) {
        DataCopyVectorUB2L0CImpl(dst, src, intriParams, enhancedParams);   // copy_vector_ubuf_to_cc（VEC 桩 / AIC 完整）
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "In DataCopy from UB(CO2) to L0C Buffer(CO1), blockMode should be "
                              "BLOCK_MODE_MATRIX / BLOCK_MODE_VECTOR");
        });
    }
}

// DataCopyUB2L0CIntf（L1422-1429）与 DataCopyUB2UBIntf（L1431-1436）【未修改·维持】（同处一个 #pragma begin_pipe(V) 块）
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L1156-1161）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1)");
}

// DataCopyL0C2UBImpl int8_t 同类型特化（L1167-1172）【未修改·维持·桩】
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ int8_t* dst, __cc__ int8_t* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L0C Buffer(CO1) to UB(CO2) with type int8_t");
}

// DataCopyL0C2UBImpl 模板版（L1175-1232）【未修改·维持·VEC 桩 / AIC 完整（含 VDEQ/DEQ 标量模式）】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_DEBUG_ASSERT(
        (TransUBAddr<TPosition::VECIN>(reinterpret_cast<uint64_t>(dst)) % ONE_BLK_SIZE == 0),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "dst address should be 32B aligned \n"));
    if ((enhancedParams.deqScale == DeqScale::DEQ8) || (enhancedParams.deqScale == DeqScale::DEQ16)) {
        uint64_t deqScaleSpr = enhancedParams.deqValue;
        if (enhancedParams.isRelu) {
            deqScaleSpr |= 1ULL << 47;
        }
        if (IsSameType<T, int8_t>::value) {
            deqScaleSpr |= 1ULL << 46;
        }
        set_deqscale(deqScaleSpr);
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    } else if (
        (enhancedParams.deqScale == DeqScale::VDEQ8) || (enhancedParams.deqScale == DeqScale::VDEQ16) ||
        (enhancedParams.deqScale == DeqScale::VDEQ)) {
        uint64_t deqScaleSpr = enhancedParams.deqTensorAddr >> 5;
        if (enhancedParams.isRelu) {
            deqScaleSpr |= 1ULL << 15;
        }
        set_deqscale(deqScaleSpr);
#if ASCENDC_CPU_DEBUG
        uint64_t deqScaleSprAddr = enhancedParams.deqTensorAddr;
        SetModelDeqTensor((void*)deqScaleSprAddr);
        deqScaleSprAddr = 0;
        if (enhancedParams.isRelu) {
            deqScaleSprAddr = static_cast<uint64_t>(1) << static_cast<uint64_t>(15);
        }
        set_deqscale(deqScaleSprAddr);
#endif
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    } else if (enhancedParams.deqScale == DeqScale::DEQ) { // half -> half / int32_t -> half
        uint64_t deqScaleSpr = enhancedParams.deqValue;
        set_deqscale(deqScaleSpr);
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    }
    if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_MATRIX) {
        DataCopyMatrixL0C2UBImpl(dst, src, intriParams, enhancedParams);   // copy_matrix_cc_to_ubuf（VEC 桩 / AIC 完整）
    } else if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_VECTOR) {
        DataCopyVectorL0C2UBImpl(dst, src, intriParams, enhancedParams);   // copy_vector_cc_to_ubuf（VEC 桩 / AIC 完整）
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "In DataCopy from L0C Buffer(CO1) to UB(CO2), blockMode should be "
                              "BLOCK_MODE_MATRIX / BLOCK_MODE_VECTOR");
        });
    }
}
```

### 3.8 dav_m300（3002）——全部未修改

```cpp
// DataCopyUB2UBImpl（L60-66）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_ubuf(
        (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyUB2L1Impl（L68-75）【未修改·维持·完整】
template <typename T>
__aicore__ inline __in_pipe__(MTE3)
    __out_pipe__(MTE3) void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L77-83）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBIntf（L993-998）【未修改·维持】
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L786-792）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(
        (false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to l0c on current deviceB1"); });
}

// DataCopyUB2L0CIntf（L1000-1007）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

// DataCopyUB2UBIntf（L1009-1016）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L766-771）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy from cbuf to l0c on current device");
}

// DataCopyL0C2UBImpl（L778-784）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(
        (false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf on current deviceB1"); });
}
```

### 3.9 dav_m310（3102）——全部未修改

```cpp
// DataCopyUB2UBImpl（L40-46）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_ubuf(
        (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyUB2L1Impl（L48-55）【未修改·维持·完整】
template <typename T>
__aicore__ inline __in_pipe__(MTE3)
    __out_pipe__(MTE3) void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L57-63）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBIntf（L919-924）【未修改·维持】
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L621-626）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to l0c on current device"); });
}

// DataCopyUB2L0CIntf（L901-908）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

// DataCopyUB2UBIntf（L910-917）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L602-607）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy from cbuf to l0c on current device");
}

// DataCopyL0C2UBImpl（L614-619）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf on current device"); });
}
```

### 3.10 dav_m510（5102）——全部未修改

```cpp
// DataCopyUB2UBImpl（L543-548）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    CopyUbufToUbuf(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
}

// DataCopyUB2L1Impl（L550-556）【未修改·维持·完整】
template <typename T>
__aicore__ inline __in_pipe__(MTE3)
    __out_pipe__(MTE3) void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    CopyUbufToCbuf(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBImpl（L712-717）【未修改·维持·完整】
// SRC: L1
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    CopyCbufToUbuf(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
}

// DataCopyL12UBIntf（L1169-1174）【未修改·维持】
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}

// DataCopyUB2L0CImpl（L704-709）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to L0C on current device"); });
}

// DataCopyUB2L0CIntf（L1176-1184）【未修改·维持·经桩】
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dstLocal.GetPhyAddr(), (__ubuf__ PrimT<T>*)srcLocal.GetPhyAddr(), intriParams,
        enhancedParams);
}

// DataCopyUB2UBIntf（L1186-1194）【未修改·维持】
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl(
        (__ubuf__ PrimT<T>*)dstLocal.GetPhyAddr(), (__ubuf__ PrimT<T>*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L735-740）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to L0C on current device"); });
}

// DataCopyL0C2UBImpl（L743-752）【未修改·维持·桩（带 FixPipe 迁移指引）】
// SRC: L0C
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "unsupported data copy from l0c to ubuf on current device, please use FixPipe function instead");
    });
}
```

### 3.11 dav_3510（3510，参照版本）——全部未修改（移植源）

```cpp
// 底层包装（L174-208，供 Impl 调用）【未修改·维持】
template <typename T>
__aicore__ inline void CopyUbufToUbuf(
    __ubuf__ T* dst, __ubuf__ T* src, const uint16_t blockCount, const uint16_t blockLen, const uint16_t srcStride,
    const uint16_t dstStride)
{
    if ASCEND_IS_AIV {
        copy_ubuf_to_ubuf((__ubuf__ void*)dst, (__ubuf__ void*)src, 0, blockCount, blockLen, srcStride, dstStride);
    }
}

// DataCopyUB2UBImpl（L607-612）【未修改·维持·完整】
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    CopyUbufToUbuf(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
}

// DataCopyUB2L1Impl（L617-674）【未修改·维持·完整（条件宏控：直连或 TSCM 软件模拟）】
template <typename T>
__aicore__ inline __in_pipe__(MTE3)
    __out_pipe__(MTE3) void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
#if KFC_C310_SSBUF == 1 || __MIX_CORE_AIC_RATION__ != 1
    CopyUbufToCbuf(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
#else
    // Add for TSCM AIV: Copy buffer into workspace, and send message to AIC
    //（AllocUB → V_MTE3 事件同步 → CopyUbufToGmAlignV2 → ScmDataCopyMsg，含 g_gm_overflow_check 溢出检查）
#endif
}

// DataCopyL12UBImpl（L876-890）【未修改·维持·条件完整】
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

// DataCopyL12UBIntf（L1410-1417）【未修改·维持·桩（Impl 条件完整但 Intf 无条件桩）】
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) on current device");
    });
}

// DataCopyUB2L0CImpl（L868-873）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to L0C on current device"); });
}

// DataCopyUB2L0CIntf（L1419-1427）【未修改·维持·经桩】—— 本次 dav_5161 新增 DataCopyUB2UBIntf 的移植源（同函数组）
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ PrimT<T>*)dstLocal.GetPhyAddr(), (__ubuf__ PrimT<T>*)srcLocal.GetPhyAddr(), intriParams,
        enhancedParams);
}

// DataCopyUB2UBIntf（L1429-1437）【未修改·维持】—— 本次 dav_5161 新增 DataCopyUB2UBIntf 的移植源（逐字一致）
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl(
        (__ubuf__ PrimT<T>*)dstLocal.GetPhyAddr(), (__ubuf__ PrimT<T>*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// DataCopyL12L0CImpl（L908-913）【未修改·维持·桩】
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to L0C on current device"); });
}

// DataCopyL0C2UBImpl（L916-925）【未修改·维持·桩（带 FixPipe 迁移指引）】
// SRC: L0C
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "unsupported data copy from l0c to ubuf on current device, please use FixPipe function instead");
    });
}
```

---

## 4. 遗留说明

1. `DataCopyL12UBIntf` 宏控扩展是方案条目 3（解决四架构编译失败）对 L1→UB 路径的落地方式：复用文件内既有完整 `DataCopyL12UBImpl`（FIX 指令族）。方案仅明确要求对 `DataCopyUB2L0CIntf` 打桩，未要求 L1→UB 打桩，故按完整实现暴露。
2. 既有 ASSERT 风格桩（dav_5161 的 L12L0CImpl/L0C2UBImpl/UB2L0CImpl 等）按"已存在代码不做修改"要求全部原样保留；本次唯一新增桩（`DataCopyUB2L0CIntf`）按要求使用 `ASCENDC_REPORT_NOT_SUPPORT(false, ...)` 统一格式。
3. V2~V8 类型转换变体在 5101 家族维持桩（无 L0C 硬件，依赖 L0C↔UB 直达搬运的转换路径无法落地），方案未涉及，未做改动。
