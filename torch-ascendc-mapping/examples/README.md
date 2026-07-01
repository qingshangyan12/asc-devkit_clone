# PyTorch → AscendC 中间组件示例

## 示例1: PyTorch 用户代码 (三种路径)

```python
# ================================================================
# 用户 PyTorch 脚本 — 展示三种执行路径
# ================================================================
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch_npu  # 注册 NPU 设备

class MyModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(512, 256)
        self.fc2 = nn.Linear(256, 128)

    def forward(self, x):
        x = self.fc1(x)
        x = F.relu(x)
        x = self.fc2(x)
        return x

# ─── 路径A: Eager 逐算子执行 ───
model = MyModel().to("npu")
input = torch.randn(16, 512).to("npu")
output = model(input)  # 每个 op 独立调用 aclnn API

# ─── 路径B: torch.compile + Inductor + Autofuse ───
model = MyModel().to("npu")
model = torch.compile(model, backend="inductor")
output = model(input)  # 整图编译为融合核函数

# ─── 路径C: TorchAir → GE (用于复杂模型) ───
# 通过 TorchAir 将 AtenIR 导出为 AscendIR, 交给 GE 编译
import torchair
torchair.dynamo_backend.register()
model = torch.compile(model, backend="torchair")
output = model(input)  # 走 GE 编译管线
```

## 示例2: torch_npu 算子映射机制

```cpp
// ================================================================
// torch_npu 内部: PyTorch ATen 算子 → aclnn API 映射 (简化逻辑)
// ================================================================

/**
 * PyTorch 框架会为每个 "npu" 设备注册对应的算子实现。
 * 当用户调用 torch.add(a, b) 且两个 tensor 都在 NPU 上时，
 * PyTorch dispatcher 会路由到 NPU 实现。
 *
 * 映射路径:
 *   torch.add(Tensor NPU, Tensor NPU)
 *     → aten::add (dispatch key: NPU)
 *       → torch_npu::add_kernel_npu
 *         → aclnnAddGetWorkspaceSize / aclnnAdd
 *           → [算子库实现]
 */
namespace torch_npu {

// Torch op → aclnn 的分发桥梁
at::Tensor add_kernel_npu(const at::Tensor &self, const at::Tensor &other,
                          const at::Scalar &alpha) {
    // 1. 获取 NPU stream
    aclrtStream stream = c10::npu::getCurrentNPUStream().stream();

    // 2. 将 ATen Tensor 转换为 aclTensor 描述符
    aclTensor *self_acl = at_tensor_to_acl_tensor(self);
    aclTensor *other_acl = at_tensor_to_acl_tensor(other);
    aclTensor *out_acl = nullptr;

    // 3. 获取 Workspace 大小
    uint64_t workspaceSize = 0;
    aclOpExecutor *executor = nullptr;
    aclnnAddGetWorkspaceSize(self_acl, other_acl, alpha_to_scalar(alpha),
                             out_acl, &workspaceSize, executor);

    // 4. 分配 Workspace
    void *workspace = nullptr;
    if (workspaceSize > 0) {
        aclrtMalloc(&workspace, workspaceSize, ACL_MEM_MALLOC_HUGE_FIRST);
    }

    // 5. 执行算子
    aclnnAdd(workspace, workspaceSize, executor, stream);

    // 6. 清理 & 返回
    aclrtFree(workspace);
    return acl_tensor_to_at_tensor(out_acl);
}

// TORCH_LIBRARY_IMPL 注册 NPU 实现
TORCH_LIBRARY_IMPL(aten, NPU, m) {
    m.impl("add.Tensor", torch_npu::add_kernel_npu);
    m.impl("add_.Tensor", torch_npu::add__kernel_npu);
    m.impl("matmul", torch_npu::matmul_kernel_npu);
    m.impl("relu", torch_npu::relu_kernel_npu);
    // ... 数百个算子映射
}

} // namespace torch_npu
```

## 示例3: Autofuse — AscendIR 构建 (Python 端)

