# 【AscendC】A6（9201）支持 L1/L0A/L0B 的数据 dump

## 一、需求描述

【需求概述】：本设计目标是在不改变用户侧 DumpTensor 公共接口的前提下，为 A6（9201）ring buffer dump 路径补齐 LocalTensor 位于 L1、L0A、L0B 三个物理位置时的数据 dump 能力。L0C 不属于本需求范围。

| 逻辑 Buffer | TPosition | 物理位置 | 地址空间类型 |
|---|---|---|---|
| L1 | A1/B1 | L1 | `__cbuf__` |
| L0A | A2 | L0A | `__ca__` |
| L0B | B2 | L0B | `__cb__` |

【资料变更】：不新增用户可见 API，不调整 DumpTensor 函数签名。资料侧如需更新，应补充 A6（9201）ring buffer dump 模式下支持 L1/L0A/L0B，并说明该能力依赖 runtime 正确初始化 debugBusAddr，且不支持 CPU debug 模式；L0A/L0B 输出按 cube 侧 NZ 物理存储布局解读，与 L1 线性布局不同；L0C 不属于本需求范围。

【DFX】：本能力依赖 ring buffer dump 框架、芯片 debug bus 和 `DebugBlockHeadInfo::debugBusAddr`。当 `debugBusAddr == 0` 或 block info 为空时，kernel 侧安全返回。debug bus ready 轮询设置了有限次数保护（最多 5 次），避免死循环。

【验收目标】：

A6（9201）下 DumpTensor(LocalTensor) ring buffer 路径支持 L1、L0A、L0B 三个位置的 dump，`DumpTensorTlv.position` 分别为 L1/L0A/L0B，payload 与硬件存储数据一致。

## 二、方案详述

### 2.1 总体方案

本次实现采用"保留用户接口、扩展 kernel 内部分发、在 A6（9201）架构层实现 debug bus 搬运"的方案。

核心链路如下：

1. 用户仍调用既有 `DumpTensor(LocalTensor<T>, desc, dumpSize)`。

2. `impl/basic_api/dav_9201/kernel_operator_dump_tensor_impl.h` 中的 `DumpTensorLocal2GMImpl` 判断 `LocalTensor` 所在 `Hardware`。

3. 当 `g_sysPrintFifoSpace != nullptr` 时进入 ring buffer dump 路径：

    - `UB` 调用 `__asc_aicore::asc_dump_ubuf`；

    - `L1` 在 AIC 侧调用 `__asc_aicore::asc_dump_l1buf`；

    - `L0A` 在 AIC 侧调用 `__asc_aicore::asc_dump_abuf`；

    - `L0B` 在 AIC 侧调用 `__asc_aicore::asc_dump_bbuf`；

4. `impl/utils/debug/asc_aicore_dump_impl.h` 中统一通过 `asc_dump_local_impl<hardware, T>` 完成控制寄存器保护、诊断能力启用、ring buffer 空间检查、TLV header 填写和 payload 写入。

5. `impl/utils/debug/npu_arch_9201/asc_aicore_dump_utils.h` 中为 L1/L0A/L0B 实现 debug bus 到 GM 的 32B chunk 拷贝。

6. `DumpTensorTlv` 格式不变，消费侧继续按既有 tensor dump TLV 解析；L0A/L0B 仅是 `position` 字段新增取值，解析侧需按位置值区分解读布局。

### 2.2 涉及文件

| 文件 | 变更点 |
|---|---|
| `impl/basic_api/dav_9201/kernel_operator_dump_tensor_impl.h` | ring buffer 路径新增 L1/L0A/L0B 分发到 `asc_dump_l1buf / asc_dump_abuf / asc_dump_bbuf`。 |
| `impl/utils/debug/asc_aicore_dump_impl.h` | 通过模板化 `asc_dump_local_impl` 统一 GM/UB/L1/L0A/L0B ring buffer dump；超长 SuperTensor 分包编译条件补 9201，`is_super_tensor_hardware_supported` 补 L0A/L0B。L0C 不属于本需求范围。 |
| `impl/utils/debug/asc_debug_types.h` | `DebugBlockHeadInfo` 新增 `uint64_t debugBusAddr` 字段；`DumpTensorTlv::position` 注释扩展到 GM/UB/L1/L0A/L0B。 |
| `impl/utils/debug/npu_arch_9201/asc_aicore_dump_utils.h` | 新增 debug bus 寄存器常量、model base 映射、chunk 读取、L1/L0A/L0B 到 GM 的 debug bus 搬运。 |
| `impl/utils/debug/asc_debug_utils.h` | arch 分发链新增 `__NPU_ARCH__ == 9201` 分支，接入 `npu_arch_9201/asc_debug_utils_impl.h`。 |
| `impl/basic_api/kernel_operator_dump_tensor_intf_impl.h` | arch include 链新增 `__NPU_ARCH__ == 9201` 分支，引入 `dav_9201/kernel_operator_dump_tensor_impl.h`。 |

