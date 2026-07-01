# TensorFlow → AscendC 中间组件示例

## 示例1: TF Python 用户代码

```python
# tf_user_script.py — 用户编写的 TensorFlow 代码
import tensorflow as tf
import numpy as np

def create_model():
    """定义一个简单的 CNN 模型"""
    x = tf.placeholder(tf.float32, shape=[None, 28, 28, 1], name='input')
    w1 = tf.Variable(tf.truncated_normal([5, 5, 1, 32], stddev=0.1), name='w1')
    b1 = tf.Variable(tf.constant(0.1, shape=[32]), name='b1')
    conv1 = tf.nn.conv2d(x, w1, strides=[1, 1, 1, 1], padding='SAME')
    relu1 = tf.nn.relu(tf.nn.bias_add(conv1, b1))
    pool1 = tf.nn.max_pool(relu1, ksize=[1, 2, 2, 1],
                           strides=[1, 2, 2, 1], padding='SAME')
    fc = tf.layers.flatten(pool1)
    logits = tf.layers.dense(fc, 10)
    return x, logits

# 在 Ascend NPU 上的执行路径:
# 1. 上述 Python 代码经 convert_tf2npu 工具转换后添加 NPU 配置
# 2. 或通过手动配置 NPU session config
# 3. TF 图在运行时被 GeOp 捕获并转换为 AscendIR
```

## 示例2: TF Adapter — GeOp 桥接 (关键代码摘录)

```cpp
// ================================================================
// 文件: tensorflow/tf_adapter/kernels/geop_npu.cc
// 功能: TF → GE 的核心桥接类
// 位置于 CANN 仓库的 tensorflow/tf_adapter/ 中
// ================================================================

/**
 * GeOp 是 TF op → GE 引擎的桥梁:
 * - 作为 AsyncOpKernel 被 TF executor 调用
 * - 将 TF 子图转换为 GE 可编译执行的 AscendIR
 * - 管理编译缓存和生命周期
 */
class GeOp : public AsyncOpKernel {
public:
  explicit GeOp(OpKernelConstruction *ctx);

  // ─── 核心入口: TF executor 调用 ───
  void ComputeAsync(OpKernelContext *ctx, DoneCallback done) override {
    // 1. 验证初始化状态
    // 2. 获取 TF session 句柄
    // 3. 配置 profiling / AOE 调优参数
    // 4. 固定输入为常量
    // 5. BuildInputTensorInfo() — TF Tensor → ge::Tensor
    // 6. InitGraphShape(ctx)
    // 7. CompileAndRunGraph() — 编译+执行
  }

  // ─── 图构建: TF FunctionDef → GraphDef proto ───
  Status BuildGraphDef(
      FunctionLibraryDefinition &flib_def,
      const std::vector<Tensor> &input_vec,
      GraphDef &graph_def,
      bool &is_initialize,
      bool &is_allreduce)
  {
    const FunctionDef *function_def = flib_def.Find(function_.name());

    // 推断 Shape
    Graph graph(OpRegistry::Global());
    InferShapeUtil::InferShape(input_vec, &flib_def, function_def, &graph);

    // 遍历所有节点, 添加输入/输出描述符
    ProcessForDiffNodeTypes(graph, is_initialize, is_allreduce);

    // 序列化为 GraphDef proto
    graph.ToGraphDef(&graph_def);
  }

  // ─── 编译: GraphDef → GE Compile ───
  Status CompileGraph(
      OpKernelContext *ctx,
      const std::vector<Tensor> &input_vec,
      const std::vector<ge::Tensor> &inputs,
      const uint32_t &graph_id)
  {
    // 调用 GE 解析器: GraphDef proto → ComputeGraph (AscendIR)
    DoGraphParser(proto_data);

    // 注册到 GE Session 并触发编译
    ge_session_->AddGraph(graph_id, compute_graph);
    ge_session_->BuildGraph(graph_id);  // 编译管线: Prepare→Optimize→Partition→Build
  }

  // ─── 执行: GE Run ───
  Status RunGraph(
      OpKernelContext *ctx,
      const uint32_t &graph_id,
      const std::shared_ptr<std::vector<ge::Tensor>> &inputs,
      DoneCallback done)
  {
    ge_session_->RunGraphAsync(graph_id, *inputs, outputs,
        [ctx, done](ge::Status status, std::vector<ge::Tensor> &output) {
            // 将 GE 输出 Tensor 转换回 TF Tensor
            BuildOutputTensorInfo(ctx, output);
            done();  // 通知 TF executor 完成
        });
  }

private:
  ge::Session *ge_session_;                          // GE Session 句柄
  std::string lib_name_;                             // 算子.so库名
  std::string op_name_;                              // 算子名
  NameAttrList function_;                            // TF 子图函数
  std::map<std::vector<std::string>, uint32_t> cache_graphs_; // 编译缓存
  // ... profiling / AOE 配置
};
```

