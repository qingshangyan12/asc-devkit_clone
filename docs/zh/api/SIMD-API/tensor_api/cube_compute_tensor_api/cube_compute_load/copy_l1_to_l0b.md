# copy（L1 Buffer到L0B Buffer数据搬运）

> [!NOTE]说明
>本接口为试验接口，在后续版本中可能会调整或改进，不保证后续兼容性。请开发者在使用过程中关注后续版本更新。

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

头文件路径为：`tensor_api/tensor.h`。

Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的右矩阵数据搬运到L0B Buffer。

该通路以512Byte数据分形为基本搬运单位。不同数据类型对应的分形矩阵形态如下：

| 数据位宽 | 分形矩阵形态 |
| :--- | :--- |
| b4 | 64 * 16 |
| b8 | 32 * 16 |
| b16 | 16 * 16 |
| b32 | 8 * 16 |

接口支持非转置搬运和转置搬运。源张量和目的张量的layout需要匹配当前通路支持的格式组合。

接口支持Batch模式。Batch模式下，源张量和目的张量的layout需要在原有分形layout最前面增加Batch维，shape形态为`(b, 单矩阵shape)`，其中`b`表示Batch数量。用户可使用`make_frame_layout<zn_layout_ptn, DataType>(b, k, n)`或`make_frame_layout<nz_layout_ptn, DataType>(b, k, n)`构造带Batch维的layout。

## 函数原型

- 执行L1 Buffer到L0B Buffer的数据搬运。

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src)
    ```

- 构造默认搬运原子对象。

    ```cpp
    template <typename CopyOperation>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation)
    ```

- 构造指定Trait的搬运原子对象。

    ```cpp
    template <typename CopyOperation, typename CopyTrait>
    __aicore__ inline constexpr auto make_copy(
        const CopyOperation& operation, const CopyTrait& trait)
    ```

## 参数说明

**表1**  `copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| atom | 输入 | 搬运原子对象。L1 Buffer到L0B Buffer通路可通过`make_copy(copy_l1_to_l0b{})`或`make_copy(copy_l1_to_l0b{}, l1_to_l0b_trait_default{})`构造。 |
| dst | 输出 | 目的张量，存储位置必须为`location::l0b`。 |
| src | 输入 | 源张量，存储位置必须为`location::l1`。 |

**表2**  `make_copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| operation | 输入 | 搬运操作对象。L1 Buffer到L0B Buffer通路使用`copy_l1_to_l0b{}`。 |
| trait | 输入 | 搬运Trait对象，用于指定搬运特性。L1 Buffer到L0B Buffer默认Trait使用`l1_to_l0b_trait_default{}`。 |

## 数据类型

支持的数据类型包括：

`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`float`。

源张量和目的张量的数据类型需要保持一致。

## 返回值说明

`copy`无返回值。`make_copy`返回`copy_atom`对象。

## 约束说明

- 目的地址位于L0B Buffer时，起始地址需要满足512Byte对齐要求。
- 源地址位于L1 Buffer时，起始地址需要满足32Byte对齐要求。
- 特殊数据类型约束：L1 Buffer到L0B Buffer通路使能转置时，即`NZ2ZN`，支持的数据类型约束如下：
  - b32数据类型要求源矩阵shape在K轴方向16对齐。
  - b8数据类型要求源矩阵shape在M轴方向32对齐。
  - b4数据类型要求源矩阵shape在M轴方向64对齐。
- Batch模式要求源张量和目的张量的Batch数量一致，且在L1 Buffer和L0B Buffer上张量数据连续排布。

## 关键特性说明

### 非转置搬运

当源张量和目的张量数据格式均为`ZN`时，接口按非转置方式从L1 Buffer搬运到L0B Buffer。

**图1**  非转置搬运示意图

![非转置搬运示意图](../../../../figures/notranspose.png)

### 转置搬运

当源张量数据格式为`NZ`、目的张量数据格式为`ZN`时，接口按转置方式搬运。转置搬运会对源张量每个分形矩阵执行转置。

不同数据位宽的转置搬运示意如下。

#### b4转置搬运

**图2**  b4转置搬运示意图

![b4转置搬运示意图](../../../../figures/transpose_b4.png)

#### b8转置搬运

**图3**  b8转置搬运示意图

![b8转置搬运示意图](../../../../figures/transpose_b8.png)

#### b16转置搬运

**图4**  b16转置搬运示意图

![b16转置搬运示意图](../../../../figures/transpose_b16.png)

#### b32转置搬运

**图5**  b32转置搬运示意图

![b32转置搬运示意图](../../../../figures/transpose_b32.png)

### Batch搬运

当源张量和目的张量均使用带Batch维的layout时，`copy`接口会按照Batch维描述的数据范围完成L1 Buffer到L0B Buffer的数据搬运。非转置搬运时源张量、目的张量均为`ZN`格式，转置搬运时源张量为`NZ`格式，目的张量为`ZN`格式。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l1_to_l0b_example()
{
    using b_type = half;
    constexpr uint32_t k = 16;
    constexpr uint32_t n = 16;

    __cbuf__ b_type l1_buf[k * n];
    __cb__ b_type l0b_buf[k * n];

    auto l1_tensor = make_tensor(make_mem_ptr(l1_buf), make_frame_layout<zn_layout_ptn, b_type>(k, n));

    auto l0b_tensor = make_tensor(make_mem_ptr(l0b_buf), make_frame_layout<zn_layout_ptn, b_type>(k, n));

    auto atom = make_copy(copy_l1_to_l0b{}, l1_to_l0b_trait_default{});
    copy(atom, l0b_tensor, l1_tensor);
}
```

Batch模式示例如下。

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l1_to_l0b_batch_example()
{
    using b_type = half;
    constexpr uint32_t batch = 2;
    constexpr uint32_t k = 16;
    constexpr uint32_t n = 16;

    __cbuf__ b_type l1_buf[batch * k * n];
    __cb__ b_type l0b_buf[batch * k * n];

    auto layout = make_frame_layout<zn_layout_ptn, b_type>(batch, k, n);
    auto l1_tensor = make_tensor(make_mem_ptr(l1_buf), layout);
    auto l0b_tensor = make_tensor(make_mem_ptr(l0b_buf), layout);

    auto atom = make_copy(copy_l1_to_l0b{}, l1_to_l0b_trait_default{});
    copy(atom, l0b_tensor, l1_tensor);
}
```
