# API 选择指南

Ascend C 提供三层梯度化可编程接口体系。本文档聚焦**基础API（C++ Tensor 类库）**与**C API（C 语言指针接口）**的对比与选型，帮助开发者在实际项目中快速选择合适的编程接口。TPipe/TQue 框架是基于基础API 的上层封装，本文在相关章节简要说明其定位。

> 📌 **头文件互斥约束**：基础API 通过 `#include "kernel_operator.h"` 引入，C API 通过 `#include "c_api/asc_simd.h"` 引入，两者**不可在同一源文件中混用**。

---

## 快速决策

### 一句话总结

**基础API 是推荐起点**——它提供 Tensor 抽象、高阶算法库、自动同步框架，覆盖绝大多数算子开发场景。**C API 在以下条件满足时使用**：需要 4 事件通道并发流水重叠 / SubBlock 级核间同步 / 内存屏障按 GM/UB 范围控制 / 纯 C 语言接口。

### 决策流程

```
需要高阶算法？
(LayerNorm/SoftMax/Matmul/Conv/量化/TopK/Dropout...)
    │
    ├── 是 → 基础API (+ TPipe/TQue 框架加速开发)
    │
    └── 否 → 追求极致性能？（4事件通道流水重叠 / 分Pipe全局同步 / 内存屏障范围控制）
                │
                ├── 是 → C API（精细异步模式）
                │
                └── 否 → 基础API (+ TPipe/TQue)
```

---

## 定位与核心差异

### 基础API（C++）

基础API 是 Ascend C 中的 **C++ Tensor 类库层**，定位为通用算子开发的主力接口。它在「硬件指令精确映射」与「开发效率」之间取得平衡：

- **编程模型**：Tensor 抽象（`LocalTensor<T>` / `GlobalTensor<T>`），`TPosition` 枚举（VECIN/VECOUT/VECCALC/A1/B1/CO1 等）隐藏物理存储差异
- **内存管理**：`LocalMemAllocator` 线性分配器，`InitBuffer` 框架级，TBufPool 池化
- **数据搬运**：`DataCopy`（连续/高维切分）、`DataCopyPad`（非对齐），统一接口
- **计算接口**：`Add/Sub/Mul/Exp/Relu/Cast/Compare/Select/Mmad...`，`AscendC::` 命名空间
- **同步机制**：`SetFlag/WaitFlag`（多流水 ISASI）、`PipeBarrier`（单流水）、`Barrier`（全局）、`CrossCoreSetFlag`/`SyncAll`/`IBSet`（核间）、`Mutex`（arch 3510）
- **独有优势**：高阶算法库（LayerNorm、SoftMax、Matmul、Conv、量化等 15 类开箱即用）、扩展 Tensor（带 Layout 自动推导参数）、Reg 矢量计算全套（22 个头文件）、Debug Dump

### C API（C）

C API 是 Ascend C 中的**最底层 C 语言接口**，定位为追求极致性能、充分释放 NPU 硬件潜能的核心路径：

- **编程模型**：指针编程，`__ubuf__ T arr[N]` 静态数组 + `__gm__ T*` 原始指针
- **内存管理**：8 个地址空间限定符（`__gm__`/`__ubuf__`/`__cbuf__`/`__ca__`/`__cb__`/`__cc__`/`__fbuf__`/`__biasbuf__`）直接映射硬件存储
- **数据搬运**：`asc_copy_gm2ub/ub2gm/ub2ub`（方向独立函数），支持 Loop 双循环、N-dim 多维搬运、随路格式转换
- **计算接口**：`asc_add/mul/exp/relu...`（100+ 矢量指令）、`asc_mmad`（矩阵乘加）、`reg_load/store/vector`（寄存器级）
- **同步机制**：三级粒度 — 同步 `_sync` / 异步 `asc_sync` / 精细异步 `asc_sync_notify/wait`（4 个独立 EVENT_ID 通道）
- **独有优势**：4 事件通道并发流水重叠、8 流水线精确控制（PIPE_S/V/M/MTE1/2/3/FIX/ALL）、内存屏障按 GM/UB 范围控制、SubBlock/组间/Intra 核间同步、Lock 非阻塞模式、额外标量/原子操作扩展