## 示例3: GE TF Parser — GraphDef → AscendIR 转换

```cpp
// ================================================================
// 文件: ge/parser/parser/tensorflow/tensorflow_parser.cc
// 功能: TF GraphDef proto → GE ComputeGraph (AscendIR)
// ================================================================

/**
 * 入口: 将 TF proto 字节流解析为 GE 计算图
 *
 * 流程:
 *   1. 反序列化 GraphDef proto
 *   2. 遍历每个 NodeDef
 *   3. OpParserFactory 分派到对应解析器
 *   4. 自动映射属性 (OperatorAutoMapping)
 *   5. 建立节点间 Anchor 连接
 */
graphStatus aclgrphParseTensorFlow(
    const uint8_t *proto_data,
    uint32_t proto_size,
    ge::Graph &graph)
{
    // 反序列化 TF GraphDef
    tensorflow::GraphDef graph_def;
    graph_def.ParseFromArray(proto_data, proto_size);

    // 创建目标 GE Graph
    ComputeGraphPtr compute_graph = MakeShared<ComputeGraph>(graph_name);

    // 遍历 TF 算子节点
    for (const auto &node_def : graph_def.node()) {
        // 通过工厂分发到注册的解析器
        // DOMI_REGISTER_TENSORFLOW_PARSER(OpType)
        auto parser = OpParserFactory::Instance()
            ->CreateParser(node_def.op());

        if (parser != nullptr) {
            // 自定义解析器
            parser->Parse(node_def, compute_graph);
        } else {
            // 自动映射: TF NodeDef.attr → GE Operator.attr
            OperatorAutoMapping(node_def, compute_graph);
        }
    }

    // 建立节点连接 (Anchor)
    BuildGraphConnections(graph_def, compute_graph);

    return GRAPH_SUCCESS;
}
```

## 示例4: 算子定义 — op_host 层 (REG_OP 注册)

```cpp
// ================================================================
// 文件: ops-math/math/add/op_host/add_def.cpp
// 功能: 注册 Add 算子到 CANN 系统
// ================================================================

#include "op_def/op_def.h"

namespace ops {

/**
 * Add 算子定义
 *
 * 该定义告诉 GE:
 *   - 输入/输出的名称、类型约束、格式约束
 *   - 支持哪些芯片 (ascend910b, ascend950 等)
 *   - 哪些属性支持动态编译
 */
class Add : public OpDef {
public:
    explicit Add(const char* name) : OpDef(name) {
        this->Input("x1")
            .ParamType(REQUIRED)
            .DataType({
                ge::DT_FLOAT, ge::DT_FLOAT16, ge::DT_BF16,
                ge::DT_INT32, ge::DT_INT8, ge::DT_BOOL,
                ge::DT_UINT8, ge::DT_INT64, ge::DT_DOUBLE
            })
            .Format({ge::FORMAT_ND, ge::FORMAT_NCHW, ge::FORMAT_NHWC});

        this->Input("x2")
            .ParamType(REQUIRED)
            .DataType({/* 同上 */})
            .Format({ge::FORMAT_ND, ge::FORMAT_NCHW, ge::FORMAT_NHWC});

        this->Output("y")
            .ParamType(REQUIRED)
            .DataType({/* 向上转型兼容所有输入类型 */})
            .Format({ge::FORMAT_ND, ge::FORMAT_NCHW, ge::FORMAT_NHWC});

        // 芯片配置
        OpAICoreConfig aicoreConfig;
        aicoreConfig.DynamicCompileStaticFlag(true)
            .DynamicRankSupportFlag(true)
            .DynamicShapeSupportFlag(true);
        this->AICore().AddConfig("ascend950", aicoreConfig);
    }
};

// 全局注册
OP_ADD(Add);

} // namespace ops
```

