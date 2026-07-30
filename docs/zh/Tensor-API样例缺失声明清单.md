# Tensor API样例缺失声明清单

## 核查范围和判定规则

本次核查以下目录中的全部`.asc`文件：

- `examples/01_simd_cpp_api/03_basic_api/00_data_movement`
- `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute`

两个目录下共有35个`.asc`文件，其中直接使用`AscendC::Te`Tensor API
的文件有4个：

**表 1**  直接使用Tensor API的样例文件

| 编号 | `.asc`文件 |
| --- | --- |
| A | `examples/01_simd_cpp_api/03_basic_api/00_data_movement/copy_in_tensor_api/copy_in_tensor_api.asc` |
| B | `examples/01_simd_cpp_api/03_basic_api/00_data_movement/copy_out_tensor_api/copy_out_tensor_api.asc` |
| C | `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/batch_matmul_tensor_api/batch_matmul_tensor_api.asc` |
| D | `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_tensor_api/mmad_tensor_api.asc` |

缺失声明按照以下规则判定：

1. 符号必须被上述4个`.asc`文件直接使用。
2. 修改前，符号在`include/tensor_api`中没有显式声明。
3. 符号即使已经在`impl/tensor_api`中声明或定义，仍然属于需要在
   `include/tensor_api`补充的公开声明。
4. 对于`.with(...)`调用，样例直接调用的公开接口是
   `CopyAtom::with`或`MmadAtom::with`，不是其内部委托的
   `CopyTraits::with`或`MmadTraits::with`。
5. `Routing`、`Impl`、指令封装和格式检查等样例没有直接使用的内部符号，
   不属于本次补充范围。

## 总体结论

4个样例合计直接使用36个Tensor API符号或成员接口：

**表 2**  样例直接使用的Tensor API分类

| 类别 | 数量 | 内容 |
| --- | ---: | --- |
| 命名空间函数 | 9 | `Copy`、`MakeCopy`、`Mmad`、`MakeMmad`、`MakeMemPtr`、`MakeTensor`、`MakeFrameLayout`、`MakeCoord`、`MakeShape` |
| 成员接口 | 3 | `BaseTensor::Slice`、`CopyAtom::with`、`MmadAtom::with` |
| 操作类型 | 8 | 7个Copy Operation和`MmadOperation` |
| Trait | 8 | 7个Copy Trait和`MmadTraitDefault` |
| 布局模式 | 4 | `NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn` |
| 参数和枚举类型 | 4 | `MmadParams`、`FixpipeParams`、`DualDstMode`、`RoundMode` |
| 合计 | 36 | 33个命名空间级符号和3个成员接口 |

修改前，36个直接使用的接口中有14个只在`impl/tensor_api`中定义，
没有在`include/tensor_api`中显式声明。14个缺失项包括11个结构体和
3个成员接口。当前已经全部补充，缺失数由14个降为0。

**表 3**  修改前缺失接口的补充位置

| 缺失类别 | 修改前缺失数量 | 新增或修改的公开文件 |
| --- | ---: | --- |
| 布局模式 | 4 | `include/tensor_api/tensor/layout_pattern.h` |
| Cube Copy Trait | 6 | `include/tensor_api/arch/cube/copy_trait.h` |
| Vector Copy Trait | 1 | `include/tensor_api/arch/vector/copy_trait.h` |
| Tensor成员接口 | 1 | `include/tensor_api/tensor/tensor_base.h` |
| Atom成员接口 | 2 | `include/tensor_api/atom/cube/copy_atom.h`、`include/tensor_api/atom/cube/mmad_atom.h` |
| 合计 | 14 | 4个新增公开头文件和2个修改的公开头文件 |

## 每个`.asc`文件使用的缺失声明

下表中的行号为当前样例代码中的使用位置。一个符号在同一文件中多次使用时，
列出首次使用位置或能够体现该符号的代表位置。

**表 4**  各样例使用的原缺失接口

