# TensorFlow → AscendC 映射层次结构与原理

## 一、整体架构总览

```
┌──────────────────────────────────────────────────────────────────────┐
│ 用户层                                                                │
│   TensorFlow Python 脚本 (tf.nn.conv2d, tf.matmul, tf.add, ...)      │
├──────────────────────────────────────────────────────────────────────┤
│ TF Adapter 适配层                                                     │
│   tf_adapter/   tf_adapter_2.x/   convert_tf2npu/                    │
├──────────────────────────────────────────────────────────────────────┤
│ GE (Graph Engine) 图引擎层                                            │
│   Parser → Compiler → Optimizer → Partition → Build → Executor       │
├──────────────────────────────────────────────────────────────────────┤
│ 算子库层 (Operator Library)                                          │
│   ops-math / ops-nn / ops-transformer / ops-cv                       │
│   ┌──────────────┬──────────────────┬─────────────────┐             │
│   │ op_graph     │ op_host          │ op_kernel       │ op_api      │
│   │ (图集成)     │ (Tiling/InferShape│ (AscendC核函数) │ (aclnn API) │
│   └──────────────┴──────────────────┴─────────────────┘             │
├──────────────────────────────────────────────────────────────────────┤
│ Ascend C 核函数编程层                                                 │
│   asc-devkit (C API / Base API / Advanced API / SIMT API)            │
│   atvoss (向量算子模板库)                                             │
├──────────────────────────────────────────────────────────────────────┤
│ metadef 元数据基础层                                                  │
│   Tensor / Shape / DataType / Format / OpDesc / ComputeGraph         │
├──────────────────────────────────────────────────────────────────────┤
│ Runtime 运行时层                                                      │
│   Device / Stream / Event / Memory / Task / Kernel / Profile         │
├──────────────────────────────────────────────────────────────────────┤
│ 昇腾 NPU 硬件 (AI Core / Vector Core / AI CPU / DVPP / HCCL)         │
└──────────────────────────────────────────────────────────────────────┘
```

## 二、层次结构详解

### 第 0 层: TensorFlow 用户代码

用户在 Python 中编写 TF 计算图:

```python
import tensorflow as tf
x = tf.placeholder(tf.float32, [None, 224, 224, 3])
w = tf.Variable(tf.truncated_normal([3, 3, 3, 64]))
conv = tf.nn.conv2d(x, w, strides=[1, 1, 1, 1], padding='SAME')
relu = tf.nn.relu(conv)
pool = tf.nn.max_pool(relu, [1, 2, 2, 1], [1, 2, 2, 1], padding='SAME')
fc = tf.layers.dense(tf.layers.flatten(pool), 1000)
```

### 第 1 层: TF Adapter — GraphDef → Ascend 桥接

**核心组件**: `GeOp` (AsyncOpKernel, 位于 `tf_adapter/kernels/geop_npu.cc`)

```
ComputeAsync()
  │
  ├─ BuildInputTensorInfo()    ← TF Tensor → ge::Tensor 转换
  │
  ├─ BuildGraphDef()           ← 获取 FunctionDef, 推断Shape, 序列化GraphDef
  │     ├─ InferShapeUtil::InferShape()
  │     └─ ProcessForDiffNodeTypes() → 遍历节点, 添加描述符
  │
  ├─ CompileGraph()            ← 调用 GE Session API
  │     ├─ DoGraphParser()     ← GraphDef → ComputeGraph (AscendIR)
  │     ├─ AddGraph()          ← 注册图到 GE Session
  │     └─ BuildGraph()        ← 触发编译 (Tiling + Kernel 编译)
  │
  ├─ RunGraph()                ← ge_session_->RunGraphAsync()
  │     └─ BuildOutputTensorInfo()  ← 输出 GE Tensor → TF Tensor
  │
  └─ 回调 → TF OpKernelContext::done()
```

**关键映射文件**:

| 组件 | 文件 | 功能 |
|------|------|------|
| GeOp 内核 | `tf_adapter/kernels/geop_npu.cc:1383` | TF 算子入口, 驱动图编译与执行 |
| NPU Op 注册 | `tf_adapter/ops/npu_ops.cc` | 注册 TF NPU 相关算子 |
| 代码转换工具 | `convert_tf2npu/main.py` | AST 转换将 TF API 映射到 NPU API |
| API 映射表 | `convert_tf2npu/mappings/ast.json` | 62 项 TF→NPU API 映射 |

### 第 2 层: GE Parser — GraphDef → AscendIR

**核心组件**: `ge/parser/parser/tensorflow/tensorflow_parser.cc`

```
aclgrphParseTensorFlow(protobuf)
       │
       ▼
tensorflow_parser.cc 入口函数
       │
       ├─ 遍历 GraphDef 中所有 NodeDef
       ├─ OpParserFactory 分派到对应算子解析器
       │     ├─ tensorflow_constant_parser.cc   (Const)
       │     ├─ tensorflow_data_parser.cc       (Data)
       │     ├─ tensorflow_fusion_op_parser.cc  (融合算子)
       │     └─ DOMI_REGISTER_TENSORFLOW_PARSER 注册的自定义解析器
       │
       ├─ OperatorAutoMapping() 自动属性映射
       │     NodeDef.attr → Operator.attr
       │
       └─ 输出: ge::ComputeGraph (AscendIR)
```

**AscendIR 数据结构**:

```
ComputeGraph
  └─ Node[]
       ├─ OpDesc (算子描述: 名称, 类型, 输入输出张量)
       │     └─ GeTensorDesc[] (张量描述: Shape, DataType, Format)
       ├─ DataAnchor (数据连接: src_node:output_idx → dst_node:input_idx)
       └─ CtrlAnchor (控制依赖)
```

### 第 3 层: GE Compiler — 图编译优化

```
CompilerStages (四阶段编译管线)

阶段1: GraphPrepare (预处理)
  ├─ 图规范化, Shape 推理, 量化准备, 常量准备
  └─ 输入: ComputeGraph → 输出: 规范化 ComputeGraph

阶段2: GraphOptimize (图优化, 三批次)
  ├─ Stage1 (分区前): 常量折叠, CSE, DCE, 控制流转换
  ├─ Stage2 (分区后): EnginePartitioner 拆分 → 各引擎并行优化子图
  └─ Stage3 (合并后): 内存冲突处理, 流标签分配, BufferPool 优化

阶段3: EnginePartitioner (引擎分区)
  ┌──────────┬─────────────────────────────┐
  │ AI Core  │ MatMul, Conv, Softmax       │
  │ VectorEngine │ ElementWise, Activate   │
  │ HostCpu  │ Data, Reshape, Format       │
  │ HCCL     │ AllReduce, Broadcast        │
  │ DVPP     │ 图像/视频预处理               │
  │ RTS      │ StreamSwitch, StreamActive  │
  └──────────┴─────────────────────────────┘

阶段4: GraphBuilder (构建)
  ├─ StreamAllocator    (流分配 + 同步节点插入)
  ├─ GraphMemoryAssigner (块级内存复用)
  ├─ TaskGenerator      (生成 TaskDef 序列)
  └─ 产出: GeRootModel (含 Task 序列 + 权重 + kernel二进制)
```

### 第 4 层: Operator Library — 算子实现

每个算子在 `ops-math/` 中的标准结构 (`ops-math/math/add/` 为例):

```
math/add/
├── op_graph/               ← GE 图级别的集成
│   ├── add_prototype.cpp   ← 算子原型定义
│   ├── add_infershape.cpp  ← Shape 推断
│   └── add_fusion_pass.cpp ← 融合优化 pass
├── op_host/                ← 主机端计算
│   ├── add_def.cpp         ← 算子定义 (Input/Output/DataType/Format)
│   ├── add_infershape.cpp  ← InferShape 实现
│   └── arch35/add_tiling_arch35.cpp  ← Tiling 计算
├── op_kernel/              ← 设备端核函数 (Ascend C)
│   └── arch35/
│       ├── add.cpp         ← __global__ __aicore__ 核函数入口
│       ├── add_dag.h       ← 计算 DAG 定义
│       └── add_struct.h    ← 模板参数结构
└── op_api/                 ← aclnn API (单算子调用接口)
    ├── add.cpp             ← L0 内部 API (AiCore/AiCpu 分发)
    └── aclnn_add.cpp       ← aclnn 公共 API
```