### 核心差异速览

| 核心差异 | 基础API | C API |
|------|--------|-------|
| **语言** | C++，`AscendC::` 命名空间 | C，`asc_` 前缀 |
| **编程范式** | Tensor 抽象（`LocalTensor<T>` / `GlobalTensor<T>`），`TPosition` 枚举隐藏物理存储 | 指针编程，`__ubuf__ T arr[N]` 静态数组 + `__gm__ T*` 原始指针 |
| **内存抽象** | `LocalTensor`/`GlobalTensor` + `TPosition`（VECIN/VECOUT/VECCALC/A1/B1/CO1 等 11 种逻辑位置） | `__gm__`/`__ubuf__`/`__cbuf__`/`__ca__`/`__cb__`/`__cc__`/`__fbuf__`/`__biasbuf__` 共 **8 个地址空间限定符，直接对应物理存储** |
| **UB 分配** | `LocalMemAllocator.Alloc<T, N>()` 线性分配器（按调用次序线性，不可回退）；TBufPool 支持池化复用 | `__ubuf__ T arr[N]` 编译期静态声明；或 `extern __ubuf__ char[]` + `<<<dynamic_ub_size>>>` 运行期动态指定 |
| **L1/L0 Cube Buffer** | `LocalMemAllocator<Hardware::L1/L0A/L0B>` 统一接口分配 | `__cbuf__`(L1)/`__ca__`(L0A)/`__cb__`(L0B)/`__cc__`(L0C)/`__fbuf__`(Fixpipe)/`__biasbuf__`(BiasTable) **专属限定符** |
| **Register** | `RegTensor<T>` 对象 + `MaskReg` 掩码寄存器（arch 3510） | C 风格寄存器变量声明（arch 3510） |
| **数据搬运** | `DataCopy(dst, src, count)` 统一接口，支持连续 / 高维切分（`DataCopyParams`）/ 非对齐（`DataCopyPad`） | `asc_copy_gm2ub`/`asc_copy_ub2gm`/`asc_copy_ub2ub` 等**方向独立函数**，支持 **Loop 双循环控制**、**N-dim 多维搬运**（最多 5 维）、随路格式转换（ND2NZ/DN2NZ） |
| **Tensor 抽象层次** | **两层**：基础 Tensor（不带 Layout，需手动传入 size）+ **扩展 Tensor**（`AscendC::Te::MakeTensor` 带 Layout，**自动推导**搬运长度和元素数，当前赋能 Cube 计算） | **无 Tensor 抽象**，需手动计算偏移、步长、元素数 |
| **核内多流水同步** | `SetFlag<P_SRC, P_DST>(0)`/`WaitFlag<P_SRC, P_DST>(0)` — **ISASI 接口**，不保证跨架构兼容，**仅 1 个事件通道** | `asc_sync_notify(pipe, tpipe, id)`/`asc_sync_wait(pipe, tpipe, id)` — **保证兼容**，**4 个独立 EVENT_ID**（EVENT_ID0~3），支持多路并发流水重叠 |
| **核内单流水同步** | `PipeBarrier<P>()`（不支持 PIPE_S） | `asc_sync_pipe(pipe)`（不支持 PIPE_S，**额外支持 PIPE_ALL**） |
| **核内全局同步** | `Barrier()` | `asc_sync()` / `asc_sync_vec()` / `asc_sync_mte2()` / `asc_sync_mte3()` — **按流水线细分** |
| **核内内存屏障** | `DataSyncBarrier()` — 无条件等待**全部**内存访问完成 | `asc_sync_data_barrier(DSB_DDR/DSB_UB)` —**可按需只等 GM 或只等 UB** |
| **核内锁（arch 3510）** | `Lock/Unlock` 仅阻塞模式 + `AllocMutexID`/`ReleaseMutexID` 动态 ID 管理 | `asc_lock/unlock` 支持 `ASC_LOCK_BLOCK`/`ASC_LOCK_NON_BLOCK`（**非阻塞模式**），需手动指定 mutex_id [0,31] |
| **硬件核间同步** | `CrossCoreSetFlag(mode)`/`CrossCoreWaitFlag(mode)`（ISASI，mode 0~3） | `asc_sync_block_arrive/wait`（flag_id 0~15）+ **`asc_sync_subblock/inter/intra_arrive/wait`**（SubBlock/组间/Intra 核内，C API 独有） |
| **软件核间同步** | `IBSet/IBWait`(GM 轮询) + `SyncAll`(软件全核) + `InitDetermineComputeWorkspace/NotifyNext/WaitPre`(确定性顺序) — **基础API 独有** | ❌ 无 |
| **标量原子操作** | `AtomicAdd/Max/Min/Cas/Exch`（5 种） | `asc_atomic_add/sub/or/xor/and/max/min/inc/dec/exch/cas`（**12 种**，多出 sub/or/xor/and/inc/dec） |
| **标量计算** | `Cast/CountLeadingZero/GetBitCount/CountBitsCntSameAsSignBit/WriteGmByPassDCache`... | `asc_clz/asc_ffs/asc_ffz/asc_popc/asc_zero_bits_cnt/asc_set_nthbit/asc_clear_nthbit/asc_sflbits/asc_store_dev/asc_float2int32`...（**各有侧重，部分互无等价接口**） |
| **高阶算法库** | ✅ **15 类开箱即用**（LayerNorm/SoftMax/Matmul/Conv/量化/TopK/Dropout...） | ❌ 需从原语自行组合 |
| **性能天花板** | 中（单事件通道 + Tensor 抽象开销） | 高（4 事件通道并发 + 直接硬件映射 + 分 Pipe 全局同步） |

