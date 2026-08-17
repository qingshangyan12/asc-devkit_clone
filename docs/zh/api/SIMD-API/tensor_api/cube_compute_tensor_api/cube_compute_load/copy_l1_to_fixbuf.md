# copy（L1 Buffer到Fixpipe Buffer数据搬运）

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

Tensor API通过`copy`接口统一执行不同通路数据搬运。该接口用于将L1 Buffer中的量化数据搬运到Fixpipe Buffer。量化数据可用于L0C输出到GM/UB时做随路量化。

## 函数原型

- 执行L1 Buffer到Fixpipe Buffer的数据搬运。

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src)
    ```

- 按指定源坐标、目的坐标和搬运形状执行L1 Buffer到Fixpipe Buffer搬运。

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst,
        const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
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
| atom | 输入 | 搬运原子对象。L1 Buffer到Fixpipe Buffer通路可通过`make_copy(copy_l1_to_fixbuf{})`或`make_copy(copy_l1_to_fixbuf{}, l1_to_fixbuf_trait_default{})`构造。 |
| dst | 输出 | 目的张量，存储位置为`location::fixbuf`，数据格式为`ND`。 |
| src | 输入 | 源张量，存储位置为`location::l1`，数据格式为`ND`。 |
| dst_coord | 输入 | 搬运区域在目的张量中的起始坐标，也可传入`zero_coord`。 |
| src_coord | 输入 | 搬运区域在源张量中的起始坐标，也可传入`zero_coord`。 |
| copy_shape | 输入 | 搬运区域的shape。 |

**表2**  `make_copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| operation | 输入 | 搬运操作对象。L1 Buffer到Fixpipe Buffer通路使用`copy_l1_to_fixbuf{}`。 |
| trait | 输入 | 搬运Trait对象，用于指定搬运特性。L1 Buffer到Fixpipe Buffer通路默认Trait使用`l1_to_fixbuf_trait_default{}`。 |

## 数据类型

源张量和目的张量的数据类型要求是`uint64_t`。

## 返回值说明

`copy`无返回值。`make_copy`返回`copy_atom`对象。

## 约束说明

- 源Tensor内存地址和大小需要满足32Byte对齐要求。
- 目的Tensor内存地址和大小需要满足128Byte对齐要求。
- 由于Fixpipe Buffer大小为4KB，因此每次搬运最多4KB数据。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l1_to_fixbuf_example()
{
    using data_type = uint64_t;
    constexpr uint32_t n = 16;

    __cbuf__ data_type l1_buf[n];
    __fbuf__ data_type fixbuf[n];

    auto l1_tensor = make_tensor(make_mem_ptr(l1_buf), make_frame_layout<nd_ext_layout_ptn>(1, n));

    auto fixbuf_tensor = make_tensor(make_mem_ptr(fixbuf), make_frame_layout<nd_ext_layout_ptn>(1, n));

    auto atom = make_copy(copy_l1_to_fixbuf{}, l1_to_fixbuf_trait_default{});
    copy(atom, fixbuf_tensor, l1_tensor);
}
```
