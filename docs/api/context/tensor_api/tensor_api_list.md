# Tensor API 接口列表

本页面列出 Tensor API 的全部公开接口，按功能分类组织。每个接口的详细参数说明和约束条件请参考对应的 API 文档链接。

头文件：`#include "tensor_api/tensor.h"`

命名空间：`AscendC::Te`

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

---

## 1. 数据类型别名

| 类型别名 | 定义 | 说明 |
|:---|:---|:---|
| `Shape<Ts...>` | `Std::tuple<Ts...>` | 张量形状 |
| `Stride<Ts...>` | `Std::tuple<Ts...>` | 张量步长 |
| `Tile<Ts...>` | `Std::tuple<Ts...>` | 分块描述 |
| `Coord<Ts...>` | `Std::tuple<Ts...>` | 多维坐标 |

---

## 2. Layout 构造接口

| 接口 | 说明 | 参考文档 |
|:---|:---|:---|
| `MakeShape(ts...)` | 构造 Shape 元组 | [MakeShape](./data_structure/layout_structure/MakeShape.md) |
| `MakeStride(ts...)` | 构造 Stride 元组 | [MakeStride](./data_structure/layout_structure/MakeStride.md) |
| `MakeCoord(ts...)` | 构造 Coord 元组 | [MakeCoord](./data_structure/layout_structure/MakeCoord.md) |
| `MakeTile(ts...)` | 构造 Tile 元组 | — |
| `MakeLayout(shape, stride)` | 从 Shape 和 Stride 构造 Layout | [MakeLayout](./data_structure/layout_structure/MakeLayout.md) |
| `MakeLayout(shape)` | 从 Shape 构造 Layout，Stride 自动推导（行优先） | [MakeLayout](./data_structure/layout_structure/MakeLayout.md) |
| `MakeFrameLayout<Pattern>(row, col)` | 按指定 Pattern 构造标准分形 Layout | [MakeFrameLayout](./data_structure/layout_structure/MakeFrameLayout.md) |
| `MakeFrameLayout<Pattern, Trait>(row, col)` | 按指定 Pattern 和 Trait 构造 Layout | [MakeFrameLayout](./data_structure/layout_structure/MakeFrameLayout.md) |
| `MakePatternLayout<Pattern, Trait>(shape, stride)` | 从 Pattern、Trait、Shape、Stride 构造 Layout | [MakePatternLayout](./data_structure/layout_structure/MakePatternLayout.md) |

---

## 3. Layout 查询接口

| 接口 | 说明 | 参考文档 |
|:---|:---|:---|
| `layout.Shape<Is...>()` | 获取 Shape 或其子维度 | [GetShape](./data_structure/layout_structure/GetShape.md) |
| `layout.Stride<Is...>()` | 获取 Stride 或其子维度 | [GetStride](./data_structure/layout_structure/GetStride.md) |
| `layout.Size<Is...>()` | 获取元素总数 | [Size](./data_structure/layout_structure/Size.md) |
| `layout.Capacity<Is...>()` | 获取内存容量（含对齐填充） | [Capacity](./data_structure/layout_structure/Capacity.md) |
| `layout.Rank<Is...>()` | 获取维度数 | [Rank](./data_structure/layout_structure/Rank.md) |
| `layout.Get<Is...>()` | 获取 Layout 子元素 | [Get](./data_structure/layout_structure/Get.md) |
| `layout(coord)` | 坐标转线性偏移 | [Layout](./data_structure/layout_structure/Layout.md) |
| `GetShape<Is...>(layout)` | 自由函数形式获取 Shape | [GetShape](./data_structure/layout_structure/GetShape.md) |
| `GetStride<Is...>(layout)` | 自由函数形式获取 Stride | [GetStride](./data_structure/layout_structure/GetStride.md) |
| `Size<Is...>(layout)` | 自由函数形式获取 Size | [Size](./data_structure/layout_structure/Size.md) |
| `Capacity<Is...>(layout)` | 自由函数形式获取 Capacity | [Capacity](./data_structure/layout_structure/Capacity.md) |
| `Rank<Is...>(layout)` | 自由函数形式获取 Rank | [Rank](./data_structure/layout_structure/Rank.md) |
| `Cosize<Is...>(layout)` | 获取 Co-size | [Cosize](./data_structure/layout_structure/Cosize.md) |
| `Coshape<Is...>(layout)` | 获取 Co-shape | [Coshape](./data_structure/layout_structure/Coshape.md) |
| `Select<Is...>(layout)` | 选择子维度构成新 Layout | [Select](./data_structure/layout_structure/Select.md) |
| `Get<Is...>(layout)` | 获取子 Layout | [Get](./data_structure/layout_structure/Get.md) |
| `Crd2Idx(coord, shape, stride)` | 坐标转线性偏移 | — |

