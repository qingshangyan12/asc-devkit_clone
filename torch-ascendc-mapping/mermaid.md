# PyTorch → AscendC 映射层次结构 Mermaid 图

## 完整映射层次关系

```mermaid
graph TD
    subgraph "PyTorch 用户层"
        PT_USER["PyTorch Python API<br/>torch.nn.Conv2d, torch.matmul<br/>F.relu, nn.Linear, ..."]
        PT_COMPILE["torch.compile()<br/>+ Inductor 后端"]
        PT_EAGER["Eager Mode<br/>逐算子执行"]
    end

    subgraph "PyTorch 后端适配层"
        TORCH_NPU["torch_npu<br/>设备后端<br/>算子映射 → aclnn<br/>内存/Stream 管理"]
        TORCHAIR["TorchAir<br/>AtenIR → AscendIR<br/>FX Graph lowering"]
        INDUCTOR_NPU["inductor-npu-ext<br/>Inductor 后端<br/>→ Autofuse"]
    end

    subgraph "Autofuse 自动融合 (graph-autofusion/)"
        ASCIR_API["ascir_api.py<br/>Ascend IR 构造<br/>Data/Load/Store/Add/MatMul/Reduce"]
        SCHEDULE["Schedule<br/>自动 Tiling<br/>Buffer 分配<br/>Task 并行化"]
        CODEGEN["CodeGen<br/>Kernel Codegen (Ascend C)<br/>Tiling Codegen<br/>Expression 转换"]
        COMPILE_PY["编译管线<br/>asc_codegen_compile.py<br/>ascendc_compile.py<br/>ascbc_kernel_compile.py"]
        SUPERK["SuperKernel<br/>全图融合<br/>ICache Preload"]
        PGO["PGO<br/>Profile-Guided<br/>优化 Tiling"]
    end

    subgraph "GE 图引擎层 (ge/)"
        ONNX_PARSER["ONNX/AtenIR Parser<br/>→ AscendIR"]
        COMPILER["GE Compiler<br/>Prepare→Optimize→Partition→Build"]
        ASCIR["AscendIR<br/>ComputeGraph<br/>Node→OpDesc→GeTensorDesc"]
        GEMODEL["GeRootModel<br/>TaskDef + 权重 + Kernel"]
    end

    subgraph "算子库 (ops-math/)"
        ACLNN_API["aclnn API<br/>单算子调用入口<br/>aclnnAdd/aclnnMatMul/..."]
        OP_KERNEL["op_kernel/<br/>Ascend C 核函数<br/>__global__ __aicore__"]
        OP_HOST["op_host/<br/>算子注册 / Tiling"]
    end

    subgraph "Ascend C 编程框架 (asc-devkit)"
        HIGH_API["High-level API<br/>预构建: MatMul, Softmax"]
        TPIPE_API["Tpipe/Tque API<br/>Tensor+Queue 自动管理"]
        BASE_API["Base API<br/>手动 Tensor/Buffer"]
        C_API["C API<br/>纯C指针"]
        SIMT_API["SIMT API<br/>Warp 并行"]
    end

    subgraph "模板库 (atvoss)"
        ATVOSS["atvoss<br/>表达式模板库<br/>out = in * scalar<br/>零开销抽象"]
    end

    subgraph "元数据基础 (metadef)"
        METADEF["类型系统<br/>Tensor/Shape/DataType/Format<br/>OpDesc/ComputeGraph"]
    end

    subgraph "Runtime (runtime)"
        ACL_RT["ACL API<br/>aclrt (设备/Stream/内存)<br/>aclmdl (模型)<br/>aclop (单算子)"]
        GE_RT_V1["GE Runtime v1<br/>DavinciModel<br/>TaskSink"]
        GE_RT_V2["GE Runtime v2<br/>ModelV2Executor<br/>动态Shape"]
    end

    subgraph "硬件"
        NPU["昇腾 NPU<br/>AI Core / Vector Core<br/>AI CPU / HBM"]
    end

    %% 路径A: Eager
    PT_USER -->|"逐算子<br/>torch.add(a,b)"| PT_EAGER
    PT_EAGER -->|"分发"| TORCH_NPU
    TORCH_NPU -->|"映射"| ACLNN_API
    ACLNN_API -->|"Tiling"| OP_HOST
    ACLNN_API -->|"核函数"| OP_KERNEL
    OP_KERNEL -->|"基于"| BASE_API
    OP_KERNEL -->|"基于"| TPIPE_API

    %% 路径B: torch.compile + Inductor
    PT_USER -->|"torch.compile(model,<br/>backend='inductor')"| PT_COMPILE
    PT_COMPILE -->|"FX Graph"| INDUCTOR_NPU
    INDUCTOR_NPU -->|"构建 AscendIR"| ASCIR_API
    ASCIR_API -->|"优化调度"| SCHEDULE
    ASCIR_API -->|"代码生成"| CODEGEN
    SCHEDULE -->|"Tiling计划"| CODEGEN
    CODEGEN -->|"生成核函数"| COMPILE_PY
    COMPILE_PY -->|"bisheng 编译"| OP_KERNEL

    %% SuperKernel
    SCHEDULE -->|"全图融合"| SUPERK
    SUPERK -->|"指令预取"| OP_KERNEL

    %% PGO
    COMPILE_PY -->|"Profile反馈"| PGO
    PGO -->|"优化参数"| SCHEDULE

    %% 路径C: TorchAir
    PT_USER -->|"TorchAir"| TORCHAIR
    TORCHAIR -->|"AtenIR→AscendIR"| ONNX_PARSER
    ONNX_PARSER -->|"Parse"| ASCIR
    ASCIR -->|"编译"| COMPILER
    COMPILER -->|"产出"| GEMODEL

    %% GE 到 Runtime
    GEMODEL -->|"Load"| GE_RT_V1
    GEMODEL -->|"Load"| GE_RT_V2
    GE_RT_V1 -->|"rtModelExecute"| ACL_RT
    GE_RT_V2 -->|"SequentialExec"| ACL_RT

    %% 算子库到 AscendC + atvoss
    OP_KERNEL -->|"构建"| ATVOSS
    ATVOSS -->|"生成"| OP_KERNEL
    OP_HOST -->|"Tiling API"| METADEF

    %% Runtime 到硬件
    ACL_RT --> NPU

    %% 样式
    style PT_USER fill:#4a90d9,color:#fff
    style TORCH_NPU fill:#2d8c8c,color:#fff
    style ASCIR_API fill:#e67e22,color:#fff
    style CODEGEN fill:#e74c3c,color:#fff
    style OP_KERNEL fill:#c0392b,color:#fff
    style NPU fill:#2c3e50,color:#fff
    style SUPERK fill:#8e44ad,color:#fff
```

