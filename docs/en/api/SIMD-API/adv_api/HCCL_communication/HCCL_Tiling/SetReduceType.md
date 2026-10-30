# SetReduceType

<!-- md-trans-meta sourceCommit=324fbb1aae3d804989827fac48c891278861740d translatedAt=2026-10-08T12:05:56.718Z pushedAt=2026-10-27T11:48:33.306Z nl=2 -->

## Description

Sets the Reduce operation type, which takes effect only on communication tasks with reduction operations.

## Prototype

```
uint32_t SetReduceType(uint32_t reduceType, uint8_t dstDataType = 0, uint8_t srcDataType = 0)
```

## Parameters

**Table 1**  Parameters

| Parameter | Input/Output | Description |
| --- | --- | --- |
| reduceType | Input | Reduction operation type, which takes effect only on communication tasks with reduction operations. It is of the uint32_t type. For details about the values, see the parameter description in [Table 2](../HCCL_Kernel/HCCL_usage.md#table2469980529). |
| dstDataType | Input | Data type of the output data in the communication task. It is of the uint8_t type. For the value range of this parameter, see [Table 1](../HCCL_Kernel/HCCL_usage.md#table116710585514).<!-- npu="950" id1 --><br><br>For Ascend 950PR&950DT products, when the CCU acts as the server, different communication tasks support different output data types, as follows:<br>For the AllReduce, AllGather, AllToAll, AllToAllV, and AllToAllVWrite communication tasks: the output data type must be the same as the input data type. For the input data types supported by each communication task, see srcDataType.<br>For the ReduceScatter communication task, when the input data type is int16_t, int32_t, half, float, or bfloat16_t, the output data type must be the same as the input data type; when the input data type is int8_t, hifloat8_t, fp8_e5m2_t, or fp8_e4m3fn_t, the output data type must be one of half, bfloat16_t, and float.<!-- end id1 --> |
| srcDataType | Input | Data type of the input data in the communication task. It is of the uint8_t type. For the value range of this parameter, see [Table 1](../HCCL_Kernel/HCCL_usage.md#table116710585514).<!-- npu="950" id2 --><br><br>For Ascend 950PR&950DT products, when the CCU acts as the server, the input data types supported by different communication tasks are as follows:<br>AllReduce communication task: the supported input types are int8_t (the sum operation type is not supported), int16_t, half, bfloat16_t, int32_t, and float.<br>AllGather, AllToAll, AllToAllV, and AllToAllVWrite communication tasks: the supported input types are int8_t, uint8_t, hifloat8_t, fp8_e5m2_t, fp8_e4m3fn_t, int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float, int64_t, and double.<br>ReduceScatter communication task: the supported input types are int8_t (the sum operation type is not supported), hifloat8_t, fp8_e5m2_t, fp8_e4m3fn_t, int16_t, half, bfloat16_t, int32_t, and float.<!-- end id2 --> |

## Return Value

-   0 indicates success.
-   Non-zero indicates failure.

## Constraints
<!-- npu="950" id3 -->
For Ascend 950PR&950DT products, when the AI CPU acts as the server, this API is not supported.
<!-- end id3 -->
<!-- npu="A3" id4 -->
For Atlas A3 products, this API is not supported.
<!-- end id4 -->
<!-- npu="910b" id5 -->
For Atlas A2 products, this API is not supported.
<!-- end id5 -->

## Examples

For the example of calling this API, see [Examples](SetOpType.md#examples).
