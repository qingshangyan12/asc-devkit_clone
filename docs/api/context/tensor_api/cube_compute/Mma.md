# Mma

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

执行矩阵乘加（C += A \* B 或 C = A \* B + Bias）操作。支持 `MmadOperation` 操作，用于执行基本计算。

## 函数原型

```cpp
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Mma(const MmaAtom<T>& atomMma, const Params& ...params)

template <typename T, typename... Params>
__aicore__ inline void Mma(const MmaAtom<T>& atomMma, const Params& ...params)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| atomMma | 输入 | MmaAtom 对象，封装了具体的矩阵乘加操作和 traits 配置 |
| params | 输入 | 矩阵乘加操作的参数，可变参数。通常包括：<br>- 目的张量 C（L0C）<br>- 源张量 B（L0B，右矩阵）<br>- 源张量 A（L0A，左矩阵）<br>- 参数（如 defaultMmadParams）<br>- 可选：偏置张量（Bias） |

## 返回值说明

无

## 约束说明

- atomMma必须是有效的MmaAtom对象。
- params的数量和类型必须与矩阵乘加操作的要求匹配。
- 源操作数和目的操作数必须位于支持的存储空间。

## 调用示例

```cpp
// 下面接口为3种调用方式的示例
// 调用方式1
Mma(MmaAtom<MmaTraits<MmadOperation, MmadTraitDefault>>{}, L0CTensor, L0ATensor, L0BTensor, para);
// 调用方式2
MmaAtom<MmaTraits<MmadOperation, MmadTraitDefault>>{}.Call(L0CTensor, L0ATensor, L0BTensor, para);
// 调用方式3
auto atomMma = MakeMma(MmadOperation{}, MmadTraitDefault{});
atomMma.Call(L0CTensor, L0ATensor, L0BTensor, para);

// 带Bias的调用方式
Mma(MmaAtom<MmaTraits<MmadOperation, MmadTraitDefault>>{}, L0CTensor, L0ATensor, L0BTensor, biasTensor, para);

```

## 相关参考

- [MmaAtom](../struct/atom/MmaAtom.md)
- [Mmad](./Mmad.md)