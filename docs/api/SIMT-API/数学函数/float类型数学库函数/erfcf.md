# erfcf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据的互补误差函数值。

![](../../../figures/zh-cn_formulaimage_0000002516816375.png)

## 函数原型

```
inline float erfcf(float x)
```

## 参数说明

**表 1**  函数形参说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

-   当x为nan时，返回值为nan。
-   当x为inf时，返回值为0。
-   当x为-inf时，返回值为2。

## 约束说明

Ascend 950PR/Ascend 950DT底层采用DML实现erfc，不支持subnormal（非规格化数），对输出执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：erfcf的正常浮点输入不会因FTZ产生明显偏差，该场景一般不会触发。
-   **输出FTZ**：当erfcf的真实计算结果落入subnormal区间（0 < |result| < 1.17549435e-38）时，输出被刷零为0。该场景在x为大正输入时触发：erfcf(大正输入)按libm标准应为subnormal量级的小正数，DML实现将其FTZ为0。此FTZ行为属于设计内行为，对大多数业务场景影响有限。

若业务对erfcf在大正输入下的subnormal精度敏感，需在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelErfc(float* x, float* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        y[idx] = erfcf(x[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelErfc(__gm__ float* x, __gm__ float* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        y[idx] = erfcf(x[idx]);
    }
    ```

