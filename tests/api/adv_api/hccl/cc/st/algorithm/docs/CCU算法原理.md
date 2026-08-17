# CCU 算法原理：编程模型、微码编译与执行机制

> 文档状态：2026-07-28。
> 代码基线：当前本地 `hccl`、`asc-devkit` 及 `asc-comm@pr24`。
> 迁移决策：CCU 算法和 ST 均直接使用 PR24 `AscendC::ccu` 新 API；老 `hcomm::CcuKernel`/`hcomm::CcuRep` 算法接口只作语义参考，不建设中间兼容层。

## 1. 结论先行

CCU 算法不是 Host 调用 `ccu::Write()` 时立即搬运数据，而是一种“注册期构建程序，Launch 期绑定参数，CCU 硬件执行微码”的声明式模型。

```text
函数式 CCU Kernel
  -> ccu::Variable/Write/Event/Loop
  -> 底层 Ccu* C API
  -> 内部 Representation(IR)
  -> Translator
  -> CcuInstr 微码
  -> 加载到 CCU 指令空间
  -> Launch 绑定 taskArgs
  -> rtCCULaunch
  -> CCU 按微码执行通信
```

其中最容易混淆的几点是：

1. 普通 C++ 编译只把 Kernel `.cc` 编译进库，不生成最终 CCU 微码。
2. `HcommCcuKernelRegister()` 执行 Kernel 函数，生成 Representation 和资源请求。
3. `HcommCcuKernelRegisterEnd()` 将 Representation 翻译为微码并加载到设备。
4. `HcommCcuKernelLaunch()` 不重新编译算法，只绑定当次 `taskArgs`并下发已注册 Kernel。
5. 新 API 的内部实现仍使用 `hcomm::CcuRep` 作为编译器 IR；“不用老 API”指算法不再直接依赖这套内部对象模型，不是删除 Representation 层。

## 2. 软件分层

```text
算子入口 / selector / executor
  -> CCU template
       - CalcRes：Channel、Thread、Kernel 函数和静态参数
       - PrepareLaunchArgs：构造每次执行的动态参数
  -> CCU Kernel
       - 函数式 CcuResult XxxKernel(CcuKernelArg)
       - 通过 ccu:: 类型和原语描述算法
  -> PR24 C++ API
       - Variable/Address/LocalAddr/RemoteAddr/Event/CcuBuffer
       - LoadArg/Write/Read/LocalCopy/Reduce/Notify/Loop
  -> C API adapter
       - CcuVariableAlloc/CcuLoadArg/CcuWriteMemToMem/...
  -> CcuKernel + Representation
  -> CcuRepTranslator + CcuInstr
  -> CCU Resource/Driver/Runtime
```

### 2.1 新老 API 的边界

| 维度 | asc-devkit 现有老接口 | PR24 目标新接口 |
|---|---|---|
| Kernel | `class Xxx : CcuKernelAlgBase` | `CcuResult XxxKernel(CcuKernelArg)` |
| 算法类型 | `CcuRep::Variable/LocalAddr/...` | `ccu::Variable/LocalAddr/...` |
| 原语 | `WriteNb/ReadNb/LocalCopyNb` | `ccu::Write/Read/LocalCopy` |
| 注册 | `HcclCcuKernelRegister(KernelCreator, ...)` | `HcommCcuKernelRegister(kernelFunc, ...)` |
| 动态参数 | `CcuTaskArg -> GeneArgs()` | `uint64_t taskArgs[] -> ccu::LoadArg()` |

两套接口表达的硬件概念相同：XN Variable、GSA Address、CKE Event、CcuBuffer/MS、Channel、本地/远端地址、读写、Reduce 和 Loop。改变的是算法编程表面和模块边界。

## 3. 算法可见的新 API 数据结构

### 3.1 Handle

PR24 对外类型基本都是 64 位 handle：

```cpp
using CcuInsHandle        = uint64_t;
using CcuKernelHandle     = uint64_t;
using CcuVariableHandle   = uint64_t;
using CcuAddressHandle    = uint64_t;
using CcuEventHandle      = uint64_t;
using CcuBufferHandle     = uint64_t;
using CcuLocalAddrHandle  = uint64_t;
using CcuRemoteAddrHandle = uint64_t;
using CcuKernelArg        = void *;
```

Handle 不是实际数据值，而是注册期的虚拟资源标识。物理 XN/GSA/CKE/MS 在后续资源分配和 Translate 过程中绑定。

### 3.2 Variable 和 Address

```cpp
class Variable {
public:
    CcuVariableHandle handle{0};
};

class Address {
public:
    CcuAddressHandle handle{0};
};
```

`Variable` 对应 CCU XN 标量资源，`Address` 对应 GSA 地址资源。运算符重载不会在 CPU 上计算出最终数值，而是生成 Assign/Add Representation：

```cpp
ccu::Variable size;
ccu::Address srcAddr;

size = 4096;       // 生成 immediate -> XN
srcAddr = input;   // 生成 XN/Address 赋值
srcAddr += offset; // 生成 Address + Variable
```

### 3.3 LocalAddr 和 RemoteAddr

```cpp
class LocalAddr {
public:
    Address addr;
    Variable token;
    CcuLocalAddrHandle handle{0};
};

class RemoteAddr {
public:
    Address addr;
    Variable token;
    CcuRemoteAddrHandle handle{0};
};
```

CCU 访问内存时需要地址和 token 成对出现。`LocalAddr` 和 `RemoteAddr` 形态相同，但对原语的本地/远端方向有类型约束。

```cpp
ccu::LocalAddr src;
src.addr = input;
src.addr += inputOffset;
src.token = localToken;

ccu::RemoteAddr dst;
dst.addr = peerOutput;
dst.addr += outputOffset;
dst.token = peerToken;
```

### 3.4 Event 和 CcuBuffer

```cpp
class Event {
public:
    CcuEventHandle handle{0};
};

class CcuBuffer {
public:
    CcuBufferHandle handle{0};
};
```

- `Event` 对应 CKE 完成事件资源，mask 不保存在 Event 对象里，而是每次 Record/Wait 独立传入。
- `CcuBuffer` 对应 CCU 管理的 MS/中转资源，常用于 `LocalCopy(Mem -> Buffer -> Mem)` 和 Buffer Reduce。

## 4. CCU 硬件资源与执行机制

### 4.1 总体硬件模型

CCU 是位于 IO Die 上的集合通信专用协处理器。它不是通用 CPU，也不直接执行 Kernel C++ 源码；Host 先将 `ccu::` API 记录并翻译成微码，运行时再由 Mission 执行这些微码。

```text
Host / HCOMM
  |
  | Register：Representation -> CcuInstr -> 指令空间
  | Launch：CCU SQE + CcuTaskParam
  v
+--------------------------- CCU / IO Die ---------------------------+
| Mission：顶层微码执行上下文                                        |
|   +-- Instruction：CCU微码                                         |
|   +-- XN：长度、offset、token、计数器、条件值                       |
|   +-- GSA：本端/远端HBM地址                                        |
|   +-- CKE：异步操作完成事件                                        |
|   +-- MS：4 KiB片上数据切片                                        |
|   +-- LoopEngine：硬件循环执行槽                                   |
|   +-- LocalCopy/Reduce：本地搬运和归约                              |
|   +-- Channel -> Jetty/WQEBB -> PFE/URMA：跨Rank传输                |
+--------------------------------------------------------------------+
```

软件接口与硬件资源的主要对应关系如下：

| 新 API 对象 | 物理资源 | 作用 |
|---|---|---|
| `ccu::Variable` | XN | 标量、长度、偏移、token、计数器、条件值 |
| `ccu::Address` | GSA | HBM 地址及地址运算结果 |
| `ccu::Event` | CKE | 异步搬运/归约的完成状态 |
| `ccu::CcuBuffer` | MS | 片上数据暂存和多路归约输入 |
| `ccu::Loop` | Loop 描述 | 循环 body、次数和偏移配置 |
| `ccu::LoopGroup` | LoopEngine 池 | 组织、展开和调度多个硬件 Loop |
| CCU Kernel | Mission | 一条参数化微码程序的执行上下文 |

### 4.2 存储和状态资源

#### 4.2.1 Instruction：CCU 指令空间

Instruction 空间保存 Translator 生成的 CCU 微码。注册结束阶段的实际路径是：

```text
CcuKernelMgr::Translate
  -> TransRepResToPhyRes
  -> TransRepSequenceToMicrocode
       -> CcuRepTranslator::Translate
       -> Rep::Translate生成CcuInstr
  -> LoadInstruction
       -> HostToDevice memcpy
       -> CCU_U_OP_SET_INSTRUCTION
```

