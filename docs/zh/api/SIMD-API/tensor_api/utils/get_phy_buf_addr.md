# get_phy_buf_addr

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

基于偏移量获取片上实际物理地址。

本接口适用于调试、地址推导或tiling数据地址计算等需要将相对偏移量转换为硬件可用物理地址的场景。

## 函数原型

```cpp
__aicore__ inline uint64_t get_phy_buf_addr(uint64_t offset)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| offset | 输入 | 片上内存的地址偏移量，取值范围为`[0, 物理内存大小]`。 |

## 返回值说明

返回物理内存地址。

## 约束说明

- 本接口不能与以数组方式申请片上内存的方法混用。

## 调用示例

以下示例获取偏移量为0对应的片上实际物理地址。返回值为物理地址，不是Tensor指针或GM虚拟地址。

```cpp
#include "tensor_api/tensor.h"

__global__ __vector__ void get_phy_buf_addr_example()
{
    using namespace asc::te;
    // address is the physical address for offset 0.
    const uint64_t address = get_phy_buf_addr(0);
}
```
