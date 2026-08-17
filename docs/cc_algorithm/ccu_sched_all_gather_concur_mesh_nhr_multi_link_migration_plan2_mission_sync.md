# CcuSchedAllGatherConcurMeshNHRMultiLink 迁移方案二：CCU mission同步

## 1. 文档目标

本文给出将HCCL中的 `CcuSchedAllGatherConcurMeshNHRMultiLink` 迁移到asc-devkit MC2/KFC架构的
第二套方案和实施记录。该算法历史名称为 `CcuAllGatherConcurrentMesh1DNHRMem`。

本文以
`ccu_sched_all_gather_concur_mesh_nhr_multi_link_migration_plan.md` 为算法分析和迁移范围基线，保留其中
selector、双template、资源、参数、Mesh/NHR数据面和测试边界，仅替换双mission的控制协议：

- 方案一遵循原则X，即Host优先、双独立队列、不做CCU mission间同步。
- 方案二取消原则X，由mission 0作为队列owner，并通过共享HBM信号与mission 1同步。
- Host对一个逻辑消息只发送一次start并等待一次done。
- 两个CCU kernel、两个CCU thread、两套channel和两组六字段启动参数保持不变。

方案一已固定在分支 `cc0806_2`、提交 `c1f2a9327` 和远端 `origin/cc0806_2`。方案二在分支
`ccuCurMisSyn` 上实施，便于独立评审和对比。

真实CCU硬件上的数据正确性、稳定性及并发性能尚未验证。Host/device UT和IR检查通过不能替代真机验收。

## 2. 分析基线

| 项目 | 值 |
| --- | --- |
| 分析日期 | 2026-08-17，Asia/Shanghai |
| asc-devkit路径 | `/mnt/c/data/gitcodes/asc-devkit` |
| 方案二分支 | `ccuCurMisSyn` |
| 方案二基线提交 | `c1f2a9327` |
| 方案一远端 | `origin/cc0806_2` |
| HCCL路径 | `/mnt/c/data/gitcodes/hccl` |
| HCCL HEAD | `52c9634afbe1d2f7e4e508f5efbe0ee34a2e1f18` |
| CANN | `/home/houyu/Ascend/cann-9.2.0` |
| 目标架构 | `ascend950pr_9599`、`SocVersion=ASCEND950`、`__NPU_ARCH__=3510` |
| 目标引擎 | CCU Schedule、MC2/KFC Server |

分析参考：

- `docs/cc_algorithm/ccu_sched_all_gather_sole_mesh_session_memory.md`。
- `docs/cc_algorithm/ccu_sched_all_gather_sole_mesh_migration_plan.md`。
- `docs/cc_algorithm/mc2_st_test_framework_design.md`。
- 方案一迁移文档和实现。
- HCCL最新AllGather selector、concurrent executor、Mesh template/kernel和NHR MultiJetty template/kernel。

规范名称如下：

| 类型 | 名称 |
| --- | --- |
| HCCL canonical名称 | `CcuSchedAllGatherConcurMeshNHRMultiLink` |
| 历史名称 | `CcuAllGatherConcurrentMesh1DNHRMem` |
| asc-devkit参考算法 | `CcuSchedAllGatherSoleMesh` |

生产注册、selector、枚举、CMake和测试继续使用canonical名称。历史名称只保留在迁移说明中。

## 3. 算法和资源差异

### 3.1 单template与双template

`CcuSchedAllGatherSoleMesh` 使用一个Mesh executor template。目标算法使用Concurrent executor和两个
template：

```cpp
REGISTER_EXECUTOR_BY_TWO_TEMPS(
    HCCL_CMD_ALLGATHER,
    CcuSchedAllGatherConcurMeshNHRMultiLink,
    InsV2AllGatherConcurrentExecutor,
    TopoMatchUBX,
    CcuTempAllGatherMesh1DMem2Mem,
    CcuTempAllGatherNHR1DMultiJettyMem2Mem);
```

目标算法为Mesh和NHR分别生成一个原生CCU kernel，并通过两个template thread并行执行不同数据段。因此不能
把NHR作为SoleMesh Server内的串行分支交付。

