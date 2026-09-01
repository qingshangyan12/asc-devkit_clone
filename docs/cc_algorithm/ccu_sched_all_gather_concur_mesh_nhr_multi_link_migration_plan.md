# CcuSchedAllGatherConcurMeshNHRMultiLink 迁移方案与实施记录

## 1. 文档目标

本文给出将 HCCL 中的 `CcuSchedAllGatherConcurMeshNHRMultiLink` 迁移到 asc-devkit MC2/KFC
架构的方案和实施结果。该算法历史名称为 `CcuAllGatherConcurrentMesh1DNHRMem`。

当前已完成 Host 控制面、双 mission KFC Server、Mesh/NHR dispatch、MC2 client 参数准备和相关测试迁移。
真实 CCU 硬件上的数据正确性、稳定性及并发性能尚未验证，不能据本文中的 Host/device UT 结果推断硬件验收通过。

## 2. 分析基线

| 项目 | 值 |
| --- | --- |
| 分析日期 | 2026-08-17，Asia/Shanghai |
| asc-devkit 路径 | `/mnt/c/data/gitcodes/asc-devkit` |
| asc-devkit 分支 | `cc0806_2` |
| asc-devkit HEAD | `a6a39addb161e37ed3332a7a75954eff57087614` |
| HCCL 路径 | `/mnt/c/data/gitcodes/hccl` |
| HCCL 更新前 HEAD | `a5101c3161e975df635df46e296182ca6143340b` |
| HCCL 更新后 HEAD | `52c9634afbe1d2f7e4e508f5efbe0ee34a2e1f18` |
| CANN | `/home/houyu/Ascend/cann-9.2.0` |
| 目标架构 | `ascend950pr_9599`、`SocVersion=ASCEND950`、`__NPU_ARCH__=3510` |
| 目标引擎 | CCU Schedule、MC2/KFC Server |

分析参考：

- `docs/cc_algorithm/ccu_sched_all_gather_sole_mesh_session_memory.md`
- `docs/cc_algorithm/ccu_sched_all_gather_sole_mesh_migration_plan.md`
- `docs/cc_algorithm/mc2_st_test_framework_design.md`
- HCCL 最新 AllGather selector、concurrent executor、Mesh template/kernel 和 NHR MultiJetty template/kernel。

规范名称如下：

| 类型 | 名称 |
| --- | --- |
| HCCL canonical 名称 | `CcuSchedAllGatherConcurMeshNHRMultiLink` |
| 历史名称 | `CcuAllGatherConcurrentMesh1DNHRMem` |
| asc-devkit 参考算法 | `CcuSchedAllGatherSoleMesh` |

生产注册、selector、枚举、CMake 和测试统一使用 canonical 名称。历史名称只允许保留在迁移说明中。

### 2.1 实施状态

| 项目 | 状态 |
| --- | --- |
| HCCL差异分析 | 完成，基于 `52c9634afbe1d2f7e4e508f5efbe0ee34a2e1f18` |
| Host资源和双mission启动 | 完成 |
| MC2 client双start/双done | 完成 |
| Mesh/NHR KFC Server dispatch | 完成 |
| NHR静态step/channel元数据 | 完成 |
| `mc2_client`、Host/device专项构建 | 通过 |
| CCU真机正确性和并发性能 | 待验证 |

实施遵循 Host 优先原则：算法选择、资源边界、mission角色、数据切分、XN参数生成、信号区规划和启动顺序均由
Host/MC2 client确定；CCU Server仅按固定role消费固定参数布局并执行对应subkernel，不增加mission间状态机。

## 3. 算法差异分析

### 3.1 注册和执行结构

`CcuSchedAllGatherSoleMesh` 使用一个 executor template：

```cpp
REGISTER_EXEC_V2(
    HCCL_CMD_ALLGATHER,
    CcuSchedAllGatherSoleMesh,
    InsV2AllGatherSoleExecutor,
    TopoMatch1D,
    CcuTempAllGatherMesh1DMem2Mem);
```

