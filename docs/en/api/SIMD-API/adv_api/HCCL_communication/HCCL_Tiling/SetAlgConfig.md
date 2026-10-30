# SetAlgConfig

<!-- md-trans-meta sourceCommit=4231244ca1bc9ad7ef5518150039e7d5ed136210 translatedAt=2026-10-08T12:02:44.217Z pushedAt=2026-10-27T11:47:31.587Z nl=2 -->

## Description

Sets the communication algorithm.

## Prototype

```
uint32_t SetAlgConfig(const std::string& algConfig)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| algConfig | Input | Communication algorithm configuration. String type, with a maximum supported length of 128 bytes. <!-- npu="950" id1 --><br><br>For Ascend 950PR&950DT products, the currently supported values are as follows. For details, see the description below:<br>AllGather communication task (AI CPU server): "sole[mesh]", "sole[nhr]", "concur[mesh,nhr]", "parallel[mesh,nhr]", "pipeline[mesh,nhr]".<br>AllGather communication task (CCU server): "sole[mesh]", "concur[mesh,nhr.multi_channel]", "parallel[mesh,nhr.multi_channel]".<br>ReduceScatter communication task (AI CPU server): "sole[mesh]", "sole[nhr]", "parallel[mesh,nhr]", "concur[mesh,nhr]", "pipeline[mesh,nhr]".<br>ReduceScatter communication task (CCU server): "sole[mesh]", "sole[nhr.multi_channel]", "concur[mesh,nhr.multi_channel]".<br>AllReduce communication task (CCU server): "sole[mesh]".<br>AlltoAll communication task (AI CPU server): "sole[mesh]", "concur[mesh,mesh]".<br>AlltoAll communication task (CCU server): "sole[mesh]", "sole[mesh.multi_channel]", "concur[mesh,mesh]".<br>AlltoAllV communication task (CCU server): "sole[mesh]".<!-- end id1 --><!-- npu="A3" id2 --><br><br>For Atlas A3 products, the currently supported values are:<br>"AllReduce=level0:doublering": AllReduce communication task.<br>"AllGather=level0:doublering": AllGather communication task.<br>"ReduceScatter=level0:doublering": ReduceScatter communication task.<br>"AlltoAll=level0:fullmesh;level1:pairwise": AlltoAllV and AlltoAll communication tasks.<br>"BatchWrite=level0:fullmesh": BatchWrite communication task.<!-- end id2 --><!-- npu="910b" id3 --><br><br>For Atlas A2 products, this parameter is a reserved field and does not take effect after configuration. By default, only the FullMesh algorithm is supported. The FullMesh algorithm is a full connection between NPUs, where any two NPUs can directly send and receive data.<!-- end id3 --> |

<!-- npu="950" id4 -->
For Ascend 950PR&950DT products, the algorithm configuration fields are described in detail as follows:

1. Syntax

    ```text
    orchestration mode[<in-domain traversal mode>(.<additional attribute>)*,<in-domain traversal mode>(.<additional attribute>)*,...]
    ```

    Example:

    ```text
    sole[mesh]
    concur[mesh,mesh]
    parallel[mesh,nhr.multi_channel]
    ```

    | Field | Description | Required |
    |---|---|---|
    | `orchestration mode` | Pipeline relationship between domains | Required |
    | `<in-domain traversal mode>` | Multiple values in `[]` are arranged from low to high by communication hierarchy | At least one |
    | `.<additional attribute>` | Additional functional attribute of this layer | Optional |

2. Orchestration Mode

    | Value | Description | Example |
    |---|---|---|
    | `sole` | Completes communication within a single domain | `sole[mesh]` |
    | `sequence` | Multiple domains execute serially in order, with the output of the previous domain serving as the input of the next domain | `sequence[mesh,nhr]` |
    | `parallel` | Multiple domains execute in parallel, with no dependency between domains | `parallel[mesh,nhr]` |
    | `concur` | The same data is split into two parts, and the two domains process them concurrently, with each domain processing one part | `concur[mesh,nhr]`, `concur[mesh,mesh]` |
    | `pipeline` | Multi-batch pipeline orchestration, where data is divided into multiple batches; while the first batch is being transmitted in domain A, the second batch is already being transmitted in domain B | `pipeline[mesh,nhr]` |

3. In-Domain Traversal Mode and Additional Attribute

    In-Domain Traversal Mode:

    | In-Domain Traversal Mode | Description |
    |---|---|
    | `mesh` | 1D Fullmesh |
    | `nhr` | Unbalanced hierarchical ring |

    Externally configurable additional attributes:

    | Value | Function Category | Description |
    |---|---|---|
    | `mesh.multi_channel` | Channel | Establishes multiple physical channels between each pair of ranks in the Mesh domain for concurrent data transmission. |
    | `nhr.multi_channel` | Channel | Establishes multiple physical channels between each pair of ranks in the NHR domain for concurrent data transmission. |
<!-- end id4 -->

## Return Value

-   0 indicates that the setting is successful.
-   A non-zero value indicates that the setting fails.

## Constraints

None

## Examples

For the example of calling this API, see [Examples](SetOpType.md#examples).