---

## 4. Layout Pattern 标签

| Pattern | 说明 | 适用场景 |
|:---|:---|:---|
| `NDLayoutPtn` | 行优先 | GM 普通矩阵 |
| `DNLayoutPtn` | 列优先 | GM 转置矩阵 |
| `NDExtLayoutPtn` | 行优先扩展（两层） | GM 矩阵（推荐） |
| `DNExtLayoutPtn` | 列优先扩展（两层） | GM 转置矩阵 |
| `NZLayoutPtn` | NZ 分形 | L0A、L0C |
| `ZNLayoutPtn` | ZN 分形 | L0B |
| `ZZLayoutPtn` | ZZ 分形 | ScaleA |
| `NNLayoutPtn` | NN 分形 | ScaleB（仅 fp8_e8m0_t） |
| `ScaleANDLayoutPtn` | ScaleA ND 形式 | MX 量化 ScaleA 输入 |
| `ScaleADNLayoutPtn` | ScaleA DN 形式 | MX 量化 ScaleA 输入 |
| `ScaleBNDLayoutPtn` | ScaleB ND 形式 | MX 量化 ScaleB 输入 |
| `ScaleBDNLayoutPtn` | ScaleB DN 形式 | MX 量化 ScaleB 输入 |

---

## 5. Layout Trait 类型

| 类型 | 说明 |
|:---|:---|
| `LayoutTrait<T, C0>` | Layout 特征，T 为数据类型，C0 为 C0 元素数 |
| `LayoutTraitDefault<T>` | 默认 Layout 特征，C0 自动推导为 32/sizeof(T) |
| `LayoutTraitFP4` | FP4 类型专用 Layout 特征 |
| `LayoutTraitScale` | Scale 数据专用 Layout 特征（fp8_e8m0_t） |

---

## 6. 内存指针接口

| 接口 | 说明 | 参考文档 |
|:---|:---|:---|
| `MakeMemPtr(ptr)` | 从硬件指针自动识别存储位置 | [MakeMemPtr](./data_structure/tensor_structure/MakeMemPtr.md) |
| `MakeMemPtr<Location>(addr)` | 显式指定存储位置构造指针 | [MakeMemPtr](./data_structure/tensor_structure/MakeMemPtr.md) |
| `MakeMemPtr<Location, DataType>(addr)` | 指定存储位置和数据类型构造指针 | [MakeMemPtr](./data_structure/tensor_structure/MakeMemPtr.md) |

---

## 7. 张量构造与操作接口

