# nanf

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/math_functions/float_math_functions/nanf_res.md#id1 -->

## 功能说明

根据输入的字符串生成一个float类型的安静NaN值（浮点尾数最高位为1）。

## 函数原型

```cpp
inline float nanf(const char* tagp)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| tagp | 输入 | Global Memory上的源操作字符串首地址。支持十进制和十六进制格式；格式非法时按0处理。 |

## 返回值说明

返回float类型的安静NaN值。

## 约束说明

`tagp`只支持传入`__gm__`地址空间的字符串指针，不支持字符串字面量、栈空间或UB地址。

## 需要包含的头文件

使用该接口需要包含`simt_api/math_functions.h`头文件。

```cpp
#include "simt_api/math_functions.h"
```

## 调用示例

- SIMT编程场景：

    ```cpp
    __global__ __launch_bounds__(1024) void kernel_nanf(float* dst)
    {
        dst[0] = nanf("0x1A");
        dst[1] = nanf("123");
        dst[2] = nanf("abc");
        dst[3] = nanf("0");
    }
    ```

- SIMD与SIMT混合编程场景：

    ```cpp
    __simt_vf__ __launch_bounds__(1024) inline void kernel_nanf(__gm__ float* dst)
    {
        dst[0] = nanf("0x1A");
        dst[1] = nanf("123");
        dst[2] = nanf("abc");
        dst[3] = nanf("0");
    }
    ```

输入输出示例如下：

```text
tagp："0x1A", "123", "abc", "0"
result: nan(0x7fc0001a) nan(0x7fc0007b) nan(0x7fc00000) nan(0x7fc00000)
```
