<!--
@source: include/adv_api/hccl/hccl_common.h
@source: include/adv_api/hccl/internal/hccl_msg.h
@source: include/adv_api/hccl/internal/hccl_tiling_msg.h
@source: impl/adv_api/detail/hccl/common/hccl_inner_def.h
@source: impl/adv_api/detail/hccl/cc/src/ops/op_common/inc/alg_param.h
-->

# 核心数据结构

本文档梳理 HCCL 中各层的核心数据结构定义。

## 1. 枚举类型

### HcclCMDType — 集合通信命令类型

> 定义见 `include/adv_api/hccl/hccl_common.h:22-44`。

| 枚举值 | 整数值 | 说明 |
|--------|--------|------|
| `HCCL_CMD_INVALID` | 0 | 无效 |
| `HCCL_CMD_BROADCAST` | 1 | Broadcast |
| `HCCL_CMD_ALLREDUCE` | 2 | AllReduce |
| `HCCL_CMD_REDUCE` | 3 | Reduce |
| `HCCL_CMD_SEND` | 4 | Send |
| `HCCL_CMD_RECEIVE` | 5 | Receive |
| `HCCL_CMD_ALLGATHER` | 6 | AllGather |
| `HCCL_CMD_REDUCE_SCATTER` | 7 | ReduceScatter |
| `HCCL_CMD_ALLTOALLV` | 8 | AlltoAllV |
| `HCCL_CMD_ALLTOALLVC` | 9 | AlltoAllVC |
| `HCCL_CMD_ALLTOALL` | 10 | AlltoAll |
| `HCCL_CMD_GATHER` | 11 | Gather |
| `HCCL_CMD_SCATTER` | 12 | Scatter |
| `HCCL_CMD_BATCH_SEND_RECV` | 13 | BatchSendRecv |
| `HCCL_CMD_BATCH_PUT` | 14 | BatchPut |
| `HCCL_CMD_BATCH_GET` | 15 | BatchGet |
| `HCCL_CMD_ALLGATHER_V` | 16 | AllGatherV |
| `HCCL_CMD_REDUCE_SCATTER_V` | 17 | ReduceScatterV |
| `HCCL_CMD_BATCH_WRITE` | 18 | BatchWrite |
| `HCCL_CMD_HALF_ALLTOALLV` | 20 | HalfAlltoAllV |
| `HCCL_CMD_ALL` | — | 上限标志 |

### HcclReduceOp — 规约操作类型

> 定义见 `hccl_common.h:46-52`。

| 枚举值 | 整数值 | 说明 |
|--------|--------|------|
| `HCCL_REDUCE_SUM` | 0 | 求和 |
| `HCCL_REDUCE_PROD` | 1 | 求积 |
| `HCCL_REDUCE_MAX` | 2 | 最大值 |
| `HCCL_REDUCE_MIN` | 3 | 最小值 |
| `HCCL_REDUCE_RESERVED` | 4 | 保留 |

### HcclDataType — 数据类型

> 定义见 `hccl_common.h:74-94`。共 18 种数据类型，从 INT8 到 MXFP8。

### OpExecuteConfig — 执行模式

> 定义见 `impl/.../cc/src/ops/op_common/inc/alg_param.h:98-109`。

| 枚举值 | 说明 |
|--------|------|
| `DEFAULT` | 默认模式 |
| `HOSTCPU_TS` | Host CPU 任务调度 |
| `AICPU_TS` | AICPU 任务调度 |
| `AIV` | AIV 执行 |
| `AIV_ONLY` | 仅 AIV 执行 |
| `CCU_MS` | CCU 多核调度 |
| `CCU_SCHED` | CCU 调度模式 |
| `AICPU` | AICPU 执行 |
| `HOSTCPU` | Host CPU 执行 |
| `CCU_FAIL` | CCU 失败回退 |

---

## 2. 设备侧数据结构

### CommonPrepareParam — 通用 Prepare 参数

> 定义见 `impl/.../common/hccl_inner_def.h:42-54`。