## 三种执行路径对比

```mermaid
graph LR
    subgraph "路径A: Eager 逐算子"
        A1["torch.add(a,b)"] -->|"torch_npu"| A2["aclnnAdd()"] -->|"L0 API"| A3["Tiling + Kernel"] --> A4["AI Core"]
    end

    subgraph "路径B: compile+Inductor+Autofuse"
        B1["torch.compile(model)"] -->|"TorchDynamo"| B2["FX Graph"] -->|"Inductor"| B3["Autofuse"] -->|"自动融合"| B4["融合 AscendC 核"] --> B5["AI Core"]
    end

    subgraph "路径C: TorchAir+GE"
        C1["AtenIR"] -->|"TorchAir"| C2["AscendIR"] -->|"GE Compiler"| C3["OM Model"] -->|"GE Runtime"| C4["AI Core"]
    end

    style A4 fill:#27ae60,color:#fff
    style B5 fill:#27ae60,color:#fff
    style C4 fill:#27ae60,color:#fff
```

## Autofuse 内部管线

```mermaid
graph TD
    subgraph "Python 接口"
        P1["用户 FX Graph<br/>(torch.compile 产物)"]
        P2["ascir_api.py<br/>构建 AscendIR"]
        P3["pyautofuse.cpp<br/>Python→C++ 绑定"]
    end

    subgraph "C++ Schedule"
        S1["autoschedule.cpp<br/>自动 Tiling<br/>计算每核分块大小"]
        S2["buffer_allocate.cpp<br/>Buffer 分配<br/>复用中间 Buffer"]
        S3["task_generator.cpp<br/>Task 生成<br/>并行化调度"]
        S4["fused_graph.cpp<br/>融合图管理<br/>依赖分析"]
    end

    subgraph "C++ CodeGen"
        C1["kernel_codegen.cpp<br/>生成 Ascend C .cpp<br/>DAG + 循环 + 同步"]
        C2["tiling_codegen.cpp<br/>生成 Tiling Data .cpp<br/>块大小/步长计算"]
        C3["expression_convert.cpp<br/>表达式转换<br/>AscendIR → AscendC"]
        C4["ascendc_api.cpp<br/>AscendC API 封装<br/>类型映射"]
    end

    subgraph "编译与链接"
        L1["ascendc_compile.py<br/>bisheng 编译 → .o"]
        L2["link_kernel_target<br/>链接 → .so<br/>libascendcl, libruntime"]
        L3["PGO 调优<br/>运行反馈 → 重新编译"]
    end

    P1 --> P2
    P2 --> P3
    P3 --> S1
    S1 --> S2
    S2 --> S3
    S3 --> S4
    S4 --> C1
    S1 --> C2
    P3 --> C3
    C1 --> C4
    C2 --> L1
    C4 --> L1
    L1 --> L2
    L2 --> L3

    style P1 fill:#4a90d9,color:#fff
    style S1 fill:#e67e22,color:#fff
    style C1 fill:#e74c3c,color:#fff
    style L2 fill:#27ae60,color:#fff
```

