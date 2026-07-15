# 575b5e224 A5 AICPU算法同步变更说明

## 1. 变更目标

提交`575b5e224f6b6153fbad6dfe3fdc0a65e6033d36`原始内容来自HCCL仓的算法更新，但同步范围同时包含AllGather和ReduceScatter之外的文档、Scatter、CCU、AIV、DPU优化及公共能力改造。

本次重新审视以父提交`575b5e224^`为基线，只保留以下目标所需的代码：

- 同步A5平台AICPU AllGather算法。
- 同步A5平台AICPU ReduceScatter算法。
- 保留上述算法能够完成选择、注册、资源计算、拓扑匹配、编排和执行所必需的公共依赖。
- 回退不能由上述AICPU调用链证明必要的CCU、AIV、独立DPU、Scatter、文档和公共改造。

参考上游代码目录：

```text
/mnt/workspace/gitCode/chenma/hccl/hccl/src/ops
```

## 2. 是否属于AICPU A5算法相关

结论：**属于AICPU A5算法相关变更。**

判断依据如下：

1. 算法入口位于`AllGatherAutoSelector::SelectAicpuAlgo`和`ReduceScatterAutoSelector::SelectAicpuAlgo`，返回的均为`Ins*`或`ReduceScatterOrderPreserved*` AICPU执行器名称。
2. 新增和更新的主体位于`all_gather/template/aicpu`、`reduce_scatter/template/aicpu`以及对应AICPU执行器中。
3. 新增的`TopoMatch3Level`和`TopoMatchSqueeze2D`仅接受`DEV_TYPE_950`，旧枚举兼容分支使用`DEV_TYPE_910_95`。这两个枚举对应本代码基线中的A5产品形态。
4. OmniPipe、三级跨Pod、UBOE、PCIe混合拓扑和严格保序路径均由AICPU选择器触发。

需要注意：保留代码并非每一行都只被A5使用。序列化结构、通道计算、拓扑匹配和注册宏属于共享基础设施，但它们是A5 AICPU AllGather/ReduceScatter运行所需的直接依赖。

## 3. 最终变更范围

相对父提交的代码差异统计，不含本说明文档：

```text
96 files changed, 12329 insertions(+), 2858 deletions(-)
39 added files, 57 modified files
```

原提交为132个文件、19463行新增和3962行删除。本次清理删除了36个无关文件的变更，并继续回退了多个共享文件中的CCU/DPU专用逻辑。

最终代码差异中不存在`aiv`或`ccu`目录下的文件变化。保留的DPU命名模板仅用于AICPU OmniPipe执行器的第三维模板，原因见第7节。

## 4. AllGather AICPU算法更新

### 4.1 选择策略

`all_gather_auto_selector.cc`保留以下AICPU选择变化：

- 三级拓扑且Level2为UBOE时：
  - 单模块8卡选择`InsV2AllGatherOmniPipeUboe`。
  - 中间层单实例退化为`InsAllGatherNHR`。
  - 其他情况选择`InsAllGatherParallelMesh1DNHRUboe`。
- MESH_1D多层拓扑：
  - 三级拓扑选择`InsAllGatherSequenceNHRNHRMesh1D`。
  - 两级拓扑大数据在`InsAllGatherSequenceNHRMesh1D`和`InsAllGatherParallelMesh1DNHR`之间选择。
  - 小数据回退`InsAllGatherNHR`。
- 单层MESH_1D且存在两级网络、大数据量时选择`InsAllGatherMesh1D1DZAxisDetour`。
- PCIe混合拓扑：
  - 小数据选择`InsAllGatherParallelMesh1DNHRPcie`。
  - 大数据选择`InsV2AllGatherOmniPipePcie`。
- UBX非全连接拓扑：
  - 小数据选择`InsAllGatherParallelMesh1DNHRMultiJetty`。
  - 大数据选择`InsV2AllGatherOmniPipe`。
- CLOS单层和不适合并行拆分的场景继续使用`InsAllGatherNHR`。

关键阈值包括：

- AICPU小数据阈值：1 MiB。
- PCIe OmniPipe切换阈值：4 MiB。
- 两级网络Z轴绕行阈值：总数据量1 GiB。
- Sequence切换阈值：总数据量4 GiB。

### 4.2 执行器

新增或保留更新的执行器包括：

