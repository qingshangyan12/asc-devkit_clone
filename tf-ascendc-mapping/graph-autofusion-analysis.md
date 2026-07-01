# Graph-Autofusion 自动融合原理深度分析

> `graph-autofusion` 是 AscendC/CANN 框架中**最关键的性能优化组件**之一。
> 它通过对计算图的自动分析，将多个用户级算子融合为单个 Ascend C 核函数，
> 使得多个算子在同一块物理内存 (UB) 上连续计算，彻底消除中间结果的数据搬运开销。

---

## 一、Autofuse (自动融合) 核心原理

### 1.1 为什么需要融合

在标准执行模式下，每个算子独立执行:

```
算子1 (Add)                       算子2 (Mul)                       算子3 (Relu)
  │                                  │                                  │
  ├─ Load x from HBM → UB           ├─ Load y1 from HBM → UB          ├─ Load y2 from HBM → UB
  ├─ Compute (UB → UB)              ├─ Compute (UB → UB)              ├─ Compute (UB → UB)
  └─ Store y1 to HBM                └─ Store y2 to HBM                └─ Store y3 to HBM
```

每个算子都要经过 **HBM → UB 加载** 和 **UB → HBM 写回**。HBM 带宽是现代 NPU 的核心瓶颈。

Autofuse 将三个算子融合为一个:

```
融合后 (Add + Mul + Relu)
  │
  ├─ Load x from HBM → UB         ← 只加载一次
  ├─ Add  (UB → UB)               ← UB 原地计算
  ├─ Mul  (UB → UB)               ← UB 原地计算
  ├─ Relu (UB → UB)               ← UB 原地计算
  └─ Store y3 to HBM              ← 只写回一次
```

**消除 (N-1) × 2 次 HBM 读写**。

### 1.2 融合的基本原理

Autofuse 不依赖手工编写的融合 Pattern，而是通过**算子分类 + 依赖分析 + 代码合成**三个步骤自动生成融合核函数:

```
完整算子图 (TensorFlow / PyTorch)
    │
    ▼
[1] 算子分类 ──────────────────────────────────────────────
    识别每个算子的类别: Elewise / Broadcast / Reduce / MatMul / Concat / ...
    按硬件执行单元分组: AICore (Cube+Vector) / AIVCore (Vector-only)
    │
    ▼
[2] 依赖分析 ──────────────────────────────────────────────
    构建算子间数据流图，合并连续轴
    检测可融合链: 同类型 + 无Data依赖切断
    │
    ▼
[3] 图分区 ────────────────────────────────────────────────
    ScheduleGroupPartitioner 将图划分为 ScheduleGroup
    每组是可在 UB 上连续计算的算子序列
    │
    ▼
[4] 自动调度 ──────────────────────────────────────────────
    为每个 Group 生成最优 Tiling 方案:
      - 轴划分: BlockOuter(跨核) / TileOuter(迭代) / TileInner(单次)
      - 双缓冲策略的选择
      - Cube+Vector 融合模式选择
    │
    ▼
[5] 代码生成 ──────────────────────────────────────────────
    按 ApiCall 类型生成 Ascend C 源码:
      - 循环结构 (双缓冲Pipeline)
      - ApiCall 序列 (Add/Mul/Relu/...)
      - 同步指令 (SetFlag/WaitFlag/PipeBarrier)
    │
    ▼
[6] 编译链接 ──────────────────────────────────────────────
    bisheng 编译器编译 → .so + .json
```

---

## 二、输入输出格式

### 2.1 输入格式

**HintGraph (AscGraph)** — 内部图 IR:

