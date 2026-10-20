# 【AscendC】A6（9201）AscendC 支持 BT/FP Buffer 的数据 dump

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

## 2.1 调用链与位置分发

用户仍调用现有 `DumpTensor(LocalTensor, desc, dumpSize)`。9201 实现根据 `LocalTensor::GetPosition()` 获取物理位置：BIAS 转换为 `__biasbuf__` 地址空间，FIXBUF 转换为 `__fbuf__` 地址空间，然后进入现有 asc_dump 模板链路。

```cpp
DumpTensor
  -> DumpTensorLocal2GMImpl
  -> asc_dump(__biasbuf__ T* / __fbuf__ T*)
  -> asc_dump_local_impl<Hardware::BIAS / Hardware::FIXBUF>
  -> asc_dump_impl
  -> set_dump_tlv_data
  -> mem_copy_biasbuf_to_gm_impl / mem_copy_fbuf_to_gm_impl
  -> debug_bus_copy_local_to_gm_impl
```

## 2.2 Dump 数据组织

TLV 头、Block 信息、环形缓冲区申请和写指针更新沿用既有 DumpTensor 实现。`dumpSize` 表示元素个数，实际字节数为 `dumpSize * sizeof(T)`，内部向上对齐到 32B 后搬运。调用方必须保证 LocalTensor 从起始地址到对齐后长度均为可读空间。

`set_dump_tlv_data` 通过编译期硬件类型选择搬运函数：BIAS 调用 `mem_copy_biasbuf_to_gm_impl`，FIXBUF 调用 `mem_copy_fbuf_to_gm_impl`。模型基地址、单轮读取长度、地址换算和步长均通过 `constexpr` 计算，避免为不同 Buffer 引入运行时分支。

## 2.3 DebugBus 参数

| 路径 | 模型基地址 | 单轮读取 | Local 地址换算 | 每轮地址步进 |
|---|---|---|---|---|
| 既有 L1 | `0x620000` | 32B | 本地字节地址右移 5 位 | 1 个 32B 地址单元 |
| BT / BIAS | `0x480000` | 8B | 原地址 | 8 个地址单位 |
| FP / FIXBUF | `0x480000` | 8B | 先解析通道，再使用低 16 位地址 | 8 个地址单位 |

BT/FP 每轮执行两次 32bit DebugBus 寄存器读取并写入 GM，共输出 8B；循环次数按对齐后字节数右移 3 位计算。GM 目的地址和 BT/FP 源地址每轮均前进 8B，因此连续打印不会把同一 8B 重复四次。

## 2.4 BT 打印

BT 使用 `mem_copy_biasbuf_to_gm_impl`，DebugBus 地址为 `0x480000 + localAddr`。该通路不修改数据位，Dump 结果应与 L1 搬入 BT 前的数据逐元素一致。

| 项目 | 要求 |
|---|---|
| 支持数据类型 | `float`、`int32_t` |
| L1 源地址 | 32B 对齐 |
| BT 目的地址 | 64B 对齐 |
| 输出规则 | 透明输出，不做掩码或重排 |

## 2.5 FP 地址解析

FP 传入地址的高 16 位标识通道，低 16 位表示通道内地址。当前仅支持 quant 和 relu 两个值，其他高 16 位值不在本需求支持范围内。

| 高 16 位 | 通道 | 模型地址补偿 | 实际 DebugBus 基址 |
|---|---|---|---|
| `0` | quant | `+ 0x2000` | `0x482000 + low16` |
| `1` | relu | `+ 0x1000` | `0x481000 + low16` |

```cpp
uint32_t channel = localAddr >> 16;
uint32_t offset = localAddr & 0xffff;
uint32_t busAddr = 0x480000 + offset;
if (channel == 0) {
    busAddr += 0x2000;
} else if (channel == 1) {
    busAddr += 0x1000;
}
```

## 2.6 FP 数据规则

FP 通路按 `uint64_t` 打印。Dump 值反映 Fixpipe Buffer 的硬件有效位，不承诺与 L1 原始值逐位一致。

| 通道 | 有效位 | 期望值计算 |
|---|---|---|
| quant | 每 8B 的 bit[7:0]、bit[31:13]、bit[46:37] | `input & 0x00007fe0ffffe0ffULL` |
| relu | 每个 4B 字的 bit[31:13] | `word & 0xffffe000U` |

FP 的 L1 源地址要求 32B 对齐，Fixpipe 目的地址要求 128B 对齐。

## 2.7 DebugBus 搬运设计

### 2.7.1 常量定义

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

### 2.7.2 model base 与地址步进映射

```cpp
constexpr uint32_t get_debug_bus_model_base() {
    if constexpr (hardware == Hardware::BIAS || hardware == Hardware::FIXBUF) {
        return 0x480000U;
    }
}
```

BT/FP 的地址步进与 L1 不同：L1 按 32B 粒度编址（本地地址右移 5 位，每轮步进 1），BT/FP 按字节编址（原地址，每轮步进 8）。

### 2.7.3 单 chunk 读取流程

`debug_bus_read_chunk<8>(debugBusAddr, localOffset, modelBase, dataDst)`：

