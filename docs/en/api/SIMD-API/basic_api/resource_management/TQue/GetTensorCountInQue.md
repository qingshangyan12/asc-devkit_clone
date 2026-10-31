# GetTensorCountInQue<a name="ZH-CN_TOPIC_0000001500027581"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-09-23T17:55:04.772Z pushedAt=2026-10-26T07:41:44.703Z nl=2 -->

## Applicable Products<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: supported
<!-- end id7 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/resource_management/TQue/GetTensorCountInQue_res.md#id1 -->

## Description<a name="section618mcpsimp"></a>

The header file path is `"basic_api/kernel_tpipe.h"`.

Queries the number of tensors that have been enqueued in the queue.

## Prototype<a name="section620mcpsimp"></a>

```cpp
__aicore__ inline int32_t GetTensorCountInQue()
```

## Parameters<a name="section622mcpsimp"></a>

None

## Constraints<a name="section633mcpsimp"></a>

This API does not support [Tensor in-place operation](../../../../../guide/programming_guide/appendix/common_operations/tensor_inplace_performance.md), that is, the scenario where the depth of TQue is set to 0.

## Return Value<a name="section640mcpsimp"></a>

Returns the number of **Tensor**s that have been enqueued in the **Queue**.

## Example<a name="section642mcpsimp"></a>

```cpp
// Query the number of tensors enqueued in the queue through GetTensorCountInQue. Memory is allocated through the AllocTensor API and added to the queue, with num being 1.
AscendC::TPipe pipe;
AscendC::TQue<AscendC::TPosition::VECOUT, 4> que;
int num = 4;
int len = 1024;
pipe.InitBuffer(que, num, len);
AscendC::LocalTensor<half> tensor1 = que.AllocTensor<half>();
que.EnQue(tensor1);// Add tensor to the VECOUT queue.
int32_t numb = que.GetTensorCountInQue();
```