```python
# 通过 Python API 构建 (ascir_api.py)
g = ascir.HintGraph("my_fusion")

# Data: 输入张量, 指定shape和dtype
x = ascir.Data([batch, 512], name="input", dtype=ascir.DType.FLOAT16)
w = ascir.Data([512, 256], name="weight", dtype=ascir.DType.FLOAT16)

# Load: 从 HBM 加载到 UB 的指令, 指定轴、尺寸、步长
x_ub = ascir.Load(g, x, axis=[0, 1], size=[16, 512], stride=[512, 1])

# Broadcast: 广播操作
bias = ascir.Data([1, 256], name="bias")
bias_ub = ascir.Broadcast(g, bias, axis=[0, 1])

# 计算操作
y = ascir.Add(g, x_ub, bias_ub)
y = ascir.Relu(g, y)

# Store: 从 UB 写回 HBM
ascir.Store(g, y)
```

**FusedGraph** — GE 或 Inductor 产出的复合图:
```
FusedGraph
  ├─ AscGraph[0]: sub-graph-0 (多项算子)
  ├─ AscGraph[1]: sub-graph-1 (多项算子)
  └─ ...
```

### 2.2 输出格式

```cpp
struct CodegenResult {
    string proto;         // 算子接口定义 (inputs/outputs/attrs)
    string tiling_data;   // Tiling 数据结构体头文件
    string tiling;        // 主机端 Tiling 函数 (.cpp)
    string kernel;        // 设备端核函数 (.cpp, Ascend C 代码)
    string infer_shape;   // Shape 推断函数
};
```

最终产出: **`lib{kernel_name}.so`** + **`{kernel_name}_op_kernel.json`** 放入 `kernel_meta/` 目录。

---

## 三、完整融合管线 (每步详解)

### 步骤1: 图预处理 (PreProcess)

**文件**: `optimize/pre_process.cpp`

```
输入: ComputeGraph / AscGraph
操作:
  1. 删除悬空节点 (RemoveDanglingNodes)
  2. 归一化轴 ID (NormalizeAxisIds)
  3. 插入类型转换节点 (DtypeConsistency)
     当算子输入/输出类型不匹配时，自动插入 Cast 节点
  4. 标量广播优化 (ScalarBroadcastOptimization)
  5. 标量转 1D 张量 (ScalarTo1DTensor)
  6. MaskedFill 输入重排 (MaskedFillInputReorder)
  7. Cast 优化 (CastOptimization)
```

### 步骤2: 轴合并 (MergeContinuousAxis)

**文件**: `optimize/fused_graph.cpp`

核心思想: 当相邻轴的 stride 连续时合并为单一轴，减少循环嵌套深度:

```
合并前: shape=[512], stride=[1]        → 1 次循环
合并前: shape=[16, 32], stride=[32, 1] → 2 次嵌套循环
检查:   stride[0] == stride[1] * repeat[1]  (32 == 1*32 ✓)
合并后: shape=[512], stride=[1]        → 1 次循环
```

```cpp
bool IsAxisContinuous(int axis0, int axis1) {
    // stride[i] == stride[i+1] * repeat[i+1]
    return stride[axis0] == stride[axis1] * repeat[axis1];
}
```

### 步骤3: 图分区 (ScheduleGroupPartitioner)

**文件**: `optimize/schedule_task_generator.cpp`

这是融合的核心决策步骤。根据算子类型将图划分为 `ScheduleGroup`:

```
分组规则:

┌────────────────────┬──────────────────────────────────────┐
│ 算子组类型          │ 包含的算子                            │
├────────────────────┼──────────────────────────────────────┤
│ Elewise            │ Add, Sub, Mul, Div, Relu, Exp, Sqrt │
│ Broadcast          │ Broadcast, Duplicate                 │
│ Reduce             │ ReduceSum, ReduceMax, ArgMax         │
│ Concat             │ Concat, ConcatD                      │
│ Transpose          │ Transpose, TransposeD                │
│ Gather             │ Gather                               │
│ Cube (MatMul/Conv) │ MatMul, Conv2D, BatchMatMul          │
│ Split              │ Split                                │
└────────────────────┴──────────────────────────────────────┘
```

**跨组关系** (`groups_relations_in`): 记录组间的 Tensor 流动方向，用于确定组间依赖和并行可能性。