| 修改前缺失的符号或成员接口 | A：`copy_in_tensor_api.asc` | B：`copy_out_tensor_api.asc` | C：`batch_matmul_tensor_api.asc` | D：`mmad_tensor_api.asc` | 原`impl/tensor_api`定义位置 |
| --- | --- | --- | --- | --- | --- |
| `NDExtLayoutPtn` | 第77、98行 | 第108行 | 第47行 | 第39行 | `impl/tensor_api/tensor/layout_pattern.h` |
| `DNExtLayoutPtn` | 第77行 | 未使用 | 未使用 | 第40行 | `impl/tensor_api/tensor/layout_pattern.h` |
| `NZLayoutPtn` | 第78、162行 | 第124、180行 | 第60、65行 | 第71、90行 | `impl/tensor_api/tensor/layout_pattern.h` |
| `ZNLayoutPtn` | 第78、181行 | 第198行 | 第64行 | 第72、94行 | `impl/tensor_api/tensor/layout_pattern.h` |
| `CopyGM2L1TraitDefault` | 第136行 | 第151行 | 第68行 | 第65行 | `impl/tensor_api/arch/cube/gm_to_l1/copy.h` |
| `CopyL12BTTraitDefault` | 第137行 | 第152行 | 第71行 | 未使用 | `impl/tensor_api/arch/cube/l1_to_bt/copy.h` |
| `CopyL12L0ATraitDefault` | 第138行 | 第153行 | 第69行 | 第66行 | `impl/tensor_api/arch/cube/l1_to_l0a/copy.h` |
| `CopyL12L0BTraitDefault` | 第139行 | 第154行 | 第70行 | 第67行 | `impl/tensor_api/arch/cube/l1_to_l0b/copy.h` |
| `CopyL0C2GMTraitDefault` | 第140行 | 第155行 | 第72行 | 第68行 | `impl/tensor_api/arch/cube/l0c_to_gm/copy.h` |
| `CopyL0C2UBTrait` | 未使用 | 第42、43、45行 | 未使用 | 未使用 | `impl/tensor_api/arch/cube/l0c_to_ub/copy_impl/data_copy.h` |
| `CopyUB2GMTraitDefault` | 未使用 | 第157行 | 未使用 | 未使用 | `impl/tensor_api/arch/vector/ub_to_gm/copy.h` |
| `BaseTensor::Slice` | 第103行 | 第115行 | 第83行 | 第45行 | `impl/tensor_api/tensor/tensor_impl.h` |
| `CopyAtom::with` | 未使用 | 第263行 | 未使用 | 第157行 | `impl/tensor_api/atom/copy_atom_impl.h` |
| `MmadAtom::with` | 第222、226行 | 第239、245行 | 第109行 | 第138行 | `impl/tensor_api/atom/mmad_atom_impl.h` |

### `copy_in_tensor_api.asc`

该文件使用了11个修改前缺少公开声明的符号或成员接口：

- 布局模式：`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`、
  `ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、`CopyL12BTTraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`。
- 成员接口：`BaseTensor::Slice`、`MmadAtom::with`。

### `copy_out_tensor_api.asc`

该文件使用了13个修改前缺少公开声明的符号或成员接口：

- 布局模式：`NDExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、`CopyL12BTTraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`、`CopyL0C2UBTrait`。
- Vector Copy Trait：`CopyUB2GMTraitDefault`。
- 成员接口：`BaseTensor::Slice`、`CopyAtom::with`、`MmadAtom::with`。

其中，`CopyL0C2UBTrait`被样例用于构造自定义Trait常量和
`CopyL0C2UBTraitCustom`。

### `batch_matmul_tensor_api.asc`

该文件使用了10个修改前缺少公开声明的符号或成员接口：

- 布局模式：`NDExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、`CopyL12BTTraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`。
- 成员接口：`BaseTensor::Slice`、`MmadAtom::with`。

### `mmad_tensor_api.asc`

该文件使用了11个修改前缺少公开声明的符号或成员接口：

- 布局模式：`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`、
  `ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`。
- 成员接口：`BaseTensor::Slice`、`CopyAtom::with`、`MmadAtom::with`。

## 添加位置和添加内容

### 布局模式声明

新增文件：

`include/tensor_api/tensor/layout_pattern.h`

新增4个结构体声明：

```cpp
namespace AscendC {
namespace Te {

struct NDExtLayoutPtn;
struct DNExtLayoutPtn;
struct NZLayoutPtn;
struct ZNLayoutPtn;

} // namespace Te
} // namespace AscendC
```

同时在`include/tensor_api/tensor/layout.h`中包含该文件。公开包含链为：

```text
tensor_api/tensor.h
└── tensor_api/tensor/layout.h
    └── tensor_api/tensor/layout_pattern.h
```

### Cube Copy Trait声明

新增文件：

`include/tensor_api/arch/cube/copy_trait.h`

新增6个结构体声明：

```cpp
namespace AscendC {
namespace Te {

struct CopyGM2L1TraitDefault;
struct CopyL12BTTraitDefault;
struct CopyL12L0ATraitDefault;
struct CopyL12L0BTraitDefault;
struct CopyL0C2GMTraitDefault;
struct CopyL0C2UBTrait;

} // namespace Te
} // namespace AscendC
```

同时在`include/tensor_api/arch/cube/copy_op.h`中包含该文件。公开包含链为：

```text
tensor_api/tensor.h
└── tensor_api/arch/arch.h
    └── tensor_api/arch/cube/copy_op.h
        └── tensor_api/arch/cube/copy_trait.h
