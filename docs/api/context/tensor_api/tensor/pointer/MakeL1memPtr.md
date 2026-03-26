# MakeL1memPtr

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|x|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|

## 功能说明

创建 L1 上的指针，用于访问 AI Core 的 L1 的内存空间。

## 函数原型

```cpp
template <typename Iterator>
__aicore__ inline constexpr auto MakeL1memPtr(Iterator iter)

template <typename T, typename U>
__aicore__ inline auto MakeL1memPtr(const U& byteOffset)
```

## 参数说明

`表1` 模板参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| T | 输入 | L1 的指针的类型。 |
| U | 输入 | 字节偏移量类型。 |

`表2` 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| iter | 输入 | 迭代器或指针，指向GM的起始地址。 |


## 返回值说明

- 返回 L1 内存指针对象，类型为 `HardwareMemPtr<Hardware::L1, Iterator>`


## 约束说明

- `iter` 必须是有效的迭代器类型
- 偏移地址必须在 L1 内存范围内

## 调用示例

```cpp
// 示例 1： 使用指针创建
constexpr uint32_t TILE_LENGTH = 4;
__cbuf__ float data[TILE_LENGTH] = {1, 2, 3, 4};
auto ptr = MakeL1memPtr(data);

// 示例 2： 使用地址字节偏移创建
uint32_t byteOffset = 128;
auto ptr = MakeL1memPtr<float>(byteOffset);

```