目标算法使用两个 executor template：

```cpp
REGISTER_EXECUTOR_BY_TWO_TEMPS(
    HCCL_CMD_ALLGATHER,
    CcuSchedAllGatherConcurMeshNHRMultiLink,
    InsV2AllGatherConcurrentExecutor,
    TopoMatchUBX,
    CcuTempAllGatherMesh1DMem2Mem,
    CcuTempAllGatherNHR1DMultiJettyMem2Mem);
```

两者不是单纯的拓扑分支差异。目标算法会为 Mesh 和 NHR 分别生成一个原生 CCU kernel，并通过两个
template thread 并行执行不同数据段。

### 3.2 资源差异

| 项目 | `CcuSchedAllGatherSoleMesh` | `CcuSchedAllGatherConcurMeshNHRMultiLink` |
| --- | --- | --- |
| template 数 | 1，Mesh | 2，Mesh + NHR MultiJetty |
| 原生 CCU kernel 数 | 1 | 2，`ccuKernelNum={1,1}` |
| executor thread | 1 | 2 个 template thread |
| slave thread | 0 | 1 |
| main thread notify | 0 | 1 |
| per-thread notify | 空 | `{1}` |
| kernel静态资源 | Mesh rank/channel | Mesh资源 + NHR step/channel资源 |
| scratch | 0 | 两个template均为0 |
| topology matcher | `TopoMatch1D` | `TopoMatchUBX` |

Concurrent executor 的资源合并公式为：

```text
slaveThreadNum = mesh.slaveThreadNum + nhr.slaveThreadNum + 1
notifyNumOnMainThread = mesh.notifyNumOnMainThread + 1
notifyNumPerThread = mesh.notifyNumPerThread
                   + {nhr.notifyNumOnMainThread + 1}
                   + nhr.notifyNumPerThread
ccuKernelNum = {mesh.ccuKernelNum[0], nhr.ccuKernelNum[0]}
```

当前两个 CCU template 自身都不申请从 thread，因此最终为一个主 thread、一个从 thread以及一对前后同步
notify。

### 3.3 拓扑和channel差异

SoleMesh 面向单一 `MESH_1D` 子通信域，使用 full-mesh channel，每个rank与其余rank通信。

Concurrent Mesh+NHR 面向 UBX `MESH_1D_CLOS`：

- `algHierarchyInfo.infos[0][0]` 必须为 Mesh 子通信域。
- `algHierarchyInfo.infos[0][1]` 必须为 Clos/NHR 子通信域。
- Mesh template 独立申请 Mesh priority channel。
- NHR template 使用 `CalcChannelRequestNhrMultiJetty` 申请每一步的NHR channel。
- 最新 `TopoMatchUBX` 按拓扑类型组织实例；不能依赖拓扑实例输入顺序。

asc-devkit 当前拓扑匹配实现存在按输入顺序追加实例的行为。迁移时必须同步为按类型稳定排序，否则两个
template可能拿到相反的子通信域。

### 3.4 数据切分和并发关系

CCU 路径使用 Mesh带宽和Clos带宽常量计算比例，当前值为 `11:10`：

```text
meshCount = floor(totalCount * 11 / 21)
meshCount = align_down(meshCount, 128 / dataTypeSize)
nhrCount = totalCount - meshCount
nhrOffset = meshCount * dataTypeSize
```

两个template分别计算loop和tail，executor在同一循环中交替下发Mesh和NHR kernel。在首轮下发前执行
`PreSyncInterThreads`，全部任务下发后执行 `PostSyncInterThreads`。

这两个kernel覆盖互不重叠的数据范围，并发是算法的核心性能语义。把Mesh和NHR串行放进一个普通KFC
Server分支只能作为正确性降级方案，不能以canonical并发算法名交付。

### 3.5 动态kernel参数差异

Mesh原生kernel使用15个动态参数：

