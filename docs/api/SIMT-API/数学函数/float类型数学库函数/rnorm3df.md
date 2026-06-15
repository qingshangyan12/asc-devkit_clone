# rnorm3df

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据a、b、c的平方和a^2 + b^2 + c^2的平方根的倒数。

![](../../../figures/zh-cn_formulaimage_0000002516816367.png)

## 函数原型

```
inline float rnorm3df(float a, float b, float c)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| a | 输入 | 源操作数。 |
| b | 输入 | 源操作数。 |
| c | 输入 | 源操作数。 |

## 返回值说明

a^2 + b^2 + c^2的平方根的倒数。

-   若a^2 + b^2 + c^2的平方根超出float最大范围，返回值为0。

-   若a^2 + b^2 + c^2平方根的倒数超出float最大范围，返回值为inf。

-   若a、b、c都为0，返回值为inf。

-   若a、b、c任意一个或多个为±inf，返回值为0。

-   若a、b、c任意一个或多个为nan同时不是±inf，返回值为nan。

## 约束说明

Ascend 950PR/Ascend 950DT底层采用DML实现rnorm3d，不支持subnormal（非规格化数），对输入执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入a、b或c落在subnormal区间（0 < |val| < 1.17549435e-38）时，先被刷零为±0再参与运算。当仅部分输入为subnormal时，返回值与normal输入一致；当所有输入均为subnormal时，底层norm3df返回NaN，rnorm3df(sn, sn, sn) = 1/NaN = NaN（设计外行为），而非按非FTZ标准库（如libm）得到的有限值。
-   **输出FTZ**：rnorm3df的输出为正数倒数，正常输入难以使输出落入subnormal区间，该场景一般不会触发。

若业务对subnormal输入的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelRnorm3d(float* dst, float* a, float* b, float* c)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = rnorm3df(a[idx], b[idx], c[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelRnorm3d(__gm__ float* dst, __gm__ float* a, __gm__ float* b, __gm__ float* c)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = rnorm3df(a[idx], b[idx], c[idx]);
    }
    ```