---

## 功能能力全景

### 能力矩阵

| 能力维度 | 基础API | C API | 差异说明 |
|----------|:---:|:---:|---------|
| **内存抽象** | ✅ Tensor（基础 + 扩展 Layout） | ✅ 8 个地址空间限定符 | 基础API 更高层；C API 更贴近硬件 |
| **内存分配** | ✅ LocalMemAllocator / TBufPool | ✅ `__ubuf__` 静态数组 / `extern __ubuf__` 动态 | 基础API 支持池化复用；C API 编译期确定 |
| **数据搬运** | ✅ DataCopy（连续/高维切分/非对齐） | ✅ `asc_copy_*`（方向独立 + Loop/N-dim/L2 控制） | 基础API 接口统一；C API 参数更丰富 |
| **矢量计算** | ✅ Add/Sub/Mul/Exp/Relu... | ✅ `asc_add`/`asc_mul`/`asc_exp`... | 功能等价，接口风格不同 |
| **矩阵计算** | ✅ Mmad/LoadData 多模式 | ✅ `asc_mmad`/`asc_copy_l12l0*` | 基础API 矩阵 ISASI 更全面 |
| **Reg 寄存器计算** | ✅ 22 个头文件（全套） | ✅ 4 个头文件（核心原语） | 基础API 覆盖更广 |
| **缓存控制** | ✅ DCC/ICache/L2Cache/ByPassDCache | ✅ `asc_dcci`/`asc_icache_preload`... | 功能等价 |
| **DMA随路原子** | ✅ SetAtomicAdd/Max/Min/Type | ✅ `asc_set_atomic_*` | 功能等价 |
| **标量原子操作** | ✅ AtomicAdd/Max/Min/Cas/Exch（5 种） | ✅ `asc_atomic_*`（12 种：含 sub/or/xor/and/inc/dec） | C API 种类更全 |
| **标量计算** | ✅ Cast/CountLeadingZero/GetBitCount | ✅ `asc_clz`/`asc_ffs`/`asc_ffz`/`asc_popc`... | 各有侧重 |
| **精细事件同步** | ❌ 仅 1 个事件通道（ISASI） | ✅ 4 个独立 EVENT_ID（兼容） | **C API 独有** |
| **分 Pipe 全局同步** | ❌ 无 MTE2/MTE3 细分同步 | ✅ `asc_sync_mte2/mte3`/`asc_sync_pipe(PIPE_ALL)` | **C API 独有** |
| **内存屏障范围** | DataSyncBarrier（等全部） | `asc_sync_data_barrier(DSB_DDR/DSB_UB)`（可控范围） | **C API 优势** |
| **SubBlock/组间同步** | ❌ 无 | ✅ `asc_sync_subblock/inter_arrive/wait` | **C API 独有** |
| **Intra 核内同步** | ❌ 无 | ✅ `asc_sync_intra_arrive/wait`（arch 3510） | **C API 独有** |
| **软件核间同步** | ✅ IBSet/IBWait/SyncAll(软) | ❌ 无 | **基础API 独有** |
| **全核同步 SyncAll** | ✅ 硬件+软件模式 | ❌ 无 | **基础API 独有** |
| **确定性顺序同步** | ✅ InitDetermineComputeWorkspace/NotifyNext/WaitPre | ❌ 无 | **基础API 独有** |
| **任务间同步** | ✅ SetNextTaskStart/WaitPreTaskEnd | ❌ 无 | **基础API 独有** |
| **高阶算法库** | ✅ 15 类（LayerNorm/SoftMax/Matmul/Conv/量化/TopK...） | ❌ 无 | **基础API 独有** |
| **扩展 Tensor（Layout）** | ✅ `AscendC::Te::MakeTensor` 自动推导参数 | ❌ 无 | **基础API 独有** |
| **开发效率** | 高（Tensor + 高阶 API + TPipe/TQue） | 低（手动管理一切） | — |
| **调试/断言** | ✅ `assert`/`ascendc_assert`（运行时）+ `static_assert`（编译期） | ❌ 无 | **基础API 独有** |
| **CPU 调试/仿真** | ✅ `ASCENDC_CPU_DEBUG=1`（CPU 孪生调试） | ❌ 无 | **基础API 独有** |
| **上板打印/异常检测/性能统计** | ✅ printf + 异常检测 + 性能统计 | ❌ 无 | **基础API 独有** |
| **Tiling（Host 侧参数传递）** | ✅ 13 个 Tiling 接口 + adv_api 高阶 Tiling | ❌ 无 | **基础API 独有** |
| **Cube 分组管理** | ✅ CubeResGroupHandle/GroupBarrier/KfcWorkspace | ❌ 无（仅有基本 Cube 计算/搬运） | **基础API 独有** |
| **特殊寄存器访问** | ✅ SPR 读写 + 控制寄存器 + 饱和标志 | ❌ 无（仅 sys_var 系统变量查询） | **基础API 独有** |

