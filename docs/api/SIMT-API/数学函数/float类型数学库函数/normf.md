# normf

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 功能说明

获取输入数据a中前n个元素的平方和a\[0\]^2 + a\[1\]^2 +...+ a\[n-1\]^2的平方根。

![](../../../figures/zh-cn_formulaimage_0000002484776410.png)

## 函数原型

```
inline float normf(int n, float* a)
```

## 参数说明

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| n | 输入 | 源操作数。输入数据a中连续计算的元素个数。 |
| a | 输入 | 源操作数。Unified Buffer、Global Memory或栈空间的地址。 |

## 返回值说明

a\[0\]^2 + a\[1\]^2 + ...+ a\[n-1\]^2的平方根。

-   若a\[0\]^2 + a\[1\]^2 + ...+ a\[n-1\]^2的平方根超出float最大范围，返回值为inf。
-   若a\[0\]、a\[1\] 、... 、a\[n-1\]任意一个或多个为±inf，返回值为inf。
-   若a\[0\]、a\[1\] 、... 、a\[n-1\]任意一个或多个为nan同时不是±inf，返回值为nan。
-   若n小于1，返回a\[0\]的绝对值。

## 约束说明

-   输入数据a的长度必须大于等于参数n。
-   若n过大，接口性能无法保证。

Ascend 950PR/Ascend 950DT底层采用DML实现norm，不支持subnormal（非规格化数），对输入执行刷零（Flush-To-Zero, FTZ）：

-   **输入FTZ**：当输入a中元素落在subnormal区间（0 < |a\[i\]| < 1.17549435e-38）时，先被刷零为±0再参与运算。当仅部分元素为subnormal时，normf(2, \[sn, normal\]) ≈ |normal|，结果一致；当所有元素均为subnormal时，底层DML运算产生0/0=NaN，经fma(NaN)=NaN、sqrtf(NaN)=NaN路径，返回NaN（设计外行为），而非按非FTZ标准库（如libm）得到的有限值。
-   **输出FTZ**：normf的输出为正数平方根，正常输入难以使输出落入subnormal区间，该场景一般不会触发。

若业务对subnormal输入的精度敏感，需在调用前自行将subnormal区间的输入上调至最小规格化数，或在host侧做相应补偿。

## 需要包含的头文件

使用该接口需要包含"simt\_api/math\_functions.h"头文件。

```
#include "simt_api/math_functions.h"
```

## 调用示例

-   SIMT编程场景：

    ```
    __global__ __launch_bounds__(1024) void KernelNorm(float* dst, int* n, float* a)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = normf(n[idx], a);
    }
    ```

-   SIMD与SIMT混合编程场景：

    SIMD与SIMT混合编程场景，需要显式使用地址空间限定符表示地址空间：\_\_gm\_\_表示Global Memory内存空间，\_\_ubuf\_\_表示Unified Buffer内存空间，栈空间无需添加地址空间限定符。

    ```
    __simt_vf__ __launch_bounds__(1024) inline void KernelNorm(__gm__ float* dst, __gm__ int* n, __gm__ float* a)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = normf(n[idx], a);
    }
    ```

