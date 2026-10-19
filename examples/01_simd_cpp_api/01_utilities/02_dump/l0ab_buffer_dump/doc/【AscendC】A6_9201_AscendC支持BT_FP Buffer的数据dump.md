# 【AscendC】A6（9201）AscendC 支持 BT/FP Buffer 的数据 dump

> 参照 dav_3510 设计文档《【AscendC】AscendC支持BT_FP Buffer的数据dump》编写。
> 与 3510 的关键差异：3510 需要在 `npu_arch_3510` 与 `dav_3510` 中新增 BIAS/FIXBUF 分支与搬运实现；
> A6（dav_9201，`__NPU_ARCH__ == 9201`）侧 **`dav_9201` 的 BIAS/FIXBUF 分发分支与
> `npu_arch_9201` 的 `mem_copy_biasbuf_to_gm_impl / mem_copy_fbuf_to_gm_impl` 均已从 3510 拷贝就绪**，
> 本需求的主要工作是架构分发接线（与《A6_9201 支持L1的数据dump》共用同一套接线改动）。

# 一、需求描述

## 1.1 需求概述

在 A6（9201）AIC 场景下，扩展现有 `DumpTensor` 的 LocalTensor 打印能力，使其支持 Bias Table（BT）和 Fixpipe Buffer（FP）。本需求复用既有 DumpTensor 接口、TLV 元数据、Dump 环形缓冲区和 DebugBus 搬运链路，不新增用户接口。

| 逻辑 Buffer | TPosition | 物理位置 | 地址空间类型 |
|---|---|---|---|
| BT | C2 | BIAS | `__biasbuf__` |
| FP | C2PIPE2GM | FIXBUF | `__fbuf__` |

## 1.2 资料变更

需在既有 DumpTensor 资料中补充 A6（9201）芯片对 BT/FP Buffer 的支持范围、数据类型、地址编码、对齐要求和 FP 数据位规则；不新增独立 API 文档。

## 1.3 DFX 与验收目标

- A6（9201）AIC 上可以通过现有 `DumpTensor` 接口打印 BT 和 FP LocalTensor。
- BT 的 `float`、`int32_t` 数据按实际存储值完整输出。
- FP 的 quant 和 relu 两种通道按硬件定义的有效位规则输出。

# 二、方案详述

## 2.1 现状与接线（与 3510 的差异）

**已就绪（无需实现）**：

1. `impl/basic_api/dav_9201/kernel_operator_dump_tensor_impl.h` 的 `DumpTensorLocal2GMImpl`（L55-62）**BIAS/FIXBUF 分支已存在**：AIC 侧分别转发
   `__asc_aicore::asc_dump((__biasbuf__ T*)src.GetPhyAddr(), desc, dumpSize, shape, shapeDim)` 与
   `__asc_aicore::asc_dump((__fbuf__ T*)...)`。
2. 公共层 `impl/utils/debug/asc_aicore_dump_impl.h` 已具备：`asc_dump(__biasbuf__ T*)`（L342/416）、`asc_dump(__fbuf__ T*)`（L349/422）重载（带 shape 版 + 3 参版），`set_dump_tlv_data` 的 BIAS/FIXBUF 分发（L100-103）。即 3510 文档"三、新增接口"中的两个 `asc_dump` 内部入口在当前公共层**已经存在，无需新增**。
3. `impl/utils/debug/npu_arch_9201/asc_aicore_dump_utils.h` 已从 3510 拷贝，包含：
   - `mem_copy_biasbuf_to_gm_impl`（L241-249，`__biasbuf__` → `debug_bus_copy_local_to_gm_impl<Hardware::BIAS>`）；
   - `mem_copy_fbuf_to_gm_impl`（L252-260，`__fbuf__` → `debug_bus_copy_local_to_gm_impl<Hardware::FIXBUF>`）；
   - `debug_bus_copy_local_to_gm_impl` 中的 FIXBUF quant/relu 通道特判（L160-168）；
   - `get_debug_bus_model_base`：BIAS/FIXBUF 均为 `0x480000`；`get_debug_bus_loop_shift`：BIAS/FIXBUF 为 3（每轮 8B）。

