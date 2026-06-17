# normcdfinvf

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas 训练系列产品：不支持
<!-- end id6 -->

## 功能说明

获取输入数据x的标准正态累积分布的逆函数。

![](../../../figures/zh-cn_formulaimage_0000002533422599.png)

## 函数原型

```
inline float normcdfinvf(float x)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

获取输入数据x的标准正态累积分布的逆函数。

-   当x为0时，返回值为-inf。
-   当x为nan时，返回值为nan。
-   当x为1时，返回值为inf。
-   当x∉\[0,1\]时，返回nan。

## 约束说明

Ascend 950PR/Ascend 950DT底层实现normcdfinv采用多项式逼近，对极小输入（含subnormal区间）的精度不足：

-   **精度偏差（非FTZ主导）**：当输入x落在subnormal区间（0 < |x| < 1.17549435e-38）时，subnormal输入路径经logf保护可正常运算（输入不被刷零），但多项式逼近对极小输入的精度不足，结果与libm标准库存在偏差，该偏差非FTZ行为所致，而是SIMT实现中特殊值域处理的精度差异。

若业务对极小输入的精度敏感，需在host侧做相应补偿或使用更高精度实现。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

- SIMT编程场景：

    ```
    __global__ __launch_bounds__(256) void compute_normcdfinvf(float *result, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = normcdfinvf(x[idx]);
    }
    ```

- SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(256) inline void compute_normcdfinvf_vf(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = normcdfinvf(x[idx]);
    }

    __global__ __vector__ void run_normcdfinvf(__gm__ float *result, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_normcdfinvf_vf>(dim3(256), result, x, count);
    }
    ```

输入输出示例如下：

```
x：-0.75, -0.25, 0.25, 0.75
result: -0.6744897 0.6744897 nan nan
```