### 3.2 资源差异

| 项目 | SoleMesh | Concurrent Mesh+NHR |
| --- | --- | --- |
| template数 | 1，Mesh | 2，Mesh + NHR MultiJetty |
| 原生CCU kernel数 | 1 | 2，`ccuKernelNum={1,1}` |
| executor thread | 1 | 2 |
| slave thread | 0 | 1 |
| main thread notify | 0 | 1 |
| per-thread notify | 空 | `{1}` |
| kernel静态资源 | Mesh rank/channel | Mesh资源 + NHR step/channel资源 |
| scratch | 0 | 两个template均为0 |
| topology matcher | `TopoMatch1D` | `TopoMatchUBX` |

Concurrent executor的资源合并公式保持与HCCL一致：

```text
slaveThreadNum = mesh.slaveThreadNum + nhr.slaveThreadNum + 1
notifyNumOnMainThread = mesh.notifyNumOnMainThread + 1
notifyNumPerThread = mesh.notifyNumPerThread
                   + {nhr.notifyNumOnMainThread + 1}
                   + nhr.notifyNumPerThread
ccuKernelNum = {mesh.ccuKernelNum[0], nhr.ccuKernelNum[0]}
```

HCCL中的notify表达原生两个template thread之间的前后同步；asc-devkit MC2/KFC改造后使用四个共享HBM信号
区域表达常驻Server之间的每轮握手，不能把两者误认为两个额外Host队列。

### 3.3 拓扑、切分和参数

目标拓扑为单层、单die UBX `MESH_1D_CLOS`。Mesh和Clos子通信域数量必须相等，`TopoMatchUBX`按拓扑
类型稳定排列子通信域。

数据切分保持HCCL的11:10带宽比例和128B向下对齐：

```text
meshCount = floor(totalCount * 11 / 21)
meshCount = align_down(meshCount, 128 / dataTypeSize)
nhrCount = totalCount - meshCount
nhrOffset = meshCount * dataTypeSize
```

共享XN布局使用索引 `0..24`。Mesh动态参数和NHR动态参数、`JETTY_NUM=1`、NHR step信息、
`rank2ChannelIdx`、`subCommRanks`均沿用方案一，不因同步协议变化而复制或重排。

## 4. 方案二设计原则

1. mission 0是唯一Host队列owner，负责64槽游标和Host完成语义。
2. mission 1不读取Host start，也不向Host写done，只跟随mission 0的轮次。
3. 两个mission读取同一个XN槽，避免复制25个动态参数或增加第二套消息区。
4. mission 0在执行Mesh前唤醒mission 1，使Mesh和NHR仍可并发执行。
5. mission 0只有在自身Mesh完成且收到NHR done后才向Host写done。
6. 正常消息和 `INVALID_U64` 终止消息使用同一mission握手，防止worker无法退出。
7. 默认单mission KFC算法继续走原有队列函数，不进入mission同步分支。
8. 不新增CCU硬件notify API；同步仅复用已经分配的HBM/CKE内存和既有 `ccu::Load/Store` 原语。

第8点限制了硬件侧改动范围：本方案仅增加固定地址计算和轮询/写信号，不增加复杂状态机、原子计数器、动态
资源申请或新的CCU API组合。

## 5. 目标架构

### 5.1 双mission角色

| mission | role | 原始kernel | 队列职责 | 数据范围 |
| --- | --- | --- | --- | --- |
| 0 | Mesh owner | `CcuAllGatherMesh1DMem2MemKernel` | Host入口、worker协调、Host出口 | 对齐后的前段 |
| 1 | NHR worker | `CcuAllGatherNHR1DMultiJettyMem2MemKernel` | 跟随owner | 剩余数据段 |

资源上下文保留：

- 两个CCU thread。
- 两个Server kernel handle。
- `ccuKernelNum={1,1}` 的template边界。
- 两套独立channel和静态kernel元数据。
- 两组六字段Server启动参数。
- `missionNum=2`、`missionIndex=0/1`。
- 同一个 `xnAddr`、`ckeAddr` 和token。

