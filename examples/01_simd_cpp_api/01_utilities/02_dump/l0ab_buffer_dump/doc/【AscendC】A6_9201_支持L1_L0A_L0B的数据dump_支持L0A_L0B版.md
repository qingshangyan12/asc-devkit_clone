# 【AscendC】A6（9201）支持 L1/L0A/L0B 的数据 dump（支持 L0A/L0B 版）

> 参照 dav_3510 设计文档《【AscendC】支持L1_L0A_L0B的数据dump》编写。
> 与 3510 的关键差异：3510 仅交付 L1 通路（L0A/L0B 未交付）；本方案为 A6（dav_9201，
> `__NPU_ARCH__ == 9201`）**同时交付 L1、L0A、L0B 三条通路**。L0C 不属于本需求范围。

# 一、需求描述

## 1.1 需求概述

在不改变用户侧 `DumpTensor` 公共接口的前提下，为 A6（dav_9201）ring buffer dump 路径补齐 LocalTensor 位于 **L1、L0A、L0B** 三个物理位置时的数据 dump 能力。L0C 不属于本需求范围。

| 逻辑 Buffer | TPosition | 物理位置 | 地址空间类型 |
|---|---|---|---|
| L1 | A1/B1 | L1 | `__cbuf__` |
| L0A | A2 | L0A | `__ca__` |
| L0B | B2 | L0B | `__cb__` |

与 3510 的实现差异：3510 交付时 `npu_arch_3510/asc_aicore_dump_utils.h` 需要从零实现 debug bus 搬运，且 L0A/L0B 通路不交付；A6 侧 `npu_arch_9201/asc_aicore_dump_utils.h` 已从 3510 整体拷贝就绪（含 L0A/L0B 的 model base 与搬运函数），**本需求的主要工作是架构分发接线，并为 `dav_9201` 的 `DumpTensorLocal2GMImpl` 补齐 L0A/L0B 分发分支**。

## 1.2 资料变更

不新增用户可见 API，不调整 `DumpTensor` 函数签名。资料侧如需更新，应补充 A6（9201）ring buffer dump 模式下支持 L1/L0A/L0B，并说明：

- 该能力依赖 runtime 正确初始化 `DebugBlockHeadInfo::debugBusAddr`，`debugBusAddr == 0` 时 kernel 侧安全返回；
- 不支持 CPU debug 模式；
- L0A/L0B 输出按 cube 侧 NZ 物理存储布局解读（与 L1 线性布局不同）。

## 1.3 DFX

本能力依赖 ring buffer dump 框架、芯片 debug bus 和 `DebugBlockHeadInfo::debugBusAddr`。当 `debugBusAddr == 0` 或 block info 为空（`magic != 0xAE86`）时，kernel 侧安全返回，不产出 TLV。debug bus ready 轮询设置了有限次数保护（最多 5 次），避免死循环。

## 1.4 验收目标

A6（9201）下 `DumpTensor(LocalTensor)` ring buffer 路径支持 L1、L0A、L0B 三个位置的 dump，`DumpTensorTlv.position` 分别为 L1/L0A/L0B，payload 与硬件存储数据一致。

# 二、方案详述

## 2.1 现状分析（与 3510 的最大差异：底层已备好、上层未接线）

**已就绪（无需实现）**：

1. `impl/utils/debug/npu_arch_9201/asc_aicore_dump_utils.h` 已从 3510 整体拷贝，包含：
   - debug bus 寄存器常量（unlock/ctrl/addr_low/model_base/data/busy 偏移、`ASC_DEBUG_BUS_UNLOCK_VALUE = 0x19101920`、读掩码 `0x2` 等）；
   - `get_debug_bus_model_base<hardware>()`：L1=`0x620000`、**L0A=`0x410000`**、**L0B=`0x420000`**、L0C=`0x430000`、BIAS/FIXBUF=`0x480000`；
   - `debug_bus_write_reg / debug_bus_read_reg`（`dsb + st_dev/ld_dev`）；
   - `debug_bus_read_chunk<readLen>`（单轮 32B 读取 + busy 轮询超时保护）；
   - `debug_bus_copy_local_to_gm_impl<hardware>`（多 chunk 循环搬运，FIXBUF 通道特判）；
   - `mem_copy_l1buf_to_gm_impl`（L183）、`mem_copy_abuf_to_gm_impl`（L193，L0A）、`mem_copy_bbuf_to_gm_impl`（L203，L0B）——均带 `#if defined(__DAV_CUBE__)` gating。
