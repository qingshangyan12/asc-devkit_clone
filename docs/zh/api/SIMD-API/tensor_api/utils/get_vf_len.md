# get_vf_len

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

获取当前芯片的矢量寄存器长度VL（Vector Length），单位为字节。常用于通过`get_vf_len() / sizeof(T)`推导单次寄存器计算可处理的元素个数。

## 函数原型

```cpp
__aicore__ inline constexpr int64_t get_vf_len()
```

## 参数说明

无

## 返回值说明

返回矢量寄存器长度VL，单位为字节。当前产品返回值为`256`。

## 约束说明

- 本接口不触发硬件指令，不依赖任何特殊寄存器或前置配置接口。
- AIC与AIV均可调用本接口，返回值含义一致。
- 同一核函数（Kernel）内多次调用返回值相同。
- 本接口不支持纯SIMT编译模式，在该模式下包含对应头文件会触发编译错误。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__global__ __vector__ void kernel_example(__gm__ float* output)
{
    constexpr int64_t vf_bytes = get_vf_len();
    constexpr int64_t elements = vf_bytes / sizeof(float);
    output[0] = static_cast<float>(elements);
}
```