```text
input, output, token,
currentRankSliceInputOffset, currentRankSliceOutputOffset,
repeatNum,
inputRepeatStride, outputRepeatStride,
normalSliceSize, lastSliceSize,
isInputOutputEqual,
goSize[0], goSize[1], goSize[2], goSize[3]
```

NHR原生kernel使用16个动态参数：

```text
input, output, token,
sliceSize, sliceSizePerJetty, lastSliceSizePerJetty,
repeatNumInv,
inputSliceStride, outputSliceStride,
inputRepeatStride, outputRepeatStride,
isInputOutputEqual,
goSize[0], goSize[1], goSize[2], goSize[3]
```

NHR还需要在kernel生成阶段固化以下静态元数据：

- `rankSize`、`rankId`。
- `jettyNum`。
- `stepInfoVector`。
- `rank2ChannelIdx`。
- `subCommRanks`。

NHR step数为 `ceil(log2(rankSize))`。最新HCCL的MultiJetty实现当前使用 `JETTY_NUM=1`；asc-devkit
遗留NHR源码中的4不能继续沿用。

## 4. asc-devkit现有KFC限制

当前KFC Server框架只适配单末端CCU kernel，不能直接承载目标算法：

1. `CcuTempKfcServer::CalcRes` 只检查和替换 `ccuKernelInfos.back()`。
2. 原始kernel名称校验只识别已经迁移的SoleMesh AllGather kernel。
3. KFC adapter只继承一套channel和静态kernel参数。
4. `HcclGetThread` 对KFC路径只保留一个CCU thread。
5. `CcuKernelLaunch` 只启动 `threads[0]` 和 `ccuKernels[0]`。
6. `kfcServerArgs` 固定为 `missionNum=1`、`missionIndex=0`。
7. Server内部多mission同步仍为TODO或空实现。
8. `WaitTurnStartSig` 会复位共享start信号，两个Server复用同一信号会产生竞争或死锁。
9. Client每轮只发送一个start并等待一个completion。
10. 当前Server cache key只覆盖 `algTag` 和有序opId，没有覆盖算法名与mission role布局。

如果直接把 `{Mesh,NHR}` 注册到现有路径，adapter只会把最后一个NHR kernel替换成Server，Mesh kernel仍保留为
原生kernel；随后launch路径又只启动一个Server，结果是Mesh数据段没有执行。这种失败可能不会在host资源构造
阶段直接报错，因此必须作为架构门禁处理。

## 5. 迁移范围

### 5.1 范围内

- CCU Schedule、MC2和KFC Server路径。
- 单层、单die、UBX `MESH_1D_CLOS`。
- Mesh和Clos子通信域数量相等。
- rank 2至4。
- `dataSize > 512 KiB`。
- input/output不overlap。
- 最新HCCL中的Mesh/NHR数据比例、对齐、channel和NHR step逻辑。
- MC2 tiling起步的CCU host ST。
- CCU device编译和IR检查。
- 真机正确性与并发性能验收方案。

### 5.2 范围外

- 双die。
- 多层、RoCE和跨Server拓扑。
- Ascend 960专用算法。
- AICPU算法变更。
- DPU。
- `CcuSchedAllGatherParallelMeshNHRMultiLink`。
- `CcuSchedAllGatherPipeLineMeshNHR`。
- HalfAllToAllV及其他相邻算法。
- 未经真机确认的自定义非连续stride。

本阶段不能把范围外算法名加入可达selector分支。对于不满足Concurrent条件的拓扑，应保留asc-devkit当前已注册
fallback或返回 `NOT_MATCH`，不能返回尚未迁移的Parallel/Pipeline算法名。

## 6. 目标架构

### 6.1 双mission KFC模型

一个MC2 AllGather逻辑消息由两个常驻KFC Server mission共同处理：

| mission | role | 原始kernel | 数据范围 |
| --- | --- | --- | --- |
| 0 | Mesh | `CcuAllGatherMesh1DMem2MemKernel` | 对齐后的前半段 |
| 1 | NHR | `CcuAllGatherNHR1DMultiJettyMem2MemKernel` | 剩余数据段 |