## 示例5: Ascend C 核函数 — op_kernel 层

```cpp
// ================================================================
// 文件: ops-math/math/add/op_kernel/arch35/add.cpp
// 功能: Add 算子的 AI Core 核函数实现 (Ascend C)
// ================================================================

#include "kernel_operator.h"    // Ascend C 框架头文件
#include "add_dag.h"            // 计算 DAG 定义
#include "add_struct.h"         // 模板参数结构

using namespace AscendC;

/**
 * Add 核函数入口
 *
 * @tparam  schMode   调度模式 (通过 Tiling 编译期选择)
 * @param   x1, x2    输入全局内存指针 (HBM/GM)
 * @param   y         输出全局内存指针
 * @param   workspace 工作区内存
 * @param   tiling    Tiling 数据 (分块策略参数)
 *
 * __global__ __aicore__ : 标记此函数在 AI Core 上执行
 */
template <uint64_t schMode>
__global__ __aicore__ void add(
    GM_ADDR x1,         // 全局内存地址输入1
    GM_ADDR x2,         // 全局内存地址输入2
    GM_ADDR y,          // 全局内存地址输出
    GM_ADDR workspace,  // 工作空间
    GM_ADDR tiling)     // Tiling 参数
{
    // 使用广播调度器, 自动执行:
    //   CopyInBrc(x1) → Add(x1, x2) → CopyOut(y)
    BroadcastSch<schMode, OpDag> sch(tiling);
    sch.Process(x1, x2, y);
}


// 文件: add_dag.h — 计算 DAG 定义 (数据流图)
// ================================================================

#include "atvoss/broadcast/broadcast_sch.h"
#include "ops_macro/macro.h"

namespace AddOp {

/**
 * 不经过类型转换的 Add 计算 DAG
 *
 * 数据流:
 *   Input0(GM) → CopyInBrc → UB
 *   Input1(GM) → CopyInBrc → UB
 *   UB(x1), UB(x2) → Add → UB(y)
 *   UB(y) → CopyOut → GM
 */
template <typename T>
struct AddWithoutCastCompute {
    // 步骤1: 从 GM 拷贝 x1 到本地 (带广播)
    using OpInputX1 = Bind<Vec::CopyInBrc<T>, Placeholder::In0<T>>;

    // 步骤2: 从 GM 拷贝 x2 到本地 (带广播)
    using OpInputX2 = Bind<Vec::CopyInBrc<T>, Placeholder::In1<T>>;

    // 步骤3: 本地向量加法
    using OpAddRes  = Bind<Vec::Add<T>, OpInputX1, OpInputX2>;

    // 步骤4: 结果拷回 GM
    using OpCopyOut = Bind<Vec::CopyOut<T>, Placeholder::Out0<T>, OpAddRes>;

    // 组合为 DAG 调度器
    using Outputs = Elems<OpCopyOut>;
    using MemCfg  = MemOptCfg<MemLevel::LEVEL_2>;  // 使用 L2 内存优化
    using OpDag   = DAGSch<Outputs, void, MemCfg>;
};

/// 对于需要类型转换的 Add (如 int8+int8→int16)
template <typename T1, typename T2>
struct AddMixDtypeCompute {
    using OpInputX1 = Bind<Vec::CopyInBrc<T1>, Placeholder::In0<T1>>;
    using OpInputX2 = Bind<Vec::CopyInBrc<T2>, Placeholder::In1<T2>>;

    // 将 T1 转为 float 再进行计算
    using OpCastX1  = Bind<Vec::Cast<float>, OpInputX1>;
    using OpCastX2  = Bind<Vec::Cast<float>, OpInputX2>;
    using OpAddRes  = Bind<Vec::Add<float>, OpCastX1, OpCastX2>;
    using OpCopyOut = Bind<Vec::CopyOut<float>, Placeholder::Out0<float>, OpAddRes>;

    using Outputs = Elems<OpCopyOut>;
    using MemCfg  = MemOptCfg<MemLevel::LEVEL_2>;
    using OpDag   = DAGSch<Outputs, void, MemCfg>;
};

} // namespace AddOp
```

