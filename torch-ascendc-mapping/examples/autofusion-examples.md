# Graph-Autofusion 自动融合代码示例

## 示例1: Python IR 构建 — 定义融合图

```python
# ================================================================
# 文件: autofuse/compiler/python/ascir_api.py (使用示例)
# 功能: 通过 Python DSL 定义需要融合的算子图
# ================================================================

from autofuse import ascir

def build_fc_relu_fusion(batch=16, in_dim=512, out_dim=256):
    """构建 MatMul + Bias + Relu 融合图"""
    g = ascir.HintGraph("fc_relu_fusion")

    # 输入: [batch, in_dim]
    x = ascir.Data([batch, in_dim], name="input_x",
                   axis=[0, 1], size=[batch, in_dim],
                   stride=[in_dim, 1], dtype=ascir.DType.FLOAT16)

    # 权重: [in_dim, out_dim]
    w = ascir.Data([in_dim, out_dim], name="weight",
                   axis=[0, 1], size=[in_dim, out_dim],
                   stride=[out_dim, 1], dtype=ascir.DType.FLOAT16)

    # bias: [1, out_dim]
    b = ascir.Data([1, out_dim], name="bias",
                   axis=[0, 1], size=[1, out_dim],
                   stride=[out_dim, 1], dtype=ascir.DType.FLOAT16)

    # ─── Load: HBM → UB ───
    x_ub = ascir.Load(g, x, axis=[0, 1],
                      size=[batch, in_dim],
                      stride=[in_dim, 1])

    # ─── Compute ───
    # MatMul: [B, D] × [D, O] → [B, O]
    fc = ascir.MatMul(g, x_ub, w, axis=[0, 1])

    # Broadcast bias: [1, O] → [B, O]
    bias_ub = ascir.Broadcast(g, b, axis=[0, 1],
                              size=[batch, out_dim])

    # Add: [B, O] + [B, O]
    add = ascir.Add(g, fc, bias_ub, axis=[0, 1])

    # Relu
    relu = ascir.Relu(g, add, axis=[0, 1])

    # ─── Store: UB → HBM ───
    ascir.Store(g, relu, axis=[0, 1])

    return g


def build_vector_fusion():
    """构建纯向量融合: Add + Mul + Sigmoid"""
    g = ascir.HintGraph("vector_fusion")

    x1 = ascir.Data([1024], name="x1", dtype=ascir.DType.FLOAT32)
    x2 = ascir.Data([1024], name="x2", dtype=ascir.DType.FLOAT32)
    scalar = ascir.Data([1], name="scalar", dtype=ascir.DType.FLOAT32)

    x1_ub = ascir.Load(g, x1, axis=[0], size=[1024], stride=[1])
    x2_ub = ascir.Load(g, x2, axis=[0], size=[1024], stride=[1])

    # Add + Mul + Sigmoid (3个算子在UB上连续计算)
    add = ascir.Add(g, x1_ub, x2_ub, axis=[0])
    mul = ascir.Mul(g, add, scalar, axis=[0])
    sigmoid = ascir.Sigmoid(g, mul, axis=[0])

    ascir.Store(g, sigmoid, axis=[0])
    return g


def build_reduce_fusion():
    """构建 Reduce + Broadcast + Elewise 融合"""
    g = ascir.HintGraph("reduce_fusion")

    x = ascir.Data([64, 128], name="x", dtype=ascir.DType.FLOAT32)
    x_ub = ascir.Load(g, x, axis=[0, 1],
                      size=[64, 128], stride=[128, 1])

    # ReduceMean 沿 axis=1: [64, 128] → [64, 1]
    mean = ascir.ReduceMean(g, x_ub, axis=[0, 1], reduce_axis=1)

    # 比较: x > mean (自动 Broadcast)
    cmp = ascir.Greater(g, x_ub, mean, axis=[0, 1])

    ascir.Store(g, cmp, axis=[0, 1])
    return g
```

## 示例2: CodeGen — ApiCall 工厂与代码生成