| **数组分配约束** | N/A（采用 Tensor 内存模型） | ✅ 仅编译期常量大小，不可与 `asc_get_phy_buf_addr` 混用 | **C API 独有约束** |
| **执行空间修饰符** | `__global__ __vector__/__cube__/__mix__` | `__global__ __vector__ __aicore__`（需显式 `__aicore__`） | **C API 独有约束** |
| **性能天花板** | 中（单事件通道 + Tensor 抽象开销） | 高（4 事件通道 + 直接硬件映射） | — |

### 基础API 独有（详表）

| 分类 | 能力 |
|------|------|
| **高阶算法** | LayerNorm/LayerNormGrad/DeepNorm、BatchNorm、RmsNorm、SoftMax(标准/FlashV2/Simple/Log/Grad)、GELU/SiLU/SwiGLU/ReGLU/GeGLU/Swish/Sigmoid、Matmul(高阶封装)、Conv3D、TopK/Sort/MrgSort/Concat/Extract、Quant/Dequant/AntiQuant、Dropout/PhiloxRandom、Transpose/Broadcast/Pad/Fill、ReduceSum/Mean/Max/Min、Arange/Select/Clamp、Erf/Erfc/Power/Frac/Lgamma/Digamma、CumSum/Round/Trunc/Ceil/Floor/Sign/Xor |
| **Tensor 编程** | LocalTensor/GlobalTensor + TPosition；DataCopy(连续/高维切分/DataCopyPad 非对齐)；扩展 Tensor(Tensor API, `AscendC::Te::MakeTensor` 带 Layout 自动推导)；Coordinate/Layout/ShapeInfo/ListTensorDesc/TensorDesc |
| **Reg 计算** | 22 个头文件：直方图/压缩/Pack/Unpack/FusedOps/三元标量/Arange/二进制标量/Memory Barrier/VF 调用机制等 |
| **矩阵 ISASI** | LoadData(2D/V2/MX/BitMode/3D/WithStride/WithTranspose/WithSparse)、BroadCastVecToMM、Fill/SetFmatrix/SetLoadDataBoundary/SetLoadDataRepeat*/SetLoadDataPaddingValue |
| **调试与诊断** | `assert`/`ascendc_assert` 运行时断言（可通过 `ASCENDC_DUMP=0` 或 `NDEBUG` 关闭）；`static_assert` 编译期类型检查（18+ 处类型校验）；DumpTensor 张量级 Debug Dump；上板打印（printf）；异常检测；性能统计；CPU 孪生调试（`ASCENDC_CPU_DEBUG=1`） |
| **Tiling（Host 侧）** | `GET_TILING_DATA`/`GET_TILING_DATA_WITH_STRUCT`/`GET_TILING_DATA_MEMBER`、`COPY_TILING_WITH_STRUCT`/`COPY_TILING_WITH_ARRAY`、`TILING_KEY_IS`/`TILING_KEY_LIST`、`REGISTER_TILING_DEFAULT`/`REGISTER_TILING_FOR_TILINGKEY`/`REGISTER_NONE_TILING` 等 13 个接口 + adv_api 高阶 Tiling 封装的 Host 侧调优接口 |
| **Cube 分组管理** | `CubeResGroupHandle`、`GroupBarrier`、`KfcWorkspace`（均为 ISASI） |
| **特殊寄存器访问** | `GetSpr`/`ClearSpr`（通用）、`SetCtrlSpr`/`GetCtrlSpr`/`ResetCtrlSpr`（ISASI 控制寄存器）、`GetSaturationFlag`/`SetSaturationFlag`（ISASI 饱和标志） |