2. 公共层 `impl/utils/debug/asc_aicore_dump_impl.h` 已具备：
   - `set_dump_tlv_data` 的 L0A/L0B 分发（L94-97：`mem_copy_abuf_to_gm_impl / mem_copy_bbuf_to_gm_impl`）；
   - 位置入口 `asc_dump_abuf(__ca__ T*)`（L313）与 `asc_dump_bbuf(__cb__ T*)`（L320），带 shape 与 3 参两个重载。
3. `impl/utils/debug/asc_debug_types.h`：`DebugBlockHeadInfo::debugBusAddr` 字段（注释即 "debug-bus base addr for L1/L0A/L0B dump"）、`DumpTensorTlv::position` 枚举（GM/UB/L1/L0A/L0B/L0C）均已支持，无需改动。

**未接线（本次主要改动点）**：

1. `asc_debug_utils.h`（L29-41）：arch 分发链无 `__NPU_ARCH__ == 9201` 分支，9201 编译时落入 `asc_debug_utils_impl_stub.h`，`get_block_info`、ringbuf 流控等原语不可用（stub 为空实现）；而 `npu_arch_9201/asc_debug_utils_impl.h` 已备好未接。
2. `asc_aicore_dump_impl.h`（L24-32）：arch include 链无 9201 分支，`npu_arch_9201/asc_aicore_dump_utils.h` 成为死代码。
3. `kernel_operator_dump_tensor_intf_impl.h`（L24-50）：顶部公共 include 条件与 arch include 链均无 9201 分支，`dav_9201/kernel_operator_dump_tensor_impl.h` 未被任何头文件包含。
4. `dav_9201/kernel_operator_dump_tensor_impl.h` 的 `DumpTensorLocal2GMImpl`（L37-70）：已有 UB/L1/L0C/BIAS/FIXBUF 分支，**缺 L0A/L0B 分支**，L0A/L0B 位置的 LocalTensor 落入 else 断言 "dump tensor only support dump tensor from local to gm"。
5. `asc_aicore_dump_impl.h` 的 `asc_dump_impl`（L235）：超长 SuperTensor 分包编译条件 `#if __NPU_ARCH__ == 3510 || 2201 || 2002 || 5102` **不含 9201**；`is_super_tensor_hardware_supported`（L109-115）**不含 L0A/L0B**。

## 2.2 总体方案

采用"保留用户接口、补齐架构分发接线、在 dav_9201 架构层补 L0A/L0B 分发分支"的方案。核心链路如下：

```
DumpTensor(LocalTensor, desc, dumpSize)                        // kernel_operator_dump_tensor_intf_impl.h（新增 9201 include 分支）
  └─ DumpTensorLocal2GMImpl(src, desc, dumpSize, shape, shapeDim)  // dav_9201（新增 L0A/L0B 分支）
       ├─ UB   -> __asc_aicore::asc_dump_ubuf((__ubuf__ T*)...)     // 既有
       ├─ L1   -> __asc_aicore::asc_dump_l1buf((__cbuf__ T*)...)   // 既有（AIC only）
       ├─ L0A  -> __asc_aicore::asc_dump_abuf((__ca__ T*)...)      // ★ 新增分支（AIC only）
       ├─ L0B  -> __asc_aicore::asc_dump_bbuf((__cb__ T*)...)      // ★ 新增分支（AIC only）
       └─ L0C/BIAS/FIXBUF -> 既有分支
            └─ asc_dump_local_impl<Hardware::L1/L0A/L0B, T>        // asc_aicore_dump_impl.h（不改）
                 └─ asc_dump_impl                                  // 控制寄存器保护已在上层完成
                      ├─ 普通 TLV 路径（头 + 32B 对齐数据）
                      └─ SuperTensor 分包路径（超长 fallback，条件补 9201）
                           └─ set_dump_tlv_data
                                ├─ L1  -> mem_copy_l1buf_to_gm_impl    // npu_arch_9201（已就绪）
                                ├─ L0A -> mem_copy_abuf_to_gm_impl     // npu_arch_9201（已就绪）
                                └─ L0B -> mem_copy_bbuf_to_gm_impl     // npu_arch_9201（已就绪）
                                     └─ debug_bus_copy_local_to_gm_impl<Hardware::L1/L0A/L0B>
                                          └─ debug_bus_read_chunk（每轮 32B）
```