## 算子层次对照: PyTorch Op → 底层 API

```mermaid
graph TD
    subgraph "PyTorch 层"
        PT_ADD["torch.add(a, b, alpha=2)"]
        PT_MATMUL["torch.matmul(a, b)"]
        PT_RELU["F.relu(x)"]
        PT_CONV["F.conv2d(x, w)"]
    end

    subgraph "aclnn API 层"
        ACL_ADD["aclnnAdd(self, other, alpha)"]
        ACL_MATMUL["aclnnMatMul(self, other)"]
        ACL_RELU["aclnnRelu(self)"]
        ACL_CONV2D["aclnnConv2d(x, w)"]
    end

    subgraph "算子库实现"
        OP_ADD["math/add/<br/>op_host + op_kernel + op_api"]
        OP_MATMUL["math/matmul/<br/>Cube 核函数 + Tiling + API"]
        OP_RELU["nn/relu/<br/>Vector 核函数 + API"]
        OP_CONV2D["nn/conv2d/<br/>Cube+Vector 混合 + Winograd"]
    end

    subgraph "Ascend C 底层"
        BASE_OPS["向量运算: Add/Mul/Sqrt/Exp/..."]
        CUBE_OPS["矩阵运算: Mmad/LoadData/Fixpipe"]
        PIPE_OPS["数据传输: DataCopy/DataCopyPad"]
        SYNC_OPS["同步: SetFlag/WaitFlag/PipeBarrier"]
        MEM_OPS["内存: GlobalTensor/LocalTensor/MakeTensor"]
    end

    PT_ADD --> ACL_ADD
    PT_MATMUL --> ACL_MATMUL
    PT_RELU --> ACL_RELU
    PT_CONV --> ACL_CONV2D

    ACL_ADD --> OP_ADD
    ACL_MATMUL --> OP_MATMUL
    ACL_RELU --> OP_RELU
    ACL_CONV2D --> OP_CONV2D

    OP_ADD --> BASE_OPS
    OP_RELU --> BASE_OPS
    OP_MATMUL --> CUBE_OPS
    OP_CONV2D --> BASE_OPS
    OP_CONV2D --> CUBE_OPS
    OP_ADD --> PIPE_OPS
    OP_ADD --> SYNC_OPS
    OP_ADD --> MEM_OPS

    style PT_ADD fill:#4a90d9,color:#fff
    style ACL_ADD fill:#2d8c8c,color:#fff
    style OP_ADD fill:#e67e22,color:#fff
    style BASE_OPS fill:#c0392b,color:#fff
```

## atvoss 模板库 - 五层分层架构