**算子层次对应**:

| 算子层 | 职责 | 语言 | 运行位置 |
|--------|------|------|----------|
| `op_graph` | GE 图集成, InferShape, 融合pass | C++ | Host |
| `op_host` | 算子注册, Tiling 计算 | C++ | Host |
| `op_kernel` | Ascend C 核函数 (计算逻辑) | C++ (Ascend C) | AI Core |
| `op_api` | aclnn 标准 API 封装 | C++ | Host |

### 第 5 层: Ascend C — 核函数编程框架

**位于 `asc-devkit`**, 提供四个编程抽象层次:

```
───────────────────────────────────────────────────────────
  High-level API (高级API)
  │ 预构建算法: MatMul, Softmax 等
  │ 简化调用, 自动内存/同步管理
  ├──────────────────────────────────────────────────────
  Tpipe/Tque Framework API (高级C++ API)
  │ Tensor 抽象 + Queue 自动内存/同步管理
  │ 声明式编程: pipe.InitBuffer() → AllocTensor() → EnQue() → DeQue()
  ├──────────────────────────────────────────────────────
  Base API (基础C++ API)
  │ Tensor 抽象: GlobalTensor, LocalTensor
  │ 手动内存管理: MakeTensor, LocalMemoryAllocator
  │ 手动同步: SetFlag/WaitFlag, PipeBarrier
  ├──────────────────────────────────────────────────────
  C API (基础C API)
  │ 纯 C 指针, 数组分配
  │ asc_add() 等 snake_case 函数
  └──────────────────────────────────────────────────────
```

**示例: 一个基础 Ascend C 核函数** (`ops-math/math/add/op_kernel/arch35/add.cpp`):

```cpp
#include "kernel_operator.h"  // Ascend C 头文件
#include "add_dag.h"

template <uint64_t schMode>
__global__ __aicore__ void add(
    GM_ADDR x1, GM_ADDR x2, GM_ADDR y,
    GM_ADDR workspace, GM_ADDR tiling)
{
    BroadcastSch<schMode, OpDag> sch(tiling);
    sch.Process(x1, x2, y);  // 自动执行 CopyIn → Add → CopyOut
}
```

**DAG 定义** (`add_dag.h`):

```cpp
template <typename T>
struct AddWithoutCastCompute {
    using OpInputX1 = Bind<Vec::CopyInBrc<T>, Placeholder::In0<T>>;
    using OpInputX2 = Bind<Vec::CopyInBrc<T>, Placeholder::In1<T>>;
    using OpAddRes  = Bind<Vec::Add<T>, OpInputX1, OpInputX2>;
    using OpCopyOut = Bind<Vec::CopyOut<T>, Placeholder::Out0<T>, OpAddRes>;
    using Outputs   = Elems<OpCopyOut>;
    using MemCfg    = MemOptCfg<MemLevel::LEVEL_2>;
    using OpDag     = DAGSch<Outputs, void, MemCfg>;
};
```

### 第 6 层: metadef — 元数据基础

所有上层组件的共享类型系统:

| 核心类型 | 说明 |
|----------|------|
| `ge::Tensor` / `GeTensorDesc` | 张量描述: Shape + DataType + Format |
| `ge::Shape` | 维度信息, 支持动态维度 |
| `ge::DataType` | DT_FLOAT, DT_INT32, DT_FLOAT16, DT_BF16, ... |
| `ge::Format` | FORMAT_ND, FORMAT_NCHW, FORMAT_NHWC, FORMAT_FRACTAL_Z, ... |
| `ge::ComputeGraph` | 计算图: 节点 DAG |
| `ge::Node` | 图节点: 包含 OpDesc + Input/Output Anchors |
| `ge::OpDesc` | 算子描述符: inputs/outputs/attrs |
| `REG_OP` 宏 | 算子注册: REG_OP(Add).Input("x1")... |
| `gert::TilingContext` | Tiling 上下文: shape/dtype/workspace |

### 第 7 层: Runtime — 硬件交互

