# Collective DSL

这层把 collective 算法与执行后端分开，但不会另造一套脱离 asc-devkit 的运行时：

```text
TopoMatch 产生的 rank 分组
        ↓
Mesh / NHR + Sole / Concurrent / Parallel / Pipeline
        ↓
dsl::Plan（LocalCopy / Exchange / Wait / Complete）
        ├─ CalcResources：提前计算 channel / queue / event 资源
        ├─ AICPU backend：TemplateResource + HComm wrapper
        └─ trace backend：与既有算法的提交轨迹比较
```

代码位于 `mc2_ops_hccl::dsl`，asc-devkit 现有代码中可直接写 `dsl::Plan`。

## 文件

- `core/`：最小 IR 和资源需求分析；这里不实现任何 collective 原语。
- `primitives/`：可复用的 Mesh、NHR 等通信原语。
- `algorithms/`：Sole、Concurrent、Parallel、Pipeline 等组合。
- `backends/`：AICPU lowering 和 trace，不定义算法。

这些源文件通过 `op_common/collective_dsl/CMakeLists.txt` 编进现有的
`libmc2_client.so`，不是单独的示例库。

## 资源申请与执行

Executor 的 `CalcRes` 阶段先构造 Plan，再把 Plan 映射到 asc-devkit 已有的资源请求：

```cpp
dsl::AllGatherArgs args{rank, rankSize, count * dtypeSize};
dsl::ChannelLayout channels = /* 由拓扑与 HcclChannelDesc 构造 */;
dsl::Plan plan = dsl::SoleNhr(args, channels, dimension);
CHK_RET(dsl::CalcAicpuResources(plan, channelDescs, resourceRequest));
```

框架按 `AlgResourceRequest` 分配资源并生成 `TemplateResource` 后，执行阶段复用同一个 Plan：

```cpp
dsl::AicpuMemory memory{/* input/output 和各 rank 的远端地址 */};
CHK_RET(dsl::RunAicpu(plan, memory, templateResource));
```

`CalcAicpuResources` 会验证 Plan 所需的每个 `(peer, path)` 都能被传入的
`HcclChannelDesc` 覆盖，并把 channel 需求写进 `AlgResourceRequest::channels`。
当前 AICPU lowering 有意使用一个 HComm thread 顺序提交，所以只申请主线程资源；在该约束下，
Plan 中的 queue/event 依赖由同一线程的提交顺序满足。后续如果改成多线程并发 lowering，必须同时把
queue 映射为 thread，并把跨 queue event 映射为 thread notify，不能只改执行代码而漏掉 `CalcRes`。

## 初始算法

- `SoleMesh`
- `SoleNhr`
- `ConcurrentMeshNhr`
- `ParallelMeshNhr`
- `PipelineMeshNhr`

同一 peer 的多条 `Channel{peer, path, weight}` 表达 MultiLink；Mesh/NHR 按 weight 切片，
因此算法组合本身不复制 MultiLink 专用实现。

## Trace 对比

```cpp
std::ofstream out("dsl.trace");
dsl::WriteTrace(plan, out);
```

trace 包含 rank、queue、send/recv peer、path、数据切片和完成 event。既有 HCCL/asc-devkit 算法可在
`LocalCopy`、`SendRecvBatchWrite` 等实际提交点记录相同字段，再通过 `CompareTrace` 比较。
这一级验证的是算法展开与依赖；CCU 的 `DumpInstruction()` 是更低层的微码，需等 CCU backend lowering
完成后再做第二层比较。

## 在 asc-devkit 中编译验证

```bash
cmake --build build --target mc2_client -j4
cmake --build build --target ascendc_ut_hccl_collective_dsl -j4
ctest --test-dir build -R ascendc_ut_hccl_collective_dsl --output-on-failure
```

测试覆盖 4/5 rank Mesh/NHR、8 rank 二维 Parallel/Pipeline、双 Channel、资源计算和 trace。