```mermaid
graph TD
    subgraph "atvoss 模板库 (表达式模板, 零运行时开销)"
        L5["Device Layer<br/>主机端入口<br/>ACL 初始化/内存/Kernel 调用"]
        L4["Kernel Layer<br/>多核任务分解与调度<br/>set_block_dim/block_idx"]
        L3["Block Layer<br/>单核任务 Tiling<br/>双缓冲 Pipeline 编排"]
        L2["Tile Layer<br/>高层抽象<br/>VecIn/VecOut/AddAssign/ReduceSum"]
        L1["Basic Layer<br/>直接使用 Ascend C API<br/>DataCopy/Add/Mul/PipeBarrier"]
    end

    subgraph "示例: RMS Norm"
        EXPR["表达式定义"]
        COMPUTE["out = in2 * (in1 / sqrt(mean(in1^2)))"]
        EXPAND["编译期展开为Tile层操作序列"]
    end

    L5 --> L4
    L4 --> L3
    L3 --> L2
    L2 --> L1
    EXPR -->|"编译期"| EXPAND
    EXPAND -->|"生成"| L2

    style L5 fill:#2c3e50,color:#fff
    style L1 fill:#c0392b,color:#fff
    style EXPR fill:#27ae60,color:#fff
```

## PyTorch ↔ TF 代码路径对比

```mermaid
graph LR
    subgraph "PyTorch"
        P1["torch.matmul(a,b)"] --> P2["torch_npu 分发"]
        P2 --> P3["aclnnMatMul API"]
        P3 --> P4["ops-math MatMul 算子实现"]
    end

    subgraph "共享"
        S1["同一套算子库<br/>(ops-math/math/matmul/)"]
        S2["同一种 IR<br/>(AscendIR/ComputeGraph)"]
        S3["同一套 Runtime<br/>(ACL/GE/Stream)"]
        S4["同一目标硬件<br/>(Ascend NPU)"]
    end

    subgraph "TensorFlow"
        T1["tf.matmul(a,b)"] --> T2["GeOp 捕获"]
        T2 --> T3["GE Parser"]
        T3 --> T4["ops-math MatMul 算子实现"]
    end

    P4 --> S1
    T4 --> S1
    S1 --> S2
    S2 --> S3
    S3 --> S4

    style S1 fill:#9b59b6,color:#fff
    style S4 fill:#2c3e50,color:#fff
```

## Autofuse 自动融合完整管线

```mermaid
graph TD
    subgraph "步骤1: 图预处理 (pre_process.cpp)"
        P1A["RemoveDanglingNodes"]
        P1B["NormalizeAxisIds"]
        P1C["DtypeConsistency(插入Cast)"]
        P1D["ScalarOptimization"]
    end

    subgraph "步骤2: 轴合并 (fused_graph.cpp)"
        P2A["判断连续轴:<br/>stride[i]==stride[i+1]×repeat[i+1]"]
        P2B["合并为单一轴<br/>减少循环嵌套"]
    end

    subgraph "步骤3: 图分区 (schedule_task_generator.cpp)"
        P3A["ScheduleGroupPartitioner<br/>按类型分组: Elewise/Broadcast/<br/>Reduce/Concat/Transpose/<br/>Gather/Cube/Split"]
        P3B["groups_relations_in<br/>组间Tensor流动"]
    end

    subgraph "步骤4: 自动调度 (autoschedule.cpp)"
        P4A["TilingCase生成<br/>5种AxisType:<br/>BlockOuter/BlockInner/<br/>TileOuter/TileInner/Original"]
        P4B["Cube模板: kFixpip/kUBFuse/kCommon"]
        P4C["Reduce模板: kAllLoad/kCommon/kRCore"]
        P4D["NDDMA打分(动态Shape运行时选)"]
        P4E["双缓冲flag"]
    end

    subgraph "步骤5: Buffer分配 (buf_que_allocator.cpp)"
        P5A["MemReuseManager<br/>生命周期复用UB"]
        P5B["TQue(Cube↔Vector)+TBuf(Vector)"]
    end

    subgraph "步骤6: 代码生成 (codegen/)"
        P6A["ApiCall工厂(20+子类)<br/>Node→AscendC API映射"]
        P6B["Loop树构建<br/>3层嵌套: BlockOuter/TileOuter/TileInner"]
        P6C["每ApiCall三阶段:<br/>PreProcess→ApiCall→PostProcess"]
        P6D["表达式转换<br/>图表达式→Tiler表达式"]
    end

    subgraph "步骤7: 编译输出"
        OUT1["lib{kernel}.so + kernel_meta/*.json"]
    end

    P1A --> P1B --> P1C --> P1D
    P1D --> P2A --> P2B
    P2B --> P3A --> P3B
    P3B --> P4A
    P4A --> P4B
    P4A --> P4C
    P4A --> P4D
    P4A --> P4E
    P4B --> P5A
    P4C --> P5A
    P4D --> P5A
    P4E --> P5A
    P5A --> P5B
    P5B --> P6A --> P6B --> P6C
    P6C --> P6D
    P6D --> OUT1

    style P6A fill:#e74c3c,color:#fff
    style P4A fill:#e67e22,color:#fff
    style OUT1 fill:#27ae60,color:#fff
```

