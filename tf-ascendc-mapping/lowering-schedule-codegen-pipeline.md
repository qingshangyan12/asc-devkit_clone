# Lowering → Symbolization → Schedule → CodeGen → ATT 完整流水线

> 本文详解 **graph-autofusion** 中五个核心模块如何在 AscendC 框架上将原生 PyTorch/TensorFlow 代码最终运行在昇腾 NPU 硬件上。

---

## 一、在已有层次架构中的定位

之前定义了 7 层架构，这五个模块集中在 **第3层 (GE Compiler)** 与 **第5层 (AscendC 核函数)** 之间:

```
┌───────────────────────────────────────────────────────────────┐
│ 第1层: Framework Adapter                                       │
│   TorchAir / TF Adapter → 生成 ComputeGraph (FusedGraph)      │
├───────────────────────────────────────────────────────────────┤
│ 第2层: GE Parser                                              │
│   TorchAir/AtenIR → ComputeGraph (含 AscBackend 子图)         │
├───────────────────────────────────────────────────────────────┤
│ ┌─ graph-autofusion ────────────────────────────────────────┐ │
│ │                                                           │ │
│ │  ① Lowering (IR 降低/映射)                                │ │
│ │    torch FX Graph → AscGraph → API 映射表 → AscendC API   │ │
│ │                                                           │ │
│ │  ② Symbolization (符号化)                                  │ │
│ │    动态维度用 Symbolic Expression 追踪                     │ │
│ │    s0 → actual_shape → TilingData 字段                    │ │
│ │                                                           │ │
│ │  ③ Schedule (调度)                                        │ │
│ │    轴分类 → TilingCase枚举 → TileSplit + BlockSplit       │ │
│ │                                                           │ │
│ │  ④ CodeGen (代码生成)                                      │ │
│ │    ImplGraph → ApiCall工厂 → Loop树 → AscendC 源码        │ │
│ │                                                           │ │
│ │  ⑤ ATT (Automatic Tiling Template — 模板选择)            │ │
│ │    生成 host 端 Tiling 函数: 分块大小/步长计算            │ │
│ │                                                           │ │
│ │  ⑥ 编译: bisheng → .o → .so + .json                      │ │
│ └───────────────────────────────────────────────────────────┘ │
├───────────────────────────────────────────────────────────────┤
│ 第5层: AscendC 核函数编程 (asc-devkit)                        │
│ 第7层: Runtime → ACL API → NPU 硬件                          │
└───────────────────────────────────────────────────────────────┘
```

---

## 二、五个模块的角色与关系

