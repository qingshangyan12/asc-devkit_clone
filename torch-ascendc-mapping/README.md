# PyTorch → AscendC 映射层次结构与原理

## 一、整体架构总览

```
┌──────────────────────────────────────────────────────────────────────┐
│ 用户层                                                                │
│   PyTorch Python (torch.nn.Conv2d, torch.matmul, F.relu, ...)        │
├──────────────────────────────────────────────────────────────────────┤
│ PyTorch 后端适配层                                                    │
│   torch_npu 适配器 / TorchAir (AtenIR → AscendIR)                    │
├──────────────────────────────────────────────────────────────────────┤
│ 三种执行路径                                                          │
│                                                                       │
│  A. Eager 模式 (逐算子)                  B. TorchDynamo + Inductor     │
│  torch op → aclnn API                   torch.compile(FX Graph)       │
│    → op_host (Tiling)                     → TorchAir (→ AscendIR)     │
│    → op_kernel (AscendC)                  → Autofuse (codegen)        │
│    → runtime                              → Ascend C kernel           │
│                                                                       │
│  C. TorchAir + GE (图编译)                                           │
│  AtenIR → GE Parser → GE Compiler → OM → Runtime                     │
├──────────────────────────────────────────────────────────────────────┤
│ GE (Graph Engine) 图引擎层                                            │
│   ONNX/AtenIR Parser → Compiler → Optimizer → Build → Executor       │
├──────────────────────────────────────────────────────────────────────┤
│ Autofuse 自动融合层 (graph-autofusion)                                │
│   AscendIR Codegen → 自动合成 Ascend C 融合核函数                      │
├──────────────────────────────────────────────────────────────────────┤
│ 算子库层 (Operator Library)                                          │
│   ops-math / ops-nn / ops-transformer / ops-cv                       │
│   ┌──────────────┬──────────────────┬─────────────────┐             │
│   │ op_graph     │ op_host          │ op_kernel       │ op_api      │
│   │ (图集成)     │ (Tiling/InferShape│ (AscendC核函数) │ (aclnn API) │
│   └──────────────┴──────────────────┴─────────────────┘             │
├──────────────────────────────────────────────────────────────────────┤
│ Ascend C 核函数编程层                                                 │
│   asc-devkit (C API / Base API / Tpipe API / High-level API)         │
│   atvoss (向量模板库: 表达式模板, 零开销)                               │
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

## 二、PyTorch 特有的三种执行路径

与 TensorFlow 不同，PyTorch 是**动态图**框架，在 Ascend 上有三条可选的执行路径:

### 路径 A: Eager 逐算子模式

```
用户写 torch.add(a, b)
    ↓
torch_npu 捕获该 op 调用
    ↓
aclnnAdd(a, b)  ← aclnn 单算子 API
    ↓
op_host/add_tiling.cpp        ← 计算 Tiling 参数
op_kernel/add.cpp             ← Ascend C 核函数
    ↓
aclrtLaunchKernel → AI Core

特点: 开发友好, 灵活, 但逐算子调度开销大
```

### 路径 B: torch.compile + Inductor + Autofuse

```
用户写 torch.compile(model)(input)
    ↓
TorchDynamo 捕获 FX Graph
    ↓
TorchInductor 接收到算子列表
    ↓ (通过 inductor-npu-ext 后端)
graph-autofusion/autofuse/
    ↓
ascir (Ascend IR 构建) → Schedule (优化调度) → CodeGen (生成 Ascend C)
    ↓
bisheng 编译器编译 → .so (融合核函数)
    ↓
Runtime 执行

特点: graph 级别优化, 自动融合, 接近 GE 的优化效果
```

### 路径 C: TorchAir + GE 图编译

```
TorchAir 将 AtenIR 转换为 AscendIR
    ↓
GE ONNX Parser (或 TorchAir 直接构造 AscendIR)
    ↓
GE Compiler (Prepare → Optimize → Partition → Build)
    ↓
OM model → GE Runtime v1/v2
    ↓
Runtime 执行

特点: 完整的 GE 优化管线, 支持离线编译
```

## 三、层次结构详解

### 第 0 层: PyTorch 用户代码

```python
import torch
import torch.nn as nn

class SimpleModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv = nn.Conv2d(3, 64, 3, padding=1)
        self.bn = nn.BatchNorm2d(64)
        self.relu = nn.ReLU()
        self.fc = nn.Linear(64 * 32 * 32, 1000)

    def forward(self, x):
        x = self.conv(x)
        x = self.bn(x)
        x = self.relu(x)
        x = torch.flatten(x, 1)
        x = self.fc(x)
        return x

# 使用 torch.compile 启用图优化
model = SimpleModel().to("npu")
model = torch.compile(model, backend="inductor")
output = model(torch.randn(1, 3, 256, 256).to("npu"))
```

### 第 1 层: torch_npu 后端适配

`torch_npu` 是 PyTorch 在 Ascend NPU 上的设备后端:

```
torch_npu 核心职责:
  ├─ 设备注册: torch.device("npu")
  ├─ 算子分发: 将 ATen IR 算子映射到 aclnn API
  ├─ 内存管理: NPU 设备内存分配器 (基于 ACL Malloc/Free)
  ├─ Stream 管理: NPU Stream 创建/同步 (基于 ACL Stream)
  └─ torch.compile 后端: 对接 TorchInductor + Autofuse
```

**算子分发到 aclnn API**:

```cpp
// torch_npu 内部映射逻辑 (简化)
ATen Add::NPU(const Tensor &self, const Tensor &other) {
    // 1. 创建 aclnn Tensor 描述符
    // 2. 调用 aclnnAddGetWorkspaceSize / aclnnAdd
    // 3. 返回输出 Tensor
}
```

### 第 2 层: TorchAir — AtenIR → AscendIR 转换

TorchAir 是连接 PyTorch IR 与 GE 的桥梁:

```
PyTorch FX Graph (AtenIR)
    ↓
TorchAir lowering
    ↓
AscendIR (ComputeGraph)
    ↓
GE Compiler
    ↓
OM → Runtime → NPU
```

**关键接口** (`ge/api/atc/`):

| 接口 | 功能 |
|------|------|
| `atc` CLI | 离线编译: ONNX/PB → OM 模型 |
| `pyatc` | Python 绑定: 在线编译 |
| `ge::Session` | 运行时图管理 |

### 第 3 层: Autofuse — 自动融合编译器

这是 PyTorch + Inductor 路径的核心优化组件 (`graph-autofusion/autofuse/`):

```
┌──────────────────────────────────────────────────────────┐
│ Autofuse 编译管线                                         │
│                                                          │
│ Ascend IR (HintGraph/FusedGraph)                         │
│     │                                                    │
│     ├─ Compiler (py_module/pyautofuse.cpp)              │
│     │     │                                              │
│     │     ├─ Schedule (optimize/autoschedule.cpp)       │
│     │     │   ├─ 自动 Tiling                            │
│     │     │   ├─ Buffer 分配                            │
│     │     │   └─ Task 并行化调度                         │
│     │     │                                              │
│     │     └─ CodeGen (codegen/)                          │
│     │         ├─ Kernel codegen       → Ascend C .cpp   │
│     │         ├─ Tiling data codegen  → Tiling .cpp     │
│     │         └─ Expression conversion → DAG 表达式     │
│     │                                                    │
│     └─ Python Bindings (compiler/python/)               │
│           ├─ ascir_api.py      (AscendIR 构造)          │
│           ├─ asc_codegen_compile.py (编译管线)           │
│           └─ ascendc_compile.py (bisheng 编译器驱动)     │
│                                                          │
│ 产出: Ascend C 融合核函数 .so + Tiling .so               │
└──────────────────────────────────────────────────────────┘
```

**Python API — Ascend IR 构造** (`ascir_api.py`):

```python
from autofuse import ascir

g = ascir.HintGraph()
x = ascir.Data([16, 512])          # 输入
w = ascir.Data([512, 256])         # 权重
b = ascir.Data([1, 256])           # bias

fc = ascir.MatMul(x, w)            # 矩阵乘
bias = ascir.Broadcast(b, [16])
y = ascir.Add(fc, bias)            # 加 bias
y = ascir.Relu(y)                  # 激活

