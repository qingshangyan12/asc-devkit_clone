# 适配报告：Fixpipe(L0C→L1) 接口

## 适配方案回顾

1. 增加 l310 的声明（intf.h V220 arch 宏控增加 3103）
2. 310 使用的本地参数结构从 `FixpipeParamsL311` 换为 `FixpipeParamsL310`（仅 dav_l310 重命名；dav_l311 维持 `FixpipeParamsL311` 不变）

关联说明：
- 本方案的宏控修改与 dav_l310 重命名，和 **Fixpipe(L0C-GM) 适配**（见 `Fixpipe(L0C-GM)-report.md`）为**同一次代码修改**——intf.h L77-78 宏控块（L80-103）同时覆盖 L0C→L1（L80/L85）与 L0C→GM（L93/L98）两族声明，加 3103 后两族同时对 3103 可见。
- dav_5161 新增的 `FixpipeL0C2UBImpl` 桩 ×2 属 Fixpipe(L0C-GM) 方案第 3 条（"L0C->UB 5101同l300，打桩"），但其作用点正是本报告 V1/V2 变体的**运行期 UB 分流分支**，故在本报告矩阵与代码中一并呈现。

> **行号说明：** intf.h 当前行号相对分析报告整体 +1（L63-64 `SetFixpipeNz2ndFlag` 宏控条件扩为两行所致，属 SetFixpipeNz2ndFlag 适配任务，见 `SetFixpipeNz2ndFlag-report.md`）。本报告使用**适配后现行行号**，关键处标注原行号。

## 验证结论

| 验证项 | 结论 |
|--------|------|
| intf.h L77-78 宏控 | `2201\|\|3003\|\|3103\|\|3113\|\|3510\|\|5102`，3103 按数字序插入 ✓ |
| dav_l310 全文（557 行逐区段读回） | `FixpipeParamsL311`→`FixpipeParamsL310`、`TransformParamsL311`→`TransformParamsL310`（含主模板+3 特化+构造函数名）、`TransFixpipeParamsV220ToFixpipeParamsL311`→`...L310`（函数名+全部调用点）重命名完整，**无 L311 残留**，函数体逻辑未变 ✓ |
| dav_l311 | 未修改，仍为 `FixpipeParamsL311`/`TransformParamsL311`/`TransFixpipeParamsV220ToFixpipeParamsL311` ✓（维持） |
| dav_l5161 | `FixpipeL0C2UBImpl` 桩 ×2 就位（`ASCENDC_REPORT_NOT_SUPPORT` 格式）✓ |
| 3103 完整调用链 | intf.h 声明（宏控+3103）→ intf_impl.h L110-147 V220 包装（本无 arch 宏控，运行期 GetPhyType 分流）→ `CheckFixpipeTensor`（kernel_operator_mm_check.h L268-311，**无 arch 宏控**，3103 可用；dst 位置检查走 L280-281 `#else` 分支仅允许 L1，与 3003/3113 行为一致）→ dav_l310 `FixpipeL0C2L1Impl`（V220 适配 ×2 + 本地 ×2）/`FixpipeL0C2UBImpl`（桩 ×2）全部就位 ✓ |
| intf_impl.h include 分派 | L50-51 `3103 → dav_l310` 本就存在，未修改 ✓ |

## 适配后支持矩阵

### 目标接口（intf.h 现行行号）

| 行号（原） | 变体 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|------|
| L80（79） | V1 V220 L0C→L1 | 完整（维持） | 未声明 → **完整** | 完整（维持） | 未声明（维持） | 完整（维持） |
| L85（84） | V2 V220 L0C→L1 deq | 完整（维持） | 未声明 → **完整** | 完整（维持） | 未声明（维持） | 完整（维持） |
| L160（159） | V7 Arch3510 L0C→L1/UB | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 完整（维持） |
| L165（164） | V8 Arch3510 L0C→L1/UB deq | 不可见（维持） | 不可见（维持） | 不可见（维持） | 不可见（维持） | 完整（维持） |

