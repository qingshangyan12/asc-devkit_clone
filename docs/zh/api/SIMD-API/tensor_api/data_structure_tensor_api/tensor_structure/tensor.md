# tensor

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

需要包含的头文件为：#include "tensor_api/tensor.h"。

Tensor用于将内存访问引擎engine与布局layout绑定起来。engine描述物理存储位置和访问方式，layout描述数据的形状和布局。Tensor分为`global_tensor`和`local_tensor`两种，其中`global_tensor`用来存放Global Memory（外部存储）的全局数据，`local_tensor`用于存放AI Core中Local Memory（内部存储）的数据。二者的公共接口由`base_tensor<TensorEngine, TensorLayout>`提供。

## 原型定义

```cpp
enum class cache_mode : uint8_t { normal = 0, disable = 4, last = 5, persistent = 6 };

template <typename TensorEngine, typename TensorLayout>
struct base_tensor {
    using iterator = typename TensorEngine::iterator;
    using value_type = typename TensorEngine::value_type;
    using element_type = typename TensorEngine::element_type;
    using elementType = element_type;
    using reference = typename TensorEngine::reference;
    using engine_type = TensorEngine;
    using layout_type = TensorLayout;

    static constexpr int rank = TensorLayout::rank_size;

    __aicore__ inline base_tensor();
    __aicore__ inline base_tensor(const TensorEngine& engine, const TensorLayout& layout);

    __aicore__ inline constexpr decltype(auto) tensor() const;
    __aicore__ inline constexpr decltype(auto) engine() const;
    __aicore__ inline constexpr decltype(auto) engine();
    __aicore__ inline constexpr decltype(auto) layout() const;
    __aicore__ inline constexpr decltype(auto) data() const;
    __aicore__ inline constexpr decltype(auto) data();
    __aicore__ inline constexpr decltype(auto) shape() const;
    __aicore__ inline constexpr decltype(auto) stride() const;
    __aicore__ inline constexpr auto size() const;
    __aicore__ inline constexpr auto capacity() const;

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord);

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const;

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord);

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const;

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs);

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) const;

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info);

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info) const;

    [[deprecated("Tensor() is deprecated. Please use tensor() instead.")]]
    __aicore__ inline constexpr decltype(auto) Tensor() const;
    [[deprecated("Engine() is deprecated. Please use engine() instead.")]]
    __aicore__ inline constexpr decltype(auto) Engine() const;
    [[deprecated("Engine() is deprecated. Please use engine() instead.")]]
    __aicore__ inline constexpr decltype(auto) Engine();
    [[deprecated("Layout() is deprecated. Please use layout() instead.")]]
    __aicore__ inline constexpr decltype(auto) Layout() const;
    [[deprecated("Data() is deprecated. Please use data() instead.")]]
    __aicore__ inline constexpr decltype(auto) Data() const;
    [[deprecated("Data() is deprecated. Please use data() instead.")]]
    __aicore__ inline constexpr decltype(auto) Data();
    [[deprecated("Shape() is deprecated. Please use shape() instead.")]]
    __aicore__ inline constexpr decltype(auto) Shape() const;
    [[deprecated("Stride() is deprecated. Please use stride() instead.")]]
    __aicore__ inline constexpr decltype(auto) Stride() const;
    [[deprecated("Size() is deprecated. Please use size() instead.")]]
    __aicore__ inline constexpr auto Size() const;
    [[deprecated("Capacity() is deprecated. Please use capacity() instead.")]]
    __aicore__ inline constexpr auto Capacity() const;

    template <typename Coord, typename Info>
    [[deprecated("Slice() is deprecated. Please use slice() instead.")]]
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info);

    template <typename Coord, typename Info>
    [[deprecated("Slice() is deprecated. Please use slice() instead.")]]
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const;
};

template <typename TensorEngine, typename TensorLayout>
struct global_tensor : public base_tensor<TensorEngine, TensorLayout> {
    using tensor_api_base = base_tensor<TensorEngine, TensorLayout>;
    using layoutType = typename tensor_api_base::layout_type;
    using tensor_api_base::tensor_api_base;

    __aicore__ inline global_tensor();
    __aicore__ inline constexpr cache_mode get_cache_mode() const;
    __aicore__ inline constexpr void set_l2_cache_hint(cache_mode mode);
    [[deprecated("SetL2CacheHint() is deprecated. Please use set_l2_cache_hint() instead.")]]
    __aicore__ inline constexpr void SetL2CacheHint(AscendC::Te::CacheMode mode);
};

template <typename TensorEngine, typename TensorLayout>
struct local_tensor : public base_tensor<TensorEngine, TensorLayout> {
    using tensor_api_base = base_tensor<TensorEngine, TensorLayout>;
    using element_type = typename tensor_api_base::element_type;
    using data_type = get_attribute_element_type<element_type*>;
    using layoutType = typename tensor_api_base::layout_type;

    __aicore__ inline local_tensor();
    __aicore__ inline local_tensor(const TensorEngine& engine, const TensorLayout& layout);
};
```