`DumpTensorTlv` 格式不变，消费侧继续按既有 tensor dump TLV 解析；L0A/L0B 仅是 `position` 字段新增取值（L1=2、L0A=3、L0B=4），解析侧需按位置值区分解读布局。

## 2.3 涉及文件

| # | 文件 | 变更点 |
|---|---|---|
| 1 | `impl/basic_api/kernel_operator_dump_tensor_intf_impl.h` | 顶部公共 include 条件（L24-25）补 `__NPU_ARCH__ == 9201`；arch include 链（L30-50）新增 `#elif __NPU_ARCH__ == 9201` → `#include "dav_9201/kernel_operator_dump_tensor_impl.h"`。9201 走带 shape 的 5 参转发形态（同 3510）。 |
| 2 | `impl/utils/debug/asc_debug_utils.h` | arch 分发（L29-41）新增 `#elif __NPU_ARCH__ == 9201` → `npu_arch_9201/asc_debug_utils_impl.h`（当前落到 stub，导致 block info / ringbuf 流控原语不可用）。 |
| 3 | `impl/utils/debug/asc_aicore_dump_impl.h` | arch include 链（L24-32）新增 `#elif __NPU_ARCH__ == 9201` → `npu_arch_9201/asc_aicore_dump_utils.h`；`asc_dump_impl` SuperTensor 分包编译条件（L235）补 `__NPU_ARCH__ == 9201`；`is_super_tensor_hardware_supported`（L109-115）补 L0A/L0B。 |
| 4 | `impl/basic_api/dav_9201/kernel_operator_dump_tensor_impl.h` | `DumpTensorLocal2GMImpl` 新增 L0A/L0B 分发分支（AIC only，仿照既有 L1/L0C 分支写法，见 2.7）。 |
| 5 | `impl/utils/debug/npu_arch_9201/asc_aicore_dump_utils.h` | **无需改动**：L1/L0A/L0B 的 debug bus 搬运实现已就绪。 |
| 6 | `impl/utils/debug/asc_debug_types.h` | **无需改动**：`debugBusAddr` 字段与 `DumpTensorTlv::position` 的 L0A/L0B 取值均已支持。 |
| 7 | `impl/utils/debug/asc_aicore_printf_impl.h` | arch include 链（L21-33）与 `npu_arch_9201/asc_aicore_printf_utils.h` 同步补 9201 接线（该文件由 intf 层公共 include 引入，与 dump 共用 ring buffer 框架）。 |

## 2.4 DebugBlockHeadInfo 布局

复用既有布局（与 3510 一致），`debugBusAddr` 显式承载 debug bus 基地址：

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

kernel 侧读取方式（`npu_arch_9201/asc_aicore_dump_utils.h` 已实现）：

```cpp
__aicore__ inline uint64_t get_debug_bus_addr_impl()
{
    __gm__ DebugBlockHeadInfo* blockInfo = get_block_info();
    return blockInfo == nullptr ? 0U : blockInfo->debugBusAddr;
}
```

设计约束：

- runtime 或上层初始化 block header 时必须按该布局写入 `debugBusAddr`（L1/L0A/L0B 共用同一基址）；
- `debugBusAddr == 0U` 时，`debug_bus_copy_local_to_gm_impl` 直接返回 1，`set_dump_tlv_data` 收到非 0 后不推进写游标，本次 dump 静默跳过；
- 结构体字段与 host/runtime 侧布局保持一致，避免字段错位（本需求不改布局）。

## 2.5 TLV 设计

本次不修改 `DumpTensorTlv` 格式（同 3510）：

```cpp
struct DumpTensorTlv {
    uint32_t type;
    uint32_t length;
    uint32_t tensorAddr;
    uint32_t dataType;
    uint32_t desc;
    uint32_t bufferId;
    uint16_t position;   // GM=0, UB=1, L1=2, L0A=3, L0B=4, L0C=5 ...
    uint16_t blockIdx;
    uint32_t dim;
    uint32_t shape[8];
    uint32_t resv1;
    uint32_t dumpSize;
};
```

字段填写逻辑位于 `set_dump_tlv_info`（公共层，不改）：`position = static_cast<uint16_t>(hardware)`，L0A/L0B 路径分别填 3/4；`dumpSize = dump_size * sizeof(T)`；`length = sizeof(DumpTensorTlv) - sizeof(uint32_t[2]) + alignDumpLen`。