```python
# ================================================================
# graph-autofusion/autofuse/compiler/python/ascir_api.py
# 功能: 构建 Ascend IR 图, 作为 CodeGen 的输入
# ================================================================

"""
Autofuse 的 Python IR API — 面向 Inductor 后端
Inductor 生成的分区建议通过此 API 转化为 AscendIR 图
"""

from autofuse import ascir

# ─── 构建一个简单的融合图: fc + bias + relu ───
def build_fused_graph(batch_size, in_dim, out_dim):
    g = ascir.HintGraph()

    # 输入 tensor: [batch_size, in_dim]
    x = ascir.Data([batch_size, in_dim], name="input_x",
                   axis=[0, 1], size=[batch_size, in_dim],
                   stride=[in_dim, 1], dtype=ascir.DType.FLOAT32)

    # 权重: [in_dim, out_dim]
    w = ascir.Data([in_dim, out_dim], name="weight",
                   dtype=ascir.DType.FLOAT32)

    # bias: [1, out_dim]
    b = ascir.Data([1, out_dim], name="bias",
                   dtype=ascir.DType.FLOAT32)

    # ─── 操作序列 ───
    # MatMul: [B, in_dim] x [in_dim, out_dim] → [B, out_dim]
    fc = ascir.MatMul(x, w)

    # Broadcast bias: [1, out_dim] → [B, out_dim]
    bias_brc = ascir.Broadcast(b, [batch_size])

    # Add: [B, out_dim] + [B, out_dim] → [B, out_dim]
    y = ascir.Add(fc, bias_brc)

    # Relu: [B, out_dim] → [B, out_dim]
    y = ascir.Relu(y)

    # 输出
    g.set_output(y)
    return g

# ─── 构建更复杂的融合: Attention 的一小部分 ───
def build_attention_subgraph(B, H, S, D):
    """
    Q * K^T / sqrt(D) + mask → softmax → dropout
    """
    g = ascir.HintGraph()

    # Q: [B, H, S, D], K: [B, H, D, S]
    q = ascir.Data([B, H, S, D], name="query")
    k = ascir.Data([B, H, D, S], name="key")

    # Q @ K^T → [B, H, S, S]
    scores = ascir.BatchMatMul(q, k)

    # Scale: scores / sqrt(D)
    scale = ascir.Div(scores, D**0.5)

    # Add mask: [B, 1, S, S] (下三角)
    mask = ascir.Data([B, 1, S, S], name="mask")
    masked = ascir.Add(scale, mask)

    # Softmax 沿最后一维
    softmax_out = ascir.Softmax(masked, axis=3)

    # Dropout (带 seed)
    do_out = ascir.Dropout(softmax_out, ratio=0.1, seed=42)

    g.set_output(do_out)
    return g
```

## 示例4: Autofuse — 代码生成 (CodeGen)