`CCU_RESOURCE_INS_RESERVE_SIZE = 1 MiB` 是软件为指令资源保留的映射空间，不等于所有产品的物理指令容量。实际指令条目数从能力寄存器读取。

#### 4.2.2 XN：标量寄存器

XN 是 CCU 算法最常用的标量资源，每个 XN 占 8 字节。典型内容包括：

```text
taskArgs中的长度、rankId、repeatNum
地址offset和stride
LocalAddr/RemoteAddr的安全token
循环参数、同步计数器和分支条件
Channel共享的对端地址/token
```

XN 支持赋值、加载、加法和有限条件比较，但不能把它理解为通用 CPU 的完整寄存器和指令集。

#### 4.2.3 GSA：地址寄存器

GSA 保存和计算 HBM 地址，对应 `ccu::Address`。本地或远端可访问地址实际由地址和 token 共同组成：

```text
LocalAddr  = GSA中的本端地址 + XN中的本端token
RemoteAddr = GSA中的对端地址 + XN中的对端token
```

VA 和 token 必须来自同一块已注册内存；只有 VA 而没有匹配 token，不能视为完整可访问地址。`CCU_V1_RESOURCE_GSA_RESERVE_SIZE = 32 KiB` 是软件映射预留空间，不表示一个 GSA 的大小是 32 KiB。

#### 4.2.4 MS：片上 Memory Slice

MS 对应 `ccu::CcuBuffer`，每片固定为 4096 字节。主要数据路径是：

```text
HBM -> MS
MS  -> HBM
MS  -> MS
多个MS -> LocalReduce -> MS[0]
```

涉及单个 `CcuBuffer` 的一次操作长度不能超过 4096 字节，大数据需要分片并由 LoopEngine 重复处理。多 Buffer `LocalReduce` 当前支持 2 到 8 路输入，结果写到第 0 个 MS。

#### 4.2.5 CKE：片上完成事件

CKE 对应 `ccu::Event`，用于衔接异步搬运、归约与 Mission 控制流：

```text
Mission发起LocalCopy/Write/Reduce
  -> 数据引擎异步执行
  -> 完成后置位 CKE[mask]
  -> EventWait等待相同mask
  -> Mission继续执行
```

每个 CKE 使用 16 位 mask，支持 bit0 到 bit15，最大 mask 为 `0xFFFF`。这里的 16 是单个 CKE 的事件位数，不是物理 CKE 总数；总数仍由能力寄存器确定。生产侧与等待侧 mask 不一致，或者遗漏生产事件，都可能造成硬件级死锁。

### 4.3 计算和数据处理能力

从软件可见语义看，CCU 包含以下专用能力；代码没有公开各物理单元的数量、流水级数和仲裁细节，因此不应进一步推断微架构参数。

1. **标量/地址计算**：对 XN、GSA 执行参数加载、加法、比较和地址递进。
2. **本地搬运**：`LocalCopy` 支持 HBM 到 HBM、HBM 到 MS、MS 到 HBM 等路径。
3. **跨 Rank 搬运**：`Read/Write` 经 Channel、Jetty、PFE 和 URMA 访问对端 HBM。
4. **归约**：`LocalReduce/ReadReduce/WriteReduce` 支持 SUM、MAX、MIN，以及文档列明的 UINT8、INT16、INT32、FP16、BF16、FP32 类型。
5. **控制流**：Mission 执行分支、跳转、`CCU_IF/WHILE` 和 LoopGroup 微码。

CCU 的优势来自将常见集合通信控制和数据通路固化到 IO Die；代价是资源和指令能力有限，不能执行任意通用计算。

### 4.4 队列术语：SQE、WQE、CQE、WQEBB 和 CKE

`QE` 是 Queue Entry/Element 的统称。前缀表示条目所在队列和承担的职责：

| 名称 | 全称 | 作用 |
|---|---|---|
| SQE | Submission Queue Entry | 向某个执行引擎提交任务 |
| WQE | Work Queue Element | 描述一次具体通信工作，如地址、长度和操作类型 |
| CQE | Completion Queue Entry | 报告队列任务完成或错误 |
| WQEBB | WQE Basic Block | WQE/SQE 队列存储的基本分配块 |
| CKE | CCU Kernel Event/完成事件 | Mission 内部同步状态，不是 Queue Entry |

系统中存在两个容易混淆的提交层级：

```text
第一层：Host -> CCU任务SQE -> 启动指定Mission和指令区间
第二层：Mission执行Read/Write -> Jetty/URMA WQE或SQE -> 网络数据传输
```

当前资源代码规定：

```text
一个WQEBB = 64 bytes
一个URMA SQE占4个WQEBB = 256 bytes
软件资源规格默认管理4096个WQEBB
```

Jetty 的 SQ 深度由实际申请的 WQEBB 数和配置计算，不应将某个示例中的 16 WQEBB 当作所有 SQ 的固定深度。

完整链路为：

```text
Host提交CCU SQE
  -> Mission执行微码
  -> Write/Read生成或驱动Jetty传输工作
  -> URMA/PFE执行传输
  -> 完成后设置CKE，必要时产生传输CQE
```

### 4.5 Channel、Jetty、PFE 和 URMA

#### 4.5.1 Channel

Channel 是算法可见的逻辑通信路径，绑定 peer、Die、链路、共享 XN、Notify/CKE 以及传输上下文。`GetResByChannel<Variable>` 引用的是 Channel 预配置的共享 XN 槽位，不会再申请一份普通 XN。

一个 Channel 可以配置一个或多个 Jetty。multi-jetty 算法利用多个传输队列提高并行度，但实际收益受链路和硬件仲裁影响。

#### 4.5.2 Jetty

Jetty 是 URMA 传输端点/队列上下文，可类比 RDMA QP，但两者不是完全等价的软件对象。它维护 SQ、WQEBB、producer/consumer index、doorbell 和完成状态等信息。

`CCU_PER_DIE_JETTY_RESERVED_NUM = 128`、`CCU_START_TA_JETTY_ID = 1024` 是当前软件预留或编号策略；Jetty 物理总数仍从硬件能力寄存器读取。

#### 4.5.3 PFE

从当前代码能确认，PFE 是 Jetty 的上级配置和归属资源，并参与传输路径配置。代码没有权威展开 PFE 的英文全称；可将其理解为包处理/转发配置单元，但不要仅凭名称推断更细的硬件流水线。

`CCU_V1_PER_DIE_PFE_RESERVED_NUM = 16` 是软件每 Die 的预留策略；物理 `pfeNum` 由能力寄存器给出。运行时还会按使能 Die 和 `feId` 生成实际 PFE/Jetty 配置，两者是“物理容量”和“本次配置量”的区别。

更完整的 Channel/Jetty/PFE、双 Die 和队列上下文说明见 `CCU硬件资源_Channel_Jetty_PFE.md`。

### 4.6 Loop、LoopGroup 和 LoopEngine

#### 4.6.1 三层对象关系

```text
ccu::Loop      = 循环程序描述：body、iterNum、addrOffset
ccu::LoopGroup = 一组Loop的组织、展开和并行配置
LoopEngine     = 执行Loop的物理硬件槽位
```

构造 `Loop` 时 body lambda 在 Host 注册期执行一次，用于录制 IR；单独的 Loop 不会在运行期产生硬件循环。加入 LoopGroup 后，Translator 才生成指向 body 指令区间并携带 LoopEngine ID 的 Loop 微码。

#### 4.6.2 组内绑定和组间复用

算法层不直接指定物理 LoopEngine ID，而是通过“将多个 `ccu::Loop` 按顺序放入同一个
`ccu::LoopGroup`”表达逻辑并行结构。例如：

```cpp
std::vector<ccu::Loop> grpLoops{
    *loops.loops[0],
    *loops.loops[1]
};
ccu::LoopGroup group(parallelParam, offsetParam, maxLoopNum, grpLoops);
```

`ccu::LoopGroup::AddLoops()` 按 vector 顺序调用 `CcuLoopGroupAddLoop`；HCOMM 再以组内
`loopIdx` 从 LoopEngine 池中选择 executor：

```cpp
const uint32_t loopIdx = grpDesc.loopCount;
entry.executor = loopEnginePool[loopIdx];
```

因此当前实现中，同一 LoopGroup 内按 Loop 下标绑定不同 Engine：

```text
loop[0] -> loopEnginePool[0]
loop[1] -> loopEnginePool[1]
loop[2] -> loopEnginePool[2]
```

因此一个 LoopEngine 在一次 Group 执行中承载一个 Loop 上下文，不是把多个组内 Loop 同时塞进同一个 Engine。不同 LoopGroup 会复用同一个资源池的低编号 Engine：

