# atan2f

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x |
| Atlas 200I/500 A2 推理产品 | x |
| Atlas 推理系列产品AI Core | x |
| Atlas 推理系列产品Vector Core | x |
| Atlas 训练系列产品 | x |

## 功能说明

获取输入数据y/x的反正切值。

![](../../../figures/zh-cn_formulaimage_0000002516816331.png)

## 函数原型

```
inline float atan2f(float y, float x)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| y | 输入 | 源操作数。 |
| x | 输入 | 源操作数。 |

## 返回值说明

y/x的反正切值。

-   当x，y任意为nan时，返回值为nan。
-   当x和y均为±0时，返回值为0。
-   当y=inf，x=inf时，返回值为π/4。
-   当y=-inf，x=inf时，返回值为-π/4。
-   当y=1，x=inf时，返回值为0.0。
-   当y=inf，x=-inf时，返回值为π3/4。
-   当y=-inf，x=-inf时，返回值为-π3/4。
-   当y=1，x=-inf时，返回值为π。
-   当y=inf，x=1时，返回值为π/2。
-   当y=-inf，x=1时，返回值为-π/2。

## 约束说明

无

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

- SIMT编程场景：

    ```
    __global__ __launch_bounds__(256) void compute_atan2f(float *result, const float *x, const float *y, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = atan2f(y[idx], x[idx]);
    }
    ```

- SIMD与SIMT混合编程场景：

    ```
    __simt_vf__ __launch_bounds__(256) inline void compute_atan2f_vf(__gm__ float *result, __gm__ const float *x, __gm__ const float *y, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = atan2f(y[idx], x[idx]);
    }

    __global__ __vector__ void run_atan2f(__gm__ float *result, __gm__ const float *x, __gm__ const float *y, uint32_t count)
    {
        asc_vf_call<compute_atan2f_vf>(dim3(256), result, x, y, count);
    }
    ```

输入输出示例如下：

```
y：1.5, 2.5, 3.5, 4.5
x：0.25, 0.75, 1.25, 1.75
result: 1.405648 1.27934 1.227772 1.199905
```