```cpp
// ================================================================
// 文件: autofuse/codegen/api_call.h (ApiCall 子类的代码生成逻辑简化)
// 功能: 每种算子类型的代码生成模板
// ================================================================

/**
 * ApiCall 基类:
 *   每个 ApiCall 负责为一个 AscGraph 节点生成完整的 Ascend C 代码段
 *   包含 PreProcess (同步+分配) → ApiCall (计算) → PostProcess (释放+标记)
 */
class ApiCall {
public:
    virtual ~ApiCall() = default;
    virtual string GenPreProcess()  = 0;  // 等待输入, 分配输出buffer
    virtual string GenApiCall()     = 0;  // 生成计算指令
    virtual string GenPostProcess() = 0;  // 标记输出就绪, 释放输入buffer
};

// ─── BinaryApiCall: Add/Sub/Mul/Div ───
class BinaryApiCall : public ApiCall {
public:
    string GenApiCall() override {
        // 生成: Add/Mul/Sub/Div(output, input1, input2, repeat, mask)
        string op_func = OpTypeToFuncName(node_.op_type);  // "Add" / "Mul" / "Sub" / "Div"
        
        // 根据数据类型选择具体指令
        if (node_.output_dtype == DT_FLOAT16) {
            return fmt("%s(%s, %s, %s, %s, %s);",
                op_func,
                output_tensor_name,   // 输出UB地址
                input1_tensor_name,   // 输入1 UB地址
                input2_tensor_name,   // 输入2 UB地址
                tiler.GetRepeatCount(), // 重复次数
                mask_config             // 精度掩码
            );
        } else if (node_.output_dtype == DT_INT32) {
            // 整数版 add 指令
            return fmt("AddInt32(%s, %s, %s, %s);", ...);
        }
        // ... 其他类型的处理
    }

    string GenPreProcess() override {
        // 1. SetFlag 等待输入数据就绪
        string code;
        code += fmt("SetFlag<HardEvent::MTE2_MTE1>(EVENT_%d);\n", event_id_);
        code += fmt("WaitFlag<HardEvent::MTE2_MTE1>(EVENT_%d);\n", event_id_);
        // 2. 从 Allocator 分配输出 buffer
        code += fmt("auto %s = ub_alloc.Alloc<%s>(%s);\n",
                     output_tensor_name, dtype_str, size_str);
        return code;
    }

    string GenPostProcess() override {
        // 1. 标记输出数据就绪
        string code;
        code += fmt("SetFlag<HardEvent::V>(EVENT_%d);\n", output_event_id_);
        // 2. 释放输入 buffer (如果不再使用)
        code += fmt("ub_alloc.Free(%s);\n", input1_tensor_name);
        code += fmt("ub_alloc.Free(%s);\n", input2_tensor_name);
        return code;
    }
};

// ─── UnaryApiCall: Relu/Exp/Sqrt/Neg/Log ───
class UnaryApiCall : public ApiCall {
public:
    string GenApiCall() override {
        string op_func = OpTypeToFuncName(node_.op_type);
        // Relu → "Relu", Exp → "Exp", Sqrt → "Sqrt", Neg → "Neg"
        return fmt("%s(%s, %s, %s);",
            op_func,
            output_tensor_name,
            input_tensor_name,
            tiler.GetRepeatCount()
        );
    }
};

// ─── ReduceApiCall: ReduceSum/ReduceMax/ReduceMean ───
class ReduceApiCall : public ApiCall {
public:
    string GenApiCall() override {
        string op_func = OpTypeToFuncName(node_.op_type);
        // ReduceSum → "ReduceSum", ReduceMax → "ReduceMax"
        return fmt("%s(%s, %s, %s, %s);",
            op_func,
            output_tensor_name,
            input_tensor_name,
            tiler.GetAxisStr(node_.reduce_axis),  // 归约轴
            tiler.GetRepeatCount()
        );
    }
};

// ─── BroadcastApiCall: Broadcast/Duplicate ───
class BroadcastApiCall : public ApiCall {
public:
    string GenApiCall() override {
        // Broadcast: 沿指定轴复制数据
        return fmt("Duplicate(%s, %s, %s, %s);",
            output_tensor_name,
            input_tensor_name,
            tiler.GetBroadcastCount(),
            tiler.GetBroadcastAxis()
        );
    }
};

// ─── 工厂函数 ───
unique_ptr<ApiCall> CreateApiCallObject(const AscGraphNode& node) {
    switch (node.op_type) {
        case OpType::kAdd:
        case OpType::kSub:
        case OpType::kMul:
        case OpType::kDiv:
            return make_unique<BinaryApiCall>(node);

        case OpType::kRelu:
        case OpType::kExp:
        case OpType::kSqrt:
        case OpType::kNeg:
        case OpType::kLog:
            return make_unique<UnaryApiCall>(node);

        case OpType::kAbs:
        case OpType::kSigmoid:
        case OpType::kReciprocal:
            return make_unique<UnaryApiTmpCall>(node);  // 需要中间buffer

        case OpType::kReduceSum:
        case OpType::kReduceMax:
        case OpType::kReduceMin:
        case OpType::kReduceMean:
            return make_unique<ReduceApiCall>(node);

        case OpType::kBroadcast:
        case OpType::kDuplicate:
            return make_unique<BroadcastApiCall>(node);

        case OpType::kLoad:
            return make_unique<LoadApiCall>(node);  // DataCopy

        case OpType::kStore:
            return make_unique<StoreApiCall>(node);  // DataCopy

        case OpType::kCast:
            return make_unique<CastApiCall>(node);

        case OpType::kTranspose:
            return make_unique<TransposeApiCall>(node);

        case OpType::kGather:
            return make_unique<GatherApiCall>(node);

        case OpType::kConcat:
            return make_unique<ConcatApiCall>(node);

        case OpType::kMatMul:
        case OpType::kConv2D:
            // Cube 算子走独立调度器
            return nullptr;

        default:
            return make_unique<GenericApiCall>(node);
    }
}
```

