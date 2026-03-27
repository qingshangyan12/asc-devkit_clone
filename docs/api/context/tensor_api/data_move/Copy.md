# Copy

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

执行数据复制操作，通过CopyAtom选择不同存储空间之间的数据搬运。Copy API 是 Tensor API 中的统一数据搬运接口，支持在不同存储空间之间进行数据传输。

**表1** 支持的数据搬运通路

| 操作类型 | 说明 |
|---------|------|
| CopyGM2L1 | GM 到 L1 |
| CopyL12L0A | L1 到 L0A |
| CopyL12L0B | L1 到 L0B|
| CopyL12BT |  L1 到 Bias Table Buff|
| CopyL12FB |  L1 到 Fixpipe Buff|
| CopyL0C2GM | L0C矩阵结果搬出到 GM |
| CopyL0C2UB | L0C矩阵结果搬出到 UB |


## 函数原型

```cpp
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)

template <typename T, typename... Params>
__aicore__ inline void Copy(const CopyAtom<T>& atomCopy, const Params& ...params)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| atomCopy | 输入 | CopyAtom 对象，封装了具体的复制操作和 traits 配置 |
| params | 输入 | 复制操作的参数，可变参数。通常包括：<br>- 目的张量（dst tensor）<br>- 源张量（src tensor）<br>- 可选：坐标偏移（Coord）<br>- 可选：操作参数（如 FixpipeParams） |

## 返回值说明

无

## 约束说明

- atomCopy 必须是有效的 CopyAtom 对象。w
- params 的数量和类型必须与复制操作的要求匹配。
- 源操作数和目的操作数的内存空间必须支持对应的复制操作。

## 调用示例

```cpp
// 以CopyL0C2GM为例
// 调用方式1
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor);
// 调用方式2
CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}.Call(globalC, l0CTensor);
// 调用方式3
auto atomCopyL0C2GM = MakeCopy(CopyL0C2GM{}, FixpipeTraitDefault{});
atomCopyL0C2GM.Call(globalC, l0CTensor);
```

## 相关参考

- [CopyAtom](../struct/atom/CopyAtom.md)
- [CopyTraits](../struct/atom/CopyTraits.md)
- [DataCopy](.DataCopy.md)
- [LoadData](.LoadData.md)
- [Fixpipe](.Fixpipe.md)