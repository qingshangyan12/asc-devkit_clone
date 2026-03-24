# L0CLayoutFormat

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

L0CLayoutFormat用于描述矩阵计算的结果在物理位置为L0C Buffer的布局格式，L0CLayout是Nz分形的一种，Shape的大小固定为16x16。

## 结构体定义

```cpp
template <typename T>
struct L0CLayoutFormat {
    template <size_t row, size_t column>
    using type = L0CFormatLayout<row, column>;

    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeL0CLayout<U, S>(row, column);
    }  
};
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `T`` | 输入 | 数据类型模板参数。<br>AscendC 950PR/AscendC 950DT 支持的数据类型为：int32_t、float。 |
| `row` | 输入 | 矩阵的总行数 |
| `column` | 输入 | 矩阵的总列数 |

## 返回值

- 输入为编译时常量时，返回DN格式的Layout类型。
- 输入为整型变量时，返回DN格式的Layout对象。
- L0CLayout的小分形的行数为16，列数为16。
- **自动对齐**: 如果输入的行数不是 16 的倍数，列数不是16的倍数，返回的Layout会向上取整。例如：
  - 输入 `row = 17, column = 33` 会自动对齐为 `row = 32, column = 48`

## 约束说明

- 参数row和column需为整型。

## 调用示例

```cpp
// 编译时常量
using MyL0CLayout = L0CLayoutFormat<uint16_t>::type<Std::Int<32>, Std::Int<32>>;
auto staticL0CLayout = MyL0CLayout{};

// 运行时值
L0CLayoutFormat<uint16_t> l0cFormat;
auto layout = l0cFormat(32, 32);
```