> V1/V2 的 dst 为 `LocalTensor`，包装层运行期按 `GetPhyType(dst.GetPosition())` 分流：L1 → `FixpipeL0C2L1Impl`，UB → `FixpipeL0C2UBImpl`。故 V1/V2 实际覆盖 L0C→L1 与 L0C→UB 两条路径。

### V1/V2 运行期 UB 分流目标（FixpipeL0C2UBImpl，V220 参数）

| 函数 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|
| `FixpipeL0C2UBImpl(V220, 3参)` | 桩（维持） | 桩（维持） | 桩（维持） | 未定义 → **桩** | 完整（维持） |
| `FixpipeL0C2UBImpl(V220, 4参)` | 桩（维持） | 桩（维持） | 桩（维持） | 未定义 → **桩** | 完整（维持） |

> 5101 仍不在 intf.h V220 宏控中，公共 API 层面 `Fixpipe` 声明对该版本不可见；桩为防御性打桩（与 l300 打桩路径一致），格式按新增桩规范使用 `ASCENDC_REPORT_NOT_SUPPORT`。

### 实现函数层（V1/V2 L0C→L1 路径）

| 函数 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|
| `FixpipeL0C2L1Impl(V220, 3参)` | 完整（维持） | 完整（死代码 → 可用，重命名 L311→L310） | 完整（维持） | 未定义（维持） | 完整（维持） |
| `FixpipeL0C2L1Impl(V220, 4参 deq)` | 完整（维持） | 完整（死代码 → 可用，重命名） | 完整（维持） | 未定义（维持） | 完整（维持） |
| 本地 `FixpipeL0C2L1Impl` ×2（标量/deq） | 完整（维持，`FixpipeParamsL300`） | 完整（重命名，`FixpipeParamsL310`） | 完整（维持，`FixpipeParamsL311`） | 未定义（维持） | 完整（维持，`FixpipeParamsArch3510`） |
| `FixpipeL0cToL1`（ISA `copy_matrix_cc_to_cbuf`） | 22 参（维持） | 24 参（重命名） | 24 参（维持） | 未定义（维持） | 25 参（维持） |
| `TransFixpipeParamsV220To...` | `...L300`（维持） | `...L310`（重命名） | `...L311`（维持） | N/A（维持） | `...Arch3510`（维持） |
| `SetLoop3Para` / `FixpipeL0C2L1ImplN` / `CopyDeqTensorToFbuf` | 完整（维持，burstLen=128B） | 完整（重命名，burstLen=64B） | 完整（维持，burstLen=64B） | 未定义（维持） | 完整（维持，burstLen=64B） |

### 适配前后 V220 arch 宏控对比

- 适配前（intf.h L76-77，189 行版）：`__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102`
- 适配后（intf.h L77-78，190 行版）：`__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3103 || __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102`

## 修改的文件清单

| 文件 | 修改内容 | 方案归属 |
|------|---------|---------|
| `include/basic_api/kernel_operator_fixpipe_intf.h` | L77-78：V220 arch 宏控增加 `__NPU_ARCH__ == 3103`（同块 L80/L85 L1 变体与 L93/L98 GM 变体声明同时对 3103 可见） | 本方案第 1 条（与 GM 适配共用同一修改） |
| `impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h` | 全文件重命名：`FixpipeParamsL311`→`FixpipeParamsL310`（23 处）+ `TransformParamsL311`→`TransformParamsL310`（8 处，含主模板、3 个特化、构造函数名）；函数名 `TransFixpipeParamsV220ToFixpipeParamsL311`→`TransFixpipeParamsV220ToFixpipeParamsL310` 随之更新。函数体逻辑未变 | 本方案第 2 条（与 GM 适配共用同一修改） |
| `impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h` | L81-99：新增 `FixpipeL0C2UBImpl` 桩 ×2（V220 参数 3 参/4 参，`ASCENDC_REPORT_NOT_SUPPORT` 格式） | Fixpipe(L0C-GM) 方案第 3 条（V1/V2 UB 分流路径） |