## Autofuse 融合前后数据搬运对比

```mermaid
graph LR
    subgraph "融合前: 逐算子 = 6次HBM事务"
        H0A["HBM"] -->|"Load"| O1["Add"] -->|"Store"| H0B["HBM"]
        H0C["HBM"] -->|"Load"| O2["Mul"] -->|"Store"| H0D["HBM"]
        H0E["HBM"] -->|"Load"| O3["Relu"] -->|"Store"| H0F["HBM"]
    end

    subgraph "融合后: 融合核 = 2次HBM事务"
        H1A["HBM"] -->|"Load"| F1["Add→Mul→Relu"] -->|"Store"| H1B["HBM"]
    end

    style O1 fill:#e74c3c,color:#fff
    style O2 fill:#e74c3c,color:#fff
    style O3 fill:#e74c3c,color:#fff
    style F1 fill:#27ae60,color:#fff
```

## SuperKernel 运行时调度

```mermaid
graph TD
    subgraph "AOT编译时"
        A1["解析模型RI节点"] --> A2["SuperKernelGraph构建"] --> A3["Scope分割(4Pass)"] --> A4["SkTaskBuilder生成TaskQue"]
    end

    subgraph "设备端执行 (sk_entry.asc)"
        D1["coreId%entryCnt选副本"]
        D2["遍历TaskQue"]
        D3["PRELOAD: dc_preload预取"]
        D4["FUNC: 函数指针调用"]
        D5["SYNC: CrossCore同步"]
        D6["EVENT: 内存语义通知"]
    end

    subgraph "核心优化"
        O1["子Kernel拆分(×4): 缓解L2争用"]
        O2["ICache Preload: 消除ICache Miss"]
        O3["Early-Start: MTE+Scalar并发"]
    end

    A4 --> D1
    D1 --> D2
    D2 --> D3
    D2 --> D4
    D2 --> D5
    D2 --> D6
    D3 --> O2
    D4 --> O1
    D5 --> O3

    style D2 fill:#e74c3c,color:#fff
    style O1 fill:#27ae60,color:#fff
    style O2 fill:#27ae60,color:#fff
    style O3 fill:#27ae60,color:#fff
```

## Cube+Vector 融合模式决策树

```mermaid
graph TD
    START["含Cube算子?"]
    START -->|"否"| VEC["Vector-only: 标准Elewise"]
    START -->|"是"| HAS["后续有Elewise?"]
    HAS -->|"否"| COM["kCommon: Fixpipe→HBM"]
    HAS -->|"是"| CAN["Vec能直接读Cube输出?"]
    CAN -->|"是"| FIX["kFixpip: 最快,零拷贝"]
    CAN -->|"否"| UB["kUBFuse: UB原地,无HBM写回"]

    style FIX fill:#e74c3c,color:#fff
    style UB fill:#e67e22,color:#fff
    style COM fill:#3498db,color:#fff
```

## ApiCall 代码生成流程

