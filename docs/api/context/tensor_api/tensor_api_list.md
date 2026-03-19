# Tensor API

Tensor API文档目录，整体使用时可以引入"tensor.h":，Tensor API列表如下：

## 数据结构

| 结构名 | 说明 |
| ----------------------- | ----------------------- |
| [Shape](struct/definition/Shape.md) | 定义张量的形状 |
| [Stride](struct/definition/Stride.md) | 定义张量的步长 |
| [Coord](struct/definition/Coord.md) | 定义张量的坐标 |
| [Tile](struct/definition/Tile.md) | 定义张量的分块 |
| [Layout](struct/definition/Layout.md) | 定义张量的布局 |
| [Tensor](struct/definition/Tensor.md) | 定义张量 |
| [Pointer](struct/definition/Pointer.md) | 定义指针迭代器 |
| [NzLayoutFormat](struct/definition/NzLayoutFormat.md) | NZ布局格式 |
| [ZnLayoutFormat](struct/definition/ZnLayoutFormat.md) | Zn布局格式 |
| [L0CLayoutFormat](struct/definition/L0CLayoutFormat.md) | L0C布局格式 |
| [NDLayoutFormat](struct/definition/NDLayoutFormat.md) | ND布局格式 |
| [DNLayoutFormat](struct/definition/DNLayoutFormat.md) | DN布局格式 |
| [ZzLayoutFormat](struct/definition/ZzLayoutFormat.md) | Zz布局格式 |
| [NnLayoutFormat](struct/definition/NnLayoutFormat.md) | Nn布局格式 |
| [ScaleANDLayoutFormat](struct/definition/ScaleANDLayoutFormat.md) | ScaleAND布局格式 |
| [ScaleADNLayoutFormat](struct/definition/ScaleADNLayoutFormat.md) | ScaleADN布局格式 |
| [ScaleBNDLayoutFormat](struct/definition/ScaleBNDLayoutFormat.md) | ScaleBND布局格式 |
| [ScaleBDNLayoutFormat](struct/definition/ScaleBDNLayoutFormat.md) | ScaleBDN布局格式 |
| [CopyTraits](struct/definition/CopyTraits.md) | 复制操作特性 |
| [CopyAtom](struct/definition/CopyAtom.md) | 复制原子操作 |
| [MmadTraits](struct/definition/MmadTraits.md) | 矩阵乘加操作特性 |
| [MmadAtom](struct/definition/MmadAtom.md) | 矩阵乘加原子操作 |
| [Hardware](struct/definition/Hardware.md) | 硬件位置枚举 |

## 数据接口

基础结构类API，可以高效地管理内存，进行复杂的张量操作。此类API列表如下：

| API名称 | 说明 |
| ---------- | ----------- |
| [MakeShape](struct/constructor/MakeShape.md) | 构造Shape |
| [MakeStride](struct/constructor/MakeStride.md) | 构造Stride |
| [MakeTile](struct/constructor/MakeTile.md) | 构造Tile |
| [MakeCoord](struct/constructor/MakeCoord.md) | 构造Coord |
| [MakeLayout](struct/constructor/MakeLayout.md) | 构造Layout |
| [MakeNzLayout](struct/constructor/MakeNzLayout.md) | 构造NZ格式Layout |
| [MakeL0CLayout](struct/constructor/MakeL0CLayout.md) | 构造L0C格式Layout |
| [MakeDNLayout](struct/constructor/MakeDNLayout.md) | 构造DN格式Layout |
| [MakeNDLayout](struct/constructor/MakeNDLayout.md) | 构造ND格式Layout |
| [MakeZnLayout](struct/constructor/MakeZnLayout.md) | 构造Zn格式Layout |
| [MakeZzLayout](struct/constructor/MakeZzLayout.md) | 构造Zz格式Layout |
| [MakeNnLayout](struct/constructor/MakeNnLayout.md) | 构造Nn格式Layout |
| [MakeScaleANDLayout](struct/constructor/MakeScaleANDLayout.md) | 构造ScaleAND格式Layout |
| [MakeScaleADNLayout](struct/constructor/MakeScaleADNLayout.md) | 构造ScaleADN格式Layout |
| [MakeScaleBNDLayout](struct/constructor/MakeScaleBNDLayout.md) | 构造ScaleBND格式Layout |
| [MakeScaleBDNLayout](struct/constructor/MakeScaleBDNLayout.md) | 构造ScaleBDN格式Layout |
| [MakeTileLayout](struct/constructor/MakeTileLayout.md) | 构造分块Layout |
| [Rank](struct/constructor/Rank.md) | 返回Layout的秩 |
| [GetShape](struct/constructor/GetShape.md) | 返回Shape |
| [GetStride](struct/constructor/GetStride.md) | 返回Stride |
| [Select](struct/constructor/Select.md) | 选择Layout的维度 |
| [Get](struct/constructor/Get.md) | 获取Layout元素 |
| [Size](struct/constructor/Size.md) | 返回Shape的总大小 |
| [Capacity](struct/constructor/Capacity.md) | 返回Layout的容量 |
| [Coshape](struct/constructor/Coshape.md) | 返回实际Shape空间 |
| [Cosize](struct/constructor/Cosize.md) | 返回实际占用的内存 |
| [Crd2Idx](struct/constructor/Crd2Idx.md) | Coordinate转Index |
| [MakeTensor](struct/constructor/MakeTensor.md) | 构造Tensor |
| [MakeGMmemPtr](struct/constructor/MakeGMmemPtr.md) | 构造Global Memory指针 |
| [MakeUBmemPtr](struct/constructor/MakeUBmemPtr.md) | 构造Unified Buffer指针 |
| [MakeL1memPtr](struct/constructor/MakeL1memPtr.md) | 构造L1 Buffer指针 |
| [MakeL0AmemPtr](struct/constructor/MakeL0AmemPtr.md) | 构造L0A Buffer指针 |
| [MakeL0BmemPtr](struct/constructor/MakeL0BmemPtr.md) | 构造L0B Buffer指针 |
| [MakeL0CmemPtr](struct/constructor/MakeL0CmemPtr.md) | 构造L0C Buffer指针 |
| [MakeBiasmemPtr](struct/constructor/MakeBiasmemPtr.md) | 构造Bias Table Buffer指针 |
| [MakeFixbufmemPtr](struct/constructor/MakeFixbufmemPtr.md) | 构造Fixpipe Buffer指针 |

## 数据搬运

数据搬运类API，单独使用时可以引入tensor_api/algorithm/copy.h，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [Copy](algorithm/Copy.md) | 执行数据复制操作 |

## 矩阵计算

矩阵计算类API，单独使用时可以引入tensor_api/algorithm/mad.h，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [Mad](algorithm/Mad.md) | 完成矩阵乘加操作 |

## 架构接口

架构相关类API，单独使用时可以引入tensor_api/arch/arch.h，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [LoadData](arch/LoadData.md) | L1 Buffer到L0A/L0B Buffer的数据加载 |
| [Fixpipe](arch/Fixpipe.md) | 固定管道数据传输 |