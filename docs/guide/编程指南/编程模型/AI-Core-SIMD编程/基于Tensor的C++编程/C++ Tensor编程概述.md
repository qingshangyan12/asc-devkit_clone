# C++ Tensor编程概述
## 引言
Ascend C C++ Tensor编程是面向LocalTensor与GlobalTensor的编程范式。核心设计理念是将内存地址抽象为Tensor对象，开发者通过Tensor进行数据搬运、计算操作、同步管理。相比C指针编程的直接地址操作，Tensor编程隐藏内存地址细节，提供更高级的抽象层次，降低开发复杂度。Ascend C提供基础能力接口，支持LocalTensor/GlobalTensor定义、数据搬运、向量计算、矩阵乘、同步等核心功能。在此基础上，对LocalTensor进行扩展，引入Layout概念描述数据布局，实现类型安全、编译期确定的张量操作，并提供Copy/Mmad Atom、零开销切片等扩展能力。本章旨在帮助开发者建立对Ascend C编程范式的整体认知。全文遵循「设计理念—Tensor抽象—基础能力—扩展能力—接口选择」的递进逻辑展开。

## 设计理念：面向Tensor的编程抽象
### Tensor编程的核心思想
Tensor编程的核心思想是将原始内存地址抽象为Tensor对象，实现从"地址操作"到"对象管理"的编程层次提升。Tensor体系分为两类：LocalTensor对应片上内存体系（包括UB、L1、L0A、L0B、L0C等多级缓冲区），承载计算过程中的输入、输出、中间数据；GlobalTensor对应全局内存（GM），作为算子与外部数据交互的门户。基础能力的Tensor对象携带数据类型和内存位置两类属性，接口调用需手动传递操作参数。扩展能力的Tensor在此基础上引入Layout概念，携带Shape和Stride信息，成为"自描述数据容器"。接口调用时编译器自动推断搬运长度、计算元素数等参数，实现"一次定义、自动推断"的便捷开发体验。
```cpp
// ===== 基础能力的Tensor =====
// 定义：携带数据类型 + 内存位置，需手动传参
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
LocalTensor<half> ubTensor = ubAllocator.Alloc<half, 256>();
GlobalTensor<half> gmTensor;
gmTensor.SetGlobalBuffer((__gm__ half*)gmBuffer, bufferSize);
// 操作：需手动传递size/count参数
DataCopy(ubTensor, gmTensor, 256);
Add(dstTensor, src0Tensor, src1Tensor, 256);

// ===== 扩展能力的Tensor (Tensor API) =====
// 定义：通过MakeTensor创建，携带Shape和Stride
auto gmTensor = MakeTensor(gmPointer, MakeShape(128, 64));
auto ubTensor = MakeTensor(ubPointer, MakeShape(128, 64));
// 操作：无需手动设置size/count，Copy接口根据Tensor信息自动推导
Copy(ubPointer, gmPointer);
```

### Tensor编程的优势
- **降低开发复杂度**：隐藏内存地址细节，Tensor携带数据类型和形状信息，接口自动推断操作参数。
- **提升代码可读性**：Tensor命名表达数据语义，接口调用直观，参数通过Tensor对象传递。
- **扩展能力优化**：Layout编译期确定实现类型安全，Slice/Tile零开销切片，Atom设计简化参数配置。

## 接口体系：基础能力与扩展能力
Ascend C Tensor编程分为基础能力与扩展能力两个层次：
- **基础能力接口**：提供Tensor定义、数据搬运、向量计算、矩阵乘、同步机制等核心功能。参数通过结构体传递，内存位置用Hardware枚举表达，大小运行期设置，同步显式控制。
- **扩展能力接口**：在原有Tensor定义上引入Layout抽象实现编译期类型安全，Copy/Mmad Atom简化参数配置，Slice/Tile零开销切片，Operation驱动隐式同步减少手动调用。

## 基础能力：Tensor定义与核心接口
### Tensor定义
**LocalTensor定义**：对应片上内存（UB、L1、L0A等），通过LocalMemAllocator方式指定物理位置申请。
```cpp
// LocalTensor定义示例
constexpr uint32_t TILE_LENGTH = 256;  // 示意性参数
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
AscendC::LocalTensor<float> xLocalPing = ubAllocator.Alloc<float, TILE_LENGTH>();
```
**GlobalTensor定义**：对应全局内存（GM），通过SetGlobalBuffer设置地址。
```cpp
GlobalTensor<half> gmTensor;
gmTensor.SetGlobalBuffer((__gm__ half*)gmBuffer, bufferSize);
```

### 数据搬运与计算示例
```cpp
constexpr uint32_t TILE_LENGTH = 256;  // 示意性参数
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, TILE_LENGTH>();
AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, TILE_LENGTH>();
AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, TILE_LENGTH>();
// ===== 向量加法完整流程 =====
// 1. 数据搬运：GM→UB
DataCopy(xLocal, gmIn0, TILE_LENGTH);
DataCopy(yLocal, gmIn1, TILE_LENGTH);
// 2. 向量计算：Add
Add(zLocal, xLocal, yLocal, TILE_LENGTH);
// 3. 结果输出：UB→GM
DataCopy(gmOut, zLocal, TILE_LENGTH);
```

## 扩展能力：Layout抽象与Tensor定义
### Layout表述
我们使用Shape和Stride来表达Layout排布格式，比如下图中的行优先和列优先排布：
Shape用于表达Tensor形状，Stride则用于区分不同的排布：
- 行优先:Shape (2, 4) Stride (4, 1)
- 列优先:Shape (2, 4) Stride (1, 2)
图中每个方格中的数字表示该位置元素在内存中按顺序排列时的下标。对于相同的矩阵位置，排布方式不同时，其在内存中的顺序可能是不同的，例如，矩阵坐标 (1, 0) 在行优先和列优先的情况下，对应元素在内存中的顺序分别是4和1。