## 示例3: 循环结构构建

```cpp
// ================================================================
// 文件: autofuse/codegen/loop.cpp (简化)
// 功能: 从 Graph 节点构建分层循环结构
// ================================================================

class Loop {
public:
    /**
     * 从调度图中的节点构建循环树
     *
     * 输入: 一组已按拓扑排序的 ScheduleNode
     * 输出: 分层循环树: BlockOuter → TileOuter → TileInner
     *
     * 每个 ScheduleNode 带有 axis 属性指示其所属循环层级
     */
    static LoopTree ConstructFromNodes(
        const vector<ScheduleNode>& nodes,
        const TilingData& tiling)
    {
        LoopTree tree;

        // ─── Level 0: BlockOuter — 跨核分裂循环 ───
        LoopLevel* block_outer = tree.AddLevel("block_outer");
        block_outer->loop_var = "block_idx";
        block_outer->range = tiling.blockDim;  // 例如: 8 (8个core)
        block_outer->stride = 1;

        // ─── Level 1: TileOuter — 外层Tile循环 (多轮) ───
        LoopLevel* tile_outer = tree.AddLevel("tile_outer");
        tile_outer->loop_var = "tile_i";
        tile_outer->range = tiling.tileOuterCount;  // 例如: 4 (需要4轮)
        tile_outer->stride = 1;
        // DataCopy 在此层的 LoopBody 中执行: 每轮加载新的 Tile 数据

        // ─── Level 2: TileInner — 内层UB驻留循环 ───
        LoopLevel* tile_inner = tree.AddLevel("tile_inner");
        tile_inner->loop_var = "ub_i";
        tile_inner->range = tiling.tileInnerSize;   // 例如: 64 (UB可容纳64个元素)
        tile_inner->stride = tiling.tileInnerStride;
        // 计算指令在此层执行: UB 数据常驻, 无需反复加载

        // ─── 将节点分配到对应层级 ───
        for (const auto& node : nodes) {
            if (node.axis_type == AxisType::kBlockOuter) {
                block_outer->AddNode(node);
            } else if (node.axis_type == AxisType::kTileOuter) {
                tile_outer->AddNode(node);
            } else if (node.axis_type == AxisType::kTileInner) {
                tile_inner->AddNode(node);
            }
        }

        return tree;
    }

    /**
     * 生成循环代码
     *
     * 输出示例:
     *   for (int64_t block = 0; block < t.blockDim; block++) {
     *     for (int64_t tile = 0; tile < t.tileCnt; tile++) {
     *       DataCopy(ub, gm[offset], size);  // TileOuter: 加载数据
     *       for (int64_t i = 0; i < t.tileSize; i++) {
     *         Add(z, x, y, size);   // TileInner: 计算
     *         Mul(z, z, s, size);
     *       }
     *       DataCopy(gm[offset], ub, size);  // TileOuter: 写回结果
     *     }
     *   }
     */
    string GenerateCode() const {
        stringstream ss;
        // 递归生成嵌套 for 循环
        GenerateLoopLevel(ss, levels_, 0);
        return ss.str();
    }

private:
    vector<LoopLevel> levels_;
};
```