```
PyTorch FX Graph (torch.compile 产物)
    │
    ▼
┌──────────────────────────────────────────────────────┐
│ TorchAir: AtenIR → AscendIR (FusedGraph)              │  ← 第1-2层完成
│  每个子图作为 "AscBackend" 节点嵌入 FusedGraph        │
└──────────────────────────────────────────────────────┘
    │
    ▼  ← 自此进入 graph-autofusion
┌──────────────────────────────────────────────────────┐
│ ② Symbolization (符号化)                              │
│   输入: FusedGraph/AscGraph (可能含动态维度)           │
│   作用: 追踪动态尺寸 → af::Expression 树               │
│   结构: SizeExpr = Expression { Symbol/Const/Expr }   │
│   输出: 带 Symbolic Size 的有标注 AscGraph             │
│                                                        │
│   示例:                                                │
│     batch_size 未知 → s0                              │
│     output_size = s0 * 512                            │
│     生成 TilingData 时:                               │
│       t.actualSize[i] = min(tileSize, total - offset) │
└──────────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────────┐
│ ③ Schedule (调度)                                     │
│   输入: 带符号的有标注 AscGraph                        │
│   作用:                                               │
│     a) 轴分类: X(非缩减)/Y(UB拆分)/R(缩减)/N(块)     │
│     b) TilingCase 枚举: 穷举所有轴划分组合            │
│     c) TileSplit: 轴拆分为 TileOuter + TileInner      │
│     d) BlockSplit: 轴拆分为 BlockOuter + BlockInner   │
│     e) Axis合并: 连续轴合并减少循环嵌套                │
│   输出: ScheduledResult (ImplGraph + Tiling元数据)    │
│                                                        │
│   示例:                                                │
│     axis[0]=1024 → TileOuter=4 × TileInner=256        │
│     axis[1]=512  → BlockOuter=8 × BlockInner=64       │
│     ReduceAxis   → RCore: 跨核归约                     │
└──────────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────────┐
│ ⑤ ATT (Automatic Tiling Template — 模板选择)        │
│   输入: FusedScheduledResult                          │
│   作用:                                               │
│     a) 解析 Tiling 空间的 ModelInfo                   │
│     b) 选择 Tiling 策略: AxesReorder / HighPerf       │
│     c) 生成 host 端 Tiling 函数源码 (.cpp)            │
│   输出: Tiling 函数 (.cpp) — 运行在 Host 上           │
│                                                        │
│   这是 "模板选择" 的核心:                              │
│     - AxesReorder: 尝试不同轴排序, 搜索最优分块        │
│     - HighPerf: 预训练模型/查表法快速得到最优配置       │
│     - 生成的函数在推理时执行: 根据实际 shape 计算分块   │
└──────────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────────┐
│ ④ CodeGen (代码生成)                                  │
│   包含 ① Lowering (IR → AscendC API 的映射)           │
│   输入: FusedScheduledResult + Tiling 信息             │
│   作用:                                               │
│     a) ApiCall 工厂: 图节点 → AscendC API 调用        │
│        ┌──────────────┬───────────────────────────┐  │
│        │ ASCIR 节点    │ ApiCall 类 → AscendC API  │  │
│        ├──────────────┼───────────────────────────┤  │
│        │ Add          │ BinaryApiCall → ::Add()   │  │
│        │ Mul          │ BinaryApiCall → ::Mul()   │  │
│        │ Relu         │ UnaryApiCall  → ::Relu()  │  │
│        │ ReduceSum    │ ReduceApiCall → ::Reduce*()│  │
│        │ Load         │ LoadApiCall   → DataCopy  │  │
│        │ Store        │ StoreApiCall  → DataCopy  │  │
│        │ Transpose    │ TransposeApiCall → ::Tran*│  │
│        │ MatMul       │ (Cube调度器) → Mmad/Fixpipe│  │
│        └──────────────┴───────────────────────────┘  │
│     b) Loop树: 构建嵌套循环 (Block→Tile→Inner)       │
│     c) Tiler: 表达式→C++代码转换                      │
│   输出: AscendC kernel 源码 + TilingData 头文件        │
└──────────────────────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────────────────────┐
│ 编译                                                    │
│   bisheng kernel.cpp → kernel.o                        │
│   g++     tiling.cpp → tiling.o                        │
│   链接 → kernel.so + kernel.json                       │
└──────────────────────────────────────────────────────┘
    │
    ▼
Runtime (ACL) → Ascend NPU 硬件执行
```

---

## 三、Pass 系统 — IR 优化的基本单元

在 Schedule 之前，graph-autofusion 通过 **Pass 管线** 对 HintGraph 进行预处理优化。

### 3.0 什么是 Pass

```
Pass = 对 IR 图执行单一变换/优化的基本单元
输入: 一张计算图 (AscGraph 或 ComputeGraph)
输出: 原地修改后的同一张图
```

核心抽象:

```cpp
// autofuse/optimize 中的 Pass 基类
class BaseGraphPass {
    virtual Status Run(af::AscGraph &graph) = 0;  // 接收图, 原地修改
};

// GE Compiler 中的 Pass 基类
template <typename T>
class Pass {
    virtual Status Run(ge::ComputeGraph &graph) = 0;
};
```

### 3.1 Pass 运行器 — 按序执行

```cpp
class BasePassRunner {
    vector<unique_ptr<BaseGraphPass>> passes_;  // Pass 列表

    void RunAll(af::AscGraph &graph) {
        for (auto &pass : passes_) {
            pass->Run(graph);  // 每个 Pass 修改同一张图, 结果传递到下一个
        }
    }
};

// 平台特定实现:
// PassRunnerV1 (ascend950) → 注册 6 个 Pass
// PassRunnerV2 (v35)       → 注册 9 个 Pass
```