```mermaid
graph TD
    NODE["AscGraph节点 type=Add"] --> FACTORY["ApiCall工厂<br/>→BinaryApiCall"]
    FACTORY --> LOOP["Loop树<br/>BlockOuter→TileOuter→TileInner"]
    LOOP --> PRE["PreProcess()<br/>WaitFlag+AllocTQue"]
    PRE --> CALL["ApiCall()<br/>Add(z,x,y,size)"]
    CALL --> POST["PostProcess()<br/>SetFlag+FreeTQue"]
    POST --> CODE["生成: SetFlag;Alloc;Add;SetFlag;Free"]

    style FACTORY fill:#e67e22,color:#fff
    style CALL fill:#e74c3c,color:#fff
    style CODE fill:#27ae60,color:#fff
```

## Lowering → Symbolization → Schedule → CodeGen → ATT 完整流水线

```mermaid
graph TD
    subgraph "用户层 + 框架适配"
        TORCH["torch.compile(model, backend='inductor')"]
        FX["TorchDynamo → FX Graph"]
        TA["TorchAir<br/>AtenIR → FusedGraph<br/>(含 AscBackend 子图)"]
    end

    subgraph "graph-autofusion ═══ 六个核心模块 ═══"

        subgraph "Pass (IR预处理优化)"
            PASS_A["GraphPass管线<br/>PowEquivSubstitution<br/>(Pow→Mul/Sqrt)<br/>ScalarBroadcastOpt<br/>CastOptimization<br/>ConcatInputUnification"]
            PASS_OUT["输出: 规范化后的HintGraph"]
        end

        subgraph "② Symbolization (符号化)"
            SYM["追踪动态Shape<br/>SizeExpr = Expression<br/>s0: 符号变量<br/>s0*512: 算术表达式"]
            SYM_OUT["输出: 带Symbolic Size<br/>的标注AscGraph"]
        end

        subgraph "③ Schedule (调度)"
            SCH_A["轴分类<br/>X组(非缩减)/Y组(UB拆分)<br/>R组(缩减轴)/N组(块轴)"]
            SCH_B["TilingCase枚举<br/>5种AxisType穷举<br/>+ 剪枝无效组合"]
            SCH_C["TileSplit<br/>轴→TileOuter+TileInner<br/>UB容量约束"]
            SCH_D["BlockSplit<br/>轴→BlockOuter+BlockInner<br/>多核并行"]
            SCH_OUT["输出: ScheduledResult<br/>ImplGraph + Tiling元数据"]
        end

        subgraph "⑤ ATT (模板选择)"
            ATT_A["ModelInfo构建<br/>Tiling空间的维度变量"]
            ATT_B["AxesReorder策略<br/>遍历轴排序+BlockDim排列<br/>搜索带宽最大化配置"]
            ATT_C["HighPerf策略<br/>预训练查表<br/>Shape→最优Tiling"]
            ATT_OUT["输出: tiling.cpp<br/>Host端Tiling函数源码"]
        end

        subgraph "④ CodeGen (含 ① Lowering)"
            CG_A["① Lowering<br/>ApiCall工厂<br/>ASCIR节点→AscendC API<br/>Add→::Add(), Relu→::Relu()"]
            CG_B["Loop树<br/>BlockOuter→TileOuter→TileInner<br/>嵌套for循环"]
            CG_C["Tiler表达式转换<br/>SizeExpr→C++代码<br/>t.tiler.sizes[0]"]
            CG_D["三阶段代码<br/>PreProcess→ApiCall→PostProcess<br/>同步+计算+释放"]
            CG_OUT["输出: kernel.cpp (设备源码)"]
        end

        subgraph "编译"
            COMPILE["bisheng → kernel.o<br/>g++ → tiling.o<br/>链接 → kernel.so + .json"]
        end
    end

    subgraph "Runtime + 硬件"
        RT["ACL Runtime<br/>Load kernel.so<br/>RunTiling函数<br/>LaunchKernel"]
        NPU["Ascend NPU<br/>AI Core 执行"]
    end

    TORCH --> FX --> TA
    TA -->|"含符号化Shape"| PASS_A
    PASS_A --> PASS_OUT
    PASS_OUT -->|"②"| SYM
    SYM --> SYM_OUT
    SYM_OUT --> SCH_A
    SCH_A --> SCH_B
    SCH_B --> SCH_C
    SCH_B --> SCH_D
    SCH_C --> SCH_OUT
    SCH_D --> SCH_OUT

    SCH_OUT --> ATT_A
    ATT_A --> ATT_B
    ATT_A --> ATT_C
    ATT_B --> ATT_OUT
    ATT_C --> ATT_OUT

    SCH_OUT --> CG_A
    CG_A --> CG_B
    CG_B --> CG_C
    CG_C --> CG_D
    CG_D --> CG_OUT
    
    ATT_OUT --> COMPILE
    CG_OUT --> COMPILE
    COMPILE --> RT --> NPU

    style PASS_A fill:#3498db,color:#fff
    style SYM fill:#9b59b6,color:#fff
    style SCH_C fill:#e67e22,color:#fff
    style ATT_A fill:#2d8c8c,color:#fff
    style CG_A fill:#e74c3c,color:#fff
    style CG_OUT fill:#27ae60,color:#fff
    style NPU fill:#2c3e50,color:#fff
```