```text
Group A：Loop A0 -> Engine 0，Loop A1 -> Engine 1
Group A结束
Group B：Loop B0 -> Engine 0，Loop B1 -> Engine 1
```

多个 LoopEngine 具备并行执行多个 Loop 的设计能力。`LoopGroupConfig::cloneNum` 还可按 Address、MS 和 Event offset 展开并发实例。真实重叠度仍受搬运引擎、MS、CKE、Channel、Jetty和链路带宽限制，源码不足以证明精确的周期级并行度。

Translator 会把 `executor.Id()` 编入各 Loop 的参数，并为组内每个 Loop 生成一条
`LoopInstr`。所以完整绑定路径是：

```text
算法：vector<Loop>中的顺序
  -> HCOMM：loopIdx -> loopEnginePool[loopIdx]
  -> Representation：LoopEntry.executor
  -> Translator：executor.Id()进入LoopParam
  -> 微码：每个Loop生成一条LoopInstr
```

多个 Loop 放进同一个 LoopGroup 表示具备并行能力；多个独立、按顺序创建和执行的
LoopGroup 不自动构成多 Loop 并发。

#### 4.6.3 Loop GSA offset 与 LoopGroup GSA offset

GSA 是保存 HBM 地址的硬件地址资源。Loop 控制中的 GSA offset 是地址自动推进步长，
不是 Mission ID、Rank ID、taskArg 下标或 buffer 绝对基址。

当前有两个相关但不同的 offset：

1. 单个 Loop 的迭代 offset：`GetLoopParam(loopCtxId, gsaOffset, loopIterNum)`。每执行一轮
   Loop body，相关 GSA 地址按该步长推进。
2. LoopGroup clone 的 offset：`GetOffsetParam(gsaOffset, msOffset, ckeOffset)`。同一并行
   批次内，不同 clone 按该 GSA 步长访问相邻数据，同时按 `msOffset` 和 `ckeOffset`
   错开 MS、CKE 资源。

若 `S = memSlice`、`L = loopCount`，完整批次常见配置为：

```text
LoopGroup clone GSA offset = S
Loop迭代 GSA offset        = L * S
第i轮、第j个clone地址       = base + i * (L * S) + j * S
```

例如 `S=4096`、`L=4`，同一批次四个 clone 的起始地址依次相差 4096 字节；下一轮
Loop 迭代整体前进 `4 * 4096 = 16384` 字节。offset 是并行实例避免地址重叠的必要配置，
但是否存在数据竞争仍要结合每个原语的实际读写范围、Event 依赖和不同 Mission 的资源
归属判断。

#### 4.6.4 LoopEngine block

LoopEngine block 只是软件分配粒度，不是新的执行单元。当前默认 `loopNum = 8`：

```text
请求1..8个LoopEngine  -> 占用1个block -> 保留8个
请求9..16个LoopEngine -> 占用2个block -> 保留16个
```

分块可以降低零散分配和碎片管理复杂度，但可能造成内部空闲。它不能用于推断一个 Group 必然有 8 个并发 Loop。

### 4.7 Mission、Mission block 与 LoopEngine 的关系

Mission 是 CCU 顶层微码执行上下文，可近似理解为 CCU 硬件线程。CCU SQE 通过 `dieId、missionId、instStartId、instCnt` 选择执行位置和指令范围，Mission 再从 `args[13]` 将动态参数装载到 XN/GSA。

Mission 和 LoopEngine 的控制关系是：

```text
CCU SQE启动Mission
  -> Mission顺序执行主微码
  -> 遇到LoopGroup指令
       -> 配置多个Loop和LoopEngine
       -> LoopEngine迭代执行各自body并推进offset
  -> Mission继续同步、通信和退出流程
```

一个 Mission 可以使用多个 LoopEngine。LoopEngine 只卸载结构化循环，不负责 Kernel 的参数加载、总体分支和生命周期控制，也不能替代 Mission。

Mission 同样按 block 分配，当前默认 `missionNum = 2`：请求 1 个 Mission 时资源管理器会保留一个包含 2 个 Mission 的分配块。这是分配策略，不表示一个 Kernel 一定同时执行两个 Mission，也不表示一个 Mission 内含两个线程。

#### 4.7.1 多 Rank、Mission 与 LoopEngine 三层并行

这三种并行处于不同层次，不能用 ST 中的线程数量相互替代：

```text
一次集合通信
  -> 多Rank并行：多个通信参与者同时调用同一集合通信
       -> 单Rank内Mission并发：多个CCU Kernel/Mission同时运行
            -> 单Mission内LoopEngine并行：一个LoopGroup驱动多个LoopEngine
```

| 维度 | 范围 | 典型表达 | 主要校验点 |
|---|---|---|---|
| 多 Rank 并行 | 多个设备/Rank | 每个 Rank 一个通信域和算子调用；ST 可用每 Rank 一个 Host 线程模拟 | 跨 Rank 地址/token 交换、Notify、数据匹配 |
| Mission 并发 | 同一 Rank/Die | 多 Stream/Thread 下发多个 Kernel/Mission | Mission、GSA、MS、CKE、Channel 等资源隔离和调度 |
| LoopEngine 并行 | 单个 Mission 内 | 多个 Loop 放进同一个 LoopGroup | Engine 绑定、clone offset、Event 依赖和读写范围 |

例如 4 Rank 集合通信可以同时具备“每 Rank 2 个 Mission、每 Mission 2 个 LoopEngine”，
形成 `4 Rank * 2 Mission * 2 LoopEngine` 的三维并行。只创建 4 个 Rank 线程只能证明多
Rank 参与，不能证明单 Rank Mission 并发；一个 Mission 内使用多个 LoopEngine 也不能
反推存在多个 Mission。

### 4.8 每 Die 资源总量和软件分配量

物理总量由驱动返回的能力寄存器按 Die 解析：

```text
cap0[15:0]  + 1 -> instructionNum
cap0[23:16] + 1 -> missionNum
cap0[31:24] + 1 -> loopEngineNum

cap1[15:0]  + 1 -> gsaNum
cap1[31:16] + 1 -> xnNum

cap2[15:0]  + 1 -> ckeNum
cap2[31:16] + 1 -> msNum

cap3[15:0]  + 1 -> channelNum
cap3[31:16] + 1 -> jettyNum

cap4[7:0]   + 1 -> pfeNum
```

所以脱离具体芯片、驱动和 Run 包，不能给出固定的 XN/MS/Mission 总数。以下数字必须明确标为软件策略而非硬件上限：

| 项目 | 当前值 | 性质 |
|---|---:|---|
| XN 单项大小 | 8 bytes | 资源项大小 |
| MS 单片大小 | 4096 bytes | 资源项大小 |
| CKE mask | 16 bits | 单 CKE 事件位宽 |
| WQEBB | 64 bytes | 队列基本块大小 |
| URMA SQE | 4 WQEBB | 队列格式约束 |
| LoopEngine block | 8 | 软件分配粒度 |
| CKE block | 8 | 软件分配粒度 |
| MS block | 64 | 软件分配粒度 |
| Mission block | 2 | 软件分配粒度 |
| 每 Die PFE reserved | 16 | 软件预留策略 |
| Jetty reserved/default | 128 | 软件预留/配置策略 |

### 4.9 以 AllToAllV 为例的硬件执行流程

```text
注册阶段
1. Host执行CcuAlltoAllVMesh1DKernel，记录ccu:: API为Representation
2. 统计XN/GSA/CKE/MS/LoopEngine/Mission需求
3. 虚拟handle绑定物理资源ID
4. Translator生成微码并加载到CCU指令空间

运行阶段
5. Host取得input/output VA和token，构造taskArgs/CcuTaskParam
6. Runtime提交CCU SQE，启动指定Die上的Mission
7. LoadArg把VA、token、length、offset加载到GSA/XN
8. Rank间通过Channel共享XN交换对端output地址和token，并用Notify/CKE同步
9. Mission计算LocalAddr/RemoteAddr，发起LocalCopy或远端Write
10. Jetty/WQEBB/PFE/URMA完成传输，完成路径设置CKE
11. LoopEngine按分片重复搬运，Mission通过EventWait组织依赖
12. 全部分片完成后PostSync，Mission退出，Runtime获得完成状态
```

### 4.10 可确认范围与未知微架构

当前仓库能够确认资源抽象、能力寄存器布局、微码生成、资源分配策略和 API 语义，但没有公开以下参数：

```text
CCU时钟和峰值带宽
标量/地址ALU物理数量
MS bank组织和端口数
LoopEngine周期级调度与仲裁
各流水级延迟和并发退化条件
```