### C API 独有（详表）

> 📌 经核实，缓存控制、DMA随路原子操作**基础API 同样具备**。以下仅列**确认独有**的能力。

| 分类 | 能力 | 说明 |
|------|------|------|
| **精细事件同步** | `asc_sync_notify/wait` + EVENT_ID0~3 | 4 个独立事件通道，支持多路并发流水重叠 |
| **分 Pipe 全局同步** | `asc_sync_pipe(PIPE_ALL)` | 基础API PipeBarrier 不支持 PIPE_ALL |
| | `asc_sync_mte2/mte3` | 按 MTE 细分维度同步（基础API 无） |
| | `asc_sync_vec` | 全部流水线同步 |
| **内存屏障范围** | `asc_sync_data_barrier(DSB_DDR/DSB_UB)` | 按需只等 GM 或只等 UB |
| **锁非阻塞** | `asc_lock(pipe, id, ASC_LOCK_NON_BLOCK)` | 仅 arch 3510 |
| **SubBlock 同步** | `asc_sync_subblock_arrive/wait` | SubBlock 级核间同步 |
| **组间 Block 同步** | `asc_sync_inter_arrive/wait` | 组间 Block 信号同步 |
| **Intra 核内同步** | `asc_sync_intra_arrive/wait` | Vector Core ↔ Cube Core（arch 3510） |
| **标量操作扩展** | `asc_ffs/ffz`、`asc_popc/zero_bits_cnt`、`asc_set_nthbit/clear_nthbit`、`asc_sflbits`、`asc_store_dev`、`asc_float2int32` | 部分基础API 有等价功能，部分无 |
| **标量原子扩展** | `asc_atomic_sub/or/xor/and/inc/dec`（6 种） | 基础API 仅有 add/max/min/cas/exch |
| **UDMA 搬运** | Loop 双循环、N-dim 搬运、随路格式转换（ND2NZ/DN2NZ）、图像加载（`asc_load_image_to_cbuf`） | 基础API 无 N-dim 搬运和图像加载 |
| **L0C 随路量化** | `asc_set_l0c_copy_params/prequant` + `asc_set_l0c2gm_config` + `asc_get_l0c2gm_relu/unitflag/prequant` | L0C→GM 搬运过程中的量化/Relu 参数显式配置 |