```cpp
struct CommonPrepareParam {
    HcclCommType commType;       // 通信类型（联合体：prepareType 或 msgType）
    GM_ADDR sendBuf;             // 源数据缓冲区
    GM_ADDR recvBuf;             // 目标数据缓冲区
    uint64_t count;              // 数据数量
    HcclDataType dataType;       // 数据类型
    HcclDataType dstDataType;    // 目标数据类型
    HcclReduceOp op;             // 规约操作
    uint64_t strideCount;        // 步长计数
    uint8_t repeat;              // 重复次数
    AlltoAllVParamExt paramExt;  // AlltoAllV 专用扩展
    AlltoAllvWriteParamExt wParamExt; // AlltoAllvWrite 专用扩展
};
```

所有算子的 Prepare 接口最终都构造此结构体，传给 `CommonPrepareImpl()`。

### HcclCombineOpParam — 通信组上下文

> 定义见 `hccl_inner_def.h:88-111`。

```cpp
struct HcclCombineOpParam {
    uint64_t workSpace;          // Client-Server 通信地址
    uint64_t workSpaceSize;      // 通信空间大小
    uint32_t rankId;             // 本 rank ID
    uint32_t rankNum;            // 通信组 rank 数量
    uint64_t winSize;            // 每个 window 内存大小
    uint64_t windowsIn[HCCL_MAX_RANK_NUM];   // 各 rank 的输入窗口地址
    uint64_t windowsOut[HCCL_MAX_RANK_NUM];  // 各 rank 的输出窗口地址
    // V310 额外字段：
    GM_ADDR xnOffset;            // XN 偏移地址
    GM_ADDR ckeOffset;           // CKE 偏移地址
    uint32_t opType[MAX_CC_TILING_NUM];       // 算子类型表
    uint8_t algorithmType[MAX_CC_TILING_NUM]; // 算法类型表
};
```

### OpResCtx — 运行时上下文

> 定义见 `hccl_msg.h:212-219`。

```cpp
struct OpResCtx {
    uint64_t version;
    uint64_t workspace;          // 共享内存地址（指向 HcclMsgArea）
    uint64_t workspaceSize;
    uint64_t rankId;
    uint64_t rankSize;
    AlgInfo algInfo[HCCL_ALG_MAX_NUM]; // 算法信息表（offset + opParam）
};
```

### Tiling 内部结构

#### Mc2InitTilingInner

> 定义见 `hccl_tiling_msg.h:22-32`。

```cpp
struct Mc2InitTilingInner {
    uint32_t version;
    uint32_t mc2HcommCnt;
    uint32_t offset[MAX_CC_TILING_NUM]; // 8 个 tiling 偏移
    uint8_t debugMode;
    uint8_t preparePosition;
    uint16_t queueNum;            // 队列数（0 表示单队列）
    uint16_t commBlockNum;
    uint8_t devType;              // 设备类型
};
```

#### Mc2CcTilingInner

> 定义见 `hccl_tiling_msg.h:36-50`。

```cpp
struct Mc2CcTilingInner {
    uint8_t skipLocalRankCopy;    // 跳过本地 rank 拷贝
    uint8_t skipBufferWindowCopy; // 跳过窗口拷贝
    uint8_t stepSize;             // 步长大小
    uint8_t version;
    uint8_t commEngine;           // 通信引擎
    uint8_t srcDataType;
    uint8_t dstDataType;
    char groupName[128];          // 通信组名
    char algConfig[128];          // 算法配置字符串
    uint32_t opType;              // 算子类型
    uint32_t reduceType;          // 规约类型
};
```

---

## 3. Host 侧数据结构

### OpParam — 算子参数

> 定义见 `impl/.../cc/src/ops/op_common/inc/alg_param.h:471-552`。这是 Host 侧算子执行的核心参数结构体。

关键字段：

