# fill

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

`fill`将标量填充值写入L1 Buffer，并按32字节物理块单位广播到多个块，可用于矩阵计算前的L1 Buffer初始化。

Tensor API通过`fill_params<T>`描述填充配置，并要求填充值类型`T`与目的Tensor的`data_type`完全一致。

## 函数原型

```cpp
template <typename T>
struct fill_params {
    uint16_t repeat_times = 0;
    uint16_t block_num = 0;
    uint16_t dst_gap = 0;
    T init_value = {};
};

template <typename Tensor, typename T>
__aicore__ inline void fill(const Tensor& dst, const fill_params<T>& params)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| Tensor | 目的Tensor类型。 |
| T | 填充值类型，必须与`Tensor::data_type`一致。 |

**表2** `fill_params`成员说明

| 成员 | 默认值 | 描述 |
| :--- | :---: | :--- |
| repeat_times | `0` | 重复写入次数。每次写入`block_num`个32字节块，两次写入之间跳过`dst_gap`个32字节块。取值范围为`[1, 32767]`。 |
| block_num | `0` | 单次重复写入的32字节物理块数。取值范围为`[1, 32767]`。 |
| dst_gap | `0` | 相邻两次重复之间的间隔，单位为32字节块。取值范围为`[0, 32767]`。 |
| init_value | `T{}` | 标量填充值。 |

**表3** `fill`参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | L1目的Tensor。 |
| params | 输入 | 物理块填充配置。 |

## 支持的数据类型

`T`支持：

- `half`
- `bfloat16_t`
- `int16_t`
- `uint16_t`
- `int32_t`
- `uint32_t`
- `float`

## 返回值说明

无

## 约束说明

- 本接口仅在AIC上执行有效，非AIC调用直接返回。
- 目的Tensor必须位于L1 Buffer。
- `T`必须与`Tensor::data_type`一致，否则编译期报错。
- 目的Tensor起始地址需要按照32字节对齐。
- L1 Buffer总容量为512KB。目的Tensor偏移量与实际占用空间之和不可超过L1 Buffer容量。
- 如果本指令与其他指令存在数据依赖，需要插入同步指令，保证多个指令串行化，防止出现异常数据。
- 填充范围由`repeat_times`、`block_num`和`dst_gap`共同描述，Tensor Layout不参与物理块范围推导。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void fill_l1_example()
{
    auto dst = make_tensor(
        make_mem_ptr<location::l1, float>(0),
        make_layout(make_shape(256)));

    // 写入256个float，共1024字节，即32个32字节块。
    fill(
        dst,
        fill_params<float>{1, 32, 0, 1.0f});
}
```

以下示例每次写入1个32字节块，重复5次，每次写入后跳过1个32字节块：

```cpp
// dst沿用上一示例中的L1 Tensor。
fill(
    dst,
    fill_params<float>{5, 1, 1, 0.0f});
```
