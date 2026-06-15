# cbrtf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据x的立方根。

![](../../../figures/zh-cn_formulaimage_0000002484776414.png)

## 函数原型

```
inline float cbrtf(float x)
```

## 参数说明

**表 1**  函数形参说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

x的立方根。

-   当x为0时，返回值为0。
-   当x为nan时，返回值为nan。
-   当x为inf时，返回值为inf。
-   当x为-inf时，返回值为-inf。
-   当x为subnormal（非规格化数，即0 < |x| < 1.17549435e-38）时，底层位操作缺少subnormal浮点数的implicit 1 bit，导致x_adjusted错误，Newton迭代收敛至错误值，而非正确的立方根结果。

## 约束说明

Ascend 950PR/Ascend 950DT底层采用DML实现cbrt，不支持subnormal（非规格化数），对输入执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入x落在subnormal区间（0 < |x| < 1.17549435e-38）时，底层位操作缺少subnormal浮点数的implicit 1 bit，导致x_adjusted错误，Newton迭代收敛至错误值，而非按非FTZ标准库（如libm）得到的正确立方根结果。
-   **输出FTZ**：cbrt的正常输入返回值不会落入subnormal区间（cbrt将指数除以3），该场景一般不会触发。

若业务对subnormal输入的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelCbrt(float* x, float* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        y[idx] = cbrtf(x[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelCbrt(__gm__ float* x, __gm__ float* y)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        y[idx] = cbrtf(x[idx]);
    }
    ```