1. `sync_all()`；
2. 写 `ASC_DEBUG_BUS_UNLOCK_VALUE` 到 unlock 寄存器；
3. 写 `localOffset` 到地址低位寄存器；
4. 写 `modelBase` 到 model base 寄存器；
5. 写 `ASC_DEBUG_BUS_READ_MASK` 到 ctrl 寄存器触发读取；
6. 轮询 busy/ready 寄存器，直到 ready bit `0x80000000U` 置位，或计数超过 5 次后退出；
7. 读取 data 寄存器共 2 个 word（8B），通过 `st_dev` 写入目标 GM 地址；
8. `sync_all()`。

### 2.7.4 多 chunk 搬运流程

`debug_bus_copy_local_to_gm_impl<Hardware::BIAS / Hardware::FIXBUF>`：

```cpp
const int32_t loopCount = alignDumpBytes >> 3;  // 8B per chunk
for (uint32_t idx = 0; idx < loopCount; ++idx) {
    debug_bus_read_chunk<8>(debugBusAddr, srcLocalAddr, modelBase, dstGlobalAddr);
    srcLocalAddr += 8;
    dstGlobalAddr += 8;
}
```

FIXBUF 通路在进入循环前需先解析通道（见 2.5），将 `srcLocalAddr` 修正为 `0x480000 + 通道补偿 + low16` 后再进入循环。

# 三、新增接口

## 3.1 用户接口

```C++
template<typename T>
__aicore__ inline void asc_dump(__biasbuf__ T* input, uint32_t desc, uint32_t dump_size);

template<typename T>
__aicore__ inline void asc_dump(__fbuf__ T* input, uint32_t desc, uint32_t dump_size);
```

# 四、约束条件

1. 功能范围仅为 A6（9201）AIC；

2. BT 仅支持通过 L1 搬入的 `float` 和 `int32_t` 数据；FP 当前仅按 `uint64_t` 验证和支持。

3. FP 地址高 16 位仅支持 `0`（quant）和 `1`（relu）；其他值不做功能保证，也不新增运行时错误码。

4. BT 的 L1 源地址需 32B 对齐，BT 目的地址需 64B 对齐；FP 的 L1 源地址需 32B 对齐，Fixpipe 目的地址需 128B 对齐。

5. Dump 字节数按 32B 向上对齐，LocalTensor 必须为对齐后的读取范围预留有效空间。

6. quant 和 relu 输出受 Fixpipe 硬件位规则约束，不能以与 L1 原始值全位相等作为正确性标准。

7. 依赖 runtime 正确初始化 `DebugBlockHeadInfo::debugBusAddr`；`debugBusAddr == 0` 时该次 dump 静默跳过，不产出 TLV。

8. CPU Debug 不执行 BT/FP 硬件 Dump；调用该路径会沿用既有 `asc_dump` 的断言行为。

9. 其余 Dump 大小、Dump 开关、环形缓冲区容量和 Block 元数据规则均继承既有 DumpTensor 约束。

# 五、兼容性分析

## 5.1 接口兼容性

公共头文件中的 DumpTensor 签名、参数和调用方式不变。新增分支仅在 9201 且 LocalTensor 物理位置为 BIAS 或 FIXBUF 时生效；UB、L1、L0C 等已有路径仍进入原实现。

## 5.2 编译兼容性

- 9201 的 BIAS/FIXBUF 地址空间重载同时提供三参数和五参数内部 `asc_dump` 入口，以兼容当前公共调用链。

- CPU Debug 所需的 `__biasbuf__`、`__fbuf__` 地址空间定义由 tools 仓 stub 定义提供；devkit 功能实现不增加 CPU 专用隔离。

- 非 9201 架构仅补齐同名内部 helper 桩函数并返回失败，用于保证公共模板可解析，不开放 BT/FP 功能。

- 新增模板不得与架构头文件中的既有 `mem_copy_abuf_to_gm_impl`、`mem_copy_bbuf_to_gm_impl` 重复定义，避免非目标架构发生重定义编译失败。

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
| BT float | 256B 连续 float 数据，GM -> L1 -> BIAS，分别 Dump L1 和 BT | BT 与 L1 逐元素一致，前后 128B 均连续完整 |
| BT int32_t | 256B 连续 int32_t 数据，GM -> L1 -> BIAS，分别 Dump L1 和 BT | BT 与 L1 逐元素一致，无 8B 重复或中间断段 |
| FP quant | 256B uint64_t 数据，分两个 128B 块搬入 quant 通道并连续 Dump | 每个 8B 值等于输入值与 `0x00007fe0ffffe0ff` 的按位与结果，跨 128B 边界连续正确 |
| FP relu | 256B 数据按 4B 字组织，分两个 128B 块搬入 relu 通道并连续 Dump | 每个 4B 字等于输入值与 `0xffffe000` 的按位与结果，跨 128B 边界连续正确 |
| `debugBusAddr == 0` | 不初始化 debugBusAddr 时 Dump BT/FP | 安全返回，不产出 TLV，不推进写游标 |
| 已有路径回归 | 执行既有 UB、L1、L0C DumpTensor 用例 | 输出排布和历史预期不变 |

> （注：部分内容由 AI 生成）
