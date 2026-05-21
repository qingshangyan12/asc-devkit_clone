# MakeMemPtr

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 头文件/库文件

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

## 功能说明

`MakeMemPtr` 用于构造带物理存储位置标记和数据类型信息的**内存指针适配器**（`HardwareMemPtr`），它是 Tensor API 三层抽象（Ptr → Layout → Tensor）中的第一层。`MakeMemPtr` 生成的指针对象被 `MakeTensor` 消费，编译器根据指针上的位置标记（`PtrPattern`）在 `Copy` 或 `Mmad` 操作中自动选择对应的硬件搬运/计算指令。

`MakeMemPtr` 提供了三种调用形式：

1. **从原始地址构造**：指定物理位置和数据类型，传入基地址偏移量。
2. **从已有迭代器构造（指定位置）**：接收已有的硬件地址迭代器，强制指定新的物理位置标记。
3. **从已有迭代器构造（自动推导）**：从迭代器的元素类型中自动推导物理位置标记。

## 支持的物理位置

| Location | 说明 | 用途 |
| :--- | :--- | :--- |
| `Location::GM` | Global Memory | 全局内存，数据收发的大本营 |
| `Location::L1` | L1 Buffer | 一级缓存，矩阵操作的数据暂存区 |
| `Location::L0A` | L0A Buffer | 矩阵计算左矩阵输入缓存 |
| `Location::L0B` | L0B Buffer | 矩阵计算右矩阵输入缓存 |
| `Location::L0ScaleA` | L0 ScaleA Buffer | MX 矩阵计算 ScaleA 缓存 |
| `Location::L0ScaleB` | L0 ScaleB Buffer | MX 矩阵计算 ScaleB 缓存 |
| `Location::L0C` | L0C Buffer | 矩阵计算结果缓存 |
| `Location::UB` | Unified Buffer | 统一缓存，矢量计算的数据存储区 |
| `Location::BIAS` | BiasTable Buffer | 偏置表缓存，带偏置矩阵计算的偏置存放区 |
| `Location::FIXBUF` | Fixpipe Buffer | Fixpipe 输出缓存，L0C → GM/UB 的中转区 |

## 函数原型

- 指定物理位置和数据类型，从原始地址构造。

    ```cpp
    template <typename Hardware, typename DataType, typename Addr,
        EnableMakePtrByTrait<Hardware, Addr> Enable>
    __aicore__ inline constexpr auto MakeMemPtr(Addr arg)
    ```

- 指定物理位置，从已有迭代器构造。

    ```cpp
    template <typename Hardware, typename Iterator,
        EnableMakeHardwarePtr<Hardware, Iterator> Enable>
    __aicore__ inline constexpr auto MakeMemPtr(Iterator iterator)
    ```

- 从已有迭代器中自动推导物理位置。

    ```cpp
    template <typename Iterator, EnableMakePtrByIter<Iterator> Enable>
    __aicore__ inline constexpr auto MakeMemPtr(Iterator iterator)
    ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `Hardware` | 输入 | 物理存储位置。|
| `DataType` | 输入 | 数据类型。 |
| `Addr` | 输入 | 原始地址偏移量（通常为字节偏移），从该地址构造指向对应物理位置的指针适配器。 |
| `Iterator`| 输入 | 已有的硬件地址迭代器或指针适配器。|

## 返回值说明

返回 `HardwareMemPtr<PtrPattern, Pointer>` 类型的指针适配器对象，其中：

- `PtrPattern` 为指定的 `Hardware` 物理位置；
- `Pointer` 为对应的硬件地址空间指针类型（如 `__gm__ float*`、`__cbuf__ half*` 等）。

## 调用示例

```cpp
using namespace AscendC::Te;

// 1. 从原始偏移地址构造：指定物理位置和数据类型
auto gmPtr = MakeMemPtr<Location::GM, float>(gmBuf);
auto l1Ptr = MakeMemPtr<Location::L1, float>(l1Buf);
auto l0aPtr = MakeMemPtr<Location::L0A, half>(l0aBuf);
auto ubPtr = MakeMemPtr<Location::UB, bfloat16_t>(ubBuf);

// 2. 配合 MakeTensor 构造张量
auto gmA = MakeTensor(gmPtr, MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n));
auto l1A = MakeTensor(l1Ptr, MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float>>(m, n));

// 3. 从已有指针适配器继承位置（自动推导）
auto anotherPtr = MakeMemPtr(gmPtr);

// 4. 从已有迭代器指定新位置
auto l1FromIter = MakeMemPtr<Location::L1>(someIterator);
```