---

## 同步控制深度对比

### 全景一览

```
                      核内同步                        核间同步             任务间
                 多流水  单流水  全局  内存屏障  流水锁  Block  SB/组间/Intra  子Kernel
基础API           S/W-F  PipeBr   -   DataSyncB Lock  CCS-F+IB/SyncAll  -   SetNextTask+
                  (ISASI)(ISASI)     (ISASI)  (ISASI)                                WaitPreTaskEnd

C API          notify/ sync_  sync/ data_    lock/ block_  subblock/inter/   -
               wait    pipe   sync_ barrier  unlock arrive  intra_arrive
                               vec/
                               mte2/3
```

S/W-F = SetFlag/WaitFlag, PipeBr = PipeBarrier, CCS-F = CrossCoreSetFlag/WaitFlag

### 关键差异

| 同步类型 | 基础API | C API | 差异 |
|---------|--------|-------|------|
| **多流水事件同步** | `SetFlag<P_V, P_MTE3>(0)`（ISASI，1 通道） | `asc_sync_notify(P_V, P_MTE3, EVENT_ID0)`（兼容，4 通道） | C API 事件通道更多、跨架构兼容 |
| **单流水同步** | `PipeBarrier<P_V>()` | `asc_sync_pipe(P_V)` | 均不支持 PIPE_S |
| **全局同步** | `Barrier()` | `asc_sync()` / `asc_sync_mte2/mte3()` / `asc_sync_vec()` | C API 按流水线细分 |
| **内存屏障** | `DataSyncBarrier()`（等全部） | `asc_sync_data_barrier(DSB_DDR/DSB_UB)` | C API 可按 GM/UB 范围 |
| **锁（arch 3510）** | `Lock/Unlock` + `AllocMutexID` 分配 | `asc_lock/unlock` 含 `ASC_LOCK_NON_BLOCK` | 基础API 有 ID 管理；C API 有非阻塞模式 |
| **硬件核间同步** | `CrossCoreSetFlag/WaitFlag`（mode 0~3） | `asc_sync_block_arrive/wait`（flag_id 0~15） | 语义等价 |
| **软件核间同步** | `IBSet/IBWait`(GM 轮询) + `SyncAll`(软) | ❌ | 基础API 独有 |
| **全核 SyncAll** | 硬件融合模式 + 软件模式 | ❌ | 基础API 独有 |
| **确定性顺序** | `InitDetermineComputeWorkspace/NotifyNext/WaitPre` | ❌ | 基础API 独有 |
| **任务间同步** | `SetNextTaskStart/WaitPreTaskEnd`（SuperKernel） | ❌ | 基础API 独有 |
| **SubBlock 同步** | ❌ | `asc_sync_subblock_arrive/wait` | C API 独有 |
| **组间 Block 同步** | ❌ | `asc_sync_inter_arrive/wait` | C API 独有 |
| **Intra 核内同步** | ❌ | `asc_sync_intra_arrive/wait`（arch 3510） | C API 独有 |


TPipe/TQue 框架本质是对基础API `SetFlag`/`WaitFlag` 的工程化封装：