未修改：`dav_l300`、`dav_l311`、`dav_3510` 的 impl 文件；`intf_impl.h` 共享层（V220 包装 L110-188 本无 arch 宏控；include 分派 L50-51 本含 `3103 → dav_l310`）；`kernel_operator_mm_check.h`（`CheckFixpipeTensor` 无 arch 宏控，3103 直接可用）；`kernel_struct_fixpipe.h`（共享参数结构体）。

关联修改（非本方案范围，属 SetFixpipeNz2ndFlag 适配任务，见 `SetFixpipeNz2ndFlag-report.md`）：
- intf.h L63-68：`SetFixpipeNz2ndFlag` 宏控由 `3510||5102` 扩展至 `3510||5102||5101||5161||5165||5163`（uint32_t dstNdStride 签名分支）
- dav_5161 L29-37：`SetFixPipeClipReluImpl` 增加 5101 专属桩守卫；L71-77：`SetFixpipeNz2ndFlagImpl` uint32_t 签名桩

## 适配后各版本原始代码

### dav_l310（目标版本，3103，修改：本地参数结构重命名 L311→L310）

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

#### `FixpipeTiling` + `GenFixpipeTiling`（L133-160，未修改）

```cpp
// tiling params
struct FixpipeTiling {
    uint16_t nIterNum = 0;
    uint16_t nSize = 0;
    bool isDb = false;
    uint16_t tailNSize = 0;
};

// fixpipe tiling calculating
__aicore__ inline FixpipeTiling GenFixpipeTiling(uint16_t n)
{
    FixpipeTiling tiling;
    // deqTensor/reluTensor in FB valid num is 1024
    uint16_t maxDeqNums = 1024;
    if (n <= maxDeqNums) {
        tiling.nIterNum = 1;
        tiling.nSize = n;
        tiling.isDb = false;
        tiling.tailNSize = 0;
    } else {
        tiling.isDb = true;
        uint16_t dbMaxDeqNums = maxDeqNums / 2;
        tiling.nIterNum = n / dbMaxDeqNums;
        tiling.nSize = dbMaxDeqNums;
        tiling.tailNSize = n % dbMaxDeqNums;
    }
    return tiling;
}
```

#### `CopyDeqTensorToFbuf` — deq tensor L1→FB（L178-195，未修改，burstLen=64B）

```cpp
__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    constexpr uint32_t deqTensorAddrAlignValue = 128;
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), deqTensorAddrAlignValue) * deqTensorAddrAlignValue;
    __fbuf__ uint64_t* deqTensorTempBuf =
        AscendCUtils::GetTemporaryFbBufferAddr<uint64_t>(0, deqDataSize / sizeof(uint64_t));
    uint32_t deqValueOffset = nIterIndex * fixpipeTiling.nSize;

    // L1 -> FB
    constexpr uint16_t fbufBurstLenUnit = 64;
    uint16_t fbufBurstLen = deqDataSize / fbufBurstLenUnit; // copy from cbuf to fbuf, burst_len unit is 64Bytes
    copy_cbuf_to_fbuf(deqTensorTempBuf, cbufWorkspace + deqValueOffset, 1, fbufBurstLen, 0, 0);
    // FPC of fixpipe buffer for Quant_PRE is FPC[15:8], unit is 128Bytes
    uint64_t deqTensorAddr = ((uint64_t)deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
    set_fpc(deqTensorAddr);
    AscendCUtils::FreeTemporaryFbBuffer<uint64_t>(deqTensorTempBuf);
}
```

#### `SetLoop3Para`（L197-219，**修改（参数类型重命名）**）

```cpp
template <const FixpipeConfig& config>
__aicore__ inline void SetLoop3Para(const FixpipeParamsL310<config.format>& intriParams)
{
    // the loop3DstStride range has increased.
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.ndNum > 0), { KERNEL_LOG(KERNEL_ERROR, "ndNum must larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstNdStride) << 32; // LOOP3_PARA[63:32]
        // original src_nd_stride in uint of fractal_size(1024B = 16 * 16 * sizeof(SrcT))
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.ndNum);             // LOOP3_PARA[15:0]
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNdStride) << 16; // LOOP3_PARA[31:16]
        set_loop3_para(loop3Para);
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.dnNum > 0), { KERNEL_LOG(KERNEL_ERROR, "dnNum must larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstDnMatrixStride) << 32; // LOOP3_PARA[63:32]
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.dnNum);                   // LOOP3_PARA[15:0]
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNzMatrixStride) << 16; // LOOP3_PARA[31:16]
        set_loop3_para(loop3Para);
    }
}
```

