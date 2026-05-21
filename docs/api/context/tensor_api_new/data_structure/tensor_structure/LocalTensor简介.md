# LocalTensor简介

LocalTensor由MakeTensor将内存指针适配器（Engine）与布局对象（Layout）绑定后生成。它封装了"数据在哪里（Engine）、数据长什么样（Layout）、如何访问（operator[]/operator()）"的全部信息，并作为 `Copy`（数据搬运）和 `Mmad`（矩阵计算）的统一操作数类型。

`LocalTensor` 的完整类型为：

```
LocalTensor<EngineType, LayoutType>
```

## 需要包含的头文件

```cpp
#include "tensor_api/tensor.h"
```

## 原型定义

```cpp
template <typename EngineType, typename LayoutType>
struct LocalTensor<EngineType, LayoutType>
{
    using engineType  = EngineType;
    using layoutType  = LayoutType;
    static constexpr int rank = LayoutType::rank;

    __aicore__ inline constexpr LocalTensor()
    __aicore__ inline constexpr LocalTensor(const EngineType& engine, const LayoutType& layout)

    __aicore__ inline constexpr decltype(auto) Tensor() const
    __aicore__ inline constexpr decltype(auto) Engine() const
    __aicore__ inline constexpr decltype(auto) Engine()
    __aicore__ inline constexpr decltype(auto) Layout() const
    __aicore__ inline constexpr decltype(auto) Data() const
    __aicore__ inline constexpr decltype(auto) Shape() const
    __aicore__ inline constexpr decltype(auto) Stride() const
    __aicore__ inline constexpr auto Size() const
    __aicore__ inline constexpr auto Capacity() const

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord)
    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord)
    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info)

    __aicore__ inline constexpr void SetL2CacheHint(CacheMode mode)
};
```

## 成员函数
### Tensor()

- 功能说明

    获取张量对象。

- 函数原型
  ```cpp
  __aicore__ inline constexpr decltype(auto) Tensor() const
  ```

- 返回值说明
    
    返回LocalTensor的常量引用。

---

### Engine() 

- 功能说明

    获取ViewEngine对象。

- 函数原型
  ```cpp
  __aicore__ inline constexpr decltype(auto) Engine() const
  __aicore__ inline constexpr decltype(auto) Engine()
  ```

- 返回值说明
    
    返回ViewEngine对象的（常量）引用。

---

### Layout()

- 功能说明

    获取Layout对象。

- 函数原型
  ```cpp
  __aicore__ inline constexpr decltype(auto) Layout() const
  ```

- 返回值说明

    返回Layout对象的常量引用。

---

### Data()

- 功能说明

    获取数据指针。

- 函数原型
  ```cpp
  __aicore__ inline constexpr decltype(auto) Data() const
  __aicore__ inline constexpr decltype(auto) Data()
  ```

- 返回值说明
    
    返回数据迭代器的（常量）引用。

---

### Shape()

- 功能说明

    获取Shape信息。

- 函数原型
  ```cpp
  __aicore__ inline constexpr decltype(auto) Shape() const
  ```

- 返回值说明

    返回Shape元组。

---

### Stride()

- 功能说明

    获取Stride信息。

- 函数原型
  ```cpp
  __aicore__ inline constexpr decltype(auto) Stride() const
  ```

- 返回值说明
    
    返回Stride元组。

---

### Size()

- 功能说明

    获取元素总数。

- 函数原型
  ```cpp
  __aicore__ inline constexpr auto Size() const
  ```

- 返回值说明

    返回LocalTensor的元素总数。

---

### Capacity() 
- 功能说明

    获取总容量。当非连续存储时，容量可能大于实际元素数量。

- 函数原型
  ```cpp
  __aicore__ inline constexpr auto Capacity() const
  ```

- 返回值说明

    返回LocalTensor的总容量。

---

### operator[] 
- 功能说明
    
    坐标访问元素。

- 函数原型
  ```cpp
  template <typename Coord>
  __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord)

  template <typename Coord>
  __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const
  ```

- 参数说明

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | coord | 输入 | 坐标元组。 |

