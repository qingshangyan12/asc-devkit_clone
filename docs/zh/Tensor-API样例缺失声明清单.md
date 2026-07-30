# Tensor API 样例缺失声明清单

## 1. 核查范围和判定规则

本次核查以下目录中的全部 `.asc` 文件：

- `examples/01_simd_cpp_api/03_basic_api/00_data_movement`
- `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute`

两个目录下共有 35 个 `.asc` 文件，其中直接使用 `AscendC::Te` Tensor API
的文件有 4 个：

| 编号 | `.asc` 文件 |
| --- | --- |
| A | `examples/01_simd_cpp_api/03_basic_api/00_data_movement/copy_in_tensor_api/copy_in_tensor_api.asc` |
| B | `examples/01_simd_cpp_api/03_basic_api/00_data_movement/copy_out_tensor_api/copy_out_tensor_api.asc` |
| C | `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/batch_matmul_tensor_api/batch_matmul_tensor_api.asc` |
| D | `examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_tensor_api/mmad_tensor_api.asc` |

缺失声明按照以下规则判定：

1. 符号必须被上述 4 个 `.asc` 文件直接使用。
2. 修改前，符号在 `include/tensor_api` 中没有显式声明。
3. 符号即使已经在 `impl/tensor_api` 中声明或定义，仍然属于需要在
   `include/tensor_api` 补充的公开声明。
4. `Routing`、`Impl`、指令封装和格式检查等样例没有直接使用的内部符号，
   不属于本次补充范围。

## 2. 总体结论

4 个样例直接使用的 Tensor API 函数均已在 `include/tensor_api` 中声明，
本次没有新增函数声明。

修改前缺失的是 11 个结构体声明，包括 4 个布局模式、6 个 Cube Copy Trait
和 1 个 Vector Copy Trait。当前已经全部补充，缺失数由 11 个降为 0。

| 类别 | 缺失数量 | 新增声明文件 |
| --- | ---: | --- |
| 布局模式 | 4 | `include/tensor_api/tensor/layout_pattern.h` |
| Cube Copy Trait | 6 | `include/tensor_api/arch/cube/copy_trait.h` |
| Vector Copy Trait | 1 | `include/tensor_api/arch/vector/copy_trait.h` |
| 合计 | 11 | 3 个新增头文件 |

## 3. 每个 `.asc` 文件使用的缺失声明

下表中的行号为当前代码中的使用位置。一个符号在同一文件中多次使用时，
列出首次使用位置或能够同时体现一组符号的代表位置。

| 修改前缺失的符号 | A：`copy_in_tensor_api.asc` | B：`copy_out_tensor_api.asc` | C：`batch_matmul_tensor_api.asc` | D：`mmad_tensor_api.asc` | `impl/tensor_api` 定义位置 |
| --- | --- | --- | --- | --- | --- |
| `NDExtLayoutPtn` | 第 77、98 行 | 第 108 行 | 第 47 行 | 第 39 行 | `impl/tensor_api/tensor/layout_pattern.h:36` |
| `DNExtLayoutPtn` | 第 77 行 | 未使用 | 未使用 | 第 40 行 | `impl/tensor_api/tensor/layout_pattern.h:37` |
| `NZLayoutPtn` | 第 78、162 行 | 第 124、180 行 | 第 60、65 行 | 第 71、90 行 | `impl/tensor_api/tensor/layout_pattern.h:33` |
| `ZNLayoutPtn` | 第 78、181 行 | 第 198 行 | 第 64 行 | 第 72、94 行 | `impl/tensor_api/tensor/layout_pattern.h:30` |
| `CopyGM2L1TraitDefault` | 第 136 行 | 第 151 行 | 第 68 行 | 第 65 行 | `impl/tensor_api/arch/cube/gm_to_l1/copy.h:32` |
| `CopyL12BTTraitDefault` | 第 137 行 | 第 152 行 | 第 71 行 | 未使用 | `impl/tensor_api/arch/cube/l1_to_bt/copy.h:32` |
| `CopyL12L0ATraitDefault` | 第 138 行 | 第 153 行 | 第 69 行 | 第 66 行 | `impl/tensor_api/arch/cube/l1_to_l0a/copy.h:32` |
| `CopyL12L0BTraitDefault` | 第 139 行 | 第 154 行 | 第 70 行 | 第 67 行 | `impl/tensor_api/arch/cube/l1_to_l0b/copy.h:32` |
| `CopyL0C2GMTraitDefault` | 第 140 行 | 第 155 行 | 第 72 行 | 第 68 行 | `impl/tensor_api/arch/cube/l0c_to_gm/copy.h:34` |
| `CopyL0C2UBTrait` | 未使用 | 第 42、43、45 行 | 未使用 | 未使用 | `impl/tensor_api/arch/cube/l0c_to_ub/copy_impl/data_copy.h:33` |
| `CopyUB2GMTraitDefault` | 未使用 | 第 157 行 | 未使用 | 未使用 | `impl/tensor_api/arch/vector/ub_to_gm/copy.h:33` |

