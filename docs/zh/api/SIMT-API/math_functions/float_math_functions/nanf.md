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

根据输入的字符串生成一个float类型的安静NaN值（浮点尾数最高位为1），并将`tagp`解析值的低22位写入该NaN值的低22位。

## 函数原型

```cpp
// SIMT编程场景
inline float nanf(const char* tagp)

// SIMD与SIMT混合编程场景
inline float nanf(const __gm__ char* tagp)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| tagp | 输入 | 源操作字符串首地址。支持十进制、八进制和十六进制格式；字符串无法按支持的进制解析时按0处理；解析溢出时按`UINT64_MAX`处理。 |

## 返回值说明

返回float类型的安静NaN值。返回值的低22位为`tagp`解析值的低22位；`tagp`字符串无法按支持的进制解析时，低22位为0；`tagp`解析溢出时，低22位为全1。

## 约束说明

- SIMT编程场景支持字符串字面量或`const char*`指针。
- SIMD与SIMT混合编程场景支持传入符串字面量或`__gm__`地址空间的字符串指针，不支持栈空间或UB地址。
- `tagp`以`0x`或`0X`开头时，按十六进制格式解析；以0开头且不以`0x`或`0X`开头时，按八进制格式解析；其他情况按十进制格式解析。

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
    __simt_vf__ __launch_bounds__(1024) inline void kernel_nanf(__gm__ float* dst,
        const __gm__ char* tagp0, const __gm__ char* tagp1, const __gm__ char* tagp2, const __gm__ char* tagp3)
    {
        dst[0] = nanf(tagp0);
        dst[1] = nanf(tagp1);
        dst[2] = nanf(tagp2);
        dst[3] = nanf(tagp3);
    }
    ```

输入输出示例如下：

```text
tagp："0x1A", "123", "abc", "0"
result: nan(0x7fc0001a) nan(0x7fc0007b) nan(0x7fc00000) nan(0x7fc00000)
```