### 2.3 DebugBlockHeadInfo 布局

在 `DebugBlockHeadInfo` 中显式增加 `debugBusAddr` 字段：

```cpp
struct DebugBlockHeadInfo {
    uint32_t length = 0U;        // total size per block (include head and r/w info)
    uint32_t coreId = 0U;        // current core id
    uint32_t blockNum = 0U;      // total core num
    uint32_t ringBufLen = 0U;    // fifo buff size (print tlv storage)
    uint16_t magic = 0U;         // magic number
    uint16_t flag = 0U;          // 0: simd, 1: simt
    uint32_t rsv = 0U;           // reserve
    uint64_t ringBufAddr = 0U;   // start addr of fifo buff
    uint64_t debugBusAddr = 0U;  // debug-bus base addr for L1/L0A/L0B dump
    uint32_t resvMem[4];         // reserved
};
```

kernel 侧读取方式：

```cpp
__aicore__ inline uint64_t get_debug_bus_addr_impl()
{
    __gm__ DebugBlockHeadInfo* blockInfo = get_block_info();
    return blockInfo == nullptr ? 0U : blockInfo->debugBusAddr;
}
```

设计约束：

- runtime 或上层初始化 block header 时必须按该布局写入 `debugBusAddr`（L1/L0A/L0B 共用同一基址）；

- `debugBusAddr == 0U` 时，debug bus 搬运函数直接返回；

- 结构体字段变化需要与 host/runtime 侧布局保持一致，避免字段错位。

### 2.4 TLV 设计

本次不修改 `DumpTensorTlv` 格式：

```cpp
struct DumpTensorTlv {
    uint32_t type;
    uint32_t length;
    uint32_t tensorAddr;
    uint32_t dataType;
    uint32_t desc;
    uint32_t bufferId;
    uint16_t position;
    uint16_t blockIdx;
    uint32_t dim;
    uint32_t shape[8];
    uint32_t resv1;
    uint32_t dumpSize;
};
```

`position` 取值：GM=0, UB=1, L1=2, L0A=3, L0B=4, L0C=5 ...

字段填写逻辑位于 `set_dump_tlv_info`：

- `type = DumpType::DUMP_TENSOR`；

- `length = sizeof(DumpTensorTlv) - sizeof(uint32_t[2]) + alignDumpLen`；

- `tensorAddr = reinterpret_cast<uintptr_t>(src)` 的低 32 位；

- `dataType = get_dump_datatype<T>()`；

- `desc = desc`；

- `blockIdx = asc_debug_get_block_idx()`；

- `bufferId = 0`；

- `position = static_cast<uint16_t>(hardware)`，L0A/L0B 路径分别填 3/4；

- `dim/shape/resv1` 置 0；

- `dumpSize = dump_size * sizeof(T)`。

### 2.5 asc_aicore_dump_impl 分发设计

`asc_aicore_dump_impl.h` 中新增统一模板流程：

```cpp
template <AscendC::Hardware hardware, typename T, typename U>
__aicore__ inline void asc_dump_local_impl(U input, uint32_t desc, uint32_t dump_size)
{
    uint64_t ctrlValue = get_ctrl();
    set_atomic_none();
    enable_asc_diagnostics();
    if (g_sysPrintFifoSpace != nullptr) {
        asc_dump_impl<hardware, T>(input, desc, dump_size);
    }
    set_ctrl(ctrlValue);
}
```

对外内部 helper 分发如下：

```cpp
asc_dump_gm   -> Hardware::GM
asc_dump_ubuf -> Hardware::UB
asc_dump_l1buf -> Hardware::L1
asc_dump_abuf -> Hardware::L0A
asc_dump_bbuf -> Hardware::L0B
```

`set_dump_tlv_data` 根据 `hardware` 调用对应 copy helper：

```cpp
GM   -> mem_copy_gm_to_gm
UB   -> mem_copy_ub_to_gm_impl
L1   -> mem_copy_l1buf_to_gm_impl
L0A  -> mem_copy_abuf_to_gm_impl
L0B  -> mem_copy_bbuf_to_gm_impl
```

### 2.6 A6（9201）debug bus 搬运设计

#### 2.6.1 常量定义

`npu_arch_9201/asc_aicore_dump_utils.h` 定义 debug bus 寄存器偏移：