```cpp
// ================================================================
// graph-autofusion/autofuse/codegen/kernel_codegen.cpp (简化)
// 功能: 从 AscendIR 图生成 Ascend C 核函数代码
// ================================================================

/**
 * 代码生成器将 AscendIR 图转换为 Ascend C 源码:
 *
 * 输入: FusedGraph (融合算子图, 含节点和依赖关系)
 * 输出: Ascend C .cpp 源码字符串
 *
 * 生成的内容包括:
 *   1. #include 头文件
 *   2. Tiling 数据结构
 *   3. 核函数模板 (__global__ __aicore__)
 *   4. Pipeline 初始化
 *   5. 每个节点的计算代码 (Add/Mul/Sqrt/Cast...)
 *   6. 同步和内存管理代码
 */

class KernelCodeGen {
public:
    /**
     * 生成设备端 (AI Core) 核函数代码
     */
    std::string GenerateDeviceCode(const FusedGraph &graph,
                                    const ScheduleResult &schedule) {
        std::stringstream ss;

        // 1. 头文件
        ss << "#include \"kernel_operator.h\"\n";
        ss << "#include \"kernel_tpipe.h\"\n\n";

        // 2. 核函数入口
        ss << "extern \"C\" __global__ __aicore__ void "
           << graph.kernel_name() << "(\n";
        ss << "    GM_ADDR input0, GM_ADDR input1, ..., GM_ADDR output0,\n";
        ss << "    GM_ADDR workspace, GM_ADDR tiling)\n";
        ss << "{\n";

        // 3. 初始化
        ss << "    AscendC::TPipe pipe;\n";
        ss << "    auto tilingData = (TilingData*)tiling;\n\n";

        // 4. 设置 Global Tensor
        for (auto &input : graph.inputs()) {
            ss << "    AscendC::GlobalTensor<" << type_to_str(input.type)
               << "> " << input.name << "Global;\n";
            ss << "    " << input.name << "Global.SetGlobalBuffer("
               << "(__gm__ " << type_to_str(input.type) << "*)"
               << input.name << ", "
               << "tilingData->" << input.name << "Size);\n";
        }

        // 5. 遍历计算节点, 生成代码
        for (auto &node : graph.sorted_nodes()) {
            switch (node.op_type) {
                case OpType::kMatMul:
                    GenerateMatMulCode(ss, node, schedule);
                    break;
                case OpType::kAdd:
                    GenerateAddCode(ss, node);
                    break;
                case OpType::kRelu:
                    GenerateReluCode(ss, node);
                    break;
                case OpType::kSoftmax:
                    GenerateSoftmaxCode(ss, node);
                    break;
                case OpType::kReduceSum:
                    GenerateReduceCode(ss, node);
                    break;
                default:
                    GenerateGenericOpCode(ss, node);
            }
        }

        // 6. 拷贝结果回 GM
        ss << "    AscendC::DataCopy(outputGlobal, localY, "
           << "tilingData->outputSize);\n";
        ss << "    AscendC::PipeBarrier<AscendC::PIPE_ALL>();\n";
        ss << "}\n";

        return ss.str();
    }

    /**
     * 生成主机端 (Host) Tiling 代码
     */
    std::string GenerateTilingCode(const FusedGraph &graph,
                                    const ScheduleResult &schedule) {
        std::stringstream ss;

        ss << "#include \"tiling/tiling_api.h\"\n\n";

        ss << "extern \"C\" int TilingFunc(gert::TilingContext* context) {\n";
        ss << "    // 获取平台信息\n";
        ss << "    uint64_t ubSize;\n";
        ss << "    int64_t coreNum;\n";
        ss << "    GetPlatformInfo(context, ubSize, coreNum);\n\n";

        ss << "    // 计算分块策略\n";
        ss << "    auto* tiling = context->GetTilingData<TilingData>();\n";

        for (auto &tile : schedule.tiles()) {
            ss << "    tiling->" << tile.name << "BlockSize = "
               << tile.calculate(ubSize, coreNum) << ";\n";
        }

        ss << "    context->SetBlockDim(usedCoreNum);\n";
        ss << "    return ge::GRAPH_SUCCESS;\n";
        ss << "}\n";

        return ss.str();
    }

private:
    void GenerateMatMulCode(std::stringstream &ss,
                            const FusedNode &node,
                            const ScheduleResult &schedule) {
        ss << "    // MatMul: " << node.inputA << " x "
           << node.inputB << " → " << node.output << "\n";
        ss << "    AscendC::LocalTensor<"
           << type_to_str(node.output_type) << "> "
           << node.output << "Local = l0cAllocator.Alloc<"
           << type_to_str(node.output_type) << ">("
           << node.baseM * node.baseN << ");\n";

        ss << "    AscendC::Mmad(" << node.output << "Local, "
           << node.inputA << "L0, " << node.inputB << "L0, "
           << "AscendC::MmadParams{" << node.baseM << ", "
           << node.baseN << ", " << node.baseK << "});\n";

        ss << "    AscendC::SetFlag<AscendC::HardEvent::M_FIX>(EVENT);\n";
        ss << "    AscendC::WaitFlag<AscendC::HardEvent::M_FIX>(EVENT);\n";
    }

    void GenerateAddCode(std::stringstream &ss, const FusedNode &node) {
        ss << "    // Add: " << node.inputA << " + "
           << node.inputB << " → " << node.output << "\n";
        ss << "    auto " << node.output << "UB = ubAlloc.Alloc<"
           << type_to_str(node.output_type) << ">("
           << node.blockSize << ");\n";
        ss << "    AscendC::Add(" << node.output << "UB, "
           << node.inputA << "UB, " << node.inputB << "UB, "
           << node.blockSize << ");\n";
    }

    void GenerateReluCode(std::stringstream &ss, const FusedNode &node) {
        ss << "    // Relu: max(0, " << node.input << ")\n";
        ss << "    AscendC::Relu(" << node.output << "UB, "
           << node.input << "UB, " << node.blockSize << ");\n";
    }
};
```