资源上下文必须保留：

- 两个CCU thread。
- 两个Server kernel handle。
- `ccuKernelNum={1,1}` 的template边界。
- 两套独立channel和静态kernel元数据。
- 两组六字段Server启动参数。
- `missionNum=2`，`missionIndex=0/1`。

两个mission属于同一个资源组并同时常驻，避免因拆成两个独立资源上下文而破坏并发资源核算。

### 6.2 信号和队列协议

每个队列turn需要独立的mission信号槽：

```text
client write logical message
        |
        +--> post start[mission 0] --> Mesh Server --> post done[mission 0]
        |
        +--> post start[mission 1] --> NHR Server  --> post done[mission 1]
        |
        +--> wait done[0] and done[1] --> finish logical turn
```

落地协议如下：

- Host分配4个CKE区域，顺序为Mesh start/done、NHR start/done。
- 两个mission读取同一个XN逻辑槽，但分别维护本地队列游标并使用独立start/done地址。
- MC2 client先写入一次XN参数，再依次post两个start；只有两个done均到达后才完成逻辑turn并复用槽位。
- `Finalize`向两个mission发送相同终止消息。
- 两个Server不执行mission间同步，也不共享需要原子更新的队列owner状态，避免把复杂协调逻辑放到CCU硬件侧。
- Host cache identity固定mission数量、顺序和role，Host启动时校验两组thread/kernel/args完整性。

当前协议没有设备异常状态聚合通道。任一CCU mission异常时的超时和错误上报能力依赖运行时/硬件机制，列入真机
稳定性验收，不在Host UT中宣称已覆盖。

### 6.3 Server role和dispatch

KFC Server静态参数增加role枚举，例如：

```text
KFC_ROLE_DEFAULT
KFC_ROLE_ALLGATHER_MESH
KFC_ROLE_ALLGATHER_NHR
```

AllGather dispatch同时使用 `opId` 和静态role：

```text
(opId, ALLGATHER_MESH) -> KFC Mesh subkernel
(opId, ALLGATHER_NHR)  -> KFC NHR subkernel
```

设备分支继续使用 `CCU_IF` 生成IR，不使用普通运行时C++ `switch` 替代。

### 6.4 动态参数合同

在现有32个动态参数槽内定义一个公共/superset布局。实际使用索引 `0..24`，包括：

- opId。
- input/output基地址、token。
- count、dtype size和rank slice stride。
- repeat及repeat stride。
- Mesh offset、normal size、tail size、同址标记和 `goSize`。
- NHR offset、slice size、per-jetty size、last-jetty size、同址标记和 `goSize`。

参数下标定义在轻量共享头 `common/kfc_server_message.h`，由MC2 client prepare和KFC Server dispatch共同引用；
编译期断言保证25个参数不超过单mission的32个XN槽。协议版本只参与Host cache identity，不占用动态XN槽。

MC2继续采用“每个repeat对应一个逻辑KFC消息”的既有协议。不能同时在Client拆repeat、又把HCCL原生repeat
循环复制到子kernel，否则会重复执行数据范围。

### 6.5 Cache身份

Server资源cache key扩展为以下有序信息：

```text
algTag
canonical algName
missionNum
ordered mission roles
ordered opIds
protocol version
```

同一MC2 context中，不允许两个不同AllGather算法使用相同opId并共享Server，除非后续显式扩展opId编码。

## 7. 分阶段实施方案

### 7.1 阶段A：双mission KFC公共框架

1. 扩展资源请求和序列化结构，保存多组KFC thread、kernel和Server args。已完成。
2. 修改KFC资源分配，不再强制折叠为一个thread/kernel。已完成。
3. 扩展 `CcuKernelLaunch`，按mission顺序启动两个常驻Server。已完成。
4. 实现独立start/done信号地址计算和双completion等待。已完成。
5. 两个Server各自推进相同槽号的独立队列，不引入CCU mission间同步。已完成。
6. 扩展cache key和cache冲突校验。已完成。
7. 保证现有单mission KFC算法仍使用 `missionNum=1`，无行为回归。已完成Host/device回归。