### 3.2 具体 Pass 及其优化

| Pass | 作用 | 输入→输出示例 |
|------|------|--------------|
| `PowEquivSubstitutionPass` | 将幂运算替换为等价简单操作 | `Pow(x,2)` → `Mul(x,x)`, `Pow(x,0.5)` → `Sqrt(x)` |
| `ScalarBroadcastOptimizationPass` | 消除冗余标量广播 | 多个相同的 scalar broadcast → 复用同一份 |
| `ScalarTo1DTensorPass` | 标量转为 1D 张量 | `scalar=3.0` → `tensor([3.0])` |
| `BroadcastConstToStorePass` | 常量广播转为存储操作 | 避免无意义的广播计算 |
| `MaskedFillInputReorderPass` | 重排 MaskedFill 参数顺序 | 匹配 Select 算子的输入约定 |
| `CastOptimizationPass` | 消除 Concat 周围的冗余 Cast | `Cast(Concat(...))` → 优化移入 Concat 内部 |
| `ConcatInputUnificationPass` | 统一 Concat 输入对齐 | 插入 Ub2ub Copy 确保 dtype/alignment |
| `ContinuesBroadcastOptimizationPass` | 消除连续 Broadcast 链 | `Brc(Brc(x))` → `Brc(x)` |
| `ExpandDimsForAllReducePass` | 为 AllReduce 扩展维度 | 确保归约前维度匹配 |
| `SplitConcatOptimizationPass` | 融合 Split+Concat 对 | `Concat(Split(x))` → 直接透传 |
| `GatherToLoadPass` | Gather 转 Load | 利用连续内存减少随机访问 |

### 3.3 Pass 在全栈中的三层分布

```
┌─ GE Compiler (Layer 3) ────────────────────────────────────┐
│ 运行在 ge::ComputeGraph 上:                                  │
│   FusionPass (基于Pattern匹配的子图替换)                      │
│     例: ConvBatchnormFusionPass → Conv+Batchnorm → 单一融合算子│
│   DecomposePass (复杂算子分解为硬件原语)                      │
│     例: LayerNorm → ReduceMean + Sub + Square + ...        │
│   QuantProcessPass (量化处理)                                │
│   BufferFusionPass (UB缓冲区融合)                            │
│                                                             │
│  80+ 个命名 Pass, 通过 FusionPassExecutor 管理执行顺序        │
└─────────────────────────────────────────────────────────────┘
    │ 输出: 融合/分解后的 ComputeGraph → 给 graph-autofusion
    ▼
┌─ graph-autofusion (Layer 3.5) ─────────────────────────────┐
│ 运行在 af::AscGraph (HintGraph) 上:                          │
│   GraphPass (预处理优化 Pass)                                 │
│     例: PowEquivSubstitutionPass, CastOptimizationPass      │
│                                                             │
│   目的: 在 Schedule 之前简化图结构, 规范化操作语义            │
│   这样 Schedule 可以更好地枚举 TilingCase                     │
└─────────────────────────────────────────────────────────────┘
    │ 输出: 优化后的 HintGraph → Schedule
    ▼
┌─ Schedule + CodeGen ────────────────────────────────────────┐
│  不在 Pass 框架中, 是独立的调度和代码生成阶段                  │
└─────────────────────────────────────────────────────────────┘
```

### 3.4 Pass 注册机制

```cpp
// GE Compiler 中注册融合 Pass
REG_FUSION_PASS(ConvBatchnormFusion)
    .Stage(kAfterInferShape)
    .CreatePassFn([]() { return make_shared<ConvBatchnormFusionPass>(); });

// graph-autofusion 中注册优化 Pass
// PassRunnerV1::RegisterPasses() 中直接添加:
passes_.push_back(make_unique<PowEquivSubstitutionPass>());
passes_.push_back(make_unique<ScalarBroadcastOptimizationPass>());
// ... (按注册顺序依次执行)

// 也支持外部 .so 插件注册自定义 Pass
REGISTER_CUSTOM_PASS("MyCustomFusion");
```

### 3.5 Pass 与 Schedule 的关系