## 示例4: Schedule — TilingCase 生成与模板选择

```cpp
// ================================================================
// 文件: autofuse/optimize/autoschedule.cpp (简化)
// 功能: 自动调度 — 生成 TilingCase 并选择最优方案
// ================================================================

class AutoScheduler {
public:
    /**
     * 为 ScheduleGroup 生成所有可行的 Tiling 方案
     * 并选择得分最高者
     */
    ScheduledResult Schedule(ScheduleGroup& group) {
        // 步骤1: 生成所有候选 TilingCase
        vector<TilingCase> cases = PrepareTilingCases(group);

        ScheduledResult best_result;
        double best_score = -1.0;

        // 步骤2: 评估每个候选
        for (auto& tc : cases) {
            if (!tc.IsValid(group)) continue;  // 排除不合法方案

            ScheduledResult result = ProcessOneTilingCase(group, tc);
            double score = EvaluateTilingCase(tc, group);

            if (score > best_score) {
                best_score = score;
                best_result = result;
            }
        }

        return best_result;
    }

private:
    /**
     * 生成所有 TilingCase
     *
     * 为图的每个轴尝试分配 5 种 AxisType:
     *   BlockOuter / BlockInner / TileOuter / TileInner / Original
     *
     * 例如有 2 个轴: 5×5 = 25 种组合
     */
    vector<TilingCase> PrepareTilingCases(ScheduleGroup& group) {
        vector<TilingCase> cases;
        int num_axes = group.GetNumAxes();

        // 递归枚举所有轴划分方案
        vector<AxisType> current(num_axes, AxisType::kOriginal);
        EnumerateAxisTypes(cases, current, 0, group);

        return cases;
    }

    void EnumerateAxisTypes(vector<TilingCase>& cases,
                            vector<AxisType>& current,
                            int axis_idx,
                            ScheduleGroup& group) {
        if (axis_idx == group.GetNumAxes()) {
            cases.emplace_back(current, group);
            return;
        }

        // BlockOuter: 分裂到多个core
        if (group.CanSplitToBlock(axis_idx)) {
            current[axis_idx] = AxisType::kBlockOuter;
            EnumerateAxisTypes(cases, current, axis_idx + 1, group);
        }

        // BlockInner: core内部块迭代
        current[axis_idx] = AxisType::kBlockInner;
        EnumerateAxisTypes(cases, current, axis_idx + 1, group);

        // TileOuter: 多轮迭代, 需重新加载数据
        current[axis_idx] = AxisType::kTileOuter;
        EnumerateAxisTypes(cases, current, axis_idx + 1, group);

        // TileInner: UB驻留, 最内层
        current[axis_idx] = AxisType::kTileInner;
        EnumerateAxisTypes(cases, current, axis_idx + 1, group);

        // Original: 不分裂
        current[axis_idx] = AxisType::kOriginal;
        EnumerateAxisTypes(cases, current, axis_idx + 1, group);
    }

    /**
     * 评估 TilingCase 得分
     *
     * 核心指标:
     *   - 数据复用率: 计算量 / 数据搬运量 (越高越好)
     *   - UB 利用率: UB_used / 256KB (越高越好)
     *   - 并行度: 实际core数 / 总core数 (越高越好)
     *   - 指令并行度: Cube+Vector流水线利用率
     */
    double EvaluateTilingCase(TilingCase& tc, ScheduleGroup& group) {
        double reuse_ratio = tc.GetComputeOps() /
                             (double)tc.GetDataCopyBytes();  // 数据复用率

        double ub_util = tc.GetUBUsage() / (double)UB_TOTAL_SIZE;  // UB利用

        double par_eff = tc.GetActiveCores() /
                         (double)tc.GetTotalCores();  // 并行效率

        double pipe_eff = tc.GetPipelineOverlap();  // 流水线重叠度

        // 加权评分
        return reuse_ratio * 0.40 +
               ub_util * 0.20 +
               par_eff * 0.25 +
               pipe_eff * 0.15;
    }
};
```

