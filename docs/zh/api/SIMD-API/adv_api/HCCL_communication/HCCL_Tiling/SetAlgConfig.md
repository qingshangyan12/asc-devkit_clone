# SetAlgConfig

## 功能说明

设置通信算法。

## 函数原型

```
uint32_t SetAlgConfig(const std::string& algConfig)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| algConfig | 输入 | 通信算法配置。string类型，支持的最大长度为128字节。不指定（置空或传入"default"）时通信库根据组网拓扑自动选择算法，推荐优先使用该方式。<!-- npu="950" id1 --><br><br>针对Ascend 950PR&950DT系列产品，当前支持的取值如下，具体含义请参考下方说明：<br>AllGather通信任务（AI CPU服务端）："sole[mesh]"、"sole[nhr]"、"concur[mesh,nhr]"、"parallel[mesh,nhr]"、"pipeline[mesh,nhr]"。<br>AllGather通信任务（CCU服务端）："sole[mesh]"、"concur[mesh,nhr.multi_channel]"、"parallel[mesh,nhr.multi_channel]"。<br>ReduceScatter通信任务（AI CPU服务端）："sole[mesh]"、"sole[nhr]"、"parallel[mesh,nhr]"、"concur[mesh,nhr]"、"pipeline[mesh,nhr]"。<br>ReduceScatter通信任务（CCU服务端）："sole[mesh]"、"sole[nhr.multi_channel]"、"concur[mesh,nhr.multi_channel]"。<br>AllReduce通信任务（CCU服务端）："sole[mesh]"。<br>AlltoAll通信任务（AI CPU服务端）："sole[mesh]"、"concur[mesh,mesh]"。<br>AlltoAll通信任务（CCU服务端）："sole[mesh]"、"sole[mesh.multi_channel]"、"concur[mesh,mesh]"。<br>AlltoAllV通信任务（AI CPU服务端）："sole[mesh]"、"concur[mesh,mesh]"。<br>AlltoAllV通信任务（CCU服务端）："sole[mesh]"。<!-- end id1 --><!-- npu="A3" id2 --><br><br>针对Atlas A3系列产品，当前支持的取值为：<br>"AllReduce=level0:doublering"：AllReduce通信任务。<br>"AllGather=level0:doublering"：AllGather通信任务。<br>"ReduceScatter=level0:doublering"：ReduceScatter通信任务。<br>"AlltoAll=level0:fullmesh;level1:pairwise"：AlltoAllV和AlltoAll通信任务。<br>"BatchWrite=level0:fullmesh"：BatchWrite通信任务。<!-- end id2 --><!-- npu="910b" id3 --><br><br>针对Atlas A2系列产品，该参数为预留字段，配置后不生效，默认仅支持FullMesh算法。FullMesh算法即NPU之间的全连接，任意两个NPU之间可以直接进行数据收发。<!-- end id3 --> |

<!-- npu="950" id4 -->
针对Ascend 950PR&950DT系列产品，算法配置字段详细说明如下：

1. 语法

    ```text
    编排方式[<域内遍历方式>(.<附加属性>)*,<域内遍历方式>(.<附加属性>)*,...]
    ```

    示例：

    ```text
    sole[mesh]
    concur[mesh,mesh]
    parallel[mesh,nhr.multi_channel]
    ```

    | 字段 | 含义 | 是否必填 |
    |---|---|---|
    | `编排方式` | 各域之间的流水关系 | 必填 |
    | `<域内遍历方式>` | `[]`中的多个取值按通信层级从低到高排列 | 至少一个 |
    | `.<附加属性>` | 该层的附加功能属性 | 可选 |

2. 编排方式

    | 取值 | 含义 | 示例 |
    |---|---|---|
    | `sole` | 在单个域内完成通信 | `sole[mesh]` |
    | `sequence` | 多域按序串行执行，前一域的输出作为后一域的输入 | `sequence[mesh,nhr]` |
    | `parallel` | 多域并行执行，各域互不依赖 | `parallel[mesh,nhr]` |
    | `concur` | 同一份数据切分为两份，两域并发处理，每域处理一份数据 | `concur[mesh,nhr]`、`concur[mesh,mesh]` |
    | `pipeline` | 多批次流水编排，数据划分为多个批次，第1批数据在域A传输的同时，第2批数据已在域B传输 | `pipeline[mesh,nhr]` |

3. 域内遍历方式与附加属性

    域内遍历方式：

    | 域内遍历方式 | 含义 |
    |---|---|
    | `mesh` | 1D Fullmesh |
    | `nhr` | 非均衡层次环 |

    外部可配置的附加属性：

    | 取值 | 功能类别 | 含义 |
    |---|---|---|
    | `mesh.multi_channel` | 通道 | Mesh域内每对rank之间建立多条物理通道并发传输数据 |
    | `nhr.multi_channel` | 通道 | NHR域内每对rank之间建立多条物理通道并发传输数据 |

4. 配置指导

    **（1）不指定算法（推荐）**

    将`algConfig`置空或传入`"default"`，通信库根据实际组网拓扑自动选择算法。用户无需了解机型拓扑细节，该方式在多数场景下即可获得较优性能，建议作为首选。

    **（2）按物理组网确定域内遍历方式**

    需要显式指定算法时，用户唯一能明确的是物理组网方式（即通信域内任意两卡之间是否存在直连通路）。域内遍历方式代表通信算法在当前通信域内如何使用这些链路，物理组网方式决定了域内遍历方式的可用选项：

    - Mesh组网（任意两卡直连）：使用`mesh`遍历方式。
    - CLOS组网（经交换芯片互连）：通常使用`nhr`遍历方式。
    - 混合组网（如机内Mesh直连+机间经交换芯片互连）：`[]`中按level0->level1从低到高逐层指定，最多支持两个通信域，例如机内Mesh+机间CLOS的双层组网使用`parallel[mesh,nhr]`。

    **（3）AlltoAll/AlltoAllV任务的CLOS组网例外**

    AlltoAll/AlltoAllV任务的数据块按目的rank独立切分，不存在跨rank接力传播，因此即使CLOS组网也使用直连方式（`mesh`）传输：指定`"sole[mesh]"`时，通信库会结合实际机型拓扑，在Mesh组网下走直连建链、在CLOS组网（含交换芯片互连的双die机型）下自动选择适配的直连算法实现。用户无需感知该差异，CLOS组网下同样配置`"sole[mesh]"`即可。

    **（4）编排方式与附加属性的选择**

    在确定物理组网（即确定各域的遍历方式）之后，编排方式（`sole`、`concur`等）与附加属性（`multi_channel`等）的可选项随之确定。建议用户结合自身实测性能benchmark曲线和业务数据量，在不同可选项中确定最终配置。后续版本将推出自动选择工具辅助用户决策。

    **（5）配置错误处理**

    指定了当前版本不支持的算法配置（语法错误、未注册的算法名等）时，接口返回非0，且日志中输出`algConfig is invalid or not registered`错误信息，通信任务不会静默回退到其他算法。若出现该报错，请对照本节取值说明修正配置，或改用不指定算法的自动选择方式。
<!-- end id4 -->

## 返回值说明

-   0表示设置成功。
-   非0表示设置失败。

## 约束说明

无

## 调用示例

本接口的调用示例请见[调用示例](SetOpType.md#调用示例)。