```
Pass 是 Schedule 的 "预处理工"
─────────────────────────────────────
如果没有 Pass:
  graph 包含 Pow(x,2), Pow(x,0.5), 冗余 Broadcast...
  → Schedule 需要处理更复杂的算子类型
  → TilingCase 枚举空间更大, 很多组合无效
  → 生成的代码效率差

有了 Pass:
  Pow(x,2) → Mul(x,x)   ← 统一为 Binary 操作
  Pow(x,0.5) → Sqrt(x)   ← 统一为 Unary 操作
  冗余 Broadcast 消除    ← 减少不必要的数据搬运
  → Schedule 面对的是规范化的简单算子图
  → 分拆策略更有效, 生成的融合核更紧凑
```

---

## 四、各模块详解

### 4.1 Lowering (IR 降低/映射) — 属于 CodeGen 内部

**在 graph-autofusion 中的特殊含义**: Lowering 不是传统编译器的"语法树→中间码"转换，而是 **ASCIR 图节点 → AscendC API 调用** 的**语义映射**。

**两层 Lowering**:

| 阶段 | 输入 | 输出 | 负责组件 |
|------|------|------|----------|
| **上段 Lowering** | Torch FX Graph | ComputeGraph/AscGraph | TorchAir (ge/dflow/python/.../torch_plugin.py) |
| **下段 Lowering** | ImplGraph (调度后) | AscendC 源码 | CodeGen ApiCall 工厂 (codegen/api_call/) |

**下段 Lowering 的实现机制** (`codegen/api_call/`):

```
调度后的 ImplGraph 节点 (AscNode):
  type    = kComputeElementwise
  op_name = "Add"
  inputs  = [x1_ub, x2_ub]
  output  = [y_ub]
  axis    = [BlockOuter: idx=0, TileOuter: idx=0, TileInner: idx=0]

    ↓ Lowering (通过 ApiCall 工厂)

生成的 AscendC 代码:
  SetFlag<HardEvent::MTE2_MTE1>(EVENT_0);
  WaitFlag<HardEvent::MTE2_MTE1>(EVENT_0);
  LocalTensor<float> y_ub = ub_alloc.Alloc<float>(tile_inner_size);
  AscendC::Add(y_ub, x1_ub, x2_ub, tile_inner_repeat);
  SetFlag<HardEvent::V>(EVENT_1);
  WaitFlag<HardEvent::V>(EVENT_1);
```

**ApiCall 工厂注册表** (`codegen_kernel.cpp:242`):

```cpp
// 当遍历每个 ImplGraph 节点时, 通过 CreateApiCallObject() 查询注册表
unique_ptr<ApiCall> CreateApiCallObject(const NodeInfo& node) {
    auto& reg = GetApiCallRegistry(); // 从 v1_ascir_codegen_impl.h 加载
    auto api_class = reg.GetApiCallName(node.op_type);
    // 例如: node.op_type = "Add" → api_class = "BinaryApiCall"
    return ApiCallFactory::Create(api_class, node);
}
```

### 4.2 Symbolization (符号化)

**核心数据结构** (`common/expression_struct.h`):

```cpp
// 每个维度大小用表达式树表示
using SizeExpr = af::Expression;

// 表达式可以是:
//   - ConstExpr:    1024 (静态维度)
//   - SymbolExpr:   s0, s1 (动态维度符号)
//   - BinaryExpr:   s0 * 16, s0 // 8 (算术表达式)
```

**在流水线中的流转**:

```
1. 图构建时 (ascir_api.py):
   ascir.Data([batch, 512])  ← batch 是动态的, 创造符号变量 s0
   
2. 优化时 (optimize.cpp):
   MergeContinuousAxis() 检查 stride, 合并连续轴
   表达式保留为 symbolic: Size(s0 * 512) → 一个统一维度
   
3. 调度时 (autoschedule.cpp):
   PrepareTilingCases() 读取每个轴的 SizeExpr
   根据表达式推断分块可行性: Const 可以编译期确定, Symbol 需要运行时查询
   
4. CodeGen 时 (expression_convert.cpp):
   Tiler::Size(expr)  → "t.tiler.sizes[0]"     (总大小)
   Tiler::ActualSize(expr) → "min(TILE_SIZE, t.tiler.sizes[0] - offset)"
   Tiler::Offset(expr) → "block_idx * TILE_SIZE"
   
5. 生成 TilingData 结构体:
   struct AutofuseTilingData {
       int64_t sizes[4];       // 每个轴的总大小
       int64_t actual_sizes[4]; // 每次迭代的实际大小
       int64_t offsets[4];     // 偏移量
   };
```