这些信息不能从预留常量或 ST 仿真结果反推。asc-devkit Host ST 可以验证逻辑 IR、TaskArg 桥接和运行语义模拟，但不能代替真实物理资源绑定、Translator 二进制、驱动和硬件时序验证。

## 5. CCU 原语分类

| 类别 | 主要 API | 语义 |
|---|---|---|
| 资源 | `Variable/Address/Event/CcuBuffer/Array` | 申请虚拟 CCU 资源 |
| Channel 共享 | `GetResByChannel<Variable>` | 获取 Channel 共享 XN 槽位 |
| 动态参数 | `LoadArg` | Launch 参数加载到 Variable |
| 存取 | `Load/Store` | HBM/变量间加载和回写 |
| 本地数据 | `LocalCopy/LocalReduce` | 本地 Memory/Buffer 搬运或规约 |
| 远端数据 | `Write/Read/WriteReduce/ReadReduce` | 经 Channel 的远端读写/规约 |
| 事件 | `EventRecord/EventWait` | Kernel 内异步操作完成关系 |
| Rank 同步 | `NotifyRecord/NotifyWait` | 通过 Channel 的跨 Rank 通知 |
| 值+同步 | `WriteVariableWithNotify` | 写对端共享 Variable 并记录 Notify |
| 控制流 | `CCU_IF/WHILE/DO/Func/Loop/LoopGroup` | 生成条件、跳转和硬件循环 |

例如：

```cpp
ccu::Write(channel, remoteDst, localSrc, len, event, mask);
```

会通过 C++ wrapper 调用：

```text
ccu::Write
  -> CcuWriteMemToMem
  -> currentKernel->WriteMemToMem
  -> Append(CcuRepWrite)
```

这里的 `Append` 是注册期的 IR 记录，不是当场执行远端写。

## 6. Representation 究竟是什么

### 6.1 它是多态对象 IR，不是一个扁平 struct

Representation 公共基类简化后为：

```cpp
class CcuRepBase {
public:
    virtual bool Translate(
        CcuInstr *&instr,
        uint16_t &instrId,
        const TransDep &dep) = 0;

    virtual std::string Describe() = 0;
    virtual uint16_t InstrCount();

protected:
    CcuRepType type;
    bool translated;
    uint16_t instrId;
    uint16_t instrCount;
};
```

顶层及控制流 Block 保存：

```cpp
std::vector<std::shared_ptr<CcuRepBase>> repVec;
```

所以 Representation 是一棵带嵌套 Block 的 AST/IR：

```text
MainBlock
  LoadArg(input, 0)
  LoadArg(output, 1)
  LoadArg(token, 2)
  RemPostVar(output)
  RemWaitSem(...)
  If(sliceSize != 0)
    Write(...)
    LoopGroup
      LocalCopy(...)
    EventWait(...)
  RemPostSem(...)
```

### 6.2 Rep 类型

```text
LOAD_ARG / LOAD_VAR / STORE_VAR
ASSIGN / ADD
JUMP / JUMP_EQ / JUMP_NE / JUMP_LABEL
FUNC_CALL / FUNC_BLOCK
LOOP / LOOP_CALL / LOOPGROUP / LOOP_BLOCK
LOC_RECORD_EVENT / LOC_WAIT_EVENT
REM_POST_SEM / REM_WAIT_SEM / REM_POST_VAR
READ / WRITE / LOCAL_CPY / LOCAL_REDUCE
BUF_READ / BUF_WRITE / BUF_REDUCE
```

### 6.3 Write Rep 的形态

```cpp
class CcuRepWrite : public CcuRepBase {
    ChannelHandle channel;
    uint32_t channelId;
    RemoteAddr rem;
    LocalAddr loc;
    Variable len;
    CompletedEvent event;
    uint16_t mask;
    uint16_t dataType;
    uint16_t opType;
    uint16_t reduceFlag;
};
```

注册期这里保存的是资源引用/ID、Channel 和 mask，而不是已解析的用户数据。Translate 时它才会生成类似：

```cpp
TransLocMemToRmtMemInstr(
    rem.addr.Id(), rem.token.Id(),
    loc.addr.Id(), loc.token.Id(),
    len.Id(), channelId,
    event.Id(), mask, ...);
```

### 6.4 虚拟资源与物理资源

`ccu::Variable` 创建的 handle 是算法前端标识。内部 `CcuKernel` 保存从 handle 到 IR 资源对象的映射：

```text
CcuVariableHandle  -> CcuRep::Variable
CcuAddressHandle   -> CcuRep::Address
CcuEventHandle     -> CcuRep::CompletedEvent
CcuBufferHandle    -> CcuRep::CcuBuf
CcuLocalAddrHandle -> CcuRep::LocalAddr
CcuRemoteAddrHandle-> CcuRep::RemoteAddr
```

Register 后续会将这些虚拟资源绑定到真实 XN/GSA/CKE/MS/LoopEngine 编号，Translator 使用物理 ID 生成微码。

## 7. kernelArgs、taskArgs 和 LoadArg

### 7.1 kernelArgs：注册期静态信息

```cpp
struct CcuKernelArgBase {
    ChannelHandle channels[CCU_MAX_RANK_SIZE];
    uint32_t channelCount;
};

struct CcuKernelArgAllGatherMesh1DMem2Mem : CcuKernelArgBase {
    uint64_t rankSize;
    uint32_t rankId;
    OpParam opParam;
    std::vector<std::vector<uint32_t>> subCommRanks;
};
```

`kernelArgs` 在 Register 时传入，它决定：

- Kernel 面向哪个 Rank/子通信域。
- 有哪些 Channel 和对端 Rank。
- 需要构建哪些前后同步、Write/Read 和 Loop。
- 需要多少虚拟/物理 CCU 资源。

它更像编译期模板参数。

### 7.2 taskArgs：Launch 期动态数值

`taskArgs` 没有强类型结构，就是：

```cpp
std::vector<uint64_t> taskArgs;
```

当前 HCCL AllGather 完整实现为15项：

| argId | 含义 |
|---:|---|
| 0 | inputAddr |
| 1 | outputAddr |
| 2 | token |
| 3 | currentRankSliceInputOffset |
| 4 | currentRankSliceOutputOffset |
| 5 | tmpRepeatNum |
| 6 | inputRepeatStride |
| 7 | outputRepeatStride |
| 8 | normalSliceSize |
| 9 | lastSliceSize |
| 10 | isInputOutputEqual |
| 11 | goSize.addrOffset |
| 12 | goSize.loopParam |
| 13 | goSize.parallelParam |
| 14 | goSize.residual |

```cpp
HcommCcuKernelLaunch(
    thread,
    kernelHandle,
    taskArgs.data(),
    taskArgs.size());
```

`uint64_t[]` 本身不记录类型和名称，Host 构造顺序与 Kernel `LoadArg` 顺序就是 ABI。

### 7.3 LoadArg Rep

Kernel 内：

```cpp
uint32_t argId = 0;
ccu::LoadArg(ctx.input, argId++);
ccu::LoadArg(ctx.output[rankId], argId++);
ccu::LoadArg(ctx.token[rankId], argId++);
// ...
ccu::LoadArg(ctx.goSize.residual, argId++);
```

`ccu::LoadArg` 最终生成：

```cpp
class CcuRepLoadArg : public CcuRepBase {
    Variable var;       // 加载目标 XN
    uint16_t argId;     // 当前 SQE 中的参数槽位
    uint16_t fullArgId; // 完整 taskArgs 下标
};
```

其语义是：

```text
Launch 时把 taskArgs[fullArgId]
加载到 Rep 指定的 Variable/XN。
```

Translator 会按 `fullArgId` 对 LoadArg Rep 排序，并生成：

```cpp
LoadSqeArgsToXnInstr(instr, var.Id(), argId);
```

### 7.4 13 个 SQE 参数槽位

CCU Runtime 任务参数结构为：

```cpp
constexpr uint32_t CCU_SQE_ARGS_LEN = 13;

struct CcuTaskParam {
    uint8_t dieId;
    uint8_t missionId;
    uint16_t timeout;
    uint32_t instStartId;
    uint32_t instCnt;
    uint32_t key;
    uint32_t argSize;
    uint64_t args[13];
};
```

`LoadArg` 内部保存：

```cpp
slotId = fullArgId % 13;
```

AllGather 有15个参数，因此 Launch 会生成2个 `CcuTaskParam`：

```text
CcuTaskParam[0]
  args[0..12] = taskArgs[0..12]
  执行前13条 LoadArg 指令

CcuTaskParam[1]
  args[0..1] = taskArgs[13..14]
  执行剩余 LoadArg 和 Kernel 主体指令
```

Launch 会检查：