#### `FixpipeL0cToL1` — ISA 调用（L223-257，**修改（参数类型重命名）**，24 参数 `copy_matrix_cc_to_cbuf`，含 nz2dnEn）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT(
        (!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe, when src position is "
                          "L0C Buffer(CO1) and dst position is L1 Buffer(C1), isChannelSplit must be set as false \n"));

    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> L1 only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false, false, nz2dnEn);
}
```

#### `FixpipeL0C2L1ImplN` — deq 分块迭代（L334-345，**修改（参数类型重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1ImplN(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}
```

#### 本地 `FixpipeL0C2L1Impl(L310, 3参)` — 标量量化（L360-386，**修改（参数类型重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to l1
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### 本地 `FixpipeL0C2L1Impl(L310 deq, 4参)` — 向量量化（L388-421，**修改（参数类型重命名）**）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to l1
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

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

> COLUMN_MAJOR 分支引用的 `ndNum/dstNdStride/srcNdStride` 在 `TransformParamsL310<COLUMN_MAJOR>::PARAMS`（= `uint8_t` 占位）上不存在，属**历史遗留坏代码**（l300/l310/l311 各版本一致，见"遗留问题"1）。按"已存在代码不做修改"原则维持原样。

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

> GM 路径函数（`GetGMLen` L259-277、`FixpipeL0cToOut` L281-332、`FixpipeL0C2GMImplN` L347-358、本地 `FixpipeL0C2GMImpl` ×2 L423-479、V220 GM 适配 ×2 L522-537）同样完成 L311→L310 重命名，完整代码见 `Fixpipe(L0C-GM)-report.md`。

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

#### 其余函数（未修改）

> `FixpipeL0C2L1Impl(V220 deq, 4参)`（L513-520）、`FixpipeL0C2UBImpl` ×2（L539-550）、本地 `FixpipeL0C2L1Impl` ×2（L360-421）、`SetLoop3Para`（L197-219）、`FixpipeL0cToL1`（L223-257，24 参）、`FixpipeL0C2L1ImplN`（L334-345）、`CopyDeqTensorToFbuf`（L178-195，burstLen=64B）、`TransFixpipeParamsV220ToFixpipeParamsL311`（L481-503）——与 dav_l310 结构完全相同，**保留 `FixpipeParamsL311`/`TransformParamsL311`/`TransFixpipeParamsV220ToFixpipeParamsL311` 命名**（适配方案明确 dav_l311 维持不变）。

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

#### `FixpipeL0C2UBImpl(V220)` ×2 — 桩函数（L532-543，未修改）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
//（L538-543：带 cbufWorkspace 版本，同样为桩）
```

#### 本地 `FixpipeL0C2L1Impl(L300, 3参)` — 完整实现（L353-379，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### 本地 `FixpipeL0C2L1Impl(L300 deq, 4参)` — 完整实现（L381-414，未修改）

> 与 dav_l310 的 deq 版本（L388-421）结构相同，仅参数结构体为 `FixpipeParamsL300`。

#### `TransFixpipeParamsV220ToFixpipeParamsL300`（L474-496，未修改）

```cpp
template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsL300(
    const FixpipeParamsV220& intriParams, FixpipeParamsL300<format>& dstParams)
{
    dstParams.nSize = intriParams.nSize;
    dstParams.mSize = intriParams.mSize;
    dstParams.srcStride = intriParams.srcStride;
    dstParams.dstStride = intriParams.dstStride;
    dstParams.quantPre = intriParams.quantPre;
    dstParams.deqScalar = intriParams.deqScalar;
    dstParams.reluEn = intriParams.reluEn;
    dstParams.unitFlag = intriParams.unitFlag;
    dstParams.isChannelSplit = intriParams.isChannelSplit;
    if constexpr (format == CO2Layout::ROW_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.dstNdStride;
    } else if constexpr (format == CO2Layout::COLUMN_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;           // ← uint8_t 占位，无此字段
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
    }
}
```

#### `FixpipeL0cToL1` — ISA 调用（L223-253，未修改，**22 参数**，无 nz2dnEn、COLUMN_MAJOR 无专分支）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT((!(intriParams.isChannelSplit)), ...);
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        // ← 无 COLUMN_MAJOR 专分支：dstOffset 回退到 NZ 模式计算
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false);
}
```