**未接线（本次主要改动点，与 L1 dump 需求共用）**：

1. `impl/utils/debug/asc_debug_utils.h`（L29-41）：arch 分发链无 `__NPU_ARCH__ == 9201` 分支，9201 编译时落入 stub，`get_block_info`（`debugBusAddr` 来源）、ringbuf 流控原语不可用；`npu_arch_9201/asc_debug_utils_impl.h` 已备好未接。
2. `impl/utils/debug/asc_aicore_dump_impl.h`（L24-32）：arch include 链无 9201 分支，`npu_arch_9201/asc_aicore_dump_utils.h` 成为死代码；另 `asc_dump_impl` 超长 SuperTensor 分包编译条件（L235）不含 9201，需一并补齐（`is_super_tensor_hardware_supported` 已含 BIAS/FIXBUF）。
3. `impl/basic_api/kernel_operator_dump_tensor_intf_impl.h`（L24-50）：顶部公共 include 条件与 arch include 链均无 9201 分支，`dav_9201/kernel_operator_dump_tensor_impl.h` 未被任何头文件包含。
4. `impl/utils/debug/asc_aicore_printf_impl.h`（L21-33）：与 `npu_arch_9201/asc_aicore_printf_utils.h` 同步补 9201 接线（与 dump 共用 ring buffer 框架）。

## 2.2 调用链与位置分发

用户仍调用现有 `DumpTensor(LocalTensor, desc, dumpSize)`。9201 实现根据 `LocalTensor::GetPosition()` 获取物理位置：BIAS 转换为 `__biasbuf__` 地址空间，FIXBUF 转换为 `__fbuf__` 地址空间，然后进入现有 asc_dump 模板链路（分支均为既有代码，接线后生效）：

```
DumpTensor
  -> DumpTensorLocal2GMImpl                     // dav_9201（BIAS/FIXBUF 分支已存在）
  -> asc_dump(__biasbuf__ T* / __fbuf__ T*)     // asc_aicore_dump_impl.h（公共层既有重载）
  -> asc_dump_local_impl<Hardware::BIAS / Hardware::FIXBUF>
  -> asc_dump_impl
  -> set_dump_tlv_data
  -> mem_copy_biasbuf_to_gm_impl / mem_copy_fbuf_to_gm_impl   // npu_arch_9201（已就绪）
  -> debug_bus_copy_local_to_gm_impl<Hardware::BIAS / Hardware::FIXBUF>
  -> debug_bus_read_chunk（每轮 8B）
```

## 2.3 Dump 数据组织

TLV 头、Block 信息、环形缓冲区申请和写指针更新沿用既有 DumpTensor 实现。`dumpSize` 表示元素个数，实际字节数为 `dumpSize * sizeof(T)`，内部向上对齐到 32B 后搬运。调用方必须保证 LocalTensor 从起始地址到对齐后长度均为可读空间。

`set_dump_tlv_data` 通过编译期硬件类型选择搬运函数：BIAS 调用 `mem_copy_biasbuf_to_gm_impl`，FIXBUF 调用 `mem_copy_fbuf_to_gm_impl`。模型基地址、单轮读取长度、地址换算和步长均通过 `constexpr` 计算，避免为不同 Buffer 引入运行时分支。

## 2.4 DebugBus 参数

| 路径 | 模型基地址 | 单轮读取 | Local 地址换算 | 每轮地址步进 |
|---|---|---|---|---|
| 既有 L1 | `0x620000` | 32B | 本地字节地址右移 5 位 | 1 个 32B 地址单元 |
| BT / BIAS | `0x480000` | 8B | 原地址（右移 0 位） | 8 个地址单位 |
| FP / FIXBUF | `0x480000` | 8B | 先解析通道，再使用低 16 位地址 | 8 个地址单位 |

