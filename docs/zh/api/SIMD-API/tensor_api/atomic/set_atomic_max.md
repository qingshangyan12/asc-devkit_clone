# set_atomic_max

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`tensor_api/tensor.h`。

用于将待搬运到GM的数据与GM上已有数据进行比较，并将两者中的最大值写入GM，即开启后续数据搬运的原子取大功能。

## 函数原型

```cpp
template <typename T>
__aicore__ inline void set_atomic_max()
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 原子取大的数据类型。 |

`T`支持以下数据类型：

- `int8_t`
- `int16_t`
- `int32_t`
- `half`
- `bfloat16_t`
- `float`

## 返回值说明

无

## 约束说明

- 开启后对后续所有目的地址为GM的搬运指令生效，需调用[`disable_dma_atomic`](disable_dma_atomic.md)关闭。
- 本接口不会自动初始化GM上已有数据。若期望原子取大前GM上的原始数据为确定值，需手动初始化。
- 本接口仅对后续目的地址为GM的搬运指令生效，对标量写GM指令不生效。
- 后续搬运指令的操作数据类型必须与`T`一致。
- 本接口不能保证后续搬运指令的执行顺序。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void atomic_max_example()
{
    set_atomic_max<float>();
    // 执行受支持的UB或L0C到GM搬运。
    disable_dma_atomic();
}
```