| TPipe/TQue | 底层等价 | 解决的问题 |
|-----------|---------|----------|
| `EnQue` | `SetFlag<SRC, DST>(0)` | 写后读（WAR）依赖 |
| `DeQue` | `WaitFlag<SRC, DST>(0)` | 写后读（WAR）依赖 |
| `AllocTensor` | `WaitFlag`（等内存可覆写） | 读后写（RAW）冲突 |
| `FreeTensor` | `SetFlag`（通知内存已释放） | 读后写（RAW）冲突 |

**流水并行度对比**：

```
基础API 手动 Barrier（串行）：
  [======搬运======][===计算===][===写回===]

TPipe/TQue（框架双缓冲自动并行）：
  [搬运1][搬运2]...
          [计算1][计算2]...
                  [写回1][写回2]...

C API 精细异步（4事件通道并发重叠）：
  MTE2:[搬运0][搬运1]...
        ↓EV0   ↓EV1
  V:   [计算0][计算1]...
        ↓EV0   ↓EV1
  MTE3:[写回0][写回1]...
```

### Reg 矢量计算同步

| 维度 | 基础API `LocalMemBar` | C API `asc_mem_bar` |
|------|----------------------|---------------------|
| **接口** | `LocalMemBar<MemType::SRC, MemType::DST>()` | `asc_mem_bar(MEM_TYPE)` |
| **流水线类型** | VEC_STORE/LOAD/ALL, SCALAR_STORE/LOAD/ALL（6 种） | VV_ALL/VST_VLD/VLD_VST 等 16 种枚举 |
| **支持** | 仅 arch 3510 | 仅 arch 3510 |
| **约束** | 读写同一寄存器自动保序 | 同左 |

---

## C API 使用注意事项

切换到 C API 开发时，相比基础API 需要关注以下关键差异：

### 缺少的能力（基础API 有，C API 无）

| 缺失能力 | 影响 | 缓解措施 |
|---------|------|---------|
| **调试/断言子系统** | 无 `assert`/`ascendc_assert` 运行时断言，无 `static_assert` 编译期类型检查，类型错误可能延迟到运行时才发现 | 加强手工类型校验；依赖编译器的隐式类型检查 |
| **CPU 孪生调试** | 无 `ASCENDC_CPU_DEBUG=1` 模拟能力，仅能在真实 NPU 上调试 | 先在基础API 上验证逻辑，确认正确后再迁移到 C API |
| **DumpTensor / 上板打印 / 异常检测 / 性能统计** | 调试手段严重受限 | 通过 `asc_copy_ub2gm` 手动搬运中间结果到 GM 供 Host 读取 |
| **Tiling（Host 侧参数传递）** | 无 `GET_TILING_DATA` 等 Host 侧传参接口，需自行设计参数传递方案 | 通过 `<<<>>>` 的启动参数 + GM 地址偏移手动传参 |
| **自动同步（TQue 框架）** | 无 `EnQue/DeQue` 自动流水同步，所有同步必须手动插入 `asc_sync_notify/wait` 或 `asc_sync` | 严格遵循"搬运→通知→等待→计算→通知→等待→写回"的事件同步模式 |
| **高阶算法库** | LayerNorm/SoftMax/Matmul/Conv 等需从原语手动实现 | 参考基础API 高阶算法实现，或从基础API 代码逆向推导算法逻辑 |
| **Cube 分组管理** | 无 Cube 资源组管理和 GroupBarrier | 仅使用基本 Cube 计算/搬运接口 |
| **特殊寄存器访问** | 无 SPR 直接读写、无饱和标志控制 | 如有特殊寄存器需求则无法实现 |

### C API 特有的约束

