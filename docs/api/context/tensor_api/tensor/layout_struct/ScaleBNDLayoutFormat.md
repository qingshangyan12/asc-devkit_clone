# ScaleBNDLayoutFormat

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

ScaleBNDLayoutFormat用于定义ScaleBND格式的布局，ScaleBND格式是一种支持缩放的布局。

## 结构体定义

```cpp
template <typename T>
struct ScaleBNDLayoutFormat {
    template <size_t row, size_t column>
    using type = ScaleBNDFormatLayout<T, row, column>;

    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeScaleBNDLayout<T, U, S>(row, column);
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
- ScaleBND格式的小分形固定为，行数固定为2，列数为1。

## 约束说明

- 参数row和column需为整型。
- 参数row需满足2的倍数。

## 调用示例

```cpp
// 创建ScaleBND格式Layout
// 编译时常量
using MyScaleBNDLayout = ScaleBNDLayoutFormat<half>::type<Std::Int<64>, Std::Int<32>>;
constexpr MyScaleBNDLayout staticLayout1;
auto staticLayout2 = MyScaleBNDLayout{};

// 运行时值
ScaleBNDLayoutFormat<half> scaleBNDFormat;
auto layout = scaleBNDFormat(64, 32);
```