**ACL Runtime API** (`runtime/include/external/acl/`):

| API 模块 | 功能 |
|----------|------|
| `acl_rt.h` | 设备管理 (SetDevice/ResetDevice), Stream, Event, 内存 (Malloc/Free/Memcpy) |
| `acl_mdl.h` | 模型加载/执行 |
| `acl_op.h` | 单算子编译/执行 |
| `acl_tdt.h` | 数据传输 |

**GE Runtime 执行模式**:

| 模式 | v1 (静态) | v2 (动态) |
|------|-----------|-----------|
| 执行方式 | TaskSink 模式 → `rtModelExecute` | Host 顺序遍历 ExecuteGraph |
| Shape 支持 | 静态 | 动态 |
| 核心类 | `DavinciModel` | `ModelV2Executor` |
| 执行器 | 硬件 Sink | `SequentialExecutor` (纯C) |

## 三、三种执行路径

### 路径 A: 图模式 (TF Graph → GE → Runtime) — 最常用

```
TF GraphDef
  → GeOp::ComputeAsync()
    → BuildGraphDef() → 序列化 GraphDef
    → DoGraphParser() → TensorFlow Parser → ComputeGraph (AscendIR)
    → AddGraph() → BuildGraph() → Compile → OM 模型
    → RunGraph() → GE Executor → Runtime → AI Core 执行
```

### 路径 B: 单算子模式 (TF op → aclnn API)

```
TF op → aclnn 直接调用
  → op_api/aclnn_add.cpp (创建 OpExecutor, 构建计算图)
  → op_host/ (Tiling 计算)
  → op_kernel/ (Ascend C 核函数)
  → Runtime → AI Core
```

### 路径 C: 离线编译 (ATC)

```
TF GraphDef (.pb) → atc 工具
  → Parser → Compiler → OM 文件
  → 运行时 LoadModel → Execute
```

## 四、Graph-Autofusion: 自动融合深度分析

> **graph-autofusion** 是 AscendC 框架的**核心性能优化组件**。
> 通过对图的自动分析，将多个用户级算子融合为单个 Ascend C 核函数，
> 在 device 端同一块物理内存 (UB) 上连续计算，彻底消除中间数据的 HBM 搬运开销。
> 
> **详见**: [graph-autofusion-analysis.md](graph-autofusion-analysis.md) — 自动融合原理、输入输出、7步管线、模板选择原则、SuperKernel
> 
> **[lowering-schedule-codegen-pipeline.md](lowering-schedule-codegen-pipeline.md) — Lowering / Symbolization / Schedule / CodeGen / ATT 五模块完整流水线**

### 4.1 Autofuse 核心流程图

```
完整算子图 ──→ [算子分类] ──→ [依赖分析] ──→ [图分区] ──→ [自动调度] ──→ [代码生成] ──→ [编译链接]
                    │              │            │             │               │              │
                    ▼              ▼            ▼             ▼               ▼              ▼
             按硬件单元分组    识别融合链    ScheduleGroup  最优Tiling方案   Ascend C源码    .so+.json
             (Vector/Cube)  (无Data切断)   (同类型可融合)  (轴划分+双缓冲)  (ApiCall序列)   (可部署)
```

### 4.2 Autofuse 融合原理: 消除数据搬运

```
融合前 (逐算子执行, 每个算子都需要 HBM↔UB):
  算子1: Load(x1)→Add(x1,x2)→Store(y)   ← 2次HBM读 + 1次HBM写
  算子2: Load(y)→Mul(y,s)→Store(z)       ← 1次HBM读 + 1次HBM写
  算子3: Load(z)→Relu(z)→Store(out)      ← 1次HBM读 + 1次HBM写
  总计:  4次HBM读 + 3次HBM写 = 7次总线事务

融合后 (三个算子在UB上连续计算):
  融合核: Load(x1,x2)→Add→Mul→Relu→Store(out)  ← 2次HBM读 + 1次HBM写 = 3次总线事务
  消除:  4 次 HBM 传输 (57% 减少)
```

### 4.3 CodeGen 代码生成实现

图节点通过 **ApiCall 工厂** 映射到 Ascend C API:

```
AscendIR 节点      →    ApiCall 子类        →    生成的 Ascend C 代码
────────────────────────────────────────────────────────────────
Add               →    BinaryApiCall       →    Add(z, x, y, size)
Mul               →    BinaryApiCall       →    Mul(z, x, y, size)
Relu              →    UnaryApiCall        →    Relu(y, x, size)
ReduceSum         →    ReduceApiCall       →    ReduceSum(y, x, axis, size)
MatMul            →    (Cube调度器)         →    Mmad/Hadamard/Fixpipe
Broadcast         →    BroadcastApiCall    →    Broadcast(y, x, size)
Cast              →    CastApiCall         →    Cast(y, x, size)
```

每个 ApiCall 生成三阶段代码: `PreProcess(等待输入→分配输出) → ApiCall(计算指令) → PostProcess(标记就绪→释放输入)`

### 4.4 模板选择原则

| 决策维度 | 选项 | 选择条件 |
|----------|------|----------|
| **执行单元** | Vector-only / Cube+Vector | 是否包含 MatMul/Conv |
| **Cube融合模式** | kFixpip (快速) / kUBFuse (UB复用) / kCommon (标准) | 后续能否直接操作Cube输出格式 |
| **Reduce模式** | kAllLoad / kCommon / kRCore | 数据规模 + 是否需要多核归约 |
| **双缓冲** | 启用 / 不启用 | UB容量≥2份数据 + 计算可掩盖搬运 |
| **NDDMA打分** | 运行时选最优Tiling | 动态Shape场景 |

### 4.5 SuperKernel: 全图融合 (运行时级)

与 Autofuse (编译期源码融合) 不同，SuperKernel 在运行时将所有算子的**已编译二进制**合并:
- 统一调度循环 `sk_entry.asc` → TaskQue (函数指针表) → 消除 N 次 runtime launch 开销
- 子 Kernel 拆分: 同一代码 4 份副本，多核访问不同物理地址 → 缓解 L2 Cache 争用
- ICache Preload: `dc_preload()` 预取下一算子的指令
- Early-Start: MTE 搬运与标量初始化并发执行

Autofuse 解决 **Memory-Bound** → SuperKernel 解决 **Launch-Bound**，两者结合可达理论峰值。

---

## 五、关键优化技术

| 优化技术 | 实现位置 | 说明 |
|----------|----------|------|
| CSE (公共子表达式消除) | `ge/compiler/` | 消除重复计算 |
| 常量折叠 | `ge/compiler/` | 编译期计算常量表达式 |
| DCE (死代码消除) | `ge/compiler/` | 删除无用的节点 |
| 手写 Pattern 融合 | `ge/compiler/` FusionPassExecutor | 识别固定算子组合并融合 |
| **Autofuse (自动融合)** | **`graph-autofusion/autofuse/codegen/`** | **算子分类 + CodeGen 合成, 消除HBM搬运** |
| **SuperKernel (全图融合)** | **`graph-autofusion/super_kernel/`** | **TaskQue调度, 消除Launch开销** |
| 算子自动调度 | `graph-autofusion/autofuse/optimize/` | 自动 Tiling + Buffer 分配 + 并行化 |
| 表达式模板 (零开销) | `atvoss/expression/` | 编译期展开, 无虚函数开销 |
| PGO (Profile-Guided) | `graph-autofusion/autofuse/compiler/python/` | 运行时反馈优化 Tiling |
| ICache Preload | `graph-autofusion/super_kernel/` | 预取指令, 减少取指延迟 |
| Early-Start | `graph-autofusion/super_kernel/` | MTE+Scalar 并发 |
| 内存复用 | `graph-autofusion/autofuse/optimize/buf_que_allocator.cpp` | 生命周期复用UB |
| 流并行 | `ge/compiler/` StreamAllocator | 多引擎并行执行 |

> **完整的自动融合原理、CodeGen实现细节、模板选择原则、SuperKernel设计等深度分析**，见 [graph-autofusion-analysis.md](graph-autofusion-analysis.md)。

## 六、中间组件示例

详见 `examples/` 目录下的代码示例。
