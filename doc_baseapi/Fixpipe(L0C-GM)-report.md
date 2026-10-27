# 适配报告：Fixpipe(L0C→GM) 接口

## 适配方案回顾

1. 增加 l310 的声明（intf.h V220 arch 守卫增加 3103）
2. 310 使用的本地参数结构从 `FixpipeParamsL311` 换为 `FixpipeParamsL310`
3. L0C→UB 5101 同 l300，打桩

## 适配后支持矩阵

### 8 个目标接口（L92/97/120/125/146/151/170/175）

| 行号 | 变体 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|------|
| L92 | V1 V220 L0C→GM | 完整（维持） | 未声明 → **完整** | 完整（维持） | 未声明（维持） | 完整（维持） |
| L97 | V2 V220 L0C→GM deq | 完整（维持） | 未声明 → **完整** | 完整（维持） | 未声明（维持） | 完整（维持） |
| L120 | V3 M300 L0C→GM | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） |
| L125 | V4 M300 L0C→GM deq | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） |
| L146 | V5 M310 L0C→GM | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） |
| L151 | V6 M310 L0C→GM deq | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） |
| L170 | V7 Arch3510 L0C→GM | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 完整（维持） |
| L175 | V8 Arch3510 L0C→GM deq | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 完整（维持） |

### 同守卫块受影响路径（intf.h L79-103 V220 块整体对 3103 可见）

| 行号 | 变体 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|------|
| L79 | V220 L0C→L1 | 完整（维持） | 未声明 → **完整** | 完整（维持） | 未声明（维持） | 完整（维持） |
| L84 | V220 L0C→L1 deq | 完整（维持） | 未声明 → **完整** | 完整（维持） | 未声明（维持） | 完整（维持） |

### 实现函数层（FixpipeL0C2UBImpl，V220 参数）

| 函数 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|
| `FixpipeL0C2UBImpl(V220, 3参)` | 桩（维持） | 桩（维持） | 桩（维持） | 未定义 → **桩** | 完整（维持） |
| `FixpipeL0C2UBImpl(V220, 4参)` | 桩（维持） | 桩（维持） | 桩（维持） | 未定义 → **桩** | 完整（维持） |

> 注：5101 仍不在 intf.h V220 守卫中，公共 API 不可达；本次按方案补充 `FixpipeL0C2UBImpl` 桩函数（防御性打桩，与 l300 打桩路径一致）。

### 适配前后 V220 arch 守卫对比

- 适配前（intf.h L76-77）：`2201 || 3003 || 3113 || 3510 || 5102`
- 适配后（intf.h L77-78）：`2201 || 3003 || 3103 || 3113 || 3510 || 5102`

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `include/basic_api/kernel_operator_fixpipe_intf.h` | L77-78: V220 arch 守卫增加 `__NPU_ARCH__ == 3103` |
| `impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h` | 全文件重命名 `ParamsL311` → `ParamsL310`（31 处 = `FixpipeParamsL311`→`FixpipeParamsL310` 23 处 + `TransformParamsL311`→`TransformParamsL310` 8 处；函数名 `TransFixpipeParamsV220ToFixpipeParamsL311` → `TransFixpipeParamsV220ToFixpipeParamsL310` 随之更新） |
| `impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h` | L81-99: 新增 `FixpipeL0C2UBImpl` 桩 ×2（V220 参数 3参/4参，`ASCENDC_REPORT_NOT_SUPPORT` 格式） |

> dav_l300、dav_l311、dav_3510 的 impl 文件未修改。intf_impl.h 共享层未修改（V220 包装本就无 arch 守卫；include 分派 L50-51 本就含 `3103 → dav_l310`）。

## 适配后各版本原始代码

### dav_3510（参照版本，3510，未修改）

