# asc_uintdiv

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
- Atlas 推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas 训练系列产品：不支持
<!-- end id6 -->

## 功能说明

整数快除算法分为两步计算商`quotient`。第一步调用[asc_get_uintdiv_magic_and_shift](../../../SIMD-API/c_api/scalar_compute/scalar_bit/asc_get_uintdiv_magic_and_shift.md)接口，根据除数计算`magic`和`shift`；第二步调用本接口，根据被除数`dividend`、`magic`和`shift`计算商。计算公式如下：

![](../../../figures/zh-cn_formulaimage_0000002600000001.png)

根据被除数数据类型的不同，公式中$N$的取值不同：当被除数为`uint32_t`数据类型时，$N$为32；当被除数为`uint64_t`数据类型时，$N$为64。本接口需要配合[asc_get_uintdiv_magic_and_shift](../../../SIMD-API/c_api/scalar_compute/scalar_bit/asc_get_uintdiv_magic_and_shift.md)接口完成除法运算。

## 函数原型

```cpp
__SIMT_DEVICE_FUNCTIONS_DECL__ inline uint32_t asc_uintdiv(uint32_t dividend, uint32_t magic, uint32_t shift)
```

```cpp
__SIMT_DEVICE_FUNCTIONS_DECL__ inline uint64_t asc_uintdiv(uint64_t dividend, uint64_t magic, uint64_t shift)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :------ | :--- | :------------ |
| dividend | 输入 | 无符号整数被除数。 |
| magic | 输入 | 与除数对应的快速除法乘法因子。 |
| shift | 输入 | 与除数对应的快速除法移位量。 |

## 返回值说明

返回`dividend`除以由`magic`和`shift`表示的除数得到的无符号整数商。

## 约束说明

1. 仅支持`uint32_t`和`uint64_t`重载。
2. `magic`和`shift`必须由`asc_get_uintdiv_magic_and_shift`根据非0除数计算得到，且二者的数据类型必须与`dividend`一致。
3. 当被除数为`uint32_t`数据类型时，支持的被除数最大值为`INT32_MAX`，即2的31次方减1；当被除数为`uint64_t`数据类型时，支持的被除数最大值为`INT64_MAX`，即2的63次方减1。
4. 本接口仅支持Ascend 950（NPU架构`dav-3510`）的SIMD+SIMT新同构编程方式，不支持纯SIMT编程方式。

## 需要包含的头文件

使用本接口需要包含`simt_api/math_functions.h`头文件。

```cpp
#include "simt_api/math_functions.h"
```

## 调用示例

以下示例在SIMD侧计算快速除法参数，再通过`asc_vf_call`传递给SIMT函数调用`asc_uintdiv`完成除法。

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
    uint32_t magic = 0;
    uint32_t shift = 0;
    asc_get_uintdiv_magic_and_shift(&magic, &shift, divisor);
    asc_vf_call<simt_fast_div>(dim3(1024), output, value, magic, shift);
}
```

完整样例请参考[SIMD与SIMT混合编程快速整数除法样例](../../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_integer_fast_div/README.md)。
