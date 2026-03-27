# Mad

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

通过MmadAtom完成矩阵乘加（C += A \* B 或 C = A \* B + Bias）操作。

## 函数原型

```cpp
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Mad(const MmadAtom<T>& atomMad, const Params& ...params)

template <typename T, typename... Params>
__aicore__ inline void Mad(const MmadAtom<T>& atomMad, const Params& ...params)
```

## API映射关系
与built-in接口映射关系：
Mad接口是在built-in接口(mad)的基础上进行抽象封装实现的，其对应的底层built-in接口为：
```cpp

```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| atomMad | 输入 | 矩阵乘加原子操作对象。 |
| params | 输入 | 矩阵乘加操作的参数，可变参数。 |

## 返回值说明

无。

## 约束说明

- atomMad必须是有效的MmadAtom对象。
- params的数量和类型必须与矩阵乘加操作的要求匹配。
- 源操作数和目的操作数必须位于支持的存储空间。

## 调用示例

```cpp
// 下面接口为3种调用方式的示例
// 调用方式1
Mad(MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}, L0CTensor, L0ATensor, L0BTensor, para);
// 调用方式2
MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}.Call(L0CTensor, L0ATensor, L0BTensor, para);
// 调用方式3
auto atomMad = MakeMad(MmadOperation{}, MmadTraitDefault{});
atomMad.Call(L0CTensor, L0ATensor, L0BTensor, para);

// 带Bias的调用方式
Mad(MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}, L0CTensor, L0ATensor, L0BTensor, biasTensor, para);

```