### 步骤4: 自动调度 (AutoSchedule)

**文件**: `optimize/autoschedule.cpp`

对每个 `ScheduleGroup` 进行 Tiling 方案搜索:

**4.1 TilingCase 生成**

将算子轴的每个维度分配 5 种 AxisType 之一:

```
AxisType 枚举:
  ├─ kAxisTypeBlockOuter  → 按 core 数量分裂，不同 core 计算不同块
  ├─ kAxisTypeBlockInner  → 每个 core 内部的块迭代
  ├─ kAxisTypeTileOuter   → 外层循环迭代（每轮重新加载数据）
  ├─ kAxisTypeTileInner   → 内层 UB 驻留（数据一次加载后复用）
  └─ kAxisTypeOriginal    → 保持原始不分裂
```

**4.2 模板选择**

```
缩减模板 (ReduceTemplateType):
  ├─ kCommon      → 标准 Reduce 模板 (Phase1: 部分Reduce + Phase2: 合并)
  ├─ kAllLoad     → 全量加载模板 (适合小规模 Reduce)
  └─ kRCore       → 多核 Reduce 模板 (R轴分裂, 跨核归约)

Cube 融合模板 (CubeTemplateType):
  ├─ kFixpip      → 快速路径: MatMul + ReLU (直接 Fixpipe 输出)
  ├─ kUBFuse      → UB 融合: Cube 输出保留在 UB 供后续 Vec Op 使用
  └─ kCommon      → 标准路径: Cube 输出写回 HBM, 后续 Op 重新加载
```

**4.3 NDDMA 模板打分**

```cpp
// 动态 shape 场景，运行时选择最优 Tiling
using ScoreFunc = std::function<double(
    const vector<int64_t>& actual_shape,
    const TilingCase& case_info
)>;
// 分数 = 数据复用率 / 内存消耗 × 并行度
// 运行时根据实际 shape 选择得分最高的 Tiling 方案
```

### 步骤5: Buffer 分配 (BufQueAlloc)

**文件**: `optimize/buf_que_allocator.cpp`

管理 UB (Unified Buffer, 256KB) 上的内存分配:

```
分配策略:
  1. 按 Tensor 生命周期注册 → MemReuseManager
  2. 生命周期不重叠的 Tensor 共享同一块 UB 地址
  3. 分配 TQue (Queue, 用于 Cube↔Vector 通信)
  4. 分配 TBuf (Buffer, 用于 Vector 计算中间结果)
```

**内存复用示例**:
```
时间线:  Add 结果 ──→ Mul 输入 ──→ Mul 结果 ──→ Relu 输入 ──→ Relu 结果
UB分布:  [    buf0    ][    buf1    ][    buf0    ][    复用buf0     ]
```

### 步骤6: 代码生成 (CodeGen)

**文件**: `codegen/codegen.cpp`, `codegen/kernel_codegen.cpp`

**6.1 ApiCall 对象工厂**

根据 AscendIR 节点类型映射到对应代码生成器:

```
节点类型                    → ApiCall 子类
─────────────────────────────────────────────
Add, Sub, Mul, Div         → BinaryApiCall
Exp, Relu, Neg, Sqrt       → UnaryApiCall
Abs, Sigmoid, Reciprocal   → UnaryApiTmpCall (需要临时buffer)
GE, GT, LE, LT, EQ, NE     → CompareApiCall
Broadcast, Duplicate       → BroadcastApiCall
ReduceSum, ReduceMax       → ReduceApiCall
Load (HBM→UB)              → LoadApiCall
Store (UB→HBM)             → StoreApiCall
Cast (类型转换)             → CastApiCall
Transpose                  → TransposeApiCall
Gather                     → GatherApiCall
Concat                     → ConcatApiCall
Where (条件选择)            → WhereApiCall
LeakyRelu                  → LeakyReluApiCall
```