```cpp
constexpr uint32_t ASC_DEBUG_BUS_UNLOCK_OFFSET = 0x078;
constexpr uint32_t ASC_DEBUG_BUS_CTRL_OFFSET = 0x100;
constexpr uint32_t ASC_DEBUG_BUS_ADDR_LOW_OFFSET = 0x108;
constexpr uint32_t ASC_DEBUG_BUS_MODEL_BASE_OFFSET = 0x10c;
constexpr uint32_t ASC_DEBUG_BUS_DATA_OFFSET = 0x110;
constexpr uint32_t ASC_DEBUG_BUS_BUSY_OFFSET = 0x130;
constexpr uint32_t ASC_DEBUG_BUS_UNLOCK_VALUE = 0x19101920;
constexpr uint32_t ASC_DEBUG_BUS_READ_MASK = 0x2;
constexpr uint32_t ASC_DEBUG_BUS_CHUNK_WORDS = 8;
constexpr uint32_t ASC_DEBUG_BUS_CHUNK_BYTES = 32;
```

#### 2.6.2 model base 与地址步进映射

| 路径 | 模型基地址 | 单轮读取 | Local 地址换算 | 每轮地址步进 |
|---|---|---|---|---|
| L1 | `0x620000` | 32B | 本地字节地址右移 5 位 | 1 个 32B 地址单元 |
| L0A | `0x410000` | 32B | 本地字节地址原样（右移 0 位） | 32 个地址单位 |
| L0B | `0x420000` | 32B | 本地字节地址原样（右移 0 位） | 32 个地址单位 |

```cpp
if constexpr (hardware == AscendC::Hardware::L1) {
    return 0x620000U;
} else if constexpr (hardware == AscendC::Hardware::L0A) {
    return 0x410000U;
} else if constexpr (hardware == AscendC::Hardware::L0B) {
    return 0x420000U;
}
```

说明：L0C 不属于本需求范围。

#### 2.6.3 单 chunk 读取流程

`debug_bus_read_chunk<32>(debugBusAddr, localOffset, modelBase, dataDst)`：

1. `sync_all()`；

2. 写 `ASC_DEBUG_BUS_UNLOCK_VALUE` 到 unlock 寄存器；

3. 写 `localOffset` 到地址低位寄存器；

4. 写 `modelBase` 到 model base 寄存器；

5. 写 `ASC_DEBUG_BUS_READ_MASK` 到 ctrl 寄存器触发读取；

6. 轮询 busy/ready 寄存器，直到 ready bit `0x80000000U` 置位，或计数超过 5 次后退出；

7. 读取 `ASC_DEBUG_BUS_DATA_OFFSET + i * sizeof(uint32_t)` 共 8 个 word；

8. 通过 `st_dev` 写入目标 GM 地址 `dataDst + i * sizeof(uint32_t)`；

9. `sync_all()`。

#### 2.6.4 多 chunk 搬运流程

`debug_bus_copy_local_to_gm_impl<hardware>`：

```cpp
constexpr uint32_t asc_debug_bus_chunk_shift = 5; // 32 bytes per chunk
constexpr uint32_t modelBase = get_debug_bus_model_base<hardware>();
uint64_t dstGlobalAddr = reinterpret_cast<uint64_t>(dst);
uint64_t curLocalBaseAddr  = reinterpret_cast<uint64_t>(src);

if constexpr (hardware == AscendC::Hardware::L1) {
    srcLocalAddr = curLocalBaseAddr >> asc_debug_bus_chunk_shift;
    localStep = 1;
} else {
    // L0A / L0B：字节编址，每轮步进 32
    srcLocalAddr = curLocalBaseAddr;
    localStep = ASC_DEBUG_BUS_CHUNK_BYTES;
}

const int32_t loopCount = alignDumpBytes >> asc_debug_bus_chunk_shift;
for (uint32_t idx = 0; idx < loopCount; ++idx) {
    debug_bus_read_chunk<32>(debugBusAddr, srcLocalAddr, modelBase, dstGlobalAddr);
    srcLocalAddr += localStep;
    dstGlobalAddr += ASC_DEBUG_BUS_CHUNK_BYTES;
}
```

地址步进差异是关键约束：

- `L1`：debug bus local offset 使用 `本地地址 >> 5`，每次步进 1；

- `L0A / L0B`：debug bus local offset 使用原始字节地址，每次步进 32。

### 2.7 DumpTensorLocal2GMImpl 分发

`dav_9201/kernel_operator_dump_tensor_impl.h` 中 `DumpTensorLocal2GMImpl` 根据 `LocalTensor::GetPosition()` 分发：