### 7.2 阶段B：selector和TopoMatchUBX

1. 在生产selector中增加目标算法的精确命中条件：
   - CCU Schedule。
   - 单层 `MESH_1D_CLOS`。
   - Mesh/Clos数量相等。
   - `dataSize > 512 KiB`。
   - `userRankSize <= 4`。
   - input/output不overlap。
2. 小于等于512 KiB时继续选择 `CcuSchedAllGatherSoleMesh`。
3. forced algorithm路径执行同样的拓扑和参数校验。
4. 不引入尚未迁移的Parallel/Pipeline可达分支。
5. 同步 `TopoMatchUBX`，按topology type确定Mesh和Clos子通信域顺序。

### 7.3 阶段C：双template executor

1. 同步 `InsV2AllGatherConcurrentExecutor` 的目标依赖闭包。
2. 使用 `REGISTER_EXECUTOR_BY_TWO_TEMPS` 注册canonical名称。
3. 在 `CalcRes` 中分别计算Mesh和NHR资源，并保留 `ccuKernelNum={1,1}`。
4. 按HCCL公式合并thread和notify，禁止硬编码错误的资源边界。
5. 保留 `PreSyncInterThreads`、交替下发和 `PostSyncInterThreads`。
6. 同步11:10数据比例、128B向下对齐、offset、loop和tail。
7. 两个template的scratch multiplier均为0，但保留通用executor检查。
8. 保留fast-launch上下文中的两个kernel分组，适配为两个KFC mission。

### 7.4 阶段D：Mesh/NHR KFC数据面

Mesh role：

- 复用已经迁移的SoleMesh KFC AllGather subkernel。
- 接入分段后的offset、normal size和tail size。
- 保留 `isInputOutputEqual` 和 `goSize` 行为。

NHR role：

- 同步最新 `CalcChannelRequestNhrMultiJetty`。
- 使用最新HCCL的 `JETTY_NUM=1`。
- 生成 `stepInfoVector` 和 `rank2ChannelIdx`。
- 同步per-jetty切分、last-jetty尾块和 `repeatNumInv`。
- 同步local-slice equality和 `goSize`。
- 将NHR channel和静态step元数据绑定到mission 1 Server kernel。

asc-devkit现有未进入生产CMake的NHR直接launch源码只能作为差异参考。不能直接启用旧源码并绕开KFC Server。

### 7.5 阶段E：注册和构建闭环

同步以下注册面：

- Tiling算法allowlist。
- host侧 `AlgorithmType`。
- device侧 `AlgorithmType`。
- `algorithmMap`。
- KFC支持算法列表。
- 原始kernel名称到Server role的映射。
- Server dispatch include和分支。
- executor、template、kernel及测试CMake。

Host枚举原本已按操作类型使用AllGather `0..`、ReduceScatter `50..`、AllReduce `100..` 和AllToAll
`150..` 的分段稳定值。目标算法使用AllGather段内空闲值3；device枚举同步补齐50/100/150的显式分段值，避免
新增值3与旧的顺序枚举ReduceScatter发生碰撞。构建后使用 `build.make` 或 `DependInfo.cmake` 证明新增源码真实
进入 `mc2_client` 和host ST目标。

### 7.6 阶段F：stride语义

asc-devkit现有Concurrent executor已将 `strideCount` 传播为 `outputSliceStride`。本次MC2 client参数准备保持该合同：

- `strideCount == 0` 时按 `count * dtypeSize` 推导连续stride。
- 非零stride按 `strideCount * dtypeSize` 写入Mesh输出offset和NHR输出slice stride。
- 真机需要分别验证默认和非零stride；在真机通过前，不扩大支持范围声明。

