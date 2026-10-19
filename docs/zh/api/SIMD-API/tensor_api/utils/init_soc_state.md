# init_soc_state

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
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

头文件路径为：`"tensor_api/utils/utils.h"`，命名空间为`asc::te`。

`init_soc_state`用于将当前AI Core上可能由前序算子遗留的全局控制状态恢复为初始状态，避免这些状态影响当前算子的执行结果。该接口对应C API [`asc_init`](../../c_api/utils/sys_init/asc_init.md)。

本接口仅初始化控制状态，不会清零Global Memory、Local Memory或用户数据。

## 函数原型

```cpp
namespace asc {
namespace te {
__aicore__ inline void init_soc_state();
}
}
```

## 参数说明

无

## 返回值说明

无

## 流水类型

`PIPE_S`

## 约束说明

- 底层初始化范围、初始化结果和产品差异继承`asc_init`。
- 若前序算子修改过原子模式、Mask工作模式等控制状态，建议在算子入口调用本接口，避免非预期状态影响计算结果。
- 本接口不会清零Global Memory、Local Memory或用户数据。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__global__ __vector__ void kernel_example(__gm__ float* input, __gm__ float* output)
{
    asc::te::init_soc_state();
    // 后续执行Tensor API数据搬运、计算等操作。
}
```

