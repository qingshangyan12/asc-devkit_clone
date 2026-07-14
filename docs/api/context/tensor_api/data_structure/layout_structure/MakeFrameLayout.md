# MakeFrameLayout

## 产品支持情况

| 产品 | 是否支持 |
| :--- | :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

需要包含的头文件为：#include "tensor_api/tensor.h"。

MakeFrameLayout用于根据标准分形排布模式构造带布局特征信息的Layout对象。该接口可接收矩阵的行数、列数，或Batch、行数、列数，结合指定的LayoutPattern（排布模式）和LayoutTrait（布局特征参数）计算Shape和Stride，构造二维或Batch模式的Layout对象。

LayoutPattern决定数据在内存中的分形排列方式，不同存储位置间的搬运和计算操作要求源/目的张量满足特定的LayoutPattern组合。

## 函数原型

```cpp
template <typename LayoutPattern, typename TraitType, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args)
```

### TraitType的指定方式

TraitType用于确定Layout中的C0维度基数。C0表示分形布局中内部连续存放的一组元素个数。MakeFrameLayout支持以下传递方式：

1. **不传入TraitType**：内部根据LayoutPattern使用默认C0。NDLayoutPtn、DNLayoutPtn、DNExtLayoutPtn、NDExtLayoutPtn的默认C0为1；ScaleANDLayoutPtn、ScaleADNLayoutPtn、ScaleBNDLayoutPtn、ScaleBDNLayoutPtn的默认C0为2；NZLayoutPtn、ZZLayoutPtn、ZNLayoutPtn默认按uint16_t计算C0。
   ```cpp
   MakeFrameLayout<NDLayoutPtn>(m, n)
   ```

2. **数据类型T**：使用数据类型T计算C0，计算方式为C0 = 32 / sizeof(T)。数据类型T只用于计算C0，不作为Tensor的数据类型来源。
   ```cpp
   MakeFrameLayout<NZLayoutPtn, float>(m, n)
   ```

3. **LayoutTraitDefault<T>**：使用数据类型T计算C0，计算方式为C0 = 32 / sizeof(T)。数据类型T只用于计算C0，不作为Tensor的数据类型来源。
   ```cpp
   MakeFrameLayout<NZLayoutPtn, LayoutTraitDefault<float>>(m, n)
   ```

4. **_C0或Std::Int<C0>**：直接指定C0维度的元素个数，数据类型为[Int](../../../Utils-API/通用工具/Int.md)。_C0是Std::Int<C0>的简写形式。
   ```cpp
   MakeFrameLayout<NZLayoutPtn, _16>(m, n)
   MakeFrameLayout<NZLayoutPtn, Std::Int<16>>(m, n)
   ```

5. **C0元素个数**：通过非类型模板参数直接传入C0元素个数。
   ```cpp
   MakeFrameLayout<NZLayoutPtn, 16>(m, n)
   ```

6. **LayoutTrait<T, _C0>**：传入已经构造好的LayoutTrait类型。第一个模板参数T保留用于兼容已有代码，不作为Tensor的数据类型来源；第二个模板参数_C0指定C0元素个数。
   ```cpp
   MakeFrameLayout<NZLayoutPtn, LayoutTrait<float, _16>>(m, n)
   ```

7. **自定义Trait传入**：传入一个自定义Trait类型。该类型需要提供C0_ELEMENT成员，用于表示C0元素个数。当前MakeFrameLayout只读取C0_ELEMENT。
   ```cpp
   struct MyLayoutTrait {
       static constexpr auto C0_ELEMENT = Std::Int<16>{};
   };

   MakeFrameLayout<NZLayoutPtn, MyLayoutTrait>(m, n)
   ```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| LayoutPattern | 输入 | 布局模式模板参数。支持的LayoutPattern及数据排布格式见[Layout和层次化表述法](Layout和层次化表述法.md)。 |
| TraitType | 输入 | 布局特征类型，默认内部根据LayoutPattern自动推导。支持上述传递方式。 |
| Args | 输入 | 构造参数。常见为矩阵的行数m和列数n，或Batch、行数m和列数n。各LayoutPattern的构造参数数量可能不同。 |

## 返回值说明

返回Layout<ShapeType, StrideType, Std::tuple<LayoutPattern, ConvertedTraitType>>类型的对象，其中ShapeType和StrideType由LayoutPattern和构造参数在编译期推导，ConvertedTraitType为接口根据传入方式转换后的布局特征类型。

## 约束说明

- LayoutPattern必须是已支持的排布模式之一，不支持的模式会触发编译期报错。
- 构造参数的个数和含义由LayoutPattern决定。常见二维场景传入(m, n)，Batch场景传入(batch, m, n)。
- TraitType必须为整型常量、包含C0_ELEMENT成员的布局特征类型或具体数据类型，不支持的类型会触发编译期报错。

## 调用示例

```cpp
using namespace AscendC::Te;

// 构造GM端ND布局张量
auto layoutGM = MakeFrameLayout<NDExtLayoutPtn, LayoutTraitDefault<float>>(m, n);
auto gmTensor = MakeTensor(MakeMemPtr<Location::GM>(gmAddr), layoutGM);

// 构造Batch模式GM端ND布局张量
auto layoutBatchGM = MakeFrameLayout<NDExtLayoutPtn>(batch, m, n);
auto gmBatchTensor = MakeTensor(MakeMemPtr<Location::GM>(gmBatchAddr), layoutBatchGM);

// 构造L1端NZ布局张量（L0A/L0B搬运的标准格式）
auto layoutL1 = MakeFrameLayout<NZLayoutPtn, float>(m, n);
auto l1Tensor = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutL1);

// 不传TraitType构造NZ布局
auto layoutNZ = MakeFrameLayout<NZLayoutPtn>(m, n);
auto l1TensorNZ = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutNZ);

// 指定C0元素数构造
auto layoutC0 = MakeFrameLayout<NZLayoutPtn, _16>(m, n);
auto l1TensorC0 = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutC0);

// 使用非类型模板参数指定C0元素数构造
auto layoutC0Value = MakeFrameLayout<NZLayoutPtn, 16>(m, n);
auto l1TensorC0Value = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutC0Value);

// 使用LayoutTrait指定C0元素数构造
auto layoutTrait = MakeFrameLayout<NZLayoutPtn, LayoutTrait<float, _16>>(m, n);
auto l1TensorTrait = MakeTensor(MakeMemPtr<Location::L1>(l1Addr), layoutTrait);

// 自定义Trait构造
struct MyLayoutTrait {
    static constexpr auto C0_ELEMENT = Std::Int<16>{};
};
auto layoutCustom = MakeFrameLayout<NZLayoutPtn, MyLayoutTrait>(m, n);
```