## 示例6: aclnn API — 单算子外部接口

```cpp
// ================================================================
// 文件: ops-math/math/add/op_api/aclnn_add.cpp
// 功能: Add 算子的 aclnn 标准 API 实现 (用户可直接调用)
// ================================================================

/**
 * aclnn 是 Ascend C LINear Algebra Native API:
 * - 提供类似 PyTorch 风格的算子调用接口
 * - 可直接被 torch_npu 后端调用
 * - 封装了完整的图构建+编译+执行生命周期
 */

/**
 * 获取 Add 算子所需工作空间大小
 *
 * 调用流程:
 *   1. 验证输入参数
 *   2. 创建 OpExecutor
 *   3. 构建计算图 (Contiguous→Cast→Add→ViewCopy)
 *   4. 返回所需 workspace 大小
 */
aclnnStatus aclnnAddGetWorkspaceSize(
    const aclTensor *self,
    const aclTensor *other,
    const aclScalar *alpha,      // 可选: self * alpha + other
    const aclTensor *out,
    uint64_t *workspaceSize,
    aclOpExecutor *executor)
{
    // 1. 参数校验
    CHECK_RET(self != nullptr);
    CHECK_RET(other != nullptr);
    CHECK_RET(workspaceSize != nullptr);

    // 2. 创建执行器, 构建内部计算图
    //    图结构: Contiguous(self) → Cast → Add → Cast → ViewCopy → out
    auto uniqueExecutor = CREATE_OPEXECUTOR(aclnnAdd);
    uniqueExecutor->AddInput(self);
    uniqueExecutor->AddInput(other);
    // ... 计算 tensor 类型、构建节点 ...

    *workspaceSize = uniqueExecutor->GetWorkspaceSize();
    return ACLNN_SUCCESS;
}

/**
 * 执行 Add 算子
 *
 * @param workspace       工作空间内存
 * @param workspaceSize   工作空间大小
 * @param executor        执行器句柄
 * @param stream          ACL Stream
 */
aclnnStatus aclnnAdd(
    void *workspace,
    uint64_t workspaceSize,
    aclOpExecutor *executor,
    aclrtStream stream)
{
    return CommonOpExecutorRun(workspace, workspaceSize, executor, stream);
}
```

## 示例7: Tiling — 分块策略

```cpp
// ================================================================
// 文件: ops-math/math/add/op_host/arch35/add_tiling_arch35.cpp
// 功能: Add 算子的 Tiling (分块) 策略
// ================================================================

/**
 * Tiling 是 Ascend C 框架的核心优化手段:
 * - 将大张量分块, 使每块的数据能放入 AI Core 有限的本地内存
 * - 根据 Shape 和数据类型选择最优的分块配置
 * - 编译期生成调度模式 key, 避免运行时分支
 */

namespace Ops::Base {

ge::graphStatus AddTiling::DoOpTiling() {
    // 获取输入描述符 (Shape, DataType, Format)
    gert::TilingContext *context = GetTilingContext();
    auto x1_desc = context->GetInputDesc(0);
    auto x2_desc = context->GetInputDesc(1);
    auto y_desc  = context->GetOutputDesc(0);

    // 获取平台信息 (UB大小, AI Core 数量)
    uint64_t ubSize;
    int64_t coreNum;
    GetPlatformInfo(context, ubSize, coreNum);

    // 计算输入总元素数
    int64_t totalNum = GetTotalNum(x1_desc);

    // 计算分块策略:
    //   blockFactor = ceil(totalNum / coreNum)   每核心处理元素数
    //   ubFactor    = 每次UB可处理的最大块
    AddTilingData *tiling = context->GetTilingData<AddTilingData>();
    tiling->totalNum = totalNum;
    tiling->blockFactor = CeilDiv(totalNum, coreNum);
    tiling->ubFactor = FloorAlign(
        FloorDiv(ubSize / GetTypeSize(dataType), BUFFER_NUM),
        ubBlockSize);

    // 根据数据类型选择调度模式 (编译期特化, 零运行时开销)
    uint64_t tilingKey = (dataType == ge::DT_FLOAT)
        ? GET_TPL_TILING_KEY(SCH_MODE_FLOAT)       // 单精度模式
        : GET_TPL_TILING_KEY(SCH_MODE_HALF);       // 半精度模式

    context->SetTilingKey(tilingKey);
    context->SetBlockDim(usedCoreNum);

    return ge::GRAPH_SUCCESS;
}

} // namespace Ops::Base
```

