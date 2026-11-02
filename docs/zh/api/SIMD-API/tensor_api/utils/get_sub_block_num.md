# get_sub_block_num

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

在分离模式下读取当前AI Core上Cube Core（AIC）或Vector Core（AIV）的数量。

该值由系统在核函数启动前配置，核函数运行期间不可修改。

## 函数原型

```cpp
__aicore__ inline int64_t get_sub_block_num()
```

## 参数说明

无

## 返回值说明

返回当前AI Core上的AIC或AIV数量。

## 约束说明

- 本接口读取的subblock（一个group内的从核）维度保存在只读特殊寄存器中，由系统控制器在核函数启动前配置，核函数运行期间不可修改，连续两次调用返回值相同。
- 返回值反映当前AI Core的subblock数量，仅在分离模式下有意义。非分离模式下返回值的语义以系统控制器配置为准。

## 调用示例

以下示例获取当前AI Core上AIC或AIV的数量。分离模式下AIC返回1，AIV返回当前AI Core内AIV的数量。

```cpp
#include "tensor_api/tensor.h"

__global__ __mix__(1, 2) void get_sub_block_num_example()
{
    using namespace asc::te;
    // sub_block_num is the number of AICs or AIVs in the current AI Core.
    const int64_t sub_block_num = get_sub_block_num();
}
```