BT/FP 每轮执行两次 32bit DebugBus 寄存器读取并写入 GM，共输出 8B；循环次数按对齐后字节数右移 3 位计算（`loopCount = alignDumpBytes >> 3`）。GM 目的地址和 BT/FP 源地址每轮均前进 8B，因此连续打印不会把同一 8B 重复四次。

对应实现（`npu_arch_9201/asc_aicore_dump_utils.h`，已就绪）：

```cpp
// BIAS / FIXBUF：loop_shift = 3（8B/轮），addr_shift = 0（原地址），localStep = 8
const int32_t loopCount = alignDumpBytes >> 3;
for (uint32_t idx = 0; idx < loopCount; ++idx) {
    debug_bus_read_chunk<8>(debugBusAddr, srcLocalAddr, modelBase /*0x480000*/, dstGlobalAddr);
    srcLocalAddr += 8;
    dstGlobalAddr += 8;
}
```

## 2.5 BT 打印

BT 使用 `mem_copy_biasbuf_to_gm_impl`，DebugBus 地址为 `0x480000 + localAddr`。该通路不修改数据位，Dump 结果应与 L1 搬入 BT 前的数据逐元素一致。

| 项目 | 要求 |
|---|---|
| 支持数据类型 | `float`、`int32_t` |
| L1 源地址 | 32B 对齐 |
| BT 目的地址 | 64B 对齐 |
| 输出规则 | 透明输出，不做掩码或重排 |

9201 的 L1 → BT 搬运通路：`dav_9201/kernel_operator_data_copy_impl.h` 提供 `copy_cbuf_to_bt`（L1 → BIAS）内联指令封装，BT 数据准备方式与 3510 一致（GM → L1 → BIAS）。

## 2.6 FP 地址解析

FP 传入地址的高 16 位标识通道，低 16 位表示通道内地址。当前仅支持 quant 和 relu 两个值，其他高 16 位值不在本需求支持范围内。

| 高 16 位 | 通道 | 模型地址补偿 | 实际 DebugBus 基址 |
|---|---|---|---|
| `0` | quant | `+ 0x2000` | `0x482000 + low16` |
| `1` | relu | `+ 0x1000` | `0x481000 + low16` |

对应实现（`debug_bus_copy_local_to_gm_impl` 的 FIXBUF 特判，已就绪）：

```cpp
constexpr uint32_t ASC_DEBUG_BUS_FBUF_TYPE_SHIFT = 16;
constexpr uint32_t ASC_DEBUG_BUS_FBUF_ADDR_MASK = 0xFFFFU;

if constexpr (hardware == AscendC::Hardware::FIXBUF) {
    const bool isRelu = (curLocalBaseAddr >> ASC_DEBUG_BUS_FBUF_TYPE_SHIFT) == 0x1;
    const bool isQuant = (curLocalBaseAddr >> ASC_DEBUG_BUS_FBUF_TYPE_SHIFT) == 0x0;
    srcLocalAddr = curLocalBaseAddr & ASC_DEBUG_BUS_FBUF_ADDR_MASK;
    if (isQuant) {
        srcLocalAddr += 0x2000U;
    } else if (isRelu) {
        srcLocalAddr += 0x1000U;
    }
}
```

## 2.7 FP 数据规则

FP 通路按 `uint64_t` 打印。Dump 值反映 Fixpipe Buffer 的硬件有效位，不承诺与 L1 原始值逐位一致。

| 通道 | 有效位 | 期望值计算 |
|---|---|---|
| quant | 每 8B 的 bit[7:0]、bit[31:13]、bit[46:37] | `input & 0x00007fe0ffffe0ffULL` |
| relu | 每个 4B 字的 bit[31:13] | `word & 0xffffe000U` |

FP 的 L1 源地址要求 32B 对齐，Fixpipe 目的地址要求 128B 对齐。L1 到 Fixpipe 的 DataCopy 指令搬运粒度（3510 上以 64B 为 `blockLen` 单位）以 9201 的 `dav_9201` fixpipe data copy 实现为准，上板验证时按实际指令规格组织用例。