## 参数说明

模板参数说明

| 参数名 | 类型 | 描述 |
|--------|------|------|
| TensorEngine | 输入 | 张量的内存访问引擎类型。 |
| TensorLayout | 输入 | 张量的布局类型，用于描述shape和stride。 |

## 返回值说明

Tensor相关类型本身为类型定义，不直接返回值。

通过make_tensor构造张量时：若存放Global Memory（外部存储）的全局数据，则返回global_tensor，存放AI Core中Local Memory（内部存储）的数据则返回local_tensor。

## 成员函数

### tensor()

- 功能说明

    获取张量对象自身。

- 函数原型

  ```cpp
  __aicore__ inline constexpr decltype(auto) tensor() const
  ```

- 返回值说明

    返回当前Tensor对象的常量引用。

---

### engine()

- 功能说明

    获取张量绑定的Engine对象。

- 函数原型

  ```cpp
  __aicore__ inline constexpr decltype(auto) engine() const
  __aicore__ inline constexpr decltype(auto) engine()
  ```

- 返回值说明

    返回Engine对象的（常量）引用。

---

### layout()

- 功能说明

    获取张量绑定的layout对象。

- 函数原型

  ```cpp
  __aicore__ inline constexpr decltype(auto) layout() const
  ```

- 返回值说明

    返回layout对象的常量引用。

---

### data()

- 功能说明

    获取底层数据起始迭代器。

- 函数原型

  ```cpp
  __aicore__ inline constexpr decltype(auto) data() const
  __aicore__ inline constexpr decltype(auto) data()
  ```

- 返回值说明

    返回`engine().begin()`的结果，即数据起始迭代器的（常量）引用或值。

---

### shape()

- 功能说明

    获取张量对应的shape信息。

- 函数原型

  ```cpp
  __aicore__ inline constexpr decltype(auto) shape() const
  ```

- 返回值说明

    返回layout中的shape对象。

---

### stride()

- 功能说明

    获取张量对应的stride信息。

- 函数原型

  ```cpp
  __aicore__ inline constexpr decltype(auto) stride() const
  ```

- 返回值说明

    返回layout中的stride对象。

---

### size()

- 功能说明

    获取张量覆盖的逻辑元素总数。

- 函数原型

  ```cpp
  __aicore__ inline constexpr auto size() const
  ```

- 返回值说明

    返回`layout().size()`的计算结果。

---

### capacity()

- 功能说明

    获取张量在当前布局下占用的实际容量。

- 函数原型

  ```cpp
  __aicore__ inline constexpr auto capacity() const
  ```

- 返回值说明

    返回`layout().capacity()`的计算结果。

---

### operator[]

- 功能说明

    按多维坐标访问单个元素。

- 函数原型

  ```cpp
  template <typename Coord>
  __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord)

  template <typename Coord>
  __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const
  ```

- 参数说明

  | 参数名 | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | coord | 输入 | 多维逻辑坐标。 |

- 约束说明
    仅\_\_gm_\_、 \_\_ubuf_\_地址空间支持使用该接口，AI Core上的地址空间（\_\_ca_\_、\_\_cb_\_、\_\_cc_\_、\_\_cbuf_\_）不具备标量直接访问能力。

- 返回值说明

    返回`data()[layout()(coord)]`对应的元素引用。

---

### operator()

- 功能说明

    以给定坐标为起点，返回一个从当前位置开始的子张量。

- 函数原型

  ```cpp
  template <typename Coord>
  __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord)

  template <typename Coord>
  __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const

  template <typename Coord0, typename Coord1, typename... Coords>
  __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs)

  template <typename Coord0, typename Coord1, typename... Coords>
  __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) const
  ```

- 参数说明

  | 参数名 | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | coord | 输入 | 起始坐标元组。 |
  | c0, c1, cs... | 输入 | 多个坐标参数，会被包装为make_coord(c0, c1, cs...)。 |

- 返回值说明

    返回从指定坐标位置开始的子张量对象。

---

### slice()

- 功能说明

    从指定坐标处切出一个带新布局信息的子张量。

- 函数原型

  ```cpp
  template <typename Coord, typename Info>
  __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info)

  template <typename Coord, typename Info>
  __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info) const
  ```

- 参数说明

  | 参数名 | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | coord | 输入 | 切片起始坐标。 |
  | info | 输入 | 切片描述信息，可以为shape或layout。 |

- 返回值说明

    返回切片后的子张量对象。新张量的engine指向切片起始位置，layout根据`coord`和`info`生成。

---

### get_cache_mode()

- 功能说明

    获取global_tensor当前配置的L2 Cache Hint。

- 函数原型

  ```cpp
  __aicore__ inline constexpr cache_mode get_cache_mode() const
  ```

