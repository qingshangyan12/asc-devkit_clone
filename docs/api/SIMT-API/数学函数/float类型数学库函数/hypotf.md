# hypotf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据x、y的平方和x^2 + y^2的平方根。

![](../../../figures/zh-cn_formulaimage_0000002516816363.png)

## 函数原型

```
inline float hypotf(float x, float y)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |
| y | 输入 | 源操作数。 |

## 返回值说明

x^2 + y^2的平方根。

-   若x^2 + y^2的平方根超出float最大范围，返回值为inf。
-   若x，y任意一个为±inf，返回值为inf。
-   若x，y任意一个为nan，另外不为±inf，返回值为nan。

## 约束说明

Ascend 950PR/Ascend 950DT底层采用DML实现hypot，不支持subnormal（非规格化数），对输入执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入x或y落在subnormal区间（0 < |x| < 1.17549435e-38）时，先被刷零为±0再参与运算。当仅部分输入为subnormal时，hypotf(sn, normal) ≈ normal，结果一致；当所有输入均为subnormal时，底层DML运算产生0/0=NaN，返回NaN（设计外行为），而非按非FTZ标准库（如libm）得到的有限值。
-   **输出FTZ**：hypotf的输出为正数平方根，正常输入难以使输出落入subnormal区间，该场景一般不会触发。

若业务对subnormal输入的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelHypot(float* dst, float* x, float* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = hypotf(x[idx], y[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelHypot(__gm__ float* dst, __gm__ float* x, __gm__ float* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = hypotf(x[idx], y[idx]);
    }
    ```