- `taskArgs` 数量与 Kernel 中不同 `argId` 数量相等。
- `0..argsNum-1` 每个下标都被 LoadArg，不允许空洞。
- Kernel 已完成 Translate，不能在未生成指令时 Launch。

### 7.5 四种对象的定位

| 对象 | 生命周期 | 定位 |
|---|---|---|
| `kernelArg` | Register | 编译期拓扑/Channel/结构参数 |
| Representation | Register 到注销 | 参数化 CCU 程序 IR |
| `taskArgs[]` | 每次 Launch | 当次地址、token、offset、size 等实参 |
| `CcuTaskParam` | Launch 下发 | 按 CCU SQE ABI 分组后的 Runtime 参数包 |

## 8. 微码编译和加载的精确过程

### 8.1 RegisterStart

```cpp
HcommCcuKernelRegisterStart(insHandle);
```

主要工作：

- 进入注册状态。
- Reset 当前 Instance 的本批注册上下文。
- 限定后续 Kernel 注册的批次边界。

### 8.2 Register：前端建图+资源申请

```cpp
HcommCcuKernelRegister(
    insHandle,
    dieId,
    kernelFuncName,
    kernelFunc,
    kernelArgs,
    argNum,
    &kernelHandle);
```

内部流程：

```text
创建 current CcuKernel
  -> 调用 kernelFunc(kernelArg)
  -> ccu:: API 向 current Kernel Append Rep
  -> 闭合 IF/WHILE/Loop 结构
  -> 根据 Channel 选择 die
  -> 统计 XN/GSA/CKE/MS/LoopEngine/指令数
  -> 申请物理资源和指令空间
  -> 生成 CcuKernelHandle
```

这一阶段的 Kernel 函数是 Host CPU 上的“程序生成器”，而不是数据通信的实时执行。

### 8.3 RegisterEnd：Rep -> 微码 -> 设备

```cpp
HcommCcuKernelRegisterEnd(insHandle);
```

它取得本批未翻译 Kernel，然后：

```text
CcuKernelMgr::Translate
  -> TransRepResToPhyRes
       虚拟资源绑定物理 XN/GSA/CKE/MS/Mission
  -> TransRepSequenceToMicrocode
       -> CcuRepTranslator::Translate
       -> 每个 Rep::Translate 生成 CcuInstr
       -> 生成 CcuInstrInfo
  -> LoadInstruction
       -> HostToDevice memcpy
       -> CCU_U_OP_SET_INSTRUCTION
       -> 微码写入 CCU 指令空间
```

微码结果保存为：

```cpp
struct CcuInstrInfo {
    std::vector<CcuInstr> instrVec;
    uint16_t startInstrId;
    uint16_t instrCount;
    uint16_t missionStartInstrId;
    uint16_t missionInstrCount;
};
```

因此严格区分：

```text
Register    = 生成 Representation
RegisterEnd = Representation 编译为微码并加载
Launch      = 绑定参数并执行已加载微码
```

### 8.4 Launch：参数绑定和 Runtime 下发

```text
HcommCcuKernelLaunch
  -> 通过 kernelHandle 找到 CcuKernel
  -> ValidateTaskArgs
  -> GeneTaskParams
  -> 按13个参数槽位分成一个或多个 CcuTaskParam
  -> rtCCULaunch
```

Launch 不再调用算法 Kernel 函数，也不再重建 Rep 或重新翻译微码。同一 `CcuKernelHandle` 可以使用不同 `taskArgs` 重复 Launch。

## 9. AllGather Mesh1D Mem2Mem 算法原理

### 9.1 集合通信目标

N 个 Rank，每个 Rank 有数据 `D_i`：

```text
Rank i input = D_i
```

AllGather 完成后每个 Rank：

```text
output = [D_0, D_1, ..., D_(N-1)]
```

Mesh1D/FullMesh 下每个 Rank 都有到其他 Rank 的直达 Channel，可以一轮完成数据交换。

### 9.2 注册期资源

Kernel 按 Rank 顺序遍历 peer：

```cpp
for (peerId = 0; peerId < rankSize; ++peerId) {
    if (peerId != rankId) {
        output[peerId] = ccu::GetResByChannel<ccu::Variable>(
            channels[channelIdx], OUTPUT_XN_ID);
        token[peerId] = ccu::GetResByChannel<ccu::Variable>(
            channels[channelIdx], TOKEN_XN_ID);
        ++channelIdx;
    }
}
```

本 Rank 的 input/output/token 来自 Launch `taskArgs`；对端 output/token 通过 Channel 共享 Variable 交换。

### 9.3 前同步

```text
每个 Rank
  -> WriteVariableWithNotify(将自己 output 基址写给每个 peer)
  -> WriteVariableWithNotify(将自己 token 写给每个 peer)
  -> NotifyWait(等待收到所有 peer 的 output/token)
```

前同步完成后，每个 Rank 都知道其他 Rank 的远端 output 地址和 token，才可安全构造 `RemoteAddr`。

### 9.4 核心数据流

以 Rank `r` 为例：

```text
远端部分：
  对每个 peer != r
    ccu::Write(
      peerChannel,
      peer.output + r * sliceStride,
      local.input,
      sliceSize,
      event,
      rankMask)

本地部分：
  GroupCopy(
    local.output + r * sliceStride,
    local.input,
    sliceSize)
```

也就是：

- 本 Rank 把自己的 `D_r` 写入每个 peer 的 output 第 `r` 块。
- 本 Rank 不通过网络写自己，而是用 GroupCopy 将 `D_r` 放入本地 output 第 `r` 块。
- 所有 Rank 并行完成后，每个 output 都聚齐 N 个 Rank 的分片。

### 9.5 mem2mem 的含义

```text
ccu::Write(channel, RemoteAddr, LocalAddr, ...)
  -> CcuWriteMemToMem
```

mem2mem 指远端 Write 的源是用户内存/LocalAddr，直接搬向远端 Memory。

对比经 CcuBuffer 的路径：

```text
非 mem2mem：Memory -> CcuBuffer -> Remote Memory
mem2mem：   Memory -------------> Remote Memory
```

本地 GroupCopy 仍可以使用 CcuBuffer 分块流水，这与远端 Write 是否 mem2mem 不矛盾。

`mem2mem` 和 `goSize` 是正交维度：

```text
mem2mem：描述数据通路两端，例如LocalAddr -> RemoteAddr
goSize：描述任意字节数如何切成Loop迭代、并行slice和尾块
```

因此 mem2mem 算法可以使用 `goSize`。但不是每一条 mem2mem 原语都必须使用它：如果
算法直接发起一次 `ccu::Write(channel, dst, src, sliceSize, event)`，且不借助公共
LoopGroup 做分片和尾块处理，这条 Write 本身不需要 `goSize`；需要把任意长度映射到
GroupCopy、GroupReduce 等 LoopGroup 执行结构时才需要。

### 9.6 Event mask

远端 Write 是异步操作，每个 Rank 使用不同 bit 表示完成：

```text
rank 0 -> mask bit 0
rank 1 -> mask bit 1
...
```

本 Rank 不对自己发 Write，因此在本地分支直接 `EventRecord(selfMask)`。最后 `EventWait(totalMask)` 或分组 EventWait 等待所有 bit，保证远端 Write 和本地分支已完成。

当前 HCCL 完整实现支持 Rank 数跨越单个16 bit CKE 的情况，按 `rankIdx / 16` 选择 Event，按 `rankIdx % 16` 选择 mask bit。

### 9.7 GroupCopy 和 CalGoSize

`GroupCopy`、`GroupReduce` 是 HCCL CCU Kernel 层的软件组合函数。它们在注册阶段调用
基础 `ccu::` 原语来录制 Representation，运行阶段由 CCU Mission 执行翻译后的微码；
不能理解成 Launch 时由 Host CPU 自己循环搬运数据。这里的“基础原语”也不是并发编程
语境中的 atomic 原子操作。

当前 GroupCopy 用于将本 Rank input 放入本地 output 正确分片，其 Loop body 通常是：

```text
LocalAddr src
  -> LocalCopy(Memory -> CcuBuffer)
  -> EventWait
  -> LocalCopy(CcuBuffer -> Memory)
  -> EventWait
```

所以当前实现可概括为：

```text
GroupCopy
  = LocalCopy基础数据操作
  + HBM -> MS -> HBM中转
  + Event同步
  + LoopGroup并行和迭代
  + 大块数据切分和尾块处理
```

GroupReduce 同样是组合行为，典型 body 包含 HBM/远端 HBM 到 MS、`LocalReduce`、结果
写回 HBM 和相应 EventWait。算法名称中的 mem2mem 主要描述跨 Rank 主数据路径，并不
保证算法内部的本地复制或归约完全不使用 MS。

