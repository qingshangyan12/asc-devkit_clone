# sinf

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

获取输入数据的三角函数正弦值。

![](../../../figures/zh-cn_formulaimage_0000002484776392.png)

## 函数原型

```
inline float sinf(float x)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

输入数据的三角函数正弦值。

-   当x为inf时，返回值为nan。
-   当x为-inf时，返回值为nan。
-   当x为nan时，返回值为nan。

## 约束说明

使用本接口时，线程配置最大不超过1024，否则有栈溢出风险。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

- SIMT编程场景：

```
#include "simt_api/asc_simt.h"
#include "simt_api/math_functions.h"

__global__ __launch_bounds__(256) void compute_sinf(float *result, const float *x, uint32_t count)
{
    const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= count) {
        return;
    }
    result[idx] = sinf(x[idx]);
}
```

- SIMD与SIMT混合编程场景：

```
#include "kernel_operator.h"
#include "simt_api/asc_simt.h"
#include "simt_api/math_functions.h"

__simt_vf__ __launch_bounds__(256) inline void compute_sinf_vf(__gm__ float *result, __gm__ const float *x, uint32_t count)
{
    const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= count) {
        return;
    }
    result[idx] = sinf(x[idx]);
}

__global__ __vector__ void run_sinf(__gm__ float *result, __gm__ const float *x, uint32_t count)
{
    asc_vf_call<compute_sinf_vf>(dim3(256), result, x, count);
}
```

- 输入输出

```
x：0.25, 0.75, 1.25, 1.75
result: 0.247404 0.6816388 0.9489846 0.983986
```
