# copy（Global Memory到Unified Buffer数据搬运）

> [!NOTE]说明
>本接口为试验接口，在后续版本中可能会调整或改进，不保证后续兼容性。请开发者在使用过程中关注后续版本更新。

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
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

本接口将源Tensor的数据从Global Memory搬运到Unified Buffer。接口支持完整Tensor搬运和指定坐标、Shape的区域搬运，并可通过`gm_to_ub_params`配置目的端左右Padding。

## 函数原型

```cpp
template <typename Atom, typename DstTensor, typename SrcTensor>
__aicore__ inline void copy(
    const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src)

template <typename Atom, typename DstTensor, typename SrcTensor,
    typename DstCoord, typename SrcCoord, typename CopyShape>
__aicore__ inline void copy(
    const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src,
    const DstCoord& dst_coord, const SrcCoord& src_coord,
    const CopyShape& copy_shape)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| atom | 输入 | 由`make_copy(copy_gm_to_ub{})`构造的搬运Atom。 |
| dst | 输出 | 目的Tensor，必须位于Unified Buffer。 |
| src | 输入 | 源Tensor，必须位于Global Memory。 |
| dst_coord | 输入 | 搬运区域在目的Tensor中的起始坐标。 |
| src_coord | 输入 | 搬运区域在源Tensor中的起始坐标。 |
| copy_shape | 输入 | 搬运区域的Shape。 |

### Operation、Trait和Params

```cpp
struct copy_gm_to_ub {
    template <typename Trait, const Trait& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};

struct gm_to_ub_trait {};

struct gm_to_ub_trait_default {
    using trait_type = gm_to_ub_trait;
    static constexpr const trait_type value = {};
};

struct gm_to_ub_params {
    uint8_t left_padding_count = 0;
    uint8_t right_padding_count = 0;
    bool enable_constant_pad = true;
};
```

`left_padding_count`和`right_padding_count`分别表示每个搬运块写入Unified Buffer时在有效数据左右保留的Padding元素数量。`enable_constant_pad`为true时写入Padding值，为false时不写Padding区域。通过`make_copy(copy_gm_to_ub{}).with(params)`绑定Params。

## 约束说明

- 源和目的Tensor的元素类型必须一致，元素类型大小支持1、2、4或8字节。
- 支持相同格式间搬运：`ND`、`NDExt`、`DN`、`DNExt`、`NZ`和`ZN`。
- Padding元素数量和目的Stride需要满足底层Global Memory到Unified Buffer搬运指令约束。
- 区域搬运的坐标、Shape及范围必须合法。

## 调用示例

```cpp
gm_to_ub_params params{0, 0, true};
auto atom = make_copy(copy_gm_to_ub{}).with(params);
copy(atom, ub_tensor, gm_tensor);
```