### 3.1 `copy_in_tensor_api.asc`

该文件使用了 9 个修改前缺少公开声明的结构体：

- 布局模式：`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`、
  `ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、`CopyL12BTTraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`。

### 3.2 `copy_out_tensor_api.asc`

该文件使用了 10 个修改前缺少公开声明的结构体：

- 布局模式：`NDExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、`CopyL12BTTraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`、`CopyL0C2UBTrait`。
- Vector Copy Trait：`CopyUB2GMTraitDefault`。

其中，`CopyL0C2UBTrait` 被样例用于构造自定义 Trait 常量和
`CopyL0C2UBTraitCustom`。

### 3.3 `batch_matmul_tensor_api.asc`

该文件使用了 8 个修改前缺少公开声明的结构体：

- 布局模式：`NDExtLayoutPtn`、`NZLayoutPtn`、`ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、`CopyL12BTTraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`。

### 3.4 `mmad_tensor_api.asc`

该文件使用了 8 个修改前缺少公开声明的结构体：

- 布局模式：`NDExtLayoutPtn`、`DNExtLayoutPtn`、`NZLayoutPtn`、
  `ZNLayoutPtn`。
- Cube Copy Trait：`CopyGM2L1TraitDefault`、
  `CopyL12L0ATraitDefault`、`CopyL12L0BTraitDefault`、
  `CopyL0C2GMTraitDefault`。

## 4. 添加位置和添加内容

### 4.1 布局模式声明

新增文件：

`include/tensor_api/tensor/layout_pattern.h`

新增内容：

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

为使布局模式声明能够通过原有布局公开头文件访问，在
`include/tensor_api/tensor/layout.h` 中新增：

```cpp
#include "tensor_api/tensor/layout_pattern.h"
```

最终包含链为：

```text
tensor_api/tensor.h
└── tensor_api/tensor/layout.h
    └── tensor_api/tensor/layout_pattern.h
```

### 4.2 Cube Copy Trait 声明

新增文件：

`include/tensor_api/arch/cube/copy_trait.h`

新增内容：

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

为使 Cube Copy Trait 声明能够通过原有 Cube Copy 公开头文件访问，在
`include/tensor_api/arch/cube/copy_op.h` 中新增：

```cpp
#include "tensor_api/arch/cube/copy_trait.h"
```

最终包含链为：

```text
tensor_api/tensor.h
└── tensor_api/arch/arch.h
    └── tensor_api/arch/cube/copy_op.h
        └── tensor_api/arch/cube/copy_trait.h
```

### 4.3 Vector Copy Trait 声明

新增文件：

`include/tensor_api/arch/vector/copy_trait.h`

新增内容：

```cpp
namespace AscendC {
namespace Te {

struct CopyUB2GMTraitDefault;

} // namespace Te
} // namespace AscendC
```

为使 Vector Copy Trait 声明能够通过原有 Vector Copy 公开头文件访问，在
`include/tensor_api/arch/vector/copy_op.h` 中新增：

```cpp
#include "tensor_api/arch/vector/copy_trait.h"
```

最终包含链为：

```text
tensor_api/tensor.h
└── tensor_api/arch/arch.h
    └── tensor_api/arch/vector/copy_op.h
        └── tensor_api/arch/vector/copy_trait.h
```

## 5. 本次未新增的声明

样例使用的下列函数和操作类型在修改前已经由 `include/tensor_api`
显式声明，因此不属于缺失项，也没有重复添加：

- 函数：`Copy`、`MakeCopy`、`Mmad`、`MakeMmad`、`MakeMemPtr`、
  `MakeTensor`、`MakeFrameLayout`、`MakeCoord`、`MakeShape`。
- 操作类型：`CopyGM2L1`、`CopyL12BT`、`CopyL12L0A`、
  `CopyL12L0B`、`CopyL0C2GM`、`CopyL0C2UB`、`CopyUB2GM`、
  `MmadOperation`。
- 其他类型：`MmadTraitDefault`、`DualDstMode`、`RoundMode`、
  `MmadParams`。

## 6. 验证位置

新增声明已经加入 Tensor API Header Checker：

- `tests/api/tensor_api/ascendc_tensor_api_header_checker/tensor_api_tensor.cpp`
  检查 4 个布局模式声明。
- `tests/api/tensor_api/ascendc_tensor_api_header_checker/tensor_api_arch.cpp`
  检查 7 个 Copy Trait 声明。

补充完成后，上述 4 个 `.asc` 文件使用的 11 个原缺失结构体均能在
`include/tensor_api` 中找到显式声明，并能够通过
`tensor_api/tensor.h` 的统一入口访问。
