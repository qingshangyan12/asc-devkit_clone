# scalblnf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据x与2的n次方的乘积。

![](../../../figures/zh-cn_formulaimage_0000002486788810.png)

## 函数原型

```
inline float scalblnf(float x, int64_t n)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |
| n | 输入 | 源操作数。 |

## 返回值说明

输入数据x乘以2的n次幂的结果。

-   当x为nan时，返回值为nan。
-   当x为inf时，返回值为inf。
-   当x为-inf时，返回值为-inf。
-   当x为subnormal（非规格化数，即0 < |x| < 1.17549435e-38）时，x被刷零（Flush-To-Zero）当作0处理，scalblnf(sn, n)返回±0。当x为正常值但x×2^n的结果落入subnormal区间时，输出被刷零为±0，而非按非FTZ标准库（如libm）得到的subnormal有限值。

## 约束说明

Ascend 950PR/Ascend 950DT底层实现scalblnf(x, n) = scalbnf(x, (int32_t)n)，与scalbnf同根因，不支持subnormal（非规格化数），对输入与输出均执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入x落在subnormal区间（0 < |x| < 1.17549435e-38）时，x先被刷零为±0再参与运算，scalblnf(sn, n)返回±0。
-   **输出FTZ**：当真实结果x×2^n落入subnormal区间时，输出被刷零为±0，而非按非FTZ标准库（如libm）得到的subnormal有限值。

若业务对subnormal输入或输出结果的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelScalblnf(float* dst, float* x, int64_t* n)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = scalblnf(x[idx], n[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelScalblnf(__gm__ float* dst, __gm__ float* x, __gm__ int64_t* n)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = scalblnf(x[idx], n[idx]);
    }
    ```
