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

头文件路径为：`"tensor_api/experimental/arch/vector/reg_sync_control.h"`，接口命名空间为`asc::te::experimental`。

`mem_bar`是Reg矢量计算函数内不同流水线之间的同步接口。接口通过模板参数指定源流水线和目的流水线：目的流水线中位于`mem_bar`之后的Unified Buffer（UB）访问，需要等待源流水线中位于`mem_bar`之前的UB访问全部完成后才能执行。

读写场景下，如果读指令和写指令使用相同的寄存器，硬件可以触发寄存器保序，指令会按照代码顺序执行，无需插入同步接口。如果使用不同的寄存器，并且需要保证两条流水线按代码顺序访问同一块UB地址空间，则需要调用本接口。写后写场景同理。

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
| src | 源流水线，类型为`mem_type`。该流水线中位于本接口之前的UB访问需要先完成，具体取值参见[表2](#mem-type-values)。 |
| dst | 目的流水线，类型为`mem_type`。该流水线中位于本接口之后的UB访问等待源流水线，具体取值参见[表2](#mem-type-values)。 |

<a id="mem-type-values"></a>

**表2** mem_type取值说明

| mem_type取值 | 含义 |
| --- | --- |
| vec_store | `__simd_vf__`函数内的矢量写UB流水线，对应寄存器到UB的数据搬出接口，例如`store`。 |
| vec_load | `__simd_vf__`函数内的矢量读UB流水线，对应UB到寄存器的数据搬入接口，例如`load`。 |
| scalar_store | `__simd_vf__`函数内的标量写UB流水线。 |
| scalar_load | `__simd_vf__`函数内的标量读UB流水线。 |
| vec_all | `__simd_vf__`函数内所有矢量读写UB流水线。 |
| scalar_all | `__simd_vf__`函数内所有标量读写UB流水线。 |

<a id="mem-bar-supported-combinations"></a>

**表3** `src`和`dst`组合取值说明

| src | dst |
| --- | --- |
| `mem_type::vec_store` | `mem_type::vec_load` |
| `mem_type::vec_load` | `mem_type::vec_store` |
| `mem_type::vec_store` | `mem_type::vec_store` |
| `mem_type::vec_store` | `mem_type::scalar_load` |
| `mem_type::vec_store` | `mem_type::scalar_store` |
| `mem_type::vec_load` | `mem_type::scalar_store` |
| `mem_type::scalar_store` | `mem_type::vec_load` |
| `mem_type::scalar_store` | `mem_type::vec_store` |
| `mem_type::scalar_load` | `mem_type::vec_store` |
| `mem_type::vec_all` | `mem_type::vec_all` |
| `mem_type::vec_all` | `mem_type::scalar_all` |
| `mem_type::scalar_all` | `mem_type::vec_all` |

## 返回值说明

无

## 数据类型

无

## 约束说明

- 仅支持[表3](#mem-bar-supported-combinations)列出的模板参数组合，其他组合会产生编译期错误。
- 读写依赖场景下，如果读指令和写指令使用相同的寄存器，会触发寄存器保序，无需额外调用本接口。
- 仅在不同流水线访问的UB地址存在依赖时使用本接口，冗余同步会降低性能。
- 本接口应在`__simd_vf__`函数或`__simd_callee__`函数中调用。

## 调用示例

如下示例中，`data`既是搬入数据的源Tensor，也是搬出数据的目的Tensor。后一次循环中的矢量读UB操作与前一次循环中的矢量写UB操作访问同一块UB地址空间，因此，每次读取前需要调用`mem_bar`，使矢量读流水线等待矢量写流水线完成。

```cpp
template <typename Tensor>
__simd_vf__ inline void Process(Tensor& data, uint16_t repeatTimes)
{
    using T = typename Tensor::data_type;
    const auto mask = asc::te::experimental::all_mask<T>();
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        asc::te::experimental::mem_bar<asc::te::experimental::mem_type::vec_store,
                                       asc::te::experimental::mem_type::vec_load>();
        auto value = asc::te::experimental::load(data, asc::te::make_coord(0));
        value.with_mask(mask);
        // 使用value完成寄存器计算。
        asc::te::experimental::store(data, asc::te::make_coord(0), value);
    }
}
```