# 三、新增接口

## 3.1 用户接口

无新增用户接口。既有 `DumpTensor` 调用方式保持不变。

## 3.2 内部接口

3510 文档中列出的两个内部 `asc_dump` 入口在当前公共层 `asc_aicore_dump_impl.h` **已存在**（带 shape 版 + 3 参版），9201 直接复用，无需新增：

```cpp
template<typename T>
__aicore__ inline void asc_dump(__biasbuf__ T* input, uint32_t desc, uint32_t dump_size);

template<typename T>
__aicore__ inline void asc_dump(__fbuf__ T* input, uint32_t desc, uint32_t dump_size);
```

# 四、约束条件

1. 功能范围仅为 A6（`__NPU_ARCH__ == 9201`）AIC（`dav_9201` 的 BIAS/FIXBUF 分支均带 `ASCEND_IS_AIC` gating，且 `mem_copy_biasbuf/fbuf_to_gm_impl` 带 `__DAV_CUBE__` gating）；
2. BT 仅支持通过 L1 搬入的 `float` 和 `int32_t` 数据；FP 当前仅按 `uint64_t` 验证和支持；
3. FP 地址高 16 位仅支持 `0`（quant）和 `1`（relu）；其他值不做功能保证，也不新增运行时错误码；
4. BT 的 L1 源地址需 32B 对齐，BT 目的地址需 64B 对齐；FP 的 L1 源地址需 32B 对齐，Fixpipe 目的地址需 128B 对齐；
5. Dump 字节数按 32B 向上对齐，LocalTensor 必须为对齐后的读取范围预留有效空间；
6. quant 和 relu 输出受 Fixpipe 硬件位规则约束，不能以与 L1 原始值全位相等作为正确性标准（位规则沿用 3510 规格，A6 上板时需验证一致性）；
7. 依赖 runtime 正确初始化 `DebugBlockHeadInfo::debugBusAddr`；`debugBusAddr == 0` 时该次 dump 静默跳过，不产出 TLV；
8. CPU Debug 不执行 BT/FP 硬件 Dump；调用该路径会沿用既有 `asc_dump` 的断言行为；
9. 其余 Dump 大小、Dump 开关、环形缓冲区容量和 Block 元数据规则均继承既有 DumpTensor 约束。

# 五、兼容性分析

## 5.1 接口兼容性

公共头文件中的 DumpTensor 签名、参数和调用方式不变。BT/FP 分支仅在 9201 且 LocalTensor 物理位置为 BIAS 或 FIXBUF 时生效（分支为 `dav_9201` 既有代码）；UB、L1、L0C 等已有路径仍进入原实现。

## 5.2 编译兼容性

- 本次接线（`asc_debug_utils.h` / `asc_aicore_dump_impl.h` / `kernel_operator_dump_tensor_intf_impl.h` / `asc_aicore_printf_impl.h` 补 9201 分支）与 L1 dump 需求共用，使 9201 从 stub 兜底切换到 `npu_arch_9201` 实现链路；需同步回归 9201 的 printf/dump 既有用例；
- 非 9201 架构（2201/3510/5102/5162 等）的 include 链与行为完全不变；
- `mem_copy_biasbuf_to_gm_impl`、`mem_copy_fbuf_to_gm_impl` 在 `npu_arch_9201` 中已存在，不与其它架构头文件产生重复定义（各 arch 头文件互斥 include）；
- `__biasbuf__`、`__fbuf__` 地址空间定义由 tools 仓 stub 提供，devkit 功能实现不增加 CPU 专用隔离。

## 5.3 行为兼容性

DebugBus 公共搬运函数通过编译期参数区分 L1 与 BT/FP。L1 仍按 32B 读取、地址右移 5 位；BT/FP 使用 8B 读取和各自地址规则，因此不会改变已有 L1 Dump 的地址计算和输出排布。

