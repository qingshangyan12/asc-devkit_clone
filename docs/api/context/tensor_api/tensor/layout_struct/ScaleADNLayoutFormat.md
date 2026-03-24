# ScaleADNLayoutFormat

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

ScaleADNLayoutFormat用于定义ScaleADN格式的布局，ScaleADN格式是一种支持缩放的布局。

## 结构体定义

```cpp
template <typename T>
struct ScaleADNLayoutFormat {
    template <size_t row, size_t column>
    using type = ScaleADNFormatLayout<T, row, column>;

    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeScaleADNLayout<T, U, S>(row, column);
    }  
};
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `T` | 输入 | 数据类型模板参数。<br>AscendC 950PR/AscendC 950DT 支持的数据类型为：fp8_e8m0_t。 |
| `row` | 输入 | 矩阵的总行数 |
| `column` | 输入 | 矩阵的总列数 |

## 返回值

- 输入为编译时常量时，返回DN格式的Layout类型。
- 输入为整型变量时，返回DN格式的Layout对象。
- ScaleADN格式的小分形固定为，行数固定为1，列数为2。

## 约束说明

- 参数row和column需为整型。
- 参数column需满足2的倍数。

## 调用示例

```cpp
// 创建ScaleADN格式Layout
// 编译时常量
using MyScaleADNLayout = ScaleADNLayoutFormat<half>::type<Std::Int<32>, Std::Int<64>>;
constexpr MyScaleADNLayout staticLayout1;
auto staticLayout2 = MyScaleADNLayout{};

// 运行时值
ScaleADNLayoutFormat<half> scaleADNFormat;
auto layout = scaleADNFormat(32, 64);
```