**符号解析示例**:

```
输入:  input shape = [batch, 512], batch = s0 (符号)
调度:  TileInner = 256, TileOuter = ceil(s0 * 512 / 256)

生成的 CodeGen 代码:
  int64_t ub_size = t.data.sizes[0];  // = batch * 512
  for (int64_t tile = 0; tile < t.tile_count; tile++) {
      int64_t actual = min(256, ub_size - tile * 256);  // 尾块处理
      DataCopy(local, gm + tile * 256, actual);
      for (int64_t i = 0; i < actual; i++) { ... }
  }
```

### 4.3 Schedule (调度)

**入口**: `AutoSchedule::DoAutoSchedule()` (`optimize/autoschedule/autoschedule.cpp`)

**完整流程**:

```
输入: HintGraph (带 axis 标注的算子图)

步骤1: 轴分类 (TilingGroup::GenTilingGroup())
  ┌──────────────────┬─────────────────────────────────┐
  │ 轴分组             │ 作用                            │
  ├──────────────────┼─────────────────────────────────┤
  │ X组 (非缩减)      │ 优先用于 Tile 拆分               │
  │ Y组 (非缩减)      │ 次要用于 Tile 拆分               │
  │ R组 (缩减轴)      │ Reduce 操作的归约轴               │
  │ N组 (块轴)        │ 用于 Block 拆分 (多核并行)       │
  └──────────────────┴─────────────────────────────────┘

步骤2: TilingCase 枚举 (GenTilingCase())
  对每个轴尝试: BlockOuter / BlockInner / TileOuter / TileInner / Original
  穷举所有组合 → PruneTilingCase() 剪枝无效组合

步骤3: 对每个 TilingCase 执行 Scheduler::DoScheduler()
  ├─ TileSplit()    ← UB 内分块
  │    将一个轴拆为: TileOuter (多轮迭代, 每轮重新加载数据)
  │                    TileInner (UB驻留, 数据常驻计算)
  │    原则: TileInner 尽量利用 256KB UB 容量
  │    示例: axis[0]=1024 → TileOuter=4 × TileInner=256
  │
  ├─ BlockSplit()   ← 多核分块
  │    将一个轴拆为: BlockOuter (跨核分裂)
  │                    BlockInner (每核内部迭代)
  │    原则: BlockOuter = device.core_count (例如8核)
  │    示例: axis[0]=1024, 8核 → BlockOuter=8 × BlockInner=128
  │
  ├─ SelectLoopAxis()  ← 选择最优循环轴顺序
  └─ 评估得分 → 选择最高分方案

步骤4: TemplateGeneratorHandler::GenerateTemplates()
  根据平台 (ascend950/mc62等) 生成多套 Tiling 模板
  用于运行时根据实际 shape 选择最优配置
```

**调度输出 — ScheduledResult**:

```cpp
struct ScheduledResult {
    vector<ScheduleGroup> schedule_groups;  // 每个调度组
    ScoreFunc score_func;                   // 动态Shape打分函数
    CubeTemplateType cube_type;             // kFixpip/kUBFuse/kCommon
    bool enable_group_parallel;             // 组间是否可并行
};

struct ScheduleGroup {
    vector<ImplGraph> impl_graphs;          // 调度后的子图
    bool double_buffer;                     // 是否启用双缓冲
};
```

### 4.4 CodeGen (代码生成) — 包含 Lowering

**入口**: `codegen::Codegen::Generate()` (`codegen/codegen.cpp:389`)

**三步输出**:

```
FusedScheduledResult
    │
    ├── GenerateTilingData()
    │   → struct AutofuseTilingData { ... }   (头文件)
    │
    ├── GenerateKernel()    ← 核心: 包含 Lowering
    │   ├─ ParseGraph()     ← 解析 ImplGraph → TPipe/Tiler/Tensor 对象
    │   ├─ Loop::Generate() ← 构建嵌套 for 循环
    │   ├─ 遍历节点, ApiCall::Generate() ← Lowering: 节点→AscendC API
    │   └─ → kernel.cpp   (AscendC 设备端源码)
    │
    └── GenerateTiling()    ← 调用 ATT
        → tiling.cpp        (Host 端 Tiling 函数)
```

**Loop 生成示例**:

```cpp
// Loop::Generate() 根据调度好的 axis 属性生成:
for (int64_t block_idx = 0; block_idx < t.block_dim; block_idx++) {
    for (int64_t tile_i = 0; tile_i < t.tile_out_count; tile_i++) {
        // ── TileOuter: DataCopy 在此 ──
        DataCopy(local_ub0, gm[block_idx * t.block_size + tile_i * t.tile_out_size],
                 t.tile_out_size);

        for (int64_t i = 0; i < t.tile_in_size; i++) {
            // ── TileInner: 计算指令在此 ──
            AscendC::Add(local_ub1, local_ub0[i], local_ub2[i], t.repeat);
            AscendC::Mul(local_ub1, local_ub1, scalar, t.repeat);
            AscendC::Relu(local_ub1, local_ub1, t.repeat);
            //   ↑ 三个算子在 UB 上原地连续计算，无 HBM 读写
        }

        DataCopy(gm_output[offset], local_ub1, t.tile_out_size);
    }
}
```

### 4.5 ATT (Automatic Tiling Template — 模板选择)

**文件**: `autofuse/att/` (独立的插件式编译模块)

**ATT 不是 "Attention"**，而是 **Automatic Tiling Template** — 自动 Tiling 模板生成器。

**作用**: 为 host 端生成 Tiling 函数，在推理时根据实际输入 shape 计算分块策略。

**入口函数**: `GenTilingImplAutoFuseV3()` (`att/gen_tiling_impl.cpp`)

```
FusedScheduledResult (调度后的图 + 元数据)
    │
    ▼
ModelInfo 构建
  ├─ 解析 ImplGraph → 提取 Tiling 空间的维度变量
  ├─ 建立性能模型: 估算每种分块配置的执行时间
  └─ 确定搜索空间: 哪些轴可以分块, 分块粒度范围
    │
    ▼
模板策略选择
  ├─ AxesReorder 策略:
  │    遍历所有轴排序和 BlockDim 排列
  │    搜索使内存带宽最大化的配置
  │    输出: 带 Cache 的搜索函数
  │
  └─ HighPerf 策略:
       使用预训练的查找表
       直接映射 Shape → 最优 Tiling
       输出: key→value 查找函数
    │
    ▼
Tiling 代码生成
  ├─ 生成 for(轴排序) { for(块维度) { 评估得分 } } 的搜索代码
  ├─ 生成最优配置的缓存逻辑
  └─ → tiling.cpp (host 端源码)
```

**生成的 Tiling 函数示例**:

```cpp
// ATT 生成的 Host 端 Tiling 函数 (简化)
extern "C" int AutofuseTilingFunc(gert::TilingContext* context) {
    // 1. 获取平台信息和输入 shape
    uint64_t ub_size; int64_t core_num;
    GetPlatformInfo(context, ub_size, core_num);
    auto shape = context->GetInputShape(0);  // 如 [batch, 512]
    
    // 2. 查缓存或搜索最优配置
    auto* tiling = context->GetTilingData<AutofuseTilingData>();
    
    // 3. 计算分块大小
    tiling->block_dim = core_num;
    tiling->tile_inner_size = CalculateMaxTileSize(ub_size, shape, dtype);
    tiling->tile_outer_size = tiling->tile_inner_size;
    tiling->tile_out_count = CeilDiv(shape[0], tiling->tile_outer_size);
    
    // 4. 设置 blockDim 和 tiling key
    context->SetBlockDim(tiling->block_dim);
    context->SetTilingKey(tiling_key);
    
    return GRAPH_SUCCESS;
}
```

---

## 五、完整流水线: Torch 代码 → NPU 执行

