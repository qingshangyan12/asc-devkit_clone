# jnf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据x的n阶第一类贝塞尔函数jn的值。

![](../../../figures/zh-cn_formulaimage_0000002484776432.png)

## 函数原型

```
inline float jnf(int n, float x)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| n | 输入 | 源操作数。 |
| x | 输入 | 源操作数。 |

## 返回值说明

输入数据的n阶第一类贝塞尔函数jn的值。

-   当n<0时，返回值为nan。
-   当x=inf或-inf时，返回值为0。
-   当x=nan时，返回值为nan。
-   当x为subnormal（非规格化数，即0 < |x| < 1.17549435e-38）时，x被刷零（Flush-To-Zero）当作0处理，jnf(n, sn)与jnf(n, 0)返回值一致（设计内FTZ行为）。当结果落入subnormal区间时（如jnf(85, 22)≈3.31e-41），输出被刷零为±0，而非libm得到的subnormal值。

## 约束说明

-   使用本接口时，配置的线程数不应超过256，否则有栈溢出风险。
-   n的最大取值为128。

Ascend 950PR/Ascend 950DT底层采用DML实现jn，不支持subnormal（非规格化数），对输入与输出均执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入x落在subnormal区间（0 < |x| < 1.17549435e-38）时，x先被刷零为±0再参与运算，jnf(n, sn)与jnf(n, 0)返回值一致（设计内FTZ行为）。
-   **输出FTZ**：当真实结果落入subnormal区间时（如jnf(85, 22)≈3.31e-41），输出被刷零为±0，而非按非FTZ标准库（如libm）得到的subnormal值。

若业务对subnormal输入或输出结果的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(256) void KernelJn(float* dst, int* n, float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = jnf(n[idx], x[idx]);
    }
    ```

-   SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(256) inline void KernelJn(__gm__ float* dst, __gm__ int* n, __gm__ float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = jnf(n[idx], x[idx]);
    }
    ```