`CalGoSize(size, config)` 把任意字节数分解成硬件 LoopGroup 能表达的四个参数。设：

```text
S = config.memSlice
L = config.loopCount
loopSize = L * S

size = m * L * S + n * S + p
0 <= n < L
0 <= p < S
```

上式是一般情况。若 `size` 正好等于 Loop 次数编码可覆盖的最大总量，代码会为避免
`loopIterNum` 溢出，将最后一个完整并行批次改写为 `n=L-1、p=S`；此时总字节数不变，
但 `p` 允许等于 `S`。

代码中的计算是：

```cpp
m = size / loopSize;
n = (size - m * loopSize) / memSlice;
p = size - m * loopSize - n * memSlice;
```

四个结果的含义是：

| `GroupOpSizeVars` 字段 | 来源 | 含义 |
|---|---|---|
| `addrOffset` | `m * L * S` | 完整批次处理完后，尾段相对原始 base 的起点 |
| `loopParam` | `m` | 完整批次的串行 Loop 迭代次数 |
| `parallelParam` | 根据 `n/p` 编码 | 尾段展开几个 Loop、从哪个 Loop 开始展开、共有几个 Loop |
| `residual` | `p` 或 `S` | 尾 Loop 当次实际处理字节数 |

`goSize.addrOffset` 和 GSA offset 的职责不同：

```text
goSize.addrOffset：完整批次结束后，显式执行src.addr/dst.addr += offset，切换到尾段
Loop GSA offset：完整批次内部，每次Loop迭代自动前进L * S
LoopGroup GSA offset：同一批次内部，每个并行clone自动前进S
```

三者来自同一份切片配置，数学关系是：

```text
goSize.addrOffset
  = goSize.loopParam * Loop GSA offset
  = m * L * S
```

例如 `S=4096`、`L=4`、`size=43000` 字节：

```text
m = 2，n = 2，p = 2040
完整批次字节数  = 2 * 4 * 4096 = 32768
剩余完整slice   = 2 * 4096 = 8192
最后尾块         = 2040

LoopGroup GSA offset = 4096
Loop GSA offset      = 16384
goSize.addrOffset    = 32768
```

CCU 先用 clone offset 和 Loop offset 自动处理前 32768 字节，再将 src/dst 显式移动到
`base + goSize.addrOffset`，通过 `parallelParam/residual` 处理后续 `8192 + 2040` 字节。
这种拆分避免 Host 为每个 4 KiB slice 单独 Launch。

AllGather mem2mem 是实际使用示例：Host 按 `normalSliceSize` 计算 goSize，并放入 taskArgs；
Kernel 用 `LoadArg` 装入四个 `GroupOpSizeVars`，远端数据直接通过 mem2mem Write 发送，本
Rank 的本地落位则调用 `GroupCopy(..., goSize)`。ReduceScatter mem2mem 同样会针对 normal
slice 或 last slice 计算 goSize，用于本地归约 LoopGroup 的完整区和尾区处理。

### 9.8 完整 HCCL 实现的三阶段展开

当前 HCCL 不只是单次 Write，还支持 repeat/stride 和 `AG_UNROLL_NUM=16` 展开：

```text
Phase 1：批量生成多个 repeat 的远端 Write，Event 错开
Phase 2：用 CCU_WHILE 完成各 repeat 的本地 GroupCopy
Phase 3：批量 EventWait，等待各 repeat/各 Rank 完成
```

如果：

```cpp
inputAddr + currentRankSliceInputOffset
    == outputAddr + currentRankSliceOutputOffset
```

则 `isInputOutputEqual=1`，自己的数据已在正确 output 位置，可跳过本地 GroupCopy。

### 9.9 后同步

本 Rank 的 Write 完成只说明本端发起的操作完成，还需与 peer 确认整个 AllGather 阶段结束：

```text
for each channel: NotifyRecord(POST_SYNC_ID)
for each channel: NotifyWait(POST_SYNC_ID)
```

这保证 Kernel 返回前，所有 Rank 都已完成对应轮次。

## 10. PR24 独立样例与 HCCL 完整实现的边界

| 项目 | `asc-comm@pr24` 独立样例 | 当前 HCCL 完整实现 |
|---|---|---|
| 定位 | API/自定义算子样例 | 产品算法 template/kernel |
| `taskArgs` | 10项 | 15项 |
| repeat/stride | 外层简化切片 | Kernel 内 repeat/stride |
| in-place | 样例当前无 `isInputOutputEqual` | 显式跳过 GroupCopy |
| Event | 单 Event/mask 示例 | 支持16 bit 分组和16路展开 |
| 架构 | 自定义 op host + exec_op | selector/executor/template/kernel |

迁移时应：

- 用 HCCL 完整实现保留算法语义、边界和优化。
- 用 PR24 作为 `ccu::` API、Register/Launch 和数据面实现基线。
- 不应为了贴合简化样例而丢失 HCCL 的 repeat、tail、in-place 和大 Rank Event 处理。

## 11. Notify 与 Event 的区别

| 对象 | 主要范围 | 用途 |
|---|---|---|
| `EventRecord/EventWait` | 同一 Kernel 内的异步数据操作 | 等 Write/Read/LocalCopy/Reduce 完成 |
| `NotifyRecord/NotifyWait` | 经 Channel 的跨 Rank 同步 | 前后同步、peer 阶段完成 |
| `WriteVariableWithNotify` | 跨 Rank 值交换+同步 | 传播远端地址/token 等动态值 |

Event mask 是 bitset，不是一个普通的单值 Notify ID。ST 成图时必须按 `(event, bit)` 跟踪生产和消费关系，否则会把多个并行完成位错误串行化。

## 12. 与 AICPU 算法执行的本质差异

| 维度 | AICPU | CCU |
|---|---|---|
| 编程模式 | 运行时命令式调用 HCOMM | 注册期声明式生成微码 |
| 地址 | 调用原语时已是具体地址 | LoadArg 在 Launch 时绑定 |
| 并行 | 多 Thread/Stream+从流 | Channel/Jetty/Event/LoopGroup 硬件并行 |
| 同步 | Stream Notify/Post/Wait | CKE Event mask + Channel Notify |
| 执行 | AICPU/KFC Server 编排 | CCU 微码自主执行 |
| ST 捕获 | HCOMM 原语调用时直接生成 Task | Register 捕获 IR，Launch 绑定参数后实例化子图 |

CCU Kernel 通常不需要 AICPU 意义上的 N-1 从流。它的并行性在 Kernel 内的多 Channel、Event mask、Jetty 和 LoopGroup 中，不能简单映射为 AICPU 从流队列。

## 13. 对 CCU ST 的直接要求

### 13.1 两阶段捕获

```text
Register 阶段：
  执行函数式 Kernel
  -> 捕获资源、LoadArg、Write/Read、Event/Notify、Loop
  -> 生成参数化 CCU IR

Launch 阶段：
  提供 taskArgs
  -> 绑定 LoadArg
  -> 解析具体地址/token/offset/size
  -> 实例化跨 Rank Task 子图
  -> DAG/SimWorld/BufferSemantic/Checker
```

### 13.2 建议的 ST 边界

ST 应在新 API 底层 `Ccu*` C 接口建模：

```text
CcuVariableAlloc/CcuAddressAlloc/CcuEventAlloc
CcuLoadArg
CcuWrite*/CcuRead*
CcuLocalCopy*/CcuLocalReduce*
CcuNotify*/CcuEvent*
CcuLoop*/CcuFunc*
HcommCcuKernelRegisterStart/Register/End
HcommCcuKernelLaunch
HcommCcuGetMemToken
```

不应只让这些接口返回 `CCU_SUCCESS`，否则 Kernel 表面执行成功，Checker 却看不到任何数据流，形成假通过。

### 13.3 AllGather 最小 ST 闭环

```text
selector 选中 COMM_ENGINE_CCU
  -> 函数式 AllGather Kernel 被 Register
  -> LoadArg ABI 与 taskArgs 一致
  -> 前同步地址/token 交换正确
  -> 每个 Rank 生成 N-1 个远端 Write 语义
  -> 本地 GroupCopy/in-place 分支正确
  -> Event/Notify 同步闭合且无死锁
  -> 远端写的 peer 侧访存被双边建模
  -> 每个 Rank output 的来源集合为 [D0...Dn-1]
```

### 13.4 离线 ST 不能单独证明的能力

- Representation 到真实硬件微码编码完全正确。
- 真实 XN/GSA/CKE/MS/Jetty/PFE 资源分配和耗尽行为。
- Driver 指令装载、token 安全机制和物理链路。
- 真实数值、带宽、时延、Profiling 和可靠性。