**6.2 循环结构建立 (Loop)**

```
Loop::ConstructFromNodes() 构建分层循环树:

  BlockOuter 层:
    for (block = 0; block < tiling.blockDim; block++)
      TileOuter 层:
        for (tile = 0; tile < tiling.tileCount; tile++)
          ┌─ DataCopy (加载 TileOuter 数据)
          └─ TileInner 层 (UB 驻留, 无数据搬运)
               ├─ Loop①: for (i = 0; i < tileSize; i++)
               │     ApiCall①: Add(ub_a, ub_b, ub_c, align)
               │     ApiCall②: Mul(ub_c, ub_d, ub_e, align)
               └─ Loop②: for (j = 0; j < tileSize; j++)
                     ApiCall③: Relu(ub_f, ub_g, align)
          └─ DataCopy (写回 TileOuter 结果)
```

**6.3 每个 ApiCall 的三个阶段**

```cpp
class ApiCall {
    // 阶段1: Pre-Process — 等待输入数据就绪, 分配输出buffer
    string GenPreProcess();
    
    // 阶段2: API Call — 生成计算指令
    string GenApiCall();  // 例如: "Muls(x, y, scalar, size, align)"
    
    // 阶段3: Post-Process — 标记输出就绪, 释放输入buffer
    string GenPostProcess();
};
```

**6.4 表达式转换**

`codegen/expression_convert.cpp` 处理循环索引到实际地址的转换:

```cpp
// 图表达式: y = x[i + offset]
// 转换为 Tiler 表达式:
//   y_addr = (i + offset) * stride
//   y_size = Tiler::Size(i, offset, actualSize)
```

**6.5 生成的核函数结构**

```cpp
#include "kernel_operator.h"

struct AutofuseTilingData {
    int64_t totalSize;
    int64_t blockDim;
    // ...
};

extern "C" __global__ __aicore__ void fused_kernel_name(
    GM_ADDR input0, GM_ADDR input1, GM_ADDR output0,
    GM_ADDR workspace, GM_ADDR tiling_data)
{
    // 1. 解析 Tiling 数据
    AutofuseTilingData t;
    GET_TILING_DATA(t, gm_tiling_data);
    
    // 2. 设置 Global Tensor
    AscendC::GlobalTensor<half> in0, in1, out0;
    in0.SetGlobalBuffer((__gm__ half*)input0, t.totalSize);
    out0.SetGlobalBuffer((__gm__ half*)output0, t.totalSize);
    
    // 3. Pipe + Queue 初始化
    AscendC::TPipe pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 2> que0;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> que1;
    pipe.InitBuffer(que0, 2, t.tileSize * sizeof(half));
    pipe.InitBuffer(que1, 1, t.tileSize * sizeof(half));
    
    // 4. 主循环 (双缓冲Pipeline)
    for (int64_t i = 0; i < t.tileCount; i++) {
        // Load: HBM → UB
        AscendC::LocalTensor<half> local = que0.AllocTensor<half>();
        AscendC::DataCopy(local, in0[i * t.tileSize], t.tileSize);
        que0.EnQue(local);
        que0.DeQue<half>();
        
        // Compute: Add + Mul + Relu (全部在 UB 上原地完成)
        AscendC::Add(local, local, local, t.tileSize);   // 算子1
        AscendC::Mul(local, local, local, t.tileSize);   // 算子2
        AscendC::Relu(local, local, t.tileSize);          // 算子3
        
        // Store: UB → HBM
        AscendC::DataCopy(out0[i * t.tileSize], local, t.tileSize);
        que0.FreeTensor(local);
    }
    
    AscendC::PipeBarrier<AscendC::PIPE_ALL>();
}
```

### 步骤7: 编译链接

**文件**: `compiler/python/ascendc_compile.py`, `asc_codegen_compile.py`

