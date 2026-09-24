# Mc2SetCcAlgConfig

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas训练系列产品：不支持
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_MC2/Mc2SetCcAlgConfig_res.md#id1 -->

## 功能说明

设置MC2通信任务的通信算法。若不调用本接口或传入空字符串，接口内部会通过算法选择器自动选择算法。

## 函数原型

```cpp
extern Mc2Result __attribute__((visibility("default"))) Mc2SetCcAlgConfig(void* ccArgs, const char* algConfig)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| ccArgs | 输入 | [Mc2GetCcArgs](Mc2GetCcArgs.md)返回的MC2通信参数对象指针。 |
| algConfig | 输入 | 通信算法名，以'\0'结尾的字符串，最大有效长度为127字节。 |

CCU路径可显式配置的算法名如下。算法实际可用性还与网络拓扑、资源状态和通信任务参数有关。

**表2**  CCU路径支持的算法名

| 通信任务 | 算法名 |
| --- | --- |
| AllGather | CcuAllGatherMesh1DMem2Mem |
| AllGather | CcuAllGatherMeshMem2Mem1D |
| AllGather | CcuSchedAllGatherConcurMeshNHRMultiLink |
| AllGather | CcuSchedAllGatherParallelMeshNHRMultiLink |
| AllGather | CcuSchedAllGatherMesh1DMem2Mem |
| AllGather | CcuSchedAllGatherSoleMesh |
| AllReduce | CcuSchedAllReduceSoleMesh |
| ReduceScatter | CcuSchedReduceScatterConcurMeshNHRMultiLink |
| ReduceScatter | CcuSchedReduceScatterSoleMesh |
| ReduceScatter | CcuSchedReduceScatterSoleMeshPeerOnly |
| ReduceScatter | CcuSchedReduceScatterSoleNHRMultiLink |
| AlltoAll | CcuSchedAllToAllMesh1DMultiJetty |
| AlltoAll | CcuSchedAllToAllSoleMesh |
| AlltoAll | CcuSchedAllToAllSoleMeshConcurrent |
| AlltoAllV | CcuSchedAllToAllVSoleMesh |

AI CPU路径暂不提供可显式配置的算法名称列表。建议不调用本接口或传入空字符串，由内部算法选择器根据通信任务、拓扑和资源状态选择算法。

## 返回值说明

-   0表示设置成功。
-   非0表示设置失败，例如参数为空指针或算法名长度非法。

## 约束说明

-   算法名长度必须小于128字节。
-   使用本接口设置的通信算法必须与指定的通信任务类型匹配。

## 调用示例

```cpp
char algConfig[] = "CcuSchedAllToAllSoleMesh";
Mc2Result ret = Mc2SetCcAlgConfig(ccArgs, algConfig);
```
