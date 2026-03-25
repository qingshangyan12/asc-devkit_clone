# MakeTensor

## 功能概述
创建 LocalTensor 对象，用于描述和操作 AI Core 中的数据。LocalTensor 结合[ViewEngine]()（数据指针）和 [Layout]()（布局信息）信息，提供了高效的张量操作接口。

## 函数原型
```cpp
template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iter, const Args&... args)
```

## 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| iter | 输入 | 迭代器，要求传入ViewEngine对象，用于创建LocalTensor。 |
| args | 输入 | 可变参数，当前支持传入一个或两个参数。<br>&bull; 当传入一个入参时：根据传入的 Layout 对象创建LocalTenosr。<br>&bull; 当传入两个入参时：根据传入的参数构建Layout 对象，并基于该 Layout 创建 LocalTensor。 |

## 返回值说明
- 返回 `LocalTensor<TensorAttribute<Engine, Layout>>` 类型的张量对象

## 约束说明
- `iter` 必须是迭代器类型。
- `args...` 当前仅支持传入一个或者两个参数。

## 调用示例
```cpp
// 示例 1: 使用指针和Layout创建张量
constexpr int tileNum = 4;
__cbuf__ half dataPtr[tileNum]; // 初始化
auto ptr = MakeL1memPtr<half>(dataPtr);
auto layout = MakeNzLayout<half>(32, 32);
auto tensor = MakeTensor(ptr, layout);

// 示例 2: 使用指针和形状创建张量（自动计算步幅）
auto tensor2 = MakeTensor(ptr, MakeShape(32, 32), MakeStride(32, 32));
```