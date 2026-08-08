# InsAllReduceNHR MC2 迁移方案

## 1. 迁移目标

参考 `D:/code/comm/hccl` 中 `InsAllReduceNHR` 的实现，将该算法迁移到 `asc-devkit-4645` 的 MC2 HCCL 实现，使 MC2 AllReduce 可以在 Host 资源创建和 AICPU Device 执行阶段使用本地注册的 `InsAllReduceNHR`。

本次迁移范围严格限定为：

- 迁移 `InsTempAllReduceNHR` template。
- 在已有 `InsV2AllReduceSoleExecutor` 中注册 `InsAllReduceNHR`。
- 接入 MC2 Host `mc2_client` 和 Device `mc2_server` 构建。
- 适配 MC2 默认 selector 和 `algConfig` 强制算法选择路径。
- 不迁移 `InsAllReduceAicpuReduceNHR`、`AicpuAllReduceSoleNHRTwoShotMultiLink`、并行、并发、sequence 或其他 AllReduce 算法。
- 不迁移源仓库的 multi-jetty channel 扩展。
- 不修改公共通信框架，不进行无关重构或算法优化。

## 2. 源实现链路

源仓库中 `InsAllReduceNHR` 是算法注册名，实际实现链路为：

```text
InsAllReduceNHR
  -> InsV2AllReduceSoleExecutor<TopoMatch1D, InsTempAllReduceNHR>
    -> InsTempAllReduceNHR::CalcRes
      -> CalcChannelRequestNhr / CalcChannelRequestNhrMultiJetty
    -> InsTempAllReduceNHR::KernelRun
      -> PreCopy
      -> RunReduceScatter
      -> RunAllGather
      -> PostCopy
```

NHR 的 ReduceScatter 和 AllGather 均执行 `ceil(log2(rankSize))` 个通信 step。每个 step 根据 `2^step` 计算发送和接收 rank，并计算本轮收发的数据片编号。

## 3. 迁移内容

### 3.1 新增 NHR Template

新增文件：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_nhr.h`
- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/ins_temp_all_reduce_nhr.cc`

保留的源实现功能：

1. `CalcScratchMultiple` 返回 1，当前轮完整数据存放在 HCCL buffer。
2. 输入数据按 rank 数向下取整切片，前 `rankSize - 1` 片大小相同，最后一片接收尾部数据。
3. `PreCopy` 将当前轮输入复制到 HCCL buffer。
4. ReduceScatter 按 NHR step 计算收发对端和数据片，通过 `SendRecvReadReduce` 或 `SendRecvBatchWriteReduce` 在通信过程中完成归约。
5. AllGather 按逆序 NHR step 交换已归约的数据片，通过 `SendRecvRead` 或 `SendRecvBatchWrite` 恢复完整数据。
6. `PostCopy` 将 HCCL buffer 中的完整结果复制到输出。
7. 根据 channel 协议决定使用 DMA Read 或 Batch Write 模式。
8. 保留多 channel 数据分流、线程和 notify 处理结构。
9. 增加单 rank 本地复制路径，避免强制选择 NHR 时因没有远端 channel 返回失败。


NHR 调用效果
```text
KernelRun
-> PreCopy(input -> HCCL buffer)
-> NHR ReduceScatter
-> NHR AllGather
-> PostCopy(HCCL buffer -> output)
```
Mesh1DTwoShot 调用效果
```text
KernelRun
-> SplitData
-> RunReduceScatter
-> RunAllGather
```

### 3.2 Executor 注册

修改文件：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/executor/ins_v2_all_reduce_sole_executor.cc`

复用目标仓已有通用 executor，不新增 executor 类，只增加 template include 和注册项：

```text
(HCCL_CMD_ALLREDUCE, InsAllReduceNHR)
  -> InsV2AllReduceSoleExecutor<TopoMatch1D, InsTempAllReduceNHR>
```

MC2 Host 资源阶段和 Device 运行阶段编译同一个 executor 文件，因此两侧 registry 使用相同算法名和 template 类型。

### 3.3 MC2 Selector

修改文件：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/selector/all_reduce_auto_selector.cc`

目标仓 MC2 在资源创建阶段将 AllReduce `DataDes.count` 初始化为 0，真实 count 在 Device 收到通信消息后写回。因此本次 selector 不按数据量选择算法，只按资源阶段已经确定的拓扑、数据类型和 reduce op 选择：

