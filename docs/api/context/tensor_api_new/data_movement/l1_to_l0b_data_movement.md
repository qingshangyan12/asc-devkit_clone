# L1到L0B数据搬运

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

头文件为：`#include "tensor_api/tensor.h"`

该接口用于将L1 Buffer中的矩阵数据搬运到L0B Buffer，通常用于Cube矩阵计算前将右矩阵操作数加载到L0B。当前接口通过`CopyAtom`配置搬运功能，调用`Copy`时需要显式传入由`MakeCopy`构造的原子对象。

该通路以512 Byte数据分形为基本搬运单位。不同数据类型在L0B中对应的单分形矩阵形态如下：

| 数据位宽 | L0B单分形矩阵形态 |
| :--- | :--- |
| b4 | 64 * 16 |
| b8 | 32 * 16 |
| b16 | 16 * 16 |
| b32 | 8 * 16 |

接口支持非转置搬运和转置搬运。源张量和目的张量的Layout需要匹配当前通路支持的布局组合。

## 函数原型

- 执行搬运。

    ```cpp
    template <typename AtomType, typename DstTensor, typename SrcTensor>
    __aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src);
    ```

- 构造默认搬运原子对象。

    ```cpp
    template <typename CopyOperationType>
    __aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation);
    ```

- 构造指定Trait的搬运原子对象。

    ```cpp
    template <typename CopyOperationType, typename CopyTraitType>
    __aicore__ inline constexpr auto MakeCopy(
        const CopyOperationType& copyOperation, const CopyTraitType& copyTrait);
    ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| `atomCopy` | 输入 | 搬运原子对象。L1 -> L0B通路可通过`MakeCopy(CopyL12L0B{})`或`MakeCopy(CopyL12L0B{}, CopyL12L0BTraitDefault{})`构造。 |
| `copyOperation` | 输入 | 搬运操作对象。L1 -> L0B通路使用`CopyL12L0B{}`。 |
| `copyTrait` | 输入 | 搬运Trait对象，用于指定搬运特性。L1 -> L0B默认Trait使用`CopyL12L0BTraitDefault{}`。 |
| `dst` | 输出 | 目的张量，存储位置必须为`Location::L0B`。目的地址需要满足L0B Buffer的分形和对齐约束。 |
| `src` | 输入 | 源张量，存储位置必须为`Location::L1`。源地址需要满足L1 Buffer的分形和对齐约束。 |

**表2**`CopyL12L0BTrait`说明

```cpp
struct LoadDataTrait {
    bool transposed = false;
};

using CopyL12L0BTrait = LoadDataTrait;
```

| 字段名 | 类型 | 默认值 | 描述 |
| :--- | :--- | :--- | :--- |
| `transposed` | `bool` | `false` | 表示底层搬运指令是否按转置方式执行。`false`表示非转置搬运，`true`表示转置搬运。接口会根据源张量和目的张量的Layout选择非转置或转置搬运，通常直接使用`CopyL12L0BTraitDefault{}`。 |

## 返回值说明

`Copy`无返回值。`MakeCopy`返回`CopyAtom`对象。

## 数据类型

Ascend 950PR/Ascend 950DT支持的数据类型包括：

`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`half`、`bfloat16_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`float`。

源张量和目的张量的基础数据类型需要保持一致。

## 约束说明

- 该接口仅在AI Core侧执行。
- 目的张量必须位于L0B Buffer，源张量必须位于L1 Buffer。
- 目的地址位于L0B Buffer时，起始地址需要满足512 Byte对齐要求；源地址位于L1 Buffer时，起始地址需要满足32 Byte对齐要求。
- 非转置搬运支持`ZNLayoutPtn -> ZNLayoutPtn`。
- 转置搬运支持`NZLayoutPtn -> ZNLayoutPtn`。
- 启用转置搬运时，不同数据类型需要满足不同约束：b4数据类型要求K方向分形数为4的倍数；b8数据类型要求K方向分形数为2的倍数；b16数据类型无额外倍数要求；b32数据类型要求N方向分形数为2的倍数。
- Tensor Layout采用层次化表述法，Shape、Stride等数值数据支持运行时整数和`Te::_N`等静态整数。

## 关键特性说明

### 非转置搬运

当源张量和目的张量均为`ZNLayoutPtn`时，接口按非转置方式从L1 Buffer搬运到L0B Buffer。

![非转置搬运示意图](../../figures/notranspose.png)

### 转置搬运

当源张量为`NZLayoutPtn`、目的张量为`ZNLayoutPtn`时，接口按转置方式搬运。转置搬运会对每个分形矩阵执行转置，适用于矩阵计算中需要按L0B分形形态组织右矩阵操作数的场景。

不同数据位宽的转置搬运示意如下。

#### b4转置搬运

![b4转置搬运示意图](../../figures/transpose_b4.png)

#### b8转置搬运

![b8转置搬运示意图](../../figures/transpose_b8.png)

#### b16转置搬运

![b16转置搬运示意图](../../figures/transpose_b16.png)

#### b32转置搬运

![b32转置搬运示意图](../../figures/transpose_b32.png)


## 调用示例

```cpp
#include "tensor_api/tensor.h"

__aicore__ inline void CopyL1ToL0BExample()
{
    using namespace AscendC::Te;

    using BType = half;
    constexpr uint32_t k = 16;
    constexpr uint32_t n = 16;

    __cbuf__ BType l1Buf[k * n];
    __cb__ BType l0bBuf[k * n];

    auto l1Tensor = MakeTensor(
        MakeMemPtr(l1Buf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<BType>>(k, n));

    auto l0bTensor = MakeTensor(
        MakeMemPtr(l0bBuf),
        MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<BType>>(k, n));

    auto copyAtom = MakeCopy(CopyL12L0B{}, CopyL12L0BTraitDefault{});
    Copy(copyAtom, l0bTensor, l1Tensor);
}
```

- 复制子张量

  当只需要从L1 Tensor中搬运一个子区域到L0B时，可以先通过`tensor.Slice()`构造源子张量，再将该子张量作为`Copy`的源张量传入。

  `tensor.Slice(startCoord, sliceInfo)`中，`startCoord`表示子张量在原Tensor中的起始逻辑坐标，`sliceInfo`可使用`MakeShape`描述子张量的逻辑尺寸，也可传入Layout描述子张量布局。子张量的Layout、数据类型和地址对齐要求仍需满足L1 -> L0B通路约束。

  ```cpp
  #include "tensor_api/tensor.h"

  __aicore__ inline void CopyL1SubTensorToL0BExample()
  {
      using namespace AscendC::Te;

      using BType = half;
      constexpr uint32_t k = 32;
      constexpr uint32_t n = 32;
      constexpr uint32_t tileK = 16;
      constexpr uint32_t tileN = 16;

      __cbuf__ BType l1Buf[k * n];
      __cb__ BType l0bBuf[tileK * tileN];

      auto l1Tensor = MakeTensor(
          MakeMemPtr(l1Buf),
          MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<BType>>(k, n));

      auto l0bTensor = MakeTensor(
          MakeMemPtr(l0bBuf),
          MakeFrameLayout<ZNLayoutPtn, LayoutTraitDefault<BType>>(tileK, tileN));

      auto l1SubTensor = l1Tensor.Slice(MakeCoord(16, 0), MakeShape(tileK, tileN));
      auto copyAtom = MakeCopy(CopyL12L0B{}, CopyL12L0BTraitDefault{});
      Copy(copyAtom, l0bTensor, l1SubTensor);
  }
  ```