## 8. Source-to-target清单

| 层级 | HCCL源参考 | asc-devkit目标或改造点 |
| --- | --- | --- |
| Selector | `src/ops/all_gather/selector/all_gather_auto_selector.cc` | 同路径生产selector |
| Topology | `src/ops/op_common/topo_match/`中的 `TopoMatchUBX` | 同步UBX分组顺序和校验 |
| Executor | `src/ops/all_gather/executor/ins_v2_all_gather_concurrent_executor.*` | 迁移并接入KFC双mission |
| Mesh template | `src/ops/all_gather/template/ccu/ccu_temp_all_gather_mesh_1D_mem2mem.*` | 复用现有迁移并扩展分段参数 |
| NHR template | `src/ops/all_gather/template/ccu/ccu_temp_all_gather_nhr_1D_multi_jetty_mem2mem.*` | 同步最新资源和静态元数据 |
| Mesh kernel | `src/ops/all_gather/template/ccu/kernel/ccu_kernel_all_gather_mesh1d_mem2mem.*` | 现有KFC Mesh subkernel |
| NHR kernel | `src/ops/all_gather/template/ccu/kernel/ccu_kernel_all_gather_nhr1d_multi_jetty_mem2mem.*` | 新增/同步KFC NHR subkernel |
| KFC adapter | 不适用，HCCL为直接launch | `ccu_temp_kfc_server.*` |
| KFC dispatch | 不适用 | `ccu_kernel_kfc_server.*` |
| Client prepare | 不适用 | `hccl_ccu_v0_prepare.h` |
| Client queue | 不适用 | `hccl_ccu_v0.h`及definition |
| 资源序列化 | HCCL原生resource ctx | `hccl_alloc_ctx_res.*` |
| Server launch | HCCL fast/direct launch | `hccl_mc2.cc` |
| Tiling映射 | 不适用 | `impl/adv_api/tiling/hccl/hccl_tiling.cpp` |
| Host ST | 不适用 | `tests/.../mc2_ccu/host/` |

不得迁移或重新引入HCCL的 `all_gather_op.h/.cc` legacy host入口。所有新增ST必须从MC2 tiling开始。

## 9. 测试方案

### 9.1 Production selector测试

至少覆盖：

| 场景 | 预期 |
| --- | --- |
| 512 KiB | `CcuSchedAllGatherSoleMesh` |
| 512 KiB + 1 | 满足其他条件时命中Concurrent |
| rank 2、4 | 命中Concurrent |
| rank 5 | 不命中Concurrent |
| Mesh/Clos数量不等 | 不命中Concurrent |
| `MESH_1D`非UBX | 不命中Concurrent |
| PCIe/UB混合channel | 拒绝或fallback |
| input/output overlap | 拒绝Concurrent |
| forced算法但拓扑不匹配 | 参数错误或 `NOT_MATCH` |

### 9.2 MC2 CCU host ST

从 `Mc2InitTilingInner` 和 `Mc2CcTilingInner` 构造输入，调用产品
`HcclAllocComResourceByTiling`，断言：

- 实际算法名为canonical名称。
- `AlgorithmType`和 `algorithmMap`一致。
- `isKfc`为真。
- 两个thread、两个Server kernel。
- `ccuKernelNum={1,1}`。
- mission role顺序为Mesh、NHR。
- 两套channel互不混用。
- NHR `stepInfoVector`、`rank2ChannelIdx`、`jettyNum=1`正确。
- `missionNum=2`、`missionIndex=0/1`。
- 每个Server均有完整六字段启动参数。
- canonical算法与SoleMesh的cache不能碰撞。
- 相同opId、不同role可以在同一算法上下文中共存。
- 相同opId、不同AllGather算法配置被拒绝或隔离。