#### `CopyDeqTensorToFbuf`（L178-195，未修改，**burstLen=128B**，l300 独有）

> 结构与 dav_l310 版本相同，差异点：`constexpr uint16_t fbufBurstLenUnit = 128;`（l310/l311/3510 为 64）。

---

### dav_3510（参照版本，3510，未修改）

#### V7 `FixpipeL0C2L1Impl(Arch3510, 3参)` — 完整实现（L572-601，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2L1Param<DstT, SrcT, config>(dst, src, intriParams);
        // nz2nd/nz2dn need set LOOP3_PARA
        SetLoop3Para<config>(intriParams);
        // nz2dn mode need set CHANNEL_PARA extra
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            // Loop0_dst_stride in uint of CO_SIZE
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
            set_channel_para(channelPara);
        }
        if (IsScalarQuantMode(intriParams.quantPre)) {
            // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
            set_quant_pre(intriParams.deqScalar);
        }
        PipeBarrier<PIPE_FIX>();
        FixpipeTiling fixpipeTiling;
        // LOC -> L1
        FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
    }
}
```

#### V8 `FixpipeL0C2L1Impl(Arch3510 deq, 4参)` — 完整实现（L603-640，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2L1Param<DstT, SrcT, config>(dst, src, intriParams);
        SetLoop3Para<config>(intriParams);
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
            set_channel_para(channelPara);
        }
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        if (IsVectorQuantMode(intriParams.quantPre)) {
            for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
                FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
            }
            if (fixpipeTiling.tailNSize > 0) {
                FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize,
                    fixpipeTiling.nIterNum);
            }
            return;
        }
    }
}
```

#### V7/V8 的 UB 分流目标：`FixpipeL0C2UBImpl(Arch3510)` — 完整实现（L642-670 / L672-708，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2UBParam<DstT, SrcT, config>(dst, src, intriParams);
        SetLoop3Para<config>(intriParams);
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
            set_channel_para(channelPara);
        }
        if (IsScalarQuantMode(intriParams.quantPre)) {
            set_quant_pre(intriParams.deqScalar); // float32->uint64_t
        }
        PipeBarrier<PIPE_FIX>();
        // LOC->UB
        FixpipeTiling fixpipeTiling;
        FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
    }
}
//（L672-708：带 cbufWorkspace 的 vector quant 版本，结构同 L1 版）
```

#### V1 `FixpipeL0C2L1Impl(V220, 3参)` — 适配包装（L800-806，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 `FixpipeL0C2L1Impl(V220 deq, 4参)` — 适配包装（L808-815，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### V1/V2 的 UB 分流目标：`FixpipeL0C2UBImpl(V220)` — 适配包装（L817-832，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, params);
}
//（L825-832：带 cbufWorkspace 版本，同构）
```

#### `TransFixpipeParamsV220ToFixpipeParamsArch3510`（L776-798，未修改）

```cpp
template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsArch3510(
    const FixpipeParamsV220& intriParams, FixpipeParamsArch3510<format>& dstParams)
{
    dstParams.nSize = intriParams.nSize;
    dstParams.mSize = intriParams.mSize;
    dstParams.srcStride = intriParams.srcStride;
    dstParams.dstStride = intriParams.dstStride;
    dstParams.quantPre = intriParams.quantPre;
    dstParams.deqScalar = intriParams.deqScalar;
    dstParams.reluEn = intriParams.reluEn;
    dstParams.unitFlag = intriParams.unitFlag;
    dstParams.isChannelSplit = intriParams.isChannelSplit;
    if constexpr (format == CO2Layout::ROW_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.dstNdStride;
    } else if constexpr (format == CO2Layout::COLUMN_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;           // ← Nz2DnParams 无此字段（历史遗留）
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
    }
}
```

