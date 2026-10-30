# get_sub_block_id

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
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`tensor_api/tensor.h`。

在分离模式下获取当前逻辑AI Core上Cube Core（AIC）或Vector Core（AIV）的逻辑索引。subblock（一个group内的从核）即当前逻辑AI Core内的AIC或AIV。

## 函数原型

```cpp
__aicore__ inline int64_t get_sub_block_id()
```

## 参数说明

无

## 返回值说明

返回当前逻辑AI Core上AIC或AIV的逻辑索引。

返回值范围为`[0, get_sub_block_num())`。

## 约束说明

- 本接口为只读查询接口，不修改寄存器或存储状态。

## 调用示例

以下示例获取当前AIC或AIV的逻辑索引。分离模式下AIC返回0；AIV返回其在当前AI Core内的AIV编号。

```cpp
#include "tensor_api/tensor.h"

__global__ __mix__(1, 2) void get_sub_block_id_example()
{
    using namespace asc::te;
    // sub_block_id is the logical index of the current AIC or AIV.
    const int64_t sub_block_id = get_sub_block_id();
}
```