当前Host ST从MC2 tiling进入真实 `HcclAllocComResourceByTiling`、selector、资源序列化和launch路径，但
`HcclGetAlgRes`使用测试资源桩生成thread/kernel handle。生产双template资源通过 `mc2_client` 编译、注册和
`CcuTempKfcServer::CalcRes`约束闭环验证；真实channel申请和CCU kernel注册仍需真机验证。测试报告必须继续说明
这一真实性边界。

### 9.3 双mission协议UT

至少覆盖：

- Client同时post两个start。
- Client等待两个done。
- mission 0和1使用不同信号地址。
- 两个mission读取同一队列turn。
- 64槽队列回绕。
- 连续多repeat。
- mission 0先完成和mission 1先完成两种顺序。
- 任一mission失败时错误可返回且不死锁。
- 单mission旧算法仍按原协议工作。
- cache配置顺序变化和role变化能够隔离。

### 9.4 Device编译和IR检查

没有CCU仿真器时，只验证：

- Mesh/NHR两个dispatch分支均进入设备编译。
- `CCU_IF`按opId和role生成分支。
- 两个Server使用不同mission信号地址。
- NHR各step的channel和event引用已生成。
- 静态参数和动态参数数量不越界。

不得把device编译成功描述为数据正确性E2E通过。

### 9.5 真机验收

真机测试覆盖：

- rank 2、4。
- 512 KiB阈值前后。
- 128B切分边界前后。
- Mesh段或NHR段为0的极小边界保护。
- 支持的全部dtype。
- 单repeat和多repeat。
- 整块和tail。
- input/output非原地及overlap拒绝。
- 默认连续stride和非法自定义stride。
- Mesh/NHR结果拼接后的完整AllGather语义。
- 两个Server实际同时活跃，没有串行退化。
- 与SoleMesh及HCCL原生算法对比带宽和时延。

### 9.6 已执行验证

| 目标或用例 | 结果 | 结论边界 |
| --- | --- | --- |
| `mc2_client` | 构建通过 | 新executor/template/kernel和KFC dispatch进入生产构建 |
| `ascendc_ut_adv_api_hccl_ccu_mc2` | 30/30通过 | 公共资源、算法枚举和映射回归 |
| `ascendc_ut_adv_api_hccl_ccu_mc2_host` | 36/36通过 | MC2 Host控制面；含15个参数化AllGather用例 |
| `ascendc_ut_adv_api_hccl_ccu_mc2_device` | 18/18通过 | Server IR/队列既有用例通过，NHR subkernel真实参与编译链接 |
| `ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC` | 86/86通过 | 公共MC2改动未破坏AICPU算法回归 |

Host AllGather专项覆盖selector阈值、Mesh/Clos数量门禁、SoleMesh cache隔离、双thread、双kernel、
`ccuKernelNum={1,1}`、两组六字段启动参数、token传播、两次真实Host launch记录和混合上下文拒绝。

`bash build.sh --adv_test_two -j4` 的组合目标包含范围外 `ascend610` tiling UT，并被既有Conv3D tiling缺失链接符号
阻断。本次按需求忽略 `ascend610`，不修改相关代码；上述 `ascend950pr_9599` HCCL专项均独立通过。

## 10. 构建和覆盖率

实施时先执行窄目标，再执行相关回归：

```bash
source /home/houyu/Ascend/cann/set_env.sh

cmake --build build --target mc2_client -- -j4
cmake --build build --target ascendc_ut_adv_api_hccl_ccu_mc2_host -- -j4
cmake --build build --target ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC -- -j4
```

专项GTest通过后，已按迁移skill要求独立清零、运行24个AllGather Host用例并采集CCU Host覆盖率。报告目录为：

```text
build/coverage/ccu_sched_all_gather_concur_mesh_nhr_multi_link/
```

已生成以下交付文件：

- `coverage.info`。
- `production_coverage.info`。
- `html/index.html`。
- `html_production/index.html`。
- `line_coverage.tsv`。
- `test.log`。
- `coverage_summary.txt`。