```
┌─ 用户代码 ──────────────────────────────────────────────────────┐
│ model = torch.compile(my_model, backend="inductor")             │
│ output = model(input.to("npu"))                                  │
└─────────────────────────────────────────────────────────────────┘
    │
    ▼  TorchDynamo 截获 FX Graph
┌─ TorchAir ───────────────────────────────────────────────────────┐
│ FX Graph → ComputeGraph (FusedGraph)                            │
│ 每个子图作为 "AscBackend" 节点嵌入                              │
│ 含符号化 Shape: batch=s0, dim=512                               │
└─────────────────────────────────────────────────────────────────┘
    │
    ▼  进入 graph-autofusion
┌─ Optimizer::Optimize() ──────────────────────────────────────────┐
│ Unfold FusedGraph → HintGraph                                   │
│ Axes合并, 类型一致性, Pass优化                                   │
└─────────────────────────────────────────────────────────────────┘
    │
    ▼
┌─ AutoSchedule ───────────────────────────────────────────────────┐
│ 轴分类(X/Y/R/N) → TilingCase枚举 → TileSplit + BlockSplit       │
│ → ScheduledResult { ScheduleGroup[], cube_type, score_func }    │
└─────────────────────────────────────────────────────────────────┘
    │
    ▼
┌─ BufQueAlloc ────────────────────────────────────────────────────┐
│ UB内存分配, 生命周期复用                                         │
└─────────────────────────────────────────────────────────────────┘
    │
    ├────────────────────┬────────────────────┐
    ▼                    ▼                    ▼
┌─ ATT ──────────┐ ┌─ TilingData ────┐ ┌─ CodeGen ──────────────┐
│ 模板选择        │ │ 生成struct      │ │ ① Lowering (核心)       │
│ AxesReorder or  │ │ sizes/offsets/  │ │   ASCIR节点 → ApiCall   │
│ HighPerf 策略   │ │ actual_sizes    │ │   → AscendC API调用     │
│                 │ │                 │ │                         │
│ → tiling.cpp    │ │ → tiling_data.h │ │ ② Loop树构建            │
│ (host端源码)    │ │                 │ │ ③ Tiler表达式转换       │
└─────────────────┘ └─────────────────┘ │ → kernel.cpp (设备源码) │
                                        └─────────────────────────┘
    │                    │                    │
    ├────────────────────┴────────────────────┤
    ▼                                          │
┌─ 编译链接 ─────────────────────────────────────────────┐
│ bisheng kernel.cpp → kernel.o                        │
│ g++      tiling.cpp → tiling.o                        │
│ g++ kernel.o + tiling.o → kernel.so + kernel.json    │
└───────────────────────────────────────────────────────┘
    │
    ▼
┌─ Runtime 执行 ───────────────────────────────────────────────────┐
│ Load kernel.so → aclrtLaunchKernel() → AI Core 执行              │
└──────────────────────────────────────────────────────────────────┘
```

---

## 六、关键洞察

### 6.1 Lowering 是 CodeGen 的一部分

graph-autofusion 中的 "Lowering" 不是独立 pass，而是 CodeGen 阶段的一个子功能:
- `codegen/api_call/` 下的 ApiCall 工厂就是 Lowering 的实现
- 每个 ApiCall 子类将 ASCIR 节点映射到 AscendC API 字符串
- 这是 "op level lowering"：高层算子语义 → 底层 AscendC 函数调用

### 6.2 IR 只有一种类型，通过修改演化

```cpp
using HintGraph  = af::AscGraph;  // 输入 (from TorchAir)
using ImplGraph  = af::AscGraph;  // 调度后 (经过 Schedule)
using Graph      = af::AscGraph;  // 泛型引用
```

不是传统编译器的多 IR 设计，而是同一数据结构在不同阶段被原地修改。

### 6.3 ATT 是独立的编译插件

ATT (`autofuse/att/`) 通过 `dlopen` 动态加载，不是编译进主二进制:
```cpp
using TilingLibCodegenFunc = bool (*)(...);
// 在 codegen_tiling.h 中定义为函数指针
// 运行时 dlopen 包含 GenTilingImplAutoFuseV3 的 .so
```

### 6.4 Pass 是 Schedule 的预处理