- `InsV2AllGatherOmniPipeExecutor`：支持UBX、PCIe、UBOE及多层OmniPipe编排。
- `InsV2AllGatherSequenceExecutor3Level`：支持NHR-NHR-Mesh1D三级顺序编排。
- `InsV2AllGatherSequenceExecutorAicpu`：同步两级大数据Sequence实现。
- `InsV2AllGatherParallelExecutor`：支持MultiJetty、PCIe和UBOE组合。
- `InsV2AllGatherConcurrentExecutor`：保留AICPU MultiJetty通道、资源校验和数据切分修正。
- `InsV2AllGatherSoleExecutor`：保留Z轴绕行、对称内存参数传递及AICPU单模板更新。

### 4.3 模板

新增OmniPipe模板：

- `InsTempAllGatherOmniPipeMesh1D`
- `InsTempAllGatherOmniPipeNHR`
- `InsTempAllGatherOmniPipeNHRDPU`

更新既有AICPU模板：

- Mesh1D模板增加对称内存、通道数和新参数支持。
- NHR模板增加MultiJetty通道数计算、隔离判断和分片能力。
- Z轴绕行模板同步大数据路径所需参数。
- NHR DPU基类只保留OmniPipe派生所需的接口开放和统一模板接口适配；独立DPU批量传输优化已回退。

## 5. ReduceScatter AICPU算法更新

### 5.1 严格保序

严格确定性模式下，ReduceScatter必须避免进入不保证规约顺序的CCU路径：

- `CCU_MS`和`CCU_SCHED`选择函数检测严格保序条件后返回`NOT_MATCH`，使框架继续尝试AICPU。
- 参与卡数不超过32时选择`ReduceScatterOrderPreserved`。
- 参与卡数超过32时选择`ReduceScatterOrderPreservedGroup`。

保序路径新增独立执行器及Level1、Group模板，并通过`GetExternalInputHcclDeterministic()`读取确定性配置。

### 5.2 三级、OmniPipe和大数据策略

`reduce_scatter_auto_selector.cc`保留以下AICPU选择变化：

- 三级UBOE拓扑：
  - 单模块8卡选择`InsV2ReduceScatterOmniPipeUboe`。
  - 中间层单实例退化为`InsReduceScatterNHR`。
  - 其他情况选择`InsReduceScatterParallelNHRNHRUboe`。
- 三级MESH_1D拓扑选择`InsReduceScatterSequenceMesh1DNHRNHR`。
- 两级大数据在`InsReduceScatterSequenceMesh1DNhr`和`InsReduceScatterParallelMesh1DNHR`之间选择。
- 单层或两级MESH_1D的大数据路径统一选择`InsReduceScatterMesh1DMeshChunk`，不再为该条件单独选择Z轴绕行。
- PCIe混合拓扑：
  - 小数据选择`InsReduceScatterParallelMesh1DNHRPcie`。
  - 大数据选择`InsV2ReduceScatterOmniPipePcie`。
- 64位数据类型和`HCCL_REDUCE_PROD`继续优先使用兼容规约路径。

关键阈值包括：

- 单层MeshChunk数据阈值：4 MiB至16 MiB区间结合拓扑比例判断。
- PCIe OmniPipe切换阈值：4 MiB。
- 两级网络总数据阈值：1536 MiB。
- Sequence切换阈值：总数据量4 GiB。

### 5.3 执行器和模板

新增或保留更新的执行器包括：

- `InsV2ReduceScatterOmniPipeExecutor`
- `InsV2ReduceScatterSequenceExecutor3Level`
- `InsV2ReduceScatterOrderPreservedExecutor`
- 更新后的Sequence、Parallel、Concurrent和Sole AICPU执行器

新增模板包括：

- `InsTempReduceScatterOmniPipeMesh1D`
- `InsTempReduceScatterOmniPipeNHR`
- `InsTempReduceScatterOmniPipeMesh1dDpu`
- `InsTempReduceScatterOrderPreservedLevel1`
- `InsTempReduceScatterOrderPreservedGroup`

既有Mesh1D、MeshChunk、NHR、AICPU Reduce NHR和Z轴绕行模板同步了通道数、切片、对齐、规约和序列化参数。

## 6. 必要公共依赖

以下公共修改仍然保留，因为可从AICPU选择或执行链路直接触达：