## 六个模块在层次架构中的位置

```mermaid
graph TD
    subgraph "第1-2层: Framework Adapter + GE Parser"
        L12["TorchAir/TF Adapter<br/>→ FusedGraph (含符号化Shape)"]
    end

    subgraph "第3.5层: graph-autofusion (六个模块)"
        direction TB
        L35_PASS["Pass 预处理<br/>PowSub/CastOpt/BrcOpt"]
        L35_SYM["② Symbolization<br/>追踪动态维度"]
        L35_SCH["③ Schedule<br/>轴分类→Tiling→分拆"]
        L35_ATT["⑤ ATT<br/>模板选择→Tiling函数"]
        L35_LOW["① Lowering (下段)<br/>ASCIR→AscendC映射"]
        L35_CG["④ CodeGen<br/>Loop树+表达式→源码"]
        L35_PASS --> L35_SYM
        L35_SYM --> L35_SCH
        L35_SCH --> L35_ATT
        L35_SCH --> L35_LOW
        L35_LOW --> L35_CG
    end

    subgraph "第5层: AscendC 核函数"
        L5["核函数编程框架<br/>::Add(), ::Mul(), ::Relu()<br/>DataCopy, PipeBarrier"]
    end

    subgraph "第7层: Runtime + 硬件"
        L7["ACL → AI Core 执行"]
    end

    L12 -->|"HintGraph"| L35_PASS
    L35_CG -->|"生成的 AscendC 源码"| L5
    L35_ATT -->|"Tiling 函数"| L7
    L5 --> L7

    style L35_PASS fill:#3498db,color:#fff
    style L35_SYM fill:#9b59b6,color:#fff
    style L35_SCH fill:#e67e22,color:#fff
    style L35_ATT fill:#2d8c8c,color:#fff
    style L35_LOW fill:#e74c3c,color:#fff
    style L35_CG fill:#c0392b,color:#fff
```

## IR 演变: 单一类型通过修改演化

```mermaid
graph LR
    subgraph "AscGraph 类型的不同形态"
        HG["HintGraph<br/>输入: torch FX → TorchAir<br/>含符号化Shape"]
        IG["ImplGraph<br/>Schedule后:<br/>含Tile/Block分拆 + axis标注"]
        SR["ScheduledResult<br/>含ScheduleGroup[]<br/>+ cube_type + score_func"]
        CR["CodegenResult<br/>kernel.cpp + tiling.cpp<br/>+ tiling_data.h"]
    end

    HG -->|"Optimize()<br/>轴合并+Pass优化"| IG
    IG -->|"AutoSchedule()<br/>轴分类+枚举+分拆"| SR
    SR -->|"CodeGen.Generate()<br/>ApiCall+Loop+Tiler"| CR
    CR -->|"bisheng编译"| SO["kernel.so + .json"]

    style HG fill:#9b59b6,color:#fff
    style IG fill:#e67e22,color:#fff
    style SR fill:#2d8c8c,color:#fff
    style CR fill:#e74c3c,color:#fff
    style SO fill:#27ae60,color:#fff
```