```cpp
if (position == Hardware::UB) {
    __asc_aicore::asc_dump_ubuf((__ubuf__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim);
} else if (position == Hardware::L1) {
    if ASCEND_IS_AIC {
        __asc_aicore::asc_dump_l1buf((__cbuf__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim);
    }
} else if (position == Hardware::L0A) {
    if ASCEND_IS_AIC {
        __asc_aicore::asc_dump_abuf((__ca__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim);
    }
} else if (position == Hardware::L0B) {
    if ASCEND_IS_AIC {
        __asc_aicore::asc_dump_bbuf((__cb__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim);
    }
}
// ... 其余已有分支
```

说明：L1/L0A/L0B 仅存在于 AIC（cube）核，`if ASCEND_IS_AIC` gating 一致。

## 三、新增接口

### 3.1 用户接口

无新增用户接口。

既有 `DumpTensor` 调用方式保持不变，本需求仅扩展内部支持的 `LocalTensor` 硬件位置。

## 四、约束条件

1. 本次迭代交付 A6（9201）的 L1/L0A/L0B debug bus dump；L0C 不属于本需求范围。

2. 功能范围仅为 A6（9201）AIC（cube）核；L1/L0A/L0B 仅存在于 cube 核，非 cube 路径由 `__DAV_CUBE__` gating 与 `ASCEND_IS_AIC` 双重拦截。

3. 依赖 runtime 正确初始化 `DebugBlockHeadInfo::debugBusAddr`；为 0 时该次 dump 静默跳过。

4. Dump 字节数按 32B 向上对齐，LocalTensor 必须为对齐后的读取范围预留有效空间。

5. L0A/L0B 数据为 cube 侧 NZ 物理存储布局（由 LoadData2D 搬入决定），dump 结果按物理存储顺序输出，不能以与 L1 原始数据线性逐位一致作为正确性标准，解析/校验侧需按 ND2NZ 布局组织 golden 数据。

6. CPU Debug 不执行 L1/L0A/L0B 硬件 dump，沿用既有 `asc_dump` 的断言行为。

7. 其余约束与 DumpTensor 保持一致。

## 五、兼容性分析

已有接口新增功能，无兼容性问题。公共头文件中的 `DumpTensor` 签名、参数和调用方式不变。新增分支仅在 9201 且 LocalTensor 物理位置为 L1/L0A/L0B 时生效；UB、GM 等已有路径仍进入原实现。非 9201 架构的 include 链与行为完全不变。

## 六、维测分析

1. 可通过 DumpTensor 正常打印 L1/L0A/L0B 数据。

2. Dump 大小为 0、Block 信息无效（magic 错误）、环形缓冲区不足等情况继续使用既有 DumpTensor 检查和返回逻辑。

3. DebugBus 地址无效（`debugBusAddr == 0`）或搬运 helper 返回失败时，不提交本次 TLV 写指针更新，不输出不完整记录。

4. debug bus ready 轮询最多 5 次，超时退出，不死循环。

5. 非 AIC 调用 L1/L0A/L0B 路径时不产出 dump 数据（`ASCEND_IS_AIC` gating 静默跳过）。

6. 其余与 DumpTensor 保持一致。

## 七、测试用例

| 用例编号 | 场景 | 预期结果 |
|---|---|---|
| TC-1 | 9201 ring buffer 模式 dump L1 LocalTensor（多 dtype：bool/u8/i8/u16/i16/u32/i32/u64/i64/fp16/fp32/bf16） | 生成 `DumpTensorTlv`，`position=L1`，payload 与 L1 数据逐元素一致 |
| TC-2 | 9201 dump L0A LocalTensor（GM->L1 后 LoadData2D L1->L0A，再 dump） | 生成 `DumpTensorTlv`，`position=L0A`，payload 与按 NZ 布局组织的 golden 数据一致 |
| TC-3 | 9201 dump L0B LocalTensor（同上，LoadData2D L1->L0B） | 生成 `DumpTensorTlv`，`position=L0B`，payload 与 golden 一致 |
| TC-4 | 9201 dump 超长 L1/L0A/L0B 张量（超过 ringBufLen） | 走 SuperTensor 分包路径，分包重组后数据完整 |
| TC-5 | `debugBusAddr == 0` 时 dump L1/L0A/L0B | 安全返回，不产出 TLV，不推进写游标，无残留 |
| TC-6 | AIV 核 dump L1/L0A/L0B | 不进入 dump 分支（静默跳过），无错误数据 |
| TC-7 | 非 9201 架构（2201/3510/5102 等）编译回归 | 编译不因本次变更失败，既有 dump 行为不变 |
| TC-8 | 9201 CPU debug 模式 | 沿用断言行为 |
| TC-9 | 9201 既有 UB/GM dump 及 printf 用例回归 | 输出排布与历史预期不变 |

> （注：部分内容由 AI 生成）