Pass 在图进入 Schedule 之前做等价变换和规范化：`Pow(x,2)→Mul(x,x)` 统一为 Binary 操作，消除冗余 Broadcast 等。这使得 Schedule 面对的是简洁的算子图，分拆策略更有效。

---

## 七、模块层次对应关系
|------|------|---------------------|----------|
## 七、模块层次对应关系

| 模块 | 作用 | 属于之前定义的第几层 | 核心文件 |
|------|------|---------------------|----------|
| **Pass** | 图规范化/等价变换/预处理 | Layer 3-3.5 (GE Compiler → graph-autofusion) | `optimize/graph_pass/`, `ge/compiler/.../fusion_pass/` |
| **Lowering** (上段) | Torch FX → AscGraph | Layer 1-2 (Framework Adapter → GE Parser) | `ge/dflow/.../torch_plugin.py` |
| **Symbolization** | 动态Shape追踪 | Layer 3-4 (GE Compiler → Operator IR) | `common/expression_struct.h`, `ascir/ascir.h` |
| **Lowering** (下段) | ASCIR → AscendC API | Layer 4→5 (Operator IR → AscendC) | `codegen/api_call/` |
| **Schedule** | Tiling/Block分拆 | Layer 3.5 (graph-autofusion) | `optimize/autoschedule/` |
| **CodeGen** | 生成核函数源码 | Layer 3.5→5 (graph-autofusion → AscendC) | `codegen/` |
| **ATT** | Tiling函数生成+模板选择 | Layer 3.5 (graph-autofusion) | `att/` |

---

## 八、代码结构索引

```
graph-autofusion/autofuse/
├── ascir/                          ← ASCIR 图定义 (HintGraph/ImplGraph)
│   ├── generator/
│   │   └── v1_ascir_codegen_impl.h ← ApiCall 注册表 (Lowering映射)
│   └── meta/ascir.h               ← IR 类型定义 (Symbolic 集成)
├── codegen/                        ← ④ CodeGen + 下段 Lowering
│   ├── codegen.cpp/h               ← 总入口
│   ├── codegen_kernel.cpp          ← 核函数生成主逻辑
│   ├── codegen_kernel_loop.cpp     ← Loop 树构建
│   ├── api_call/                   ← ApiCall 工厂 (核心 Lowering)
│   │   ├── elewise/binary_api_call.cpp    ← Add/Mul/Sub/Div
│   │   ├── elewise/unary_api_call.cpp     ← Relu/Exp/Sqrt
│   │   ├── reduce/reduce_api_call.cpp     ← Reduce 系列
│   │   ├── load_store/load_api_call.cpp   ← DataCopy (Load)
│   │   ├── load_store/store_api_call.cpp  ← DataCopy (Store)
│   │   └── api_call_factory.h             ← 工厂注册
│   └── expression_convert.cpp     ← ② 符号表达式→C++代码 (Tiler)
├── optimize/                       ← ③ Schedule
│   ├── optimize.cpp                ← 总优化入口
│   ├── autoschedule/               ← 自动调度
│   │   ├── autoschedule.cpp        ← AutoSchedule 主逻辑
│   │   ├── schedule.cpp            ← TileSplit + BlockSplit
│   │   └── tiling_group.h          ← 轴分类 (X/Y/R/N组)
│   ├── schedule_task_generator.cpp ← 图层拆分
│   ├── buf_que_allocator.cpp       ← Buffer分配
│   └── fused_graph/                ← 图展开
│       └── fused_graph_unfolder.cpp ← AscBackend 展开
├── att/                            ← ⑤ ATT 模板选择
│   ├── gen_tiling_impl.cpp         ← GenTilingImplAutoFuseV3 入口
│   ├── gen_tiling_impl.h           ← 接口定义
│   └── generator/                  ← 策略实现
│       ├── axes_reorder_strategy.cpp ← AxesReorder 搜索
│       └── high_perf_strategy.cpp   ← HighPerf 查表
└── common/                         ← ② Symbolization 工具
    ├── expression_struct.h          ← Symbolic Expression 框架
    └── expression_convert_struct.h  ← 表达式→Tiler 转换
```