- `omnipipe_data_slice_calc`：计算AllGather/ReduceScatter OmniPipe多维分片、步进、偏移和LocalCopy信息。
- `TopoMatch3Level`：生成三级通信域。
- `TopoMatchSqueeze2D`：将三级UBOE拓扑压缩为两维组合通信域。
- `CommonAlgTemplateBase`：允许OmniPipe执行器以统一接口管理不同轴模板。
- `omnipipe_template_utils`：填充OmniPipe模板参数并按多通道拆分数据。
- `TemplateDataParams`和`StepSliceInfo`序列化扩展：用于Host、AICPU和DPU侧交换OmniPipe分片信息。
- `CalcChannelRequestNhrMultiJetty`和通道数计算：支持AICPU MultiJetty路径。
- `REGISTER_EXEC_V2_MULTI`：注册三个模板参数的OmniPipe/三级执行器。
- `level2Uboe`拓扑字段：供AICPU选择器判断三级UBOE路径。
- `AICPU_ALIGN_SIZE`：供OmniPipe和大数据切片对齐。
- `CANN_VERSION(...)`：控制AICPU新增注册在对应CANN版本生效。
- 确定性配置读取和保序公共参数：供ReduceScatter严格保序路径使用。
- `RestoreChannelMap`虚接口：由AllGather/ReduceScatter OmniPipe执行器覆盖。

## 7. DPU命名文件为何保留

保留以下DPU命名文件不代表同步独立DPU算法：

- AllGather的`ins_temp_all_gather_omnipipe_nhr_dpu.*`
- ReduceScatter的`ins_temp_reduce_scatter_omnipipe_mesh_1d_dpu.*`
- 两个既有DPU模板的最小接口适配

OmniPipe执行器是AICPU算法编排器，其第三维模板类型在部分拓扑下由DPU执行数据传输。执行器注册直接实例化这些类型，因此删除它们会导致AICPU OmniPipe注册或编译失败。

已经回退的独立DPU变化包括：

- AllGather和ReduceScatter纯DPU Sequence执行器更新。
- `InsAllGatherNHRDPU`新增Sole注册。
- `dpu_alg_nhr_opt_wrapper`及`DpuBatchTransfer`批量传输优化。
- 既有DPU模板中不被OmniPipe接口要求的传输和规约逻辑改写。

## 8. 已回退的无关修改

### 8.1 CCU

- AllGather和ReduceScatter OmniPipe 2D CCU执行器。
- CCU双模板FastLaunch公共下沉和各执行器接入。
- CCU并发执行器专用带宽权重调整。
- CCU注册名称、版本门控和选择策略改写。
- 纯CCU资源缓存字段和公共辅助函数。

ReduceScatter严格保序时CCU返回`NOT_MATCH`仍保留，因为该变化的作用是把请求路由到AICPU保序算法，而不是新增CCU算法。

### 8.2 AIV、Scatter和拓扑

- AIV选择器宏和AIV专用变化。
- `omnipipe_scatter_data_slice_calc`。
- `TopoMatchUBX1d`及对应CMake和头文件变化。

### 8.3 公共能力和文档

- `ExecTimeoutManager`和`aicpu_timeout`。
- ACL memset动态适配封装。
- 通信域状态回调声明。
- Task异常占位接口。
- 与本目标无关的公共字段和缓存。
- 原提交新增的三份算法流程/汇总文档。
- AllGather和ReduceScatter公共算子入口、头文件及非算法CMake改写。

## 9. 选择器与注册一致性

已核对AICPU选择器返回的新增算法名均有对应注册，包括：

- AllGather：OmniPipe UBX/PCIe/UBOE、Parallel MultiJetty/PCIe/UBOE、两级Sequence、三级Sequence和Z轴绕行。
- ReduceScatter：OmniPipe PCIe/UBOE、Parallel PCIe/UBOE、两级Sequence、三级Sequence、MeshChunk以及两种保序执行器。

同时恢复了父提交中的CCU注册名称，避免CCU选择器仍返回旧名称但注册项被同步快照改名的问题。

## 10. 验证结果

完成以下验证：

```text
git diff --check
结果：通过

cmake --build build_cann910 --target mc2_client -j4
结果：[100%] Built target mc2_client
```

构建过程中发现并修复两项必要依赖：

- 删除`auto_selector_base.h`中与`topo_match_base.h`重复且未使用的`RANK_SIZE_EIGHT`，避免新增三级拓扑文件同时包含两者时重复定义。
- 保留`InsCollAlgBase::RestoreChannelMap`的virtual属性，因为两个OmniPipe执行器均使用`override`覆盖该接口。

本次未创建或修改Git提交，仅调整当前工作区内容。
