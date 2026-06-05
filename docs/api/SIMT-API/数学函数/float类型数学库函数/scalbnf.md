# scalbnf

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

获取输入数据x与2的n次方的乘积。

![](../../../figures/zh-cn_formulaimage_0000002486622064.png)

## 函数原型

```
inline float scalbnf(float x, int32_t n)
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
    #include "simt_api/math_functions.h"

    __global__ __launch_bounds__(256) void compute_scalbnf(float *result, const int *n, const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = scalbnf(x[idx], n[idx]);
    }
    ```

- SIMD与SIMT混合编程场景：

    ```
    #include "kernel_operator.h"
    #include "simt_api/math_functions.h"

    __simt_vf__ __launch_bounds__(256) inline void compute_scalbnf_vf(__gm__ float *result, __gm__ const int *n, __gm__ const float *x, uint32_t count)
    {
        const uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
        if (idx >= count) {
            return;
        }
        result[idx] = scalbnf(x[idx], n[idx]);
    }

    __global__ __vector__ void run_scalbnf(__gm__ float *result, __gm__ const int *n, __gm__ const float *x, uint32_t count)
    {
        asc_vf_call<compute_scalbnf_vf>(dim3(256), result, n, x, count);
    }
    ```

输入输出示例如下：

```
n：1, 2, 3, 1
x：0.25, 0.75, 1.25, 1.75
result: 0.5 3 10 3.5
```