- 返回值说明

    返回对应位置的元素引用。

---

### operator()
- 功能说明

    该接口以coord作为起始坐标，返回一个从当前位置开始的子LocalTensor。

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

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | coord | 输入 | 偏移坐标。 |
  | c0, c1, cs... | 输入 | 多个坐标参数。 |

- 返回值说明

    返回由coord（或c0、c1...）指定偏移位置的LocalTensor对象引用。

---

### Slice(const Coord& coord, const Info& info) 

- 功能说明

    分块访问。

    该接口以coord作为起始坐标，并结合info信息返回一个从当前位置开始的子LocalTensor。

    info支持两种形式：

    1. 传入Shape
       表示按给定形状截取子张量。
       - info必须是二维Shape；
       - 当前Tensor中的Layout只支持二维或四维；
       - 若当前Tensor为二维Layout，则返回二维子Layout；
       - 若当前Tensor为四维Layout，则在保持原布局模式的前提下，按二维逻辑区域裁剪后返回四维子Layout。

    2. 传入Layout
       表示按给定Layout 的Shape信息截取子张量。
       - info必须是Layout对象；
       - info的维度必须与当前Tensor中Layout的维度相同；
       - 返回结果保持当前Tensor的布局模式和 stride，只更新切片后的 shape。

- 函数原型

  ```cpp
    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) 
  
    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const
  ```

- 参数说明

  | 参数名  | 输入/输出 | 描述 |
  | :----- | :------- | :------- |
  | coord | 输入 | 起始坐标。 |
  | info | 输入 | 分块信息。可以传入二维 Shape，也可以传入与当前Tensor的Layou 维度相同的Layout。 |

- 返回值说明

    返回从coord指定位置开始的子LocalTensor。返回结果包含新的 Layout信息，Engine 指向切片后的起始位置。

- 约束说明

    - coord的 tuple 结构必须与当前 Tensor的Layout.Shape()结构一致。
    - 当info为Shape时：info必须是二维Shape；当前Tensor的Layout必须为二维或四维。
    - 当info为Layout时，info的必须与当前Tensor的Layout的维度相同。

## 调用示例

  ```cpp
  // 示例1：基本LocalTensor创建和访问
  // 创建L1内存指针
  constexpr int tileNum = 32;
  __cbuf__ half data[tileNum];
  auto l1Ptr = MakeMemPtr(data);
  auto nzLayout = MakeFrameLayout<NZLayoutPtn, half>(32, 32); // 创建Nz布局
  auto tensor = MakeTensor(l1Ptr, nzLayout);  // 创建张量
  auto coord = MakeCoord(5, 10);              // 访问元素 
  auto value = tensor[coord];
  tensor[coord] = 2.0f;
  // 获取张量信息
  auto size = tensor.Size();          // 1024
  auto capacity = tensor.Capacity();  // 1024
  auto shape = tensor.Shape();        // (32, 32)
  
  // 示例2：分块操作
  // 创建张量1
  auto bigTensor = MakeTensor(
      MakeMemPtr(data),
      MakeFrameLayout<NZLayoutPtn, half>(64, 64));
  // 创建张量2
  auto smallTensor = MakeTensor(
      MakeMemPtr(data),
      MakeFrameLayout<NZLayoutPtn, half>(32, 32));
  
  // 示例3：多维张量操作
  // 创建多维张量
  auto multiDimTensor = MakeTensor(
      MakeMemPtr<half>(data),
      MakeLayout(MakeShape(8, 8, 8)));
  // 访问多维元素
  auto value = multiDimTensor(2, 3, 4);
  // 使用坐标元组
  auto coord = MakeCoord(2, 3, 4);
  auto value2 = multiDimTensor(coord);
  
  // 示例4：GM内存张量
  auto gmTensor = MakeTensor(         // 创建GM内存张量
      MakeMemPtr<half>(gmAddr),     // 省略gmAddr初始化
      MakeFrameLayout<NDExtLayoutPtn>(128, 128));
  auto gmValue = gmTensor(10, 20);    // 访问GM数据
  
  ```