| 条件 | 默认算法 |
| --- | --- |
| 单层 `MESH_1D` | `InsAllReduceMesh1DTwoShot` |
| 单层、Mesh 链路全连接的 PCIE mixed `MESH_1D_CLOS` | `InsAllReduceMesh1DTwoShot` |
| 单层 `CLOS` | `InsAllReduceNHR` |
| 其他单层 `MESH_1D_CLOS` | `InsAllReduceNHR` |
| 多层或其他拓扑 | `NOT_MATCH` |

- InsAllReduceNHR：扁平化 NHR，适合小数据或不适合多维并行的拓扑。
- InsAllReduceParallelRSAG：两层 Mesh1D 拓扑下的 Mesh/NHR 双维并行算法。
- InsV2AllReduceOmniPipePcie：单层 Mesh/CLOS/PCIe 混合拓扑下的大数据流水算法。

| 维度 | InsAllReduceNHR | InsAllReduceParallelRSAG | InsV2AllReduceOmniPipePcie |
| :--- | :--- | :--- | :--- |
| 主要拓扑 | 扁平 NHR，可作为多种拓扑回退 | 两层 MESH_1D | 单层 MESH_1D_CLOS、存在 PCIe |
| topoLevelNums | 多种，主要是多层退化或两层小数据 | 必须为 2 | 有效选择入口为 1 |
| level0Topo | 不完全固定 | 必须 MESH_1D | 必须 MESH_1D_CLOS |
| Level1Nhr | true 时优先选择 | 必须为 false | 不参与该分支 |
| PCIe Mix | 非必要 | 非必要 | 必须 level0PcieMix == true |
| Mesh 全覆盖 | 非必要 | Level 0 为规则 Mesh | 必须不能覆盖全部 Level 0 rank |
| 典型数据量 | 小数据或拓扑退化 | > 32 MiB && <= 4 GiB | >= 32 MiB |
| Executor | Sole | Parallel | OmniPipe |
| TopoMatch | TopoMatch1D | TopoMatchMultilevel | TopoMatchPcieMix |
| 算法结构 | 单模板 NHR RS→AG | Mesh/NHR 两种维序并行 | Mesh/NHR 分层切片流水 |
| Scratch 倍数 | 1 | 较高，取决于四个模板 | 按多层 slice 和带宽计算 |
| 固定调度开销 | 最低 | 中等 | 最高 |


### 3.4 构建接入

Host 修改：

- `impl/adv_api/detail/hccl/cc/src/ops/all_reduce/template/aicpu/CMakeLists.txt`
- 将 `ins_temp_all_reduce_nhr.cc` 加入 `mc2_client`。

Device 修改：

- `impl/adv_api/detail/hccl/cc/src/CMakeLists.txt`
- 将 `ins_temp_all_reduce_nhr.cc` 加入 `mc2_server`。

executor 文件已同时进入 Host 和 Device 构建，新增注册无需额外源文件。

## 4. 重点适配改动

### 4.1 命名空间适配

源仓库使用：

```cpp
namespace ops_hccl
```

目标仓 MC2 本地算法使用：

```cpp
namespace mc2_ops_hccl
```

template、executor 注册及所有公共类型均使用目标命名空间。

### 4.2 TemplateResource 接口适配

源仓库 `KernelRun` 接口为：

```cpp
TemplateResource& templateResource
```

目标仓 `InsAlgTemplateBase` 接口为：

```cpp
const TemplateResource& templateResource
```

NHR 运行过程只读取 channel 和 thread，不修改资源对象，因此将参数适配为 `const TemplateResource&`，算法行为不变。

### 4.3 数据分流接口适配

源仓库基类提供 `CalcDataSplitByPortGroup` 和 `channelsPerRank_`。目标仓基类没有这两个成员，但已有等价公共函数 `CalcDataSplitByPortGroupCommon` 和 `CalcChannelsPerRank`。

本次适配为：

- 在 NHR template 内保存 `channelsPerRank_`。
- `CalcRes` 使用 `CalcChannelsPerRank` 计算每个对端的 channel 数。
- `PrepareDataSplitForMultiChannel` 直接调用 `CalcDataSplitByPortGroupCommon`。

该改动仅补齐目标公共基类差异，不改变按 port group 比例分流数据的规则。

### 4.4 Channel 能力适配

源实现对 `MESH_1D_CLOS && !level0PcieMix` 使用 `CalcChannelRequestNhrMultiJetty`，并筛选 `COMM_PROTOCOL_UBC_CTP` channel。目标仓没有该 multi-jetty 接口。

本次只调用目标仓已有 `CalcChannelRequestNhr`：

- 每个 NHR 对端申请一条符合当前通信引擎协议的 channel。
- 保留 template 的 `channelIdx`、数据分流、线程和 notify 结构。
- 当前实际 `channelsPerRank_` 为 1，不额外迁移 multi-jetty 公共框架。