超长张量走 `DumpSuperTensorTlv / DumpSuperTensorBodyTlv` 分包（需 2.3 中第 3 项把 9201 加入编译条件、把 L0A/L0B 加入 `is_super_tensor_hardware_supported` 后生效）。

## 2.6 dav_9201 debug bus 搬运设计（已就绪，此处仅说明设计）

### 2.6.1 常量定义

`npu_arch_9201/asc_aicore_dump_utils.h`（与 3510 同款）：

```cpp
constexpr uint32_t ASC_DEBUG_BUS_UNLOCK_OFFSET = 0x078;
constexpr uint32_t ASC_DEBUG_BUS_CTRL_OFFSET = 0x100;
constexpr uint32_t ASC_DEBUG_BUS_ADDR_LOW_OFFSET = 0x108;
constexpr uint32_t ASC_DEBUG_BUS_MODEL_BASE_OFFSET = 0x10c;
constexpr uint32_t ASC_DEBUG_BUS_DATA_OFFSET = 0x110;
constexpr uint32_t ASC_DEBUG_BUS_BUSY_OFFSET = 0x130;
constexpr uint32_t ASC_DEBUG_BUS_UNLOCK_VALUE = 0x19101920;
constexpr uint32_t ASC_DEBUG_BUS_READ_MASK = 0x2;
constexpr uint32_t ASC_DEBUG_BUS_CHUNK_WORDS = 8;   // 8 * 4B = 32B / 轮
constexpr uint32_t ASC_DEBUG_BUS_CHUNK_BYTES = 32;
```

### 2.6.2 model base 与地址步进映射

| 路径 | 模型基地址 | 单轮读取 | Local 地址换算 | 每轮地址步进 |
|---|---|---|---|---|
| L1 | `0x620000` | 32B | 本地字节地址右移 5 位 | 1 个 32B 地址单元 |
| L0A | `0x410000` | 32B | 本地字节地址原样（右移 0 位） | 32 个地址单位 |
| L0B | `0x420000` | 32B | 本地字节地址原样（右移 0 位） | 32 个地址单位 |

对应实现（已就绪）：

```cpp
// local addr shift: L1 -> 5（32B 粒度编址），其余 -> 0（字节编址）
// local step = loop_steplen >> addr_shift：L1 -> 1，L0A/L0B -> 32
srcLocalAddr = curLocalBaseAddr >> get_debug_bus_local_addr_shift<hardware>();
...
const int32_t loopCount = alignDumpBytes >> 5;   // 32B / 轮
for (uint32_t idx = 0; idx < loopCount; ++idx) {
    debug_bus_read_chunk<32>(debugBusAddr, srcLocalAddr, modelBase, dstGlobalAddr);
    srcLocalAddr += localStep;
    dstGlobalAddr += 32;
}
```

> 注：L0A/L0B 的 model base（0x410000/0x420000）与"字节编址 + 每轮步进 32"的地址规则当前直接沿用 3510 拷贝代码，**上板前需与 A6 芯片 debug bus 规格核对**（见风险 5.1）。

### 2.6.3 单 chunk 读取流程

`debug_bus_read_chunk<readLen>(debugBusAddr, localOffset, modelBase, dataDst)`（与 3510 一致）：

1. `sync_all()`；
2. 写 `ASC_DEBUG_BUS_UNLOCK_VALUE`（0x19101920）到 unlock 寄存器（0x078）；
3. 写 `localOffset` 到地址低位寄存器（0x108）；
4. 写 `modelBase` 到 model base 寄存器（0x10c）；
5. 写 `ASC_DEBUG_BUS_READ_MASK`（0x2）到 ctrl 寄存器（0x100）触发读取；
6. 轮询 busy 寄存器（0x130），直到 ready bit `0x80000000U` 置位，或计数超过 5 次后退出（每次轮询间 `ringbuf_wait_rts_sync() + sync_all()`）；
7. 从 data 寄存器（0x110）起逐 4B 读取共 8 个 word（`ld_dev`），通过 `st_dev` 写入 GM 目的地址；
8. `sync_all()`。

### 2.6.4 mem_copy 入口（已就绪）

```cpp
template <typename T>
__aicore__ inline uint32_t mem_copy_l1buf_to_gm_impl(__gm__ T* dst, __cbuf__ T* src, const uint32_t alignDumpBytes)
{
#if defined(__DAV_CUBE__)
    return debug_bus_copy_local_to_gm_impl<AscendC::Hardware::L1>(dst, src, alignDumpBytes, get_debug_bus_addr_impl());
#else
    return 1;
#endif
}
// mem_copy_abuf_to_gm_impl（__ca__，Hardware::L0A）、mem_copy_bbuf_to_gm_impl（__cb__，Hardware::L0B）同构。
```