## 示例5: Autofuse — 编译管线 (Python)

```python
# ================================================================
# graph-autofusion/autofuse/compiler/python/asc_codegen_compile.py (简化)
# 功能: 编译管线 — 从 AscendIR 到最终可执行 .so
# ================================================================

class AscendCCodeGenCompiler:

    def compile(self, graph, schedule_result, options):
        """完整的编译管线"""

        # ─── 步骤1: 生成源码 ───
        device_code = self.code_gen.device_code_generator(graph, schedule_result)
        tiling_code = self.code_gen.tiling_code_generator(graph, schedule_result)

        # 写入临时文件
        kernel_cpp = self._write_temp("kernel.cpp", device_code)
        tiling_cpp = self._write_temp("tiling.cpp", tiling_code)

        # ─── 步骤2: 编译设备核函数 ───
        kernel_o = self._compile_device_obj(kernel_cpp)
        """
        执行: bisheng kernel.cpp -o kernel.o \
              --npu-arch=ascend950 \
              -I$ASCEND_DIR/ascendc/include \
              -I$ASCEND_DIR/ascendc/impl/aicore/basic_api \
              --cce-auto-sync=off
        """

        # ─── 步骤3: 编译主机端 Tiling ───
        tiling_o = self._compile_host_obj(tiling_cpp)
        """
        执行: g++ -shared -fPIC tiling.cpp -o tiling.o \
              -I$ASCEND_DIR/include \
              -ltiling_api
        """

        # ─── 步骤4: 链接为最终 .so ───
        kernel_so = self._link_kernel(kernel_o, tiling_o)
        """
        执行: g++ -shared kernel.o tiling.o -o kernel.so \
              -L$ASCEND_DIR/lib \
              -lascendcl -lruntime -ltiling_api
        """

        # ─── 步骤5: TBE 编译 (可选, 用于 GE 集成) ───
        if options.get("use_tbe_compile"):
            self._tbe_compile(kernel_so)
            """
            通过 TBE 工具二次编译:
              compile_op(
                  op_type="AscBc",
                  inputs=[...],
                  outputs=[...],
                  kernel_bin=kernel_so
              )
            """

        # ─── 步骤6: PGO 调优 (可选) ───
        if options.get("enable_pgo"):
            self._pgo_tuning(kernel_so, graph, schedule_result)
            """
            Profile-Guided Optimization:
              1. 生成测试数据
              2. 运行多次, 收集性能数据
              3. 根据 Profile 调整 Tiling 参数
              4. 重新编译
            """

        return kernel_so

    def _compile_device_obj(self, kernel_cpp):
        """使用 bisheng 编译器编译 Ascend C 核函数"""
        # bisheng 是华为基于 LLVM 定制的 Ascend C 编译器
        cmd = [
            "$ASCEND_DIR/compiler/ccec_compiler/bin/bisheng",
            kernel_cpp, "-o", kernel_cpp.replace(".cpp", ".o"),
            "--npu-arch=ascend950",
            "-I$ASCEND_DIR/ascendc/include",
            "-I$ASCEND_DIR/ascendc/impl/aicore/basic_api",
            "--cce-auto-sync=off"
        ]
        subprocess.run(cmd, check=True)

    def _pgo_tuning(self, kernel_so, graph, schedule_result):
        """Profile-Guided Optimization"""
        best_latency = float('inf')
        best_tiling = None

        # 遍历多种 Tiling 配置
        for tiling_config in self._generate_tiling_variants(schedule_result):
            # 使用新 Tiling 重新编译
            new_so = self.compile(graph, tiling_config, {"enable_pgo": False})

            # 运行10次, 取平均值
            latencies = []
            for _ in range(10):
                lat = self._profile_run(new_so, graph)
                latencies.append(lat)

            avg_lat = sum(latencies) / len(latencies)
            if avg_lat < best_latency:
                best_latency = avg_lat
                best_tiling = tiling_config

        # 使用最佳 Tiling 最终编译
        return self.compile(graph, best_tiling, {"enable_pgo": False})
```

## 示例6: atvoss 模板库 — 声明式算子定义