覆盖率主指标只纳入本次相关asc-devkit Host生产文件，不把测试桩和测试文件自身覆盖率混入生产指标：5个文件
行覆盖率50.8%（539/1060），函数覆盖率58.8%（50/85）。其中 `kfc_server_protocol.h` 为100%，
`kfc_server_config.h` 为96.2%，AllGather selector为65.9%，`hccl_mc2.cc` 为50.5%，
`hccl_alloc_ctx_res.cc` 为44.7%。新CCU template/kernel不属于Host覆盖率数据集；其编译链接已由device目标验证，
执行和数据语义仍必须由真机验证。

## 11. 交付阶段和提交建议

### 阶段A：公共协议

- 双mission资源、序列化、信号、队列和cache。
- 单mission回归与协议UT。

### 阶段B：算法接线

- Selector、TopoMatchUBX、Concurrent executor。
- Mesh/NHR KFC role和subkernel。
- 注册、枚举、CMake和tiling allowlist。

### 阶段C：Host验证

- Production selector测试。
- MC2 tiling host ST。
- 真实executor `CalcRes`窄测试。
- 专项覆盖率和完整相关回归。

### 阶段D：硬件验收

- 数据正确性。
- signal和队列稳定性。
- 双kernel并发证据。
- 与HCCL原生算法的性能对比。

公共协议和具体算法建议拆分提交，便于确认双mission改造对已有KFC算法的影响。

## 12. 风险和门禁

| 风险 | 门禁或处理方式 |
| --- | --- |
| 现有adapter只替换最后一个kernel | 目标注册前完成双kernel替换和资源断言 |
| 两个Server复用start信号死锁 | Host规划每mission独立start/done地址，Server独立推进队列 |
| topology实例顺序不稳定 | `TopoMatchUBX`按类型排序并断言Mesh/NHR顺序 |
| 只执行NHR数据段但host返回成功 | ST断言两个role、两个kernel和完整切分范围 |
| 单Server串行实现冒充并发 | canonical算法验收必须提供双Server同时活跃证据 |
| 遗留 `JETTY_NUM=4` | 同步最新HCCL值1并增加静态元数据断言 |
| 参数下标漂移 | MC2 client/Server共享轻量XN ABI枚举并做编译期范围检查 |
| cache误复用其他AllGather算法 | cache key加入算法名、mission布局和协议版本 |
| selector返回未迁移算法 | 不引入Parallel/Pipeline可达分支 |
| HCCL stride语义不明确 | 第一阶段仅允许默认连续stride |
| host ST使用executor桩 | 增加生产 `CalcRes`窄测试并明确结论边界 |
| device编译被误认为正确性通过 | 正确性和并发性能只由真机验收确认 |

## 13. 完成标准和剩余门禁

Host控制面迁移已满足以下条件：

1. canonical名称已传播到selector、注册、枚举、映射、CMake、ST和文档。
2. 旧名称只存在于历史说明。
3. 目标selector精确命中，且不会返回范围外未注册算法。
4. `TopoMatchUBX`稳定输出Mesh、NHR子通信域顺序。
5. MC2资源中存在两个thread、两个Server kernel和 `{1,1}` kernel边界。
6. 两个mission拥有独立channel静态元数据和独立信号区。
7. Client对一个逻辑消息启动并等待两个mission。
8. Host按11:10和128B对齐生成Mesh/NHR互不重叠的参数范围。
9. 单mission KFC算法及AICPU相关回归通过。
10. `mc2_client`、Host/device专项和 `ascend950pr_9599` 算法目标通过。

以下门禁仍需真机环境完成，未完成前不得声明整体硬件迁移验收通过：

1. CCU实际channel资源申请、kernel注册和双Server常驻成功。
2. Mesh/NHR拼接后的完整AllGather数据语义正确。
3. repeat、tail、默认/非零stride和64槽回绕稳定。
4. mission异常时的超时和错误恢复符合运行时要求。
5. 两个CCU kernel存在实际并行执行证据，并达到性能预期。
