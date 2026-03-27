# Copy

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

执行数据复制操作，通过CopyAtom选择不同存储空间之间的数据搬运。

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
| atomCopy | 输入 | 复制原子操作对象。 |
| params | 输入 | 复制操作的参数，可变参数。 |

## 返回值说明

无。

## 约束说明

- atomCopy必须是有效的CopyAtom对象。
- params的数量和类型必须与复制操作的要求匹配。
- 源操作数和目的操作数的内存空间必须支持复制操作。

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