```
编译管线:
  1. 生成 kernel.cpp (Ascend C 源码)
  2. 生成 tiling.cpp (主机端 Tiling)
  3. bisheng kernel.cpp → kernel.o  (--npu-arch=ascend950)
  4. g++ tiling.cpp → tiling.o      (链接 libtiling_api)
  5. g++ kernel.o + tiling.o → kernel.so
     (链接 -lascendcl -lruntime -ltiling_api)
  6. 可选: TBE 二次编译 compile_op() → .o + .json
  7. 可选: PGO 反馈优化 (多轮编译, 自动选择最佳 Tiling)
```

---

## 四、模板选择原则

### 4.1 Cube (MatMul/Conv) 融合模式选择

```
决策树:

是否包含 Cube 算子 (MatMul/Conv)?
  ├─ 否 → Vector-only 模式 (仅使用 Vector Core)
  │        使用标准 Elewise 调度
  │
  └─ 是 → Cube+Vector 融合 (CV Fusion)
         后续是否有 ElementWise 算子?
           ├─ 否 → 标准 Cube 模板 (kCommon)
           │        Cube 输出 → Fixpipe → HBM
           │
           └─ 是 → 融合模式判断
                   后续 Vetor Op 是否能直接操作 Cube 输出格式?
                    ├─ 是 → Fixpip 快速模式 (kFixpip)
                    │        MatMul → Fixpipe(量化ReLU) → HBM
                    │        最快, 但仅支持特定激活函数
                    │
                    └─ 否 → UB 融合模式 (kUBFuse)
                             Cube 输出保留在 UB
                             → Vector Op 直接读取 UB 数据
                             → 无需写回 HBM
```

### 4.2 Reduce 模板选择

```
输入规模判断:
  ├─ 小规模 (全部数据可放入 UB)
  │   → kAllLoad: 一次性全量加载, 内存复用最小化
  │
  └─ 大规模 (需要分块)
      └─ 是否需要多核 Reduce?
           ├─ 否 → kCommon: 标准 Phase1/Phase2 Reduce
           └─ 是 → kRCore: R轴分裂 + 跨核归约
                    Phase1: 每个 core 对局部数据 Reduce
                    Phase2: 所有 core 结果再次 Reduce
```

### 4.3 双缓冲策略选择

```
是否启用双缓冲 (double_buffer)?
  ├─ 条件1: UB 大小足够容纳 2 份数据
  ├─ 条件2: 计算量足够大，DataCopy 可被计算掩盖
  └─ 条件3: 没有跨 Tile 的数据依赖
```

双缓冲原理:
```
无缓冲:  Load → Compute → Store → Load → Compute → Store
双缓冲:  Load0 → Compute0 + Load1 → Store0 + Load2 → Compute1 + Store1 ...
                            ↑ 计算和数据搬运重叠执行
```

### 4.4 NDDMA 打分函数 (动态 Shape 场景)

当输入 Shape 在编译期未知时，生成多套 Tiling 方案，运行时按 Score 选择:

```cpp
double ScoreFunc(const Shape& actual) {
    // 数据复用率 = 计算量 / 数据搬运量
    // 内存利用率 = UB使用量 / 256KB
    // 并行度 = 实际使用 Core 数 / 可用 Core 数
    
    double reuse = actual.computeOps / actual.memBytes;
    double memEff = ubUsed / UB_SIZE;
    double parEff = actualCores / totalCores;
    
    return reuse * 0.5 + memEff * 0.3 + parEff * 0.2;
}
```

---

## 五、SuperKernel — 全图融合

### 5.1 与 Autofuse 的区别

```
                Autofuse                    SuperKernel
                ────────                    ──────────
作用层:         编译期 CodeGen              运行时 Task 调度
融合方式:       合并算子代码为单一核函数      合并多个核函数为一个 TaskQue
优化目标:       消除 HBM 数据搬运            消除 Kernel Launch 调度开销
输入:           AscGraph (算子图)           已编译的多个算子的二进制 + 模型RI
输出:           单个融合 .so                 TaskQue + sk_entry 调度器
适用场景:       Vector-only + Cube+Vector   全图任意算子 (含自定义算子)
限制:           需要源码级融合               算子必须是已编译的 .o
```