这些需要产品集成 ST 和上板测试补齐。

### 13.5 为什么 CCU Checker 必须跨阶段分层

AICPU ST 通常可以在一次运行中截获已经带有具体地址和长度的 HCOMM 原语；CCU 不同，同一个算法语义被拆散在多个时期：

```text
RegisterStart/Register
  静态 kernelArgs + ccu:: API
  -> 参数化 Representation

RegisterEnd
  Representation + 虚拟资源
  -> 物理资源绑定 + CcuInstrInfo + 已加载微码

Launch
  kernelHandle + taskArgs
  -> CcuTaskParam.args[13]
  -> LoadArg 对应的 XN 获得本次动态值

CCU 执行
  指令 + XN/GSA/CKE/Channel
  -> 实际同步和数据搬运
```

只检查其中一个时期会留下明显盲区：

- 只检查 Register：能够看到 Write 和 Event 结构，但不知道 Launch 是否把正确地址和长度放入对应 XN。
- 只检查 Launch：能够看到 15 个数值，但不知道这些值被哪个 LoadArg 和哪条指令使用。
- 只检查最终 output：参数错位、同步缺失或原语多执行一次，有时仍可能碰巧得到正确数据。
- 只检查微码：无法判断 Host template 构造的 offset、stride、repeat 和 in-place 参数是否正确。

因此 CCU ST 的可信闭环至少包含三个关联关系：

```text
编译态：ccu:: API -> Representation/IR
桥接态：LoadArg(argId) <-> taskArgs[argId] <-> TaskParam[mission][slot]
运行态：已绑定 IR -> 动态原语行为 -> 最终内存语义
```

产品集成测试再补充第四个关系：

```text
Representation/IR -> 真实 CcuInstr -> 驱动装载 -> 硬件执行
```

### 13.6 编译态 Checker 的原理

编译态 Checker 的输入不是具体数据，而是“参数化程序”。它需要区分三个层级。

#### 13.6.1 注册生命周期

`RegisterStart/Register/RegisterEnd` 定义一个 resGroup 的编译事务。同一个 resGroup 中的 Kernel 需要共同处理资源和上下文，因此注册结果应当整体成功或整体失败，不能出现前一个 Kernel 已可 Launch、后一个 Kernel 翻译失败的部分提交状态。

生命周期 Checker 主要保证：

- Kernel 只在合法注册窗口内创建。
- kernelHandle 唯一并归属于正确 Instance。
- 异常不会遗留当前 CompilerContext。
- `RegisterEnd` 失败时整个 currentGroup 都不可作为已编译产物使用。

这一层回答的是“编译事务是否成立”，不回答算法是否正确。

#### 13.6.2 Representation/IR 结构

函数式 Kernel 中的对象赋值、地址运算和原语调用都被记录为 IR。例如：

```text
ccu::LoadArg(input, 0)       -> LoadArg(inputXN, fullArgId=0)
src.addr = input             -> AddressAssign(srcGSA, inputXN)
src.addr += offset           -> AddressAdd(srcGSA, offsetXN)
ccu::Write(...)              -> Write(channel, remote, local, len, event, mask)
CCU_IF(size != 0)            -> IfBegin(condition) ... IfEnd
```

结构 Checker 要验证资源先定义后使用、控制流成对、Channel 方向、Event 完成关系以及算法阶段顺序。对 AllGather 而言，还要验证 PreSync、16 路 Write 展开、按 16 rank 分组的 Event mask、本 rank EventRecord 和 PostSync。

这层可以发现“算法生成了错误程序”，但不能证明该程序已经被真实 Translator 正确编码。

#### 13.6.3 逻辑资源与物理资源

逻辑资源 Checker 看到的是 Variable、Address、Event、Channel 和 token 之间的关系。例如每个 AllGather peer 必须有 output XN 和 token XN，RemoteAddr 必须绑定该 peer 交换得到的地址和 token。

物理资源 Checker 则需要检查：

```text
虚拟 Variable -> 物理 XN
虚拟 Address  -> 物理 GSA
Event/Notify  -> 物理 CKE
LoopGroup     -> MS/LoopEngine
Kernel        -> Mission/指令空间
```

后者必须进入 `TransRepResToPhyRes` 和真实资源管理层，Host 自建 IR 只能验证逻辑关系，不能模拟结果后宣称物理资源分配正确。

### 13.7 LoadArg、taskArgs 和 TaskParam 的桥接原理

`LoadArg` 是 CCU 编译态与运行态的数据接口。Register 时，它只记录“第 argId 个启动参数应加载到哪个 Variable”；Launch 时，Host 才提供具体 `uint64_t taskArgs[]`。

对于 AllGather 的 15 个参数：

```text
arg0..arg12 -> CcuTaskParam[0].args[0..12]
arg13..arg14 -> CcuTaskParam[1].args[0..1]
```

一般映射公式为：

```text
missionIndex = argId / CCU_SQE_ARGS_LEN
slotIndex    = argId % CCU_SQE_ARGS_LEN
CCU_SQE_ARGS_LEN = 13
```

因此桥接 Checker 必须同时读取两侧信息：

| 编译态信息 | 运行态信息 | 必须成立的关系 |
|---|---|---|
| LoadArg 数量 | argNum | 完全相等 |
| fullArgId | taskArgs 下标 | 从 0 连续、无空洞和重复 |
| Variable handle | TaskParam mission/slot | LoadArg 指令加载到同一个 XN |
| Kernel 参数用途 | PrepareLaunchArgs 顺序 | 地址、offset、size、loop 参数不能错位 |
| 指令 Mission 划分 | 多个 CcuTaskParam | startInstrId、instCnt 与参数段匹配 |

最终内存正确也不能替代这一检查。例如 input/output 地址恰好相同的 in-place 用例，交换 arg0 和 arg1 可能不影响结果；只有 ABI Checker 能稳定发现这种问题。

### 13.8 运行态行为 Checker 的原理

运行态行为 Checker 将已绑定的 IR 作为多 rank 状态机执行。每个 rank 保存：

```text
program counter
Variable/XN 当前值
Address/GSA 当前值
Event mask
按 peer 和 index 区分的 Notify mask
本地 input/output 内存
```

调度器反复选择可前进的 rank：

- `NotifyWait` 或 `EventWait` 条件不满足时，该 rank 暂停但不消费完成位。
- `WriteVariableWithNotify` 将地址/token 写入 peer 的 Channel Variable，并记录 Notify bit。
- `Write` 将本 rank LocalAddr 数据写入 Channel 指向 rank 的 RemoteAddr，并记录本地完成 Event。
- `EventWait` 在 mask 全部到达后消费对应 bit。
- 一轮调度没有任何 rank 前进且仍有未完成 rank 时，判定为死锁。

除“是否完成”外，还应记录每种原语的动态执行次数。静态 IR 中只有一条位于 `CCU_WHILE` 内的 LocalCopy，但运行时可能执行 repeatCount 次；因此静态操作计数与动态执行计数不能混为一谈。

AllGather 每 rank 的典型动态计数为：

```text
Write                   = repeatCount * (rankSize - 1)
EventRecord             = repeatCount
LocalCopy               = in-place ? 0 : repeatCount
数据 EventWait          = repeatCount * ceil(rankSize / 16)
本地拷贝完成 EventWait  = in-place ? 0 : repeatCount
```

行为 Checker 能发现最终数据检查不敏感的错误，例如多发一次 Write、等待了错误 mask、PostSync 提前或某 rank 没有真正结束。

### 13.9 Data Result Checker 为什么必须独立构造期望值

结果 Checker 应当在 Launch 前对输入和 output 初值做快照，然后按照集合通信数学定义独立生成 expected：

```text
expected[dstRank][repeat][sourceRankSlice]
    = launchBeforeSnapshot[sourceRank][repeat][sourceRankSlice]
```

不能调用被测算法的地址推进函数生成 expected，也不能在 Launch 后读取 in-place 输入作为来源，否则同一个 offset 或 stride 错误可能同时出现在算法和 Checker 中，形成自证。

AllGather 结果 Checker 至少检查：

- 每个 destination rank 是否包含所有 source rank 的切片。
- rank offset、repeat stride、tailSize 是否正确。
- in-place 场景是否基于运行前快照计算。
- tail 后 padding、未参与区间和 guard bytes 是否保持原值。
- 零长度 tail rank 是否只参与同步而不搬运数据。

正向用例只能证明 Checker 接受当前结果。还必须有负向测试，例如篡改一个 output 字节、交换 taskArg 或修改 Event mask，并确认对应层 Checker 必然失败。

### 13.10 Checker 覆盖边界

完整 CCU 验证应区分以下结论，避免把 Host 仿真能力扩大解释：