这是本次迁移与源实现最主要的能力边界。若后续需要 multi-jetty 性能能力，应作为独立需求迁移 channel 公共接口，不应混入本次仅迁移 `InsAllReduceNHR` 的范围。

### 4.5 SendRecvInfo 适配

目标仓 `SendRecvInfo` 支持显式数据类型参数。AllGather 构造 `SendRecvInfo` 时写入 `dataType_`，与目标 wrapper 的数据描述保持一致，不改变收发地址和切片计算。

### 4.6 单 Rank 适配

单 rank 不需要 NHR channel。为支持 MC2 通过 `algConfig` 显式指定 `InsAllReduceNHR`，`KernelRun` 在 `templateRankSize_ == 1` 时直接执行 input 到 output 的本地复制，不进入 channel 数据分流和 NHR step。

## 5. MC2 使用方式

### 5.1 默认自动选择

MC2 AllReduce 使用空算法配置时进入本地 selector：

```cpp
uint32_t opType = HCCL_CMD_ALLREDUCE;
std::string algConfig = "";
uint32_t reduceType = HCCL_REDUCE_SUM;
AscendC::Mc2CcTilingConfig config(group, opType, algConfig, reduceType);
```

满足第 3.3 节 NHR 条件时，Host 保存的 `OpParam.algName` 为 `InsAllReduceNHR`，Device 根据同一算法名从本地 registry 获取 executor。

### 5.2 强制选择

可使用 MC2 已有强制算法配置验证 NHR：

```cpp
uint32_t opType = HCCL_CMD_ALLREDUCE;
std::string algConfig = "InsAllReduceNHR";
uint32_t reduceType = HCCL_REDUCE_SUM;
AscendC::Mc2CcTilingConfig config(group, opType, algConfig, reduceType);
```

Host 调用链：

```text
HcclAllocComResourceByTiling
  -> GetOpParam
    -> TryForcedAlgAndPrepareEngine
      -> CollAlgExecRegistryV2::GetAlgExec
      -> InsV2AllReduceSoleExecutor::CalcAlgHierarchyInfo
      -> InsV2AllReduceSoleExecutor::CalcRes
      -> InsTempAllReduceNHR::CalcRes
```

Device 调用链：

```text
AscendC::Hccl::AllReduce
  -> AICore/KFC shared message
    -> CreateOpParamByBaseOpParam
      -> CollAlgExecRegistryV2::GetAlgExec(HCCL_CMD_ALLREDUCE, InsAllReduceNHR)
        -> InsV2AllReduceSoleExecutor::Orchestrate
          -> InsTempAllReduceNHR::KernelRun
```

强制算法会绕过 selector 的类型和拓扑分流，调用方应使用已验证的 NHR 拓扑、数据类型和 reduce op。

## 6. 与 InsAllReduceMesh1DTwoShot 对比

| 对比维度 | `InsAllReduceNHR` | `InsAllReduceMesh1DTwoShot` |
| --- | --- | --- |
| 目标拓扑 | NHR 对端可建链的 CLOS/Mesh1DClos | 单层全连接 Mesh1D，或全连接 PCIE mixed Mesh1DClos |
| 算法阶段 | NHR ReduceScatter + NHR AllGather | Mesh ReduceScatter + Mesh AllGather |
| 每阶段通信步数 | `ceil(log2(rankSize))` | 每 rank 直接遍历全部 rank |
| 每步对端 | 按 `+/- 2^step` 计算特定对端 | 每个远端 rank 均有直接 channel |
| Channel 数 | 当前适配每个 NHR 对端 1 条 | 每个远端 rank 1 条 |
| 执行线程 | 当前适配通常 1 个主线程 | `rankSize` 个线程 |
| Notify | 单 channel 时不需要主从 notify | 每个从线程需要主从同步 notify |
| Scratch 倍数 | 1 | 2 |
| 切片方式 | 前 `N-1` 片向下取整，最后一片收尾 | 按 rank 数向上取整，尾部可出现零长度片 |
| Reduce 位置 | 通信过程中 `SendRecv*Reduce` | 数据汇聚后按固定顺序 `LocalReduce` |
| 通信模式 | PCIe 使用 Read，否则使用 Batch Write | ReduceScatter 主要 Write，AllGather 主要 Read |
| 本地拷贝 | 显式 PreCopy 和 PostCopy | ReduceScatter/AllGather 内按本地 rank 拷贝 |
| 浮点归约顺序 | NHR 分层归约顺序 | 本地固定 slice 顺序归约 |
| 非 2 次幂 rank | 由 NHR step 和 slice 公式处理 | 全连接直接交换，天然覆盖 |
| 资源特点 | 较少线程和直连 channel，增加 step | 较多线程和直连 channel，通信阶段直接 |
| 64 位/PROD | 当前自动选择不支持 | 有 AICPU 本地 reduce 同步分支 |
| MC2 count=0 资源阶段 | 资源计算不依赖真实 count | 资源计算不依赖真实 count |
| 大数据处理 | 复用通用 executor 分轮 | 复用通用 executor 分轮 |