非 cube 核（如 AIV）调用时返回 1，`set_dump_tlv_data` 不推进写游标，不产出不完整记录。

## 2.7 DumpTensorLocal2GMImpl 新增 L0A/L0B 分支

`dav_9201/kernel_operator_dump_tensor_impl.h` 仿照既有 L1/L0C 分支补齐：

```cpp
} else if (position == Hardware::L0A) {
    if ASCEND_IS_AIC {
        __asc_aicore::asc_dump_abuf((__ca__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim);
    }
} else if (position == Hardware::L0B) {
    if ASCEND_IS_AIC {
        __asc_aicore::asc_dump_bbuf((__cb__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim);
    }
}
```

说明：

- `asc_dump_abuf / asc_dump_bbuf` 在公共层 `asc_aicore_dump_impl.h` 已定义（带 shape 版 + 3 参版），无需新增；
- L0A/L0B 仅存在于 AIC（cube）核，`if ASCEND_IS_AIC` gating 与 L1/L0C 分支一致；
- L1 分支已存在且转发 `asc_dump_l1buf`，接线（2.3 第 1-3 项）完成后即生效，无需改动。

# 三、新增接口

## 3.1 用户接口

无新增用户接口。既有 `DumpTensor` 调用方式保持不变，本需求仅扩展内部支持的 `LocalTensor` 硬件位置（L1/L0A/L0B）。

## 3.2 内部接口

`asc_dump_abuf(__ca__ T*)`、`asc_dump_bbuf(__cb__ T*)` 及 `mem_copy_abuf_to_gm_impl`、`mem_copy_bbuf_to_gm_impl` 均为公共层/9201 架构层既有内部 helper，本次仅接线使用，不新增符号。

# 四、约束条件

1. 功能范围仅为 A6（`__NPU_ARCH__ == 9201`）AIC（cube）核；L1/L0A/L0B 仅存在于 cube 核，非 cube 路径由 `__DAV_CUBE__` gating 与 `ASCEND_IS_AIC` 双重拦截。
2. 依赖 runtime 正确初始化 `DebugBlockHeadInfo::debugBusAddr`；为 0 时该次 dump 静默跳过。
3. Dump 字节数按 32B 向上对齐，LocalTensor 必须为对齐后的读取范围预留有效空间（尾部多读部分由解析侧按 `dumpSize` 截断）。
4. L0A/L0B 数据为 cube 侧 NZ 物理存储布局（由 LoadData2D 搬入决定），dump 结果按物理存储顺序输出，**不能以与 L1 原始数据线性逐位一致作为正确性标准**，解析/校验侧需按 ND2NZ 布局组织 golden 数据。
5. L0A/L0B 的 debug bus model base 与地址步进规则沿用 3510 拷贝实现，上板前需与 A6 硬件规格核对确认。
6. CPU Debug 不执行 L1/L0A/L0B 硬件 dump，沿用既有 `asc_dump` 的断言行为。
7. 9201 无 L1/L0A/L0B → GM 的 DataCopy 直拷通路（`DataCopyL12GMImpl` 为断言 stub），debug bus 是唯一通路。
8. 其余 Dump 大小、Dump 开关、环形缓冲区容量和 Block 元数据规则均继承既有 DumpTensor 约束。

# 五、兼容性分析

## 5.1 接口兼容性

公共头文件中的 `DumpTensor` 签名、参数和调用方式不变。新增分支仅在 9201 且 LocalTensor 物理位置为 L0A/L0B 时生效（L1 分支已存在）；UB、L0C、BIAS、FIXBUF 等已有路径仍进入原实现。

## 5.2 编译兼容性

- 本次接线（`asc_debug_utils.h` / `asc_aicore_dump_impl.h` / `kernel_operator_dump_tensor_intf_impl.h` / `asc_aicore_printf_impl.h` 的 9201 分支）使 9201 从 stub 兜底切换到 `npu_arch_9201` 实现链路，同时使 ring buffer printf/时间戳等公共调试能力对 9201 生效——需在测试中对 9201 的 printf/dump 既有用例做回归确认（见风险 5.1）。
- 非 9201 架构（2201/3510/5102/5162 等）的 include 链与行为完全不变。
- `is_super_tensor_hardware_supported` 补 L0A/L0B 后，其余架构的 SuperTensor 支持范围不变（模板常量表达式，仅 9201 编译单元会实例化 L0A/L0B 分支）。
- L0A/L0B 在 `dav_9201` 原为断言路径，改为可 dump，属新增能力，无既有行为破坏。

