# Tensor API

Tensor API文档目录，整体使用时可以引入tensor.h，Tensor API列表如下：

## 数据结构

| 结构名 | 说明 |
| --------- | ---------------- |
| [Shape](struct/Shape.md) | 定义Shape |
| [Stride](struct/Stride.md) | 定义Stride |
| [Coord](struct/coord/Coord.md) | 定义Coord |
| [Layout](struct/layout/Layout简介.md) | 定义Layout |
| [LocalTensor](struct/tensor/LocalTensor.md) | 定义LocalTensor |
| [Pointer](struct/pointer/Pointer.md) | 定义迭代器 |
| [Tile](struct/tile/Tile.md) | 张量分块 |

## 数据接口

基础结构类API，可以高效地管理内存，进行复杂的张量操作。此类API列表如下：

| API名称 | 说明 |
| ---------- | ----------- |
| [MakeShape](struct/layout/MakeShape.md) | 构造Shape |
| [MakeStride](struct/layout/MakeStride.md) | 构造Stride |
| [GetShape](struct/layout/GetShape.md) | 获取Shape |
| [GetStride](struct/layout/GetStride.md) | 获取Stride |
| [MakeCoord](struct/coord/MakeCoord.md) | 构造Coord |
| [MakeTile](struct/tile/MakeTile.md) | 构造Tile |
| [MakeLayout](struct/layout/layout_fractal/MakeLayout.md) | 构造Layout |
| [MakeDNLayout](struct/layout/layout_fractal/MakeDNLayout.md) | 构造DN Layout |
| [MakeL0CLayout](struct/layout/layout_fractal/MakeL0CLayout.md) | 构造L0C Layout |
| [MakeNDLayout](struct/layout/layout_fractal/MakeNDLayout.md) | 构造ND Layout |
| [MakeNnLayout](struct/layout/layout_fractal/MakeNnLayout.md) | 构造Nn Layout |
| [MakeNzLayout](struct/layout/layout_fractal/MakeNzLayout.md) | 构造Nz Layout |
| [MakeScaleADNLayout](struct/layout/layout_fractal/MakeScaleADNLayout.md) | 构造ScaleADN Layout |
| [MakeScaleANDLayout](struct/layout/layout_fractal/MakeScaleANDLayout.md) | 构造ScaleAND Layout |
| [MakeScaleBDNLayout](struct/layout/layout_fractal/MakeScaleBDNLayout.md) | 构造ScaleBDN Layout |
| [MakeScaleBNDLayout](struct/layout/layout_fractal/MakeScaleBNDLayout.md) | 构造ScaleBND Layout |
| [MakeZnLayout](struct/layout/layout_fractal/MakeZnLayout.md) | 构造Zn Layout |
| [MakeZzLayout](struct/layout/layout_fractal/MakeZzLayout.md) | 构造Zz Layout |
| [is_layout](struct/layout/is_layout.md) | 判断Layout |
| [Get](struct/layout/Get.md) | 构造子Layout |
| [Select](struct/layout/Select.md) | 构造子Layout |
| [GetShape](struct/layout/GetShape.md) | 返回Shape |
| [GetLayout](struct/tensor/GetLayout.md)  | 返回Layout对象 |
| [GetStride](struct/layout/GetStride.md)  | 返回Stride |
| [Size](struct/layout/Size.md) | 返回Shape的总大小 |
| [Rank](struct/layout/Rank.md) | 返回Layout中的秩 |
| [Coshape](struct/layout/Coshape.md) | 返回实际Shape空间 |
| [Cosize](struct/layout/Cosize.md) | 返回实际占用的内存 |
| [Crd2Idx](struct/tensor/Crd2Idx.md) | Coordinate转Index |
| [MakeTensor](struct/tensor/MakeTensor.md) | 构造LocalTensor |
| [MakeBiasmemPtr](struct/pointer/MakeBiasmemPtr.md) | 构造BiasTable Buffer上的Pointer迭代器 |
| [MakeFixbufmemPtr](struct/pointer/MakeFixbufmemPtr.md) | 构造Fixpipe Buffer上的Pointer迭代器 |
| [MakeGMmemPtr](struct/pointer/MakeGMmemPtr.md) | 构造GM上的Pointer迭代器 |
| [MakeL0AmemPtr](struct/pointer/MakeL0AmemPtr.md) | 构造L0A上的Pointer迭代器 |
| [MakeL0BmemPtr](struct/pointer/MakeL0BmemPtr.md) | 构造L0B上的Pointer迭代器 |
| [MakeL0CmemPtr](struct/pointer/MakeL0CmemPtr.md) | 构造L0C上的Pointer迭代器 |
| [MakeL1memPtr](struct/pointer/MakeL1memPtr.md) | 构造L1上的Pointer迭代器 |
| [MakeUBmemPtr](struct/pointer/MakeUBmemPtr.md) | 构造UB上的Pointer迭代器 |

## 数据搬运

数据搬运类API，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [DataCopyGM2L1](data_move/DataCopyGM2L1.md) | CUBE计算，支持GM2L1搬运处理 |
| [DataCopyFromL1](data_move/DataCopyFromL1.md) | CUBE计算，支持L12BT/L12FB搬运处理 |
| [FixPipe](data_move/Fixpipe.md) | CUBE计算，支持L0C2GM/L0C2UB搬运处理 |
| [LoadData](data_move/LoadData.md) | CUBE计算，支持L12L0搬运处理 |

## 矩阵计算

矩阵计算类API，此类API列表如下：

|   API名称   |   说明   |
|----------|-----------|
| [Mmad](cube_compute/Mmad.md) | 完成矩阵乘加操作。 |
