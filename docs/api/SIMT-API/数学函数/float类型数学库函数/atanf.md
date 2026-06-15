# atanf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据的反正切值。

![](../../../figures/zh-cn_formulaimage_0000002484776372.png)

## 函数原型

```
inline float atanf(float x)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

输入数据的反正切值。

-   当x为inf时，返回值为π/2。
-   当x为-inf时，返回值为-π/2。
-   当x为nan时，返回值为nan。
-   当x为subnormal（非规格化数，即0 < |x| < 1.17549435e-38）时，x被刷零（Flush-To-Zero）当作0处理，atanf(sn)返回0，而非按非FTZ标准库（如libm）得到的约等于sn的值（atanf(sn) ≈ sn）。

## 约束说明

Ascend 950PR/Ascend 950DT底层采用DML实现atan，不支持subnormal（非规格化数），对输入执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入x落在subnormal区间（0 < |x| < 1.17549435e-38）时，x先被刷零为±0再参与运算，因此atanf(sn)返回0，而非按非FTZ标准库（如libm）得到的约等于sn的值（atanf(sn) ≈ sn）。
-   **输出FTZ**：atanf的输出范围为(-π/2, π/2)，不会落入subnormal区间，该场景不会触发。

若业务对subnormal输入的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__  __launch_bounds__(1024) void KernelAtan(float* dst, float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = atanf(x[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__  __launch_bounds__(1024) inline void KernelAtan(__gm__ float* dst, __gm__ float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = atanf(x[idx]);
    }
    ```