## 示例8: convert_tf2npu — TF 代码自动转换

```python
# ================================================================
# 文件: tensorflow/convert_tf2npu/mappings/ast.json (部分)
# 功能: TF API → NPU API 的自动映射表
# ================================================================

"""
该工具通过 AST 变换, 自动将用户的 TF 脚本转换为 NPU 兼容代码。

例如:
  tf.estimator.Estimator → NPUEstimator
  tf.Session()           → tf.Session(config=npu_config)
  tf.nn.dropout          → npu_ops.dropout
"""

# 映射表示例 (ast.json):
{
    "tf.estimator.Estimator": "npu_bridge.estimator.npu.npu_estimator.NPUEstimator",
    "tf.distribute.MirroredStrategy": "npu_device.distribute.npu_distribution.NPUStrategy",
    "tf.train.MonitoredTrainingSession": "npu_bridge.estimator.npu.npu_monitored_training_session",
    "hvd.init": "npu_bridge.estimator.npu.npu_hvd_init",
    "hvd.rank": "npu_bridge.estimator.npu.npu_hvd_rank",
    "tf.Session": "npu_bridge.estimator.npu.npu_session_config_init",
    "tf.nn.dropout": "npu_ops.dropout",
    "tf.gelu": "npu_unary_ops.gelu"
}
```

## 示例9: Runtime 层 — ACL API 调用

```c
// ================================================================
// Runtime API 调用链 (简化示例)
// 这是每个 Ascend C 核函数最终被调用的底层接口
// ================================================================

#include "acl/acl.h"

// 步骤1: 初始化
aclError ret = aclInit(nullptr);
ret = aclrtSetDevice(0);

// 步骤2: 创建执行流
aclrtStream stream;
ret = aclrtCreateStream(&stream);

// 步骤3: 分配设备内存 (HBM)
float *xDevice, *yDevice, *zDevice;
uint64_t byteSize = numElements * sizeof(float);
aclrtMalloc((void**)&xDevice, byteSize, ACL_MEM_MALLOC_HUGE_FIRST);
aclrtMalloc((void**)&yDevice, byteSize, ACL_MEM_MALLOC_HUGE_FIRST);
aclrtMalloc((void**)&zDevice, byteSize, ACL_MEM_MALLOC_HUGE_FIRST);

// 步骤4: 拷贝数据到设备 (H2D)
aclrtMemcpy(xDevice, byteSize, hostX, byteSize, ACL_MEMCPY_HOST_TO_DEVICE);
aclrtMemcpy(yDevice, byteSize, hostY, byteSize, ACL_MEMCPY_HOST_TO_DEVICE);

// 步骤5: 启动核函数 (AI Core)
//   add_custom 是 Ascend C 编译后的核函数
add_custom<2048><<<numBlocks, nullptr, stream>>>(xDevice, yDevice, zDevice);

// 步骤6: 同步等待完成
aclrtSynchronizeStream(stream);

// 步骤7: 拷贝结果回主机 (D2H)
float *zHost;
aclrtMallocHost((void**)&zHost, byteSize);
aclrtMemcpy(zHost, byteSize, zDevice, byteSize, ACL_MEMCPY_DEVICE_TO_HOST);

// 步骤8: 清理
aclrtFree(xDevice); aclrtFree(yDevice); aclrtFree(zDevice);
aclrtFreeHost(zHost);
aclrtDestroyStream(stream);
aclrtResetDevice(0);
aclFinalize();
```