## 示例5: SuperKernel — sk_entry 调度循环

```cpp
// ================================================================
// 文件: super_kernel/kernel/sk_entry.asc (简化)
// 功能: SuperKernel 的统一调度循环
// ================================================================

/**
 * SuperKernel 通过函数指针表 (TaskInfo[]) 依次调用子 Kernel，
 * 消除 N 次独立的 runtime launch 开销。
 *
 * 任务类型:
 *   TYPE_PRELOAD (0) — 预取下一个函数的 ICache
 *   TYPE_FUNC    (1) — 执行子 Kernel 函数
 *   TYPE_SYNC    (2) — CrossCore 同步 (SetFlag/WaitFlag)
 *   TYPE_EVENT_* (3-5) — 内存语义事件通知/等待/重置
 */

// 入口结构体 — 由 AOT 编译时填充
struct SkDeviceEntryArgs {
    HeaderInfo header;             // 版本/大小/mode
    TaskInfo* task_que;            // 任务队列 (函数指针表)
    uint64_t total_task;          // 总任务数
    uint64_t core_cnt;            // 启用核心数
};

// 任务描述 — 每个子 Kernel 对应一个 TaskInfo
struct TaskInfo {
    uint32_t type;                // 任务类型: PRELOAD/FUNC/SYNC/EVENT
    union {
        struct { void* addr; uint32_t size; } preload;  // 预取信息
        struct { void** entry; void* params; } func;    // 函数指针表 + 参数
        struct { uint32_t id; uint32_t mask; } sync;    // 同步ID + 掩码
        struct { void* addr; uint64_t value; } event;    // 事件地址 + 值
    };
};

__global__ __aicore__ void sk_entry(__gm__ uint8_t* args) {
    SkDeviceEntryArgs* dev_args = (SkDeviceEntryArgs*)args;
    TaskInfo* task_que = dev_args->task_que;

    // ─── 子 Kernel 拆分: 每个 core 使用不同的函数副本 ───
    // 同一代码 4 份副本在内存中, 不同 core 访问不同物理地址,
    // 缓解 L2 Cache 争用
    uint32_t core_id = get_coreid();
    uint32_t func_idx = core_id % entry_cnt;  // entry_cnt=4

    // ─── 主调度循环: 按序执行 TaskQue ───
    for (uint32_t i = 0; i < dev_args->total_task; i++) {
        TaskInfo* task = &task_que[i];

        switch (task->type) {

        case TYPE_PRELOAD: {
            // ─── ICache 预取 ───
            // 在执行当前 Kernel 时, 将下一个 Kernel 的代码段
            // 提前加载到 ICache, 避免取指延迟
            dc_preload((__gm__ uint64_t*)task->preload.addr,
                       task->preload.size);
            break;
        }

        case TYPE_FUNC: {
            // ─── 子 Kernel 调用 ───
            typedef void (*KernelFunc)(__gm__ uint8_t* param);
            KernelFunc f = (KernelFunc)(task->func.entry[func_idx]);
            f(task->func.params);
            break;
        }

        case TYPE_SYNC: {
            // ─── CrossCore 同步 ───
            // 片上同步 (SetFlag/WaitFlag), 比 Runtime 调度快数百倍
            SetFlag<HardEvent::VEC_VEC>(task->sync.id);
            WaitFlag<HardEvent::VEC_VEC>(task->sync.mask);
            break;
        }

        case TYPE_EVENT_NOTIFY:
            // ─── 内存语义通知 ───
            // 写 VALUE_WRITE 到共享内存地址
            *((volatile uint64_t*)task->event.addr) = NOTIFY_VALUE;
            break;

        case TYPE_EVENT_WAIT:
            // ─── 内存语义等待 ───
            // 自旋等待直到共享内存地址的值变为 EXPECT_VALUE
            while (*((volatile uint64_t*)task->event.addr) != EXPECT_VALUE) {
                // yield: 让出流水线, 降低功耗
                asm volatile("yield");
            }
            break;

        case TYPE_EVENT_RESET:
            // ─── 事件重置 ───
            *((volatile uint64_t*)task->event.addr) = RESET_VALUE;
            break;
        }
    }
}
```