### 5.2 核心设计: sk_entry 调度循环

**文件**: `super_kernel/kernel/sk_entry.asc`

```cpp
// 一个统一的调度循环, 按 TaskQue 列表依次执行子 Kernel

__global__ __aicore__ void sk_entry(GM_ADDR args) {
    SkDeviceEntryArgs* devArgs = (SkDeviceEntryArgs*)args;
    TaskInfo* taskQue = devArgs->taskQue;
    
    // 为当前 core 选择函数副本 (缓解 L2 Cache 争用)
    uint32_t coreId = get_coreid();
    uint32_t funcIdx = coreId % entryCnt;
    
    for (uint32_t i = 0; i < devArgs->totalTask; i++) {
        TaskInfo* task = &taskQue[i];
        
        switch (task->type) {
        case TYPE_PRELOAD:
            dc_preload(task->preloadAddr, task->preloadSize);
            break;
            
        case TYPE_FUNC: {
            // 函数指针调用 (间接, 非内联)
            typedef void (*KernelFunc)(GM_ADDR param);
            KernelFunc f = (KernelFunc)(task->entry[funcIdx]);
            f(task->params);
            break;
        }
        
        case TYPE_SYNC:
            SetFlag(task->syncId);
            WaitFlag(task->syncId);
            break;
            
        case TYPE_EVENT_NOTIFY:
            *((volatile uint64_t*)task->eventAddr) = NOTIFY_VALUE;
            break;
            
        case TYPE_EVENT_WAIT:
            while (*((volatile uint64_t*)task->eventAddr) != EXPECT_VALUE);
            break;
        }
    }
}
```

### 5.3 子 Kernel 拆分的缓存优化

```
core0: 使用 entry[0] → 访问物理地址 A
core1: 使用 entry[1] → 访问物理地址 B
core2: 使用 entry[2] → 访问物理地址 C
core3: 使用 entry[3] → 访问物理地址 D

效果: 不同 core 访问不同物理 L2 Cache Line
      → 避免多核串行化争用 → 提升 L2 命中率
```

### 5.4 ICache Preload

在执行当前子 Kernel 时，预先将下一个子 Kernel 的指令加载到 ICache:

```
Timeline:
  [执行 Kernel_N]  → dc_preload(Kernel_N+1 的代码段)
  [执行 Kernel_N+1] → ICache 命中 (已在执行 Kernel_N 时预取完成)
                     → dc_preload(Kernel_N+2 的代码段)
  ...

收益: 消除 ICache Miss 导致的取指延迟 (约 10-30 cycles)
```

### 5.5 Early-Start 优化

经典场景: 前序算子的末尾是 `DataCopy` (MTE 搬运)，后续算子的开头是标量初始化（不依赖输入数据）。两者可并发:

```
操作序列:                    优化后: (Early-Start)
┌──────────────┐           ┌──────────────┐
│ Kernel_1      │           │ Kernel_1      │
│   ...         │           │   ...         │
│   SetFlag     │──────┐    │   SetFlag     │──────┐
│   DataCopy    │      │    │   DataCopy    │      │  ← MTE 和 Scalar 并行
│               │      │    ├──────────────┤      │
├──────────────┤      │    │ Kernel_2      │      │
│ Kernel_2      │      └───→│   InitScalar  │←─────┘
│   WaitFlag    │           │   WaitFlag    │
│   InitScalar  │           │   Compute...  │
│   Compute...  │           └──────────────┘
└──────────────┘
```

通过 `CrossCoreSetFlag` + `CrossCoreWaitFlag` 实现细粒度同步，让 MTE 搬运和标量初始化并发。

### 5.6 Scope 管理

Scope 是用 `aclskScopeBegin/End` 标记的融合范围:

```
Scope 分割 Pass 管线:
  1. InitialScopeSplitPass    → 按 scopeBitFlags 和可融合性分割
  2. DeadlockRefinePass       → 死锁检测，在 Wait 处切开 (防跨 Scope Wait)
  3. ScheModeKernelSplitPass  → 按 ScheMode kernel 趋势分割
  4. EventOnlyStreamRemovePass → 移除仅有事件节点的流
```

每个 Scope 生成独立的 TaskQue，SuperKernel 调度器按 Scope 顺序执行。

---

## 六、融合的关键收益量化

| 指标 | 无融合 | Autofuse | SuperKernel |
|------|--------|----------|-------------|
| HBM 读写次数 | N×2 (N个算子) | 1×2 (1个融合核) | M×2 (M个Scope) |
| Runtime Launch次数 | N (每个算子1次) | 1 次 | 1 次 (所有Scope) |
| ICache Miss | 每次 Launch 冷启动 | 1 次 | 0 (preload) |
| 中间结果占用 HBM | N-1 个 | 0 | scope 间需要 |
| UB 内存利用率 | 每个算子独立分配 | 生命周期复用 | scope 内复用 |

**核心结论**: Autofuse 解决 **Memory-Bound** 问题 (减少 HBM 搬运)，SuperKernel 解决 **Launch-Bound** 问题 (减少调度开销)。两者结合可达成接近硬件理论峰值的性能。

---

## 七、代码结构索引

```
graph-autofusion/
├── autofuse/
│   ├── ascir/                    ← Ascend IR 定义 (HintGraph/AscGraph)
│   ├── ascendc/api/              ← AscendC API 封装 (给CodeGen用)
│   ├── codegen/                  ← 代码生成核心
│   │   ├── codegen.cpp/h         ← 主入口: GenerateKernel/GenerateTiling
│   │   ├── kernel_codegen.cpp    ← 核函数代码生成
│   │   ├── tiling_codegen.cpp    ← Tiling 代码生成
│   │   ├── expression_convert.cpp ← 表达式 → Tiler 转换
│   │   └── graph_checking.cpp    ← 图合法性检查
│   ├── optimize/                 ← 优化与调度
│   │   ├── optimize.cpp          ← 主优化入口
│   │   ├── autoschedule.cpp      ← 自动 Tiling 调度
│   │   ├── buf_que_allocator.cpp ← Buffer/Queue 分配
│   │   ├── schedule_task_generator.cpp ← 图分区
│   │   ├── fused_graph.cpp       ← 融合图管理 (轴合并等)
│   │   └── pre_process.cpp       ← 预处理
│   ├── compiler/python/          ← Python 接口
│   │   ├── ascir_api.py          ← AscendIR 构造 API
│   │   ├── pyautofuse.cpp        ← Python→C++ 绑定
│   │   ├── asc_codegen_compile.py ← 编译管线
│   │   └── ascendc_compile.py    ← bisheng 编译器驱动
│   └── common/                   ← 公共工具
│       ├── code_printer.cpp      ← 代码打印/缩进
│       ├── expression_struct.h    ← 表达式 AST
│       └── schedule_utils.cpp    ← 调度工具函数
├── super_kernel/                 ← SuperKernel 全图融合
│   ├── kernel/
│   │   ├── sk_entry.asc          ← 统一调度循环
│   │   └── sk_scope_kernel.asc   ← Scope 标记 Kernel
│   ├── src/aot/
│   │   ├── sk_scope_launch.cpp    ← Scope 启动
│   │   ├── sk_scope_split.cpp    ← Scope 分割
│   │   ├── sk_optimizer.cpp      ← 图优化
│   │   ├── sk_task_builder.cpp   ← TaskQue 构建
│   │   └── sk_candidate_heap.cpp ← 融合候选选择
│   └── src/jit/superkernel/
│       └── super_kernel.py       ← JIT 编译入口
└── ...
```