| 接口 | 说明 | 参考文档 |
|:---|:---|:---|
| `MakeTensor(ptr, layout)` | 从指针和 Layout 构造张量 | [MakeTensor](./data_structure/tensor_structure/MakeTensor.md) |
| `MakeTensor(ptr, shape, stride)` | 从指针、Shape、Stride 构造张量 | [MakeTensor](./data_structure/tensor_structure/MakeTensor.md) |
| `tensor[coord]` | 坐标索引访问元素 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor(coord)` | 获取子张量视图 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Slice(coord, info)` | 切片操作 | [Slice](./data_structure/tensor_structure/Slice.md) |
| `Slice(tensor, coord, info)` | 自由函数形式的切片 | [Slice](./data_structure/tensor_structure/Slice.md) |
| `tensor.Shape()` | 获取张量 Shape | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Stride()` | 获取张量 Stride | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Size()` | 获取张量元素总数 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Capacity()` | 获取张量内存容量 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Data()` | 获取底层数据指针 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Engine()` | 获取 Engine 对象 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `tensor.Layout()` | 获取 Layout 对象 | [Tensor](./data_structure/tensor_structure/Tensor.md) |
| `globalTensor.SetL2CacheHint(mode)` | 设置 L2 Cache 策略（仅 GlobalTensor） | [Tensor](./data_structure/tensor_structure/Tensor.md) |

---

## 8. 数据搬运接口（Copy）

### 8.1 Copy 通用接口

| 接口 | 说明 |
|:---|:---|
| `Copy(atomCopy, dst, src)` | 执行数据搬运 |
| `Copy(atomCopy, dst, src, quant)` | 执行带量化参数的数据搬运 |
| `MakeCopy(copyOperation)` | 使用默认 Trait 构造 CopyAtom |
| `MakeCopy(copyOperation, copyTrait)` | 使用指定 Trait 构造 CopyAtom |

### 8.2 Cube 单元搬运通路

| 通路 | CopyOperation | 默认 Trait | 源位置 | 目的位置 | 参考文档 |
|:---|:---|:---|:---:|:---:|:---|
| GM → L1 | `CopyGM2L1{}` | `CopyGM2L1TraitDefault{}` | GM | L1 | [gm_to_l1](./data_movement/gm_to_l1.md) |
| L1 → L0A | `CopyL12L0A{}` | `CopyL12L0ATraitDefault{}` | L1 | L0A | [l1_to_l0a](./data_movement/l1_to_l0a.md) |
| L1 → L0B | `CopyL12L0B{}` | `CopyL12L0BTraitDefault{}` | L1 | L0B | [l1_to_l0b](./data_movement/l1_to_l0b.md) |
| L1 → L0ScaleA | `CopyL12L0ScaleA{}` | `CopyL12L0ScaleATraitDefault{}` | L1 | L0ScaleA | [l1_to_l0scalea](./data_movement/l1_to_l0scalea.md) |
| L1 → L0ScaleB | `CopyL12L0ScaleB{}` | `CopyL12L0ScaleBTraitDefault{}` | L1 | L0ScaleB | [l1_to_l0scaleb](./data_movement/l1_to_l0scaleb.md) |
| L1 → BiasTable | `CopyL12BT{}` | `CopyL12BTTraitDefault{}` | L1 | BIAS | [l1_to_bias_table](./data_movement/l1_to_bias_table.md) |
| L1 → Fixpipe | `CopyL12FB{}` | `CopyL12FBTraitDefault{}` | L1 | FIXBUF | [l1_to_fixpipe](./data_movement/l1_to_fixpipe.md) |
| L0C → GM | `CopyL0C2GM{}` | `CopyL0C2GMTraitDefault{}` | L0C | GM | [l0c_to_gm](./data_movement/l0c_to_gm.md) |
| L0C → UB | `CopyL0C2UB{}` | `CopyL0C2UBTraitDefault{}` | L0C | UB | [l0c_to_ub](./data_movement/l0c_to_ub.md) |

### 8.3 Vector 单元搬运通路

| 通路 | CopyOperation | 默认 Trait | 源位置 | 目的位置 |
|:---|:---|:---|:---:|:---:|
| GM → UB | `CopyGM2UB{}` | `CopyGM2UBTraitDefault{}` | GM | UB |
| UB → GM | `CopyUB2GM{}` | `CopyUB2GMTraitDefault{}` | UB | GM |
| UB → L1 | `CopyUB2L1{}` | `CopyUB2L1TraitDefault{}` | UB | L1 |
| L1 → UB | `CopyL12UB{}` | `CopyL12UBTraitDefault{}` | L1 | UB |

---

## 9. 矩阵计算接口（Mmad）

| 接口 | 说明 | 参考文档 |
|:---|:---|:---|
| `Mmad(atomMmad, dst, fm, filter)` | 矩阵乘加：C = A×B 或 C += A×B | [mmad_computation](./mmad_computation/mmad_computation.md) |
| `Mmad(atomMmad, dst, fm, filter, bias)` | 带偏置的矩阵乘加：C = A×B + Bias | [mmad_computation](./mmad_computation/mmad_computation.md) |
| `MakeMmad(mmadOperation)` | 使用默认 Trait 构造 MmadAtom | [mmad_computation](./mmad_computation/mmad_computation.md) |
| `MakeMmad(mmadOperation, mmadTrait)` | 使用指定 Trait 构造 MmadAtom | [mmad_computation](./mmad_computation/mmad_computation.md) |

### Mmad 相关结构体

| 结构体 | 字段 | 说明 |
|:---|:---|:---|
| `MmadParams` | `m`, `n`, `k`, `unitFlag`, `cmatrixInitVal` | 矩阵乘法参数 |
| `MmadTrait` | `fmOffset`, `kDirectionAlign`, `cmatrixSource`, `disableGemv`, `mmadType` | 矩阵乘法特征 |
| `MmadTraitDefault` | — | 默认 MmadTrait |

### Mmad 操作类型

| 类型 | 说明 |
|:---|:---|
| `MmadOperation{}` | 标准矩阵乘法操作 |
| `MmadType::NORMAL` | 标准矩阵乘 |
| `MmadType::MX` | MX 格式矩阵乘 |

---

## 10. Vector 运算接口（Transform）

| 接口 | 说明 |
|:---|:---|
| `Transform<CalcFunc>(dst, src0, src1)` | 二元运算：dst = src0 op src1 |
| `Transform<CalcFunc>(dst, src)` | 一元运算：dst = op(src) |

### 支持的二元运算

| 运算 | 说明 |
|:---|:---|
| `Inst::Add` | 加法 |
| `Inst::Sub` | 减法 |
| `Inst::Mul` | 乘法 |
| `Inst::Div` | 除法 |
| `Inst::Madd` | 乘加 |
| `Inst::Min` | 取最小值 |
| `Inst::Max` | 取最大值 |
| `Inst::And` | 按位与 |
| `Inst::Or` | 按位或 |
| `Inst::Xor` | 按位异或 |
| `Inst::Select` | 选择 |
| `Inst::AbsSub` | 绝对差 |
| `Inst::ShiftLeft` | 左移 |
| `Inst::ShiftRight` | 右移 |
| `Inst::ExpSubEven` | 偶数指数差 |
| `Inst::ExpSubOdd` | 奇数指数差 |
| `Inst::Prelu` | PReLU 激活 |

### 支持的一元运算

| 运算 | 说明 |
|:---|:---|
| `Inst::Ceil` | 向上取整 |
| `Inst::U82U16` | uint8 转 uint16 |

---

## 11. Trait 与参数结构体

| 结构体 | 说明 | 适用通路 |
|:---|:---|:---|
| `CopyGM2L1Trait` | GM→L1 搬运特征 | CopyGM2L1 |
| `CopyL0C2GMTrait` | L0C→GM 搬运特征（含量化/Relu/Channel Split） | CopyL0C2GM |
| `CopyL0C2UBTrait` | L0C→UB 搬运特征（含 DualDst 模式） | CopyL0C2UB |
| `FixpipeParams` | Fixpipe 参数（`unitFlag`, `subBlockId`） | CopyL0C2GM, CopyL0C2UB |
| `FixpipeTrait` | Fixpipe 特征（`roundMode`, `enableRelu`, `enableChannelSplit`, `dualDstCtl`） | CopyL0C2GM, CopyL0C2UB |
| `LoadDataTrait` | 数据加载特征（`transposed`） | CopyL12L0A, CopyL12L0B |
| `DataCopyTrait` | 通用数据搬运特征 | 所有 Copy 通路 |

---

## 12. 枚举类型

| 枚举 | 值 | 说明 |
|:---|:---|:---|
| `CacheMode::CACHE_MODE_NORMAL` | 0 | 正常 L2 Cache |
| `CacheMode::CACHE_MODE_DISABLE` | 4 | 禁用 L2 Cache |
| `CacheMode::CACHE_MODE_LAST` | — | 最后使用标记 |
| `CacheMode::CACHE_MODE_PERSISTENT` | — | 持久缓存 |
| `RoundMode::DEFAULT` | 0 | 默认舍入 |
| `RoundMode::HYBRID` | — | 混合舍入 |
| `MmadType::NORMAL` | 0 | 标准矩阵乘 |
| `MmadType::MX` | 1 | MX 格式矩阵乘 |
| `DUAL_DST_DISABLE` | 0 | 禁用双目标 |
| `DUAL_DST_SPLIT_M` | — | 按 M 维度拆分双目标 |
| `DUAL_DST_SPLIT_N` | — | 按 N 维度拆分双目标 |

---

## 13. 存储位置标签

| 标签 | 对应限定符 | 说明 |
|:---|:---:|:---|
| `Location::GM` | `__gm__` | 全局内存 |
| `Location::L1` | `__cbuf__` | L1 Buffer |
| `Location::L0A` | `__ca__` | Cube A 矩阵寄存器 |
| `Location::L0B` | `__cb__` | Cube B 矩阵寄存器 |
| `Location::L0C` | `__cc__` | Cube 累加器 |
| `Location::UB` | `__ubuf__` | Vector 统一缓冲区 |
| `Location::BIAS` | `__biasbuf__` | Bias 表 |
| `Location::FIXBUF` | `__fbuf__` | Fixpipe 缓冲区 |
| `Location::SSBUF` | `__ssbuf__` | SS Buffer |

---

## 14. 类型判断工具

| 工具 | 说明 | 参考文档 |
|:---|:---|:---|
| `IsLayoutV<T>` | 判断 T 是否为 Layout 类型 | [IsLayoutV](./data_structure/utils/IsLayoutV.md) |
| `IsAttrTensorV<T>` | 判断 T 是否为 Tensor API 张量类型 | [IsAttrTensorV](./data_structure/utils/IsAttrTensorV.md) |
| `IsHardwareV<T>` | 判断 T 是否为硬件存储位置标签 | [IsHardwareV](./data_structure/utils/IsHardwareV.md) |
| `IsIntegralConstantV<T>` | 判断 T 是否为编译期整型常量 | [IsIntegralConstantV](./data_structure/utils/IsIntegralConstantV.md) |
| `GetLayoutPattern<T>` | 获取 Layout 的 Pattern 类型 | [GetLayoutPattern](./data_structure/utils/GetLayoutPattern.md) |
| `GetLayoutTrait<T>` | 获取 Layout 的 Trait 类型 | [GetLayoutTrait](./data_structure/utils/GetLayoutTrait.md) |

---

## 15. 编译期常量

| 常量 | 值 | 说明 |
|:---|:---:|:---|
| `_0` | `Std::Int<0>` | 编译期 0 |
| `_1` | `Std::Int<1>` | 编译期 1 |
| `_2` ~ `_10` | `Std::Int<2>` ~ `Std::Int<10>` | 编译期 2-10 |
| `_16` | `Std::Int<16>` | 编译期 16 |
| `_32` | `Std::Int<32>` | 编译期 32 |
| `_64` | `Std::Int<64>` | 编译期 64 |
| `_128` | `Std::Int<128>` | 编译期 128 |
| `_256` | `Std::Int<256>` | 编译期 256 |
| `_512` | `Std::Int<512>` | 编译期 512 |
| `_1024` | `Std::Int<1024>` | 编译期 1024 |
| `_2048` | `Std::Int<2048>` | 编译期 2048 |
| `_4096` | `Std::Int<4096>` | 编译期 4096 |

---

## 参考资源

- [Tensor API 教程](./tensor_api_tutorial.md)
- [Tensor API 概览](./README.md)
- [Layout 和层次化表述法](./data_structure/layout_structure/Layout和层次化表述法.md)
- [测试用例](../../../../tests/api/tensor_api)
- [示例代码](../../../../examples/01_simd_cpp_api/02_features/05_tensor_api)