## 示例6: AOT — Scope 分割与 TaskQue 构建

```cpp
// ================================================================
// 文件: super_kernel/src/aot/sk_task_builder.cpp (简化)
// 功能: AOT 编译: 将 Scope 转化为 TaskQue
// ================================================================

/**
 * SkTaskBuilder 负责:
 *   1. 接收 ScopeSplit 结果 (已经分割好的子图)
 *   2. 解析每个子 Kernel 的二进制元数据 (SkBindMap)
 *   3. 生成 TaskQue: PRELOAD + FUNC + SYNC + EVENT 四类 Task
 *   4. 应用 Early-Start 优化
 *   5. 处理资源分配 (内存/ICache/事件)
 */

class SkTaskBuilder {
public:
    /**
     * 构建一个 Scope 的完整 TaskQue
     */
    TaskQueResult BuildTaskQue(ScopeSplitResult& scope,
                               const SkBindMap& bind_map) {
        TaskQueResult result;
        vector<TaskInfo> tasks;

        // ─── Step 1: 遍历 scope 中的节点 ───
        for (auto& node : scope.nodes) {
            // 查找该算子的子 Kernel 函数指针
            // SkBindMap 存储在算子二进制中, 记录了原始函数 → 子Kernel函数指针的映射
            auto& func_info = bind_map.Resolve(node.kernel_name());

            // ─── 生成 PRELOAD Task (ICache预取) ───
            TaskInfo preload_task;
            preload_task.type = TYPE_PRELOAD;
            preload_task.preload.addr = func_info.func_addr;
            preload_task.preload.size = func_info.prefetch_size;
            tasks.push_back(preload_task);

            // ─── 生成 FUNC Task (子Kernel调用) ───
            TaskInfo func_task;
            func_task.type = TYPE_FUNC;
            func_task.func.entry = func_info.entry_ptrs;  // 4份副本的函数指针
            func_task.func.params = node.params_addr;
            tasks.push_back(func_task);
        }

        // ─── Step 2: 插入跨节点同步 ───
        InsertSyncTasks(tasks, scope);

        // ─── Step 3: Apply Early-Start 优化 ───
        ApplyEarlyStartSync(tasks, scope);

        // ─── Step 4: 插入事件通知 (scope边界的跨流同步) ───
        InsertEventTasks(tasks, scope);

        result.task_que = tasks;
        result.total_tasks = tasks.size();
        return result;
    }

private:
    /**
     * Early-Start 优化:
     *   前序算子的 MTE 搬运与后续算子的标量初始化可以并发。
     *
     * 实现: 在前序算子的 MTE 指令前插入 CrossCoreSetFlag,
     *       在后续算子的标量初始化后插入 CrossCoreWaitFlag
     */
    void ApplyEarlyStartSync(vector<TaskInfo>& tasks,
                             ScopeSplitResult& scope) {
        for (size_t i = 0; i < tasks.size() - 1; i++) {
            if (tasks[i].type == TYPE_FUNC &&
                tasks[i+1].type == TYPE_FUNC) {

                // 获取 kernel 元信息: 是否支持 Early-Start
                auto& prev_info = scope.kernel_infos[tasks[i].func_name];
                auto& next_info = scope.kernel_infos[tasks[i+1].func_name];

                if (prev_info.has_mte_tail &&      // 前序末尾有MTE搬运
                    next_info.has_scalar_head) {    // 后续开头有标量初始化

                    // 插入 CrossCore SetFlag (在前序MTE前)
                    TaskInfo sync_set;
                    sync_set.type = TYPE_SYNC;
                    sync_set.sync.id = GetSyncId();
                    sync_set.sync.direction = SyncDir::AIC_TO_AIC_SET;
                    tasks.insert(tasks.begin() + i + 1, sync_set);

                    // 插入 CrossCore WaitFlag (在后续标量初始化后)
                    TaskInfo sync_wait;
                    sync_wait.type = TYPE_SYNC;
                    sync_wait.sync.id = sync_set.sync.id;
                    sync_wait.sync.direction = SyncDir::AIC_TO_AIC_WAIT;
                    tasks.insert(tasks.begin() + i + 3, sync_wait);
                }
            }
        }
    }

    /**
     * 子 Kernel 拆分:
     *   同一代码编译 4 份副本, 不同 core 通过 coreId % 4 选择
     *   缓解 L2 Cache 共享引发的多核串行化争用
     */
    static constexpr int K_MAX_SPLIT_BIN_COUNT = 4;
};
```