两者共用 `InsV2AllReduceSoleExecutor`，因此大数据分轮、运行时 count 恢复、HCCL buffer 管理及 MC2 Device 分发流程一致。主要差异集中在 template 的拓扑建链、线程资源、切片规则和 ReduceScatter/AllGather 通信方式。

## 7. 支持边界

- 通信引擎：AICPU TS。
- 算子：MC2 AllReduce。
- 自动选择拓扑：单层 CLOS，以及未被 Mesh1DTwoShot 覆盖的单层 Mesh1DClos。
- 自动选择数据类型/reduce op：排除 INT64、UINT64、FP64 和 PROD。
- 强制算法：支持通过 `algConfig="InsAllReduceNHR"` 使用，调用方负责满足算法能力边界。
- Channel：使用目标仓已有单 channel NHR 建链，不包含 multi-jetty。
- 多层、CCU、AIV、DPU、AICPU reduce NHR 和其他 AllReduce 变体不在本次迁移范围内。

## 8. 验证方案

### 8.1 静态检查

```bash
git diff --check
rg -n "InsAllReduceNHR|InsTempAllReduceNHR" \
  impl/adv_api/detail/hccl/cc/src/ops/all_reduce \
  impl/adv_api/detail/hccl/cc/src/CMakeLists.txt
```

预期结果：

- Host 和 Device 构建列表都包含 `ins_temp_all_reduce_nhr.cc`。
- executor 中存在 `InsAllReduceNHR` 注册。
- selector 能返回 `InsAllReduceNHR`。

### 8.2 Host 和 Device 构建

在安装 CANN 构建依赖的 Linux 环境执行：

```bash
bash build.sh --pkg --build-type=Debug -j4
```

构建后检查 client/server 均包含算法名：

```bash
find build build_device -type f \
  \( -name 'libmc2_client.so' -o -name 'libmc2_server.so' \) \
  -exec sh -c 'echo "== $1 =="; strings "$1" | grep InsAllReduceNHR' _ {} \;
```

### 8.3 功能用例

建议至少覆盖：

| 维度 | 用例 |
| --- | --- |
| rank size | 1、2、3、4、7、8 |
| count | 0、1、`rankSize - 1`、`rankSize`、非整除 count、大于单轮容量 |
| datatype | FP16、FP32、BF16、INT8 |
| reduce op | SUM、MAX、MIN |
| buffer | in-place、out-of-place |
| channel | PCIe Read、非 PCIe Batch Write |
| 选择方式 | 默认 selector、`algConfig="InsAllReduceNHR"` |

结果校验应包括：

- 每个 rank 输出包含全部 rank 输入的正确归约结果。
- 非 2 次幂 rank 的 NHR step 不丢失或重复数据片。
- `count < rankSize` 时零长度普通切片和最后尾片处理正确。
- 大数据多轮执行时 input/output offset 正确递增。
- Host 和 Device 使用相同算法名，registry 均命中。

### 8.4 日志检查

建议过滤：

```text
MC2_ALG_INFO
MC2_FORCE_ALG
AllReduceAutoSelector
InsAllReduceNHR
InsV2AllReduceSoleExecutor
CollAlgExecRegistryV2::GetAlgExec
MC2_OPEN_DIAG
MC2_BATCH
```

强制选择路径应看到 `InsAllReduceNHR` accepted，默认 NHR 场景应看到 selector 输出 `Algo match [InsAllReduceNHR]`。Host 和 Device 都应 registry hit，`KernelRun Start` 和 `KernelRun finished` 应成对出现。

## 9. 本地验证结果

- `git diff --check` 通过，仅有工作区 LF/CRLF 提示，无空白错误。
- 已完成源实现、目标公共接口、Host/Device 构建列表和 MC2 分发链路的静态复核。
- 当前 Windows 环境未安装 `bash`、`cmake` 和 `clang-format`，未执行 CANN/Linux 交叉构建和设备功能测试；需在具备 CANN 工具链的环境按第 8 节继续验证。
