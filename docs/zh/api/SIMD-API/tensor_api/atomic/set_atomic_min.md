# set_atomic_min

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
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

头文件路径为：`"tensor_api/arch/atomic/dma_atomic.h"`，命名空间为`asc::te`。

对后续受支持的目的地址为Global Memory的数据搬运开启原子取小模式。搬运时，待写入数据与GM上已有数据比较，并将较小值写入GM。该接口对应C API [`asc_set_atomic_min`](../../c_api/atomic/datamove_atomic/asc_set_atomic_min.md)。

## 函数原型

```cpp
namespace asc {
namespace te {
template <typename T>
__aicore__ inline void set_atomic_min();
}
}
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| T | 原子取小的数据类型。 |

`T`支持的数据类型与底层C API映射如下：

**表2** 数据类型与C API映射

| T | C API |
| :--- | :--- |
| `int8_t` | `asc_set_atomic_min_int8` |
| `int16_t` | `asc_set_atomic_min_int16` |
| `int32_t` | `asc_set_atomic_min_int32` |
| `half` | `asc_set_atomic_min_float16` |
| `bfloat16_t` | `asc_set_atomic_min_bfloat` |
| `float` | `asc_set_atomic_min_float` |

## 返回值说明

无

## 流水类型

`PIPE_S`

## 约束说明

- 开启后对后续所有受支持的目的地址为GM的搬运指令生效，需调用[`disable_dma_atomic`](disable_dma_atomic.md)关闭。
- 本接口不会自动初始化GM上已有数据。
- 仅对数据搬运指令生效，对标量写GM指令不生效。
- 后续搬运的数据类型必须与`T`一致。
- 产品支持的数据通路、确定性顺序和其他约束继承`asc_set_atomic_min`。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void atomic_min_example()
{
    asc::te::set_atomic_min<float>();
    // 执行受支持的UB或L0C到GM搬运。
    asc::te::disable_dma_atomic();
}
```