#### `FixpipeL0cToL1` — ISA 调用（L188-222，未修改，**25 参数**，含 cacheMode 位 + nz2dnEn）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT((!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe ..."));
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false, false, nz2dnEn);
}
```

#### `FixpipeL0cToUB` — ISA 调用（L226-282，未修改，26 参数 `copy_matrix_cc_to_ub`，l300 系无此函数）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    // ...isChannelSplit DEBUG_ASSERT + dualDstCtl 三重限制检查：
    //    dualDstCtl!=0 仅允许普通 DMA/NZ2ND；0b01 要求 mSize%2==0；0b10 要求 calNSize%32==0 ...
    return copy_matrix_cc_to_ub(
        (__ubuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, intriParams.dualDstCtl, intriParams.subBlockId, 0,
        intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        false, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false,
        false, nz2dnEn);
}
```

#### 参数检查（L445-570 摘要，未修改）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void CheckFixpipeL0C2L1Param(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& params)
{
    CheckFixpipeParams<DstT, SrcT, config>(src, params);   // static_assert 13 种类型组合 + 量化矩阵
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    uint64_t dstAbsAddr = dst - (DstT*)(GetBaseAddrCpu(int8_t(TPosition::C1)));   // ← L1 目的地 32B 对齐检查
    ASCENDC_ASSERT((dstAbsAddr * sizeof(DstT) % ONE_BLK_SIZE == 0), { ... });
#endif
}
//（CheckFixpipeL0C2UBParam L535-548 同构，基址取 TPosition::VECCALC）
```

---

### dav_5161（目标版本，5101，修改：新增 L0C→UB 桩 ×2）

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

> 桩签名与 l300 的 `FixpipeL0C2UBImpl`（V220 参数 3 参/4 参）一致，匹配 intf_impl.h 无宏控 V220 包装在 dst 为 UB 位置时的调用。格式按新增桩规范使用 `ASCENDC_REPORT_NOT_SUPPORT`。5101 仍不在 intf.h V220 宏控中，公共 API 层面 `Fixpipe` 声明对该版本不可见；`FixpipeL0C2L1Impl`/`FixpipeL0cToL1` 等数据搬运实现仍未定义（维持）。

---

### 共享层

#### intf.h V220 宏控块（L77-103，**L77-78 修改（增加 3103）**，声明本身未修改）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3103 || \
                              __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
// L0C->L1
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams);

// L0C->L1 deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams);

// L0C->GM（同宏控块，随 3103 一并可见）
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams);

// L0C->GM deq tensor quant（同上）
...
#endif
```

#### intf.h Arch3510 宏控块（L158-180，未修改）

```cpp
#elif !defined(__NPU_ARCH__) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
// L0C->L1 / L0C->L1/UB deq（FixpipeParamsArch3510 参数，编译期 config.isToUB 分流）
...
#endif
```

#### intf_impl.h V1/V2 V220 包装（L110-147，未修改，无 arch 宏控，运行期分流）

```cpp
// L0C -> L1 for v220
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config>(dst, src, intriParams, "Fixpipe");
#endif

    if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1)) {
        FixpipeL0C2L1Impl<PrimT<T>, PrimT<U>, config>(
            (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
    } else if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB)) {
        FixpipeL0C2UBImpl<PrimT<T>, PrimT<U>, config>(
            (__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
    }
}

// L0C->L1 deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config, typename S,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config, S>(dst, src, cbufWorkspace, intriParams, "Fixpipe");
#endif
    if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1)) {
        FixpipeL0C2L1Impl<PrimT<T>, PrimT<U>, config>(
            (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    } else if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB)) {
        FixpipeL0C2UBImpl<PrimT<T>, PrimT<U>, config>(
            (__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    }
}
```

#### intf_impl.h include 分派（L30-54，未修改，L50-51 本含 3103）