#### V7 `FixpipeL0C2GMImpl(Arch3510, 4参)` — 完整实现（L710-737，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2GMParam<DstT, SrcT, config>(dst, src, intriParams);
        // nz2nd/nz2dn need set LOOP3_PARA
        SetLoop3Para<config>(intriParams);
        // nz2dn mode need set CHANNEL_PARA extra
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
            set_channel_para(channelPara);
        }
        /*
        make code for scalar quant mode:
        1. copy deq scalar u64 immediate
        2. code gen: move data from l0c to gm
        */
        if (IsScalarQuantMode(intriParams.quantPre)) {
            set_quant_pre(intriParams.deqScalar);
        }
        PipeBarrier<PIPE_FIX>();
        // LOC -> GM
        FixpipeTiling fixpipeTiling;
        FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize, cacheMode);
    }
}
```

#### V8 `FixpipeL0C2GMImpl(Arch3510 deq, 5参)` — 完整实现（L739-774，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams, const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2GMParam<DstT, SrcT, config>(dst, src, intriParams);
        // nz2nd/nz2dn need set LOOP3_PARA
        SetLoop3Para<config>(intriParams);
        // nz2dn mode need set CHANNEL_PARA extra
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
            set_channel_para(channelPara);
        }
        /*
        make code for vector quant mode:
        1. generate fixpipeTiling
        2. copy deq tensor from l1 to fb1 (l1 -> fb1)
        3. code gen: move data from l0c to gm
        */
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        if (IsVectorQuantMode(intriParams.quantPre)) {
            for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
                FixpipeL0C2GMImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i, cacheMode);
            }
            // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
            if (fixpipeTiling.tailNSize > 0) {
                FixpipeL0C2GMImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize,
                    fixpipeTiling.nIterNum, cacheMode);
            }
            return;
        }
    }
}
```

#### V1 `FixpipeL0C2GMImpl(V220, 3参)` — 适配包装（L834-840，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 `FixpipeL0C2GMImpl(V220 deq, 4参)` — 适配包装（L842-849，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

---

### dav_l300（目标版本，3003，未修改）

#### `FixpipeL0C2L1Impl(V220, 3参)` — 适配包装（L498-504，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### `FixpipeL0C2L1Impl(V220 deq, 4参)` — 适配包装（L506-513，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### `FixpipeL0C2GMImpl(V220, 3参)` — 适配包装（L515-521，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### `FixpipeL0C2GMImpl(V220 deq, 4参)` — 适配包装（L523-530，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### `FixpipeL0C2UBImpl(V220, 3参)` — 桩函数（L532-536，未修改）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
```

#### `FixpipeL0C2UBImpl(V220 deq, 4参)` — 桩函数（L538-543，未修改）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
```

---

### dav_l310（目标版本，3103，**修改：本地参数结构重命名 L311→L310**）

#### `TransformParamsL310` + `FixpipeParamsL310` 结构体（L83-131，**修改（重命名）**）

```cpp
template <CO2Layout format>
struct TransformParamsL310 {};

template <>
struct TransformParamsL310<CO2Layout::NZ> {
    __aicore__ inline TransformParamsL310(){};
    using PARAMS = uint8_t;
};

template <>
struct TransformParamsL310<CO2Layout::ROW_MAJOR> {
    __aicore__ inline TransformParamsL310(){};
    using PARAMS = Nz2NdParams;
};

template <>
struct TransformParamsL310<CO2Layout::COLUMN_MAJOR> {
    __aicore__ inline TransformParamsL310(){};
    using PARAMS = uint8_t;
};

template <CO2Layout format = CO2Layout::ROW_MAJOR>
struct FixpipeParamsL310 {
    __aicore__ FixpipeParamsL310() {}

    __aicore__ FixpipeParamsL310(
        const uint16_t nSizeIn_, const uint16_t mSizeIn_, const uint16_t srcStrideIn_, const uint32_t dstStrideIn_)
    {
        nSize = nSizeIn_;
        mSize = mSizeIn_;
        srcStride = srcStrideIn_;
        dstStride = dstStrideIn_;
    }

    uint16_t nSize = 0;
    uint16_t mSize = 0; // M-DirectionSize
    uint16_t srcStride = 0;
    uint32_t dstStride = 0;
    // Params: used for Quant
    uint64_t deqScalar;
    QuantMode_t quantPre = QuantMode_t::NoQuant;
    bool reluEn = false;
    uint8_t unitFlag = 0;
    // extend param
    bool subBlockId = false;
    uint8_t dualDstCtl = 0;
    typename TransformParamsL310<format>::PARAMS params;
    bool isChannelSplit = false;
};
```

> 结构体字段与重命名前完全相同，仅名称从 `L311` 后缀换为 `L310` 后缀，与 dav_l311 的本地结构体（保留 `FixpipeParamsL311` 命名）区分。

#### `TransFixpipeParamsV220ToFixpipeParamsL310` — 参数转换（L481-503，**修改（重命名）**）

