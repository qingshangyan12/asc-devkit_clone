# \_\_isUbuf

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
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

判断输入的指针是否指向Unified Buffer内存空间的地址。

## 函数原型

```
unsigned int __isUbuf(const void* ptr)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| ptr | 输入 | 源操作数。 |

## 返回值说明

如果输入的指针指向Unified Buffer内存空间的地址，则返回1，否则返回0。

## 约束说明

SIMD与SIMT混合编程场景不支持使用该接口。

## 需要包含的头文件

使用该接口需要包含"simt\_api/device\_functions.h"头文件。

```
#include "simt_api/device_functions.h"
```

## 调用示例

SIMT编程场景：

```
__global__ __launch_bounds__(1024) void kernel__isUbuf(uint32_t* dst,
uint32_t* src)
{
    __ubuf__ uint32_t ptr[10];
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if(__isUbuf(ptr) == 1) {
       dst[idx] = 1;
    } else {
      dst[idx] = 0;
    }
}
```
