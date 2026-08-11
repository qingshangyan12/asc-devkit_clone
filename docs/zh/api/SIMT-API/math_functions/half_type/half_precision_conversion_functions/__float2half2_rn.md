# \_\_float2half2\_rn

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

遵循CAST\_RINT模式，将float类型数据转换为half类型后复制到half2的两个分量，返回转换后的half2类型数据。

## 函数原型

```cpp
inline half2 __float2half2_rn(const float x)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| x | 输入 | 源操作数。 |

## 返回值说明

输入遵循CAST\_RINT模式转换并复制到两个分量后的half2类型数据。特殊值如下：

| x值 | 返回值 |
| --- | --- |
| ±0 | 两个分量均为±0 |
| nan | 两个分量均为0 |
| inf | 两个分量均为ASCRT\_MAX\_NORMAL\_FP16 |
| -inf | 两个分量均为-ASCRT\_MAX\_NORMAL\_FP16 |

## 约束说明

无

## 需要包含的头文件

使用该接口需要包含"simt\_api/asc\_fp16.h"头文件。

```cpp
#include "simt_api/asc_fp16.h"
```

## 调用示例

- SIMT编程场景：

    ```cpp
    __global__ __launch_bounds__(1024) void kernel___float2half2_rn(half2* dst, float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __float2half2_rn(x[idx]);
    }
    ```

- SIMD与SIMT混合编程场景：

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void kernel___float2half2_rn(__gm__ half2* dst, __gm__ float* x)
    {
        int idx = threadIdx.x + blockIdx.x * blockDim.x;
        dst[idx] = __float2half2_rn(x[idx]);
    }
    ```