| 字段 | 类型 | 说明 |
|------|------|------|
| `hcclComm` | `void*` | 通信域句柄 |
| `tag` | `char[32]` | topoInfo 的 key |
| `algTag` | `char[160]` | 资源 key（绑定算法） |
| `inputPtr` / `outputPtr` | `void*` | 输入/输出指针 |
| `inputSize` / `outputSize` | `u64` | 输入/输出大小 |
| `reduceType` | `HcclReduceOp` | 规约类型 |
| `root` | `u32` | Root rank ID |
| `opType` | `HcclCMDType` | 算子类型 |
| `algType` | `AlgType` | 算法类型（三级） |
| `opExecuteConfig` | `OpExecuteConfig` | 执行模式 |
| `DataDes` | union | 数据描述（count/dataType/strideCount） |
| `all2AllVDataDes` | union | AlltoAllV 数据描述 |
| `resCtx` | `void*` | 资源上下文指针 |
| `opThread` | `ThreadHandle` | 算子流对应的 thread |

### AlgType — 算法类型

> 定义见 `impl/.../cc/src/common/alg_type.h:75-136`。

```cpp
struct AlgType {
    AlgTypeLevel0 algoLevel0;  // 节点内拓扑算法
    AlgTypeLevel1 algoLevel1;  // 节点间拓扑算法
    AlgTypeLevel2 algoLevel2;  // 超节点间拓扑算法
};
```

三级算法枚举分别定义于 `alg_type.h:36-50`、`52-64`、`66-73`。

### TopoInfoWithNetLayerDetails — 拓扑信息

> 定义见 `alg_param.h:162-264`。

继承自 `TopoInfo`，增加网络层级详情：

| 字段 | 说明 |
|------|------|
| `topoLevelNums` | 拓扑层级数 |
| `level0Topo` | Level0 拓扑形状（CLOS/MESH_1D/MESH_1D_CLOS） |
| `Level0Nhr` / `Level1Nhr` | 是否为 NHR 拓扑 |
| `is2DieFullMesh` | 是否为 2Die 全连接 Mesh |
| `level0PcieMix` | 是否为 PCIe 混合拓扑 |
| `level0MeshType` | Mesh 类型（SINGLE_DIE/TWO_DIE_REGULAR/...） |
| `netLayerDetails` | 网络层级详情（层数、实例数等） |
| `topoInstDetailsOfLayer` | 各层拓扑实例详情 |

### AlgResourceCtxSerializable — 可序列化资源上下文

> 定义见 `alg_param.h:376-463`。

包含算法执行所需的全部资源信息，支持序列化/反序列化用于 Host→Device 传递：

| 字段 | 说明 |
|------|------|
| `algType` | 环境变量设置的算法类型 |
| `algHierarchyInfo` | 算法分层信息 |
| `cclMem` | 跨 Rank 缓存 Buffer |
| `threads` | Thread 句柄数组 |
| `channels` | 通信链路信息 |
| `ccuKernels` | CCU Kernel 句柄 |
| `topoInfo` | 拓扑信息（序列化传递） |

### PrepareData — 模板执行参数

> 定义见 `impl/.../cc/src/ops/op_common/template/alg_template_base.h:68-101`。

```cpp
struct PrepareData {
    u32 root, userRank, userRankSize;
    u32 interRank, interRankSize;
    u64 count;
    HcclDataType dataType;
    HcclReduceOp reductionOp;
    HcclMem inputMem, outputMem, scratchMem, cclInMem, cclOutMem;
    ThreadHandle thread;
    const std::vector<Slice>* slicesPtr;
    HcomCollOpInfo* opInfo;
    bool disableDMAReduce;
    HcclCMDType opType;
};
```

---

## 4. CCU 相关数据结构

### CCUMsg — CCU 消息

> 定义见 `hccl_inner_def.h:157-162`。

```cpp
struct CCUMsg {
    GM_ADDR xnData;       // 转换为 CCU 寄存器值的消息
    GM_ADDR xnAddr;
    GM_ADDR commitCKEAddr; // 对应每个 msg 的 commit 地址
    GM_ADDR waitCKEAddr;   // 对应每个 msg 的 wait 地址
};
```

### CcuFastLaunchCtx — CCU 快速下发上下文

> 定义见 `alg_param.h:287-312`。

用于 CCU 快速下发场景，包含算法名、线程数和 kernel 提交信息。