| 约束 | 说明 |
|------|------|
| **`asc_init()` 必须调用** | 每个 Kernel 入口**必须显式调用**，基础API 由编译器自动生成 |
| **`__aicore__` 必须显式声明** | Kernel 函数需完整声明 `__global__ __vector__ __aicore__`，基础API 可省略 `__aicore__` |
| **数组仅编译期常量大小** | `__ubuf__ T arr[N]` 中 N 必须是编译期常量，不支持运行时动态大小（除非用 `extern __ubuf__ char[]` + `<<<dynamic_ub_size>>>`） |
| **不可混用 `asc_get_phy_buf_addr`** | 数组分配方式与 `asc_get_phy_buf_addr()` 混用会导致地址重叠 |
| **地址对齐手动保证** | UB 32B / L1 32B / L0A/L0B 512B / L0C 64B，编译器不为数组声明自动插入 padding |
| **同步完全手动** | 无 TQue 框架自动同步，每次搬运后必须显式插入 `asc_sync_notify/wait` 或 `asc_sync` |
| **架构差异显式处理** | `__NPU_ARCH__ == 2201` vs `__NPU_ARCH__ == 3510` 的接口差异在代码中直接可见，需手动编写条件编译分支 |
| **地址重叠需自行保证** | 高维切分计算中地址重叠需遵循"单次迭代 100% 重叠，多次迭代不支持"的约束 |
| **repeatTime ≤ 255** | 硬件限制，与基础API 相同 |

---

## 场景速查与迁移建议

### 场景速查

| 使用场景 | 推荐 API | 理由 |
|----------|----------|------|
| 快速原型验证 / PoC | 基础API + TPipe/TQue | 开发效率最高 |
| 标准算子（Add/Sub/Mul/Relu...） | 基础API | 通用首选 |
| 归一化（LayerNorm/BatchNorm/RmsNorm） | **基础API** | 仅限基础API 高阶算法库 |
| SoftMax / GELU / SiLU / Swish 等激活 | **基础API** | 仅限基础API 高阶算法库 |
| Matmul / Conv3D / Quant | **基础API** | 仅限基础API 高阶算法库 |
| TopK / Sort / Dropout / 随机数 | **基础API** | 仅限基础API |
| 张量变换（Transpose/Broadcast/Pad） | **基础API** | 仅限基础API |
| 4 事件通道并发流水重叠 | **C API** | `asc_sync_notify/wait` + EVENT_ID0~3 唯一支持 |
| SubBlock / 组间 Block 同步 | **C API** | 独有接口 |
| 内存屏障按范围（DSB_DDR/DSB_UB） | **C API** | 独有能力 |
| 分 Pipe 全局同步（mte2/mte3） | **C API** | 独有接口 |
| C 语言项目集成 | **C API** | 纯 C 接口 |
| 算子模板 / 代码生成 | 基础API | C++ 模板支持 |
| SIMD + SIMT 混合 | 基础API 或 C API | 基础API 需 `--enable-simt` |

### 迁移建议

| 迁移方向 | 收益 | 代价 |
|----------|------|------|
| C API → 基础API | 获得高阶算法库、Tensor 抽象、TPipe/TQue 自动同步 | 失去 4 事件通道精细同步、SubBlock/组间/Intra 同步、分 Pipe 全局同步、部分标量原子扩展 |
| 基础API → C API | 获得多事件通道流水重叠、细粒度同步控制、更多标量原子操作 | 失去高阶算法库、需手动管理内存与同步、代码量显著增加 |

> 📌 **核心建议**：基础API 已覆盖缓存控制、DMA随路原子操作和绝大多数同步场景，对于大多数算子开发场景是**最佳起点**。仅在确认需要 4 事件通道并发流水重叠、SubBlock/组间级核间同步、内存屏障按 GM/UB 范围控制或纯 C 语言接口时，才考虑迁移到 C API。

---

## 参考资料

- [SIMD API 参考](../API参考/SIMD-API/README.md)
- [基础API 文档](../API参考/SIMD-API/基础API/基础API.md)
- [C API 文档](../API参考/SIMD-API/C-API/README.md)
- [基于 Tensor 的 C++ 编程概述](./基于Tensor的CPP编程/CPPTensor编程概述.md)
- [TPipe/TQue 框架编程原理](./基于TPipe-TQue框架编程/TPipe-TQue框架编程原理.md)
- [基于指针的 C 语言编程概述](./基于指针的C语言编程/C语言编程概述.md)
- [硬件实现](../../高级编程/硬件实现/基本架构.md)