| Arch | #include 的文件 |
|------|----------------|
| 1001 | `dav_c100/kernel_operator_fixpipe_impl.h` |
| 2002 | `dav_m200/kernel_operator_fixpipe_impl.h` |
| 2201 | `dav_c220/kernel_operator_fixpipe_impl.h` + `..._v2_impl.h` |
| 3002 | `dav_m300/kernel_operator_fixpipe_impl.h` + `..._v2_impl.h` |
| 3102 | `dav_m310/kernel_operator_fixpipe_impl.h` |
| 3510 | `dav_3510/kernel_operator_fixpipe_impl.h` |
| 5102 | `dav_m510/kernel_operator_fixpipe_impl.h` |
| 3003 | `dav_l300/kernel_operator_fixpipe_impl.h` |
| 3113 | `dav_l311/kernel_operator_fixpipe_impl.h` |
| **3103** | `dav_l310/kernel_operator_fixpipe_impl.h`（L50-51，本就存在） |
| 5101/5161/5165/5163 | `dav_5161/kernel_operator_fixpipe_impl.h` |

#### debug 检查依赖：`CheckFixpipeTensor`（kernel_operator_mm_check.h L268-311，未修改，无 arch 宏控）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void CheckFixpipeTensor(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams,
    const __gm__ char* apiName)
{
    CheckFixpipeParamsV220Common<T, U, config>(intriParams, apiName);
    CheckTensorPhyPosition<Hardware::L0C>(src, "src", "L0C Buffer(CO1)", apiName);
    const uint32_t L0C_SRC_ALIGN = 16 * sizeof(float);
    CheckTensorAlignment(src, L0C_SRC_ALIGN, "src", apiName);
    CheckTensorAlignment(dst, ONE_BLK_SIZE, "dst", apiName);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3002) || (__NPU_ARCH__ == 3102))
    CheckTensorPhyPosition<Hardware::L1, Hardware::UB>(dst, "dst", "L1 Buffer(C1)/UB(VECIN/VECOUT/VECCALC)", apiName);
#else
    CheckTensorPhyPosition<Hardware::L1>(dst, "dst", "L1 Buffer(C1)", apiName);   // ← 3103 走此分支，与 3003/3113 一致
#endif
}
//（另有 GlobalTensor dst / 带 cbufWorkspace 共 4 个重载，均无 arch 宏控）
```

## 遗留问题（未处理，维持现状）

1. **`TransFixpipeParamsV220To...` 的 COLUMN_MAJOR 分支为历史坏代码（所有版本）** — l300/l310/l311 侧 COLUMN_MAJOR 的 PARAMS 为 `uint8_t` 占位（无 `ndNum/srcNdStride/dstNdStride` 字段），3510 侧 `Nz2DnParams` 亦无这些字段。任何版本以 `CFG_COLUMN_MAJOR` 实例化 V220 `Fixpipe` 均编译错误；且该分支 `dstNdStride = srcNdStride` 映射本身可疑。按"已存在代码不做修改"原则维持。
2. **l300/l310/l311 的 L0C→UB 为运行期 assert 桩** — 编译通过、运行时才报错 "Fixpipe doesn't support L0C to UB on current device"；仅 3510 完整支持（`copy_matrix_cc_to_ub`）。dav_5161 新增桩为 `ASCENDC_REPORT_NOT_SUPPORT` 格式（新增桩规范），与既有桩格式不同属历史原因。
3. **CopyDeqTensorToFbuf burstLen 差异** — l300=128B，l310/l311/3510=64B。硬件差异还是代码遗留未定。
4. **V7/V8（Arch3510 系列）仅 3510/5102 可见** — 依赖 `FixpipeConfig::isToUB` 字段（仅 3510/5102 的 FixpipeConfig 存在），结构性限制，维持。
5. **5101 公共 API 层面仍不可见 Fixpipe** — 不在 V220 宏控，仅有防御性 UB 桩；`FixpipeL0C2L1Impl` 等实现未定义。若未来 5101 需支持，需另行确认硬件能力（`copy_matrix_cc_to_cbuf` 是否可用）。
6. **l310 与 l311 文件内容仍逐行同构**（仅标识符后缀 L310/L311 区分）— 是否合并或宏区分属架构维护策略问题，维持。
