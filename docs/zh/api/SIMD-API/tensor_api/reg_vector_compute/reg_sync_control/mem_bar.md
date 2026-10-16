# mem_bar

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

头文件路径为：`"tensor_api/experimental/arch/vector/reg_sync_control.h"`，命名空间为`asc::te::experimental`。

`mem_bar`用于同步`SIMD_VF`函数内不同流水线对Unified Buffer（UB）的访问。目的流水线中位于`mem_bar`之后的UB访问，会等待源流水线中位于`mem_bar`之前的UB访问完成。

本接口仅在AIV上生效，不用于MTE流水线同步、核间同步或整核同步。

## 函数原型

```cpp
template <mem_type src, mem_type dst>
__simd_callee__ inline void mem_bar()
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| --- | --- |
| src | 源流水线，类型为`mem_type`。该流水线中位于本接口之前的UB访问需要先完成。 |
| dst | 目的流水线，类型为`mem_type`。该流水线中位于本接口之后的UB访问等待源流水线。 |

<a id="mem-bar-supported-combinations"></a>

**表2** 支持的`src`和`dst`组合

| src | dst | 含义 |
| --- | --- | --- |
| `mem_type::vec_store` | `mem_type::vec_load` | 矢量写流水线同步到矢量读流水线。 |
| `mem_type::vec_load` | `mem_type::vec_store` | 矢量读流水线同步到矢量写流水线。 |
| `mem_type::vec_store` | `mem_type::vec_store` | 矢量写流水线内部同步。 |
| `mem_type::vec_store` | `mem_type::scalar_load` | 矢量写流水线同步到标量读流水线。 |
| `mem_type::vec_load` | `mem_type::scalar_store` | 矢量读流水线同步到标量写流水线。 |
| `mem_type::vec_store` | `mem_type::scalar_store` | 矢量写流水线同步到标量写流水线。 |
| `mem_type::scalar_store` | `mem_type::vec_load` | 标量写流水线同步到矢量读流水线。 |
| `mem_type::scalar_load` | `mem_type::vec_store` | 标量读流水线同步到矢量写流水线。 |
| `mem_type::scalar_store` | `mem_type::vec_store` | 标量写流水线同步到矢量写流水线。 |
| `mem_type::vec_all` | `mem_type::vec_all` | 所有矢量读写流水线之间同步。 |
| `mem_type::vec_all` | `mem_type::scalar_all` | 所有矢量读写流水线同步到所有标量读写流水线。 |
| `mem_type::scalar_all` | `mem_type::vec_all` | 所有标量读写流水线同步到所有矢量读写流水线。 |

`vec_load`和`vec_store`分别表示SIMD VF内的矢量读UB和写UB流水线；
`scalar_load`和`scalar_store`分别表示标量读UB和写UB流水线；
`vec_all`和`scalar_all`分别表示全部矢量读写流水线和全部标量读写流水线。

## 返回值说明

无。

## 约束说明

- 仅支持[表2](#mem-bar-supported-combinations)列出的模板参数组合，其他组合会产生编译期错误。
- 仅当源流水线和目的流水线访问的UB地址存在依赖时使用本接口；冗余同步会降低性能。
- 当具有依赖关系的读写指令使用同一寄存器时，硬件可以保证指令顺序，无需额外调用本接口。
- 本接口应在`__simd_vf__`函数或`__simd_callee__`函数中调用。

## 调用示例

下面的示例用于循环间存在写后读依赖的场景。前一次循环将结果写入UB，下一次循环从相同地址读取数据，因此读取前需等待矢量写流水线完成。

```cpp
#include "tensor_api/experimental/arch/vector/reg_data_load.h"
#include "tensor_api/experimental/arch/vector/reg_data_store.h"
#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"
#include "tensor_api/experimental/arch/vector/reg_sync_control.h"

template <typename Tensor>
__simd_vf__ inline void Accumulate(Tensor data)
{
    using namespace asc::te::experimental;
    for (int32_t i = 0; i < 4; ++i) {
        mem_bar<mem_type::vec_store, mem_type::vec_load>();
        auto value = load(data);
        // 使用value完成计算后，将结果写回data。
        value.with_mask(all_mask<typename Tensor::data_type>());
        store(data, value);
    }
}
```