```

### Vector Copy Trait声明

新增文件：

`include/tensor_api/arch/vector/copy_trait.h`

新增1个结构体声明：

```cpp
namespace AscendC {
namespace Te {

struct CopyUB2GMTraitDefault;

} // namespace Te
} // namespace AscendC
```

同时在`include/tensor_api/arch/vector/copy_op.h`中包含该文件。公开包含链为：

```text
tensor_api/tensor.h
└── tensor_api/arch/arch.h
    └── tensor_api/arch/vector/copy_op.h
        └── tensor_api/arch/vector/copy_trait.h
```

### `BaseTensor::Slice`声明

新增文件：

`include/tensor_api/tensor/tensor_base.h`

该文件新增`BaseTensor<EngineType, LayoutType>`的完整公开类声明，包括类型别名、
构造函数、Tensor/Engine/Layout/Data/Shape/Stride/Size/Capacity访问接口、
索引和切片操作。样例直接使用的两个`Slice`重载声明如下：

```cpp
template <typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info);

template <typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const;
```

`include/tensor_api/tensor/tensor.h`包含`tensor_base.h`，原类内实现拆成类外模板
实现并保留在`impl/tensor_api/tensor/tensor_impl.h`。公开包含链为：

```text
tensor_api/tensor.h
└── tensor_api/tensor/tensor.h
    └── tensor_api/tensor/tensor_base.h
```

### `CopyAtom::with`和`MmadAtom::with`声明

修改以下公开文件：

- `include/tensor_api/atom/cube/copy_atom.h`
- `include/tensor_api/atom/cube/mmad_atom.h`

两个公开文件补充Atom模板特化的类定义，并分别声明`Call`和`with`。
样例直接使用的成员声明如下：

```cpp
// CopyAtom<CopyTraits<Args...>>
template <typename... TraitsArgs>
__aicore__ inline auto with(TraitsArgs&&... args) const;

// MmadAtom<MmadTraits<MmadOperation, Args...>>
template <typename... TraitsArgs>
__aicore__ inline auto with(TraitsArgs&&... args) const;
```

成员实现仍分别位于：

- `impl/tensor_api/atom/copy_atom_impl.h`
- `impl/tensor_api/atom/mmad_atom_impl.h`

`CopyAtom::with`和`MmadAtom::with`内部继续调用Trait层的`with`完成参数合并。
`CopyTraits::with`、`MmadTraits::with`是内部委托接口，不计入样例直接使用的
3个公开成员接口。

## 修改前已经存在的公开声明

36个直接使用的接口中，以下22个在修改前已经由`include/tensor_api`
显式声明，因此没有重复添加：

- 命名空间函数：`Copy`、`MakeCopy`、`Mmad`、`MakeMmad`、
  `MakeMemPtr`、`MakeTensor`、`MakeFrameLayout`、`MakeCoord`、
  `MakeShape`。
- 操作类型：`CopyGM2L1`、`CopyL12BT`、`CopyL12L0A`、
  `CopyL12L0B`、`CopyL0C2GM`、`CopyL0C2UB`、`CopyUB2GM`、
  `MmadOperation`。
- Trait：`MmadTraitDefault`。
- 参数和枚举类型：`MmadParams`、`FixpipeParams`、`DualDstMode`、
  `RoundMode`。

## 验证位置和结果

补充项已经加入Tensor API Header Checker：

- `tests/api/tensor_api/ascendc_tensor_api_header_checker/tensor_api_tensor.cpp`
  检查4个布局模式和`BaseTensor`公开声明。
- `tests/api/tensor_api/ascendc_tensor_api_header_checker/tensor_api_arch.cpp`
  检查7个Copy Trait公开声明。
- `tests/api/tensor_api/ascendc_tensor_api_header_checker/tensor_api_atom.cpp`
  检查`CopyAtom`、`CopyTraits`、`MmadAtom`、`MmadTraits`公开声明。

静态检查确认：

1. 4个新增公开头文件能够独立完成预处理或语法分析。
2. `BaseTensor`的公开类声明与`tensor_impl.h`中的类外模板实现匹配。
3. `CopyAtom`和`MmadAtom`的公开成员声明与对应`impl`实现匹配。
4. `tensor_api/tensor.h`统一入口能够预处理通过。
5. 上述4个`.asc`文件直接使用的14个原缺失项，当前均能在
   `include/tensor_api`中找到显式声明。
