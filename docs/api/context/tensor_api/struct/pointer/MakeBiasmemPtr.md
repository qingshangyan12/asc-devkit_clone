# MakeBiasmemPtr

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|

## 功能说明

创建BIAS上的指针，用于访问AI Core的BIAS的内存空间。

## 函数原型

```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeBiasmemPtr(Iterator iter)

template <typename T, typename U>
__aicore__ inline auto MakeBiasmemPtr(const U& byteOffset)
```

## 参数说明

`表1` 模板参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| T | 输入 | Bias的指针类型，`__bias__ T`。 |
| U | 输入 | 字节偏移量类型。 |

`表2` 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| iter | 输入 | 指针，指向BIAS的起始地址，类型为`__bias__ T` 。 |

## 返回值说明

- 返回BIAS内存指针对象，类型为 `HardwareMemPtr<Hardware::Bias, Iterator>`。


## 约束说明

- iter必须是有效的迭代器类型。
- 偏移地址必须在BIAS内存范围内。

## 调用示例

```cpp
// 示例 1： 使用指针创建
constexpr uint32_t Tile_LENGTH = 128;
__bias__ float data[Tile_LENGTH];
auto ptr = MakeBiasmemPtr(data);

// 示例 2： 使用地址字节偏移创建
uint32_t byteOffset = 256;
uint32_t offset = 128;
auto basePtr = MakeBiasmemPtr<__bias__ float>(byteOffset);
auto offsetPtr = basePtr + offset;
```