```cpp
// ================================================================
// atvoss/examples/abs/abs.cpp (简化)
// 功能: 使用 atvoss 模板库定义 Abs 算子
// ================================================================

#include "atvoss.h"

template <typename DType, int64_t Width>
struct AbsConfig {
    // ─── 定义 Tile Shape (每次处理的数据量) ───
    using TileShape = Atvoss::Shape<Width, 1>;

    // ─── 定义计算表达式 (编译期展开, 零运行时开销) ───
    struct AbsCompute : Atvoss::TileEvaluator<DType, DType> {
        // 定义输出 = Abs(输入)
        ATVOSS_DECLARE_COMPUTE(out, in) {
            out = Atvoss::Abs(in);
        }
    };

    // ─── Block (单核) 策略 ───
    // 使用双缓冲模式, 2个缓冲区来回切换
    static constexpr Atvoss::BlockPolicy blockPolicy =
        Atvoss::BlockPolicy::DoubleBuffer<2>;

    // ─── Kernel (多核) 策略 ───
    // 使用 Block 并行分解
    using KernelPolicy = Atvoss::KernelPolicyBlockParallel;

    // ─── Block 构建器 ───
    using BlockBuilder = Atvoss::ElewiseBlockBuilder<
        TileShape, AbsCompute, blockPolicy, KernelPolicy>;

    // ─── Kernel 构建器 ───
    using KernelBuilder = Atvoss::ElewiseKernelBuilder<
        BlockBuilder, KernelPolicy>;

    // ─── 设备适配器 (主机端入口) ───
    using DeviceAdapter = Atvoss::ElewiseDeviceAdapter<KernelBuilder>;
};

// ─── 主机端使用 ───
template <typename T>
void run_abs(const std::vector<T> &input, std::vector<T> &output) {
    constexpr int64_t WIDTH = 1024;
    using Config = AbsConfig<T, WIDTH>;

    // 1. ACL 初始化
    aclrtSetDevice(0);
    aclrtStream stream;
    aclrtCreateStream(&stream);

    // 2. 分配内存 & 拷贝数据
    size_t byteSize = input.size() * sizeof(T);
    T *d_input, *d_output;
    aclrtMalloc((void**)&d_input, byteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc((void**)&d_output, byteSize, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(d_input, byteSize, input.data(), byteSize,
                ACL_MEMCPY_HOST_TO_DEVICE);

    // 3. 设置参数并运行 (atvoss 自动处理分块和多核调度)
    auto args = Atvoss::MakeArgs(
        Atvoss::ar<0>(d_input, input.size()),     // 输入
        Atvoss::ar<1>(d_output, output.size())    // 输出
    );
    Config::DeviceAdapter::Run(args, stream);

    // 4. 拷贝结果回主机
    aclrtMemcpy(output.data(), byteSize, d_output, byteSize,
                ACL_MEMCPY_DEVICE_TO_HOST);

    // 5. 清理
    aclrtFree(d_input); aclrtFree(d_output);
    aclrtDestroyStream(stream);
}
```

## 示例7: atvoss — RMS Norm 复合算子

```cpp
// ================================================================
// atvoss/examples/rms_norm/rms_norm.cpp (简化)
// 功能: 使用 atvoss 声明式定义 RMS Normalization
// ================================================================

#include "atvoss.h"

/**
 * RMS Norm 计算公式:
 *   out = x2 * (x1 / sqrt(mean(x1^2) + eps))
 *
 * 完整的算子只需一个表达式, atvoss 自动展开为:
 *   1. Square(x1) → tmp1
 *   2. ReduceMean(tmp1) → tmp2
 *   3. Add(tmp2, eps) → tmp3
 *   4. Sqrt(tmp3) → tmp4
 *   5. Div(x1, tmp4) → tmp5
 *   6. Mul(x2, tmp5) → out
 *
 * 每次操作自动插入: Load→UB→Compute→Sync→Store
 */

template <typename DType, int64_t Width, int64_t ReduceAxis>
struct RmsNormConfig {
    // Tile 的 Shape: [Width/ReduceAxis, ReduceAxis]
    using TileShape = Atvoss::Shape<Width / ReduceAxis, ReduceAxis>;
    // 标量中间类型: 通常是 float
    using ScalarDtype = typename Atvoss::Promote<DType>::Float;

    struct RmsNormCompute : Atvoss::TileEvaluator<DType, DType, DType, ScalarDtype> {
        // 输入: in1, in2, eps → 输出: out
        ATVOSS_DECLARE_COMPUTE(out, in1, in2, eps) {
            // 语法与数学表达式完全一致:
            // out = in2 * (in1 / sqrt(mean(in1^2) + eps))
            out = in2 * (in1 / Atvoss::Sqrt(
                Atvoss::Mean<ReduceAxis>(in1 * in1) + eps
            ));
        }
    };

    // 编译期自动展开:
    //   Step 1: square = in1 * in1                    → Mul
    //   Step 2: mean   = Mean<Axis>(square)           → ReduceSum + Div
    //   Step 3: var    = mean + eps                   → Add (标量)
    //   Step 4: rsqrt  = Sqrt(var)                    → Sqrt
    //   Step 5: norm   = in1 / rsqrt                  → Div
    //   Step 6: out    = in2 * norm                   → Mul

    using BlockPolicy = Atvoss::BlockPolicy::DoubleBuffer<2>;
    using KernelPolicy = Atvoss::KernelPolicyBlockParallel;

    using BlockBuilder = Atvoss::ElewiseBlockBuilder<
        TileShape, RmsNormCompute, BlockPolicy, KernelPolicy>;
    using KernelBuilder = Atvoss::ElewiseKernelBuilder<
        BlockBuilder, KernelPolicy>;
    using DeviceAdapter = Atvoss::ElewiseDeviceAdapter<KernelBuilder>;
};
```

