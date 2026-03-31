# MakeL0CmemPtr

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

创建L0C上的指针，用于访问AI Core的L0C的内存空间。

## 函数原型

```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeL0CmemPtr(Iterator iter)

template <typename T, typename U>
__aicore__ inline auto MakeL0CmemPtr(const U& byteOffset)
```

## 参数说明

`表1` 模板参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| T | 输入 | L0C的指针类型，`__cc__ T`。 |
| U | 输入 | 字节偏移量类型。 |

`表2` 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| iter | 输入 | 指针，指向L0C的起始地址，类型为`__cc__ T`。 |


## 返回值说明

- 返回L0C内存指针对象，类型为 `HardwareMemPtr<Hardware::L0C, Iterator>`。


## 约束说明

- iter必须是有效的迭代器类型。
- 偏移地址必须在L0C内存范围内。

## 调用示例

```cpp
// 示例 1： 使用指针创建
constexpr uint32_t L1_SIZE = 128;
__cc__ float data[L1_SIZE];
auto ptr = MakeL0CmemPtr(data);

// 示例 2： 使用地址字节偏移创建
uint32_t byteOffset = 128;
auto ptr = MakeL0CmemPtr<float>(byteOffset);
```
