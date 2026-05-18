# README

Tensor API 提供面向张量描述的数据搬运和矩阵计算接口。接口以 `AscendC::Te` 命名空间为入口，通过 `MakeMemPtr`、`MakeFrameLayout`、`MakeTensor` 构造带有存储位置、数据类型和数据排布信息的张量，再通过 `Copy` 或 `Mmad` 完成硬件指令封装调用。

## 使用入口

头文件为：

```cpp
#include "tensor_api/tensor.h"
```

Tensor API 当前主要包含以下能力：

- 数据搬运：使用 `Copy` 或 `MakeCopy` 构造的 `CopyAtom`，在 GM、L1、L0A、L0B、L0C、UB、BiasTable Buffer、Fixpipe Buffer 之间搬运数据。
- 矩阵计算：使用 `Mmad` 或 `MakeMmad` 构造的 `MmadAtom`，完成 L0A 与 L0B 输入、L0C 输出的矩阵乘加计算。
- 张量基础结构：使用 `MakeMemPtr`、`MakeFrameLayout`、`MakeLayout`、`MakeTensor`、`Slice` 等接口描述内存位置、数据排布、形状、步长和切片。

## 文档索 引

- [tensor_api_list](tensor_api_list.md)
- [数据搬运导览](数据搬运/数据搬运导览.md)
- [GM到L1数据搬运](数据搬运/GM到L1数据搬运.md)
- [L1到L0A数据搬运](数据搬运/L1到L0A数据搬运.md)
- [L1到L0B数据搬运](数据搬运/L1到L0B数据搬运.md)
- [L1到BiasTable Buffer数据搬运](数据搬运/L1到BiasTable%20Buffer数据搬运.md)
- [L1到Fixpipe Buffer数据搬运](数据搬运/L1到Fixpipe%20Buffer数据搬运.md)
- [L1到UB数据搬运](数据搬运/L1到UB数据搬运.md)
- [L0C到GM数据搬运](数据搬运/L0C到GM数据搬运.md)
- [L0C到UB数据搬运](数据搬运/L0C到UB数据搬运.md)
- [矩阵计算流程](矩阵计算/矩阵计算流程.md)
- [普通矩阵计算](矩阵计算/普通矩阵计算.md)
- [MX矩阵计算](矩阵计算/MX矩阵计算.md)