## 示例8: Ascend C — MatMul + LeakyReLU 融合 (完整核函数)

```cpp
// ================================================================
// asc-devkit/examples/03_fusion_operation/matmul_leakyrelu_advanced_api/
// 功能: 融合 MatMul + Bias + LeakyReLU 为一个核函数
// ================================================================

#include "kernel_operator.h"
#include "tiling/tiling_api.h"

/**
 * 融合算子: C = LeakyReLU(A @ B + Bias, alpha=0.001)
 *
 * 好处:
 *   - 避免中间结果 (A@B+Bias) 写回 HBM
 *   - MatMul 和 LeakyReLU 共用 AI Core 流水线
 *   - 减少带宽占用和延迟
 *
 * 使用 Tpipe API (Queue 模式):
 *   自动管理 Buffer 分配、同步、释放
 *   Vector 单元和 Cube 单元通过 Queue 通信
 */

// __mix__(1, 2) 表示: 1个Vector核 + 2个Cube核混合编程
__global__ __mix__(1, 2) void matmul_leakyrelu_custom(
    __gm__ uint8_t* a,            // 矩阵A (half) [M, K]
    __gm__ uint8_t* b,            // 矩阵B (half) [K, N]
    __gm__ uint8_t* bias,         // 偏置 (float) [N]
    __gm__ uint8_t* c,            // 输出 (float) [M, N]
    __kfc_workspace__ __gm__ uint8_t* workspace,
    AscendC::tiling::TCubeTiling tiling) // Tiling 参数
{
    // ─── 1. 创建 Pipeline ───
    AscendC::TPipe pipe;

    // ─── 2. 设置 Global Tensor (HBM 上的数据) ───
    AscendC::GlobalTensor<half> aGlobal;
    AscendC::GlobalTensor<half> bGlobal;
    AscendC::GlobalTensor<float> cGlobal;
    AscendC::GlobalTensor<float> biasGlobal;

    aGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(a),
                            tiling.M * tiling.Ka);
    bGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ half*>(b),
                            tiling.Kb * tiling.N);
    cGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(c),
                            tiling.M * tiling.N);
    biasGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(bias),
                               tiling.N);

    // ─── 3. 多核拆分 (按 N 方向拆分) ───
    uint32_t blockIdx = AscendC::GetBlockIdx();
    uint32_t nCoreOffset = blockIdx * tiling.singleCoreN;
    bGlobal = bGlobal[nCoreOffset];
    biasGlobal = biasGlobal[nCoreOffset];
    cGlobal = cGlobal[nCoreOffset];

    // ─── 4. 初始化 MatMul 对象 (Cube 单元) ───
    // VECIN: MatMul输出先放到 Vector 输入 Queue
    // VECOUT: LeakyReLU输出先放到 Vector 输出 Queue
    AscendC::TQue<AscendC::TPosition::VECIN, 1> mmOutQueue;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> reluOutQueue;

    matmul::Matmul<
        matmul::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, half>,
        matmul::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, half>,
        matmul::MatmulType<AscendC::TPosition::VECIN, CubeFormat::ND, float>,
        matmul::MatmulType<AscendC::TPosition::GM, CubeFormat::ND, float>
    > matmulObj;

    REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), matmulObj, &tiling);
    matmulObj.SetTensorA(aGlobal);
    matmulObj.SetTensorB(bGlobal);
    matmulObj.SetBias(biasGlobal);

    // ─── 5. 初始化 Vector 端 Queue ───
    pipe.InitBuffer(mmOutQueue, 1,
                    tiling.baseM * tiling.baseN * sizeof(float));
    pipe.InitBuffer(reluOutQueue, 1,
                    tiling.baseM * tiling.baseN * sizeof(float));

    // ─── 6. 主循环: 分块计算 + 融合 ───
    uint32_t computeRound = 0;
    while (matmulObj.template Iterate<true>()) {
        // 6a. Cube: MatMul 计算一个 block (硬件自动执行)
        //     同时 Vector: 等待上一轮数据

        // 6b. 获取 MatMul 的输出 (从 VECIN Queue)
        AscendC::LocalTensor<float> mmOutLocal =
            mmOutQueue.AllocTensor<float>();
        matmulObj.template GetTensorC<true>(mmOutLocal, false, true);
        mmOutQueue.EnQue(mmOutLocal);
        mmOutLocal = mmOutQueue.DeQue<float>();

        // 6c. Vector: LeakyReLU 计算 (与 Cube: 下一轮 MatMul 并行)
        AscendC::LocalTensor<float> reluOutLocal =
            reluOutQueue.AllocTensor<float>();
        AscendC::LeakyRelu(reluOutLocal, mmOutLocal,
                           static_cast<float>(0.001),
                           tiling.baseM * tiling.baseN);
        reluOutQueue.EnQue(reluOutLocal);
        reluOutLocal = reluOutQueue.DeQue<float>();

        // 6d. 释放已完成 buffer
        mmOutQueue.FreeTensor(mmOutLocal);

        // 6e. 拷贝结果到 HBM
        uint64_t cOffset = static_cast<uint64_t>(computeRound)
                           * tiling.baseM * tiling.N;
        AscendC::DataCopy(cGlobal[cOffset], reluOutLocal,
            AscendC::DataCopyParams{
                static_cast<uint16_t>(tiling.baseM),
                static_cast<uint16_t>(tiling.baseN * sizeof(float) / 32),
                0,
                static_cast<uint16_t>(
                    (tiling.N - tiling.baseN) * sizeof(float) / 32)
            });

        reluOutQueue.FreeTensor(reluOutLocal);
        computeRound++;
    }

    matmulObj.End();
}
```