### 5.2 四区域信号ABI

每个区域包含64个8字节槽，区域大小为512字节：

| region | 地址 | 写方 | 读方 | 语义 |
| --- | --- | --- | --- | --- |
| 0 | `ckeAddr + 0 * 512 + slot * 8` | Host | mission 0 | `HostStart` |
| 1 | `ckeAddr + 1 * 512 + slot * 8` | mission 0 | Host | `HostDone` |
| 2 | `ckeAddr + 2 * 512 + slot * 8` | mission 0 | mission 1 | `MissionStart` |
| 3 | `ckeAddr + 3 * 512 + slot * 8` | mission 1 | mission 0 | `MissionDone` |

共享枚举定义在 `common/kfc_server_message.h`。KFC协议版本从2提升为3，使方案一和方案二的Server cache身份
不可能混用。

### 5.3 正常轮次

```text
MC2 client            mission 0 / Mesh owner           mission 1 / NHR worker
    |                            |                                 |
    | write and flush XN[slot]   |                                 |
    | post HostStart ------------>|                                 |
    |                            | load XN                         |
    |                            | post MissionStart ------------->|
    |                            | run Mesh             load XN    |
    |                            |                      run NHR     |
    |                            |<------------- post MissionDone  |
    |                            | post HostDone                    |
    |<---------------------------|                                 |
    | wait returns               |                                 |
```

mission 0先发送 `MissionStart` 再进入Mesh subkernel，因此NHR不是在Mesh完成后才启动。两个kernel覆盖互不
重叠的数据区间，仍保持算法的并发语义。

### 5.4 终止轮次

`Finalize`执行以下序列：

1. Host等待已提交的普通消息完成。
2. Host在当前槽写入 `INVALID_U64` 并只post `HostStart`。
3. mission 0读取终止消息并post `MissionStart`。
4. mission 1读取相同终止消息，不执行NHR，post `MissionDone` 后退出。
5. mission 0等待 `MissionDone` 后退出，不写 `HostDone`。

Host不等待终止消息的done，这与现有单mission协议一致；内部握手保证两个常驻Server都能退出。

### 5.5 64槽回绕

owner和worker分别维护本地 `paramAddr` 和 `queueIdx`，每轮都在完成握手后前进一个槽。到达64后共同回到
`xnAddr`。只有Host收到 `HostDone` 后才把对应槽标记为可复用，因此worker尚未完成时Host不能覆盖参数。

## 6. Host与CCU职责边界

Host仍负责：

- selector、拓扑门禁和canonical算法名。
- 两个thread、两个Server kernel和两套启动参数的资源申请及启动。
- 11:10切分、128B对齐、stride和25个XN参数生成。
- 每轮写一次XN、post一次 `HostStart`、wait一次 `HostDone`。
- cache identity和协议版本隔离。

CCU新增的职责仅为：

- mission 0写 `MissionStart`、等待 `MissionDone`。
- mission 1等待 `MissionStart`、写 `MissionDone`。
- 两个mission按同一槽号读取参数并同步回绕。
- 传播并确认终止消息。

这是一项有意接受的权衡：相较原则X，Host信号负担减半且Host只观察一个逻辑完成点，但CCU增加两次内部HBM
握手和一个owner等待环。必须由真机数据判断该权衡是否优于双独立Host队列。

## 7. 分阶段实施方案与状态

### 7.1 阶段A：双mission KFC公共框架

1. 多组KFC thread、kernel和Server args序列化：已完成，沿用方案一。
2. 双thread、双kernel资源申请和启动：已完成，沿用方案一。
3. 四个共享信号区域：已完成，区域数量不变，语义改为Host一对、mission一对。
4. mission 0 owner队列：已完成。
5. mission 1 worker队列：已完成。
6. 正常轮次的mission start/done握手：已完成。
7. `INVALID_U64` 终止传播和worker退出确认：已完成。
8. Host单start、单done：已完成。
9. 单mission旧算法继续使用原队列：已完成并回归。
10. cache协议版本隔离：已完成，版本为3。

