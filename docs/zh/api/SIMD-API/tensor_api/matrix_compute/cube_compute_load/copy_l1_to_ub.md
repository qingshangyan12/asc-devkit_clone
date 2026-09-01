# copy（L1 Buffer到Unified Buffer数据搬运）

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
<!-- npu="910" id4 -->
- Atlas 训练系列产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品 AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`tensor_api/tensor.h`。

本接口将源张量的数据从L1 Buffer搬运到Unified Buffer。当前实现支持ND、DN和NZ格式的同格式搬运，源张量和目的张量需要使用相同的Layout模式。接口支持完整Tensor搬运，也支持使用`dst_coord`、`src_coord`和`copy_shape`指定搬运起始地址和搬运大小。

当源张量或目的张量的数据在逻辑上非连续存放时，应在Layout中通过Stride描述相邻逻辑数据块之间的元素间隔。调用`copy`时，Tensor API根据Layout的Shape和Stride自动推导底层搬运参数。接口还可通过`dst_coord`、`src_coord`和`copy_shape`指定搬运起始地址和搬运大小。

## 函数原型

- 执行L1 Buffer到Unified Buffer搬运。

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const copy_atom<Atom>& atom,
        const DstTensor& dst, const SrcTensor& src)
    ```

- 按指定源坐标、目的坐标和搬运形状执行L1 Buffer到Unified Buffer搬运。

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst,
        const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
    ```

- 使用默认trait构造搬运原子对象。

    ```cpp
    template <typename CopyOperation>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation)
    ```

- 使用指定trait构造搬运原子对象。

    ```cpp
    template <typename CopyOperation, typename CopyTrait>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation,
        const CopyTrait& trait)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| Atom | `copy_atom`的模板参数，由`make_copy`接口推导得到。 |
| DstTensor | 目的张量类型，需为Tensor API Tensor类型。 |
| SrcTensor | 源张量类型，需为Tensor API Tensor类型。 |
| DstCoord | 目的张量起始坐标类型，也可为`zero_coord`。 |
| SrcCoord | 源张量起始坐标类型，也可为`zero_coord`。 |
| CopyShape | 搬运区域形状的类型。 |
| CopyOperation | 搬运操作对象类型。 |
| CopyTrait | 搬运trait对象类型。 |

**表 2**  `copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| atom | 输入 | 搬运原子对象，可由`make_copy`接口构造。 |
| dst | 输出 | 目的张量，存储位置必须为`location::ub`。 |
| src | 输入 | 源张量，存储位置必须为`location::l1`。 |
| dst_coord | 输入 | 搬运区域在目的张量中的起始坐标。 |
| src_coord | 输入 | 搬运区域在源张量中的起始坐标。 |
| copy_shape | 输入 | 搬运区域的形状，用于指定搬运区域的大小。 |

**表 3**  `make_copy`接口参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| operation | 输入 | 搬运操作对象。L1 Buffer到Unified Buffer搬运取`copy_l1_to_ub{}`。 |
| trait | 输入 | 搬运trait对象。L1 Buffer到Unified Buffer默认取`l1_to_ub_trait_default{}`。 |

### copy_l1_to_ub说明

`copy_l1_to_ub`用于标识L1 Buffer到Unified Buffer数据搬运通路，仅列出其public内容：

```cpp
struct copy_l1_to_ub {
    template <typename Trait, const Trait& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};
```

`copy`静态成员函数用于接收Trait和搬运参数，并分发L1 Buffer到Unified Buffer数据搬运。

### l1_to_ub_trait说明

`l1_to_ub_trait`表示该通路的静态配置。当前没有可配置成员，定义如下：

```cpp
struct l1_to_ub_trait {};
```

`l1_to_ub_trait_default`提供该通路的默认Trait类型和默认值：

```cpp
struct l1_to_ub_trait_default {
    using trait_type = l1_to_ub_trait;
    static constexpr const trait_type value = {};
};
```


## 数据类型

源张量和目的张量使用相同的数据类型。具体可用类型取决于对应Layout和硬件搬运指令，使用时应以当前实现和编译器诊断为准。

## 返回值说明

`copy`无返回值。`make_copy`返回`copy_atom`对象。

## 约束说明

- `dst`必须位于`location::ub`，`src`必须位于`location::l1`。
- 源、目的Layout必须为相同的`nd_layout_ptn`、`nd_ext_layout_ptn`、`dn_layout_ptn`、`dn_ext_layout_ptn`或`nz_layout_ptn`组合；不同Layout组合不会自动转换。
- 源张量和目的张量的数据类型必须满足对应搬运指令要求，元素数量和实际存储容量必须覆盖搬运区域。
- 连续搬运时总字节数、块长度和地址对齐必须满足硬件搬运指令约束；高维切分时还需满足源/目的Stride和块数范围。
- 使用区域搬运时，`src_coord`、`dst_coord`和`copy_shape`的维度必须匹配对应张量，且源、目的区域均不能越界。
- 源张量和目的张量的Shape、Stride应表达相同的逻辑数据区域；非连续场景通过Layout的Stride描述，不能依赖指针偏移替代Layout信息。
- 不支持的Layout组合会在编译期触发路由静态断言；调试构建会检查Layout、Tensor容量和搬运大小。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l1_to_ub_example()
{
    using data_type = half;
    constexpr uint32_t n = 32;

    __cbuf__ data_type l1_buf[n];
    __ubuf__ data_type ub_buf[n];

    auto l1_tensor = make_tensor(make_mem_ptr(l1_buf), make_frame_layout<nd_ext_layout_ptn>(1, n));
    auto ub_tensor = make_tensor(make_mem_ptr(ub_buf), make_frame_layout<nd_ext_layout_ptn>(1, n));

    auto atom = make_copy(copy_l1_to_ub{}, l1_to_ub_trait_default{});
    copy(atom, ub_tensor, l1_tensor);
}
```

区域搬运示例：

```cpp
auto atom = make_copy(copy_l1_to_ub{}, l1_to_ub_trait_default{});
copy(atom, ub_tensor, l1_tensor, make_coord(0, 8), make_coord(0, 8), make_shape(1, 16));
```