| Checker | 可以证明 | 不能证明 |
|---|---|---|
| Register | 注册事务和 handle 生命周期正确 | 算法数据流正确 |
| Representation/IR | 新 API 生成的参数化语义结构正确 | Rep 被正确编码成硬件指令 |
| Resource | 逻辑资源关系正确 | 真实物理资源一定可分配 |
| TaskArg/Launch | 编译态参数声明与运行态值绑定正确 | Runtime/驱动下发一定成功 |
| Runtime Behavior | Host 模型中的同步、访存和动态次数正确 | 真实 CCU 时序与链路正确 |
| Data Result | 集合通信最终内存语义正确 | 性能、稳定性和异常恢复正确 |
| Translator/Hardware | 真实指令和硬件执行正确 | 需要产品环境单独验证 |

当前 asc-devkit AllGather CCU ST 已建立 Register、AllGather IR、逻辑资源、TaskArg、Host Runtime Behavior 和 Data Result Checker。`CcuGroupCopyChecker` 还会按产品源码检查 8 个 Event、64 个 CcuBuffer、两个 Func body、两个 Loop、整块与并行/尾块 LoopGroup，以及 `CalGoSize` 到 taskArgs 11..14 的编码关系。当前检查对象仍是 Host 生成的逻辑编译描述，不是真实 `CcuRepTranslator` 输出；物理资源绑定、微码翻译/装载和硬件执行仍是后续层级。

## 14. 迁移到 asc-devkit/asc-comm 的实施原则

1. 以 `asc-comm@pr24/include/ccu` 为唯一新 API 基线。
2. 迁移 HCCL 算法数据流、offset/stride/repeat、Event/Notify 和 LoopGroup 语义，不逐行复制老 `CcuRep` Kernel 类。
3. 保持函数式 Kernel + `CcuKernelArg` + `HcommCcuKernelRegister` 模式。
4. 将 `taskArgs` 顺序视为稳定 ABI，用 enum/常量和 ST 断言防止 Host/Kernel 失配。
5. ST 直接截获新 `Ccu*` 接口，不经过老 `HcclCcuKernelRegister` 或老 `CcuKernelAlgBase` 兼容层。
6. Checker/SimWorld 消费 API 中立 CCU IR/Task IR，不与 PR24 内部实现类名紧耦合。
7. 先以 AllGather Mesh1D Mem2Mem 打通闭环，再扩展 ReduceScatter/AllReduce、2die、NHR 和 multi-jetty。

## 15. 常见误解

### 15.1 “新 API 内部有 CcuRep，所以还是老 API”

不对。算法编程 API 和编译器内部 IR 是两个层次。新 API 可以使用 CcuRep 作为内部 IR，但算法只依赖 `AscendC::ccu`。

### 15.2 “Kernel 函数在 CCU 上直接按 C++ 运行”

不对。Kernel 函数在 Register 期作为 Host 侧程序生成器执行，CCU 最终执行的是 Translator 生成的 `CcuInstr`。

### 15.3 “Launch 时才编译微码”

不对。微码在 `RegisterEnd` 翻译和加载，Launch 只传入当次动态参数并调用 `rtCCULaunch`。

### 15.4 “LoadArg 直接读了 Host vector”

不对。Register 时 `LoadArg` 只生成“第 N 个参数加载到某 XN”的 Rep/微码。Launch 才把 Host `taskArgs` 拆分为 `CcuTaskParam.args[13]` 交给 Runtime。

### 15.5 “mem2mem 就不需要 CcuBuffer”

不对。mem2mem 表示远端 Write 的源为 LocalAddr/用户内存。本地 GroupCopy 仍可以经 CcuBuffer 做分块流水。

### 15.6 “CCU_MS/CCU_SCHED 就是新老 API”

不对。CCU_MS/CCU_SCHED 是执行/调度模式，KFC/直驱是启动形式，mem2mem/GroupBroadcast 是数据路径，都与 API 新老是正交维度。

### 15.7 “ST 为每个 Rank 创建一个线程，就覆盖了 Mission 并发”

不对。每 Rank 一个 Host 线程模拟的是多个集合通信参与者并行调用。Mission 并发要求在
同一个 Rank/Die 上同时存在多个 Kernel/Mission 执行上下文，两者还要分别检查资源隔离、
调度和内存依赖。

### 15.8 “goSize.addrOffset 就是 Loop 的 GSA offset”

不对。`goSize.addrOffset` 是完整区结束后切换到尾区的一次显式基址调整；Loop GSA
offset 是每次 Loop 迭代的硬件自动地址步长；LoopGroup GSA offset 是并行 clone 之间的
地址步长。三者来自相同切片参数，但作用时机和执行主体不同。

### 15.9 “GroupCopy 是 Host 运行时调用 LocalCopy 搬数据”

不对。GroupCopy 是注册阶段执行的 CCU 程序组合函数，用基础原语构造 Func、Loop、
LoopGroup 和 Event 依赖；实际搬运由 Launch 后的 CCU Mission 按微码执行。

## 16. 关键源码索引

### 16.1 HCCL 完整 AllGather 实现

```text
hccl/src/ops/all_gather/selector/all_gather_auto_selector.cc
hccl/src/ops/all_gather/executor/ins_v2_all_gather_sole_executor.cc
hccl/src/ops/all_gather/template/ccu/ccu_temp_all_gather_mesh_1D_mem2mem.cc
hccl/src/ops/all_gather/template/ccu/kernel/ccu_kernel_all_gather_mesh1d_mem2mem.{h,cc}
hccl/src/ops/op_common/template/ccu/ccu_kernel_alg_base.{h,cc}
hccl/src/ops/op_common/template/ccu/ccu_kernel_utils.{h,cc}
hccl/src/ops/op_common/op_common.cc::HcclGetCcuKernel
hccl/src/ops/op_common/inc/alg_param.h
```

### 16.2 asc-comm PR24 新 API

```text
asc-comm@pr24/include/ccu/ccu_types.h
asc-comm@pr24/include/ccu/ccu_variable.hpp
asc-comm@pr24/include/ccu/ccu_address.hpp
asc-comm@pr24/include/ccu/ccu_local_addr.hpp
asc-comm@pr24/include/ccu/ccu_remote_addr.hpp
asc-comm@pr24/include/ccu/ccu_event.hpp
asc-comm@pr24/include/ccu/ccu_buffer.hpp
asc-comm@pr24/include/ccu/ccu_primitives.hpp
asc-comm@pr24/include/ccu/ccu_launch.h
asc-comm@pr24/include/ccu/ccu_res.h
```

### 16.3 asc-comm PR24 内部编译/执行

```text
asc-comm@pr24/src/ccu/dataplane/hcomm/base_comm/primitives/api_c_adpt/ccu/
asc-comm@pr24/src/ccu/dataplane/hcomm/base_comm/resources/ccu/ccu_kernel/
asc-comm@pr24/src/ccu/dataplane/hcomm/base_comm/resources/ccu/ccu_representation/
asc-comm@pr24/src/ccu/dataplane/hcomm/base_comm/resources/ccu/ccu_microcode/
```

### 16.4 硬件资源、LoopEngine 与队列

```text
hcomm/src/base_comm/resources/ccu/ccu_device/ccu_res_specs.{h,cc}
hcomm/src/base_comm/resources/ccu/ccu_device/ccu_res_batch_allocator.cc
hcomm/src/base_comm/resources/ccu/ccu_device/ccu_comp/ccu_channel/
hcomm/src/base_comm/resources/ccu/ccu_kernel/ccu_kernel.cc
hcomm/src/base_comm/resources/ccu/ccu_kernel/ccu_kernel_mgr.cc
hcomm/src/base_comm/resources/ccu/ccu_microcode/ccu_microcode_v1.h
hcomm/src/base_comm/resources/ccu/ccu_representation/reps/loop/
hcomm/include/ccu/ccu_loop.hpp
hcomm/include/ccu/ccu_primitives.hpp
hcomm/docs/zh/api_ref/comm_opdev/data_plane_api/ccu/
```

### 16.5 配套文档

```text
CCU_AllGather样例解读.md
CCU硬件资源_Channel_Jetty_PFE.md
Devkit_HCCL_ST整体设计与演进方案.md
HCCL_ST整体原理与迁移适配指南.md
开发串讲_AllGather两算法代码流程.md
HCCL执行模型_AICPU_vs_CCU.md
```

---

总结：CCU 算法的本质是“用 `ccu::` API 生成参数化 Representation，在 RegisterEnd 翻译和加载微码，在 Launch 用 `taskArgs/LoadArg` 绑定动态数据，由 CCU 硬件按 Event、Notify、Channel 和 LoopGroup 语义自主执行”。