### 7.2 阶段B：selector和TopoMatchUBX

已完成，沿用方案一：

- CCU Schedule、单层单die `MESH_1D_CLOS`。
- Mesh/Clos数量相等。
- `dataSize > 512 KiB`。
- rank 2至4。
- input/output不overlap。
- 小消息继续使用SoleMesh。
- 不引入Parallel/Pipeline可达分支。
- `TopoMatchUBX`按拓扑类型稳定排序。

### 7.3 阶段C：双template executor

已完成，沿用方案一：

- `REGISTER_EXECUTOR_BY_TWO_TEMPS` canonical注册。
- Mesh/NHR分别计算资源并保留 `ccuKernelNum={1,1}`。
- 按HCCL公式合并thread和notify。
- 保留11:10切分、128B对齐、offset、loop和tail。
- fast-launch上下文中的两个kernel分组映射为两个KFC mission。

### 7.4 阶段D：Mesh/NHR KFC数据面

已完成，沿用方案一：

- Mesh role复用SoleMesh KFC subkernel。
- NHR role同步MultiJetty逻辑，`JETTY_NUM=1`。
- NHR step、channel、rank映射和静态元数据绑定到mission 1。
- Server dispatch同时使用opId和静态role。
- 25个动态参数共享同一个XN槽。

### 7.5 阶段E：注册和构建闭环

已完成，沿用方案一：

- tiling allowlist、Host/device `AlgorithmType`、`algorithmMap`。
- KFC支持列表和原始kernel到Server role映射。
- executor、template、kernel和测试CMake。
- `mc2_client`、Host/device专项构建。

### 7.6 阶段F：stride语义

已完成软件合同，真机待验证：

- `strideCount == 0` 时按 `count * dtypeSize` 推导连续stride。
- 非零stride按 `strideCount * dtypeSize` 写入Mesh输出offset和NHR输出slice stride。
- 不因mission同步改变Mesh/NHR参数布局。

结论：7.1至7.6的软件实施均已完成；7.6的数据语义以及整体正确性、稳定性和性能仍受真机门禁约束。

## 8. 方案二代码改造点

| 文件 | 改造 |
| --- | --- |
| `common/kfc_server_message.h` | 定义四区域共享信号ABI |
| `common/kfc_server_protocol.h` | 协议版本提升到3并校验区域数量 |
| `ccu_kernel_kfc_server.h` | context保存mission start/done地址 |
| `ccu_kernel_kfc_server.cc` | 默认、owner、worker三条队列；mission握手和终止传播 |
| `platform_v310/hccl_ccu_v0.h` | client每轮和Finalize只post owner；Wait只等owner done |
| `platform_v310/hccl_ccu_v0_def.h` | 移除Host信号循环所需的mission数量helper |
| Host ST | 校验协议版本和四区域固定语义 |
| Device ST adapter/test | 分别捕获owner/worker生产IR并校验队列结构 |

方案一中的selector、executor、NHR template/kernel、资源序列化、launch、枚举、tiling、CMake和其余测试代码
不回退，也不重复实现。

## 9. 测试方案

### 9.1 Host和资源测试

- selector阈值、rank和拓扑边界。
- 两个thread、两个Server kernel、`ccuKernelNum={1,1}`。
- 两组六字段启动参数和相同token。
- `missionNum=2`、`missionIndex=0/1`。
- canonical算法与SoleMesh cache隔离。
- 协议版本3和四区域枚举。
- 单mission算法资源和协议无回归。

### 9.2 mission同步设备IR测试

- 默认role仍生成原单mission队列。
- Mesh owner生成Host start轮询、MissionStart存储、MissionDone轮询和HostDone存储。
- NHR worker生成MissionStart轮询和MissionDone存储，不生成Host完成路径。
- owner和worker均读取同一个32参数槽布局。
- 两条路径均包含64槽回绕。
- `INVALID_U64` 分支位于同步协议内，worker回告后两端退出。

### 9.3 真机测试