## 示例9: PyTorch Extension 构建

```cmake
# ================================================================
# ops-math/cmake/torch_extension.cmake (简化)
# 功能: 构建 PyTorch C++ 扩展, 将 AscendC 算子注册为 torch op
# ================================================================

# 链接库
set(TORCH_EXTENSION_LIBS
    torch_npu
    ascendcl        # ACL 库
    platform        # 平台抽象库
    register        # 算子注册库
    tiling_api      # Tiling API
    runtime         # 运行时
)

# 包含路径
set(TORCH_EXTENSION_INCLUDE_DIRS
    ${ASCEND_DIR}/ascendc/include
    ${ASCEND_DIR}/ascendc/impl/aicore/basic_api
    ${ASCEND_DIR}/include
    ${PROJECT_SOURCE_DIR}/math
    ${PROJECT_SOURCE_DIR}/common/inc
)

# 编译器
set(CMAKE_CXX_COMPILER ${ASCEND_DIR}/compiler/ccec_compiler/bin/bisheng)

# 构建主目标: _C.abi3.so
add_library(_C SHARED
    ${OP_SOURCES}          # 所有算子的 op_api/op_host 源码
    init.cpp               # PyInit 入口
)

target_link_libraries(_C ${TORCH_EXTENSION_LIBS})

# Python setup.py 调用
# setup(
#     name="npu_math_extension",
#     ext_modules=[CMakeExtension("npu_math_extension._C")],
#     cmdclass={"build_ext": CMakeBuild}
# )
```
