# Collective DSL

目标是把原来混在一起的三个维度拆开：

```text
算法：Sole / Parallel / Concurrent / Pipeline + Mesh / NHR
发起与调度：Host 直提，或 AIC/AIV → KFC server
执行：AICPU HComm、CCU mission、AIC/AIV 直驱或通信 kernel
```

## 目录边界

- `model/`：Plan、Stage、逻辑资源和 `Initiator/Dispatcher/Executor` 路由。
- `primitives/`：Mesh、NHR。这里只保留 rank group、owner 集合、数据区间和依赖。
- `algorithms/`：Sole、Concurrent、Parallel、Pipeline 的组合。
- `dispatch/`：KFC 请求路径。KFC 是 dispatcher，不是算法或执行引擎。
- `engines/trace/`：把 Plan lowering 成验证 trace；不申请真实通信资源。
- `adapters/ascend/`：唯一允许出现 `OpParam`、`AlgResourceRequest`、`TemplateResource`、
  `HcclChannelDesc` 和 CCU submit info 等旧框架类型的位置。

算法参数中没有 channel。MultiLink、port group、协议、thread、notify、AIV comm info、CCU kernel/mission
都由具体执行路线结合真实拓扑绑定。

## MC2 路线

`ExecutionRoute` 把“谁发起”和“谁执行”拆开，显式覆盖：

```text
AIC/AIV → HcclMsgArea/KFC queue → AICPU server → HComm primitive
AIC/AIV → HcclMsgArea/KFC queue → CCU server   → CCU mission
Host                                      → AICPU HComm
AIC/AIV                                   → 直驱或 AIV 通信 kernel
```

`dispatch/kfc_dispatch.*` 只规划 client request slot、server queue 和是否需要 CCU mission。
真正的 `HcclMsgArea` 发布/消费仍由现有 Ascend C HCCL API 和 KFC server 完成，不能在算法 DSL 中伪造。

## 已接入的真实链路

- `InsAllGatherMesh1D` → `DslAicpuAllGatherSoleMesh` → 真实 `InsTempAllGatherMesh1D`
- `CcuSchedAllGatherSoleMesh` → `DslCcuAllGatherSoleMesh` → 真实
  `CcuTempKfcAllGatherMesh1DMem2Mem`

两条链路的 `CalcRes` 与 `KernelRun` 都经过 DSL：

```text
CalcRes:   Plan → binding → channel request / thread+notify / CcuKernelInfo
KernelRun: Plan → binding → HComm operations 或 CcuKernelSubmitInfo/KFC CCU mission
```

adapter 当前复用旧 primitive 的成熟 lowering，先稳定边界，算法组合不复制 engine 细节。
AIV Mesh 的真实实现位于 HCCL/HComm，而 asc-devkit 当前只有 selector 名称，没有可链接 template，
所以这里明确返回不支持，不以假 channel 实现冒充 AIV backend。

## 初始 Plan

- `SoleMesh`
- `SoleNhr`
- `ConcurrentMeshNhr`
- `ParallelMeshNhr`
- `PipelineMeshNhr`

Stage 只包含：原语、rank group、进入该阶段前各 rank 持有的 owner、字节区间、queue 和依赖。
NHR 轮次、Mesh peer/thread 展开属于 engine binding。

## 验证

```bash
cmake --build build --target mc2_client ascendc_ut_hccl_collective_dsl -j4
```

测试检查五种 Plan、逻辑资源、KFC AICPU/CCU 路由、AIV direct 能力边界和 trace engine。
`mc2_client` 构建验证 adapter 能与现有 AICPU/CCU 实现真正链接。
