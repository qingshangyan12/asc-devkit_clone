# 复合计算

本目录介绍 `asc::te` 命名空间下将多个基本步骤融合为一次调用的寄存器计算接口。

## 头文件

```cpp
#include "tensor_api/experimental/reg/composite_compute.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [axpy](Axpy.md) | 标量乘与累加融合。 |
| [absdiff](Absdiff.md) | 减法与绝对值融合。 |
| [exp_sub](ExpSub.md) | 减法与自然指数融合。 |
| [fma](Fma.md) | `src0 * src1 + src2` 融合。 |
| [muls_cast](MulsCast.md) | 标量乘与 `float` 到 `half` 转换融合。 |

## 通用说明

- 接口返回新的 `reg_tensor`，不会要求调用方预先声明输出对象。
- 当前实现直接映射 V3510 对应的 C API，并在编译期限制类型和参数组合。
