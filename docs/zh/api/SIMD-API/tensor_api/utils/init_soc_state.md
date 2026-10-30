# init_soc_state

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

`init_soc_state`用于将当前AI Core上可能由前序算子遗留的全局控制状态恢复为接口约定的初始状态，避免这些状态影响当前算子的执行结果。

本接口仅初始化控制状态，不会清零Global Memory、Local Memory或用户数据。

## 函数原型

```cpp
__aicore__ inline void init_soc_state()
```

## 参数说明

无

## 返回值说明

无

## 约束说明

- 在实际运行中，全局控制状态可能被前序执行的算子修改。若不调用本接口进行初始化，非预期的状态可能导致计算结果出现精度错误。
- 本接口不会清零Global Memory、Local Memory或用户数据。

## 调用示例

以下示例先调用init_soc_state恢复全局控制状态，再执行两个输入的加法计算。

```cpp
#include "tensor_api/tensor.h"

__global__ __vector__ void init_soc_state_kernel(__gm__ float* src0, __gm__ float* src1, __gm__ float* dst)
{
    using namespace asc::te;
    init_soc_state();
    dst[0] = src0[0] + src1[0];
}
```