g.set_output(y)
```

### 第 4 层: 算子库 (与 TF 共享)

与 TensorFlow 路径完全共享同一套算子库，见 `tf-ascendc-mapping/README.md` 第 4 层。

### 第 5 层: Ascend C 编程框架

同样共享 `asc-devkit` 和 `atvoss`。

| 编程层次 | 适用场景 | 示例 |
|----------|----------|------|
| C API | 极简场景, C-only 项目 | `asc_add(x, y, z, length)` |
| Base API | 手动优化, 精细控制 | `MakeTensor/SetGlobalBuffer/LocalMemoryAllocator` |
| Tpipe/Tque API | 算子开发, 自动管理 | `pipe.InitBuffer/AllocTensor/EnQue/DeQue` |
| High-level API | 快速开发, 内置算法 | `Matmul::SetTensorA/Iterate/GetTensorC` |
| SIMT API | CUDA-like 编程 | `warp.reduce_sum/shuffle_xor` |
| atvoss 模板库 | 向量融合算子 | `out = in * scalar`, 表达式模板零开销 |

### 第 6 层: metadef (共享)

与 TF 路径共享，见 `tf-ascendc-mapping/README.md` 第 6 层。

### 第 7 层: Runtime (共享)

与 TF 路径共享，见 `tf-ascendc-mapping/README.md` 第 7 层。

## 四、PyTorch 特有的中间组件

### torch_extension — PyTorch C++ 扩展编译

`ops-math/cmake/torch_extension.cmake` 提供 PyTorch 扩展构建:

```cmake
# 构建目标: _C.abi3.so (Python C 扩展)
# 链接: torch_npu, ascendcl, platform, register, tiling_api, runtime
# 编译器: bisheng (Ascend C 专用编译器)
# 包含路径: ascendc/include/basic_api, ascendc/include/highlevel_api
```

### npu_math_extension — PyTorch Extension 包

`ops-math/scripts/torch_extension/`:

```
npu_math_extension/
├── setup.py           # CMakeBuild 驱动
├── _C.cpp             # PyInit__C (空模块, 算子通过 TORCH_LIBRARY 注册)
└── __init__.py        # 导入 _C 扩展
```

### inductor-npu-ext — Inductor NPU 后端

将 Autofuse 作为 TorchInductor 的编译后端:

```
torch.compile(model, backend="inductor")
    → Inductor 生成融合建议
    → inductor-npu-ext 转换为 AscendIR
    → Autofuse Schedule + CodeGen
    → Ascend C 核函数编译
```

## 五、Graph-Autofusion: 自动融合深度分析

> **graph-autofusion** 是 PyTorch + Inductor 路径的**核心性能优化组件**。
> 通过对图的分析，将多个用户级算子融合为单个 Ascend C 核函数，
> 在 device 端同一块物理内存 (UB) 上连续计算，消除中间数据的 HBM 搬运开销。
> 
> **详见**:
> - [graph-autofusion-analysis.md](graph-autofusion-analysis.md) — 自动融合原理、7步管线、模板选择原则、SuperKernel
> - **[lowering-schedule-codegen-pipeline.md](lowering-schedule-codegen-pipeline.md) — Lowering / Symbolization / Schedule / CodeGen / ATT 五模块完整流水线**

### 5.1 Autofuse 核心流程

```
PyTorch FX Graph (Inductor 融合建议)
    │
    ▼
[1] 算子分类: Elewise / Broadcast / Reduce / MatMul / Concat / ...
    │
    ▼
[2] 依赖分析 + 轴合并: 构建融合链, 合并连续轴减少循环嵌套
    │
    ▼
[3] 图分区 (ScheduleGroupPartitioner): 按算子类型划分子图组
    │
    ▼
[4] 自动调度 (AutoSchedule): 
    - TilingCase 生成: BlockOuter/TileOuter/TileInner/Original
    - Cube融合模式选择: kFixpip/kUBFuse/kCommon
    - Reduce模板选择: kAllLoad/kCommon/kRCore
    - NDDMA动态打分 (运行时选最优)
    │
    ▼
[5] 代码生成 (CodeGen):
    - ApiCall 工厂: 图节点→Ascend C API 映射
    - 循环树构建: Loop::ConstructFromNodes()
    - 表达式转换: expression_convert.cpp
    - 生成三阶段代码: PreProcess→ApiCall→PostProcess
    │
    ▼
[6] 编译链接: bisheng → .o → .so + .json