```cpp
template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsL310(
    const FixpipeParamsV220& intriParams, FixpipeParamsL310<format>& dstParams)
{
    dstParams.nSize = intriParams.nSize;
    dstParams.mSize = intriParams.mSize;
    dstParams.srcStride = intriParams.srcStride;
    dstParams.dstStride = intriParams.dstStride;
    dstParams.deqScalar = intriParams.deqScalar;
    dstParams.quantPre = intriParams.quantPre;
    dstParams.reluEn = intriParams.reluEn;
    dstParams.unitFlag = intriParams.unitFlag;
    dstParams.isChannelSplit = intriParams.isChannelSplit;
    if constexpr (format == CO2Layout::ROW_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.dstNdStride = intriParams.dstNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    } else if constexpr (format == CO2Layout::COLUMN_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    }
}
```

#### `FixpipeL0C2L1Impl(V220, 3参)` — 适配包装（L505-511，**修改（重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### `FixpipeL0C2L1Impl(V220 deq, 4参)` — 适配包装（L513-520，**修改（重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### `FixpipeL0C2GMImpl(V220, 3参)` — 适配包装（L522-528，**修改（重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### `FixpipeL0C2GMImpl(V220 deq, 4参)` — 适配包装（L530-537，**修改（重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### 本地 `FixpipeL0C2GMImpl(L310, 3参)` — 完整实现（L423-446，**修改（参数类型重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to gm
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    // LOC -> GM
    FixpipeTiling fixpipeTiling;
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### 本地 `FixpipeL0C2GMImpl(L310 deq, 4参)` — 完整实现（L448-479，**修改（参数类型重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to gm
    */
    FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
    if (IsVectorQuantMode(intriParams.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

#### `FixpipeL0C2UBImpl(V220, 3参)` — 桩函数（L539-543，未修改）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
```

#### `FixpipeL0C2UBImpl(V220 deq, 4参)` — 桩函数（L545-550，未修改）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
```

> 其余内部函数（`SetLoop3Para`、`FixpipeL0cToL1`、`GetGMLen`、`FixpipeL0cToOut`、`FixpipeL0C2L1ImplN`、`FixpipeL0C2GMImplN`、本地 `FixpipeL0C2L1Impl` ×2）同样完成 `FixpipeParamsL311` → `FixpipeParamsL310` 参数类型重命名，函数体逻辑未变。ISA 调用 `copy_matrix_cc_to_gm` 保持 24 参数（无 cacheMode，有 nz2dnEn）。

---

### dav_l311（目标版本，3113，未修改）

#### `FixpipeL0C2L1Impl(V220, 3参)` — 适配包装（L505-511，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### `FixpipeL0C2GMImpl(V220, 3参)` — 适配包装（L522-528，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### `FixpipeL0C2GMImpl(V220 deq, 4参)` / `FixpipeL0C2L1Impl(V220 deq, 4参)` / `FixpipeL0C2UBImpl ×2` — 未修改

> 与 dav_l310 结构完全相同（L513-520 / L530-537 / L539-550），保留 `FixpipeParamsL311` / `TransformParamsL311` / `TransFixpipeParamsV220ToFixpipeParamsL311` 命名。本地实现（L423-479）同样保留 L311 命名，ISA 调用 24 参数。

---

### dav_5161（目标版本，5101，**修改：新增 L0C→UB 桩**）

#### `FixpipeL0C2UBImpl(V220, 3参)` — 桩函数（L81-88，**新增**）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    (void)(dst);
    (void)(src);
    (void)(intriParams);
    ASCENDC_REPORT_NOT_SUPPORT(false, "Fixpipe L0C to UB");
}
```

#### `FixpipeL0C2UBImpl(V220 deq, 4参)` — 桩函数（L90-99，**新增**）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    (void)(dst);
    (void)(src);
    (void)(cbufWorkspace);
    (void)(intriParams);
    ASCENDC_REPORT_NOT_SUPPORT(false, "Fixpipe L0C to UB");
}
```

> 桩签名与 l300 的 `FixpipeL0C2UBImpl`（V220 参数 3参/4参）一致，匹配 intf_impl.h 无守卫 V220 包装的调用（dst 为 UB 位置时）。格式按新增桩规范使用 `ASCENDC_REPORT_NOT_SUPPORT`。5101 仍不在 intf.h V220 守卫中，公共 API 层面 `Fixpipe` 声明对该版本不可见。