- 返回值说明

    返回当前缓存模式，类型为`cache_mode`。

- 约束说明

    该接口仅存在于global_tensor中，local_tensor不提供此接口。

---

### set_l2_cache_hint()

- 功能说明

    设置global_tensor访问GM时的L2 Cache Hint。

- 函数原型

  ```cpp
  __aicore__ inline constexpr void set_l2_cache_hint(cache_mode mode)
  ```

- 参数说明

  | 参数名 | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | mode | 输入 | L2缓存模式。 |

- 返回值说明

    无返回值。

- 约束说明

    该接口仅存在于global_tensor中，local_tensor不提供此接口。

---

### 大驼峰兼容成员函数（废弃）

- 功能说明

    以下大驼峰成员函数仅用于兼容历史代码，均为deprecated接口。调用时编译器会产生废弃告警，新开发代码请使用对应的snake_case成员函数。

  | 废弃成员函数 | 所属结构体 | 替代成员函数 |
  | :----------- | :--------- | :----------- |
  | Tensor() | base_tensor | tensor() |
  | Engine() | base_tensor | engine() |
  | Layout() | base_tensor | layout() |
  | Data() | base_tensor | data() |
  | Shape() | base_tensor | shape() |
  | Stride() | base_tensor | stride() |
  | Size() | base_tensor | size() |
  | Capacity() | base_tensor | capacity() |
  | Slice() | base_tensor | slice() |
  | SetL2CacheHint() | global_tensor | set_l2_cache_hint() |

- 函数原型

  ```cpp
  [[deprecated("Tensor() is deprecated. Please use tensor() instead.")]]
  __aicore__ inline constexpr decltype(auto) Tensor() const

  [[deprecated("Engine() is deprecated. Please use engine() instead.")]]
  __aicore__ inline constexpr decltype(auto) Engine() const

  [[deprecated("Engine() is deprecated. Please use engine() instead.")]]
  __aicore__ inline constexpr decltype(auto) Engine()

  [[deprecated("Layout() is deprecated. Please use layout() instead.")]]
  __aicore__ inline constexpr decltype(auto) Layout() const

  [[deprecated("Data() is deprecated. Please use data() instead.")]]
  __aicore__ inline constexpr decltype(auto) Data() const

  [[deprecated("Data() is deprecated. Please use data() instead.")]]
  __aicore__ inline constexpr decltype(auto) Data()

  [[deprecated("Shape() is deprecated. Please use shape() instead.")]]
  __aicore__ inline constexpr decltype(auto) Shape() const

  [[deprecated("Stride() is deprecated. Please use stride() instead.")]]
  __aicore__ inline constexpr decltype(auto) Stride() const

  [[deprecated("Size() is deprecated. Please use size() instead.")]]
  __aicore__ inline constexpr auto Size() const

  [[deprecated("Capacity() is deprecated. Please use capacity() instead.")]]
  __aicore__ inline constexpr auto Capacity() const

  template <typename Coord, typename Info>
  [[deprecated("Slice() is deprecated. Please use slice() instead.")]]
  __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info)

  template <typename Coord, typename Info>
  [[deprecated("Slice() is deprecated. Please use slice() instead.")]]
  __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const

  [[deprecated("SetL2CacheHint() is deprecated. Please use set_l2_cache_hint() instead.")]]
  __aicore__ inline constexpr void SetL2CacheHint(AscendC::Te::CacheMode mode)
  ```

- 参数说明

    参数与对应的snake_case成员函数一致。

- 返回值说明

    与对应的snake_case成员函数一致。

## 调用示例

```cpp
using namespace asc::te;
constexpr uint64_t gm_addr = 128;

// 示例1：构造一个GM张量
auto gm_layout = make_layout(make_shape(128, 128), make_stride(128, 1));
auto gm_tensor = make_tensor(make_mem_ptr<location::gm, float>(gm_addr), gm_layout);

auto gm_shape = gm_tensor.shape();        // (128, 128)
auto gm_stride = gm_tensor.stride();      // (128, 1)
auto gm_size = gm_tensor.size();          // 16384
auto gm_cap = gm_tensor.capacity();       // 16384
gm_tensor.set_l2_cache_hint(cache_mode::normal);

// 示例2：按坐标访问元素
auto coord = make_coord(5, 10);
auto value = gm_tensor[coord];

// 示例3：获取子张量
auto sub_tensor = gm_tensor(make_coord(16, 16));

// 示例4：按shape切片
auto slice_tensor = gm_tensor.slice(make_coord(0, 0), make_shape(32, 32));

// 示例5：构造一个local_tensor
auto local_layout = make_frame_layout<nz_layout_ptn, layout_trait_default<float>>(32, 32);
auto local_tensor = make_tensor(make_mem_ptr<location::l1, float>(l1_addr), local_layout);
auto local_data = local_tensor.data();
```
