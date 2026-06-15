# hexp2

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

指定输入x，获取2的x次方。

![](../../../../figures/zh-cn_formulaimage_0000002533360279.png)

## 函数原型

```
inline half hexp2(half x)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

2的x次方。

-   当x为inf时，返回值为inf。
-   当x为-inf时，返回值为0。
-   当x为nan时，返回值为nan。
-   当结果超出float的最大范围时，返回值为inf。
-   当x为subnormal（非规格化数，即0 < |x| < 2^-14≈6.104e-5）时，hexp2(sn)≈1，与数学预期一致。但当x为较小负值使2^x落入F16 subnormal区间时，输出被刷零为0，而非libm得到的接近F16最小规格化数的值。

## 约束说明

不支持Subnormal场景：大多数处于Subnormal范围附近的输入和输出值，都会被刷新为保留符号的0。

## 需要包含的头文件

使用half类型接口需要包含"simt\_api/asc\_fp16.h"头文件。

```
#include "simt_api/asc_fp16.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelExp2(half* dst, half* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = hexp2(x[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelExp2(__gm__ half* dst, __gm__ half* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = hexp2(x[idx]);
    }
    ```