- rank 2、4。
- 512 KiB阈值和128B切分边界。
- 支持dtype、整块、tail、单repeat和多repeat。
- 默认和非零stride。
- 64槽回绕和长时间稳定性。
- Mesh/NHR拼接后的完整AllGather结果。
- mission 0先完成、mission 1先完成两种时序。
- `Finalize`后两个Server均退出。
- 双kernel实际并行证据。
- 与方案一和HCCL原生算法对比时延、带宽及HBM轮询开销。

## 10. 已执行验证

| 目标或用例 | 当前结果 | 结论边界 |
| --- | --- | --- |
| `mc2_client` | 构建通过 | Server/client协议进入生产构建 |
| `ascendc_ut_adv_api_hccl_ccu_mc2` | 30/30通过 | 公共资源、枚举和MC2回归 |
| `ascendc_ut_adv_api_hccl_ccu_mc2_host` | 37/37通过 | Host资源、selector和信号ABI |
| `ascendc_ut_adv_api_hccl_ccu_mc2_device` | 19/19通过 | 含owner/worker生产IR捕获 |
| `ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC` | 86/86通过 | 目标架构算法回归 |
| CCU真机 | 未执行 | 不声明数据正确性、稳定性和性能通过 |

CCU Host覆盖率已独立清零并只运行25个AllGather相关Host用例。5个相关生产文件的行覆盖率为50.8%
（539/1060），函数覆盖率为58.8%（50/85），报告目录为：

```text
build/coverage/ccu_sched_all_gather_concur_mesh_nhr_multi_link_mission_sync/
```

目录包含 `coverage.info`、`production_coverage.info`、`raw.info`、两个HTML报告、精确到仓内文件和行号的
`line_coverage.tsv`、`test.log` 和 `coverage_summary.txt`。CCU Server和AscendC client不属于Host覆盖率分母，
其结论由device IR测试、生产构建和后续真机验收分别承担。

本次继续忽略范围外的 `ascend610` 构建问题，不修改相关代码，也不把该目标作为方案二门禁。

## 11. 风险和门禁

| 风险 | 门禁或处理方式 |
| --- | --- |
| Host仍post两个start导致槽提前复用 | client固定只访问region 0/1，Host UT和代码审计确认 |
| owner在worker完成前回Host | owner必须先等region 3，再写region 1；IR测试覆盖结构 |
| worker读取错误槽 | 两端每次握手后同步递增并在64回绕；真机长稳验证 |
| 终止只退出owner | owner转发终止并等待worker回告；IR和真机Finalize测试 |
| 单mission算法误入同步路径 | 仅Mesh/NHR静态role选择新队列，default保留旧队列 |
| HBM轮询抵消并发收益 | 与方案一做真机A/B性能对比 |
| mission 1异常导致owner永久等待 | 依赖运行时超时/硬件异常机制；真机故障测试 |
| CCU寄存器或指令资源增加 | device编译和IR规模检查，真机注册门禁 |
| cache复用旧协议Server | 协议版本提升到3并进入cache hash |
| device编译被误认为正确性通过 | 完整数据语义只由真机验收确认 |

## 12. 完成标准

软件迁移完成需满足：

1. canonical名称、selector、双template、Mesh/NHR数据面与最新HCCL基线一致。
2. MC2资源保留两个thread、两个Server kernel和 `{1,1}` 边界。
3. Host只post `HostStart` 并只等待 `HostDone`。
4. mission 0在Mesh前post `MissionStart`，在HostDone前等待 `MissionDone`。
5. mission 1只跟随mission信号，不消费Host信号。
6. 正常消息、64槽回绕和终止消息使用一致槽序。
7. 单mission KFC算法不回归。
8. 生产构建、Host专项和Device专项通过。

整体硬件迁移验收还需满足：

1. 两个Server在真实CCU上注册并常驻成功。
2. Mesh/NHR拼接结果正确。
3. repeat、tail、stride和64槽回绕稳定。
4. `Finalize`无残留worker或死锁。
5. 两个kernel存在实际并行执行证据。
6. 与方案一A/B对比后确认mission同步方案的性能收益或可接受开销。