# 六、维测分析

- Dump 大小为 0、Block 信息无效、环形缓冲区不足等情况继续使用既有 DumpTensor 检查和返回逻辑。
- DebugBus 地址无效（`debugBusAddr == 0`）或搬运 helper 返回失败时，不提交本次 TLV 写指针更新，不输出不完整记录。
- 非 AIC 或非目标架构调用 BT/FP 内部 helper 时返回失败，不伪造 Dump 数据。
- FP 高 16 位为非 0/1 时属于未支持输入，当前不新增分支处理；调用方必须在接口约束内传参。
- debug bus ready 轮询最多 5 次，超时退出，不死循环。

# 七、测试用例

## 7.1 功能与精度测试

| 用例 | 输入与操作 | 预期结果 |
|---|---|---|
| BT float | 256B 连续 float 数据，GM → L1 → BIAS，分别 Dump L1 和 BT | BT 与 L1 逐元素一致，前后 128B 均连续完整 |
| BT int32_t | 256B 连续 int32_t 数据，GM → L1 → BIAS，分别 Dump L1 和 BT | BT 与 L1 逐元素一致，无 8B 重复或中间断段 |
| FP quant | 256B uint64_t 数据，分两个 128B 块搬入 quant 通道并连续 Dump | 每个 8B 值等于输入值与 `0x00007fe0ffffe0ff` 的按位与结果，跨 128B 边界连续正确 |
| FP relu | 256B 数据按 4B 字组织，分两个 128B 块搬入 relu 通道并连续 Dump | 每个 4B 字等于输入值与 `0xffffe000` 的按位与结果，跨 128B 边界连续正确 |
| 超长 BT/FP 张量（超过 ringBufLen） | 大尺寸 BIAS/FIXBUF 张量 Dump | 走 SuperTensor 分包路径（接线时把 9201 加入 `asc_dump_impl` L235 编译条件），分包重组后数据完整 |
| `debugBusAddr == 0` | 不初始化 debugBusAddr 时 Dump BT/FP | 安全返回，不产出 TLV，不推进写游标 |
| 已有路径回归 | 执行既有 UB、L1、L0C DumpTensor 用例 | 输出排布和历史预期不变 |

## 7.2 用例来源

冒烟用例直接参照 `ascendc_smoke_liux/app/00_compile_run/00_asc/arch35/asc_case/btfp_dump/` 下的三个既有 3510 用例移植到 9201 冒烟目录：

- `bias_table_raw_probe`（BT：src/bias 双 dump 对比）；
- `fixpipe_quant_bit_rule_probe`（FP quant 位规则探测）；
- `fixpipe_relu_bit_rule_probe`（FP relu 位规则探测）。

各用例自带 `decode_run.py` 解码脚本（正则解析 `DumpTensor: desc=N`），可复用。

# 八、风险与注意事项

1. **位规则与硬件规格核对**：quant/relu 有效位掩码、FP 通道地址补偿（+0x2000/+0x1000）、BT/FP model base（0x480000）均沿用 3510 拷贝代码，A6 芯片若与 3510 存在规格差异，需在 `npu_arch_9201/asc_aicore_dump_utils.h` 内修正（仅 arch 私有文件，不影响公共层）。
2. **接线影响面**：与 L1 dump 需求共用同一套接线，9201 此前整体走 stub 兜底，接线后 printf/dump 公共链路同时激活，需与 runtime 侧 block header 初始化（`debugBusAddr` 写入）联调确认。
3. **依赖顺序**：本需求依赖"9201 架构接线"合入（与《A6_9201 支持L1的数据dump》共用改动），建议两需求同版本交付或先行合入接线。
4. **debug bus 是慢速通路**：BT/FP 按每轮 8B（2 次寄存器读 + 2 次 GM 写）推进，dump 耗时随数据量线性增长，依赖 ringbuf 流控（RTS 消费）兜底。

> （注：部分内容由 AI 生成）