![行优先排布](行优先排布.png)
![列优先排布](列优先排布.png)

### Layout层次化表达
通常Shape或者Stride中的元素是一个单独的整数（如上文的示例），但是遇到一些复杂的内存排布情况可能无法表达，所以采用层次化表述法，该方式中，元素也可以是一个元组。元组的第一个元素用于描述不同层次中矩阵的行数，元组的第二个元素用于描述不同层次中矩阵的列数。
例如在下图中，Layout的Shape和Stride分别为：
- Shape((2, 3), (2, 4))
- Stride((1, 4), (2, 12))

![层次化Layout表达](层次化Layout表达.png)

图中展示了两层矩阵：内层矩阵为内部用灰色线包裹的矩阵，外层矩阵为将内层矩阵视为一个元素时黑色线包裹的矩阵。
Shape的第一个元素描述行方向的形状，(2，3)表示内层矩阵和外层矩阵的行数分别为2和3；Shape的第二个元素描述的是列方向上的形状，(2，4)表示内层矩阵和外层矩阵的列数分别为2和4。
Stride中的每个元素与Shape中的元素对应，表示该对应维度下，相邻元素首地址在内存地址上的间隔，图片中用箭头表示了每个维度相邻元素的首地址间隔。

### Tensor定义方式
扩展能力的Tensor是通过对基础Tensor的特例化扩展实现，它通过MakeTensor函数创建，Shape和Stride可以在编译期确定。针对不同的应用场景提供多种定义方式：
- **1. 定义连续的内存块的Tensor**
```cpp
// 1.1 定义动态Shape Tensor
auto dynTensor = MakeTensor(pointer, MakeShape(m, n));
// 1.2 定义静态Shape Tensor
auto staticTensor = MakeTensor(pointer, MakeShape(Std::Int<128>{}, Std::Int<64>{}));
```

- **2. 指定Stride定义任意排布的Tensor**
```cpp
// 2.1 行优先排布的Tensor定义
auto rowMajorTensor = MakeTensor(pointer, MakeLayout(MakeShape(m, n), MakeStride(n, Std::Int<1>{})));
// 2.2 列优先排布的Tensor定义
auto colMajorTensor = MakeTensor(pointer, MakeLayout(MakeShape(m, n), MakeStride(Std::Int<1>{}, n)));
```

- **3. 通过层次化布局形式定义矩阵分形Tensor**
```cpp
// 3.1 针对(128, 64)的矩阵，内部为16 * 16的分形，可以通过层次化布局来定义对应的4维Tensor
auto nzTensor = MakeTensor(pointer, MakeLayout(MakeShape((8, 16), (4, 16))));
```

- **4：通过特定的分形布局进行简化的定义** - Cube矩阵乘专用，自动生成Shape和Stride
```cpp
// 4.1 通过MakeFrameLayout，指定NZ Pattern直接指定NZ分形, 接口内部自动推导成4维分形
auto tensor = MakeTensor(l0aPointer, MakeFrameLayout<NZLayoutPtn>(128, 64));
```

### Tensor使用方式
扩展能力Tensor引入Layout抽象成为"自描述数据容器"，实现编译期自动推导操作参数、类型安全校验和零开销切片，开发者只需定义Tensor即可自动推断后续搬运、计算参数，无需手动传递。

- **1. 对元素进行访问, 自动索引计算**
```cpp
auto tensor = MakeTensor(pointer, MakeShape(128, 64));
auto element = tensor[MakeCoord(32, 16)];  // 访问第32行第16列
auto element = tensor(32, 16);              // 简化写法
```

- **2. 进行切片操作**
```cpp
auto tensor = MakeTensor(pointer, MakeShape(1024, 512));
// 切片：从(0,0)位置取128×64块
auto slice = tensor.Slice(MakeCoord(0, 0), MakeShape(128, 64));
// 动态切片：分块处理场景
auto slice = tensor.Slice(MakeCoord(i * 64, j * 32), MakeShape(64, 32));
```

- **3. 提供简化的搬运接口，无需复杂的参数计算**
```cpp
auto gmTensor = MakeTensor(gmPtr, MakeShape(128, 64));
auto ubTensor = MakeTensor(ubPtr, MakeShape(128, 64));

// 3.1 搬运：利用系统内置搬运直接搬运，无需设置Size等
Copy(gmTensor, ubTensor);

// 3.2 搬运: 通过自定义搬运Op, 进行定制化的搬运处理
auto copyAtom = MakeCopy<CopyGM2UB>();
Copy(copyAtom, gmTensor, ubTensor);
```

- **4 CUBE计算: 提供了简化的Mmad接口，方便用户直接进行矩阵乘计算**
```cpp
Mmad(l0c, l0a, l0b, {m, n, k, false, iterK == 0});
```

## 接口选择建议
基础能力适用于运行期灵活性需求、细粒度同步控制、快速原型开发场景；扩展能力适用于编译期类型安全、多种布局格式、切片分块操作、Cube算子开发场景，复杂同步场景可配合基础能力同步接口使用。

## 小结
本章阐述了Ascend C编程范式的整体体系：将内存地址抽象为Tensor对象，隐藏地址细节。基础能力接口提供Tensor定义、数据搬运、计算、同步等核心功能。扩展能力引入Layout概念，通过Shape和Stride编译期确定实现类型安全，支持平铺布局、层次化布局、分形布局等多种布局模式。开发者可根据场景选择基础能力或扩展能力，复杂场景可混合使用。
后续章节将逐层深入、循序渐进地详解内存层级管理、数据搬运与计算接口、同步机制、编程示例，帮助开发者熟练掌握Ascend C Tensor编程的核心能力。