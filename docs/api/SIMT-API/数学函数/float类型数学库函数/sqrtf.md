# sqrtf

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

![](../../../figures/zh-cn_formulaimage_0000002516816361.png)

## 函数原型

```
inline float sqrtf(float x)
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
-   当x为subnormal（非规格化数，即0 \< |x| \< 1.17549435e-38）时，x被刷零（Flush-To-Zero）当作0处理，返回值为+0；其符号位保留（-subnormal的平方根仍按负数输入返回nan）。

## 约束说明

本接口支持的输入数据范围为x大于等于0，否则返回值为nan。

Ascend 950PR/Ascend 950DT底层采用DML实现sqrt，不支持subnormal（非规格化数），对输入与输出均执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入x落在subnormal区间（0 \< |x| \< 1.17549435e-38）时，x先被刷零为±0再参与运算，因此返回值为+0，而非按非FTZ标准库（如libm）得到的约1e-19~1e-23量级的小正数。
-   **输出FTZ**：当真实平方根结果落入subnormal区间时，输出被刷零为±0。对sqrt而言，正常输入难以使输出落入subnormal区，该场景一般不会触发。

若业务对subnormal输入的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

- SIMT编程场景：

    ```
    __global__ __launch_bounds__(256) void compute_sqrtf(float *result, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = sqrtf(x[idx]);
    }
    ```

- SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(256) inline void compute_sqrtf_vf(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = sqrtf(x[idx]);
    }

    __global__ __vector__ void run_sqrtf(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_sqrtf_vf>(dim3(256), result, x, count);
    }
    ```

输入输出示例如下：

```
x：0.25, 1.25, 2.25, 3.25
result: 0.5 1.118034 1.5 1.802776
```