## 示例7: Buffer 分配与生命周期复用

```cpp
// ================================================================
// 文件: autofuse/optimize/buf_que_allocator.cpp (简化)
// 功能: UB Buffer 的分配与生命周期复用
// ================================================================

/**
 * MemReuseManager: 管理 UB (Unified Buffer, 256KB) 上的内存分配
 *
 * 核心思想: 生命周期不重叠的 Tensor 共享同一块 UB 地址
 *
 * 例如:
 *   时间线:  t1→[Add输出 buf0]  t2→[Mul输入 buf0, Mul输出 buf1]
 *   UB分配:  [   offset_0  ][   offset_0   ][    offset_256   ]
 *
 *   Add输出和Mul输入永远不会同时存在 → 复用 offset_0
 */
class MemReuseManager {
public:
    struct AllocInfo {
        size_t offset;       // UB中的偏移地址
        size_t size;         // 分配的字节数
        int64_t alloc_time;  // 分配时间戳
        int64_t free_time;   // 释放时间戳
    };

    /**
     * 分配一段 UB Buffer
     */
    AllocInfo Alloc(size_t size, int64_t timestamp) {
        // 查找可复用的已释放空间
        for (auto& hole : free_holes_) {
            if (hole.size >= size && hole.free_time < timestamp) {
                AllocInfo info;
                info.offset = hole.offset;
                info.size = size;
                info.alloc_time = timestamp;

                // 更新空洞 (如果 size < hole.size, 保留剩余部分)
                if (size < hole.size) {
                    hole.offset += size;
                    hole.size -= size;
                } else {
                    free_holes_.erase(&hole);
                }

                return info;
            }
        }

        // 无可用空洞, 在末尾分配新空间
        AllocInfo info;
        info.offset = total_allocated_;
        info.size = size;
        info.alloc_time = timestamp;
        total_allocated_ += size;

        return info;
    }

    /**
     * 释放一段 UB Buffer, 加入空洞列表
     */
    void Free(AllocInfo info, int64_t timestamp) {
        info.free_time = timestamp;
        free_holes_.push_back(info);

        // 合并相邻空洞
        MergeAdjacentHoles();
    }

private:
    size_t total_allocated_ = 0;
    vector<AllocInfo> free_holes_;  // 可用空洞 (按 offset 排序)
};

/**
 * BufQueAllocator: 利用 MemReuseManager 分配全图 Buffer
 *
 * 对每个 ApiCall:
 *   1. GenPreProcess: 调用 Alloc(output_size) 分配输出 buffer
 *   2. GenApiCall: 使用分配的 buffer 进行计算
 *   3. GenPostProcess: 调用 Free(input_info) 释放不再使用的输入 buffer
 *
 * 这使得:
 *   - Add 的输出和 Mul 的输入可以复用同一块 UB
 *   - 只要两个 Tensor 的生命周期不重叠, 就能共享内存
 *   - 最大化 UB (256KB) 的利用率, 支持更大 Tile 尺寸
 */
class BufQueAllocator {
public:
    void Allocate(const ScheduledResult& schedule) {
        MemReuseManager mem_mgr;
        int64_t timestamp = 0;

        for (auto& group : schedule.schedule_groups) {
            for (auto& graph : group.impl_graphs) {
                for (auto& node : graph.nodes) {

                    // 根据 node 输出大小分配
                    if (node.HasOutput()) {
                        size_t output_size = node.GetOutputSize();
                        auto alloc = mem_mgr.Alloc(output_size, timestamp);
                        node.ub_offset = alloc.offset;
                    }

                    // 释放不再使用的输入
                    for (auto& input : node.inputs) {
                        if (input.IsLastUse(node)) {
                            mem_mgr.Free(input.alloc_info, timestamp);
                        }
                    }

                    timestamp++;
                }
            }
        }
    }
};
```
