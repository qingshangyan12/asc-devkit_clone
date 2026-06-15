# hsqrt

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据x的平方根。

![](../../../../figures/zh-cn_formulaimage_0000002533480327.png)

## 函数原型

```
inline half hsqrt(half x)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数，输入数据。 |

## 返回值说明

输入数据的平方根。

-   当x为0时，返回值为0。
-   当x为inf时，返回值为inf。
-   当x为-inf时，返回值为nan。
-   当x为nan时，返回值为nan。
-   当x为subnormal（非规格化数，即0 < |x| < 2^-14≈6.104e-5）时，x经h2f上抬FTZ→0后，sqrtf(0)→0，返回值为+0，而非libm得到的约2^-7~2^-11量级的小正数；其符号位保留（-subnormal的平方根仍按负数输入返回nan）。

## 约束说明

本接口支持的输入数据范围为x大于等于0，否则返回值为nan。

不支持Subnormal场景：大多数处于Subnormal范围附近的输入和输出值，都会被刷新为保留符号的0。

## 需要包含的头文件

使用half类型接口需要包含"simt\_api/asc\_fp16.h"头文件。

```
#include "simt_api/asc_fp16.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelSqrt(half* dst, half* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = hsqrt(x[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelSqrt(__gm__ half* dst, __gm__ half* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = hsqrt(x[idx]);
    }
    ```