输入: HintGraph/FusedGraph (Ascend IR)
输出: lib{kernel}.so + kernel_meta/*.json
```

### 5.2 融合原理: 消除数据搬运

```
融合前: 3个算子 = 6次HBM总线事务
  Load(x)→Add→Store(y)  Load(y)→Mul→Store(z)  Load(z)→Relu→Store(out)

融合后: 1个融合核 = 2次HBM总线事务 (减少67%)
  Load(x)→[Add→Mul→Relu](UB原地)→Store(out)
```

### 5.3 CodeGen: ApiCall 工厂映射

```
图节点       → ApiCall子类        → 生成的AscendC指令
Add/Mul/Div  → BinaryApiCall      → Add(z, x, y, size)
Relu/Exp/Sqrt→ UnaryApiCall       → Relu(y, x, size)
ReduceSum    → ReduceApiCall      → ReduceSum(y, x, axis)
Broadcast    → BroadcastApiCall   → Broadcast(y, x, size)
MatMul       → (Cube调度器)        → Mmad/LoadData/Fixpipe
Cast         → CastApiCall        → Cast(y, x, size)
Load/Store   → LoadApiCall        → DataCopy(local, global, size)
```

每个 ApiCall = PreProcess(同步+分配) + ApiCall(计算) + PostProcess(释放+标记)

### 5.4 模板选择原则

| 维度 | 选项 | 条件 |
|------|------|------|
| 执行单元 | Vector-only / CV Fusion | 是否含MatMul/Conv |
| Cube融合 | kFixpip / kUBFuse / kCommon | 后续能否操作Cube输出格式 |
| Reduce | kAllLoad / kCommon / kRCore | 数据规模+多核需求 |
| 双缓冲 | 启用/不启用 | UB容量+计算可掩盖搬运 |

### 5.5 SuperKernel: 运行时全图融合

与 Autofuse (编译期源码融合) 互补:
- sk_entry.asc 统一调度循环 → 消除 N 次 Launch 开销
- 子Kernel 4副本拆分 → 缓解 L2 Cache 争用
- ICache Preload → dc_preload() 预取指令
- Early-Start → MTE搬运与标量初始化并发

Autofuse 解决 **Memory-Bound** | SuperKernel 解决 **Launch-Bound**

> 完整分析见 [graph-autofusion-analysis.md](graph-autofusion-analysis.md)

---

## 六、关键优化技术

| 优化技术 | 实现位置 | 说明 |
|----------|----------|------|
| **Autofuse (自动融合)** | **`graph-autofusion/autofuse/codegen/`** | **算子分类 + CodeGen合成, 消除HBM搬运** |
| **SuperKernel (全图融合)** | **`graph-autofusion/super_kernel/`** | **TaskQue调度, 消除Launch开销** |
| 算子自动调度 | `graph-autofusion/autofuse/optimize/autoschedule.cpp` | 自动 Tiling, Buffer 分配, 并行化 |
| 表达式模板 (零开销) | `atvoss/expression/` | 编译期展开, 无运行时虚函数开销 |
| PGO (Profile-Guided) | `graph-autofusion/autofuse/compiler/python/` | 运行时反馈优化 Tiling |
| ICache Preload | `graph-autofusion/super_kernel/` | 预取指令, 减少取指延迟 |
| Early-Start | `graph-autofusion/super_kernel/` | MTE+Scalar 并发 |
| 内存复用 | `graph-autofusion/autofuse/optimize/buf_que_allocator.cpp` | 生命周期复用UB |
| 流并行 | `ge/compiler/` StreamAllocator | 多引擎并行执行 |

> **完整分析**见 [graph-autofusion-analysis.md](graph-autofusion-analysis.md)。

## 七、PyTorch 路径与 TF 路径对比

| 维度 | PyTorch | TensorFlow |
|------|---------|------------|
| 图模式 | 动态图 (Eager 优先) | 静态图 (GraphDef) |
| 适配方式 | torch_npu + TorchAir + Autofuse | TF Adapter + GE Parser |
| 融合方式 | Autofuse 自动合成 + atvoss 模板 | 手写 Pattern + Autofuse |
| 算子库 | 共享 ops-* 算子库 | 共享 ops-* 算子库 |
| Offline编译 | atc (ONNX) | atc (PB) |
| Inductor 集成 | inductor-npu-ext 后端 | 不适用 |

## 八、中间组件示例

详见 `examples/` 目录下的代码示例。
