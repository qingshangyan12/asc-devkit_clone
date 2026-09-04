# asc_get_uintdiv_magic_and_shift

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/scalar_compute/compute/scalar_bit.h"`。

整数快除算法分为两步计算商。本接口用于第一步，根据无符号除数`divisor`计算乘法因子`magic`和移位量`shift`；第二步调用[asc_uintdiv](../../../../SIMT-API/math_functions/integer_math_functions/asc_uintdiv.md)接口，根据被除数、`magic`和`shift`计算商。

## 函数原型

```c
__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint32_t* magic, uint32_t* shift, uint32_t divisor)
```

```c
__aicore__ inline void asc_get_uintdiv_magic_and_shift(uint64_t* magic, uint64_t* shift, uint64_t divisor)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :------ | :--- | :------------ |
| magic | 输出 | 与`divisor`对应的快速除法乘法因子。 |
| shift | 输出 | 与`divisor`对应的快速除法移位量。 |
| divisor | 输入 | 无符号整数除数。 |

## 返回值说明

无返回值。计算结果通过`magic`和`shift`指针参数输出。

## 约束说明

- `divisor`不能为0。当`divisor`为`uint32_t`数据类型时，最大值为`INT32_MAX`，即2的31次方减1；当`divisor`为`uint64_t`数据类型时，最大值为`INT64_MAX`，即2的63次方减1。
- `magic`和`shift`指针必须指向可写的标量存储空间，且不能为`nullptr`。
- 本接口仅支持Ascend 950PR/Ascend 950DT（NPU架构`dav-3510`）的SIMD与SIMT混合编程方式，不支持纯SIMT编程方式。
- 本接口运行在标量流水（`PIPE_S`）上，同一流水内的数据依赖由指令执行顺序保证，无需额外同步。

## 调用示例

```cpp
#include "kernel_operator.h"

__simt_vf__ __launch_bounds__(1024) inline void simt_fast_div(
    __gm__ uint32_t* output, uint32_t value, uint32_t magic, uint32_t shift)
{
    uint32_t idx = threadIdx.x;
    output[idx] = asc_uintdiv(value, magic, shift);
}

__global__ __vector__ void integer_div_kernel(__gm__ uint32_t* output, uint32_t value, uint32_t divisor)
{
    // 在SIMD侧根据固定除数计算magic和shift，并传递给SIMT侧复用。
    uint32_t magic = 0;
    uint32_t shift = 0;
    asc_get_uintdiv_magic_and_shift(&magic, &shift, divisor);
    asc_vf_call<simt_fast_div>(dim3(1024), output, value, magic, shift);
}
```

完整样例请参考[SIMD与SIMT混合编程实现快速除法算子样例](../../../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_integer_fast_div/README.md)。