## 5.3 行为兼容性

DebugBus 公共搬运函数通过编译期参数区分 L1 与 L0A/L0B。L1 仍按 32B 读取、地址右移 5 位；L0A/L0B 使用原地址 + 每轮步进 32，不改变已有 L1 dump 的地址计算和输出排布。解析侧 `DumpTensorTlv` 二进制布局不变。

# 六、维测分析

1. 可通过 DumpTensor 正常打印 L1/L0A/L0B 数据。
2. Dump 大小为 0、Block 信息无效（magic 错误）、环形缓冲区不足等情况继续使用既有 DumpTensor 检查和返回逻辑。
3. DebugBus 地址无效（`debugBusAddr == 0`）或搬运 helper 返回失败时，不提交本次 TLV 写指针更新，不输出不完整记录。
4. debug bus ready 轮询最多 5 次，超时退出，不死循环。
5. 非 AIC 调用 L1/L0A/L0B 路径时不产出 dump 数据（`ASCEND_IS_AIC` gating 静默跳过）。
6. 其余与 DumpTensor 保持一致。

# 七、测试用例

| 用例编号 | 场景 | 预期结果 |
|---|---|---|
| TC-1 | 9201 ring buffer 模式 dump L1 LocalTensor（多 dtype：bool/u8/i8/u16/i16/u32/i32/u64/i64/fp16/fp32/bf16） | 生成 `DumpTensorTlv`，`position=L1`，payload 与 L1 数据逐元素一致 |
| TC-2 | 9201 dump L0A LocalTensor（GM→L1 后 LoadData2D L1→L0A，再 dump） | 生成 `DumpTensorTlv`，`position=L0A`，payload 与按 NZ 布局组织的 golden 数据一致 |
| TC-3 | 9201 dump L0B LocalTensor（同上，LoadData2D L1→L0B） | 生成 `DumpTensorTlv`，`position=L0B`，payload 与 golden 一致 |
| TC-4 | 9201 dump 超长 L1/L0A/L0B 张量（超过 ringBufLen） | 走 SuperTensor 分包路径，分包重组后数据完整 |
| TC-5 | `debugBusAddr == 0` 时 dump L1/L0A/L0B | 安全返回，不产出 TLV，不推进写游标，无残留 |
| TC-6 | AIV 核 dump L1/L0A/L0B | 不进入 dump 分支（静默跳过），无错误数据 |
| TC-7 | 非 9201 架构（2201/3510/5102 等）编译回归 | 编译不因本次变更失败，既有 dump 行为不变 |
| TC-8 | 9201 CPU debug 模式 | 沿用断言行为 |
| TC-9 | 9201 既有 UB/GM dump 及 printf 用例回归 | 输出排布与历史预期不变 |

> 冒烟用例建议参照 `ascendc_smoke_liux/app/00_compile_run/00_asc/arch35/asc_case/debug_dump_api`（L1 多 dtype dump 模板）移植到 9201 冒烟目录；L0A/L0B 用例可基于 `mmad` 类算子在 LoadData2D 后插入 dump 点构造。

# 八、风险与注意事项

1. **L0A/L0B 硬件规格核对**：model base（0x410000/0x420000）、字节编址、每轮 32B 步进均沿用 3510 拷贝代码，A6 芯片 debug bus 若有差异，需在 `npu_arch_9201/asc_aicore_dump_utils.h` 内修正（仅 arch 私有文件，不影响公共层）。
2. **接线影响面**：9201 此前整体走 stub 兜底，本次接线后 printf/dump 公共链路同时激活，`asc_debug_utils_impl.h`（9201 版，与 3510 同构）中的 `get_block_info`/ringbuf 流控需与 runtime 侧 block header 初始化联调确认。
3. **NZ 布局校验**：L0A/L0B 的 golden 数据必须按 LoadData2D 的 ND2NZ 存储布局组织，避免误判为数据错误。
4. **debug bus 是慢速通路**：大张量 dump 按每轮 32B（8 次寄存器读 + 8 次 GM 写）推进，耗时随数据量线性增长，依赖 ringbuf 流控（RTS 消费）兜底，不会丢数据但会拖慢 kernel。

> （注：部分内容由 AI 生成）
